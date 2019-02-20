// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

/* ++ MUIAppSample.cpp : Defines the entry point for the application.

    This file demonstrates several of the National Language Support (NLS) 
    functions that support Multilingual User Interface (MUI) technology.
    In particular, it demonstrates use of:
    * EnumUILanguages
    * GetFileMUIPath    
    * GetUserDefaultUILanguage
    * GetThreadPreferredUILanguages
    * SetThreadPreferredUILanguages

    It also demonstrates the following functions, that are not as directly 
    related to MUI:
    * LCIDToLocaleName
    * LocaleNameToLCID
    * GetLocaleInfo
    * EnumResourceLanguages

    The sample also demonstrates the use of the LANGIDFROMLCID macro.
-- */

#include "stdafx.h"
#include <commdlg.h>
#include "MUIAppSample.h"

#include "MPController.h"
#include "CCWindow.h"
#include "IEController.h"
#include "GlobalConfig.h"
#include "strsafe.h"

#include <cmath>
#include <vector>


#define MAX_LOADSTRING      100
#define MAX_MAGIC_NUMBER    500
#define IDC_URLTB           10001
#define IDC_GOBTN           10002
#define IDC_PICTUREBOX      10003
#define IDC_MEDIAFILETB     10004
#define IDC_PLAYINMPBTN     10005
#define IDC_SHOWBANNERRB    10006
#define IDC_SHOWCCWINRB     10007
#define IDC_OPENFILEBTN     10008
#define MUISAMPLE_MEDIA_FILE TEXT("MediaFile\\multilang.wma")

// Global Variables:
GlobalConfig g_GlobalConfig;

HINSTANCE g_hInstModule;                // Instance of the module to be associated with the window.
TCHAR g_szTitle[MAX_LOADSTRING];        // Application title bar text
TCHAR g_szWindowClass[MAX_LOADSTRING];  // Main window class name
TCHAR g_szGo[MAX_LOADSTRING];           // Caption for "Go" button
TCHAR g_szPlayInMP[MAX_LOADSTRING];     // Caption for "Play In Media Player" button
TCHAR g_szShowBanner[MAX_LOADSTRING];   // Caption for "Show Banner" radio box
TCHAR g_szShowCCWin[MAX_LOADSTRING];    // Caption for "Show Closed Caption Window" radio box
TCHAR g_szMainURL[MAX_LOADSTRING];      // URL for web page
LPTSTR g_szUILanguage;                  // Name of current UI language (e.g. "en-US")

// Window handles
HWND g_hMainWnd;                        // Main window handle
HWND g_hWndUrlEdit;                     // URL edit window for embedded Internet Explorer window
HWND g_hWndGoBtn;                       // "Go" button
HWND g_hWndMediaFileEdit;               // Name of media file 
HWND g_hWndShowBannerRB;                // Button to show "banner" with image appropriate to 
                                        //   selected locale
HWND g_hWndShowCCWinRB;                 // Button to show closed caption instead of "banner"
HWND g_hWndOpenFileBtn;                 // "Open file" button
HICON g_hOpenFileIcon;                  // Icon for "Open file" button

MPController* g_pMPC = NULL;            // Windows Media Player
IEController* g_pIEC = NULL;            // Internet Explorer
CCWindow*     g_pCCW = NULL;            // Closed Caption

// object map for Windows Media Player
BEGIN_OBJECT_MAP(g_ObjectMap)
END_OBJECT_MAP()

//    Structure to list the UI languages supported by this application
struct LangEnumData
{
    LangEnumData(std::vector<std::wstring>& langStorage):appLanguages(&langStorage)
    {};
    bool enumResult;
    std::vector<std::wstring>* appLanguages;
};

// Forward declarations of functions included in this code module:
ATOM                    MyRegisterClass(HINSTANCE hInstance);
BOOL                    InitInstance(HINSTANCE, int);
BOOL CALLBACK           FindDefaultUserLang(__in_ecount(MAX_PATH)LPTSTR lpUILanguageString, LONG_PTR lParam);
LRESULT CALLBACK        WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK        About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK        UILanguageSetting(HWND, UINT, WPARAM, LPARAM);
HRESULT                 GetCurrentUILangName(LPTSTR szFileLangName, int cchFileLangName, bool bExtraNull = false);
bool                    InitGlobalConfig();
bool                    LoadBanner();
bool                    GetAppSupportedLangList(LangEnumData& enumData);
void                    OnSize();
void                    NavigateToURL();
void                    PlayMediaFileInMP();
void                    OpenMediaFile();
void                    FillUserAndSystemPreferredLanguages(HWND hDlg);
void                    FillApplicationLanguages(HWND hDlg);
void                    ReloadUI();
void                    LoadAllStringResources(HINSTANCE hInstance);
bool                    CreateMediaFilePath();

