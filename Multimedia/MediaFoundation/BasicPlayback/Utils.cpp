//////////////////////////////////////////////////////////////////////////
//
// Utils.cpp : Misc. helper classes.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
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

		HRESULT hr = S_OK;
		if (sFileName)
		{
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
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			else
			{
				// Redirect debug messages to the logging file.
				_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
				_CrtSetReportFile(_CRT_WARN, g_hFile);
			}
		}
		else
		{
			// No file name, send debug messages to the debug window.
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
		}
		return hr;
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

    
    //--------------------------------------------------------------------------------------
    // Name: AllocGetWindowText
    // Description: Helper function to get text from a window.
    //
    // This function allocates a buffer and returns it in pszText. The caller must
    // call CoTaskMemFree on the buffer.
    //
    // hwnd:     Handle to the window
    // pszText:  Receives a pointer to the string.
    // pcchLen:  Receives the length of the string, in characters, not including
    //           the terminating NULL character. This parameter can be NULL.
    //--------------------------------------------------------------------------------------

    HRESULT AllocGetWindowText(HWND hwnd, WCHAR **pszText, DWORD *pcchLen)
    {
        if (pszText == NULL)
        {
            return E_POINTER;
        }

        *pszText = NULL;  

        int cch = GetWindowTextLength(hwnd);  
        if (cch < 0) 
        {
            return E_UNEXPECTED; // This function should not return a negative value.
        }

        WCHAR *szTmp = (WCHAR*)CoTaskMemAlloc(sizeof(WCHAR) * (cch + 1)); // Include room for terminating NULL character

        if (!szTmp)
        {
            return E_OUTOFMEMORY;
        }

        if (cch == 0)
        {
            szTmp[0] = L'\0';  // No text.
        }
        else
        {
            int res = GetWindowText(hwnd, szTmp, (cch + 1));  // Size includes NULL character

            // GetWindowText returns 0 if (a) there is no text or (b) it failed.
            // We checked for (a) already, so 0 means failure here.
            if (res == 0)
            {
                CoTaskMemFree(szTmp);
                return __HRESULT_FROM_WIN32(GetLastError());
            }
        }

        // If we got here, szTmp is valid, so return it to the caller.
        *pszText = szTmp;
        if (pcchLen)
        {
            *pcchLen = static_cast<DWORD>(cch);  // Return the length NOT including the '\0' 
        }
        return S_OK;
    }




}  // namespace MediaFoundationSamples