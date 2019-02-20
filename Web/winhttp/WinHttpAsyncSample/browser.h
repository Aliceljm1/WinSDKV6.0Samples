//  WinHttpAsyncSample
//  Copyright (c) Microsoft Corporation. All rights reserved. 

#ifndef _SIMPLE_BROWSER_H
#define _SIMPLE_BROWSER_H

typedef struct BROWSER_CONFIG
{
    PWSTR  pwszHomePage;
    BOOL   fProxyAutoDiscovery;
    BOOL   fEnableProxyFailover;
    UINT   nTimeLimit;         // in seconds
    UINT   nFailureRetries;    // retry the request upon some errors (e.g. ERROR_WINHTTP_TIMEOUT)
    PWSTR  pwszEmbeddedLinks;  // download these embedded links (in parallel) once the Home Page has been downloaded
} *P_BROWSER_CONFIG;

class ASYNC_REQUESTER;

class SIMPLE_BROWSER
{
public:
    SIMPLE_BROWSER(UINT nID);
    ~SIMPLE_BROWSER();

    BOOL Open(P_BROWSER_CONFIG); // initialize the object and kick start the Home Page download

    VOID Close(VOID);   // will block until all pending downloads gracefully shutdown

private:
    VOID OnRequesterStopped(ASYNC_REQUESTER*);   // indicate a request transaction stopped for some reason
    VOID OnRequesterClosed(ASYNC_REQUESTER*);    // indicate a request is shutdown gracefully

private:
    UINT        m_nID;
    HINTERNET   m_hSession;

    P_BROWSER_CONFIG    m_pBrowserConfig;
    ASYNC_REQUESTER*    m_pHomePageRequester;

    LIST_ENTRY          m_EmbeddedLinks;
    CRITICAL_SECTION    m_LinksCritSec;

    HANDLE  m_hShutdownEvent;
    BOOL    m_fShutdownInProgress;

    UINT    m_nRequesterIDSeed;

friend class ASYNC_REQUESTER;
};

class ASYNC_REQUESTER
{
public:
    ASYNC_REQUESTER(UINT nRequesterID, SIMPLE_BROWSER* pBrowser);
    
    ~ASYNC_REQUESTER();

    BOOL Open(
        PWSTR pwszUrl,
        BOOL fProxyAutoDiscovery,
        BOOL fProxyFailover,
        UINT nFailureRetries,
        UINT nTimeLimit);

    BOOL Start();

    VOID RequestToShutdown();   // use to abort live download

private:
    VOID Close();

    VOID OnRequestError(
        LPWINHTTP_ASYNC_RESULT
        );
    VOID OnSendRequestComplete(
        );
    VOID OnHeadersAvailable(
        );
    VOID OnReadComplete(
        DWORD dwBytesRead
        );
    VOID OnHandleClosing(HINTERNET);

private:
    LIST_ENTRY  _List;

#ifdef ERROR
#undef ERROR
#endif

    enum STATE 
    {
        INIT,
        OPENED,
        ERROR,
        SENDING,
        RECEIVING,
        HEADERS_AVAILABLE,
        WAITING_FOR_DATA,
        DATA_AVAILABLE,
        DATA_EXHAUSTED,
        CLOSING,
        CLOSED
    };

    PWSTR m_pwszUrl;


    HINTERNET m_hConnect;
    HINTERNET m_hRequest;

    UINT m_nID;
    SIMPLE_BROWSER* m_pBrowser;

    BOOL m_fProxyAutoDiscovery;
    BOOL m_fProxyFailover;
    UINT m_nFailureRetries;

    WINHTTP_PROXY_INFO m_ProxyInfo;

    LPWSTR m_pwszNextProxies;

    DWORD m_dwLastError;
    DWORD m_dwErrorAPI;

    STATE m_State;

    BOOL m_fClosing;

    enum { READ_BUFFER_SIZE = 1024 };
    BYTE m_ReadBuffer[READ_BUFFER_SIZE];

    int m_ContentLength;
    DWORD m_dwBytesReadSoFar;

    DWORD m_dwStartingTime; // in millisecond
    DWORD m_dwTimeLimit;    // in millisecond
    int   m_TimeRemaining;  // in millsecond

    int m_nNumAttempts;

friend class SIMPLE_BROWSER;
friend void CALLBACK RequesterStatusCallback (
    HINTERNET hInternet,
    DWORD_PTR dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength
    );
};

#endif // _SIMPLE_BROWSER_H