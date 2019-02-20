//
// TDHSample.cpp
//

#include "stdafx.h"


void
Usage (
    )
{
    wprintf(L"Usage: TdhSample etlFileName\n");
    exit (1);
}

//
//
// Routine Description:
//
//    Checks whether a file exists and is readable.
//
// Arguments:
//
//    fileName - File name.
//
// Return Value:
//
//  Non-zero if the file exists and is readable. Zero otherwise.
//
ULONG
CheckFile(
    __in LPWSTR fileName
    )
{
    HANDLE fileHandle;
    ULONG status;

	// Make sure this file can be accessed
    fileHandle = CreateFile(
                fileName,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
                );
    status = (fileHandle != INVALID_HANDLE_VALUE);

    if (fileHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(fileHandle);
    }
    
    return status;
}

//
//
// Routine Description:
//
//    Callback method for processing a buffer. Does not do anything but
//    updating global counters.
//
// Arguments:
//
//    logFile - Pointer to a log file.
//
// Return Value:
//
//    Always TRUE.
//
ULONG
WINAPI
BufferCallback(
    PEVENT_TRACE_LOGFILE logFile
    )
{
	//
	// Always return TRUE, so ProcessTrace() will continue.
	// Return FALSE will make ProcessTrace() stop working
	//
    return (TRUE);
}

//
//
// Routine Description:
//
//    Converts a GUID into a string.
//
// Arguments:
//
//    guidPointer - GUID
//
// Return Value:
// 
//    None.
//
void
PrintGuid(
    LPGUID guidPointer
    )
{
    wprintf(L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}\n",
               guidPointer->Data1, guidPointer->Data2,
               guidPointer->Data3,
               guidPointer->Data4[0], guidPointer->Data4[1],
               guidPointer->Data4[2], guidPointer->Data4[3],
               guidPointer->Data4[4], guidPointer->Data4[5],
               guidPointer->Data4[6], guidPointer->Data4[7]);
    return;
}

//
// Wrapper function for calling TdhGetProperty: top level property or property inside a struct
//
ULONG
GetProperty(
    PEVENT_RECORD eventRecord,
    __in_opt LPWSTR structureName,
    __in LPWSTR propertyName,
    ULONG propertySize,
    PBYTE propertyBuffer
    )
{
    PROPERTY_DATA_DESCRIPTOR propertyData[2];
    ULONG numberOfPropertyData;

    if (NULL == structureName) {
        // Top level property
        propertyData[0].ArrayIndex = ULONG_MAX;
        propertyData[0].PropertyName = (ULONGLONG)propertyName;
        numberOfPropertyData = 1;
    }
    else {
        // Structure member property
        propertyData[0].ArrayIndex = ULONG_MAX;
        propertyData[0].PropertyName = (ULONGLONG)structureName;
        propertyData[1].ArrayIndex = ULONG_MAX;
        propertyData[1].PropertyName = (ULONGLONG)propertyName;
        numberOfPropertyData = 2;
    }

    return TdhGetProperty(eventRecord, 0, NULL, numberOfPropertyData, &propertyData[0], propertySize, propertyBuffer);
}

//
// Wrapper function for calling TdhGetPropertySize: top level property or property inside a struct
//
ULONG
GetPropertySize(
    PEVENT_RECORD eventRecord,
    __in_opt LPWSTR structureName,
    __in LPWSTR propertyName,
    ULONG *propertySize
    )
{
    PROPERTY_DATA_DESCRIPTOR propertyData[2];
    ULONG numberOfPropertyData;

    if (NULL == structureName) {
        // Top level property
        propertyData[0].ArrayIndex = ULONG_MAX;
        propertyData[0].PropertyName = (ULONGLONG)propertyName;
        numberOfPropertyData = 1;
    }
    else {
        // Structure member property
        propertyData[0].ArrayIndex = ULONG_MAX;
        propertyData[0].PropertyName = (ULONGLONG)structureName;
        propertyData[1].ArrayIndex = ULONG_MAX;
        propertyData[1].PropertyName = (ULONGLONG)propertyName;
        numberOfPropertyData = 2;
    }

    return TdhGetPropertySize(eventRecord, 0, NULL, numberOfPropertyData, &propertyData[0], propertySize);
}

