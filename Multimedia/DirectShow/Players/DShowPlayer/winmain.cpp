//////////////////////////////////////////////////////////////////////////
// winmain.cpp : Defines the entry point for the application.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainWindow.h"

#define MAX_LOADSTRING 100




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)	
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;


	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DSHOWPLAYER));


	MainWindow *pWin = new MainWindow();

	HRESULT hr;
	
	hr = CoInitialize(NULL);

	hr = pWin->Create(hInstance);

	hr = pWin->Show(nCmdShow);


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete pWin;

	CoUninitialize();

	return (int) msg.wParam;
}