/*++ 
Function _tWinMain

Description
    The "main program"
    
--*/
int APIENTRY _tWinMain(__in HINSTANCE      hInstance,
                       __in_opt HINSTANCE  hPrevInstance,
                       __in_opt LPTSTR     lpCmdLine,
                       __in int            nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    HACCEL hAccelTable;
    LCID   ulLCID=0;
    BOOL   bFallBack=FALSE;

    InitGlobalConfig();

    // Initialize global strings
    LoadAllStringResources(hInstance);

    /*--
       Use GetLocaleInfo to retrieve the native-language name for the current default
        UI language. 

       Note that we make two calls to GetLocaleInfo. The first just determines
        how large a string buffer we will need; the second actually gets the string.
    --*/
    if ( (g_GlobalConfig.currDefaultLCID != 0x409) &&
         !_tcscmp(g_szShowBanner,MUI_SHOW_NAME))
    {
       ulLCID = 0x409;
       bFallBack=TRUE;
    }
    else
    {
        ulLCID = g_GlobalConfig.currDefaultLCID;
        bFallBack=FALSE;
    }
    int size = GetLocaleInfo(ulLCID, LOCALE_SNATIVELANGNAME, NULL, 0);
    g_szUILanguage = new TCHAR[size];
    if(GetLocaleInfo(ulLCID, LOCALE_SNATIVELANGNAME, g_szUILanguage, size) == size)
    {
        // Append native name of language and internal name to the title bar
        // For example, "MUIAppSample - Deutsch (de-DE)"
        HRESULT hr = StringCchCat(g_szTitle, MAX_LOADSTRING, TEXT(" - "));
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, g_szUILanguage);
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, TEXT(" ("));
        if (!bFallBack)
        {
           if(g_GlobalConfig.bUseOSLangSetting)
           {
               hr = StringCchCat(g_szTitle, MAX_LOADSTRING, g_GlobalConfig.osDefaultLang);
           }
           else
           {
               hr = StringCchCat(g_szTitle, MAX_LOADSTRING, g_GlobalConfig.appLangs[g_GlobalConfig.appDefaultLang].c_str());
           }
        }
        else
        {
            hr = StringCchCat(g_szTitle, MAX_LOADSTRING,EN_US_LOCALE_NAME);
        }
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, TEXT(")"));
    }

    // Register this application
    MyRegisterClass(hInstance);

    // Perform application initialization
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MUIAPPSAMPLE));

    // declare and initialize all controllers
    // a-jmabel >>> is it OK that we are making global variables point to stack
    //  variables? I realize that we get away with it because this is the "main"
    //  but this seems to me like bad practice.
    IEController iec;    // Internet Explorer
    CCWindow ccw;        // Closed captions
    MPController mpc;    // Windows Media Player

    g_pIEC = &iec;
    g_pCCW = &ccw;
    g_pMPC = &mpc;

    if(!iec.init(g_hMainWnd)) exit(0);
    if(!ccw.init(g_hMainWnd, hInstance)) exit(0);
    CoInitialize(0);
    if(!mpc.init(g_ObjectMap, hInstance, g_hMainWnd)) exit(0);

    LoadBanner();
   
    CreateMediaFilePath();

    OnSize();

    // Load an appropriate page into the Explorer window    
    SetWindowText(g_hWndUrlEdit, g_szMainURL);
    g_pIEC->Navigate(g_szMainURL);

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            // we have to intercept the WM_CHAR message here, otherwise
            // it will be consumed up by the WMP control, and the main
            // message loop will not be able to receive it
            if(msg.message == WM_CHAR)
            {
                HWND ctrl = GetFocus();
                if(msg.wParam == VK_RETURN)
                {
                    if(ctrl == g_hWndUrlEdit)
                    {
                        NavigateToURL();
                        continue;
                    }
                    else if(ctrl == g_hWndMediaFileEdit)
                    {
                        PlayMediaFileInMP();
                        continue;
                    }
                }
            }

            DispatchMessage(&msg);
        }
    }

    // release the resources

    g_pMPC->unload();
    CoUninitialize();
    g_pIEC->unload();
    g_pCCW->unload();

    //HouseKeeping();

    return (int) msg.wParam;
}

/*++ 
    ========= Initialization =========
--*/

