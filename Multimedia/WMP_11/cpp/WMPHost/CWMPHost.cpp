// CWMPHost.cpp : Implementation of the CWMPHost
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include "stdafx.h"
#include "CWMPHost.h"
#include "dialogs.h"


/////////////////////////////////////////////////////////////////////////////
// CWMPHost

void CWMPHost::OnFinalMessage(HWND /*hWnd*/)
{
    ::PostQuitMessage(0);
}

LRESULT CWMPHost::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AtlAxWinInit();
    CComPtr<IAxWinHostWindow>           spHost;
    CComPtr<IConnectionPointContainer>  spConnectionContainer;
    CComWMPEventDispatch                *pEventListener = NULL;
    CComPtr<IWMPEvents>                 spEventListener;
    HRESULT                             hr;
    RECT                                rcClient;

    m_dwAdviseCookie = 0;

    // create window

    GetClientRect(&rcClient);
    m_wndView.Create(m_hWnd, rcClient, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
    if (NULL == m_wndView.m_hWnd)
        goto FAILURE;
    
    // load OCX in window

    hr = m_wndView.QueryHost(&spHost);
    if (FAILMSG(hr))
        goto FAILURE;

    hr = spHost->CreateControl(CComBSTR(_T("{6BF52A52-394A-11d3-B153-00C04F79FAA6}")), m_wndView, 0);
    if (FAILMSG(hr))
        goto FAILURE;

    hr = m_wndView.QueryControl(&m_spWMPPlayer);
    if (FAILMSG(hr))
        goto FAILURE;

    // start listening to events

    hr = CComWMPEventDispatch::CreateInstance(&pEventListener);
    spEventListener = pEventListener;
    if (FAILMSG(hr))
        goto FAILURE;

    hr = m_spWMPPlayer->QueryInterface(&spConnectionContainer);
    if (FAILMSG(hr))
        goto FAILURE;

    // See if OCX supports the IWMPEvents interface
    hr = spConnectionContainer->FindConnectionPoint(__uuidof(IWMPEvents), &m_spConnectionPoint);
    if (FAILED(hr))
    {
        // If not, try the _WMPOCXEvents interface, which will use IDispatch
        hr = spConnectionContainer->FindConnectionPoint(__uuidof(_WMPOCXEvents), &m_spConnectionPoint);
        if (FAILMSG(hr))
            goto FAILURE;
    }

    hr = m_spConnectionPoint->Advise(spEventListener, &m_dwAdviseCookie);
    if (FAILMSG(hr))
        goto FAILURE;

    return 0;

FAILURE:
    ::PostQuitMessage(0);
    return 0;
}

LRESULT CWMPHost::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

    bHandled = FALSE;
    return 0;
}

LRESULT CWMPHost::OnErase(UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */, BOOL& bHandled)
{
    return 1;
}

LRESULT CWMPHost::OnSize(UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */, BOOL& /* lResult */)
{
    RECT rcClient;
    GetClientRect(&rcClient);
    m_wndView.MoveWindow(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
    return 0;
}

LRESULT CWMPHost::OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    CFileOpenDlg dlgOpen;
    HRESULT      hr;

    if (dlgOpen.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->put_URL(dlgOpen.m_bstrName);
        if (FAILMSG(hr))
            return 0;
    }
    return 0;
}

LRESULT CWMPHost::OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    DestroyWindow();
    return 0;
}

LRESULT CWMPHost::OnWMPCoreClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT     hr;

    hr = m_spWMPPlayer->close();
    if (FAILMSG(hr))
        return 0;

    return 0;
}

LRESULT CWMPHost::OnWMPCoreURL(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    USES_CONVERSION;
    CComBSTR    m_bstrValue;
    HRESULT     hr;

    hr = m_spWMPPlayer->get_URL(&m_bstrValue);
    if (FAILMSG(hr))
        return 0;

    CStringDlg dlgString(_T("IWMPCore->URL"), OLE2T(m_bstrValue));

    if (dlgString.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->put_URL(dlgString.m_bstrValue);
        if (FAILMSG(hr))
            return 0;
    }
    return 0;
}

