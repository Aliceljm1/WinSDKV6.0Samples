// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include "CCWindow.h"
#include "strsafe.h"

extern HWND g_hWndShowBannerRB;          // Button to show "banner" with image appropriate to 
                                         // selected locale
extern HWND g_hWndShowCCWinRB;           // Button to show closed caption instead of "banner"

bool CCWindow::init(HWND hMainWnd, HINSTANCE hInst)
{
    m_bUseJapanese = false;

    // this STATIC control is the container for the closed 
    // caption window
    m_hPanel = CreateWindow(TEXT("STATIC"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | SS_ETCHEDFRAME,
        0, 0, 0, 0, hMainWnd, NULL, NULL, NULL);

    
    m_hPanel_CC = CreateWindow(TEXT("STATIC"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | SS_ETCHEDFRAME,
        0, 0, 0, 0, hMainWnd, NULL, NULL, NULL);    

    if ((m_hInstDll = LoadLibrary(TEXT("iectrl.dll"))))
    {
        // retrieve the entry point for the 4 major procedures exported by the dll
        m_lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_hInstDll, "EmbedBrowserObject");
        m_lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_hInstDll, "UnEmbedBrowserObject");
        m_lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)m_hInstDll, "DisplayHTMLPage");
        m_lpDisplayHTMLStr = (DisplayHTMLStrPtr *)GetProcAddress((HINSTANCE)m_hInstDll, "DisplayHTMLStr");
        (*m_lpEmbedBrowserObject)(m_hPanel);                     
        Navigate(NAV_PIC,NULL); 
        (*m_lpEmbedBrowserObject)(m_hPanel_CC);      
        m_pstrLastCaption = NULL;              
        Navigate(NAV_CC,NULL);
        return true;
     }
     else
     {    
         return false;
     }    
   
}

void CCWindow::unload()
{
    if(m_pstrLastCaption)
        delete[] m_pstrLastCaption;

    (*m_lpUnEmbedBrowserObject)(m_hPanel);
    (*m_lpUnEmbedBrowserObject)(m_hPanel_CC);

    FreeLibrary(m_hInstDll);
   
}

void CCWindow::OnSize(const RECT& clientRect)
{    

    int left = (int)((double)clientRect.right * 0.6);
    int width = clientRect.right - left - 4;
    
    int height = (int)((double)clientRect.bottom * 0.4);

    MoveWindow(m_hPanel, left + 2, 2, width, height, TRUE);

    int CCHeight = (int)((double)clientRect.bottom * 0.75) + 44;    
    int CCDepth = (int)((double)clientRect.bottom * 0.25) - 44;

    MoveWindow(m_hPanel_CC,left + 2,CCHeight, width, CCDepth, TRUE);

    IOleObject* browserObject = *((IOleObject**)GetWindowLong(m_hPanel, GWLP_USERDATA));
    browserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_IWebBrowser2);
    
    m_IWebBrowser2->put_Left(0);
    m_IWebBrowser2->put_Top(0);
    m_IWebBrowser2->put_Width(width-4);
    m_IWebBrowser2->put_Height(height-4);
    m_IWebBrowser2->Release();

    browserObject = *((IOleObject**)GetWindowLong(m_hPanel_CC, GWLP_USERDATA));
    browserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_IWebBrowser2_CC);
    
    m_IWebBrowser2_CC->put_Left(0);    
    m_IWebBrowser2_CC->put_Top(0);
    m_IWebBrowser2_CC->put_Width(width-4);   
    m_IWebBrowser2_CC->put_Height(CCDepth-4);
    m_IWebBrowser2_CC->Release(); 
   
    ShowBanner();
    ShowCaption();
    
}


void CCWindow::Navigate(__in int nMode, __in_ecount(MAX_PATH)LPCTSTR url)
{       
    
    (*m_lpDisplayHTMLPage)((nMode == NAV_PIC) ? m_hPanel : m_hPanel_CC, 
                           (url == NULL)      ? TEXT("about:blank") : url);          
}

// We have to treat Japanese language as a special case since the 
// input closed caption file can be in a format other than Unicode
// We need to be aware of the fact that we are dealing with 
// Japanese language now, and do some proper adjustment.
void CCWindow::SetJapanese(bool flag)
{
    m_bUseJapanese = flag;
}

void CCWindow::OnClosedCaption(BSTR text)
{
    int size = (int)_tcslen(text)+1;
    m_pstrLastCaption = new TCHAR[size];
    HRESULT hr = StringCchCopy(m_pstrLastCaption, size, text);   
	
    if(!m_bUseJapanese)
    {
        // not dealing with Japanese, we have no problem with
        // western character set
        (*m_lpDisplayHTMLStr)(m_hPanel_CC, text);
    }
    else
    {
        // we are handling Japanese language
        // the input closed caption file can be in a format other than Unicode
        // while the WMP control will always hand us the caption string in Unicode
        // So let us do a round-cycle conversion to make sure the string is in the
        // right format (i.e. the Unicode format)
        DWORD sizeBytes = WideCharToMultiByte(CP_ACP, 0, text, size+1, NULL, 0, NULL, NULL);
        char* output = new char[sizeBytes];
        if(WideCharToMultiByte(CP_ACP, 0, text, size+1, output, sizeBytes,  NULL, NULL))
        {
            // 932 is the codepage id for Japanese (Shift-JIS). That's the language
            // we support

            // query the size with a first call
            DWORD size2 = MultiByteToWideChar(932, 0, output, sizeBytes, NULL, 0);

            PTCHAR out = new TCHAR[size2];

            if(MultiByteToWideChar(932, 0, output, sizeBytes, out, size2))
            {
                (*m_lpDisplayHTMLStr)(m_hPanel_CC, out);
            }

            delete[] out;
        }

        delete[] output;
    }
}

void CCWindow::ShowWindow(int nCmdShow)
{
    ::ShowWindow(m_hPanel, nCmdShow);
    ::ShowWindow(m_hPanel_CC,nCmdShow);
}


void CCWindow::ShowCaption()
{	
    if(m_pstrLastCaption)
        (*m_lpDisplayHTMLStr)(m_hPanel_CC, m_pstrLastCaption);
    else
        Navigate(NAV_CC,NULL);    
}

void CCWindow::SetBanner(__in_ecount(MAX_PATH)LPCTSTR filename)
{
    HRESULT hr = StringCchCopy(m_pstrBannerFileName, MAX_PATH, filename);
}

// display the banner with a specific size using a webpage
void CCWindow::ShowBanner()
{
    RECT clientRect;
    int width, height;
    
	
    //300 is the default, we will resize it
    width = height = 300;
    TCHAR htmlString[4*MAX_PATH];
	TCHAR formatString[]=TEXT("<img src=\"%s\" width=\"%d\" height=\"%d\">");
    if(GetClientRect(m_hPanel, &clientRect))
    {
        width = clientRect.right - clientRect.left;
        height = clientRect.bottom - clientRect.top;
    }

    HRESULT hr = StringCchPrintf(htmlString, sizeof(htmlString)/sizeof(TCHAR),
        formatString, 
        m_pstrBannerFileName, width-50, height-40);
    if(FAILED(hr)) return;

    (*m_lpDisplayHTMLStr)(m_hPanel, htmlString);
    

}