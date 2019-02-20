using System;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using Microsoft.Samples.EventLog.API;

namespace Microsoft.Samples.EventLog
{
    //
    // The SubscribeEvents class demonstrates some of the subscription capabilities of the new Windows
    // Event Log.  This sample allows the user to subscribe to a given channel with an optional query
    // filter and displays the events written to the log inside a callback method.
    //
    public class SubscribeEvents
    {
        static void PrintUsage()
        {
            Console.WriteLine(
                "\nUsage:\n\n" + 
                "   SubscribeEvents PATH [/OPTION:VALUE [/OPTION:VALUE] ...]\n\n" +
                "   PATH is the name of the log to subscribe.\n" +
                "\nOptions:\n\n" +
                "   /query(q):VALUE                 Filters the events to subscribe with the XPath expression specified by VALUE.  Default is to read all events.\n" +
                "   /structuredquery(sq):VALUE      Filters the events to subscribe with the structured query from the xml file specified by VALUE.  Cannot be used with /query.\n"
                );
        }

        public static void Main(string[] args)
        {
            int exitCode = 0;

            IntPtr session = IntPtr.Zero;
            string channelpath;
            string query = null;
            EventSubscribeCallback callback = new EventSubscribeCallback( DisplayEventCallback );

            try 
            {
                // parse the command line

                if(args.Length == 0) 
                {
                    Console.WriteLine("Error: No parameters provided.");
                    PrintUsage();
                    Environment.Exit(1);
                }

                if (args[0] == "/?")
                {
                    PrintUsage();
                    Environment.Exit(1);
                }

                channelpath = args[0];

                char[] delimiters = {':'};

                for ( int i=1 ; i < args.Length; i++ )
                {
                    String option = args[i].Substring( 1 );
                    String[] words = option.Split( delimiters, 2 );
                    words[0] = words[0].ToLower();

                    switch( words[0] )
                    {

                        case "query":
                        case "q":
                            if ( query != null )
                            {
                                Console.WriteLine( "Options '/query' and '/structuredquery' cannot both be specified." );
                                PrintUsage();
                                Environment.Exit(1);
                            }
                            query = words[1];
                            break;

                        case "structuredquery":
                        case "sq":
                            if ( query != null )
                            {
                                Console.WriteLine( "Options '/query' and '/structuredquery' cannot both be specified." );
                                PrintUsage();
                                Environment.Exit(1);
                            }

                            using (StreamReader sr = new StreamReader( words[1] )) 
                            {
                                String line;
                                while ((line = sr.ReadLine()) != null) 
                                {
                                    query += line;;
                                }
                            }
                            break;

                        default:
                            throw (new Exception( String.Format("Unrecognized parameter option: {0}.", words[0]) ));

                    }
                }


                // subscribe to the event log

                IntPtr subscriptionPtr = Events.Subscribe( 
                    session, 
                    IntPtr.Zero,        // SignalEvent (for polling)
                    channelpath, 
                    query, 
                    IntPtr.Zero,        // Bookmark
                    IntPtr.Zero,        // Context
                    callback,
                    SubscribeFlags.FutureEvents );
                if (subscriptionPtr == IntPtr.Zero)
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("Subscribe failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }

                using ( SafeEventHandle subscription = new SafeEventHandle( subscriptionPtr ) )
                {
                    // continue listening until user stops subscription

                    Console.WriteLine( "\nPress <enter> to stop subscription.\n" );
                    Console.Read();
                }

            }
            catch(Exception e) 
            {
                Console.WriteLine(e.ToString());
                exitCode = 1;
            }

            Environment.Exit(exitCode);
        }


        //
        // DisplayEventCallback:  subscription callback which displays the returned event
        //

        static int DisplayEventCallback( SubscribeNotifyAction action, IntPtr context, IntPtr eventPtr )
        {
            if ( SubscribeNotifyAction.Error == action )
            {
                Console.WriteLine( "***** Subscription encountered an error! *****" );
            }
            else if ( SubscribeNotifyAction.Deliver == action )
            {
                // display the event

                using ( SafeEventHandle evt = new SafeEventHandle( eventPtr, false ) )
                {
                    Console.WriteLine( "{0}", RenderEventToXml( evt, RenderFlags.RenderEventXml ) );
                }


                // list the Ids of the query clauses that selected the event

                int bufferSize = 0, bufferRequired = 0;
                IntPtr variantPtr = Marshal.AllocHGlobal( bufferRequired );

                bool result = Events.GetEventInfo( eventPtr, EventPropertyId.EventQueryIDs, bufferSize, variantPtr, ref bufferRequired );
                if ( !result && Win32Error.ErrorInsufficientBuffer == Marshal.GetLastWin32Error() )
                {
                    // resize the buffer and try again
                    Marshal.FreeHGlobal( variantPtr );
                    variantPtr = Marshal.AllocHGlobal( bufferRequired );
                    if ( variantPtr == IntPtr.Zero )
                    {
                        Console.WriteLine("Failed to allocate space for event selection data.");
                        return Win32Error.ErrorOutOfMemory;
                    }
                    bufferSize = bufferRequired;
                    result = Events.GetEventInfo( eventPtr, EventPropertyId.EventQueryIDs, bufferSize, variantPtr, ref bufferRequired );
                }
                if ( !result )
                {
                    Marshal.FreeHGlobal( variantPtr );
                    Console.WriteLine( "GetEventInfo failed with error {0}.", Marshal.GetLastWin32Error().ToString() );
                    return 0;
                }

                // the returned variant is an array of UInt32 values which are allocated just beyond the EventVariant base memory
                EventVariant var = (EventVariant) Marshal.PtrToStructure( variantPtr, typeof(EventVariant) );
                if ((EventVariantType.UInt32 | EventVariantType.Array) != (var.type))
                {
                    Console.WriteLine("Expected variant type {0} (UInt32 Array), actual {1}.", (EventVariantType.UInt32 | EventVariantType.Array), var.type);
                    Marshal.FreeHGlobal( variantPtr );
                    return 1;
                }

                IntPtr currentPtr = variantPtr;
                currentPtr = (IntPtr)((int)currentPtr + Marshal.SizeOf( var ));
                String ids = "";
                UInt32 id;

                for ( int n=0; n < var.ItemCount; n++ )
                {
                    id = (UInt32) Marshal.PtrToStructure( currentPtr, typeof(UInt32) );
                    if ( n != 0 )  ids += ", ";
                    ids += Convert.ToString( id );
                    currentPtr = (IntPtr)((int)currentPtr + Marshal.SizeOf( id ));
                }

                Console.WriteLine( "(The following query Ids selected the above event: {0})", ids );
                Marshal.FreeHGlobal( variantPtr );
            }

            return 0;
        }


        //
        // RenderEventToXml:  renders an event to xml or an xml bookmark and returns as a string 
        //

        static string RenderEventToXml( SafeEventHandle evt, RenderFlags flags )
        {
            int bufferSize = 0, bufferUsed = 0, valueCount = 0;
            StringBuilder eventXml = new StringBuilder(bufferSize);

            bool result = Events.Render( IntPtr.Zero, evt.Handle, flags, bufferSize, eventXml, out bufferUsed, out valueCount );
            if ( !result && Win32Error.ErrorInsufficientBuffer == Marshal.GetLastWin32Error() )
            {
                eventXml.Length = bufferUsed;   // resize the buffer and try again
                bufferSize = bufferUsed;
                result = Events.Render( IntPtr.Zero, evt.Handle, flags, bufferSize, eventXml, out bufferUsed, out valueCount );
            }
            if ( !result )
            {
                Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                throw (new Exception(String.Format("Render failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
            }

            return eventXml.ToString();
        }
    
    }

}
