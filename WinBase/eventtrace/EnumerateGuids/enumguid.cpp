/*++

Copyright (c) Microsoft Corporation. All rights reserved

Abstract:

    Enumerates Trace GUIDs using ETW APIs EnumerateTraceGuids and 
    EnumerateTraceGuidsEx

Environment:

    User mode

--*/
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <wmistr.h>

#include <guiddef.h>
#include <evntrace.h>
#include "strsafe.h"


#define ACTION_UNDEFINED                0
#define ACTION_HELP                     1
#define ACTION_ENUM_GUID                2
#define ACTION_ENUM_GUID_EX             3

void PrintHelpMessage()
/*++

Routine Description:

    prints out a help message.

Arguments:

    None.

Return Value:

    None.


--*/
{

    wprintf(L"Enumerate Trace GUIDs Sample\n\n");

    wprintf(L"Usage: enumguid [actions] [options] | [-h | -help | -?]\n");
    wprintf(L"\n    actions:\n");
    wprintf(L"\t-enumguid               Enumerate Registered Trace Guids\n");
    wprintf(L"\t-enumguidex   [#<guid>] Enumerate Registered Trace Guids\n");
    wprintf(L"\n    options:\n");
    wprintf(L"\t      #<guid>           provider guid\n");
    wprintf(L"\n");
    wprintf(L"\t-h\n");
    wprintf(L"\t-help\n");
    wprintf(L"\t-?                      Display usage information\n");
    
}

ULONG 
ahextoi(
    __in PWSTR Buffer
    )
/*++

Routine Description:

    Converts a hex string into a number.

Arguments:

    Buffer - A hex string in TCHAR. 

Return Value:

    ULONG - The number in the string.


--*/
{
    int Length;
    ULONG Num, Base, Hex;

    Length = (int)wcslen(Buffer);
    Hex = 0; Base = 1; Num = 0;
    while (--Length >= 0) {
        if ( (Buffer[Length] == 'x' || Buffer[Length] == 'X') &&
             (Buffer[Length-1] == '0') )
            break;
        if (Buffer[Length] >= '0' && Buffer[Length] <= '9'){
            Num = Buffer[Length] - '0';
        } else if (Buffer[Length] >= 'a' && Buffer[Length] <= 'f') {
            Num = (Buffer[Length] - 'a') + 10;
        } else if (Buffer[Length] >= 'A' && Buffer[Length] <= 'F') {
            Num = (Buffer[Length] - 'A') + 10;
        } else {
            continue;
        }

        Hex += Num * Base;
        Base = Base * 16;
    }
    return Hex;
}

void 
StringToGuid(
    __in PCWSTR Str,
    __out LPGUID Guid
)
/*++

Routine Description:

    Converts a string into a GUID.

Arguments:

    Str - A string in WCHAR.
    Guid - The pointer to a GUID that will have the converted GUID.

Return Value:

    None.


--*/
{
    WCHAR Temp[10];
    int i;


    RtlCopyMemory(Temp, Str, 8*sizeof(WCHAR));
    Temp[8] = 0;    
    Guid->Data1 = ahextoi(Temp);
    
    RtlCopyMemory(Temp, &Str[9], 4*sizeof(WCHAR));
    Temp[4] = 0;
    Guid->Data2 = (USHORT) ahextoi(Temp);
    
    RtlCopyMemory(Temp, &Str[14], 4*sizeof(WCHAR));
    Temp[4] = 0;
    Guid->Data3 = (USHORT) ahextoi(Temp);

    for (i=0; i<2; i++) {
        RtlCopyMemory(Temp, &Str[19 + (i*2)], 2*sizeof(WCHAR));
        Temp[2] = 0;
        Guid->Data4[i] = (UCHAR) ahextoi(Temp);
    }
    
    for (i=2; i<8; i++) {
        RtlCopyMemory(Temp, &Str[20 + (i*2)], 2*sizeof(WCHAR));
        Temp[2] = 0;
        Guid->Data4[i] = (UCHAR) ahextoi(Temp);
    }
 }

PWCHAR 
GuidToString(
    __out_ecount(cch) PWSTR Buffer,
    __in ULONG Length,
    __in LPGUID Id
    )
