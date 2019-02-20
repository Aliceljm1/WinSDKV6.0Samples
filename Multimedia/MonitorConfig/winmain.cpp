//------------------------------------------------------------------------------
//
// File: winmain.cpp
// Description: Entry point for the application.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//------------------------------------------------------------------------------

#include "MainDialog.h"
#include "trace.h"

HINSTANCE g_hInstance;


int WINAPI WinMain(HINSTANCE  hInstance,
                     HINSTANCE  /*hPrevInstance*/,
                     LPSTR     lpstrCmdLine,
                     int        nCmdShow)
{

    g_hInstance = hInstance;

    // Initialize the common control library.
    INITCOMMONCONTROLSEX ix;
    ix.dwSize = sizeof(ix);
    ix.dwICC = ICC_BAR_CLASSES;
    if (!InitCommonControlsEx(&ix))
    {
        MessageBox(NULL, TEXT("InitCommonControlsEx failed."), NULL, MB_ICONSTOP);
        return 1;
    }

    TRACE_INIT(L"log.txt");


    // Create the dialog.
    MainDialog *pDlg = new MainDialog();
	if (pDlg == NULL)
	{
        MessageBox(NULL, TEXT("Out of memory."), NULL, MB_ICONSTOP);
	}
	else
	{
		pDlg->ShowDialog(hInstance, NULL);

		delete pDlg;
	}


    TRACE_CLOSE();

    return 0;
}