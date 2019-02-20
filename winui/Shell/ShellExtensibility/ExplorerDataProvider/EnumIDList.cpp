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
#include "ShellFolder.h"
#include "EnumIDList.h"
#include <stdio.h>
#include <strsafe.h>

CFolderViewImplEnumIDList::CFolderViewImplEnumIDList(int nLevel, CFolderViewImplFolder* pFolderViewImplShellFolder) :
    m_cRef(1), m_nLevel(nLevel), m_nItem(0), m_pFolder(pFolderViewImplShellFolder)
{
    m_pFolder->AddRef();
}

CFolderViewImplEnumIDList::~CFolderViewImplEnumIDList()
{
    m_pFolder->Release();
}

HRESULT CFolderViewImplEnumIDList::QueryInterface(__in REFIID riid, __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplEnumIDList, IEnumIDList),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

DWORD CFolderViewImplEnumIDList::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

DWORD CFolderViewImplEnumIDList::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

// Retrieves the specified number of item identifiers in 
// the enumeration sequence and advances the current position 
// by the number of items retrieved. 
HRESULT CFolderViewImplEnumIDList::Next(ULONG celt, 
                                        __out_ecount_part(celt, *pceltFetched) PITEMID_CHILD *rgelt, 
                                        __out_opt ULONG *pceltFetched)
{
    int celtFetched = 0;
   
    HRESULT hr = (celt > 1 && !pceltFetched) ? E_INVALIDARG : S_OK;
    if (SUCCEEDED(hr))
    {
        for (ULONG i = 0; (i < celt) && SUCCEEDED(hr); i++)
        {
            hr = m_pFolder->CreateChildID(m_nLevel, m_nItem, &rgelt[i]);
            if (SUCCEEDED(hr))
            {
                celtFetched++;
            }
            m_nItem++;
        }
    }

    if (pceltFetched)
    {
        *pceltFetched = celtFetched;
    }

    return celtFetched ? S_OK : S_FALSE;
}

HRESULT CFolderViewImplEnumIDList::Skip(DWORD celt)
{
    m_nItem += celt;
    return S_OK;
}

HRESULT CFolderViewImplEnumIDList::Reset()
{
    m_nItem = 0;
    return S_OK;
}

HRESULT CFolderViewImplEnumIDList::Clone(__deref_out IEnumIDList **ppenum)
{
    *ppenum = new CFolderViewImplEnumIDList(m_nLevel, m_pFolder);

    HRESULT hr = *ppenum ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        hr = (*ppenum)->Skip(m_nItem);
    }

    return hr;
}