void
GetPropertyAndSize(
    PEVENT_RECORD eventRecord,
    __in_opt LPWSTR structureName,
    __in LPWSTR propertyName
    )
{
    ULONG status;
    ULONG propertySize;
    PBYTE property = NULL;
    BYTE properyBuffer[MAX_BUFFER];
    ULONG index;

    propertySize = MAX_BUFFER;
    property = &properyBuffer[0];

    status = GetProperty(eventRecord, structureName, propertyName, propertySize, property);

    if (ERROR_INSUFFICIENT_BUFFER == status) {
        // Static buffer is large enough, try to get the real property size

        status = GetPropertySize(eventRecord, structureName, propertyName, &propertySize);       
        if (ERROR_SUCCESS != status) {
            goto cleanup;
        }
        wprintf(L"\t\tPropertySize: %u\n", propertySize);
            
        property = (PBYTE)G_ALLOC(propertySize);
        if (NULL == property) {
            goto cleanup;
        }

        status = GetProperty(eventRecord, structureName, propertyName, propertySize, property);
        if (ERROR_SUCCESS != status) {
            goto cleanup;
        }
    }
    else if (ERROR_SUCCESS == status) {
        status = GetPropertySize(eventRecord, structureName, propertyName, &propertySize);       
        if (ERROR_SUCCESS != status) {
            goto cleanup;
        }
        wprintf(L"\t\tPropertySize: %u\n", propertySize);
    }
    else {
        // Error
        goto cleanup;
    }

    // Print out property here if needed, pProperty points to the buffer
    for (index = 0; index < propertySize; index++) {
        wprintf(L"\t\tProperty[%d]:\t0x%02X%\n", index, property[index]);
    }
    
cleanup:

    if (property && property != &properyBuffer[0]) {
        G_FREE(property);
    }
}

void
GetEventMapInfo(
    PEVENT_RECORD eventRecord,
    __in LPWSTR mapName
    )
{
    ULONG errorCode;
    EVENT_MAP_INFO eventMapInfo;
    PEVENT_MAP_INFO eventMapInfoPointer;
    PBYTE buffer = NULL;
    ULONG bufferSize = sizeof(eventMapInfo);
    
    errorCode = TdhGetEventMapInformation(eventRecord, mapName, &eventMapInfo, &bufferSize);
    if (ERROR_INSUFFICIENT_BUFFER == errorCode) {
        buffer = (PBYTE)G_ALLOC(bufferSize);
        if (NULL == buffer) {
            wprintf( L"GetEventMapInfo: Out of Memory error\n");      
            return;
        }

        eventMapInfoPointer = (PEVENT_MAP_INFO)buffer;
        
        errorCode = TdhGetEventMapInformation(eventRecord, mapName, eventMapInfoPointer, &bufferSize);
        if (ERROR_SUCCESS != errorCode) {
            wprintf(L"Error: TdhGetEventMapInformation with error code =0x%x\n", errorCode);
            goto end;
        }

        // TdhGetEventMapInformation Success here
    }
    else if (ERROR_SUCCESS == errorCode) {
        // Should not reach here, we don't have enough buffer  
    }
    else {
        // Other errors
        wprintf(L"Error: TdhGetEventMapInformation with error =0x%x\n", errorCode);
        return;
    }

end:
    
    if (buffer) {
        G_FREE(buffer);
    }
}

void
PrintEventPropertyInfo(
    PEVENT_RECORD eventRecord,
    PTRACE_EVENT_INFO buffer,
    PEVENT_PROPERTY_INFO propertyInfo,
    __in LPWSTR structureName
    )
{
    LPWSTR propertyName;
    LPWSTR mapName;
    ULONG structStartIndex;
    ULONG numOfStructMembers;
    ULONG index;
    PEVENT_PROPERTY_INFO sturctMemeberPropertyInfo;

    wprintf(L"\t--- EVENT_PROPERTY_INFO ---\n");
    wprintf(L"\tFlags: %u\n",  propertyInfo->Flags);
    propertyName = (LPWSTR)((PBYTE)buffer + propertyInfo->NameOffset);
    wprintf(L"\tPropertyName: %ws\n",  propertyName);
    wprintf(L"\tCount: %u\n",  propertyInfo->count);
    wprintf(L"\tLength: %d\n",  propertyInfo->length);

    if (PROPERTY_IS_STRUCTURE(propertyInfo->Flags)) {
        wprintf(L"\tStructStartIndex: %u\n",  propertyInfo->structType.StructStartIndex);
        wprintf(L"\tNumOfStructMembers: %u\n",  propertyInfo->structType.NumOfStructMembers);

        structStartIndex = propertyInfo->structType.StructStartIndex;
        numOfStructMembers = propertyInfo->structType.NumOfStructMembers;
        for (index = 0; index < numOfStructMembers; index++) {
            sturctMemeberPropertyInfo = &buffer->EventPropertyInfoArray[index+structStartIndex];
            PrintEventPropertyInfo(eventRecord, buffer, sturctMemeberPropertyInfo, propertyName);
        }
    }
    else {
        wprintf(L"\tInType: %u\n",  propertyInfo->nonStructType.InType);
        wprintf(L"\tOutType: %u\n",  propertyInfo->nonStructType.OutType);
        if (0 != propertyInfo->nonStructType.MapNameOffset) {
            mapName = (LPWSTR)((PBYTE)buffer + propertyInfo->nonStructType.MapNameOffset);
            wprintf(L"\tMapName: %ws\n",  mapName);

            GetEventMapInfo(eventRecord, mapName);
        }

        GetPropertyAndSize(eventRecord, structureName, propertyName);
    }
}
                
