//////////////////////////////////////////////////////////////////////////
//
// MF_Utils.h : Misc. helper classes.
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#pragma once


// SAFE_RELEASE macro.
// Releases a COM pointer if the pointer is not NULL, and sets the pointer to NULL.

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#endif


namespace MediaFoundationSamples
{

    //--------------------------------------------------------------------------------------
    // Debug logging functions
    // Description: Contains debug logging functions.
    //
    //     Initialize: Opens a logging file with the specified file name.
    //     Trace: Writes a sprintf-formatted string to the logging file.
    //     Close: Closes the logging file and reports any memory leaks.
    //
    // The TRACE_INIT, TRACE, and TRACE_CLOSE macros are mapped to the logging functions
    // in debug builds, and defined as nothing in retail builds.
    //--------------------------------------------------------------------------------------

    HRESULT DebugLog_Initialize(const WCHAR *sFileName);
    void    DebugLog_Trace(const WCHAR *sFormatString, ...);
    void    DebugLog_Close();

#ifdef USE_LOGGING

    #define TRACE_INIT(x) DebugLog_Initialize(x)
    #define TRACE(x) DebugLog_Trace x
    #define TRACE_CLOSE() DebugLog_Close()

    // Log HRESULTs on failure.
    #define LOG_IF_FAILED(x,hr) if (FAILED(hr)) { TRACE((L"%s hr=0x%X", x, hr)); }


#else

    #define TRACE_INIT(x) 
    #define TRACE(x) 
    #define TRACE_CLOSE() 

    #define LOG_IF_FAILED(x, hr)

#endif

#define IF_FAILED_GOTO(hr, label) { if (FAILED(hr)) { LOG_HRESULT(hr); goto label; } }


} // namespace MediaFoundationSamples