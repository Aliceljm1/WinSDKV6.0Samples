// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include "strsafe.h"
#include "CWMPEventDispatch.h"
#include "MPController.h"
#include "CCWindow.h"
#include "GlobalConfig.h"

extern  CCWindow* g_pCCW;
extern  GlobalConfig g_GlobalConfig;
HRESULT GetCurrentUILangName(LPTSTR szFileLangName, int cchFileLangName, bool bExtraNull = false);
CLOSEDCAPTIONLANGTAGTBL LangNameTagTbl[]=
{
    {TEXT("en-US"),TEXT("English (United States)")},
    {TEXT("fr-FR"),TEXT("French (France)")},
    {TEXT("de-DE"),TEXT("German (Germany)")},
    {TEXT("pt-BR"),TEXT("Portuguese (Brazil)")},
    {TEXT("ja-JP"),TEXT("Japanese")}
};

bool MPController::init(_ATL_OBJMAP_ENTRY* objMap, HINSTANCE hInst, HWND hMainWnd)
{
    HRESULT  hr;
    RECT rcClient;

    CComPtr<IAxWinHostWindow> spHost;
    CComPtr<IConnectionPointContainer> spConnectionContainer;
    CComWMPEventDispatch *pEventListener = NULL;
    CComPtr<IWMPEvents> spEventListener;

    m_Module.Init(objMap, hInst, &LIBID_ATLLib);
    AtlAxWinInit();

    rcClient.bottom = rcClient.left = rcClient.right = rcClient.top = 0;

    // create an ActiveX host window, which wrapps around the main window
    m_axHostWindow.Create(hMainWnd, rcClient, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

    m_axHostWindow.QueryHost(&spHost);
    spHost->CreateControl(CComBSTR(_T("{6BF52A52-394A-11d3-B153-00C04F79FAA6}")), m_axHostWindow, 0);

    m_axHostWindow.QueryControl(&m_spWMPPlayer);

    CComWMPEventDispatch::CreateInstance(&pEventListener);
    spEventListener = pEventListener;

    m_spWMPPlayer->QueryInterface(&spConnectionContainer);

    // See if OCX supports the IWMPEvents interface
    hr = spConnectionContainer->FindConnectionPoint(__uuidof(IWMPEvents), &m_spConnectionPoint);
    if (FAILED(hr))
    {
        // If not, try the _WMPOCXEvents interface, which will use IDispatch
        spConnectionContainer->FindConnectionPoint(__uuidof(_WMPOCXEvents), &m_spConnectionPoint);
    }
    m_spConnectionPoint->Advise(spEventListener, &m_dwAdviseCookie);

    m_axHostWindow.BringWindowToTop();
    m_spWMPPlayer->put_enableContextMenu(VARIANT_TRUE);
    return true;
}

void MPController::unload()
{
    // stop listening to events
    if (m_spConnectionPoint)
    {
        if (0 != m_dwAdviseCookie)
            m_spConnectionPoint->Unadvise(m_dwAdviseCookie);
        m_spConnectionPoint.Release();
    }

    // close the OCX
    if (m_spWMPPlayer)
    {
        m_spWMPPlayer->close();
        m_spWMPPlayer.Release();
    }

    m_Module.Term();
}

void MPController::terminate()
{
    // have to explicitly destroy the ActiveX window
    m_axHostWindow.DestroyWindow();
}

void MPController::OnSize(const RECT& clientRect)
{
    int left = (int)((double)clientRect.right * 0.6);
    int width = clientRect.right - left;    
    int top = (int)((double)clientRect.bottom * 0.4) + 3;    
    int height = (int)((double)clientRect.bottom * 0.35) -2;   
    m_axHostWindow.MoveWindow(left, top, width, height);
}

void MPController::OnContextMenu(UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT llResult = 0;
    CComPtr<IOleInPlaceObjectWindowless> spSite = NULL;
    HRESULT hr = m_spWMPPlayer->QueryInterface(&spSite);
    if(SUCCEEDED(hr) )
    {
        spSite->OnWindowMessage(message, wParam, lParam, &llResult);
    }
}

void MPController::ReloadMediaFile()
{
     HRESULT hr;
     BSTR    ClipName;

     hr = m_spWMPPlayer->get_URL(&ClipName);
     if( (FAILED( hr )) || (wcslen(ClipName)==0))
     {
        return;
     }

     CComPtr<IWMPControls> spControls;
     hr = m_spWMPPlayer->QueryInterface(&spControls);
     if( SUCCEEDED( hr ) )
     {
         double Seconds;
         hr = spControls->get_currentPosition(&Seconds);
         if( SUCCEEDED( hr ))
         {       
             PlayFile(ClipName,g_GlobalConfig.currDefaultLCID,(Seconds > 0)? TRUE : FALSE);
         }
         spControls.Release();
     }
}


void MPController::PlayFile(LPCTSTR file, LANGID languageID,BOOL bPLayIt)
{     
    if(GetFileAttributes(file) == INVALID_FILE_ATTRIBUTES)
        return;

    BSTR filename = SysAllocString(file);
    if(filename == NULL) return;

    TCHAR szSmiFilename[MAX_PATH];
    HRESULT hr = StringCchCopyN(szSmiFilename, MAX_PATH, file, _tcslen(file)-3);
    if(FAILED(hr)) return;
    hr = StringCchCat(szSmiFilename, MAX_PATH, TEXT("smi"));
    if(FAILED(hr)) return;

    BSTR smiFilename = SysAllocString(szSmiFilename);
    if(smiFilename == NULL) return;

    CComPtr<IWMPControls> spControls;
    CComPtr<IWMPControls3> spControls3;

    // close the previous media stream, it does not hurt
    // even if there is no media stream being played before
    //m_spWMPPlayer->close();

    m_spWMPPlayer->QueryInterface(&spControls);
    spControls->QueryInterface(&spControls3);
    
    spControls->stop();


    // specify the language for the audio stream
    spControls3->put_currentAudioLanguage(languageID);

    // now, setting up the closed caption file
    CComPtr<IWMPClosedCaption> spClosedCaption;
    m_spWMPPlayer->QueryInterface(&spClosedCaption);

    if(GetFileAttributes(smiFilename) == INVALID_FILE_ATTRIBUTES)
    {
        g_pCCW->Navigate(NAV_CC,NULL);
        spClosedCaption->put_SAMIFileName(_bstr_t(TEXT("")));
    }
    else
    {        
        spClosedCaption->put_SAMIFileName(smiFilename);             
        _bstr_t samiLang;
        GetSAMILang(smiFilename, samiLang);
        //0x411 is the language id for JA-JP
        if(languageID == 0x411)
            g_pCCW->SetJapanese(true);
        else
            g_pCCW->SetJapanese(false);
        
        hr = spClosedCaption->put_SAMILang(samiLang);
    }
   
    m_spWMPPlayer->put_URL(filename);

    

    if (!bPLayIt)
    {
       spControls->stop();         
    }

    spClosedCaption.Release();
    spControls3.Release();
    spControls.Release();

    SysFreeString(filename);
    SysFreeString(smiFilename);
}

void MPController::GetSAMILang(LPCTSTR smiFilename, _bstr_t& samiLang)
{
    int     nIdx;
    TCHAR   szDefaultLang[MAX_LOCALE_NAME_LENGTH];
    BOOL    bFound=FALSE;

    GetCurrentUILangName(szDefaultLang, MAX_LOCALE_NAME_LENGTH);

    for (nIdx=0; nIdx < (sizeof(LangNameTagTbl)/sizeof(LangNameTagTbl[0])); nIdx++)
    {
        if (!_tcsicmp(LangNameTagTbl[nIdx].CCLangTag,szDefaultLang))
        {
           samiLang = LangNameTagTbl[nIdx].CCNameTag;
           bFound = TRUE;
           break;
        }
    }
    if (!bFound)
    {
        samiLang = LangNameTagTbl[0].CCNameTag;
    }    
}