void
PrintTraceEventInfo(
    PEVENT_RECORD eventRecord,
    PTRACE_EVENT_INFO buffer,
    ULONG bufferSize
    )
{
    LPWSTR stringBuffer;
    PEVENT_PROPERTY_INFO propertyInfo;
    ULONG index;

    wprintf(L"\nTDH EVENT_PROPERTY_INFO\n");
    wprintf(L"ProviderGuid:");
    PrintGuid(&buffer->ProviderGuid);
    wprintf(L"EventGuid:");
    PrintGuid(&buffer->EventGuid);

    wprintf(L"Id: %u\n", buffer->EventDescriptor.Id);
    wprintf(L"Version: %u\n", buffer->EventDescriptor.Version);
    wprintf(L"Channel: %u\n", buffer->EventDescriptor.Channel);
    wprintf(L"Level: %u\n", buffer->EventDescriptor.Level);
    wprintf(L"Opcode: %u\n", buffer->EventDescriptor.Opcode);
    wprintf(L"Task: %u\n", buffer->EventDescriptor.Task);
    wprintf(L"Keyword: %lu\n", buffer->EventDescriptor.Keyword);
    wprintf(L"DecodingSource: %d\n", buffer->DecodingSource);

    if (0 != buffer->ProviderNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->ProviderNameOffset);
        wprintf(L"ProviderName: %ws\n", stringBuffer);
    }

    if (0 != buffer->LevelNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->LevelNameOffset);
        wprintf(L"LevelName: %ws\n", stringBuffer);
    }

    if (0 != buffer->ChannelNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->ChannelNameOffset);
        wprintf(L"ChannelName: %ws\n", stringBuffer);
    }

    if (0 != buffer->KeywordsNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->KeywordsNameOffset);
        wprintf(L"KeywordsName: %ws\n", stringBuffer);
    }

    if (0 != buffer->TaskNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->TaskNameOffset);
        wprintf(L"TaskName: %ws\n", stringBuffer);
    }

    if (0 != buffer->OpcodeNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->OpcodeNameOffset);
        wprintf(L"OpcodeName: %ws\n", stringBuffer);
    }

    if (0 != buffer->BinaryXMLOffset) {
        wprintf(L"BinaryXMLOffset: %u\n", buffer->BinaryXMLOffset);
        wprintf(L"BinaryXMLSize: %u\n", buffer->BinaryXMLSize);
    }

    if (0 != buffer->ActivityIDNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->ActivityIDNameOffset);
        wprintf(L"ActivityIDName: %ws\n", stringBuffer);
    }

    if (0 != buffer->RelatedActivityIDNameOffset) {
        stringBuffer = (LPWSTR)((PBYTE)buffer + buffer->RelatedActivityIDNameOffset);
        wprintf(L"RelatedActivityIDName: %ws\n", stringBuffer);
    }

    wprintf(L"PropertyCount: %u\n", buffer->PropertyCount);
    wprintf(L"TopLevelPropertyCount: %u\n", buffer->TopLevelPropertyCount);
    wprintf(L"Flags: %u\n", buffer->Flags);
    
    for (index = 0; index < buffer->TopLevelPropertyCount; index++) {
        propertyInfo = &buffer->EventPropertyInfoArray[index];
        PrintEventPropertyInfo(eventRecord, buffer, propertyInfo, NULL);
    }
}

