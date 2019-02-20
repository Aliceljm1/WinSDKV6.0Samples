using System;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using Microsoft.Samples.EventLog.API;

namespace Microsoft.Samples.EventLog.Querying
{
    //
    // The ReadEvents class demonstrates some of the querying and rendering capabilities 
    // of the new Windows Event Log.  This sample queries a log or log file and displays the
    // selected events as either xml or just the source and description.
    //
    public class ReadEvents
    {
        static void PrintUsage()
        {
            Console.WriteLine(
                "\nUsage:\n\n" + 
                "   ReadEvents PATH [/OPTION:VALUE [/OPTION:VALUE] ...]\n\n" +
                "   PATH is the name of the log or log file to read.  If a log file, the /lf option must be specified.\n" +
                "\nOptions:\n\n" +
                "   /logfile(lf)            Indicates that PATH is a log file instead of a log name.\n" +
                "   /query(q):VALUE         Filters the events to read with the XPath expression specified by VALUE.  Default is to read all events.\n" +
                "   /reversedirection(rd)   Reads events from the end of the log to the beginning (instead of the default beginning to end).\n" +
                "   /count(c):VALUE         Reads only the number of events specified by VALUE.\n" +
                "   /format(f):VALUE        Displays the events in the format specified by VALUE.  VALUE can be XML or Text (which simply displays the event source and description).\n"
                );
        }

