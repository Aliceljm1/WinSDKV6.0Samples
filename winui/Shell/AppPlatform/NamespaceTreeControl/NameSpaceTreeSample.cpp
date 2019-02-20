#define STRICT_TYPED_ITEMIDS
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <strsafe.h>

#include "resource.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "propsys.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

// pass a NULL shell item to cleanup the allocated bitmap

HRESULT SetItemImageImageInStaticControl(__in HWND hwndStatic, __in_opt IShellItem *psi)
{
    HBITMAP hbmp = NULL;
    HRESULT hr = S_OK;
    if (psi)
    {
        IShellItemImageFactory *psiif;
        hr = psi->QueryInterface(IID_PPV_ARGS(&psiif));
        if (SUCCEEDED(hr))
        {
            RECT rc;
            GetWindowRect(hwndStatic, &rc);
            UINT dxdy = min(rc.right - rc.left, rc.bottom - rc.top);    // make it square
            SIZE size = { dxdy, dxdy };

            hr = psiif->GetImage(size, SIIGBF_RESIZETOFIT, &hbmp);
            psiif->Release();
        }
    }

    if (SUCCEEDED(hr))
    {
        HGDIOBJ hgdiOld = (HGDIOBJ) SendMessage(hwndStatic, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hbmp);
        if (hgdiOld)
        {
            DeleteObject(hgdiOld);  // if there was an old one clean it up
        }
    }

    return hr;
}

HINSTANCE g_hinst = 0;

class CNameSpaceTreeHost : public IServiceProvider, public INameSpaceTreeControlEvents
{
public:
    CNameSpaceTreeHost() : _cRef(1), _hdlg(NULL), _pnstc(NULL), _dwAdviseCookie(0)
    {
    }

    HRESULT DoModal(HWND hwnd)
    {
        DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, s_DlgProc, (LPARAM)this);
        return S_OK;
    }
    
    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv)
    {
        static const QITAB qit[] = 
        {
            QITABENT(CNameSpaceTreeHost, IServiceProvider),
            QITABENT(CNameSpaceTreeHost, INameSpaceTreeControlEvents),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        LONG cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
            delete this;
        return cRef;
    }
    
    // IServiceProvider
    IFACEMETHODIMP QueryService(REFGUID guidService, REFIID riid, void **ppv)
    {
        HRESULT hr = E_NOINTERFACE;
        *ppv = NULL;
        return hr;
    }

    // INameSpaceTreeControlEvents
    IFACEMETHODIMP OnItemClick(IShellItem *psi, NSTCEHITTEST nstceHitTest, NSTCECLICKTYPE nstceClickType) { return S_FALSE;  }
    IFACEMETHODIMP OnPropertyItemCommit(IShellItem *psi) { return S_FALSE; }
    IFACEMETHODIMP OnItemStateChanging(IShellItem *psi, NSTCITEMSTATE nstcisMask, NSTCITEMSTATE nstcisState) {  return S_OK;  }
    IFACEMETHODIMP OnItemStateChanged(IShellItem *psi, NSTCITEMSTATE nstcisMask, NSTCITEMSTATE nstcisState) { return S_OK; }

    IFACEMETHODIMP OnSelectionChanged(IShellItemArray *psiaSelection) 
    { 
        IShellItem *psi;
        HRESULT hr = psiaSelection->GetItemAt(0, &psi);
        if (SUCCEEDED(hr))
        {
            IShellItem2 *psi2;
            hr = psi->QueryInterface(IID_PPV_ARGS(&psi2));
            if (SUCCEEDED(hr))
            {
                _InspectItem(psi2);
                psi2->Release();
            }
            psi->Release();
        }
        return S_OK; 
    }
    IFACEMETHODIMP OnKeyboardInput(UINT uMsg, WPARAM wParam, LPARAM lParam)  { return S_FALSE; }
    IFACEMETHODIMP OnBeforeExpand(IShellItem *psi) { return S_OK; }
    IFACEMETHODIMP OnAfterExpand(IShellItem *psi) { return S_OK; }
    IFACEMETHODIMP OnBeginLabelEdit(IShellItem *psi) { return S_OK; }
    IFACEMETHODIMP OnEndLabelEdit(IShellItem *psi) { return S_OK; }
    IFACEMETHODIMP OnGetToolTip(IShellItem *psi, LPWSTR pszTip, int cchTip) { return E_NOTIMPL; }
    IFACEMETHODIMP OnBeforeItemDelete(IShellItem *psi) { return E_NOTIMPL; }
    IFACEMETHODIMP OnItemAdded(IShellItem *psi, BOOL fIsRoot) { return E_NOTIMPL; }
    IFACEMETHODIMP OnItemDeleted(IShellItem *psi, BOOL fIsRoot) { return E_NOTIMPL; }
    IFACEMETHODIMP OnBeforeContextMenu(IShellItem *psi, REFIID riid, void **ppv) {  *ppv = NULL; return E_NOTIMPL; }
    IFACEMETHODIMP OnAfterContextMenu(IShellItem *psi, IContextMenu *pcmIn, REFIID riid, void **ppv) { *ppv = NULL; return E_NOTIMPL; }
    IFACEMETHODIMP OnBeforeStateImageChange(IShellItem *psi) { return S_OK; }
    IFACEMETHODIMP OnGetDefaultIconIndex(IShellItem *psi, int *piDefaultIcon, int *piOpenIcon) { return E_NOTIMPL; }

private:
    ~CNameSpaceTreeHost()
    {
    }

    static BOOL CALLBACK s_DlgProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        #pragma warning(push)
        #pragma warning(disable:4312) // GetWindowLongPtr is not w4 clean
        CNameSpaceTreeHost *pnsth = reinterpret_cast<CNameSpaceTreeHost*>(GetWindowLongPtr(hdlg, DWLP_USER));
        #pragma warning(pop)

        if (uMsg == WM_INITDIALOG)
        {
            pnsth = reinterpret_cast<CNameSpaceTreeHost*>(lParam);
            pnsth->_hdlg = hdlg;
            #pragma warning(push)
            #pragma warning(disable:4244) // SetWindowLongPtr is not w4 clean
            SetWindowLongPtr(hdlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pnsth));
            #pragma warning(pop)
        }
        return pnsth ? pnsth->_DlgProc(uMsg, wParam, lParam) : FALSE;
    }

    BOOL _DlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void _OnInitDlg();
    void _OnDestroyDlg();
    void _OnExplore();
    void _InspectItem(IShellItem2 *psi);

    long _cRef;
    HWND _hdlg;
    DWORD _dwAdviseCookie;
    INameSpaceTreeControl *_pnstc;
};

