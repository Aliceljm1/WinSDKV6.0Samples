/*************************************************************************************************
 *
 * File: Window.cpp
 *
 * Description: Entry point for a sample application that displays a dialog box containing
 * a custom contol that supports UI Automation.
 *
 * The control is a simple button-like control that supports InvokePattern. Clicking the 
 * button causes it to change color. You can also tab to the button and click it by pressing
 * the spacebar.
 * 
 * To test the functionality of InvokePattern, you can use the UISpy tool. Click on the control
 * in the UI Automation raw view or control view and then select Control Patterns from the
 * View menu. In the Control Patterns dialog box, you can call the InvokePattern::Invoke method.
 * 
 * 
 *  Copyright (C) Microsoft Corporation.  All rights reserved.
 * 
 * This source code is intended only as a supplement to Microsoft
 * Development Tools and/or on-line documentation.  See these other
 * materials for detailed information regarding Microsoft code samples.
 * 
 * THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 * 
 *************************************************************************************************/

#pragma once

#include "resource.h"
#include "stdafx.h"
#include "Control.h"
#include <ole2.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // Current instance.

// Forward declarations of functions included in this code module.
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

// Entry point.
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hInst = hInstance; 
    CoInitialize(NULL);

    // Register the window class for the CustomButton control.
    CustomButton::RegisterControl(hInstance);

    DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);

    CoUninitialize();
    
    return 0;
}


// Message handler for application dialog.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            PostQuitMessage(0);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