/*++ 
Function InitGlobalConfig

Description
    Discover the System Default Language.
    Enumerate the languages for which this application provides resources.
    Identify the current User Default Language and make sure it is one of the supported languages.
    Set appropriate default language.
    
--*/
bool InitGlobalConfig()
{
    /*++ GetUserDefaultUILanguage returns the language id of the 
        current user's default UI language; we then convert that to
        a language name.

        LCIDToLocaleName does "double duty": it converts LANGIDs 
        to language names just like it converts LCIDs to locale names
    --*/
    BOOL            bResult = FALSE;
    ENUMUILANGPARAM EnumerUILangCallbackParam={0}; // zero out entire struct
    LANGID langID =   GetUserDefaultUILanguage(); 
    g_GlobalConfig.osDefaultLang = new TCHAR[MAX_LOCALE_NAME_LENGTH ];
    LCIDToLocaleName(langID, g_GlobalConfig.osDefaultLang, MAX_LOCALE_NAME_LENGTH , 0);

    // Initialize all the UI language settings

    // Enumerate the languages for which this application has resources
    LangEnumData enumData(g_GlobalConfig.appLangs);
    if(!GetAppSupportedLangList(enumData))
    {
        return false;
    }

    // Identify the current User's UI Language
    EnumerUILangCallbackParam.uCurrentUserUILanguage = GetUserDefaultUILanguage();

    // This call is trying to find out the default UI language 
    if(!EnumUILanguages(FindDefaultUserLang, MUI_LANGUAGE_ID, reinterpret_cast<LONG_PTR>(&EnumerUILangCallbackParam)))
        return false;
    
    //one more TCHAR to account for the terminating double-NULL
    TCHAR defaultLang[MAX_LOCALE_NAME_LENGTH + 1];
    ULONG ulNumOfLangs = 1;

    // Set to either the system language or a language selected from the list of resources.
    // (in practice, at this point this will always be system language)
    HRESULT hr= GetCurrentUILangName(defaultLang, MAX_LOCALE_NAME_LENGTH);

    // Convert from the language name to an LCID; this value will also always be a valid LANGID
    g_GlobalConfig.currDefaultLCID = LocaleNameToLCID(defaultLang, 0);

    // SetThreadPreferredUILanguages will specify the UI languages that should be used
    // for the current thread
    bResult = SetThreadPreferredUILanguages(0, NULL, &ulNumOfLangs);
    bResult = SetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, defaultLang, &ulNumOfLangs);

    return true;
}

/*++
Function FindDefaultUserLang

Description
   Callback function used by EnumUILanguage to find out Current user's UI language, comparing against
    the languages supported by the system 
   Calling code must assure that nCounter is set to zero before the call to EnumUILanguage.
   
--*/
BOOL CALLBACK FindDefaultUserLang(
              __in_ecount(MAX_PATH)LPTSTR lpUILanguageString, 
              LONG_PTR lParam)
{    
    LANGID           EnumeredLangID=0;
    PENUMUILANGPARAM pEnumparameter;

    if (!lParam)
    {
       return FALSE;
    }
    pEnumparameter = reinterpret_cast<PENUMUILANGPARAM>(lParam);

    //
    // Convert language name to language ID
    //    >>> a-jmabel In the next line, we are converting a LONG to a USHORT.
    //
    EnumeredLangID=(LANGID)_tcstol(lpUILanguageString, NULL, 16);
    
    //
    // Is returned Language equal to current user's UI language ?
    //
    if(EnumeredLangID == pEnumparameter->uCurrentUserUILanguage)
    {
        g_GlobalConfig.appDefaultLang = pEnumparameter->nCounter;
        return FALSE;
    }
    pEnumparameter->nCounter+=1;
    return TRUE;
}

/*++ 
Function EnumResLangProc

Description
    Callback function used by EnumResourceLanguages to determine for what UI languages
    this application provides resources. 
--*/
BOOL CALLBACK EnumResLangProc(
    HANDLE hModule,
    LPCWSTR lpszType,
    LPCWSTR lpszName,
    WORD wIDLanguage,
    LONG_PTR lParam
)
{
    if(!lParam)
    {
        return FALSE;
    }
    LangEnumData& enumData = *(reinterpret_cast<LangEnumData*>(lParam));
    WCHAR langName[10];

    if(!LCIDToLocaleName(wIDLanguage, langName, sizeof(langName)/sizeof(WCHAR), 0))
    {
        enumData.enumResult = false;
        return FALSE;
    }
    enumData.appLanguages->push_back(std::wstring(langName));
    return TRUE;
}

/*++ 
Function GetAppSupportedLangList

Description
    Returns the list of languages for which the app itself has localized resources.

    In this particular case the app enumerates all localized versions of its menu resource.
    We rely on the fact that all resources have been localized for the same set of languages.
    This will be true for any application that consists of a single LN file and the 
    corresponding MUI files.

--*/
bool GetAppSupportedLangList(LangEnumData& enumData)
{
    enumData.enumResult = true;
    HANDLE hModule = GetModuleHandleW(NULL);
    if(!hModule) return false;

    if(!EnumResourceLanguagesW( reinterpret_cast<HMODULE>(hModule),
                                RT_MENU,  
                                MAKEINTRESOURCE(IDC_MUIAPPSAMPLE), 
                                reinterpret_cast<ENUMRESLANGPROCW>(EnumResLangProc), 
                                reinterpret_cast<LPARAM>(&enumData)))
    {
        return false;
    }
    if(!enumData.enumResult)
    {
        return false;
    }
    return true;
}

