// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// a wrapper for commands needed to control IE

#ifndef IECONTROLLER_H
#define IECONTROLLER_H

#include "iectrl.h"

class IEController
{
public:
    // empty constructor, initialization is done in init()
    IEController() {};

    bool init(HWND hMainWnd);
    void unload();
    void OnSize(const RECT& clientRect);
    void Navigate(__in_ecount(MAX_PATH)LPCTSTR url);
    void Refresh();
    IHTMLDocument2* LoadDocument(__in_ecount(MAX_PATH)LPCTSTR url);

private:
    HWND m_hPanel;
    HINSTANCE m_hInstDll;
    EmbedBrowserObjectPtr* m_lpEmbedBrowserObject;
    UnEmbedBrowserObjectPtr* m_lpUnEmbedBrowserObject;
    DisplayHTMLPagePtr* m_lpDisplayHTMLPage;
    IWebBrowser2* m_IWebBrowser2;
};

#endif