// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// a wrapper for commands needed to control the media player

#ifndef MPCONTROLLER_H
#define MPCONTROLLER_H

#include "wmpids.h"
#include "wmp.h"
//#include <mshtml.h>
#import <mshtml.tlb> auto_rename

class MPController
{
public:
    MPController() { m_dwAdviseCookie = 0; }
    bool init(_ATL_OBJMAP_ENTRY* objMap, HINSTANCE hInst, HWND hMainWnd);
    void unload();
    void terminate();
    void OnSize(const RECT& clientRect);
    void OnContextMenu(UINT message, WPARAM wParam, LPARAM lParam);
    void PlayFile(LPCWSTR filePath, LANGID languageID,BOOL bPLayIt);
    void GetSAMILang(LPCTSTR smiFilename, _bstr_t& samiLang);
    void ReloadMediaFile();

private:
    CComModule m_Module;
    CAxWindow m_axHostWindow;
    CComPtr<IConnectionPoint> m_spConnectionPoint;
    CComPtr<IWMPPlayer> m_spWMPPlayer;
    DWORD m_dwAdviseCookie;

};

#endif