/* +
  Function MyRegisterClass()

  PURPOSE: Registers the window class.

  Description
    This function and its usage are only necessary if you want this code
    to be compatible with Win32 systems prior to the 'RegisterClassEx'
    function that was added to Windows 95. It is important to call this function
    so that the application will get 'well formed' small icons associated
    with it.
--*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MUIAPPSAMPLE));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_MUIAPPSAMPLE);
    wcex.lpszClassName  = g_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

/*++
    Function InitInstance

    Purpose
        Saves instance handle and creates main window

    Description
        Save the instance handle in a global variable, then
        create and display the main program window.
--*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // Store instance handle in our global variable
    g_hInstModule = hInstance;

    g_hMainWnd = CreateWindow(g_szWindowClass, g_szTitle, WS_MAXIMIZE | WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_TABSTOP,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!g_hMainWnd)
    {
        return FALSE;
    }
    
    ShowWindow(g_hMainWnd, nCmdShow);
    BringWindowToTop(g_hMainWnd);
    UpdateWindow(g_hMainWnd);

    return TRUE;
}
/*++ 
    ========= Utility functions =========
    Abstractions of frequently repeated actions
--*/

/* +
    Function
        LoadAllStringResources

    Description
        Initialize all global strings: these global resources are loaded based on current
        UI language settings
-- */
void LoadAllStringResources(HINSTANCE hInstance)
{
    LoadString(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MUIAPPSAMPLE, g_szWindowClass, MAX_LOADSTRING);
    LoadString(hInstance, IDS_GOSTR, g_szGo, MAX_LOADSTRING);
    LoadString(hInstance, IDS_PLAYINMPSTR, g_szPlayInMP, MAX_LOADSTRING);
    LoadString(hInstance, IDS_SHOWBANNERSTR, g_szShowBanner, MAX_LOADSTRING);
    LoadString(hInstance, IDS_SHOWCCWINSTR, g_szShowCCWin, MAX_LOADSTRING);
    LoadString(hInstance, IDS_MAINURL, g_szMainURL, MAX_LOADSTRING);
}

/*++
    Function GetCurrentUILangName

    Description
        Returns (in buffer szFileLangName) the name of the currently selected UI language, 
        regardless of whether that comes from user/system preferences or an explicit choice
        from among the resources offered by this application.

        The parameter bExtraNull allows this to add an extra null character to the string, 
        because several of the functions that use this need to make this a multi-string list 
        (with a single element!), ending with TWO null characters. It's easier to program this in 
        one place.

        If we were being hyper-careful, when bExtraNull is true we'd subtract 1 from the length 
        of the buffer we pass to StringCchCopy, but we are assuming here that the caller of this
        function will pass in a buffer that is easily big enough for any reasonable values.
--*/
HRESULT GetCurrentUILangName(LPTSTR szFileLangName, int cchFileLangName, bool bExtraNull /* =false */)
{
    HRESULT hr;
    if(g_GlobalConfig.bUseOSLangSetting)
    {
        hr = StringCchCopy(szFileLangName, cchFileLangName, g_GlobalConfig.osDefaultLang);
    }
    else
    {
        hr = StringCchCopy(szFileLangName, cchFileLangName, g_GlobalConfig.appLangs[g_GlobalConfig.appDefaultLang].c_str());
    }

    if (bExtraNull)
    {
        // Add the second NULL of that terminating double-NULL.
        size_t cch; 
        hr= StringCchLength(szFileLangName, cchFileLangName, &cch);

        // Note that the following position is one past the already existing null terminator.
        szFileLangName[cch+1] = TEXT('\0');
    }
    
    return hr;
}

/*++ 
    ========= Window-related callbacks =========
/*++
    Function SetChildFont

    Description
        Callback function for setting the font for child windows
--*/
BOOL __stdcall SetChildFont(HWND hWnd, LPARAM lparam)
{
    HFONT hFont = (HFONT)lparam;
    SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
    return TRUE;
}

