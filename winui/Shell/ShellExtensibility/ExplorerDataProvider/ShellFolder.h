/**************************************************************************
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

(c) Microsoft Corporation. All Rights Reserved.
**************************************************************************/

#pragma once

#define MAX_OBJS    10
#define MYOBJID 0x1234

#pragma pack(1)
typedef struct tagObject
{
    USHORT  cb;
    WORD    MyObjID;
    BYTE    nLevel;
    BYTE    nSize;
    BYTE    nSides;
    BYTE    cchName;
    WCHAR   szName[1];
} FVITEMID;
#pragma pack()

typedef UNALIGNED FVITEMID *PFVITEMID;
typedef const UNALIGNED FVITEMID *PCFVITEMID;

class CFolderViewImplFolder : public IShellFolder2, public IPersistFolder2
{
public:
    CFolderViewImplFolder(UINT nLevel);

    // IUnknown methods
    IFACEMETHODIMP QueryInterface(__in REFIID riid, __deref_out void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellFolder
    IFACEMETHODIMP ParseDisplayName(HWND hwnd, 
                                    __in_opt IBindCtx *pbc, 
                                    __in PWSTR pszName, 
                                    __reserved ULONG *pchEaten, 
                                    __out PIDLIST_RELATIVE *ppidl, 
                                    __inout_opt ULONG *pdwAttributes);
    IFACEMETHODIMP EnumObjects(HWND hwnd, DWORD grfFlags, __out IEnumIDList **ppenumIDList);
    IFACEMETHODIMP BindToObject(PCUIDLIST_RELATIVE pidl, __in_opt IBindCtx *pbc, REFIID riid, __out void **ppv);
    IFACEMETHODIMP BindToStorage(PCUIDLIST_RELATIVE pidl, __in_opt IBindCtx *pbc, REFIID riid, __out void **ppv);
    IFACEMETHODIMP CompareIDs(LPARAM lParam, PCUIDLIST_RELATIVE pidl1, PCUIDLIST_RELATIVE pidl2);
    IFACEMETHODIMP CreateViewObject(HWND hwndOwner, REFIID riid, __out void **ppv);
    IFACEMETHODIMP GetAttributesOf(UINT cidl, __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY apidl, __inout ULONG *rgfInOut);
    IFACEMETHODIMP GetUIObjectOf(HWND hwndOwner, 
                                 UINT cidl, 
                                 __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                 REFIID riid, 
                                 __reserved UINT* prgfInOut, 
                                 __out void **ppv);
    IFACEMETHODIMP GetDisplayNameOf(PCUITEMID_CHILD pidl, SHGDNF shgdnFlags, __out STRRET *pName);
    IFACEMETHODIMP SetNameOf(HWND hwnd, PCUITEMID_CHILD pidl, PCWSTR pszName, DWORD uFlags, __out_opt PITEMID_CHILD * ppidlOut);

    // IShellFolder2
    IFACEMETHODIMP GetDefaultSearchGUID(__out GUID *pGuid);
    IFACEMETHODIMP EnumSearches(__out IEnumExtraSearch **ppenum);
    IFACEMETHODIMP GetDefaultColumn(DWORD dwRes, __out ULONG *pSort, __out ULONG *pDisplay);
    IFACEMETHODIMP GetDefaultColumnState(UINT iColumn, __out SHCOLSTATEF *pbState);
    IFACEMETHODIMP GetDetailsEx(PCUITEMID_CHILD pidl, const PROPERTYKEY *pkey, __out VARIANT *pv);
    IFACEMETHODIMP GetDetailsOf(__in_opt PCUITEMID_CHILD pidl, UINT iColumn, __out SHELLDETAILS *pDetails);
    IFACEMETHODIMP MapColumnToSCID(UINT iColumn, __out PROPERTYKEY *pkey);

    // IPersist
    IFACEMETHODIMP GetClassID(__out CLSID *pClassID);

    // IPersistFolder
    IFACEMETHODIMP Initialize(PCIDLIST_ABSOLUTE pidl);

    // IPersistFolder2
    IFACEMETHODIMP GetCurFolder(__out PIDLIST_ABSOLUTE *ppidl);

    // IDList constructor public for the enumerator object
    HRESULT CreateChildID(int nLevel, int nCurrent, __out PITEMID_CHILD* pidl);

private:
    ~CFolderViewImplFolder();

    HRESULT _GetName(PCUIDLIST_RELATIVE pidl, PWSTR pszName, int cchMax);
    HRESULT _GetName(PCUIDLIST_RELATIVE pidl, PWSTR *pszName);
    HRESULT _GetSides(PCUIDLIST_RELATIVE pidl, int* pSides);
    HRESULT _GetLevel(PCUIDLIST_RELATIVE pidl, int* pLevel);
    HRESULT _GetSize(PCUIDLIST_RELATIVE pidl, int* pSize);
    HRESULT _ValidatePidl(PCUIDLIST_RELATIVE pidl);
    PCFVITEMID _IsValid(PCUIDLIST_RELATIVE pidl);

    HRESULT _GetColumnDisplayName(LPCITEMIDLIST pidl, const PROPERTYKEY* pkey, VARIANT* pv, WCHAR* pszRet, UINT cch);

    long                m_cRef;
    int                 m_nLevel;
    PIDLIST_ABSOLUTE    m_pidl;             // where this folder is in the name space
    PWSTR               m_rgNames[MAX_OBJS];
    WCHAR               m_szModuleName[MAX_PATH];
};
