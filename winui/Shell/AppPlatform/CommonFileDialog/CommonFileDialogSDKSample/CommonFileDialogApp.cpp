//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <windows.h>    // For common windows data types and function headers
#include <objbase.h>    // For COM headers
#include "FileDialog.h" // CCommonFileDialog class is defined in this header
#include "Resource.h"   // For resources defines

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

//
// Global Variables
//
HINSTANCE           g_hInstance = NULL;               // current instance
HWND                g_hWndApp = NULL;                 // HWND of the app
TCHAR               g_szTitle[MAX_LOADSTRING];        // the title bar text
TCHAR               g_szWindowClass[MAX_LOADSTRING];  // the main window class name

//
// Forward declarations of functions used in this code module
//
ATOM                MyRegisterClass(HINSTANCE);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


//
// Application entry point
//
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int iReturn = 1;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        //
        // Initialize global strings
        //
        LoadString(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
        LoadString(hInstance, IDC_COMMONFILEDIALOGAPP, g_szWindowClass, MAX_LOADSTRING);
        
        MyRegisterClass(hInstance);
        
        //
        // Perform application initialization
        //
        if (InitInstance(hInstance, nCmdShow))
        {
            MSG msg;
            HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMMONFILEDIALOGAPP));

            //
            // Main message loop
            //
            while (GetMessage(&msg, NULL, 0, 0))
            {
                if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            iReturn = (int) msg.wParam;
        }

        CoUninitialize();
    }

    return iReturn;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex = {0};

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMMONFILEDIALOGAPP));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_COMMONFILEDIALOGAPP);
    wcex.lpszClassName  = g_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    BOOL fReturn = FALSE;
    CCommonFileDialog *pCommonFileDialog = new CCommonFileDialog();
    
    g_hInstance = hInstance;

    if (pCommonFileDialog != NULL)
    {
        g_hWndApp = CreateWindow(g_szWindowClass, 
                                 g_szTitle, 
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, 
                                 0, 
                                 CW_USEDEFAULT, 
                                 0, 
                                 NULL, 
                                 NULL, 
                                 hInstance, 
                                 pCommonFileDialog);

        if (g_hWndApp != NULL)
        {
            fReturn = TRUE;
            ShowWindow(g_hWndApp, nCmdShow);
            UpdateWindow(g_hWndApp);
        }
        else
        {
            delete pCommonFileDialog;
        }
    }

    return fReturn;
}


//
// Message handler for the main app window
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT lr = 0;
    static CCommonFileDialog *pCommonFileDialog = NULL;

    switch (message)
    {
    case WM_CREATE:
        pCommonFileDialog = (CCommonFileDialog *) ((CREATESTRUCT *) lParam)->lpCreateParams;
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            // Parse the menu selections
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            case IDM_SIMPLEOPEN:
                if (pCommonFileDialog != NULL)
                {
                    lr = pCommonFileDialog->BasicFileOpen(hWnd);
                }

                break;

            case IDM_ADDCOMMONPLACES:
                if (pCommonFileDialog)
                {
                    lr = pCommonFileDialog->AddItemsToCommonPlaces(hWnd);
                }

                break;

            case IDM_CUSTOMIZEDIALOG:
                if (pCommonFileDialog != NULL)
                {
                    lr = pCommonFileDialog->AddCustomControls(hWnd);
                }

                break;

            case IDM_CHANGEPROPERTYORDER:
                if (pCommonFileDialog != NULL)
                {
                    lr = pCommonFileDialog->SetDefaultValuesForProperties(hWnd);
                }

                break;

            case IDM_WRITEPROPERTIESUSINGHANDLERS:
                if (pCommonFileDialog != NULL)
                {
                    lr = pCommonFileDialog->WritePropertiesUsingHandlers(hWnd);
                }

                break;

            case IDM_WRITEPROPERTIESNOHANDLERS:
                if (pCommonFileDialog != NULL)
                {
                    lr = pCommonFileDialog->WritePropertiesWithoutUsingHandlers(hWnd);
                }

                break;

            default:
                lr = DefWindowProc(hWnd, message, wParam, lParam);
            }
        }

        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            
            BeginPaint(hWnd, &ps);
            
            //
            // TODO: Add any drawing code here...
            //

            EndPaint(hWnd, &ps);
        }

        break;

    case WM_DESTROY:
        if (pCommonFileDialog != NULL)
        {
            delete pCommonFileDialog;
        }

        PostQuitMessage(0);
        break;

    default:
        lr = DefWindowProc(hWnd, message, wParam, lParam);
    }

    return lr;
}


//
// Message handler for the About box
//
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    INT_PTR pReturn = (INT_PTR) FALSE;

    UNREFERENCED_PARAMETER(lParam);
    
    switch (message)
    {
    case WM_INITDIALOG:
        pReturn = (INT_PTR) TRUE;
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            pReturn = (INT_PTR) TRUE;
        }

        break;
    }

    return pReturn;
}
