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
        
        public SafeEventHandle(IntPtr eventlogHandle, bool release) : base(eventlogHandle, release)
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

#region Subscriptions

    public enum SubscribeFlags
    {
        FutureEvents            = 0x1,
        StartAtOldestRecord,
        StartAfterBookmark,
        TolerateQueryErrors     = 0x1000,
        Strict                  = 0x10000
    } 

    public enum SubscribeNotifyAction
    {
        Error = 0,
        Deliver
    } 

    public delegate int EventSubscribeCallback( 
        [MarshalAs(UnmanagedType.I4)]SubscribeNotifyAction action, 
        IntPtr userContext,
        IntPtr eventHandle);
   
    public enum EventPropertyId
    {
        EventQueryIDs           = 0x0,
        EventPath               = 0x1
    } 

    public partial class Events
    {
        [DllImport("wevtapi.dll", EntryPoint = "EvtSubscribe", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr Subscribe(
            IntPtr Session,
            IntPtr SignalEvent,
            string ChannelPath,
            string Query,
            IntPtr Bookmark,
            IntPtr context,
            [MarshalAs(UnmanagedType.FunctionPtr)]EventSubscribeCallback Callback,
            SubscribeFlags Flags);

        [DllImport("wevtapi.dll", EntryPoint = "EvtGetEventInfo", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool GetEventInfo(
            IntPtr eventHandle,
            [MarshalAs(UnmanagedType.I4)]EventPropertyId propertyIdentifier,
            int bufferSize,
            IntPtr bufferPtr,
            ref int bufferRequired);
    }

#endregion

#region Rendering

    public enum RenderContextFlags
    {
        RenderContextValues  = 0x0,      // Render specific properties
        RenderContextSystem  = 0x1,      // Render all system properties (System)
        RenderContextUser    = 0x2       // Render all user properties (User/EventData)
    }
    public enum RenderFlags 
    {
        RenderEventValues   = 0x0,
        RenderEventXml      = 0x1,
        RenderBookmark      = 0x2
    }
    public enum FormatMessageFlags
    {
        FormatMessageEvent           = 0x1,      
        FormatMessageLevel           = 0x2,
        FormatMessageTask            = 0x3,
        FormatMessageOpcode          = 0x4,
        FormatMessageKeyword         = 0x5,
        FormatMessageChannel         = 0x6, 
        FormatMessageProvider        = 0x7, 
        FormatMessageId              = 0x8,
        FormatMessageXml             = 0x9, 
    }

    public partial class Events
    {
        [DllImport("wevtapi.dll", EntryPoint = "EvtCreateRenderContext", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr CreateRenderContext(
            UInt32 countOfValues,
            String[] valuePaths,
            RenderContextFlags flags);

        [DllImport("wevtapi.dll", EntryPoint = "EvtRender", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool Render(
            IntPtr context,
            IntPtr eventHandle,
            RenderFlags flags,
            int bufferSize, 
            [Out, MarshalAs(UnmanagedType.LPWStr)] StringBuilder buff,
            out int bufferUsed,
            out int count);

        [DllImport("wevtapi.dll", EntryPoint = "EvtRender", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool RenderValues(
            IntPtr context,
            IntPtr eventHandle,
            RenderFlags flags,
            int bufferSize,
            IntPtr values,
            out int bufferUsed,
            out int valueCount);

        [DllImport("wevtapi.dll", EntryPoint = "EvtFormatMessage", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool FormatMessage(
            IntPtr publisherMetadata,
            IntPtr eventHandle,
            int messageId,
            int valueCount,
            IntPtr values,
            FormatMessageFlags flags,
            int bufferSize,
            [Out, MarshalAs(UnmanagedType.LPWStr)] StringBuilder buffer,
            out int bufferUsed );
    }

    #endregion

#region Publisher and Event Metadata

    public enum ChannelReferenceFlags
    {
        Imported = 0x1
    }

    public enum PublisherMetadataPropertyID
    {
        PublisherGuid = 0,
        ResourceFilePath,
        ParameterFilePath,
        MessageFilePath,
        HelpLink,
        PublisherMessageID,     // UInt32

        ChannelReferences,
        ChannelReferencePath,
        ChannelReferenceIndex,
        ChannelReferenceID,
        ChannelReferenceFlags,
        ChannelReferenceMessageID, // UInt32

        Levels, 
        LevelName,  
        LevelValue, 
        LevelMessageID, 

        Tasks, 
        TaskName,  
        TaskEventGuid,     
        TaskValue, 
        TaskMessageID, 

        Opcodes, 
        OpcodeName,  
        OpcodeValue, 
        OpcodeMessageID, 

        Keywords, 
        KeywordName,  
        KeywordValue, 
        KeywordMessageID, 

        PropertyIdEND 
    }

    public enum EventMetadataPropertyID
    {
        EventID,
        EventVersion,
        EventChannel,
        EventLevel,
        EventOpcode,
        EventTask,
        EventKeyword,
        MessageID,
        EventTemplate,
        PropertyIdEND
    }

    public partial class Events
    {
        [DllImport("wevtapi.dll", EntryPoint="EvtOpenPublisherEnum", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr OpenPublisherEnum(
            IntPtr session,
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtNextPublisherId", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool NextPublisherId(
            IntPtr publisherEnum,
            int bufferSize,
            [Out, MarshalAs(UnmanagedType.LPWStr)] StringBuilder publisherIdBuffer,
            out int bufferUsed );

        [DllImport("wevtapi.dll", EntryPoint="EvtOpenPublisherMetadata", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr OpenPublisherMetadata(
            IntPtr session,
            [MarshalAs(UnmanagedType.LPWStr)] String publisherIdentity,
            [MarshalAs(UnmanagedType.LPWStr)] String logfilePath,
            int locale,
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtGetPublisherMetadataProperty", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError = true)]
        public static extern bool GetPublisherMetadataProperty(
            IntPtr publisherMetadata,
            [MarshalAs(UnmanagedType.I4)] PublisherMetadataPropertyID propertyId,
            UInt32 flags,
            int  bufferSize,
            out EventVariant publisherMetadataPropertyVariant,
            out int  bufferUsed );

        [DllImport("wevtapi.dll", EntryPoint="EvtGetPublisherMetadataProperty", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError = true)]
        public static extern bool GetPublisherMetadataPropertyBuffer(
            IntPtr publisherMetadata,
            [MarshalAs(UnmanagedType.I4)] PublisherMetadataPropertyID propertyId,
            UInt32 flags,
            int  bufferSize,
            IntPtr publisherMetadataPropertyBuffer,
            out int  bufferUsed );


        [DllImport("wevtapi.dll", EntryPoint="EvtOpenEventMetadataEnum", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr OpenEventMetadataEnum(
            IntPtr publisherMetadata,
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtNextEventMetadata", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern IntPtr NextEventMetadata(
            IntPtr eventMetadataEnum,
            UInt32 flags );

        [DllImport("wevtapi.dll", EntryPoint="EvtGetEventMetadataProperty", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Auto, SetLastError=true)]
        public static extern bool GetEventMetadataProperty(
            IntPtr eventMetadata,
            [MarshalAs(UnmanagedType.I4)]  EventMetadataPropertyID propertyId,
            UInt32 flags,
            int  bufferSize,
            out EventVariant eventMetadataPropertyVariant,
            out int  bufferUsed );
    }

#endregion

}
