//////////////////////////////////////////////////////////////////////////
//
// Utils.cpp : Misc. helper classes.
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "Utils.h"

#include <strsafe.h>  
#include <crtdbg.h>


namespace MediaFoundationSamples
{

    // Debug logging

    // Log file
    HANDLE g_hFile = INVALID_HANDLE_VALUE;

    //--------------------------------------------------------------------------------------
    // Name: Initialize
    // Description: Opens a logging file with the specified file name.
    //--------------------------------------------------------------------------------------

    HRESULT DebugLog_Initialize(const WCHAR *sFileName)
    {
        // Close any existing file.
        if (g_hFile != INVALID_HANDLE_VALUE)
        {
            CloseHandle(g_hFile);
            g_hFile = INVALID_HANDLE_VALUE;
        }

        // Open the new logging file.
        g_hFile = CreateFile(
            sFileName, 
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            0, 
            NULL);

        if (g_hFile == INVALID_HANDLE_VALUE)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
        else
        {
            // Redirect debug messages to the logging file.
            _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_WARN, g_hFile);

            return S_OK;
        }
    }

    //--------------------------------------------------------------------------------------
    // Name: Trace
    // Description: Writes a sprintf-formatted string to the logging file.
    //--------------------------------------------------------------------------------------

    void DebugLog_Trace(const WCHAR *sFormatString, ...)
    {
#ifdef _DEBUG

        HRESULT hr = S_OK;
        va_list va;

        const DWORD TRACE_STRING_LEN = 512;

        WCHAR message[TRACE_STRING_LEN];

        va_start(va, sFormatString);
        hr = StringCchVPrintf(message, TRACE_STRING_LEN, sFormatString, va);
        va_end(va);

        if (SUCCEEDED(hr))
        {
            _CrtDbgReport(_CRT_WARN, NULL, NULL, NULL, "%S\r\n", message);

        }
#endif
    }

    //--------------------------------------------------------------------------------------
    // Name: Close
    // Description: Closes the logging file and reports any memory leaks.
    //--------------------------------------------------------------------------------------

    void DebugLog_Close()
    {
        _CrtDumpMemoryLeaks();
    	CloseHandle(g_hFile);
    }
    

}  // namespace MediaFoundationSamples