void _SetDialogIcon(HWND hdlg, SHSTOCKICONID siid)
{
    SHSTOCKICONINFO sii = {sizeof(sii)};
    if (SUCCEEDED(SHGetStockIconInfo(siid, SHGFI_ICON | SHGFI_SMALLICON, &sii)))
    {
        SendMessage(hdlg, WM_SETICON, ICON_SMALL, (LPARAM) sii.hIcon);
    }
    if (SUCCEEDED(SHGetStockIconInfo(siid, SHGFI_ICON | SHGFI_LARGEICON, &sii)))
    {
        SendMessage(hdlg, WM_SETICON, ICON_BIG, (LPARAM) sii.hIcon);
    }
}

void _ClearDialogIcon(HWND hdlg)
{
    DestroyIcon((HICON)SendMessage(hdlg, WM_GETICON, ICON_SMALL, 0));
    DestroyIcon((HICON)SendMessage(hdlg, WM_GETICON, ICON_BIG, 0));
}

void CNameSpaceTreeHost::_OnInitDlg()
{
    _SetDialogIcon(_hdlg, SIID_APPLICATION);

    HWND hwndStatic = GetDlgItem(_hdlg, IDC_BROWSER);
    if (hwndStatic)
    {
        RECT rc;
        GetWindowRect(hwndStatic, &rc);
        MapWindowRect(HWND_DESKTOP, _hdlg, &rc);

        HRESULT hr = CoCreateInstance(CLSID_NamespaceTreeControl, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&_pnstc));
        if (SUCCEEDED(hr))
        {
            _pnstc->TreeAdvise(static_cast<INameSpaceTreeControlEvents *>(this), &_dwAdviseCookie);

            IUnknown_SetSite(_pnstc, static_cast<IServiceProvider *>(this));

            hr = _pnstc->Initialize(_hdlg, &rc, NSTCS_HASEXPANDOS | NSTCS_SINGLECLICKEXPAND | NSTCS_SPRINGEXPAND | NSTCS_SHOWSELECTIONALWAYS);
            if (SUCCEEDED(hr))
            {
                IShellItem *psi;
                hr = SHCreateItemInKnownFolder(FOLDERID_Desktop, 0, NULL, IID_PPV_ARGS(&psi));
                // the control panel
                // hr = SHCreateItemFromParsingName(L"::{26EE0668-A00A-44D7-9371-BEB064C98683}", IID_PPV_ARGS(&psi));
                if (SUCCEEDED(hr))
                {
                    hr = _pnstc->AppendRoot(psi, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, NSTCRS_VISIBLE | NSTCRS_EXPANDED, NULL);
                    psi->Release();
                }
            }
        }
    }
}

