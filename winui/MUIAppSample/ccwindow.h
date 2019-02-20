// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// a wrapper for commands needed to control the closed caption window

#ifndef CCWINDOW_H
#define CCWINDOW_H

#include "iectrl.h"

#define NAV_CC   2
#define NAV_PIC  1
class CCWindow
{
public:
    // empty constructor, initialization is done in init()
    CCWindow() {};

    bool init(HWND hMainWnd, HINSTANCE hInst);
    void unload();
    void OnSize(const RECT& clientRect);
    void Navigate(__in int nMode,__in_ecount(MAX_PATH)LPCTSTR url);
    void SetJapanese(bool bUseJapanese);
    void OnClosedCaption(BSTR text);   
    void ShowWindow(int nCmdShow);
    void ShowCaption();
    bool IsShowingBanner();
    void SetShowingBannerState(bool bSet);
    void SetBanner(__in_ecount(MAX_PATH)LPCTSTR filename);
    void ShowBanner();

private:
    HWND m_hPanel;
    HWND m_hPanel_CC;
    HINSTANCE m_hInstDll;    
    EmbedBrowserObjectPtr* m_lpEmbedBrowserObject;
    UnEmbedBrowserObjectPtr* m_lpUnEmbedBrowserObject;
    DisplayHTMLPagePtr* m_lpDisplayHTMLPage;
    DisplayHTMLStrPtr* m_lpDisplayHTMLStr;
    IWebBrowser2* m_IWebBrowser2;    
    IWebBrowser2* m_IWebBrowser2_CC;


    TCHAR m_pstrBannerFileName[MAX_PATH];
    PTCHAR m_pstrLastCaption;    
    bool m_bUseJapanese;
};

#endif