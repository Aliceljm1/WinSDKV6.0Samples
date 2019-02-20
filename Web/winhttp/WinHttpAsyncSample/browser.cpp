//  WinHttpAsyncSample
//  Copyright (c) Microsoft Corporation. All rights reserved. #include <windows.h>

#include <windows.h>
#include <winhttp.h>
#include "defs.h"
#include "browser.h"

#include <stdio.h>

//
// SIMPLE_BROWSER class implementation
//

SIMPLE_BROWSER::SIMPLE_BROWSER(UINT nID)
{
    m_nID = nID + 1;
    m_hSession = NULL;

    m_pBrowserConfig = NULL;
    m_pHomePageRequester = NULL;

    m_hShutdownEvent = NULL;
    m_fShutdownInProgress = FALSE;

    m_nRequesterIDSeed = m_nID * 10; // for example, if the browser is 1, then the 
                                     // embedded links are numbered 10, 11, 12, and etc.
}

SIMPLE_BROWSER::~SIMPLE_BROWSER()
{
}

BOOL SIMPLE_BROWSER::Open(P_BROWSER_CONFIG pBrowserConfig)
{
    ASSERT(pBrowserConfig != NULL);

    m_pBrowserConfig = pBrowserConfig;

    InitializeListHead(&m_EmbeddedLinks);

    ::InitializeCriticalSection(&m_LinksCritSec);
    
    PWSTR pwszEmbeddedLinks = NULL;
    SIZE_T cchEmbeddedLinks = 0;

    fprintf(stdout, 
            "Browser #%d is initializing...\n",
            m_nID);

    m_hShutdownEvent = ::CreateEvent(NULL,
                                     TRUE,  // manual reset
                                     FALSE, // not initially set
                                     NULL);

    if (m_hShutdownEvent == NULL)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; CreateEvent() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_cleanup;
    }

    DWORD dwAccessType = pBrowserConfig->fProxyAutoDiscovery ? 
                         WINHTTP_ACCESS_TYPE_NO_PROXY : 
                         WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;

    m_hSession = ::WinHttpOpen(L"Microsoft-Windows-HTTP-Services-Async-Sample/1.0",
                               dwAccessType,
                               WINHTTP_NO_PROXY_NAME,
                               WINHTTP_NO_PROXY_BYPASS,
                               WINHTTP_FLAG_ASYNC);
    if (m_hSession == NULL)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; WinHttpOpen() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_cleanup;
    }

    m_pHomePageRequester = new ASYNC_REQUESTER(m_nRequesterIDSeed++, this);
    if (m_pHomePageRequester == NULL)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; not enough memory.\n",
                m_nID);

        goto error_cleanup;
    }

    if (m_pHomePageRequester->Open(pBrowserConfig->pwszHomePage,
                                 pBrowserConfig->fProxyAutoDiscovery,
                                 pBrowserConfig->fEnableProxyFailover,
                                 pBrowserConfig->nFailureRetries,
                                 pBrowserConfig->nTimeLimit) == FALSE)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; can not initialize a requester.\n",
                m_nID);

        goto error_cleanup;
    }

    if (m_pBrowserConfig->pwszEmbeddedLinks == NULL)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; invalid list of embedded links.\n",
                m_nID);

        goto error_cleanup;
    }

    cchEmbeddedLinks = ::wcslen(m_pBrowserConfig->pwszEmbeddedLinks) + 1;
    if (cchEmbeddedLinks < 1)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; integer overflow.\n",
                m_nID);

        goto error_cleanup;
    }

    pwszEmbeddedLinks = new WCHAR[cchEmbeddedLinks];
    if (pwszEmbeddedLinks == NULL)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; not enough memory.\n",
                m_nID);

        goto error_cleanup;
    }

    ::wcscpy_s(pwszEmbeddedLinks, cchEmbeddedLinks, m_pBrowserConfig->pwszEmbeddedLinks);

    PWSTR pwszContext = NULL;
    PWSTR pwszLink = ::wcstok_s(pwszEmbeddedLinks, L";", &pwszContext);
    while (pwszLink != NULL)
    {
        fprintf(stdout, 
                "Browser #%d is initializing link %S...\n",
                m_nID,
                pwszLink);

        ASYNC_REQUESTER* pNewRequester = new ASYNC_REQUESTER(m_nRequesterIDSeed++, this);
        if (pNewRequester == NULL)
        {
            fprintf(stderr, 
                    "Browser #%d failed to create a new request; not enough memory.\n",
                    m_nID);

            goto next_link;
        }

        if (pNewRequester->Open(pwszLink,
                                m_pBrowserConfig->fProxyAutoDiscovery,
                                m_pBrowserConfig->fEnableProxyFailover,
                                m_pBrowserConfig->nFailureRetries,
                                m_pBrowserConfig->nTimeLimit) == FALSE)
        {
            fprintf(stderr, 
                "Browser #%d failed to initialize a new requester.\n",
                    m_nID);

            goto next_link;
        }

        InsertHeadList(&m_EmbeddedLinks, (PLIST_ENTRY)pNewRequester);
        pNewRequester = NULL;
    
    next_link:

        if (pNewRequester)
        {
            if (m_pHomePageRequester->m_State == ASYNC_REQUESTER::OPENED)
            {
                m_pHomePageRequester->Close();
            }

            delete pNewRequester;
        }

        pwszLink = ::wcstok_s(NULL, L";", &pwszContext);
    }

    delete [] pwszEmbeddedLinks;
    pwszEmbeddedLinks = NULL;

    if (m_pHomePageRequester->Start() == FALSE)
    {
        fprintf(stderr, 
               "Browser #%d failed to open; can not start a requester.\n",
                m_nID);

        goto error_cleanup;
    }

    return TRUE;

