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
#include <shlobj.h>    // IDropTarget
#include <shlwapi.h>   // QISearch
#include <strsafe.h>   // StringCchCopyW

class CDropTarget : public IDropTarget
{
public:
    CDropTarget(HWND hwnd) : 
      _cRef(1), _hwnd(hwnd), _pdthelper(NULL), _pdtobj(NULL), _cfDropDescription(0), _pszItem(NULL)
    {
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
    
    // IDropTarget
    IFACEMETHODIMP DragEnter(IDataObject *pdtobj, DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect);
    IFACEMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
    IFACEMETHODIMP DragLeave();
    IFACEMETHODIMP Drop(IDataObject *pdtobj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

    HRESULT CreateDropTargetHelper();

private:
    ~CDropTarget();
    HRESULT _SetBlob(IDataObject *pdtobj, CLIPFORMAT cf, const void *pvBlob, UINT cbBlob);
    void _SetDropDescription();
    
    long _cRef;
    HWND _hwnd;
    IDropTargetHelper *_pdthelper;
    IDataObject *_pdtobj;
    CLIPFORMAT _cfDropDescription;
    PWSTR _pszItem;
};

CDropTarget *g_pData = NULL;

BOOL InitInstance(HINSTANCE);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE g_hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = OleInitialize(NULL);    // needed for Drag and Drop
    if (SUCCEEDED(hr))
    {
        if (InitInstance(g_hInstance)) 
        {
            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0)) 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }  
        }
       
        OleUninitialize();
    }
    return 0;
}

BOOL InitInstance(HINSTANCE hInst)
{
    BOOL fRet = FALSE;
    PCWSTR pwszWndClass = L"MyWindowClass";
    WNDCLASSEXW wcex = {0};

    wcex.cbSize         = sizeof(WNDCLASSEXW); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance      = hInst;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = pwszWndClass;

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(pwszWndClass, 
                          L"Drag and Drop Example", 
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, 
                          0, 
                          CW_USEDEFAULT, 
                          0, 
                          NULL, 
                          NULL, 
                          hInst, 
                          NULL);
    
    if (hWnd)
    {
        g_pData = new CDropTarget(hWnd);
        if (g_pData)
        {
            ShowWindow(hWnd, SW_SHOWNORMAL);
            UpdateWindow(hWnd);
            
            IDropTarget *pdt;
            if (SUCCEEDED(g_pData->QueryInterface(IID_PPV_ARGS(&pdt))))
            {
                if (SUCCEEDED(RegisterDragDrop(hWnd, pdt)) && 
                    SUCCEEDED(g_pData->CreateDropTargetHelper()))
                {
                    // Getting here implies that we are a registered drop target
                    // and we have a valid IDropTargetHelper interface.
                    fRet = TRUE;
                }
                
                pdt->Release();
            }
        }

        if (!fRet)
        {
            PostQuitMessage(0);
        }
   }
    
   return fRet;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
    case WM_DESTROY:
        if (g_pData)
        {
            g_pData->Release();
            g_pData = NULL;
        }

        PostQuitMessage(0);
        break;
  
    default: 
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    
    return 0;
}

CDropTarget::~CDropTarget()
{
    if (_pdthelper)
    {
        _pdthelper->Release();
    }
}

HRESULT CDropTarget::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(CDropTarget, IDropTarget),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

ULONG CDropTarget::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

ULONG CDropTarget::Release()
{
    ULONG cRef = InterlockedDecrement(&_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    
    return cRef;
}

HRESULT CDropTarget::CreateDropTargetHelper()
{
    _cfDropDescription = (CLIPFORMAT)RegisterClipboardFormat(CFSTR_DROPDESCRIPTION);
    return CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&_pdthelper));
}

STDAPI SHCreateItemFromObject(__in IUnknown *punk, __in REFIID riid, __deref_out void **ppv)
{
    PIDLIST_ABSOLUTE pidl;
    HRESULT hr = SHGetIDListFromObject(punk, &pidl);
    if (SUCCEEDED(hr))
    {
        hr = SHCreateItemFromIDList(pidl, riid, ppv);
        ILFree(pidl);
    }
    return hr;
}

