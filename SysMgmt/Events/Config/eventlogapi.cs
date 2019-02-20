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

    public partial class Events
    {
        // standard function to convert EventVariant to a .NET object
        public static Object VariantToObject(EventVariant var)
        {
            Object val = null;

            switch (var.type)
            {
                case EventVariantType.Null:
                    val = null;
                    break;

                case EventVariantType.Boolean:
                    if (var.Int32Val == 0)
                        val = false;
                    else
                        val = true;
                    break;

                case EventVariantType.AnsiString:
                    val = Marshal.PtrToStringAnsi(var.EvtHandle);
                    break;

                case EventVariantType.SByte:
                    val = var.SByteVal;
                    break;

                case EventVariantType.Int16:
                    val = var.Int16Val;
                    break;

                case EventVariantType.Int32:
                    val = var.Int32Val;
                    break;

                case EventVariantType.Int64:
                    val = var.Int64Val;
                    break;

                case EventVariantType.Byte:
                    val = var.ByteVal;
                    break;

                case EventVariantType.UInt16:
                    val = var.UInt16Val;
                    break;

                case EventVariantType.UInt32:
                case EventVariantType.HexInt32:
                    val = var.UInt32Val;
                    break;

                case EventVariantType.UInt64:
                case EventVariantType.HexInt64:
                    val = var.UInt64Val;
                    break;

                case EventVariantType.Single:
                    val = var.SingleVal;
                    break;

                case EventVariantType.Double:
                    val = var.DoubleVal;
                    break;

                case EventVariantType.FileTime:
                    val = DateTime.FromFileTime((long)var.FileTimeVal);
                    break;

                case EventVariantType.SysTime:
                    val = new DateTime((long)var.SysTimeVal);
                    break;

                case EventVariantType.String:
                case EventVariantType.EventXml:
                    val = Marshal.PtrToStringAuto(var.StringVal);
                    break;

                case EventVariantType.Guid:
                    Byte[] GuidArray = new Byte[16];
                    for (int i = 0; i < 16; i++)
                    {
                        GuidArray[i] = Marshal.ReadByte(var.GuidVal, i);
                    }
                    val = new Guid(GuidArray);
                    break;

                case EventVariantType.Sid:
                    break;
            }
 
            return val;
        }

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

#region Channel Configuration

    public enum ChannelConfigPropertyID
    {
        Enabled = 0,
        Isolation,
        Type,
        OwningPublisher,
        IsClassicChannel,
        Access,
        LogRetention,
        LogAutoBackup,
        LogMaxSize,
        LogFilePath,
        PublishingLevel,
        PublishingKeywords,
        PublishingControlGuid,
        PublishingBufferSize,
        PublishingMinBuffers,
        PublishingMaxBuffers,
        PublishingLatency,
        PublishingClockType,
        PublishingSidType,
        PublisherList,
        PropertyIdEND
    } ;

    public enum ChannelType
    {
        Admin,
        Operational,
        Analytic,
        Debug,
        ExternalLog
    }
    public enum ChannelIsolationType
    {
        Application,
        System,
        Custom
    }

    public enum ChannelClockType
    {
        ClockTypeSystemTime,
        ClockTypeQPC
    }

    public enum ChannelSidType
    {
        None,
        Publishing
    }

    public partial class Events
    {
        [DllImport("wevtapi.dll", EntryPoint="EvtOpenChannelEnum", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr OpenChannelEnum(
            IntPtr session, 
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtNextChannelPath", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool NextChannelPath(
            IntPtr channelEnum, 
            int  bufferSize,
            [Out, MarshalAs(UnmanagedType.LPWStr)] StringBuilder channelPathBuffer,
            out int  bufferRequired );

        [DllImport("wevtapi.dll", EntryPoint="EvtOpenChannelConfig", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr OpenChannelConfig(
            IntPtr Session,
            [MarshalAs(UnmanagedType.LPWStr)] String channelPath,
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtSaveChannelConfig", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool SaveChannelConfig(
            IntPtr ChannelConfig,
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtSetChannelConfigProperty", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool SetChannelConfigProperty(
            IntPtr channelConfig,  
            [MarshalAs(UnmanagedType.I4)]ChannelConfigPropertyID propertyId,
            UInt32 flags,
            ref EventVariant propertyValue ); 

        [DllImport("wevtapi.dll", EntryPoint="EvtGetChannelConfigProperty", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool GetChannelConfigProperty(
            IntPtr channelConfig, 
            [MarshalAs(UnmanagedType.I4)]ChannelConfigPropertyID propertyId,
            UInt32 flags,
            int  bufferSize,
            ref EventVariant propertyValueVariant,
            out int  bufferUsed );

        [DllImport("wevtapi.dll", EntryPoint="EvtGetChannelConfigProperty", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool GetChannelConfigPropertyBuffer(
            IntPtr channelConfig,
            [MarshalAs(UnmanagedType.I4)]ChannelConfigPropertyID propertyId,
            UInt32 flags,
            int  bufferSize,
            IntPtr propertyValueBuffer,
            out int  bufferUsed );
    }

#endregion

}