error_cleanup:

    if (m_pHomePageRequester)
    {
        if (m_pHomePageRequester->m_State == ASYNC_REQUESTER::OPENED)
        {
            m_pHomePageRequester->Close();
        }
        delete m_pHomePageRequester;
        m_pHomePageRequester = NULL;
    }
    if (m_hSession)
    {
        ::WinHttpCloseHandle(m_hSession);
        m_hSession = NULL;
    }
    if (m_hShutdownEvent)
    {
        ::CloseHandle(m_hShutdownEvent);
        m_hShutdownEvent = NULL;
    }
    if (pwszEmbeddedLinks)
    {
        delete [] pwszEmbeddedLinks;
    }
    ::DeleteCriticalSection(&m_LinksCritSec);

    return FALSE;
}

VOID SIMPLE_BROWSER::Close(VOID)
{
    m_fShutdownInProgress = TRUE;

    BOOL fNeedToWait = FALSE;

    ::EnterCriticalSection(&m_LinksCritSec);

    if (m_pHomePageRequester)
    {
        if (m_pHomePageRequester->m_State != ASYNC_REQUESTER::CLOSED)
        {
            m_pHomePageRequester->RequestToShutdown();
            fNeedToWait = TRUE;
        }
    }

    for (PLIST_ENTRY entry = (&m_EmbeddedLinks)->Flink;
            entry != (PLIST_ENTRY)&((&m_EmbeddedLinks)->Flink);
            entry = entry->Flink)
    {
        ASYNC_REQUESTER* pRequester = (ASYNC_REQUESTER*)entry;

        if (pRequester->m_State != ASYNC_REQUESTER::CLOSED)
        {
            pRequester->RequestToShutdown();
            fNeedToWait = TRUE;
        }
    }

    ::LeaveCriticalSection(&m_LinksCritSec);

    if (fNeedToWait)
    {
        fprintf(stdout, 
               "Browser #%d waiting for all requesters to exit...\n",
                m_nID);

        if (::WaitForSingleObject(m_hShutdownEvent, /*60000*/INFINITE) == WAIT_TIMEOUT)
        {
            fprintf(stderr, 
                "...and Browser #%d timed out waiting all requesters to exit... exit anyway...\n",
                    m_nID);
        }
        else
        {
            fprintf(stdout, 
                "...and Browser #%d is notified that all requesters are exited...\n",
                    m_nID);
        }

    }

    while (!IsListEmpty(&m_EmbeddedLinks)) 
    {
        PLIST_ENTRY pEntry = RemoveHeadList(&m_EmbeddedLinks);
        ASYNC_REQUESTER* pRequester = (ASYNC_REQUESTER*)pEntry;
        
        delete pRequester;
    }

    if (m_hSession)
    {
        ::WinHttpCloseHandle(m_hSession);
        m_hSession = NULL;
    }
    if (m_hShutdownEvent)
    {
        ::CloseHandle(m_hShutdownEvent);
        m_hShutdownEvent = NULL;
    }
    ::DeleteCriticalSection(&m_LinksCritSec);
}