        public static void Main(string[] args)
        {
            int exitCode = 0;

            String path;
            String query = "*";
            QueryFlags queryflags = QueryFlags.QueryChannelPath | QueryFlags.QueryForwardDirection;
            UInt32 count = UInt32.MaxValue;
            String format = "xml";

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

                path = args[0];

                char[] delimiters = {':'};

                for ( int i=1 ; i < args.Length; i++ )
                {
                    String option = args[i].Substring( 1 );
                    String[] words = option.Split( delimiters, 2 );
                    words[0] = words[0].ToLower();

                    switch( words[0] )
                    {
                        case "logfile":
                        case "lf":
                            queryflags &= ~QueryFlags.QueryChannelPath;
                            queryflags |= QueryFlags.QueryFilePath;
                            break;

                        case "query":
                        case "q":
                            query = words[1];
                            break;

                        case "reversedirection":
                        case "rd":
                            queryflags &= ~QueryFlags.QueryForwardDirection;
                            queryflags |= QueryFlags.QueryReverseDirection;
                            break;

                        case "count":
                        case "c":
                            count = Convert.ToUInt32( words[1] );
                            break;

                        case "format":
                        case "f":
                            format = words[1].ToLower();
                            if ( format != "text" && format != "xml" )
                                throw (new Exception( String.Format("Unrecognized format option: {0}.", format) ));
                            break;

                        default:
                            throw (new Exception( String.Format("Unrecognized parameter option: {0}.", words[0]) ));

                    }
                }


                // query the event log

                IntPtr resultSetPtr = Events.Query( IntPtr.Zero, path, query, queryflags );
                if (resultSetPtr == IntPtr.Zero)
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("Query failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }

                using ( SafeEventHandle resultSet = new SafeEventHandle( resultSetPtr ) )
                {
                    // read the selected events

                    IntPtr[] events = new IntPtr[1];
                    int timeout = 5000;     // 5 seconds
                    int returned = 0;
                    UInt32 n = 0;

                    while ( Events.Next( resultSet.Handle, 1, events, timeout, 0, out returned ) && n++ < count )
                    {
                        foreach (IntPtr evtptr in events)
                        {
                            using (SafeEventHandle evt = new SafeEventHandle(evtptr) )
                            {
                                if ( format == "xml" )
                                    Console.WriteLine( "\n" + RenderEventToXml( evt, RenderFlags.RenderEventXml ) );
                                else if ( format == "text" )
                                    Console.WriteLine( "\n" + FormatEventAsText( evt ) );
                            }
                        }
                    }

                    if ( Win32Error.ErrorNoMoreItems != Marshal.GetLastWin32Error() &&
                        Win32Error.ErrorSuccess != Marshal.GetLastWin32Error() )
                    {
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception(String.Format("Failed to read next event. error: {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                    }
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
    

        //
        // FormatEventAsText:  selects and formats specific event properties and returns them in an easy to read string
        //

        static string FormatEventAsText( SafeEventHandle evt )
        {
            // first render the provider (publisher) name which will be needed to open the publisher metadata

            String EventText = "";
            String[] valuePaths = {"Event/System/Provider/@Name"};
            UInt32 numValuePaths = 1;
            int bufferSize = 1024, bufferUsed = 0, valueCount = 0;
            IntPtr values = IntPtr.Zero;
            bool result = false;

            IntPtr ctxPtr = Events.CreateRenderContext( numValuePaths, valuePaths, RenderContextFlags.RenderContextValues );
            if (ctxPtr == IntPtr.Zero)
            {
                Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                throw (new Exception(String.Format("CreateRenderContext failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
            }

            values = Marshal.AllocHGlobal( bufferSize );
            if ( values == IntPtr.Zero )
            {
                throw (new Exception( String.Format("Failed to allocate space for rendered data.") ));
            }

            using ( SafeEventHandle ctx = new SafeEventHandle( ctxPtr ) )
            {
                result = Events.RenderValues( ctx.Handle, evt.Handle, RenderFlags.RenderEventValues, 
                    bufferSize, values, out bufferUsed, out valueCount );

                if ( !result && Win32Error.ErrorInsufficientBuffer == Marshal.GetLastWin32Error() )
                {
                    // resize the buffer and try again
                    Marshal.FreeHGlobal( values );
                    values = Marshal.AllocHGlobal( bufferUsed );
                    if ( values == IntPtr.Zero )
                    {
                        throw (new Exception( String.Format("Failed to allocate space for rendered data.") ));
                    }
                    bufferSize = bufferUsed;

                    result = Events.RenderValues( ctx.Handle, evt.Handle, RenderFlags.RenderEventValues, 
                        bufferSize, values, out bufferUsed, out valueCount );
                }
            }

            if ( !result )
            {
                Marshal.FreeHGlobal( values );
                Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                throw (new Exception(String.Format("RenderValues failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
            }

            EventVariant var = (EventVariant) Marshal.PtrToStructure( values, typeof(EventVariant) );
            if (EventVariantType.String != var.type)
            {
                Marshal.FreeHGlobal( values );
                throw (new Exception(String.Format("Expected variant type {0} (String), actual {1}.", EventVariantType.String, var.type)));
            }
            Object publisher = Events.VariantToObject(var);
            Marshal.FreeHGlobal( values );

            EventText += "Source:\t\t" + publisher.ToString();


            // now open the publisher metadata and read the event message

            IntPtr publisherMetadata = Events.OpenPublisherMetadata( IntPtr.Zero, publisher.ToString(), null, 0x409, 0 );
            if ( IntPtr.Zero == publisherMetadata )
            {
                Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                throw (new Exception(String.Format("OpenPublisherMetadata failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
            }

            using (SafeEventHandle hPublisherMetadata = new SafeEventHandle( publisherMetadata ))
            {
                StringBuilder message = new StringBuilder(bufferSize);
                result = Events.FormatMessage( publisherMetadata, evt.Handle, 0, 0, values, 
                    FormatMessageFlags.FormatMessageEvent, bufferSize, message, out bufferUsed );
                if ( !result && Win32Error.ErrorInsufficientBuffer == Marshal.GetLastWin32Error() )
                {
                    // resize the buffer and try again
                    message.Length = bufferUsed;
                    bufferSize = bufferUsed;
                    result = Events.FormatMessage( publisherMetadata, evt.Handle, 0, 0, IntPtr.Zero, 
                        FormatMessageFlags.FormatMessageEvent, bufferSize, message, out bufferUsed );
                }
                if ( !result )
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("FormatMessage failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }

                EventText += "\nDescription:\t" + message.ToString();
                return EventText;
            }
        }

    }

}
