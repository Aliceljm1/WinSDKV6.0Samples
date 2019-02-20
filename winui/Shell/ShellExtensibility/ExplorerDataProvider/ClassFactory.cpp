/**************************************************************************
    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
 
   (c) Microsoft Corporation. All Rights Reserved.
**************************************************************************/
#include <shlobj.h>
#include <shlwapi.h>
#include "Utils.h"
#include "FolderViewImpl.h"
#include "ClassFactory.h"
#include "ShellFolder.h"
#include "EnumIDList.h"

CFolderViewImplClassFactory:: CFolderViewImplClassFactory() : m_cRef(1)
{
    DllAddRef();
}

CFolderViewImplClassFactory::~CFolderViewImplClassFactory()
{
    DllRelease();
}

HRESULT CFolderViewImplClassFactory::QueryInterface(__in REFIID riid, __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplClassFactory, IClassFactory),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

DWORD CFolderViewImplClassFactory::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

DWORD CFolderViewImplClassFactory::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}

HRESULT CFolderViewImplClassFactory::CreateInstance(__in_opt IUnknown* punkOuter, 
                                                    __in REFIID riid, 
                                                    __deref_out void **ppv)
{
    *ppv = NULL;

    HRESULT hr = !punkOuter ? S_OK : CLASS_E_NOAGGREGATION;
    if (SUCCEEDED(hr))
    {
        CFolderViewImplFolder* pFolderViewImplShellFolder = new CFolderViewImplFolder(0);
        hr = pFolderViewImplShellFolder ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {          
            hr = pFolderViewImplShellFolder->QueryInterface(riid, ppv);
            pFolderViewImplShellFolder->Release();
        }
    }
    return hr;
}

HRESULT CFolderViewImplClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        DllAddRef();
    }
    else
    {
        DllRelease();
    }
    return S_OK;
}