VOID SIMPLE_BROWSER::OnRequesterStopped(ASYNC_REQUESTER* pRequester)
{
    ASSERT(pRequester != NULL);

    //if (m_fShutdownInProgress  && 
    //    pRequester->m_State != ASYNC_REQUESTER::CLOSING)
    //{
    //    return;
    //}

    ::EnterCriticalSection(&m_LinksCritSec);

    if (pRequester == m_pHomePageRequester)
    {
        if (pRequester->m_State == ASYNC_REQUESTER::DATA_EXHAUSTED)
        {
            fprintf(stdout, 
                    "Browser #%d fetched its Home Page, now it attempts to download all embedded lnks.\n",
                    m_nID);

            for (PLIST_ENTRY entry = (&m_EmbeddedLinks)->Flink;
                    entry != (PLIST_ENTRY)&((&m_EmbeddedLinks)->Flink);
                    entry = entry->Flink)
            {
                ASYNC_REQUESTER* pRequesterNew = (ASYNC_REQUESTER*)entry;
                if (pRequesterNew->Start() == FALSE)
                {
                    fprintf(stderr, 
                        "Browser #%d failed to start a new requester.\n",
                            m_nID);
                }
            }

            // now we can close the home page request
            pRequester->Close();
        
            goto exit;
        }
    }

    BOOL fClosingRequest = TRUE;

    if (pRequester->m_State == ASYNC_REQUESTER::CLOSING)
    {
        pRequester->Close();    // a live download has been aborted, now close it.
    }
    else if (pRequester->m_State == ASYNC_REQUESTER::ERROR)
    {
        if (m_pBrowserConfig->nFailureRetries)
        {
            switch(pRequester->m_dwLastError)
            {
                case ERROR_WINHTTP_CANNOT_CONNECT:
                case ERROR_WINHTTP_TIMEOUT:
                case ERROR_NOT_ENOUGH_MEMORY:
                case ERROR_WINHTTP_INTERNAL_ERROR:
                case ERROR_WINHTTP_NAME_NOT_RESOLVED:

                    if (pRequester->Start() == FALSE)
                    {
                        pRequester->Close(); // retry upon certain errors
                    }
                    else
                    {
                        fClosingRequest = FALSE;
                    }
                    break;

                default:
                    pRequester->Close();
            }
        }
        else  // close the request since we don't need to retry 
        {
            pRequester->Close();
        }
    }
    else // close the request if some other state is hit
    {
        pRequester->Close();
    }

    // the main page is interrupted, so we'll need to close all embedded links
    if (pRequester == m_pHomePageRequester && fClosingRequest)
    {
        for (PLIST_ENTRY entry = (&m_EmbeddedLinks)->Flink;
                entry != (PLIST_ENTRY)&((&m_EmbeddedLinks)->Flink);
                entry = entry->Flink)
        {
            ASYNC_REQUESTER* pRequesterToC = (ASYNC_REQUESTER*)entry;
            pRequesterToC->Close();
        }
    }

exit:
    ::LeaveCriticalSection(&m_LinksCritSec);
}

VOID SIMPLE_BROWSER::OnRequesterClosed(ASYNC_REQUESTER* pRequester)
{
    ::EnterCriticalSection(&m_LinksCritSec);
    
    if (m_fShutdownInProgress)
    {
        BOOL fAllClosed = TRUE;
        if (m_pHomePageRequester && m_pHomePageRequester->m_State != ASYNC_REQUESTER::CLOSED)
        {
            fAllClosed = FALSE;
        }

        for (PLIST_ENTRY entry = (&m_EmbeddedLinks)->Flink;
                entry != (PLIST_ENTRY)&((&m_EmbeddedLinks)->Flink);
                entry = entry->Flink)
        {
            ASYNC_REQUESTER* pRequesterNew = (ASYNC_REQUESTER*)entry;
            if (pRequesterNew->m_State != ASYNC_REQUESTER::CLOSED)
            {
                fAllClosed = FALSE;
            }
        }

        if (fAllClosed)
        {
            ::SetEvent(m_hShutdownEvent);
        }
    }
    
    ::LeaveCriticalSection(&m_LinksCritSec);
}

//
// ASYNC_REQUESTER class implementation
//
    
ASYNC_REQUESTER::ASYNC_REQUESTER(
    UINT nRequesterID, 
    SIMPLE_BROWSER* pBrowser)
{
    m_hConnect = NULL;
    m_hRequest = NULL;

    m_pwszUrl = NULL;

    m_nID = nRequesterID;
    m_pBrowser = pBrowser;
    
    m_fProxyAutoDiscovery = FALSE;
    m_fProxyFailover = FALSE;
    m_nFailureRetries = 0;
    
    ZeroMemory(&m_ProxyInfo, sizeof(m_ProxyInfo));

    m_pwszNextProxies = NULL;

    m_State = INIT;

    m_fClosing = FALSE;

    m_dwLastError = ERROR_SUCCESS;

    m_ContentLength = -1;
    m_dwBytesReadSoFar = 0;
}