void
QueryProviderFieldInformation(
    LPGUID guidPointer,
    EVENT_FIELD_TYPE eventFieldType,
    ULONGLONG enabledInfo
    )
{
    ULONG errorCode = ERROR_SUCCESS;
    ULONG size;
    PPROVIDER_FIELD_INFOARRAY buffer;

    buffer = NULL;
    size = 0;
    errorCode = TdhQueryProviderFieldInformation(guidPointer, enabledInfo, eventFieldType, buffer, &size);
    do {
        if (ERROR_INSUFFICIENT_BUFFER == errorCode) {
            if (buffer != NULL) {
                G_FREE(buffer);
            }
            buffer = (PPROVIDER_FIELD_INFOARRAY)G_ALLOC(size);
            if (!buffer) {
                return;
            }

            errorCode = TdhQueryProviderFieldInformation(guidPointer, enabledInfo, eventFieldType, buffer, &size);
        }
        else if (ERROR_SUCCESS == errorCode) {
            // Success
            break;
        }
        else if (ERROR_NOT_FOUND == errorCode) {
            break;
        }
        else {
            wprintf(L"TdhQueryProviderFieldInformation with error code %d\n", errorCode);
            break;
        }
    }  while (TRUE);

    if (buffer != NULL) {
        G_FREE(buffer);
    }
}

//
//
// Routine Description:
//
//    Callback method for processing an event. 
//
// Arguments:
//
//    eventRecord - Pointer to an event.
//
// Return Value:
//
//    None.
void
WINAPI
DumpEvent(
    PEVENT_RECORD eventRecord
	)
{
    ULONG status;
    PTRACE_EVENT_INFO traceEventInfo = NULL;
    ULONG bufferSize;
    BYTE tdhBuffer[MAX_BUFFER];

    traceEventInfo = (PTRACE_EVENT_INFO)&tdhBuffer[0];
    bufferSize = MAX_BUFFER;
    status = TdhGetEventInformation(eventRecord, 0, NULL, traceEventInfo, &bufferSize);
    
    if (ERROR_INSUFFICIENT_BUFFER == status) {
        traceEventInfo = (PTRACE_EVENT_INFO)G_ALLOC(bufferSize);
        if (NULL == traceEventInfo) {
            wprintf(L"DumpEvent: Out of Memory error\n");       
            goto cleanup;
        }

        status = TdhGetEventInformation(eventRecord, 0, NULL, traceEventInfo, &bufferSize);

        if (ERROR_SUCCESS != status) {
            wprintf(L"Error: TdhGetEventInformation with error code =0x%x\n", status);
            goto cleanup;
        }
    }
    else if (ERROR_SUCCESS != status) {
        // Other errors
        wprintf(L"Error: TdhGetEventInformation with error =0x%x\n", status);
        goto cleanup;
    }

    // Print out event information
    PrintTraceEventInfo(eventRecord, traceEventInfo, bufferSize);

   //
   // Query metadata (provider fields) - we only query EventLevelInformation here
   //
   QueryProviderFieldInformation(
        &eventRecord->EventHeader.ProviderId, 
        EventLevelInformation, 
        eventRecord->EventHeader.EventDescriptor.Level
        );
    
cleanup:

    if (traceEventInfo && traceEventInfo != (PTRACE_EVENT_INFO)&tdhBuffer[0]) {
         G_FREE(traceEventInfo);
    }
}

void
EnumerateProviderFieldInformation(
    LPGUID guidPointer,
    EVENT_FIELD_TYPE eventFieldType
    )
{
    ULONG errorCode = ERROR_SUCCESS;
    ULONG size;
    PPROVIDER_FIELD_INFOARRAY buffer;

    buffer = NULL;
    size = 0;
    errorCode = TdhEnumerateProviderFieldInformation(guidPointer, eventFieldType, buffer, &size);
    do {
        if (ERROR_INSUFFICIENT_BUFFER == errorCode) {
            if (buffer != NULL) {
                G_FREE(buffer);
            }
            buffer = (PPROVIDER_FIELD_INFOARRAY)G_ALLOC(size);
            if (!buffer) {
                return;
            }
            errorCode = TdhEnumerateProviderFieldInformation(guidPointer, eventFieldType, buffer, &size);
        }
        else if (ERROR_SUCCESS == errorCode) {
            // Success
            break;
        }
        else if (ERROR_NOT_FOUND == errorCode) {
            break;
        }
        else {
            wprintf(L"EnumerateProviderFieldInformation with error code %d\n", errorCode);
            break;
        }
    }  while (TRUE);

    if (buffer != NULL) {
        G_FREE(buffer);
    }
}


