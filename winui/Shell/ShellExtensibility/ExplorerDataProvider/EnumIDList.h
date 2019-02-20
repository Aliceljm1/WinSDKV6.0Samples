/**************************************************************************
    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
 
   (c) Microsoft Corporation. All Rights Reserved.
**************************************************************************/

#pragma once

class CFolderViewImplEnumIDList : public IEnumIDList
{
public:
    CFolderViewImplEnumIDList(int nCurrent, CFolderViewImplFolder* pFolderViewImplShellFolder);

    // IUnknown methods
    IFACEMETHODIMP QueryInterface(__in REFIID riid, __deref_out void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IEnumIDList
    IFACEMETHODIMP Next(ULONG celt, 
                        __out_ecount_part(celt, *pceltFetched) PITEMID_CHILD *rgelt, 
                        __out_opt ULONG *pceltFetched);
    IFACEMETHODIMP Skip(DWORD celt);
    IFACEMETHODIMP Reset();
    IFACEMETHODIMP Clone(__deref_out IEnumIDList **ppenum);

private:
    ~CFolderViewImplEnumIDList();

    LONG m_cRef;
    int m_nItem;
    int m_nLevel;
    CFolderViewImplFolder* m_pFolder;
};