void CNameSpaceTreeHost::_OnDestroyDlg()
{
    _ClearDialogIcon(_hdlg);

    // cleanup the allocated HBITMAP
    SetItemImageImageInStaticControl(GetDlgItem(_hdlg, IDC_IMAGE), NULL);   

    if (_pnstc)
    {
        if (_dwAdviseCookie != -1)
        {
            _pnstc->TreeUnadvise(_dwAdviseCookie);
            _dwAdviseCookie = -1;
        }

        IUnknown_SetSite(_pnstc, NULL);
        _pnstc->Release();
        _pnstc = NULL;
    }
}

void CNameSpaceTreeHost::_OnExplore()
{
    IShellItemArray *psiaItems;
    HRESULT hr = _pnstc->GetSelectedItems(&psiaItems);
    if (SUCCEEDED(hr))
    {
        DWORD iCount;
        hr = psiaItems->GetCount(&iCount);
        if (SUCCEEDED(hr))
        {
            for (DWORD i = 0; (i < iCount) && SUCCEEDED(hr); i++)
            {
                IShellItem *psi;
                hr = psiaItems->GetItemAt(i, &psi);
                if (SUCCEEDED(hr))
                {
                    PIDLIST_ABSOLUTE pidl;
                    hr = SHGetIDListFromObject(psi, &pidl);
                    if (SUCCEEDED(hr))
                    {
                        SHELLEXECUTEINFO ei = { sizeof(ei) };
                        ei.fMask = SEE_MASK_INVOKEIDLIST;
                        ei.hwnd = _hdlg;
                        ei.nShow = SW_NORMAL;
                        ei.lpIDList = pidl;

                        ShellExecuteEx(&ei);
                        ILFree(pidl);
                    }
                    psi->Release();
                }
            }
        }
        psiaItems->Release();
    }
}

const SFGAOF sfgaofAll = 
    SFGAO_CANCOPY           |
    SFGAO_CANMOVE           |
    SFGAO_CANLINK           |
    SFGAO_STORAGE           |
    SFGAO_CANRENAME         |
    SFGAO_CANDELETE         |
    SFGAO_HASPROPSHEET      |
    SFGAO_DROPTARGET        |
    SFGAO_ENCRYPTED         |
    SFGAO_ISSLOW            |
    SFGAO_GHOSTED           |
    SFGAO_SHARE             |
    SFGAO_READONLY          |
    SFGAO_HIDDEN            |
//    SFGAO_HASSUBFOLDER      |
    SFGAO_REMOVABLE         |
    SFGAO_COMPRESSED        |
    SFGAO_BROWSABLE         |
    SFGAO_NONENUMERATED     |
    SFGAO_NEWCONTENT        |
    SFGAO_LINK              |
    SFGAO_STREAM            |
    SFGAO_FILESYSTEM        |
    SFGAO_FILESYSANCESTOR   |
    SFGAO_FOLDER            |
    SFGAO_STORAGEANCESTOR;

