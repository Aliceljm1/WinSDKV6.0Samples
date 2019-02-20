// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include "IECOntroller.h"

bool IEController::init(HWND hMainWnd)
{
    // this is the container for the embedded IE browser control
    m_hPanel = CreateWindow(TEXT("STATIC"), NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | SS_ETCHEDFRAME,
        0, 0, 0, 0, hMainWnd, NULL, NULL, NULL);

    if ((m_hInstDll = LoadLibrary(TEXT("iectrl.dll"))))
    {
        // get the entry point to the needed procedures exported by the dll
        m_lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_hInstDll, "EmbedBrowserObject");
        m_lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_hInstDll, "UnEmbedBrowserObject");
        m_lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)m_hInstDll, "DisplayHTMLPage");

        (*m_lpEmbedBrowserObject)(m_hPanel);

        return true;
    }
    else
        return false;
}

void IEController::unload()
{
    (*m_lpUnEmbedBrowserObject)(m_hPanel);
    FreeLibrary(m_hInstDll);
}

void IEController::OnSize(const RECT& clientRect)
{
    int right = (int)((double)clientRect.right * 0.6);

    MoveWindow(m_hPanel, 0, 28, right, clientRect.bottom - 28, TRUE);

    IOleObject* browserObject = *((IOleObject**)GetWindowLong(m_hPanel, GWLP_USERDATA));
    browserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_IWebBrowser2);

    m_IWebBrowser2->put_Left(2);
    m_IWebBrowser2->put_Top(2);
    m_IWebBrowser2->put_Width(right - 6);
    m_IWebBrowser2->put_Height(clientRect.bottom - 34);
    m_IWebBrowser2->Release();
}

void IEController::Navigate(__in_ecount(MAX_PATH)LPCTSTR url)
{
    (*m_lpDisplayHTMLPage)(m_hPanel, url);
}

void IEController::Refresh()
{
    IOleObject* browserObject = *((IOleObject**)GetWindowLong(m_hPanel, GWLP_USERDATA));
    browserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_IWebBrowser2);
    CComVariant v(REFRESH_COMPLETELY);
    m_IWebBrowser2->Refresh2(&v);
    m_IWebBrowser2->Release();
}

// Get the handle to the IHTMLDocument2 interface for the currently loaded webpage.
// We will use this handle to access the content of the webpage.
IHTMLDocument2* IEController::LoadDocument(__in_ecount(MAX_PATH)LPCTSTR url)
{
    (*m_lpDisplayHTMLPage)(m_hPanel, url);

    IOleObject* browserObject = *((IOleObject**)GetWindowLong(m_hPanel, GWLP_USERDATA));
    browserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_IWebBrowser2);

    HRESULT hr;
    IDispatch* pHtmlDocDispatch = NULL;
    IHTMLDocument2 * pHtmlDoc = NULL;

    hr = m_IWebBrowser2->get_Document(&pHtmlDocDispatch);
    m_IWebBrowser2->Release();
    if(FAILED(hr) || (pHtmlDocDispatch == NULL)) {goto Exit;}

    hr = pHtmlDocDispatch->QueryInterface(IID_IHTMLDocument2,  (void**)&pHtmlDoc);
    pHtmlDocDispatch->Release();

    if(FAILED(hr) || (pHtmlDoc == NULL)) {goto Exit;}

    return pHtmlDoc;

Exit:
    return NULL;
}