void
PrintTraceProviderInfo(
    PBYTE buffer,
    PTRACE_PROVIDER_INFO traceProviderInfo
    )
{
    PWCHAR stringBuffer;
    LPGUID guidPointer = &traceProviderInfo->ProviderGuid;
    ULONG i;

    wprintf(L"\n--- TRACE_PROVIDER_INFO ---\n");
    wprintf(L"ProviderGuid:");
    PrintGuid(guidPointer);

    wprintf(L"SchemaSource: %u\n", traceProviderInfo->SchemaSource);

    if (0 != traceProviderInfo->ProviderNameOffset) {
        stringBuffer = (PWCHAR)(buffer + traceProviderInfo->ProviderNameOffset);
        wprintf(L"ProviderName: %ls\n", stringBuffer);
    }

    for (i = EventKeywordInformation; i <= EventChannelInformation; i++) {
        EnumerateProviderFieldInformation(guidPointer, (EVENT_FIELD_TYPE)i);
    }
}

void
PrintProviderEnumeration(
    PPROVIDER_ENUMERATION_INFO buffer
    )
{
    ULONG i;
    PTRACE_PROVIDER_INFO traceProviderInfo;

    for (i = 0; i < buffer->NumberOfProviders; i++) {
        traceProviderInfo = &buffer->TraceProviderInfoArray[i];
        PrintTraceProviderInfo((PBYTE)buffer, traceProviderInfo);
    }
}

ULONG
EnumerateProviders(
    )
{
    ULONG errorCode = ERROR_SUCCESS;
    PROVIDER_ENUMERATION_INFO providerEnumerationInfo;
    ULONG size;
    PPROVIDER_ENUMERATION_INFO buffer;

    buffer = &providerEnumerationInfo;
    size = sizeof(providerEnumerationInfo);
    errorCode = TdhEnumerateProviders(buffer, &size);
    do {
        if (ERROR_INSUFFICIENT_BUFFER == errorCode) {
            if (buffer != &providerEnumerationInfo) {
                G_FREE(buffer);
            }
            buffer = (PPROVIDER_ENUMERATION_INFO)G_ALLOC(size);
            if (NULL == buffer) {
                return ERROR_NOT_ENOUGH_MEMORY;
            }
            errorCode = TdhEnumerateProviders(buffer, &size);
        }
        else if (ERROR_SUCCESS == errorCode) {
            PrintProviderEnumeration(buffer);
            break;
        }
        else {
            wprintf(L"TdhEnumerateProviders with error code %d\n", errorCode);
            break;
        }
    }  while (TRUE);

    if (buffer != &providerEnumerationInfo) {
        G_FREE(buffer);
    }

    return errorCode;
}


int 
__cdecl 
wmain(
    int argc, 
    __in_ecount(argc) LPWSTR argv[]
    )
{
    LPWSTR etlFileName = NULL;
    ULONG status = ERROR_SUCCESS;
    EVENT_TRACE_LOGFILE logFile;
    TRACEHANDLE logHandle;

    if (argc != 2) {
        Usage();
    }
    etlFileName = argv[1];

    RtlZeroMemory(&logFile, sizeof(EVENT_TRACE_LOGFILE));
    logFile.LoggerName = NULL;
    logFile.LogFileName = etlFileName;

    if (NULL == etlFileName) {
        goto cleanup;
    }

    // If the log files is not readable, exit.
    if (!CheckFile(logFile.LogFileName)) {
        wprintf(L"Cannot open logfile for reading\n");
        status = ERROR_INVALID_PARAMETER;
        goto cleanup;
    }

    logFile.ProcessTraceMode |= PROCESS_TRACE_MODE_EVENT_RECORD;
    logFile.BufferCallback = BufferCallback;
    logFile.EventRecordCallback = &DumpEvent;

    // OpenTrace calls
    logHandle = OpenTrace(&logFile);
    if (0 == logHandle) {
        status = GetLastError();
        wprintf(L"Error Opening Trace with status=%d\n", status);
        CloseTrace(logHandle);
        goto cleanup;
    }

    // Actual processing takes place here. EventCallback function will be invoked for each event.
    status = ProcessTrace(
        &logHandle,
        1,
        NULL,
        NULL
        );

    if (status != ERROR_SUCCESS) {
        wprintf(L"Error processing with status=%dL (GetLastError=0x%x)\n", status, GetLastError());
    }

    status = CloseTrace(logHandle);
    if (status != ERROR_SUCCESS) {
        wprintf(L"Error Closing Trace with status=%d\n", status);
    }

    status = EnumerateProviders();
    if (status != ERROR_SUCCESS) {
        wprintf(L"Error enumerating providers with status=%d\n", status);
    }
    
cleanup:
    
    SetLastError(status);
    if(status != ERROR_SUCCESS ) {
        wprintf(L"Exit status: %d\n", status);
    }
    
    return status;
}

