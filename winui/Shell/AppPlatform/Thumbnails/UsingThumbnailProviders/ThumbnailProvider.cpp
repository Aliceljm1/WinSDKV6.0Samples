//////////////////////////////////////////////////////////////////
//
// ThumbnailProvider.cpp : 
//		Defines the entry point for the ThumbnailProvider 
//		application, which demonstrates the use of IThumbnailProvider
//		to retrieve a thumbnail for an image file.  
//		This application takes two command line arguments.
//		The first argument should be the name of the 
//		image file to retrieve a thumbnail for, and the second 
//		argument should be a number indicating the size of the
//		thumbnail to retrieve.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//
//////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Resource.h"

using namespace Gdiplus;

ULONG_PTR g_startupToken;           // for starting GdiPlus
HBITMAP g_hThumbnail;				// Thumbnail to create
WTS_ALPHATYPE g_alphaType;			// Indicates whether thumbnail has alpha
int g_nThumbSize = 0;				// Size of thumbnail

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    TCHAR szTitle[100];
    LoadString(hInstance, IDS_APP_TITLE, szTitle, ARRAYSIZE(szTitle));
    HWND hWnd = CreateWindow(L"ThumbnailAppClass", szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, g_nThumbSize, g_nThumbSize, NULL, NULL, hInstance, NULL);
    if (hWnd)
    {
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
    }
    return hWnd ? TRUE : FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId    = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            Graphics * pGraphics = new Graphics(hdc);
            if (pGraphics)
            {
                Gdiplus::Bitmap * pBitmap = Gdiplus::Bitmap::FromHBITMAP(g_hThumbnail, NULL);
                if (pBitmap)
                {
                    Gdiplus::Status	status = pGraphics->DrawImage(pBitmap, 0, 0);
                    delete pBitmap;
                }
                pBitmap = NULL;
                delete pGraphics;
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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex = { sizeof(wcex) };
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THUMBNAILPROVIDER));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_THUMBNAILPROVIDER);
    wcex.lpszClassName	= L"ThumbnailAppClass";
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
    Gdiplus::GdiplusStartupInput startupInput;
    Gdiplus::GdiplusStartup(&g_startupToken, &startupInput, NULL);

    int numArgs = 0;
    PWSTR *ppszArgs = CommandLineToArgvW(lpCmdLine, &numArgs);
    if (ppszArgs && (numArgs == 2))
    {
        PCWSTR pszArg1 = L"C:\\Users\\Public\\Pictures\\Sample Pictures\\Creek.jpg"; // ppszArgs[0]; 
        PCWSTR pszArg2 = ppszArgs[1];

        g_nThumbSize = _wtoi(pszArg2);

        HRESULT hr = CoInitialize(NULL);
        if (SUCCEEDED(hr))
        {
            IShellItem* pFileItem;
            hr = SHCreateItemFromParsingName(pszArg1, NULL, IID_PPV_ARGS(&pFileItem));
            if (SUCCEEDED(hr))
            {
                IThumbnailProvider *pThumbProvider;
                hr = pFileItem->BindToHandler(NULL, BHID_ThumbnailHandler, IID_PPV_ARGS(&pThumbProvider));
                if (SUCCEEDED(hr))
                {
                    hr = pThumbProvider->GetThumbnail(g_nThumbSize, &g_hThumbnail, &g_alphaType);
                    if (SUCCEEDED(hr))
                    {
                        MyRegisterClass(hInstance);

                        // Perform application initialization:
                        if (InitInstance(hInstance, nCmdShow))
                        {
                            MSG msg;
                            while (GetMessage(&msg, NULL, 0, 0))
                            {
                                TranslateMessage(&msg);
                                DispatchMessage(&msg);
                            }
                        }
                        DeleteObject(g_hThumbnail);
                    }
                    pThumbProvider->Release();
                }
                pFileItem->Release();
            }
            CoUninitialize();
        }
    } 
    else
    {
        MessageBox(NULL, L"Syntax: ThumbnailProvider.exe sourceimagefullpath thumbsize", L"Wrong # of arguments.", MB_OK);
    }

    Gdiplus::GdiplusShutdown(g_startupToken); // shut down GDIPlus

    return 0;
}