HRESULT CDropTarget::DragEnter(IDataObject *pdtobj, DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect)
{
    // You will need access to the IDataObject to
    // set the description from the DragOver function.
    // Be sure to release it both DragLeave and Drop.
    pdtobj->AddRef();
    _pdtobj = pdtobj;

    // To use the helpers you must create a POINT from the POINTL
    // and then call through to the corresponding helper function.
    POINT pt = {ptl.x, ptl.y};
    _pdthelper->DragEnter(_hwnd, pdtobj, &pt, *pdwEffect);
    
    // Set the drop effect that the program supports and then
    // set the description.  This must be done at the DragEnter,
    // and ever subsequent DragOver call.  Although we are supporting
    // drag and drop, for this example we will only return DROPEFFECT_NONE.
    IShellItem *psi;
    HRESULT hr = SHCreateItemFromObject(pdtobj, IID_PPV_ARGS(&psi));
    if (SUCCEEDED(hr))
    {
        CoTaskMemFree(_pszItem);
        psi->GetDisplayName(SIGDN_NORMALDISPLAY, &_pszItem);
        psi->Release();
    }

    *pdwEffect = DROPEFFECT_NONE;
    _SetDropDescription();
    return S_OK;
}
    
HRESULT CDropTarget::DragOver(DWORD grfKeyState, POINTL ptl,  DWORD *pdwEffect)
{
    // To use the helpers you must create a POINT from the POINTL
    // and then call through to the corresponding helper function.
    POINT pt = {ptl.x, ptl.y};
    _pdthelper->DragOver(&pt, *pdwEffect);
   
    // Set the drop effect that the program supports and then
    // set the description.  This must be done at the DragEnter,
    // and ever subsequent DragOver call.  Although we are supporting
    // drag and drop, for this example we will only return DROPEFFECT_NONE.
    *pdwEffect = DROPEFFECT_NONE;    
    _SetDropDescription();
    return S_OK;
}

HRESULT CDropTarget::DragLeave()
{
    // Here we just pass the DragLeave call to the helpers.
    _pdthelper->DragLeave();

    DROPDESCRIPTION dd = { DROPIMAGE_INVALID };
    _SetBlob(_pdtobj, _cfDropDescription, &dd, sizeof(dd));

    CoTaskMemFree(_pszItem);
    _pszItem = NULL;

    // We are done with the IDataObject so we release it.
    _pdtobj->Release();
    _pdtobj = NULL;
    return S_OK;
}

HRESULT CDropTarget::Drop(IDataObject *pdtobj, DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect)
{
    // First we inform the DragDropHelper that we are performing the drop.
    POINT pt = {ptl.x, ptl.y};
    _pdthelper->Drop(pdtobj, &pt, *pdwEffect);

    // *** do drop operaiton here ***
    // this sample does nothing on drop
  
    // We are done with the IDataObject so we release it.
    _pdtobj->Release();
    _pdtobj = NULL;

    *pdwEffect = DROPEFFECT_NONE;
    return S_OK;
}

// Individual drop targets know what will happen if an object is dropped
// on them.  The drop target must set the description of what will happen
// on a drop so the DragDropHelper can display the description.

void CDropTarget::_SetDropDescription()
{
    DROPDESCRIPTION dd;
    ZeroMemory(&dd, sizeof(dd));

    dd.type = DROPIMAGE_COPY;
    StringCchCopyW(dd.szMessage, ARRAYSIZE(dd.szMessage), L"Drop %1 on this app");
    StringCchCopyW(dd.szInsert, ARRAYSIZE(dd.szInsert), _pszItem ? _pszItem : L"<unknown>");

    _SetBlob(_pdtobj, _cfDropDescription, &dd, sizeof(dd));
}

HRESULT CDropTarget::_SetBlob(IDataObject *pdtobj, CLIPFORMAT cf, const void *pvBlob, UINT cbBlob)
{
    HRESULT hr = E_OUTOFMEMORY;
    void *pv = GlobalAlloc(GPTR, cbBlob);
    if (pv)
    {
        CopyMemory(pv, pvBlob, cbBlob);

        FORMATETC fmte = {cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
        
        // The STGMEDIUM structure is used to define how to handle a global memory transfer. 
        // This structure includes a flag, tymed, which indicates the medium 
        // to be used, and a union comprising pointers and a handle for getting whichever 
        // medium is specified in tymed.
        STGMEDIUM medium = {0};
        medium.tymed = TYMED_HGLOBAL;
        medium.hGlobal = pv;

        hr = pdtobj->SetData(&fmte, &medium, TRUE);
        if (FAILED(hr))
        {
            GlobalFree(pv);
        }
    }
    return hr;
}
