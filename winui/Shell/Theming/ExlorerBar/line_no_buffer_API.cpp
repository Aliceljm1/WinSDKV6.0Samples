// line_no_buffer_API.cpp : Defines the entry point for the application without bufferpaint.
//

#include "stdafx.h"
#include <d3d9.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <vssym32.h>
#pragma once
#include "resource.h"


#define MAX_LOADSTRING 100
#define RECT_INCREMENT 20
#define BIT_COUNT      32

#ifndef RECTWIDTH
#define RECTWIDTH(rc)   ((rc).right-(rc).left)
#endif

#ifndef RECTHEIGHT
#define RECTHEIGHT(rc)  ((rc).bottom-(rc).top)
#endif

// Global Variables:
HINSTANCE g_hInst;                                // current instance
TCHAR     g_szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR     g_szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE   hInstance,
                       HINSTANCE   /*hPrevInstance*/,
                       __in LPTSTR /*lpCmdLine*/,
                       int         nCmdShow)
{
    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_LINE, g_szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_LINE);

    // Main message loop:
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) 
    {
        if (!TranslateAccelerator(message.hwnd, hAccelTable, &message)) 
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

    return (int)message.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_LINE);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = (LPCTSTR)IDC_LINE;
    wcex.lpszClassName  = g_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    BOOL fPassed = FALSE;
    g_hInst = hInstance; // Store instance handle in our global variable
    
    HWND hWnd = CreateWindow(g_szWindowClass, 
                             g_szTitle, 
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, 
                             0, 
                             CW_USEDEFAULT, 
                             0, 
                             NULL, 
                             NULL, 
                             hInstance, 
                             NULL);
    
    if (hWnd)
    {
        fPassed = TRUE;
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
    }   
    
    return fPassed;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;

    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 

            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
                    break;

                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;

        case WM_PAINT:
            //Lets have glass background, initialize a bitmap structure and draw some text on glass
            {
                RECT rcClient;
                GetClientRect(hWnd, &rcClient);

                MARGINS marGlassInset = {0, 0, rcClient.bottom, 0 };

                DwmExtendFrameIntoClientArea(hWnd, &marGlassInset);

                HDC hdc = BeginPaint(hWnd, &ps);

                HTHEME hTheme = OpenThemeData(NULL, L"ExplorerBar");
                if (hTheme)
                {
                    HDC hdcPaint = CreateCompatibleDC(hdc);
                    if (hdcPaint)
                    {
                        BITMAPINFO dib = { 0 };
                        dib.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
                        dib.bmiHeader.biWidth           = RECTWIDTH(rcClient);
                        dib.bmiHeader.biHeight          = -RECTHEIGHT(rcClient);
                        dib.bmiHeader.biPlanes          = 1;
                        dib.bmiHeader.biBitCount        = BIT_COUNT;
                        dib.bmiHeader.biCompression     = BI_RGB;

                        HBITMAP hbm = CreateDIBSection(hdc, &dib, DIB_RGB_COLORS, NULL, NULL, 0);
                        if (hbm)
                        {
                            HBITMAP hbmOld = (HBITMAP)SelectObject(hdcPaint, hbm);

                            // Fill with black to erase the background if needed:
                            // PatBlt(hdcPaint, 0, 0, RECTWIDTH(rc), RECTHEIGHT(rc), BLACKNESS);

                            //Lets start with some GDI text in the default font
                            DTTOPTS DttOpts = {sizeof(DTTOPTS)};
                            DttOpts.dwFlags = DTT_COMPOSITED;

                            DrawText(hdcPaint, L"This is some GDI text in the default font", -1, &rcClient, 0);

                            //Now some themed text in the default font
                            rcClient.top += RECT_INCREMENT;
                            DrawThemeTextEx(hTheme, hdcPaint, 0, 0, L"This is some text in the default font", -1, 0, &rcClient, &DttOpts);

                            rcClient.top += RECT_INCREMENT;
                            DrawThemeTextEx(hTheme, hdcPaint, EBP_HEADERBACKGROUND, 0, L"This is some text in a themed font", -1, 0, &rcClient, &DttOpts);

                            //Now lets select a font and draw some GDI text
                            LOGFONT lgFont;
                            HFONT hFontOld = NULL;
                            if (SUCCEEDED(GetThemeSysFont(hTheme, TMT_CAPTIONFONT, &lgFont)))
                            {
                                HFONT hFont = CreateFontIndirect(&lgFont);
                                hFontOld = (HFONT) SelectObject(hdcPaint, hFont);
                            }

                            rcClient.top += RECT_INCREMENT;
                            DrawText(hdcPaint, L"This is some GDI text in the selected font", -1, &rcClient, 0);

                            //Some themed text in the selected font
                            rcClient.top += RECT_INCREMENT;
                            DrawThemeTextEx(hTheme, hdcPaint, 0, 0, L"This is some text in the selected font", -1, 0, &rcClient, &DttOpts);

                            BitBlt(hdc, 0, 0, RECTWIDTH(rcClient), RECTHEIGHT(rcClient), hdcPaint, 0, 0, SRCCOPY);

                            SelectObject(hdcPaint, hbmOld);
                            if (hFontOld)
                            {
                                SelectObject(hdcPaint, hFontOld);
                            }
                            DeleteObject(hbm);
                        }
                        DeleteDC(hdcPaint);
                    }
                    CloseThemeData(hTheme);
                }
                EndPaint(hWnd, &ps);
            }
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/)
{
    switch (message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;
}
