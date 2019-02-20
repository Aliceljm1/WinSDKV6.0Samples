//////////////////////////////////////////////////////////////////////////////
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>   //for MapWindowRect 
#include <shlobj.h>     //for IExplorerBrowser, ICommDlgBrowser, IServiceProvider
#include <shellapi.h>   //for ShellExecuteEx and SHELLEXECUTEINFO
#include <shlwapi.h>    //for IUnknown_SetSite, SHCreateThread, StrRetToBufW
#include <propkey.h>    //for PKEY_DisplayName, PKEY_DisplayPath
#include "resource.h"  

#define KFD_SELCHANGE     WM_USER

HINSTANCE g_hinst = 0;
HRESULT DisplayNameOf(IShellFolder *psf, PCUITEMID_CHILD pidl, DWORD flags, PWSTR psz, UINT cch);


///////////////////////////////////////////////////////////////////////////////
// CKnownFolderDialog class
//
// Explorer Dialog class that can display known folders and can navigate to them
//
///////////////////////////////////////////////////////////////////////////////
class CKnownFolderDialog : public ICommDlgBrowser, public IServiceProvider
{
public:
    CKnownFolderDialog() : _cRef(1), _hdlg(NULL), _peb(NULL), _fEnumerated(FALSE), _pStream(NULL)
    {
    }

    HRESULT DoModal(HWND hwnd);   

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);  

    // IServiceProvider
    STDMETHODIMP QueryService(REFGUID guidService, REFIID riid, void **ppv);

    // ICommDlgBrowser - for callbacks from ExplorerBrowser
    STDMETHODIMP OnDefaultCommand(IShellView *psv);
    STDMETHODIMP OnStateChange(IShellView *psv, ULONG uChange);
    STDMETHODIMP IncludeObject(IShellView *psv, __in PCUITEMID_CHILD pidl);

private:
    //Callback functions
    static BOOL CALLBACK s_DlgProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL CALLBACK _DlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
  
    //InitDlg initializes the explorerbrowser settings
    void _OnInitDlg();

    //Event handler functions
    void    _OnDestroyDlg();
    static  DWORD WINAPI s_DoFolderEnum(void *pParam);
    void    _DoFolderEnum();
    void    _StartFolderEnum(IResultsFolder *prf);
    HRESULT _FillView(IResultsFolder *prf);
    void    _OnSelChange();

    LONG    _cRef; //reference counter
    HWND    _hdlg; //handle to the dialog

    IExplorerBrowser    *_peb;          //Explorer Dialog
    BOOL                _fEnumerated;   //folder enumerated already?
    IStream             *_pStream;      //used to marshall the folder results between threads
};