/*++ 
    ========= Main window =========
--*/
/*++
    Function WndProc

    Purpose
        Processes messages for the main window.

    WM_COMMAND    - process the application menu
    WM_PAINT    - Paint the main window
    WM_DESTROY    - post a quit message and return
--*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
    int width, bannerWidth, bannerHeight;
    HFONT hFont;

    switch (message)
    {
    case WM_CREATE:

        // this is to adjust the font used for the UI
        hFont = (HFONT)GetStockObject( DEFAULT_GUI_FONT );
        SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

        g_hWndUrlEdit = CreateWindow(TEXT("EDIT"), NULL,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_DLGFRAME | ES_LEFT | ES_AUTOHSCROLL,
            0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_URLTB),
            g_hInstModule, NULL);

        g_hWndGoBtn = CreateWindow(TEXT("BUTTON"), g_szGo,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_GOBTN),
            g_hInstModule, NULL);
        SendMessage(g_hWndGoBtn, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

        g_hWndShowBannerRB = CreateWindow(TEXT("BUTTON"), g_szShowBanner,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
            0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_SHOWBANNERRB),
            g_hInstModule, NULL);
        SendMessage(g_hWndShowBannerRB, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
        SendMessage (g_hWndShowBannerRB, BM_SETCHECK, 1, 0);

        //g_hWndShowCCWinRB = CreateWindow(TEXT("BUTTON"), g_szShowCCWin,
        //    WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
        //    0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_SHOWCCWINRB),
        //    g_hInstModule, NULL);
        g_hWndShowCCWinRB = CreateWindow(TEXT("STATIC"), g_szShowCCWin,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_DLGFRAME | ES_LEFT | ES_AUTOHSCROLL,
            0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_MEDIAFILETB),
            g_hInstModule, NULL);
        SendMessage(g_hWndShowCCWinRB, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

        g_hWndMediaFileEdit = CreateWindow(TEXT("STATIC"), NULL,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_DLGFRAME | ES_LEFT | ES_AUTOHSCROLL,
            0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_MEDIAFILETB),
            g_hInstModule, NULL);

        g_hWndOpenFileBtn = CreateWindow(TEXT("BUTTON"), NULL,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_ICON,
            0, 0, 0, 0, hWnd, reinterpret_cast<HMENU>(IDC_OPENFILEBTN),
            g_hInstModule, NULL);
        g_hOpenFileIcon = (HICON)LoadImage(g_hInstModule, MAKEINTRESOURCE(IDI_ICON_OPENFILE), 
            IMAGE_ICON,24,24, LR_LOADTRANSPARENT | LR_DEFAULTCOLOR);

        SendMessage(g_hWndOpenFileBtn, BM_SETIMAGE, (WPARAM)IMAGE_ICON, reinterpret_cast<LPARAM>(g_hOpenFileIcon));
        
        break;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInstModule, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
        case ID_OPTIONS_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_OPTIONS_UISETTINGS:
            DialogBox(g_hInstModule, MAKEINTRESOURCE(IDD_UILANGSETTING), hWnd, UILanguageSetting);
            break;
        case IDC_GOBTN:
            NavigateToURL();
            break;
        case IDC_OPENFILEBTN:
            OpenMediaFile();
            break;
        case IDC_PLAYINMPBTN:
            PlayMediaFileInMP();
            break;
        case IDC_SHOWBANNERRB:
            g_pCCW->ShowBanner();
            break;
        case IDC_SHOWCCWINRB:
            g_pCCW->ShowCaption();
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        width = (int)((double)rect.right * 0.6);
        MoveWindow(g_hWndUrlEdit, 0, 2, width-76, 24, TRUE);
        MoveWindow(g_hWndGoBtn, width-74, 2, 72, 24, TRUE);
        bannerWidth = rect.right - width - 4;
        bannerHeight = (int)((double)rect.bottom * 0.75);        
        MoveWindow(g_hWndMediaFileEdit, width + 2, bannerHeight, bannerWidth - 27, 24, TRUE);
        MoveWindow(g_hWndOpenFileBtn, rect.right - 27, bannerHeight, 24, 24, TRUE);  
        MoveWindow(g_hWndShowCCWinRB, width + 2, bannerHeight + 24, bannerWidth ,20, TRUE);
        OnSize();
        break;
    case WM_CONTEXTMENU:
        g_pMPC->OnContextMenu(message, wParam, lParam);
        break;
    case WM_DESTROY:
        g_pMPC->terminate();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


/*++
    Function OnSize

    Description
        Show the main window after resizing; we also use this as our way to force an update of
        the main window in other circumstances.
        
--*/
void OnSize()
{
    if(g_pIEC == NULL || g_pMPC == NULL)
        return;

    RECT rcClient;
    GetClientRect(g_hMainWnd, &rcClient);

    g_pIEC->OnSize(rcClient);
    g_pCCW->OnSize(rcClient);
    g_pMPC->OnSize(rcClient);
}

