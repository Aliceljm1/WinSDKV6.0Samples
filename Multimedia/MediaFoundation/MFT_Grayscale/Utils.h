//////////////////////////////////////////////////////////////////////////
//
// Utils.h : Misc. helper classes.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
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


// SAFE_DELETE macro.
// Deletes a pointer allocated with new.

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }
#endif

// SAFE_ARRAY_DELETE macro.
// Deletes an array allocated with new [].

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(x) if (x) { delete [] x; x = NULL; }
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif


namespace MediaFoundationSamples
{

    //////////////////////////////////////////////////////////////////////////
    //  CCritSec
    //  Description: Wraps a critical section.
    //////////////////////////////////////////////////////////////////////////

    class CCritSec
    {
    private:
        CRITICAL_SECTION m_criticalSection;
    public:
        CCritSec();
        ~CCritSec();

        void Lock();
        void Unlock();
    };


    //////////////////////////////////////////////////////////////////////////
    //  CAutoLock
    //  Description: Provides automatic locking and unlocking of a 
    //               of a critical section.
    //////////////////////////////////////////////////////////////////////////

    class CAutoLock
    {
    private:
        CCritSec *m_pCriticalSection;
    public:
        CAutoLock(CCritSec& crit);
        ~CAutoLock();
    };


#ifdef _DEBUG

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

} // namespace MediaFoundationSamples