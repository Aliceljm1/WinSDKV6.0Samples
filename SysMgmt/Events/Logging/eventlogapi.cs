using System;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Samples.EventLog.API;

namespace Microsoft.Samples.EventLog.API
{

#region Common structures, enumerations, and related methods 

    /// <summary>
    /// Error codes
    /// </summary>
    public class Win32Error
    {
        
        public const int ErrorSuccess                    = 0;
        public const int ErrorOutOfMemory                = 14;
        public const int ErrorInsufficientBuffer         = 122;
        public const int ErrorNoMoreItems                = 259;
    }

    /// <summary>
    /// EventVariant types
    /// </summary>
    [ComVisible(false)]
    public enum EventVariantType
    {
        Null = 0,
        String = 1,
        AnsiString = 2,
        SByte = 3,
        Byte = 4,
        Int16 = 5,
        UInt16 = 6,
        Int32 = 7,
        UInt32 = 8,
        Int64 = 9,
        UInt64 = 10,
        Single = 11,
        Double = 12,
        Boolean = 13,
        Binary = 14,
        Guid = 15,
        SizeT = 16,
        FileTime = 17,
        SysTime = 18,
        Sid = 19,
        HexInt32 = 20,
        HexInt64 = 21,
        EvtHandle = 32,
        EventXml = 35,
        Array = 128
    }
    
    /// <summary>
    /// Mask values for Array
    /// </summary>
    [ComVisible(false)]
    public enum EVT_MASK
    {
        TYPE_MASK = 0x7f,
        TYPE_ARRAY = 128
    }

    /// <summary>
    /// EventVariant definition
    /// </summary>
    [ComVisible(false)]
    [StructLayout(LayoutKind.Explicit)]
    public struct EventVariant
    {
        [FieldOffset(0)]
        public int BooleanVal;
        [FieldOffset(0)]
        public sbyte SByteVal;
        [FieldOffset(0)]
        public Int16 Int16Val;
        [FieldOffset(0)]
        public Int32 Int32Val;
        [FieldOffset(0)]
        public Int64 Int64Val;
        [FieldOffset(0)]
        public byte ByteVal;
        [FieldOffset(0)]
        public UInt16 UInt16Val;
        [FieldOffset(0)]
        public UInt32 UInt32Val;
        [FieldOffset(0)]
        public UInt64 UInt64Val;
        [FieldOffset(0)]
        public float SingleVal;
        [FieldOffset(0)]
        public double DoubleVal;
        [FieldOffset(0)]
        public UInt64 FileTimeVal;
        [FieldOffset(0)]
        public UInt64 SysTimeVal;
        [FieldOffset(0)]
        public IntPtr GuidVal;
        [FieldOffset(0)]
        public IntPtr StringVal;
        //[FieldOffset(0)]
        //public IntPtr AnsiStringVal;
        //[FieldOffset(0)]
        //public IntPtr BinaryVal;
        [FieldOffset(0)]
        public IntPtr SidVal;
        //[FieldOffset(0)]
        //public UInt64 SizeTVal;
        [FieldOffset(0)]
        public IntPtr EvtHandle;
        //[FieldOffset(0)]
        //public IntPtr refPtr;      // For all ref pointers

        [FieldOffset(8)]
        public int ItemCount;   // number of elements (not length) in bytes.  
        [FieldOffset(12)]
        public EventVariantType type;
    }

#endregion

#region General
        
    //
    // The Events class imports the Windows Event Log API.
    // 

    public partial class Events
    {
        [DllImport("wevtapi.dll", EntryPoint = "EvtClose", SetLastError = true)]
        public static extern bool CloseHandle(IntPtr eventHandle);
    }

    //
    // The SafeEventHandle class extends the SafeHandle class to provider a managed
    // wrapper for native Windows Event Log API handles.
    // 

    public class SafeEventHandle : SafeHandle
    {
        /// <summary>
        /// The constructors specify that the native Event Log handles will be owned
        /// by SafeHandle and consequently freed on Dispose.
        /// </summary>

        public SafeEventHandle(): base(IntPtr.Zero, true)
        {
        }

        public SafeEventHandle(IntPtr eventlogHandle) : base(eventlogHandle, true)
        {
        }
        

        /// <summary>
        /// required overrides
        /// </summary>

        public override bool IsInvalid
        {
            get
            {
                if (handle == IntPtr.Zero)
                {
                    return true;
                }
                return false;
            }
        }

        protected override bool ReleaseHandle()
        {
            return Events.CloseHandle(handle);
        }
        

        /// <summary>
        /// handle accessor property
        /// </summary>
        
        public IntPtr Handle
        {
            get
            {
                if (IsInvalid)
                {
                    return IntPtr.Zero;
                }
                return handle;
            }
        }

    }


#endregion

#region Log Maintenance and Information

    public enum OpenLogFlags
    {
        ChannelPath         = 0x1,
        FilePath            = 0x2
    }

    public enum ClearLogFlags
    {
        Default             = 0x0
    }

    public enum ExportLogFlags
    {
        ChannelPath         = 0x1,
        FilePath            = 0x2
    }

    public enum LocalizeExportLogFlags
    {
        Default             = 0x0
    }

    public enum LogPropertyIdentifier
    {
        LogCreationTime          = 0x0,
        LogLastAccessTime        = 0x1,   
        LogLastWriteTime         = 0x2,
        LogFileSize              = 0x3,   
        LogAttributes            = 0x4,
        LogNumberOfLogRecords    = 0x5,
        LogOldestRecordNumber    = 0x6,
        LogFull                  = 0x7
    }

    public partial class Events
    {
        [DllImport("wevtapi.dll", EntryPoint="EvtOpenLog", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr OpenLog(
            IntPtr session,
            string channelPath,
            [MarshalAs(UnmanagedType.I4)]OpenLogFlags flags);

        [DllImport("wevtapi.dll", EntryPoint="EvtGetLogInfo", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool GetLogInfo(
            IntPtr log,
            [MarshalAs(UnmanagedType.I4)]LogPropertyIdentifier propertyId,
            int propertyValueBufferSize,
            ref EventVariant propertyValueBuffer,
            ref int propertyValueBufferUsed);

        [DllImport("wevtapi.dll", EntryPoint="EvtClearLog", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool ClearLog(
            IntPtr session,
            [MarshalAs(UnmanagedType.LPWStr)]string channelPath,
            [MarshalAs(UnmanagedType.LPWStr)]string targetFilePath,
            [MarshalAs(UnmanagedType.I4)]ClearLogFlags flags);

        [DllImport("wevtapi.dll", EntryPoint="EvtExportLog", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool ExportLog(
            IntPtr session,
            [MarshalAs(UnmanagedType.LPWStr)]string channelPath,
            [MarshalAs(UnmanagedType.LPWStr)]string query,
            [MarshalAs(UnmanagedType.LPWStr)]string targetFilePath,
            [MarshalAs(UnmanagedType.I4)]ExportLogFlags flags);

        [DllImport("wevtapi.dll", EntryPoint="EvtArchiveExportedLog", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool LocalizeExportedLog(
            IntPtr session, 
            string logFilePath,
            int locale,
            [MarshalAs(UnmanagedType.I4)]LocalizeExportLogFlags flags);
    }

#endregion

}