/*++ 
    ========= "About" window =========
--*/
/*++
    Function About

    Description
        Message handler for about box.
--*/
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    HFONT hFont;
    switch (message)
    {
    case WM_INITDIALOG:
        hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(hDlg, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
        EnumChildWindows(hDlg, SetChildFont, reinterpret_cast<LPARAM>(hFont));
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/*++ 
    ========= "Banner" window =========
--*/
/*++
    Function LoadBanner

    Description
        Loads an image associated with the locale of the current UI language.
        Note that this JPEG is a non-Win32 Resource, so weuse GetFileMUIPath to find the 
        language-appropriate JPEG. (The resource loader can't help us here.)
        
--*/
bool LoadBanner()
{
    BOOL        bResult;
    TCHAR       fileMUIPath[MAX_PATH]={0};
    ULONG       fileMUIPathLength = MAX_PATH;
    TCHAR       fileLangName[MAX_LOCALE_NAME_LENGTH ]={0};
    ULONG       fileLangNameLength = MAX_LOCALE_NAME_LENGTH;
    ULONGLONG   ullEnumerator = 0;

    HRESULT hr= GetCurrentUILangName(fileLangName, MAX_LOCALE_NAME_LENGTH);
       

    // GetFileMUIPath returns (in *fileMUIPath) the pathname of the file that matches
    // the current UI language and matches the queried-for name. In the second paramater, 
    // there is an implied path to the module that is running; alternatively, caller could
    // use an absolute path.

    // So, if your current UI language is de-DE, GetFileMUIPath will return
    // the path to the German version of the banner file (banner.jpg)
   

    bResult = GetFileMUIPath(0,
                             TEXT("banner.jpg"),
                             fileLangName, &fileLangNameLength,
                             fileMUIPath, &fileMUIPathLength,
                             &ullEnumerator);
    if(bResult)
    {
        g_pCCW->SetBanner(fileMUIPath);
        return true;
    }

    return false;
}

/*++ 
    ========= Interactive language-setting =========
--*/
/*++
    Function UILanguageSetting

    Description
        Message handler for UI Language Setting dialog.
--*/
INT_PTR CALLBACK UILanguageSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    HWND hWndOSRB, hWndUserRB, hWndComboBox;
    HFONT hFont;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(hDlg, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
        EnumChildWindows(hDlg, SetChildFont, reinterpret_cast<LPARAM>(hFont));
        hWndComboBox = GetDlgItem(hDlg, IDC_COMBO_USERLANGSETTING);
        if(g_GlobalConfig.bUseOSLangSetting)
        {
            hWndOSRB = GetDlgItem(hDlg, IDC_RADIO_OSLANGSETTING);
            SendMessage (hWndOSRB, BM_SETCHECK, 1, 0);
            EnableWindow(hWndComboBox, false);
        }
        else
        {
            hWndUserRB = GetDlgItem(hDlg, IDC_RADIO_USERLANGSETTING);
            SendMessage (hWndUserRB, BM_SETCHECK, 1, 0);
            EnableWindow(hWndComboBox, true);
        }
        FillUserAndSystemPreferredLanguages(hDlg);
        FillApplicationLanguages(hDlg);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch(wmId)
        {
        case IDOK:
            hWndOSRB = GetDlgItem(hDlg, IDC_RADIO_OSLANGSETTING);
            if((int)SendMessage (hWndOSRB, BM_GETCHECK, 0, 0))
                g_GlobalConfig.bUseOSLangSetting = true;
            else
            {
                g_GlobalConfig.bUseOSLangSetting = false;
                hWndComboBox = GetDlgItem(hDlg, IDC_COMBO_USERLANGSETTING);
                int index = (int)SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0);
                if(index != CB_ERR)
                {
                    g_GlobalConfig.appDefaultLang  = index;
                }
            }
            ReloadUI();
            g_pMPC->ReloadMediaFile();
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_RADIO_OSLANGSETTING:
            hWndComboBox = GetDlgItem(hDlg, IDC_COMBO_USERLANGSETTING);
            EnableWindow(hWndComboBox, false);
            return (INT_PTR)TRUE;
        case IDC_RADIO_USERLANGSETTING:
            hWndComboBox = GetDlgItem(hDlg, IDC_COMBO_USERLANGSETTING);
            EnableWindow(hWndComboBox, true);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/*++
    Function FillUserAndSystemPreferredLanguages

    Description
        Display the User/System Preferred UI Languages list
        Note that as a side effect, this clears out the Thread Preferred UI Languages list.
         If you didn't want that side effect, you could save that list with 
         GetThreadPreferredUILanguages() as a first step, then restore it with
         SetThreadPreferredUILanguages() as a last step.
        
--*/
void FillUserAndSystemPreferredLanguages(HWND hDlg)
{
    ULONG   ulNumOfLangs = 0;
    ULONG   ulAdded      = 0;
    PTSTR   ptstrLangsBuffer = NULL;
    PTSTR   outputBuffer=NULL;
    PTSTR   pstrBuffer;
    ULONG   ulBufferSize = 0;
    LCID    lcid;
    size_t  tcharsPrinted = 0;
    HRESULT hr;     
      
    TCHAR   szUILanguage[MAX_LOCALE_NAME_LENGTH ]={0};
    TCHAR    ItemBuffer[MAX_LOCALE_NAME_LENGTH*2]={0};
    
    outputBuffer=new WCHAR[MAX_LOCALE_NAME_LENGTH*10];
    
    if (!outputBuffer)
    {
        goto Fail;
    }

    HWND hWndLBox = GetDlgItem(hDlg, IDC_LIST_OSLANGSETTING);
    if(!hWndLBox) {return;}

    // clear out any explicitly set Thread Preferred UI Languages 
    if(!SetThreadPreferredUILanguages(0, NULL, &ulNumOfLangs))
    {
        goto Fail;
    }
    // The following calls will give us a merged list: User Preferred UI Languages (if any),
    // then System Preferred UI Languages (if there were no User Preferred UI Languages), and continuing
    // to fall back clear to the machine's install language if necessary. (If we hadn't cleared out
    // the Thread Preferred UI Languages first, those would get precedence.)
    // We call GetThreadPreferredUILanguages() twice: the first call is just to get buffer size.
    GetThreadPreferredUILanguages(MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK | MUI_LANGUAGE_NAME, 
                                    &ulNumOfLangs, 
                                    ptstrLangsBuffer, 
                                    &ulBufferSize);

    ptstrLangsBuffer = new WCHAR[ulBufferSize*sizeof(WCHAR)];
    if (!ptstrLangsBuffer)
    {
        goto Fail;
    }
    if(!GetThreadPreferredUILanguages(MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK | MUI_LANGUAGE_NAME, 
                                                        &ulNumOfLangs, 
                                                        ptstrLangsBuffer, 
                                                        &ulBufferSize))
    {
        goto Fail;
    }
  
     // language names are delimited by NULL, and the buffer terminates with two NULLs
    outputBuffer[0]=NULL;
    for(ULONG i=0; i<ulNumOfLangs; i++)
    {  
        pstrBuffer = ptstrLangsBuffer+tcharsPrinted;
        
        if ( _tcschr(pstrBuffer,TEXT('-'))  )
        {              
          lcid = LocaleNameToLCID(pstrBuffer, 0);    
       
          if(GetLocaleInfo(lcid, LOCALE_SNATIVELANGNAME, szUILanguage,MAX_PATH ))
          {                                    
             StringCchPrintf(ItemBuffer, MAX_LOCALE_NAME_LENGTH *2, TEXT("%s [%s]\n"), szUILanguage,pstrBuffer);
             StringCchCat(outputBuffer,MAX_LOCALE_NAME_LENGTH*10,ItemBuffer);            
             ulAdded++;
          }
        }
        tcharsPrinted += _tcslen(pstrBuffer) + 1;                       
    }
    SetWindowText(hWndLBox,outputBuffer);
  
Fail:
    if (outputBuffer)
    {
       delete[] outputBuffer;
    }
    if (ptstrLangsBuffer)
    {
       delete[] ptstrLangsBuffer;
    }      
    return;
}

/*++
    Function FillApplicationLanguages

    Description
        Display list of languages for which this application has resources.
        List was previously filled in using GetAppSupportedLangList(), which called
        EnumResourceLanguagesW()
                
--*/
void FillApplicationLanguages(HWND hDlg)
{
    HWND hWndComboBox = GetDlgItem(hDlg, IDC_COMBO_USERLANGSETTING);
    if(!hWndComboBox) {return;}

    TCHAR szTemp[MAX_LOADSTRING];
    TCHAR szLanguageString[MAX_LOADSTRING];

    for(unsigned int i = 0; i < g_GlobalConfig.appLangs.size(); i++)
    {
        if(g_GlobalConfig.appLangs[i].length())
        {
            LCID lcid = LocaleNameToLCID(g_GlobalConfig.appLangs[i].c_str(), 0);
            GetLocaleInfo(lcid, LOCALE_SNATIVELANGNAME, szLanguageString, MAX_LOADSTRING);
            HRESULT hr = StringCchPrintf(szTemp, MAX_LOADSTRING, TEXT("%s [%s]"), szLanguageString, g_GlobalConfig.appLangs[i].c_str());
            SendMessage(hWndComboBox, CB_ADDSTRING, i, reinterpret_cast<LPARAM>(szTemp));
        }
        else
            break;
    }

    // denote the current default UI language
    SendMessage(hWndComboBox, CB_SETCURSEL, g_GlobalConfig.appDefaultLang, 0);
}

/*++
    Function ReloadUI

    Description
        When the user changes the UI language setting, we want to 
        refresh the user interface of the software immediately without
        relaunching it.  This function does that job.
--*/
void ReloadUI()
{
    // one more TCHAR to account for the terminating double-NULL for SetThreadPreferredUILanguages()
    TCHAR defaultLang[MAX_LOCALE_NAME_LENGTH + 1];
    ULONG ulNumOfLangs = 1;
    BOOL  bResult=FALSE;
    LCID  ulLCID=0;

    HRESULT hr= GetCurrentUILangName(defaultLang, MAX_LOCALE_NAME_LENGTH, true /* add extra null */);

    g_GlobalConfig.currDefaultLCID = LocaleNameToLCID(defaultLang, 0);

    // Change the preferred UI languages list for the main thread
    // Effectively, this thread tells the resource loader "I know what UI language
    //  I want. Don't worry about user or system preferences except as a fallback if
    //  my chosen resources are unavailable: just load the resources for this language."
    //  In this case, the app is setting a single language; an app could give a list 
    //  of up to five languages to try before falling back to user or system preferences.
    bResult = SetThreadPreferredUILanguages(0, NULL, &ulNumOfLangs);
    bResult = SetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, defaultLang, &ulNumOfLangs);

    // Now that the app has said what language's resources it wants, 
    //  reload all the resources we are using. 
    LoadAllStringResources(g_hInstModule);

    if ( (g_GlobalConfig.currDefaultLCID != 0x409) &&
         !_tcscmp(g_szShowBanner,MUI_SHOW_NAME))
    {
       ulLCID = 0x409;
    }
    else
    {
        ulLCID = g_GlobalConfig.currDefaultLCID;
    }
    
    int size = GetLocaleInfo(ulLCID, LOCALE_SNATIVELANGNAME, NULL, 0);
    delete[] g_szUILanguage;
    g_szUILanguage = new TCHAR[size];    

    if(GetLocaleInfo(ulLCID, LOCALE_SNATIVELANGNAME, g_szUILanguage, size) == size)
    {
        HRESULT hr = StringCchCat(g_szTitle, MAX_LOADSTRING, TEXT(" - "));
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, g_szUILanguage);
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, TEXT(" ("));
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, defaultLang);
        hr = StringCchCat(g_szTitle, MAX_LOADSTRING, TEXT(")"));
    }

    SetWindowText(g_hMainWnd, g_szTitle);
    SetWindowText(g_hWndGoBtn, g_szGo);    
    SetWindowText(g_hWndShowBannerRB, g_szShowBanner);
    SetWindowText(g_hWndShowCCWinRB, g_szShowCCWin);

    HMENU mainMenu = LoadMenu(g_hInstModule, MAKEINTRESOURCE(IDC_MUIAPPSAMPLE));
    if(mainMenu)
    {
        HMENU oldMenu = GetMenu(g_hMainWnd);
        if(SetMenu(g_hMainWnd, mainMenu))
        {
            DestroyMenu(oldMenu);
        }
    }   
    SetWindowText(g_hWndUrlEdit, g_szMainURL);
    g_pIEC->Navigate(g_szMainURL);
    LoadBanner();    
    g_pCCW->ShowBanner();
    g_pCCW->Navigate(NAV_CC,NULL);
}