/*++

Routine Description:

    Converts a GUID into a string.

Arguments:

    s - A string in TCHAR that will have the converted GUID.
    Id - The pointer to a GUID.

Return Value:

    PTSTR - The string containig the convereted GUID.


--*/
{
    StringCchPrintfW(Buffer, Length, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
               Id->Data1, Id->Data2,
               Id->Data3,
               Id->Data4[0], Id->Data4[1],
               Id->Data4[2], Id->Data4[3],
               Id->Data4[4], Id->Data4[5],
               Id->Data4[6], Id->Data4[7]);
    return(Buffer);
}

int __cdecl main(int Argc, 
		   __in_ecount(Argc) LPWSTR Argv[])
{
    ULONG Status = 0;
    USHORT Action = ACTION_UNDEFINED;
    GUID SourceGuid;
    BOOLEAN GuidFound = FALSE;
    LPWSTR *Targv,  *CmdArgv;
    BOOL EnumerateSelfGuid = FALSE;
    ULONG i,j;


    if ((CmdArgv = CommandLineToArgvW(
                                     GetCommandLineW(),
                                     &Argc
                                     )) == NULL) {
        return(GetLastError());
    }
    Targv = CmdArgv;

    while (--Argc > 0) {
        ++Targv;
        if (**Targv == L'-' || **Targv == L'/') {
			//
			// Argument Found
			//
			
			if(Targv[0][0] == L'/' ) {
				Targv[0][0] = L'-';
			}

            if (!_wcsicmp(Targv[0], L"-enumguid")) {
                Action = ACTION_ENUM_GUID;
            } else if (!_wcsicmp(Targv[0], L"-enumguidex")) {
                Action = ACTION_ENUM_GUID_EX;
                if (Argc > 1) {
                    if (Targv[1][0] == L'#') {
                        StringToGuid(&Targv[1][1], &SourceGuid);
                        GuidFound = TRUE;
                    } else if (Targv[1][0] == L'.') {
                        EnumerateSelfGuid = TRUE;
                    }
                    ++Targv; --Argc;
                }                
            } else if ( Targv[0][1] == L'h' || Targv[0][1] == L'H' || Targv[0][1] == L'?'){
                Action = ACTION_HELP;
                PrintHelpMessage();
                return 0;
            }
        }
        else { // get here if "-" or "/" given
            wprintf(L"Invalid option given: %s\n", Targv[0]);
            Status = ERROR_INVALID_PARAMETER;
            return Status;
        }
    }

    switch (Action) {

    case ACTION_ENUM_GUID_EX:
    {
        PBYTE GuidBuffer = NULL;
        LPGUID Guids;
        ULONG ReturnLength = 0;
        PTRACE_GUID_INFO TraceGuidInfo;
        PTRACE_PROVIDER_INSTANCE_INFO ProviderInstance;
        WCHAR GuidString[128];
        
        // 
        // EnumerateTraceGuidsEx is not available pre - Windows Vista
        //

        if ( GuidFound ){
            // 
            // Get the buffer length required to store the GUID info
            //

            Status = EnumerateTraceGuidsEx( TraceGuidQueryInfo, &SourceGuid, sizeof(GUID), NULL, 0, &ReturnLength);
            
            if ( Status == ERROR_INSUFFICIENT_BUFFER) {

                GuidBuffer = (PBYTE) malloc(ReturnLength);

                if (GuidBuffer == NULL) {
                    Status = GetLastError();
                    wprintf(L"TraceLog Failed to allocate buffer for EnumerateTraceGuidsEx %d\n",Status );
                    goto CleanupExit;
                }
                
            } else {
            
                wprintf(L"EnumerateTraceGuidsEx Failed  %d\n", Status );
                goto CleanupExit;
                
            }
            
            Status = EnumerateTraceGuidsEx(
                                    TraceGuidQueryInfo,
                                    &SourceGuid,
                                    sizeof(GUID),
                                    GuidBuffer,
                                    ReturnLength,
                                    &ReturnLength);
            if (Status != ERROR_SUCCESS) {
                wprintf(L"EnumerateTraceGuidsEx Failed  %d\n", Status);
                free(GuidBuffer);
                goto CleanupExit;
            }
            
            TraceGuidInfo = (PTRACE_GUID_INFO)GuidBuffer;
            wprintf(L"GUID = %s  \nInstance Count = %d\n", 
				    GuidToString(GuidString, ARRAYSIZE(GuidString), 
                    &SourceGuid), TraceGuidInfo->InstanceCount );

            if(TraceGuidInfo->InstanceCount > 0) {
                wprintf(L"----------------------------------------------------------------\n");
                ProviderInstance = (PTRACE_PROVIDER_INSTANCE_INFO)(GuidBuffer + sizeof(TRACE_GUID_INFO));
                j = 1;
                while (TRUE){
                    
                    if(ProviderInstance->Pid != 0) {                        
                        
                        wprintf(L"[Instance %-4d: User Mode %6s] Pid = %-8d %s\n", 
                                 j,
                                 (ProviderInstance->Flags & TRACE_PROVIDER_FLAG_LEGACY) ? L"Legacy" : L" ",
                                 ProviderInstance->Pid,
                                 (ProviderInstance->EnableCount > 0) ? L" " : L"Not Enabled"
                                 );
                        
                    } else if (TRACE_PROVIDER_FLAG_PRE_ENABLE & ProviderInstance->Flags){
                    
                        wprintf(L"[Instance %-4d: Pre-Enabled %6s]\n",
                                 j,
                                 (ProviderInstance->Flags & TRACE_PROVIDER_FLAG_LEGACY) ? L"Legacy" : L" "
                                 );
                        
                    } else {
                    
                        wprintf(L"[Instance %-4d: Driver %6s]   %s\n",
                                 j,
                                 (ProviderInstance->Flags & TRACE_PROVIDER_FLAG_LEGACY) ? L"Legacy" : L" ",
                                 (ProviderInstance->EnableCount > 0) ? L" " : L"Not Enabled"
                                 );
                        
                    }
                    j++;

                    if ( ProviderInstance->EnableCount > 0 ) {
                        PTRACE_ENABLE_INFO EnableInfo = (PTRACE_ENABLE_INFO)((PBYTE)ProviderInstance + sizeof(TRACE_PROVIDER_INSTANCE_INFO));

                        wprintf(L"---------------------- Enable Information ----------------------\n");
                        wprintf(L"Session  Level  Property   MatchAny           MatchAll\n");
                        wprintf(L"ID                         Keyword            Keyword\n");
                        wprintf(L"----------------------------------------------------------------\n");

                        for ( i =0; i < ProviderInstance->EnableCount; i++ ) {
                            wprintf(L"%-8d %5d  0x%08X 0x%016I64X 0x%016I64X\n",
                                                 EnableInfo->LoggerId,
                                                 EnableInfo->Level,
                                                 EnableInfo->EnableProperty,
                                                 EnableInfo->MatchAnyKeyword,
                                                 EnableInfo->MatchAllKeyword
                                                 );
                            EnableInfo ++;
                        }
                         wprintf(L"\n");
                    }

                    if (ProviderInstance->NextOffset ==  0) {
                        break;
                    }
                    ProviderInstance = (PTRACE_PROVIDER_INSTANCE_INFO)((PBYTE)ProviderInstance + ProviderInstance->NextOffset);
                }
            }

        } else {
            // 
            // Get the buffer length required to store the list of GUIDs
            //
            TRACE_QUERY_INFO_CLASS InfoClass = TraceGuidQueryList;

            if ( EnumerateSelfGuid ) {
                InfoClass = TraceGuidQueryProcess;
            }

            Status = EnumerateTraceGuidsEx( InfoClass, NULL, 0, NULL, 0, &ReturnLength);
            
            if ( Status == ERROR_INSUFFICIENT_BUFFER) {

                GuidBuffer = (PBYTE) malloc(ReturnLength);

                if (GuidBuffer == NULL) {
                    Status = GetLastError();
                    wprintf(L"TraceLog Failed to allocate buffer for EnumerateTraceGuidsEx %d\n",Status );
                    goto CleanupExit;
                }
                
            } else {
            
                wprintf(L"EnumerateTraceGuidsEx Failed  %d\n", Status );
                goto CleanupExit;
                
            }

            // 
            // Now Get the list of GUIDs
            //
            
            Status = EnumerateTraceGuidsEx(
                                    InfoClass,
                                    NULL,
                                    0,
                                    GuidBuffer,
                                    ReturnLength,
                                    &ReturnLength);
            
            if ( Status != ERROR_SUCCESS) {

                wprintf(L"EnumerateTraceGuidsEx Failed  %d\n", Status);
                free(GuidBuffer);
                goto CleanupExit;
            }

            Guids = (LPGUID)GuidBuffer;
            wprintf(L"GUID %s\n", EnumerateSelfGuid ? L"Registerd by this Process" : L" ");
            wprintf(L"------------------------------------------------------------\n");
            for (i=0; i < (ReturnLength / sizeof(GUID)); i++) {
                wprintf(L"%s \n", GuidToString(GuidString, ARRAYSIZE(GuidString), Guids++) );
            }
            wprintf(L"------------------------------------------------------------\n");
            wprintf(L"\n    ETW Tracing Guid Count = %d            \n", (ReturnLength / sizeof(GUID)));

        }

        if (GuidBuffer != NULL) {
            free(GuidBuffer);
        }

        break;
    }
    
    case ACTION_ENUM_GUID:
    {
        ULONG PropertyArrayCount=10;
        PTRACE_GUID_PROPERTIES  *GuidPropertiesArray;
        ULONG EnumGuidCount;
        ULONG SizeStorage;
        PVOID StorageNeeded;
        PTRACE_GUID_PROPERTIES CleanStorage;
        WCHAR Str[128];

        // 
        // EnumTraceGuids is not implemented on Win2K
        // 

Retry:
        SizeStorage = PropertyArrayCount * (sizeof(TRACE_GUID_PROPERTIES) + sizeof(PTRACE_GUID_PROPERTIES));
        StorageNeeded =  malloc(SizeStorage);
        if (StorageNeeded== NULL) {
            Status = ERROR_OUTOFMEMORY;
            break;
        }
        RtlZeroMemory(StorageNeeded, SizeStorage);
        GuidPropertiesArray = (PTRACE_GUID_PROPERTIES *)StorageNeeded;
        CleanStorage = (PTRACE_GUID_PROPERTIES)((char*)StorageNeeded + PropertyArrayCount * sizeof(PTRACE_GUID_PROPERTIES));
        for (i=0; i < PropertyArrayCount; i++) {
            GuidPropertiesArray[i] = CleanStorage;
            CleanStorage = (PTRACE_GUID_PROPERTIES) (
                                (char*)CleanStorage + sizeof(TRACE_GUID_PROPERTIES)
                                );
        }

		Status = EnumerateTraceGuids(GuidPropertiesArray,PropertyArrayCount,&EnumGuidCount);
        if(Status == ERROR_MORE_DATA)
        {
            PropertyArrayCount=EnumGuidCount;
            free(StorageNeeded);
            goto Retry;

        }

        //
        // print the GUID_PROPERTIES and Free Strorage
        //

        wprintf(L"    Guid                     Enabled  LoggerId Level Flags\n");
        wprintf(L"------------------------------------------------------------\n");
        for (i=0; i < EnumGuidCount; i++) {
            wprintf(L"%s     %5s  %d    %d    %d\n",
                                       GuidToString(Str, ARRAYSIZE(Str),
                                       &GuidPropertiesArray[i]->Guid), 
                                       (GuidPropertiesArray[i]->IsEnable) ? L"TRUE" : L"FALSE",
                                       GuidPropertiesArray[i]->LoggerId,
                                       GuidPropertiesArray[i]->EnableLevel,
                                       GuidPropertiesArray[i]->EnableFlags 
                                                           );
        }
        free(StorageNeeded);
    }
    break;

    case ACTION_UNDEFINED:
    case ACTION_HELP:
        PrintHelpMessage();
        break;
	}
        
CleanupExit:
    
    GlobalFree(CmdArgv);

	return 0;
}

