// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

//
// Alarms.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AlarmClient.h"
#include "commctrl.h"


//
// Global Variables
//
CAlarmClient *g_pAlarmClient = NULL;

INT_PTR CALLBACK AlarmDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL bResult = FALSE;

    switch(msg)
    {
    case WM_INITDIALOG:
        {
            bResult = TRUE;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            {
                EndDialog(hDlg, 0);
                PostQuitMessage(0);
                bResult = TRUE;
            }
            break;

        case IDC_BUTTON_SETALARM:
            {
                HWND hWndTime = GetDlgItem(hDlg, IDC_DATETIMEPICKER_ALARM);
                SYSTEMTIME alarmTime;
                SendMessage(hWndTime, DTM_GETSYSTEMTIME, 0, (LPARAM)&alarmTime);
                if (g_pAlarmClient != NULL)
                {
                    g_pAlarmClient->SetAlarm(&alarmTime);
                }
                bResult = TRUE;
            }
            break;
        }
        break;
    }

    return bResult;
}


int __stdcall WinMain(
       __in HINSTANCE hInstance,
   __in_opt HINSTANCE hPrevInstance,
   __in_opt LPSTR     lpCmdLine,
       __in int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG     msg;
    HWND    hDlg = NULL;

    //
    // Initialize COM; we use the Apartment threading model
    // because our implemented COM objects are not thread-safe
    //
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

    g_pAlarmClient = new CAlarmClient();

    if (g_pAlarmClient == NULL)
    {
        //new failed, so abort the application.
        return ERROR_OUTOFMEMORY;
    }
    
    //
    // Register this client application with the Windows SideShow
    // platform
    //
    g_pAlarmClient->Register();
    
    //
    // Add content to the display
    //
    g_pAlarmClient->AddContent();

    //
    // Show the Alarms dialog
    //
    hDlg = ::CreateDialog(hInstance,
                          MAKEINTRESOURCE(IDD_DIALOG_ALARM), 
                          NULL, 
                          &AlarmDlgProc);

    if (NULL != hDlg)
    {
        ::ShowWindow(hDlg, SW_SHOW);

        //
        // Main message pump; run until the window
        // closes
        //
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    else
    {
        WCHAR errorMsg[256] = {0};
        StringCchPrintf(errorMsg, 256, L"CreateDialog failed with 0x%08x", HRESULT_FROM_WIN32(::GetLastError()));
        MessageBox(NULL, errorMsg, L"Error", MB_OK);
    }

    //
    // Remove all of the content from the display
    // so it's no longer available once this application
    // closes
    //
    g_pAlarmClient->RemoveAllContent();
    //
    // Unregister this client application from the platform
    //
    g_pAlarmClient->Unregister();

    // 
    // free g_pAlarmClient
    // 
    delete g_pAlarmClient;

    //
    // Finally, uninitialize COM
    //
    ::CoUninitialize();

    return 0;
}