/*++ 
    ========= Embedded Internet Explorer Window =========
--*/
/*++
    Function NavigateToURL

    Description
        Reload the content of the Internet Explorer browser window based on a URL
        
--*/
void NavigateToURL()
{
    TCHAR szURL[MAX_PATH];
    GetWindowText(g_hWndUrlEdit, szURL, MAX_PATH);
    g_pIEC->Navigate(szURL);
}

/*++ 
    ========= Windows Media Player =========
--*/
/*++
    Function OpenMediaFile

    Description
        Use a common dialog to ask the user to identify a media file;
        place the name of that file in a visible window. The copy in the
        window actually functions as our storage of this name.
        
--*/
void OpenMediaFile()
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH]={0};
    TCHAR szMUIAppFolder[MAX_PATH]={0};
    TCHAR *lpszPath=NULL;
    TCHAR *lpszNext=NULL;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hMainWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = UNICODE_NULL;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("Windows Media File\0*.wma\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrInitialDir=szMUIAppFolder;
   
    GetWindowText(g_hWndMediaFileEdit, szMUIAppFolder,MAX_PATH);
    lpszPath = szMUIAppFolder;
    while ( (lpszNext=_tcschr(lpszPath,TEXT('\\')))  )
    {    
          lpszPath = lpszNext+1;
    }
    *lpszPath=TEXT('\0');


    // GetOpenFileName displays the "Open File" common dialog
    if(GetOpenFileName(&ofn))
    {
        SetWindowText(g_hWndMediaFileEdit, ofn.lpstrFile);
        PlayMediaFileInMP();
    }
}

