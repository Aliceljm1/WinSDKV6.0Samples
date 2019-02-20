using System;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using Microsoft.Samples.EventLog.API;

namespace Microsoft.Samples.EventLog.Configuration
{
    //
    // The ChannelConfig class demonstrates some of the configuration capabilities of the 
    // new Windows Event Log.  This sample reads and sets a log's maximum size.
    //

    public class ChannelConfig
    {
        public static void Main(string[] args)
        {
            int exitCode = 0;

            IntPtr session = IntPtr.Zero;
            String channelPath = "Application";
            ChannelConfigPropertyID propertyId = ChannelConfigPropertyID.LogMaxSize;

            try 
            {
                // get command line information

                if ( args.Length > 0 ) 
                {
                    if ( args[0] == "/?" )
                    {
                        Console.WriteLine("Usage: ChannelConfig [<channelPath> [<newMaxLogSize>]]");
                        Environment.Exit(0);
                    }
                    else
                    {
                        channelPath = args[0];
                    }
                }


                //
                // Read a configuration property of the specified channel
                //

                IntPtr configPtr = Events.OpenChannelConfig( session, channelPath, 0 );
                if (configPtr == IntPtr.Zero)
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("OpenChannelConfig failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }

                using ( SafeEventHandle configHandle = new SafeEventHandle( configPtr ) )
                {
                    EventVariant propertyValue = new EventVariant();
                    int bufferSize = Marshal.SizeOf( propertyValue );
                    int bufferRequired = 0;
                    if ( !Events.GetChannelConfigProperty( configHandle.Handle, propertyId, 0, bufferSize, ref propertyValue, out bufferRequired ) )
                    {
                        // This method allows for a sizing call (InsufficientBuffer return) but the LogMaxSize property does not
                        // require memory beyond that already allocated by EventVariant.
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception(String.Format("GetChannelConfigProperty failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                    }
                    if (EventVariantType.UInt64 != propertyValue.type)
                        throw (new Exception(String.Format("Expected variant type {0} (UInt64), actual {1}.", EventVariantType.UInt64, propertyValue.type)));

                    Console.WriteLine("The {0} log's configured maximum size was {1} bytes.", channelPath, propertyValue.UInt64Val);


                    //
                    // Set and save a configuration property value
                    //

                    if ( args.Length > 1 )
                        propertyValue.UInt64Val = Convert.ToUInt64( args[1] );
                    else
                        propertyValue.UInt64Val *= 2;   // double the maximum log size

                    if ( !Events.SetChannelConfigProperty( configHandle.Handle, propertyId, 0, ref propertyValue ) )
                    {
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception(String.Format("SetChannelConfigProperty failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                    }
                    if ( !Events.SaveChannelConfig( configHandle.Handle, 0 ) )
                    {
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception(String.Format("SaveChannelConfig failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                    }
                    Console.WriteLine( "The {0} log's maximum size has been re-configured to {1} bytes.", channelPath, propertyValue.UInt64Val );
                }

            }
            catch(Exception e) 
            {
                Console.WriteLine(e.ToString());
                exitCode = 1;
            }

            Environment.Exit(exitCode);
        }

    }

}