LRESULT CWMPHost::OnWMPCoreOpenState(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    WMPOpenState osValue;

    hr = m_spWMPPlayer->get_openState(&osValue);
    if (FAILMSG(hr))
        return 0;

    TCHAR   szValue[MAX_STRING];

    ::_stprintf_s(szValue, MAX_STRING, _T("Value = %d"), osValue);
    ::MessageBox(NULL, szValue, _T("IWMPCore->openState"), MB_OK);

    return 0;
}

LRESULT CWMPHost::OnWMPCorePlayState(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    WMPPlayState psValue;

    hr = m_spWMPPlayer->get_playState(&psValue);
    if (FAILMSG(hr))
        return 0;

    TCHAR   szValue[MAX_STRING];

    ::_stprintf_s(szValue, MAX_STRING, _T("Value = %d"), psValue);
    ::MessageBox(NULL, szValue, _T("IWMPCore->playState"), MB_OK);

    return 0;
}

LRESULT CWMPHost::OnWMPCoreVersionInfo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    USES_CONVERSION;
    CComBSTR    m_bstrValue;
    HRESULT     hr;

    hr = m_spWMPPlayer->get_versionInfo(&m_bstrValue);
    if (FAILMSG(hr))
        return 0;

    TCHAR   szValue[MAX_STRING];

    ::_stprintf_s(szValue, MAX_STRING, _T("Version = %s"), OLE2T(m_bstrValue));
    ::MessageBox(NULL, szValue, _T("IWMPCore->versionInfo"), MB_OK);

    return 0;
}

LRESULT CWMPHost::OnWMPCoreLaunchURL(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    CComBSTR    m_bstrValue;
    HRESULT     hr;

    CStringDlg dlgString(_T("IWMPCore->LaunchURL"));

    if (dlgString.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->launchURL(dlgString.m_bstrValue);
        if (FAILMSG(hr))
            return 0;
    }
    return 0;
}

LRESULT CWMPHost::OnWMPCoreIsOnline(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    VARIANT_BOOL fValue;

    hr = m_spWMPPlayer->get_isOnline(&fValue);
    if (FAILMSG(hr))
        return 0;

    TCHAR   szValue[MAX_STRING];

    ::_stprintf_s(szValue, MAX_STRING, _T("Value = %s"), fValue ? _T("TRUE") : _T("FALSE"));
    ::MessageBox(NULL, szValue, _T("IWMPCore->isOnline"), MB_OK);

    return 0;
}

LRESULT CWMPHost::OnWMPCoreStatus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    USES_CONVERSION;
    CComBSTR    m_bstrValue;
    HRESULT     hr;

    hr = m_spWMPPlayer->get_status(&m_bstrValue);
    if (FAILMSG(hr))
        return 0;

    TCHAR   szValue[MAX_STRING];

    ::_stprintf_s(szValue, MAX_STRING, _T("Status = %s"), OLE2T(m_bstrValue));
    ::MessageBox(NULL, szValue, _T("IWMPCore->status"), MB_OK);

    return 0;
}