bool CreateMediaFilePath()
{
    TCHAR    szMUIAppFolder[MAX_PATH+1]={0};
    LPTSTR   lpszPath,lpszNext=NULL;        
    TCHAR    szCurrentLanguage[MAX_LOCALE_NAME_LENGTH ];
    LCID     idCurrentLanguage;
    bool     bResult = FALSE;
    
    if (::GetModuleFileName(NULL, szMUIAppFolder, MAX_PATH) )
    {                     
       lpszPath = szMUIAppFolder;
       while ( (lpszNext=_tcschr(lpszPath,TEXT('\\')))  )
       {    
            lpszPath = lpszNext+1;
       }
       *lpszPath=TEXT('\0');
       StringCchCat(szMUIAppFolder,MAX_PATH,MUISAMPLE_MEDIA_FILE);   
       if (GetFileAttributes(szMUIAppFolder) != (DWORD)-1)
       {
          SetWindowText(g_hWndMediaFileEdit, szMUIAppFolder);

          GetCurrentUILangName(szCurrentLanguage, MAX_LOCALE_NAME_LENGTH);
          idCurrentLanguage = LocaleNameToLCID(szCurrentLanguage, 0);
          g_pMPC->PlayFile(szMUIAppFolder, LANGIDFROMLCID(idCurrentLanguage),FALSE);
          bResult = TRUE;
       }
     }
     return bResult;
}

/*++
    Function PlayMediaFileInMP

    Description
        Look in the window where OpenMediaFile() placed a pathname;
        Launch the Windows Media Player to play that file.
        
--*/
void PlayMediaFileInMP()
{
    TCHAR szFilename[MAX_PATH];
    TCHAR szCurrentLanguage[MAX_LOCALE_NAME_LENGTH ];
    LCID idCurrentLanguage;

    SendMessage (g_hWndShowCCWinRB, BM_CLICK, 1, 0);
    GetWindowText(g_hWndMediaFileEdit, szFilename, MAX_PATH);

    HRESULT hr= GetCurrentUILangName(szCurrentLanguage, MAX_LOCALE_NAME_LENGTH);

    idCurrentLanguage = LocaleNameToLCID(szCurrentLanguage, 0);
    g_pMPC->PlayFile(szFilename, LANGIDFROMLCID(idCurrentLanguage),TRUE);
}