ASYNC_REQUESTER::~ASYNC_REQUESTER()
{
    if (m_ProxyInfo.lpszProxy)
    {
        ::GlobalFree(m_ProxyInfo.lpszProxy);
    }
    if (m_ProxyInfo.lpszProxyBypass)
    {
        ::GlobalFree(m_ProxyInfo.lpszProxyBypass);
    }
    if (m_pwszUrl)
    {
        delete [] m_pwszUrl;
    }
}

void CALLBACK RequesterStatusCallback (
    HINTERNET hInternet,
    DWORD_PTR dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength
    )
{
    ASYNC_REQUESTER* pRequester = (ASYNC_REQUESTER*)dwContext;
    ASSERT(pRequester != NULL);

    ASSERT(pRequester->m_hRequest == hInternet);

    switch(dwInternetStatus)
    {
        case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
            pRequester->OnSendRequestComplete();
            break;
        case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
            pRequester->OnHeadersAvailable();
            break;
        case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
            pRequester->OnReadComplete(dwStatusInformationLength);
            break;
        case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
            pRequester->OnRequestError((LPWINHTTP_ASYNC_RESULT)lpvStatusInformation);
            break;
        case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
            pRequester->OnHandleClosing(hInternet);
            pRequester->m_hRequest = NULL;
            break;
        default:
            ;
    }
}

