#include <stdafx.h>

//
//  Prototype -- Helper function to create a window
//
VOID CreateSimpleWindow(__in PTSTR windowTitle);

//-----------------------------------------------------------------------------
//
//  Sample to show an app declaring itself DPI-aware
//
//-----------------------------------------------------------------------------
INT _cdecl _tmain(INT argc, __in_ecount(argc) TCHAR* argv[])
{
    //
    //  Window will be scaled by the window manager to match DPI
    //
    CreateSimpleWindow(_T("Not DPI Aware"));

    //
    //  Attempt to specify that this process is DPI-aware 
    //  (i.e. will do its own scaling)
    //
    if (SetProcessDPIAware())
    {
        _tprintf(_T("Process successfully declared DPI-aware.\n"));
    }
    else
    {
        _tprintf(_T("Process was NOT declared DPI-aware.\n"));
    }

    //
    //  Window will not be scaled to match DPI
    //
    CreateSimpleWindow(_T("DPI Aware"));
    
    return 0;
}

//-----------------------------------------------------------------------------
//
//  Create a simple window. Window will be scaled by the window manager if the
//  process has not yet been set High-DPI aware, otherwise it will not be
//  scaled.
//
//-----------------------------------------------------------------------------
VOID CreateSimpleWindow(__in PTSTR windowTitle)
{
    HWND hWindow = NULL;

    hWindow = CreateWindow(
        _T("Edit"),
        windowTitle,
        WS_VISIBLE | WS_OVERLAPPED,
        100,
        100,
        200,
        200,
        NULL,
        NULL,
        NULL,
        NULL);

    if (hWindow == NULL)
    {
        _tprintf(_T("Could not create simple window\n"));
    }

    //
    //  Pause to show window
    //
    Sleep(1500);
}