///////////////////////////////////////////////////////////////////////////////
// _OnInitDialog
//
// Cocreates and initializes IExplorerBrowser
// Illustrates the usage of Initialize, FillFromObject, GetCurrentView, SetOptions,
// methods of IExplorerBrowser and calls _StartFolderEnum for folder enumeration
//
///////////////////////////////////////////////////////////////////////////////
void CKnownFolderDialog::_OnInitDlg()
{
    HWND hwndStatic;
    if ((hwndStatic = GetDlgItem(_hdlg, IDC_FOLDERNAME)) != NULL)
    {
        ShowWindow(hwndStatic, SW_HIDE);
    }

    if ((hwndStatic = GetDlgItem(_hdlg, IDC_FOLDERPATH)) != NULL)
    {
        ShowWindow(hwndStatic, SW_HIDE);
    }

    if ((hwndStatic = GetDlgItem(_hdlg, IDC_LBLFOLDER)) != NULL)
    {
        ShowWindow(hwndStatic, SW_HIDE);
    }

    if ((hwndStatic = GetDlgItem(_hdlg, IDC_LBLPATH)) != NULL)
    {
        ShowWindow(hwndStatic, SW_HIDE);
    }
    
    hwndStatic = GetDlgItem(_hdlg, IDC_BROWSER);
    if (hwndStatic)
    {
        RECT rc;
        GetWindowRect(hwndStatic, &rc);
        MapWindowRect(HWND_DESKTOP, _hdlg, &rc);    //Aligns the dialog to the center

        //CoCreateInstance IExplorerBrowser
        HRESULT hr = CoCreateInstance(CLSID_ExplorerBrowser, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&_peb));
        if (SUCCEEDED(hr))
        {
            hr = IUnknown_SetSite(_peb, static_cast<IServiceProvider *>(this));
            if(SUCCEEDED(hr))
            {
                FOLDERSETTINGS fs = {0};
                
                //Set the foldersettings view mode to list mode. 
                fs.ViewMode = FVM_LIST;
                
                //Set the flags such that the folders autoarrange and non web view is presented
                fs.fFlags = FWF_AUTOARRANGE | FWF_NOWEBVIEW;   
                
                hr = _peb->Initialize(_hdlg, &rc, &fs);             //Initialize IExplorerBrowser
                if (SUCCEEDED(hr))
                {
                    //ExplorerBrowser options: EBO_NAVIGATEONCE ->Don't navigate after initial navigation
                    hr = _peb->SetOptions(EBO_NAVIGATEONCE);
                    if (SUCCEEDED(hr))
                    {
                        //passing NULL to FillFromObject will generate an empty results folder that you can 
                        //later manipulate yourself. EBF_NODROPTARGET:Don't allow dropping on the folder
                        hr = _peb->FillFromObject(NULL, EBF_NODROPTARGET); 
                        if (SUCCEEDED(hr))
                        {
                            IFolderView2 *pfv2;
                            hr = _peb->GetCurrentView(IID_PPV_ARGS(&pfv2));  //get the current view
                            if (SUCCEEDED(hr))
                            {
                                // IColumnManager is an interfaced provided by defview to
                                // allow the manipulation of columns 
                                IColumnManager *pcm;
                                
                                hr = pfv2->QueryInterface(IID_PPV_ARGS(&pcm));
                                if (SUCCEEDED(hr))
                                {
                                    PROPERTYKEY rgkeys[] = {PKEY_DisplayName, PKEY_DisplayPath};
                                    
                                    //Sets name and path as the available columns in the view
                                    hr = pcm->SetColumns(rgkeys, ARRAYSIZE(rgkeys));

                                    if (SUCCEEDED(hr))
                                    {
                                        //sets the current view mode to details view
                                        hr = pfv2->SetCurrentViewMode(FVM_DETAILS);

                                        if (SUCCEEDED(hr))
                                        {
                                            IResultsFolder *prf;
                                            
                                            hr = pfv2->GetFolder(IID_PPV_ARGS(&prf));
                                            if (SUCCEEDED(hr))
                                            {
                                                // Displayed when view is empty
                                                hr = _peb->SetEmptyText(L"No known folders yet...");  
                                                if (SUCCEEDED(hr))
                                                {
                                                    _StartFolderEnum(prf);
                                                    prf->Release();
                                                }
                                            }
                                        }
                                    }

                                    pcm->Release();
                                }

                                pfv2->Release();
                            }
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// _FillView
//
// Fill the explorer dialog with all the known folders using IKnownFolderManager
//
///////////////////////////////////////////////////////////////////////////////
HRESULT CKnownFolderDialog::_FillView(IResultsFolder *prf)
{
    IKnownFolderManager *pManager;
    HRESULT hr = CoCreateInstance(CLSID_KnownFolderManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pManager));

    if (SUCCEEDED(hr))
    {
        UINT cCount;
        KNOWNFOLDERID *pkfid;

        hr = pManager->GetFolderIds(&pkfid, &cCount);
        if (SUCCEEDED(hr))
        {
            for (UINT i = 0; i < cCount; i++)
            {
                IKnownFolder *pKnownFolder;

                hr = pManager->GetFolder(pkfid[i], &pKnownFolder);
                if (SUCCEEDED(hr))
                {
                    IShellItem *psi;

                    hr = pKnownFolder->GetShellItem(0, IID_PPV_ARGS(&psi));
                    if (SUCCEEDED(hr))
                    {
                        hr = prf->AddItem(psi);
                        psi->Release();
                    }

                    pKnownFolder->Release();
                }  
            }
        }

        CoTaskMemFree(pkfid); 
        pManager->Release();
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////////////
// s_DlgProc
//
// static callback function that gets called from CKnownFolderDialog::DoModal
// calls the _DlgProc method on CKnownFolderDialog
//
/////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CKnownFolderDialog::s_DlgProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CKnownFolderDialog *pcd; 
    
    if (uMsg == WM_INITDIALOG)
    {
        pcd = (CKnownFolderDialog *) lParam;
        pcd->_hdlg = hdlg;
        SetWindowLongPtr(hdlg, DWLP_USER, (LONG_PTR)pcd);
    }
    else
    {
        pcd = (CKnownFolderDialog*) (LONG_PTR) GetWindowLongPtr(hdlg, DWLP_USER);
    }

    return pcd ? pcd->_DlgProc(uMsg, wParam, lParam) : FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// DoModal
//
// DialogBox initialization. Gets called from WinMain
//
/////////////////////////////////////////////////////////////////////////////////
HRESULT CKnownFolderDialog::DoModal(HWND hwnd)
{
    DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)s_DlgProc, (LPARAM)this);
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// _OnSelChange
//
// Explorer dialog event handler function for select event. Displays the name and 
// path of the selected item in the explorer dialog
//
/////////////////////////////////////////////////////////////////////////////////
void CKnownFolderDialog::_OnSelChange()
{
    if (_fEnumerated)
    {
        IFolderView2 *pfv2;
        HRESULT hr = _peb->GetCurrentView(IID_PPV_ARGS(&pfv2));
        
        if (SUCCEEDED(hr))
        {
            int iItem;
        
            hr = pfv2->GetSelectedItem(-1, &iItem); //Returns S_FALSE if none selected
            if (S_OK == hr)
            {
                PITEMID_CHILD pidlSel = NULL;

                hr = pfv2->Item(iItem, &pidlSel);
                if (SUCCEEDED(hr))
                {
                    IShellFolder *psfRoot;

                    hr = pfv2->GetFolder(IID_PPV_ARGS(&psfRoot));
                    if (SUCCEEDED(hr))
                    {
                        WCHAR szFolderName[MAX_PATH];
                        
                        hr = DisplayNameOf(psfRoot, pidlSel, 0, szFolderName, ARRAYSIZE(szFolderName));
                        if (SUCCEEDED(hr))
                        {
                            SetDlgItemTextW(_hdlg, IDC_FOLDERNAME, szFolderName);
                            
                            WCHAR szFolderPath[MAX_PATH];
                            
                            hr = DisplayNameOf(psfRoot, pidlSel, SHGDN_FORPARSING, szFolderPath, ARRAYSIZE(szFolderPath));
                            if (SUCCEEDED(hr))
                            {
                                SetDlgItemTextW(_hdlg, IDC_FOLDERPATH, szFolderPath);                              
                                EnableWindow(GetDlgItem(_hdlg, IDC_EXPLORE), TRUE);
                            }
                        }

                        psfRoot->Release();
                    }

                    ILFree(pidlSel);
                }
            }
            else if (S_FALSE == hr)
            {
                SetDlgItemTextW(_hdlg, IDC_FOLDERNAME, L"");
                SetDlgItemTextW(_hdlg, IDC_FOLDERPATH, L"");
                EnableWindow(GetDlgItem(_hdlg,IDC_EXPLORE), FALSE);
            }
    
            pfv2->Release();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// _OnDestroyDlg
//
// Explorer dialog event handler function for destroy event
//
/////////////////////////////////////////////////////////////////////////////////
void CKnownFolderDialog::_OnDestroyDlg()
{
    if (_peb)
    {
        IUnknown_SetSite(_peb, NULL);
        _peb->Destroy();    //This call is necessary before calling release
        _peb->Release();
        _peb = NULL;
    }
}

//add additional data members in this struct that your worker thread might need
struct KNOWNFOLDERDIALOGPARAMS
{
    CKnownFolderDialog *pThis;
};

///////////////////////////////////////////////////////////////////////////////
// _StartFolderEnum
//
// Create a new thread to enumerate the folders
//
/////////////////////////////////////////////////////////////////////////////////
void CKnownFolderDialog::_StartFolderEnum(IResultsFolder *prf)
{
    KNOWNFOLDERDIALOGPARAMS *pkfdp = new KNOWNFOLDERDIALOGPARAMS;
    
    if (pkfdp)
    {
        pkfdp->pThis = this;
        AddRef();
        
        // Marshal the ResultsFolder into the worker thread, so we can add items to it
        HRESULT hr = CoMarshalInterThreadInterfaceInStream(IID_IResultsFolder, prf, &_pStream);
        
        if (SUCCEEDED(hr))
        {
            hr = SHCreateThread(s_DoFolderEnum, pkfdp, CTF_COINIT, NULL) ? S_OK : E_FAIL;
        }
        
        if (FAILED(hr))
        {
            Release();
            delete pkfdp;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// s_DoFolderEnum
//
// Static method that calls _DoFOlderEnum
//
/////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI CKnownFolderDialog::s_DoFolderEnum(__in void *pvParam)
{    
    KNOWNFOLDERDIALOGPARAMS *pkfdp = (KNOWNFOLDERDIALOGPARAMS*)pvParam;
    CKnownFolderDialog *pThis = pkfdp->pThis;;
        
    pThis->_DoFolderEnum();
    pThis->Release();
    delete pkfdp;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// _DoFolderEnum
//
// refreshes the UI labels and fills the explorer dialog view by calling _FillView
//
/////////////////////////////////////////////////////////////////////////////////
void CKnownFolderDialog::_DoFolderEnum()
{
    // Adjust dialog to show proper enumerating info and buttons
    SetDlgItemTextW(_hdlg, IDC_ENUMNAME, L"Starting Enumeration...");
    SetDlgItemTextW(_hdlg, IDC_ENUMPATH, L"");
    
    struct DIALOG_ITEMS
    {
        int iDlgItem;
        int iInitialShow;
        int iFinalShow;
    } dialogItems[] = {{IDC_STATUS,     SW_SHOW, SW_HIDE},
                       {IDC_ENUMNAME,   SW_SHOW, SW_HIDE},
                       {IDC_ENUMPATH,   SW_SHOW, SW_HIDE},
                       {IDC_FOLDERNAME, SW_HIDE, SW_SHOW},
                       {IDC_FOLDERPATH, SW_HIDE, SW_SHOW},
                       {IDC_LBLFOLDER,  SW_HIDE, SW_SHOW},
                       {IDC_LBLPATH,    SW_HIDE, SW_SHOW}};

    for (int i = 0; i < ARRAYSIZE(dialogItems); i++)
    {
        HWND hwndDlgItem = GetDlgItem(_hdlg, dialogItems[i].iDlgItem);

        if (hwndDlgItem != NULL)
        {
            ShowWindow(hwndDlgItem, dialogItems[i].iInitialShow);
        }
    }

    // UnMarshal the ResultsFolder so we can add items to it.
    IResultsFolder *prf;
    HRESULT hr = CoGetInterfaceAndReleaseStream(_pStream, IID_PPV_ARGS(&prf));
    
    _pStream = NULL;
    if (SUCCEEDED(hr))
    {
        hr = _FillView(prf);
        prf->Release();
    }

    _fEnumerated = TRUE;

    // Adjust dialog to show proper enumerating info and buttons
    for (int i = 0; i < ARRAYSIZE(dialogItems); i++)
    {
        HWND hwndDlgItem = GetDlgItem(_hdlg, dialogItems[i].iDlgItem);

        if (hwndDlgItem != NULL)
        {
            ShowWindow(hwndDlgItem, dialogItems[i].iFinalShow);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// _DlgProc
//
// Call the appropriate event handlers for the Explorer Dialog
//
/////////////////////////////////////////////////////////////////////////////////
BOOL CKnownFolderDialog::_DlgProc(UINT uMsg, WPARAM wParam, LPARAM)
{
    BOOL fResult = TRUE;
    
    switch (uMsg)
    {
    case WM_INITDIALOG:
        _OnInitDlg();
        break;
    
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
        case IDC_CANCEL:        
            fResult = EndDialog(_hdlg, TRUE);
        }

        break;        

    case KFD_SELCHANGE:
        _OnSelChange();
        break;
    
    case WM_DESTROY:
        _OnDestroyDlg();
        break;
        
    default:
        fResult = FALSE;
    }

    return fResult;
}

///////////////////////////////////////////////////////////////////////////////
// DisplayNameOf
//
// helper function to display the file name and path
//
/////////////////////////////////////////////////////////////////////////////////
HRESULT DisplayNameOf(IShellFolder *psf, PCUITEMID_CHILD pidl, DWORD flags, PWSTR psz, UINT cch)
{
    STRRET sr;
    HRESULT hr = psf->GetDisplayNameOf(pidl, flags, &sr);

    *psz = 0;
    if (SUCCEEDED(hr))
    {
        hr = StrRetToBufW(&sr, pidl, psz, cch);
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////////////
// QueryInterface, Addref, Release
//
// IUnknown method implementations of QueryInterface, AddRef and Release 
//
/////////////////////////////////////////////////////////////////////////////////
 STDMETHODIMP  CKnownFolderDialog::QueryInterface(REFIID riid, __deref_out void **ppv)
{
    HRESULT hr = E_NOINTERFACE;

    *ppv = NULL;

    if (IID_IUnknown == riid || IID_IServiceProvider == riid)
    {
        *ppv = static_cast<IServiceProvider *>(this);
    }
    else if (IID_ICommDlgBrowser == riid)
    {
        *ppv = static_cast<ICommDlgBrowser *>(this);
    }

    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

    return hr;
}

STDMETHODIMP_(ULONG)  CKnownFolderDialog::AddRef(void)
{
    return InterlockedIncrement(&_cRef);
}

STDMETHODIMP_(ULONG)  CKnownFolderDialog::Release(void)
{
    LONG cRef = InterlockedDecrement(&_cRef);
    
    if (!cRef)
    {
        delete this;
    }
    
    return cRef;
}

///////////////////////////////////////////////////////////////////////////////
// QueryService
//
// responding to this SID allows us to hook up our ICommDlgBrowser
// implementation so we get selection change events from the view
//
/////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP  CKnownFolderDialog::QueryService(REFGUID guidService, REFIID riid, void **ppv)
{
    HRESULT hr = E_NOINTERFACE;
    if (guidService == SID_SExplorerBrowserFrame)
    {
        hr = QueryInterface(riid, ppv);
    }
    else
    {
        *ppv = NULL;
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////////////
// OnDefaultCommand, OnStateChange, IncludeObject
//
// ICommDlgBrowser methods for handling callbacks from ExplorerBrowser
//
/////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP  CKnownFolderDialog::OnDefaultCommand(IShellView *) 
{ 
    _OnSelChange();
    return S_OK; 
}

STDMETHODIMP  CKnownFolderDialog::OnStateChange(IShellView *, ULONG uChange)  
{
    if (uChange == CDBOSC_SELCHANGE)
    {
        PostMessage(_hdlg, KFD_SELCHANGE, 0, 0);
    }

    return S_OK;
}

STDMETHODIMP  CKnownFolderDialog::IncludeObject(IShellView *, PCUITEMID_CHILD)  
{ 
    return S_OK; 
}

///////////////////////////////////////////////////////////////////////////////
// WinMain
//
// CoInitialize and instantiate the CKnownFolderDialog
//
/////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    //Initializes the thread for apartment-threaded object concurrency 
    //and disables DDE for Ole1 support
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    
    g_hinst = hInstance;
    
    if (SUCCEEDED(hr))
    {
        CKnownFolderDialog *pdlg = new CKnownFolderDialog();
        
        if (pdlg)
        {
            pdlg->DoModal(NULL);
            pdlg->Release();
        }

        CoUninitialize();
    }

    return 0;
}