BOOL ASYNC_REQUESTER::Open(
    PWSTR pwszUrl,
    BOOL fProxyAutoDiscovery,
    BOOL fProxyFailover,
    UINT nFailureRetries,
    UINT dwTimeLimit
    )
{
    m_fProxyFailover = fProxyFailover;

    if (pwszUrl == NULL)
    {
        fprintf(stderr, 
                "Requester #%d failed to open; invalid URL.\n",
                m_nID);

        goto error_exit;
    }

    SIZE_T cchUrl = ::wcslen(pwszUrl) + 1;
    if (cchUrl < 1)
    {
        fprintf(stderr, 
                "Requester #%d failed to open; integer overflow.\n",
                m_nID);

        goto error_exit;
    }

    m_pwszUrl = new WCHAR[cchUrl];
    if (m_pwszUrl == NULL)
    {
        fprintf(stderr, 
                "Requester #%d failed to open; not enough memory.\n",
                m_nID);

        goto error_exit;
    }
    ::wcscpy_s(m_pwszUrl, cchUrl, pwszUrl);
    
    m_nNumAttempts = nFailureRetries + 1;

    m_dwStartingTime = ::GetTickCount();
    m_TimeRemaining = m_dwTimeLimit = dwTimeLimit * 1000;

    BOOL fProxyDiscovered = FALSE;

    if (fProxyAutoDiscovery)
    {
        fprintf(stdout, 
                "Requester #%d is detecting proxy settings for %S...\n",
                m_nID,
                m_pwszUrl);

        WINHTTP_AUTOPROXY_OPTIONS AutoProxyOptions;
        ZeroMemory(&AutoProxyOptions, sizeof(AutoProxyOptions));

        AutoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
        AutoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
        AutoProxyOptions.lpszAutoConfigUrl = NULL;
        AutoProxyOptions.fAutoLogonIfChallenged = TRUE;

        if (::WinHttpGetProxyForUrl(m_pBrowser->m_hSession,
                                    m_pwszUrl,
                                    &AutoProxyOptions, 
                                    &m_ProxyInfo) == FALSE)
        {
            fprintf(stderr, 
                   "Requester #%d failed to discover proxy info; ::WinHttpGetProxyForUrl() failed; error = %d.\n",
                    m_nID,
                    ::GetLastError());

            WINHTTP_PROXY_INFO pi;
            pi.dwAccessType = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;
            pi.lpszProxy = NULL;
            pi.lpszProxyBypass = NULL;

            if (::WinHttpSetOption(m_hRequest,
                                WINHTTP_OPTION_PROXY,
                                &pi,
                                sizeof(pi)) == FALSE)
            {
                fprintf(stderr, 
                    "Requester #%d failed to open; ::WinHttpSetOption() failed; error = %d.\n",
                        m_nID,
                        ::GetLastError());

                goto error_exit;
            }
        }
        else
        {
            fProxyDiscovered = TRUE;

            fprintf(stdout, 
                    "Requester #%d detected the proxy settings for %S is %S.\n",
                    m_nID,
                    m_pwszUrl,
                    m_ProxyInfo.lpszProxy
                    );
        }
    }

    if (!fProxyDiscovered)
    {
        DWORD dwProxyInfoSize = sizeof(m_ProxyInfo);
        ::WinHttpQueryOption(NULL, // query for global proxy config
                             WINHTTP_OPTION_PROXY,
                             &m_ProxyInfo,
                             &dwProxyInfoSize);

    }

    if (fProxyFailover && m_ProxyInfo.lpszProxy)
    {
        m_pwszNextProxies = ::wcschr(m_ProxyInfo.lpszProxy, L';');
        if (m_pwszNextProxies)
        {
            ++m_pwszNextProxies;
        }
    }
 
    URL_COMPONENTS UrlComponents;
    ZeroMemory(&UrlComponents, sizeof(UrlComponents));
    UrlComponents.dwStructSize = sizeof(UrlComponents);

    UrlComponents.dwSchemeLength    = (DWORD)-1;
    UrlComponents.dwHostNameLength  = (DWORD)-1;
    UrlComponents.dwUrlPathLength   = (DWORD)-1;

    if (::WinHttpCrackUrl(m_pwszUrl, 
                      (DWORD)::wcslen(m_pwszUrl),
                      0,
                      &UrlComponents) == NULL)
    {
        fprintf(stderr, 
               "Requester #%d failed to open; ::WinHttpCrackUrl() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_exit;
    }

    WCHAR wCharSave = UrlComponents.lpszHostName[UrlComponents.dwHostNameLength];
    UrlComponents.lpszHostName[UrlComponents.dwHostNameLength] = L'\0';

    m_hConnect = ::WinHttpConnect(m_pBrowser->m_hSession,
                                  UrlComponents.lpszHostName,
                                  UrlComponents.nPort,
                                  0);

    UrlComponents.lpszHostName[UrlComponents.dwHostNameLength] = wCharSave;

    if (m_hConnect == NULL)
    {
        fprintf(stderr, 
               "Requester #%d failed to open; ::WinHttpConnect() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_exit;
    }

    m_hRequest = ::WinHttpOpenRequest(m_hConnect,
                                      L"GET",
                                      UrlComponents.lpszUrlPath,
                                      L"HTTP/1.1",
                                      WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      (UrlComponents.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0);

    if (m_hRequest == NULL)
    {
        fprintf(stderr, 
               "Requester #%d failed to open; ::WinHttpOpenRequest() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_exit;
    }

    if (::WinHttpSetOption(m_hRequest,
                           WINHTTP_OPTION_PROXY,
                           &m_ProxyInfo,
                           sizeof(m_ProxyInfo)) == FALSE)
    {
        fprintf(stderr, 
               "Requester #%d failed to open; ::WinHttpSetOption() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_exit;
    }

    // in some cases we need to abort the transaction beforing sending (i.e. calling WinHttpSendRequest()), to make sure we
    // can point back to this object in thoses cases we need to set the context here.

    void* _this = this;
    if (::WinHttpSetOption(m_hRequest,
                           WINHTTP_OPTION_CONTEXT_VALUE,
                           &_this,
                           sizeof(this)) == FALSE)
    {
        fprintf(stderr, 
               "Requester #%d failed to open; ::WinHttpSetOption() failed; error = %d.\n",
                m_nID,
                ::GetLastError());

        goto error_exit;
    }

    ::WinHttpSetStatusCallback(m_hRequest,
                               RequesterStatusCallback,
                               WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS |
                               WINHTTP_CALLBACK_FLAG_HANDLES,   // to listen to the HANDLE_CLOSING event
                               0);
                             
    m_State = OPENED;

    return TRUE;

error_exit:

    if (m_hRequest)
    {
        ::WinHttpCloseHandle(m_hRequest);
        // m_hRequest will be set to NULL during the HANDLE_CLOSING callback
    }
    if (m_hConnect)
    {
        ::WinHttpCloseHandle(m_hConnect);
        m_hConnect = NULL;
    }
    return FALSE;
}

BOOL ASYNC_REQUESTER::Start(
    )
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    // make sure we are in the right state to kick start a transaction
    if (m_State != OPENED && 
        m_State != ERROR  &&
        m_State != DATA_EXHAUSTED)
    {
        return FALSE;
    }

    if (m_nNumAttempts-- <= 0)
    {
        m_State = ERROR;
        m_dwLastError = (DWORD)-1; // retry limit is up
        return FALSE;
    }

    // initialize or re-initialize states
    
    m_ContentLength = -1;
    m_dwBytesReadSoFar = 0;
    
    m_State = SENDING;

    if (::WinHttpSendRequest(m_hRequest,
                             WINHTTP_NO_ADDITIONAL_HEADERS, 
                             0,
                             WINHTTP_NO_REQUEST_DATA, 
                             0,
                             0, 
                             (DWORD_PTR)this)== FALSE)
    {
        m_dwLastError = ::GetLastError();
        
        fprintf(stderr, 
                "[%d] The Requester #%d failed to fetch %S, WinHttpSendRequest() failed; error = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);

        m_State = ERROR;
        return FALSE;
    }

    return TRUE;
}

VOID ASYNC_REQUESTER::OnSendRequestComplete(
    )
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    if (m_fClosing)
    {
        fprintf(stdout, 
               "[%d] The Requester #%d stops downloading from %S; it's signaled to shut down.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl);

        m_State = CLOSING;
        goto exit;
    }

    m_TimeRemaining = m_dwTimeLimit - (::GetTickCount() - m_dwStartingTime);
    if (m_TimeRemaining <= 0)
    {
        fprintf(stderr, 
                "[%d] The Requester #%d timed out fetching %S; elapsed time = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwTimeLimit);

        m_TimeRemaining = 0;

        m_State = ERROR;
        goto exit;
    }

    m_State = RECEIVING;

    if (::WinHttpReceiveResponse(m_hRequest, NULL) == FALSE)
    {
        m_dwLastError = ::GetLastError();
        
        fprintf(stderr, 
                "[%d] The Requester #%d failed to fetch %S, WinHttpReceiveResponse() failed; error = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);

        m_State = ERROR;
        goto exit;
    }

    return;

exit:

    m_pBrowser->OnRequesterStopped(this);
}

VOID ASYNC_REQUESTER::OnHeadersAvailable(
        )
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    if (m_fClosing)
    {
        fprintf(stdout, 
               "[%d] The Requester #%d stops downloading from %S; it's signaled to shut down.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl);

        m_State = CLOSING;
        goto exit;
    }

    m_TimeRemaining = m_dwTimeLimit - (::GetTickCount() - m_dwStartingTime);
    if (m_TimeRemaining <= 0)
    {
        fprintf(stderr, 
                "[%d] The Requester #%d timed out fetching %S; elapsed time = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwTimeLimit);

        m_TimeRemaining = 0;

        m_State = ERROR;
        goto exit;
    }

    m_State = HEADERS_AVAILABLE;

    DWORD dwStatusCode = 0;
    DWORD dwSize = sizeof(DWORD);

    if (::WinHttpQueryHeaders(m_hRequest,
                             WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, 
                             NULL,
                             &dwStatusCode,
                             &dwSize,
                             NULL) == FALSE)
    {
        m_dwLastError = ::GetLastError();

        fprintf(stderr, 
                "[%d] The Requester #%d failed to fetch %S, WinHttpQueryHeaders() failed; error = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);

        m_State = ERROR;
        goto exit;
    }

    if ((dwStatusCode == HTTP_STATUS_DENIED) || (dwStatusCode == HTTP_STATUS_PROXY_AUTH_REQ))
    {
        fprintf(stderr, 
                "[%d] The Requester #%d failed to fetch %S, the proxy or server requires authentication; Status Code = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                dwStatusCode);

        // NOTE: This sample does not show how authentication should be handled; please refer to the
        // authentication sample in this SDK to see how credentials should be supplied in respond
        // to auth requests.

        m_dwLastError = dwStatusCode;
        
        m_State = ERROR;
        goto exit;
    }

    if (dwStatusCode != HTTP_STATUS_OK)
    {
        fprintf(stderr, 
                "[%d] The Requester #%d failed to fetch %S; Status Code = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                dwStatusCode);

        m_dwLastError = dwStatusCode;
        
        m_State = ERROR;
        goto exit;
    }

    DWORD dwContentLength = 0;
    DWORD dwCLSize = sizeof(DWORD);

    if (::WinHttpQueryHeaders(m_hRequest,
                             WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, 
                             NULL,
                             &dwContentLength,
                             &dwCLSize,
                             NULL) == FALSE)
    {
        m_dwLastError = ::GetLastError();

        fprintf(stderr, 
                "[%d] The Requester #%d failed to fetch %S, WinHttpQueryHeaders() failed; error = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);

    }
    else
    {
        fprintf(stdout, 
            "[%d] The Requester #%d attemps to download %d bytes of data from %S.\n", 
            dwThreadId,
            m_nID,
            dwContentLength,
            m_pwszUrl);

        m_ContentLength = dwContentLength;
    }

    m_State = WAITING_FOR_DATA;

    if (::WinHttpReadData(m_hRequest,
                          m_ReadBuffer,
                          READ_BUFFER_SIZE,
                          NULL) == FALSE)
    {
        m_dwLastError = ::GetLastError();
        fprintf(stderr, 
                "[%d] The Requester #%d failed to read data from %S, WinHttpReadData() failed; error = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);

        m_State = ERROR;
        goto exit;
    }

    return;

exit:

    m_pBrowser->OnRequesterStopped(this);
}

VOID ASYNC_REQUESTER::OnReadComplete(
    DWORD dwBytesRead
    )
{
   DWORD dwThreadId = ::GetCurrentThreadId();

    if (m_fClosing)
    {
        fprintf(stdout, 
               "[%d] The Requester #%d stops downloading from %S; it's signaled to shut down.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl);

        m_State = CLOSING;
        goto exit;
    }

    m_TimeRemaining = m_dwTimeLimit - (::GetTickCount() - m_dwStartingTime);
    if (m_TimeRemaining <= 0)
    {
        fprintf(stderr, 
                "[%d] The Requester #%d timed out fetching %S; elapsed time = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwTimeLimit);

        m_TimeRemaining = 0;

        m_State = ERROR;
        goto exit;
    }

    if (dwBytesRead == 0)
    {
        m_State = DATA_EXHAUSTED;
    
        fprintf(stdout, 
            "[%d] Requester #%d has downloaded all data from from %S.\n",
            dwThreadId,
            m_nID, 
            m_pwszUrl);

        goto exit;
    }
    else
    {
        m_State = DATA_AVAILABLE;
    
        m_dwBytesReadSoFar += dwBytesRead;

        fprintf(stdout, 
                "[%d] Requester #%d has downloaded %d bytes from from %S",
                dwThreadId,
                m_nID, 
                m_dwBytesReadSoFar,
                m_pwszUrl);

        if (m_ContentLength == -1)
        {
            fprintf(stdout, ".\n");  // total size unknown
        }
        else
        {
            fprintf(stdout, " (%.2f%% Complete).\n", ((float)m_dwBytesReadSoFar/m_ContentLength) * 100);
        }

    }

    m_State = WAITING_FOR_DATA;

    if (::WinHttpReadData(m_hRequest,
                          m_ReadBuffer,
                          READ_BUFFER_SIZE,
                          NULL) == FALSE)
    {
        m_dwLastError = ::GetLastError();

        fprintf(stderr, 
                "[%d] The Requester #%d failed to read data from %S, WinHttpReadData() failed; error = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);

        m_State = ERROR;
        goto exit;
    }

    return;

exit:

    m_pBrowser->OnRequesterStopped(this);
}

VOID ASYNC_REQUESTER::OnRequestError(
    LPWINHTTP_ASYNC_RESULT pAsyncResult
)
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    if (m_fClosing)
    {
        fprintf(stdout, 
               "[%d] The Requester #%d stops downloading from %S; it's signaled to shut down.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl);

        m_State = CLOSING;
        goto exit;
    }

    m_TimeRemaining = m_dwTimeLimit - (::GetTickCount() - m_dwStartingTime);
    if (m_TimeRemaining <= 0)
    {
        fprintf(stderr, 
                "[%d] The Requester #%d timed out fetching %S; elapsed time = %d.\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwTimeLimit);

        m_TimeRemaining = 0;

        m_State = ERROR;
        goto exit;
    }

    m_State = ERROR;

    m_dwErrorAPI = (DWORD)pAsyncResult->dwResult;
    m_dwLastError = pAsyncResult->dwError;

    if (m_dwErrorAPI == API_SEND_REQUEST)
    {
        if (m_dwLastError == ERROR_WINHTTP_CANNOT_CONNECT || m_dwLastError == ERROR_WINHTTP_NAME_NOT_RESOLVED)
        {
            fprintf(stderr, 
                    "[%d] The Requester #%d failed to fetch %S, WinHttpSendRequest() c/b failed; error = %d.\n", 
                    dwThreadId,
                    m_nID,
                    m_pwszUrl, 
                    m_dwLastError);

            // *** proxy failover logic ***

            // WinHttp 5.1 has a limitation that it only uses the first proxy in a WINHTTP_PROXY_INFO structure
            // to attempt connect. Here we try to work around that by feeding the rest of the proxies and force
            // winHttp to retry.

            if (m_fProxyFailover && m_pwszNextProxies && m_pwszNextProxies[0])
            {
                WINHTTP_PROXY_INFO NextProxiesInfo;
                NextProxiesInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
                NextProxiesInfo.lpszProxy = m_pwszNextProxies;
                NextProxiesInfo.lpszProxyBypass = m_ProxyInfo.lpszProxyBypass;

                m_pwszNextProxies = ::wcschr(m_pwszNextProxies, L';');
                if (m_pwszNextProxies)
                {
                    ++m_pwszNextProxies;
                }

                if (::WinHttpSetOption(m_hRequest,
                                       WINHTTP_OPTION_PROXY,
                                       &NextProxiesInfo,
                                       sizeof(NextProxiesInfo)) == TRUE)
                {
                    fprintf(stdout, 
                            "[%d] The Requester #%d retries sending using a new proxy list %S.\n", 
                            dwThreadId,
                            m_nID,
                            NextProxiesInfo.lpszProxy);
                }
            }
        }
    }
    else if (m_dwErrorAPI == API_RECEIVE_RESPONSE)
    {
        if (m_dwLastError == ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED)
        {
            fprintf(stderr, 
                    "[%d] The Requester #%d failed to fetch %S, WinHttpReceiveResponse() c/b failed; the server requires SSL client auth.\n", 
                    dwThreadId,
                    m_nID,
                    m_pwszUrl);

            // NOTE: This sample does not show how SSL client authentication should be handled; please refer to the
            // SSL client authentication sample in this SDK to see how a client certificate should be supplied in respond
            // to SSL client auth requests.
        }
        else
        {
            fprintf(stderr, 
                    "[%d] The Requester #%d failed to fetch %S, WinHttpReceiveResponse() c/b failed; error = %d.\n", 
                    dwThreadId,
                    m_nID,
                    m_pwszUrl, 
                    m_dwLastError);
        }
    }
    else if (m_dwErrorAPI == API_READ_DATA)
    {
        fprintf(stderr, 
                "[%d] The Requester #%d failed to read data from %S, WinHttpReadData() c/b failed; error = %d\n", 
                dwThreadId,
                m_nID,
                m_pwszUrl, 
                m_dwLastError);
    }


exit:

    m_pBrowser->OnRequesterStopped(this);
}


VOID ASYNC_REQUESTER::RequestToShutdown()
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    fprintf(stdout, 
            "[%d] Requester #%d has been signaled to shutdown.\n",
            dwThreadId,
            m_nID);

    m_fClosing = TRUE;

    // this will cause an on-going request to transit to CLOSING state. The browser
    // will then be notified the CLOSING event, and it can then safely call
    // ASYNC_REQUESTER::Close().

    // if a requester has been stopped, it can be safely closed w/o calling RequestToShutdown() first.
}

VOID ASYNC_REQUESTER::OnHandleClosing(HINTERNET hRequest)
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    ASSERT(hRequest == m_hRequest);

    if (m_State == CLOSING)
    {
        m_State = CLOSED;

        ASSERT(m_pBrowser != NULL);

        fprintf(stdout, 
                "[%d] Requester #%d shuts down.\n",
                dwThreadId,
                m_nID);

        m_pBrowser->OnRequesterClosed(this);
    }
}

VOID ASYNC_REQUESTER::Close(VOID)
{
    DWORD dwThreadId = ::GetCurrentThreadId();

    if (m_State == ERROR ||
        m_State == DATA_EXHAUSTED || 
        m_State == CLOSING || 
        m_State == OPENED)
    {
        m_State = CLOSING;

        fprintf(stdout, 
                "[%d] Requester #%d is shutting down.\n",
                dwThreadId,
                m_nID);

        if (m_hRequest)
        {
            ::WinHttpCloseHandle(m_hRequest);
            // m_hRequest will be set to NULL during the HANDLE_CLOSING callback
        }
        if (m_hConnect)
        {
            ::WinHttpCloseHandle(m_hConnect);
            m_hConnect = NULL;
        }
        return;
    }

    ASSERT(0);
}