HRESULT ShellAttributesToString(SFGAOF sfgaof, PWSTR *ppsz)
{
    *ppsz = NULL;

    WCHAR sz[512] = {0};
    if (sfgaof & SFGAO_FOLDER)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"FOLDER ");
    }
    if (sfgaof & SFGAO_LINK)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"LINK ");
    }
    if (sfgaof & SFGAO_STREAM)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"STREAM ");
    }
    if (sfgaof & SFGAO_FILESYSTEM)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"FILESYSTEM ");
    }
    if (sfgaof & SFGAO_FILESYSANCESTOR)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"FILESYSANCESTOR ");
    }
    if (sfgaof & SFGAO_CANCOPY)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"CANCOPY ");
    }
    if (sfgaof & SFGAO_CANLINK)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"CANLINK ");
    }
    if (sfgaof & SFGAO_CANMOVE)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"CANMOVE ");
    }
    if (sfgaof & SFGAO_STORAGE)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"STORAGE ");
    }
    if (sfgaof & SFGAO_CANRENAME)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"CANRENAME ");
    }
    if (sfgaof & SFGAO_CANDELETE)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"CANDELETE ");
    }
    if (sfgaof & SFGAO_HASPROPSHEET)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"HASPROPSHEET ");
    }
    if (sfgaof & SFGAO_DROPTARGET)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"DROPTARGET ");
    }
    if (sfgaof & SFGAO_ENCRYPTED)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"ENCRYPTED ");
    }
    if (sfgaof & SFGAO_ISSLOW)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"ISSLOW ");
    }
    if (sfgaof & SFGAO_BROWSABLE)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"BROWSABLE ");
    }
    if (sfgaof & SFGAO_READONLY)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"READONLY ");
    }
    if (sfgaof & SFGAO_REMOVABLE)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"REMOVABLE ");
    }
    if (sfgaof & SFGAO_SHARE)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"SHARE ");
    }
    if (sfgaof & SFGAO_GHOSTED)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"GHOSTED ");
    }
    if (sfgaof & SFGAO_STORAGEANCESTOR)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"STORAGEANCESTOR ");
    }
    if (sfgaof & SFGAO_COMPRESSED)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"COMPRESSED ");
    }
    if (sfgaof & SFGAO_NEWCONTENT)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"NEWCONTENT ");
    }
    if (sfgaof & SFGAO_HIDDEN)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"HIDDEN ");
    }
    if (sfgaof & SFGAO_NONENUMERATED)
    {
        StringCchCat(sz, ARRAYSIZE(sz), L"NONENUMERATED ");
    }
    return SHStrDup(sz, ppsz);
}

void CNameSpaceTreeHost::_InspectItem(IShellItem2 *psi)
{
    HRESULT hr = SetItemImageImageInStaticControl(GetDlgItem(_hdlg, IDC_IMAGE), psi);

    PWSTR psz;
    hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &psz);
    SetDlgItemText(_hdlg, IDC_NAME, SUCCEEDED(hr) ? psz : L"");
    CoTaskMemFree(psz);

    hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &psz);
    SetDlgItemText(_hdlg, IDC_PATH, SUCCEEDED(hr) ? psz : L"");
    CoTaskMemFree(psz);

    PWSTR pszAttributes = NULL;
    SFGAOF sfgaof;
    hr = psi->GetAttributes(sfgaofAll, &sfgaof);
    if (SUCCEEDED(hr))
    {
        hr = ShellAttributesToString(sfgaof, &pszAttributes);
    }

    SetDlgItemText(_hdlg, IDC_ATTRIBUTES, SUCCEEDED(hr) ? pszAttributes : L"");
    CoTaskMemFree(pszAttributes);

    // this shows how to get to the QueryAssociations handler for an item
    IQueryAssociations *pqa;
    hr = psi->BindToHandler(NULL, BHID_SFUIObject, IID_PPV_ARGS(&pqa));
    if (SUCCEEDED(hr))
    {
        WCHAR szDefaultIcon[MAX_PATH];
        DWORD dwSize = ARRAYSIZE(szDefaultIcon);
        hr = pqa->GetString(0, ASSOCSTR_DEFAULTICON, NULL, szDefaultIcon, &dwSize);

        // SetDlgItemText(_hdlg, IDC_PATH, SUCCEEDED(hr) ? szDefaultIcon : L"");

        pqa->Release();
    }
}

BOOL CNameSpaceTreeHost::_DlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        _OnInitDlg();
        break;
    
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
        case IDOK:
        case IDCANCEL:
        case IDC_CANCEL:        
            return EndDialog(_hdlg, TRUE);

        case IDC_EXPLORE:
            _OnExplore();
            break;
        }
        break;
    
    case WM_DESTROY:
        _OnDestroyDlg();
        break;
        
    default:
        return FALSE;
    }
    return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, __in LPSTR pszCmdLine, int nCmdShow)
{
    g_hinst = hInstance;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        OleInitialize(0);   // for drag and drop

        CNameSpaceTreeHost *pdlg = new CNameSpaceTreeHost();
        if (pdlg)
        {
            pdlg->DoModal(NULL);
            pdlg->Release();
        }

        OleUninitialize();
        CoUninitialize();
    }

    return 0;
}
