using System;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using Microsoft.Samples.EventLog.API;

namespace Microsoft.Samples.EventLog.Logging
{
    //
    // The LogSample class demonstrates some of the logging capabilities of the new Windows
    // Event Log.  This sample retrieves log information, exports events in a log, archives
    // the localized message information for the exported log, and clears the log.
    //

    public class LogSample
    {

        public static void Main(string[] args)
        {
            int exitCode = 0;

            IntPtr session = IntPtr.Zero;
            String logPath = "Application";
            OpenLogFlags openflags = OpenLogFlags.ChannelPath;

            LogPropertyIdentifier propertyId = LogPropertyIdentifier.LogNumberOfLogRecords;
            EventVariant propertyValue = new EventVariant();
            int propertySize = Marshal.SizeOf(propertyValue);
            int sizeNeededUsed = 0;

            String query = "*/System[Level <= 3 and Level >=1]";     // select all events of level warning or higher 
            String targetFile = "%USERPROFILE%\\export.evtx";
            ExportLogFlags exportflags = ExportLogFlags.ChannelPath;

            try 
            {
                // get command line information

                if ( args.Length > 0 ) 
                {
                    if ( args[0] == "/?" )
                    {
                        Console.WriteLine("Usage: LogSample [<logname> [<exportfile>]]");
                        Environment.Exit(0);
                    }
                    else
                    {
                        logPath = args[0];
                        if ( args.Length > 1 )
                            targetFile = args[1];
                    }
                }

                
                //
                // Get log information
                //

                IntPtr logPtr = Events.OpenLog( session, logPath, openflags );
                if (logPtr == IntPtr.Zero)
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("OpenLog failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }
                
                using ( SafeEventHandle logHandle = new SafeEventHandle( logPtr ) )
                {
                    if ( !Events.GetLogInfo( logHandle.Handle, propertyId, propertySize, ref propertyValue, ref sizeNeededUsed ) )
                    {
                        // This method allows for a sizing call (InsufficientBuffer return) but none of the properties
                        // currently supported require memory beyond that already allocated by EventVariant.
                        Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                        throw (new Exception( String.Format("GetLogInfo failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message )));
                    }
                }

                if (EventVariantType.UInt64 != propertyValue.type)
                    throw (new Exception(String.Format("Expected variant type {0} (UInt64), actual {1}.", EventVariantType.UInt64, propertyValue.type)));

                Console.WriteLine( "The {0} log contains {1} events.", logPath, propertyValue.UInt64Val );


                //
                // Export selected events from a log
                //

                if ( !Events.ExportLog( session, logPath, query, targetFile, exportflags ) )
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("ExportLog failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }
                Console.WriteLine( "Selected events from the {0} log have been exported to file {1}.", logPath, targetFile );


                //
                // Capture localized event information so that the exported log can be viewed on 
                // systems that might not have some of the event providers installed.
                //

                int locale = 0x409;
                if ( !Events.LocalizeExportedLog( session, targetFile, locale, LocalizeExportLogFlags.Default ) )
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("LocalizeExportedLog failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }
                Console.WriteLine( "The export file {0} has been localized to locale {1} for archive.", targetFile, locale );


                //
                // Clear the log
                //

                if ( !Events.ClearLog( session, logPath, null, ClearLogFlags.Default ) )
                {
                    Win32Exception ex = new Win32Exception(Marshal.GetLastWin32Error());
                    throw (new Exception(String.Format("ClearLog failed with error {0}, {1}.", Marshal.GetLastWin32Error().ToString(), ex.Message)));
                }
                Console.WriteLine( "The {0} log has been cleared.", logPath );

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