LRESULT CWMPHost::OnWMPCoreInterface(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT     hr;
    TCHAR       szName[MAX_STRING];
    void        *pUnknown;

    hr = E_FAIL;
    szName[0] = _T('\0');
    pUnknown = NULL;

    switch (wID)
    {
    case ID_WMPCORE_CONTROLS:
        {
            CComPtr<IWMPControls> spWMPControls;

            hr = m_spWMPPlayer->get_controls(&spWMPControls);
            if (spWMPControls)
            {
                spWMPControls->QueryInterface(__uuidof(IWMPControls), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->controls"));
            }
        }
        break;
    case ID_WMPCORE_SETTINGS:
        {
            CComPtr<IWMPSettings> spWMPSettings;

            hr = m_spWMPPlayer->get_settings(&spWMPSettings);
            if (spWMPSettings)
            {
                spWMPSettings->QueryInterface(__uuidof(IWMPSettings), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->settings"));
            }
        }
        break;
    case ID_WMPCORE_CURRENTMEDIA:
        {
            CComPtr<IWMPMedia> spWMPMedia;

            hr = m_spWMPPlayer->get_currentMedia(&spWMPMedia);
            if (spWMPMedia)
            {
                spWMPMedia->QueryInterface(__uuidof(IWMPMedia), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->currentMedia"));
            }
        }
        break;
    case ID_WMPCORE_MEDIACOLLECTION:
        {
            CComPtr<IWMPMediaCollection> spWMPMediaCollection;

            hr = m_spWMPPlayer->get_mediaCollection(&spWMPMediaCollection);
            if (spWMPMediaCollection)
            {
                spWMPMediaCollection->QueryInterface(__uuidof(IWMPMediaCollection), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->currentMediaCollection"));
            }
        }
        break;
    case ID_WMPCORE_PLAYLISTCOLLECTION:
        {
            CComPtr<IWMPPlaylistCollection> spWMPPlaylistCollection;

            hr = m_spWMPPlayer->get_playlistCollection(&spWMPPlaylistCollection);
            if (spWMPPlaylistCollection)
            {
                spWMPPlaylistCollection->QueryInterface(__uuidof(IWMPPlaylistCollection), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->playlistCollection"));
            }
        }
        break;
    case ID_WMPCORE_NETWORK:
        {
            CComPtr<IWMPNetwork> spWMPNetwork;

            hr = m_spWMPPlayer->get_network(&spWMPNetwork);
            if (spWMPNetwork)
            {
                spWMPNetwork->QueryInterface(__uuidof(IWMPNetwork), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->network"));
            }
        }
        break;
    case ID_WMPCORE_CURRENTPLAYLIST:
        {
            CComPtr<IWMPPlaylist> spWMPPlaylist;

            hr = m_spWMPPlayer->get_currentPlaylist(&spWMPPlaylist);
            if (spWMPPlaylist)
            {
                spWMPPlaylist->QueryInterface(__uuidof(IWMPPlaylist), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->currentPlaylist"));
            }
        }
        break;
    case ID_WMPCORE_CDROMCOLLECTION:
        {
            CComPtr<IWMPCdromCollection> spWMPCDRomCollection;

            hr = m_spWMPPlayer->get_cdromCollection(&spWMPCDRomCollection);
            if (spWMPCDRomCollection)
            {
                spWMPCDRomCollection->QueryInterface(__uuidof(IWMPCdromCollection), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->cdromCollection"));
            }
        }
        break;
    case ID_WMPCORE_CLOSEDCAPTION:
        {
            CComPtr<IWMPClosedCaption> spWMPClosedCaption;

            hr = m_spWMPPlayer->get_closedCaption(&spWMPClosedCaption);
            if (spWMPClosedCaption)
            {
                spWMPClosedCaption->QueryInterface(__uuidof(IWMPClosedCaption), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->closedCaption"));
            }
        }
        break;
    case ID_WMPCORE_ERROR:
        {
            CComPtr<IWMPError> spWMPError;

            hr = m_spWMPPlayer->get_error(&spWMPError);
            if (spWMPError)
            {
                spWMPError->QueryInterface(__uuidof(IWMPError), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore->error"));
            }
        }
        break;
    case ID_WMPCORE2_DVD:
        {
            CComPtr<IWMPPlayer3> spWMPPlayer3;

            hr = m_spWMPPlayer.QueryInterface(&spWMPPlayer3);
            if (FAILMSG(hr))
                return 0;

            CComPtr<IWMPDVD> spWMPDVD;

            hr = spWMPPlayer3->get_dvd(&spWMPDVD);
            if (spWMPDVD)
            {
                spWMPDVD->QueryInterface(__uuidof(IWMPDVD), &pUnknown);
                _tcscpy_s(szName, MAX_STRING, _T("IWMPCore2->dvd"));
            }
        }
        break;
    }

    if (FAILMSG(hr))
        return 0;

    if (!pUnknown)
    {
        FAILMSG(E_NOINTERFACE);
        return 0;
    }

    ((IUnknown *)pUnknown)->Release();

    ::MessageBox(NULL, _T("Got the expected interface"), szName, MB_OK);

    return 0;
}

LRESULT CWMPHost::OnWMPPlayerEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    VARIANT_BOOL fValue;

    hr = m_spWMPPlayer->get_enabled(&fValue);
    if (FAILMSG(hr))
        return 0;

    CBooleanDlg dlgBoolean(_T("IWMPPlayer->enabled"), fValue);

    if (dlgBoolean.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->put_enabled(dlgBoolean.m_fValue);
        if (FAILMSG(hr))
            return 0;
    }

    return 0;
}

LRESULT CWMPHost::OnWMPPlayerFullScreen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    VARIANT_BOOL fValue;

    hr = m_spWMPPlayer->get_fullScreen(&fValue);
    if (FAILMSG(hr))
        return 0;

    CBooleanDlg dlgBoolean(_T("IWMPPlayer->fullScreen"), fValue);

    if (dlgBoolean.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->put_fullScreen(dlgBoolean.m_fValue);
        if (FAILMSG(hr))
            return 0;
    }

    return 0;
}

LRESULT CWMPHost::OnWMPPlayerEnableContextMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    VARIANT_BOOL fValue;

    hr = m_spWMPPlayer->get_enableContextMenu(&fValue);
    if (FAILMSG(hr))
        return 0;

    CBooleanDlg dlgBoolean(_T("IWMPPlayer->enableContextMenu"), fValue);

    if (dlgBoolean.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->put_enableContextMenu(dlgBoolean.m_fValue);
        if (FAILMSG(hr))
            return 0;
    }

    return 0;
}

LRESULT CWMPHost::OnWMPPlayerUIMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    USES_CONVERSION;
    CComBSTR    m_bstrValue;
    HRESULT     hr;

    hr = m_spWMPPlayer->get_uiMode(&m_bstrValue);
    if (FAILMSG(hr))
        return 0;

    CStringDlg dlgString(_T("IWMPCore->uiMode"), OLE2T(m_bstrValue));

    if (dlgString.DoModal(m_hWnd) == IDOK)
    {
        hr = m_spWMPPlayer->put_uiMode(dlgString.m_bstrValue);
        if (FAILMSG(hr))
            return 0;
    }
    return 0;
}

LRESULT CWMPHost::OnWMPPlayer2StretchToFit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT      hr;
    VARIANT_BOOL fValue;
    CComPtr<IWMPPlayer2> spWMPPlayer2;

    hr = m_spWMPPlayer.QueryInterface(&spWMPPlayer2);
    if (FAILMSG(hr))
        return 0;

    hr = spWMPPlayer2->get_stretchToFit(&fValue);
    if (FAILMSG(hr))
        return 0;

    CBooleanDlg dlgBoolean(_T("IWMPPlayer2->stretchToFit"), fValue);

    if (dlgBoolean.DoModal(m_hWnd) == IDOK)
    {
        hr = spWMPPlayer2->put_stretchToFit(dlgBoolean.m_fValue);
        if (FAILMSG(hr))
            return 0;
    }

    return 0;
}

LRESULT CWMPHost::FowardMsgToWMP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HRESULT hr;
    LRESULT llResult = 0;
    CComPtr<IOleInPlaceObjectWindowless> spSite = NULL;

    hr = m_spWMPPlayer->QueryInterface(&spSite);
    if( SUCCEEDED(hr) )
    {
        spSite->OnWindowMessage(uMsg, wParam, lParam, &llResult);
    }
    bHandled = TRUE;

    return llResult;
}
