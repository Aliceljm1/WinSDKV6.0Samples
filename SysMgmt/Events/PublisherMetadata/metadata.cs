using System;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using Microsoft.Samples.EventLog.API;

namespace Microsoft.Samples.EventLog.PublisherMetadata
{
    //
    // The PublisherMetadata class demonstrates some of the publisher and event discovery
    // capabilities of the new Windows Event Log.  This sample enumerates installed publishers
    // as well as reads publisher and event properties.
    //

    public class PublisherMetadata
    {
        public static void Main(string[] args)
        {
            int exitCode = 0;

            bool listPublishers = true;
            IntPtr session = IntPtr.Zero;
            String publisherName = "";
            bool result;
            IntPtr propertyValuePtr = IntPtr.Zero;

            try 
            {
                // get command line information

                if ( args.Length > 0 ) 
                {
                    if ( args[0] == "/?" )
                    {
                        Console.WriteLine(
                            "Usage: PublisherMetadata [<publisherName>]\n" + 
                            "(When publisherName is not specified, the names of the first 20 publishers will be listed.)" );
                        Environment.Exit(0);
                    }
                    else
                    {
                        publisherName = args[0];
                        listPublishers = false;
                    }
                }

                if ( listPublishers )
                {
                    //
                    // List the first 'n' publishers
                    //

                    int count = 20;
                    Console.WriteLine( "First {0} publishers: ", count );

                    IntPtr publisherEnumPtr = Events.OpenPublisherEnum( session, 0 );
                    if (publisherEnumPtr == IntPtr.Zero)
                    {
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception(String.Format("OpenPublisherEnum failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                    }

                    using ( SafeEventHandle publisherEnum = new SafeEventHandle( publisherEnumPtr ) )
                    {
                        int bufferSize = 0, bufferUsed = 0;
                        StringBuilder publisherId = new StringBuilder( bufferSize );

                        for ( int i=0; i < count; i++ )
                        {
                            result = Events.NextPublisherId( publisherEnumPtr, bufferSize, publisherId, out bufferUsed );
                            if ( !result && Win32Error.ErrorInsufficientBuffer == Marshal.GetLastWin32Error() )
                            {
                                publisherId.Length = bufferUsed;   // resize the buffer and try again
                                bufferSize = bufferUsed;
                                result = Events.NextPublisherId( publisherEnumPtr, bufferSize, publisherId, out bufferUsed );
                            }
                            if ( !result )
                            {
                                Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                                throw (new Exception(String.Format("NextPublisherId failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                            }
                            Console.WriteLine( "\t{0}", publisherId.ToString() );
                        }
                    }
                }
                else
                {
                    //
                    // Read the specified publisher's metadata
                    //

                    IntPtr publisherMetadataPtr = IntPtr.Zero;
                    IntPtr eventMetadataEnumPtr = IntPtr.Zero;
                    int bufferSize = 0, bufferUsed = 0;

                    publisherMetadataPtr = Events.OpenPublisherMetadata( session, publisherName, null, 0, 0 );
                    if (publisherMetadataPtr == IntPtr.Zero)
                    {
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception(String.Format("OpenPublisherMetadata failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                    }

                    using ( SafeEventHandle publisherMetadata = new SafeEventHandle( publisherMetadataPtr ) )
                    {
                        PublisherMetadataPropertyID propertyId = PublisherMetadataPropertyID.MessageFilePath;
                        propertyValuePtr = Marshal.AllocHGlobal( bufferSize );

                        result = Events.GetPublisherMetadataPropertyBuffer( publisherMetadata.Handle, propertyId, 0, bufferSize, propertyValuePtr, out bufferUsed );
                        if ( !result && Win32Error.ErrorInsufficientBuffer == Marshal.GetLastWin32Error() )
                        {
                            // resize the buffer and try again
                            Marshal.FreeHGlobal( propertyValuePtr );
                            propertyValuePtr = Marshal.AllocHGlobal( bufferUsed );
                            if ( propertyValuePtr == IntPtr.Zero )
                            {
                                throw (new Exception( String.Format("Failed to allocate space for rendered data.") ));
                            }
                            bufferSize = bufferUsed;
                            result = Events.GetPublisherMetadataPropertyBuffer( publisherMetadata.Handle, propertyId, 0, bufferSize, propertyValuePtr, out bufferUsed );
                        }
                        if ( !result )
                        {
                            Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                            throw (new Exception(String.Format("GetPublisherMetadataPropertyBuffer failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                        }

                        EventVariant var = (EventVariant) Marshal.PtrToStructure( propertyValuePtr, typeof(EventVariant) );
                        if (EventVariantType.String != var.type)
                            throw (new Exception(String.Format("Expected variant type {0} (String), actual {1}.", EventVariantType.String, var.type)));
                        Object messageFilePath = Events.VariantToObject(var);

                        if ( null == messageFilePath )
                            messageFilePath = "<null>";
                        Console.WriteLine( "MessageFilePath for publisher '{0}' is '{1}'.", publisherName, messageFilePath.ToString() );


                        //
                        // Read the publisher's event metadata
                        //

                        eventMetadataEnumPtr = Events.OpenEventMetadataEnum( publisherMetadata.Handle, 0 );
                        if (eventMetadataEnumPtr == IntPtr.Zero)
                        {
                            Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                            throw (new Exception(String.Format("OpenEventMetadataEnum failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                        }
                    }

                    using ( SafeEventHandle eventMetadataEnum = new SafeEventHandle( eventMetadataEnumPtr ) )
                    {
                        Console.WriteLine( "Publisher '{0}' contains event metadata for the following event ids:", publisherName );

                        IntPtr eventMetadataPtr = IntPtr.Zero;
                        while ( IntPtr.Zero != (eventMetadataPtr = Events.NextEventMetadata( eventMetadataEnum.Handle, 0 )) )
                        {
                            using ( SafeEventHandle eventMetadata = new SafeEventHandle( eventMetadataPtr ) )
                            {
                                EventVariant metadataValue = new EventVariant();
                                bufferSize = Marshal.SizeOf( metadataValue );
                                bufferUsed = 0;
                                if ( !Events.GetEventMetadataProperty( eventMetadata.Handle, EventMetadataPropertyID.EventID, 0, bufferSize, out metadataValue, out bufferUsed ) )
                                {
                                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                                    throw (new Exception(String.Format("GetEventMetadataProperty failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                                }

                                if (EventVariantType.UInt32 != metadataValue.type)
                                    throw (new Exception(String.Format("Expected variant type {0} (UInt32), actual {1}.", EventVariantType.UInt32, metadataValue.type)));
                                Console.WriteLine("\t{0}", metadataValue.UInt32Val);
                            }
                        }
                        if ( Win32Error.ErrorNoMoreItems != Marshal.GetLastWin32Error() )
                        {
                            Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                            throw (new Exception(String.Format("NextEventMetadata failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                        }
                    }

                }

            }
            catch(Exception e) 
            {
                Console.WriteLine(e.ToString());
                exitCode = 1;
            }
            finally
            {
                if ( IntPtr.Zero != propertyValuePtr )
                    Marshal.FreeHGlobal( propertyValuePtr );
            }

            Environment.Exit(exitCode);
        }

    }

}
