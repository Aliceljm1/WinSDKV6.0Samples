/**************************************************************************
    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
 
   (c) Microsoft Corporation. All Rights Reserved.
**************************************************************************/

#include <windows.h>
#include <shlobj.h>
#include <propkey.h>
#include <shlwapi.h>
#include <strsafe.h>

#include "FolderViewImpl.h"
#include "resource.h"
#include "Utils.h"
#include "ShellFolder.h"
#include "EnumIDList.h"
#include "Category.h"
#include "Guid.h"

const int g_nMaxLevel = 5;

CFolderViewImplFolder::CFolderViewImplFolder(UINT nLevel) : m_cRef(1), m_nLevel(nLevel), m_pidl(NULL)
{
    DllAddRef();
    ZeroMemory(m_rgNames, sizeof(m_rgNames));
}

CFolderViewImplFolder::~CFolderViewImplFolder()
{
    CoTaskMemFree(m_pidl);

    for (int i = 0; i < ARRAYSIZE(m_rgNames); i++)
    {
        CoTaskMemFree(m_rgNames[i]);
    }

    DllRelease();
}
 
HRESULT CFolderViewImplFolder::QueryInterface(__in REFIID riid, __deref_out void **ppv)
{
     static const QITAB qit[] = 
     {
        QITABENT(CFolderViewImplFolder, IShellFolder2),                        
        QITABENTMULTI(CFolderViewImplFolder, IShellFolder, IShellFolder2),     
        QITABENT(CFolderViewImplFolder, IPersist),
        QITABENTMULTI(CFolderViewImplFolder, IPersistFolder, IPersist),
        QITABENTMULTI(CFolderViewImplFolder, IPersistFolder2, IPersistFolder),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplFolder::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplFolder::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}

////////////////////////////////////////////////////////////////
//
//  Translates a display name into an item identifier list.
//
////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::ParseDisplayName(HWND hwnd, 
                                                __in_opt IBindCtx *pbc, 
                                                __in PWSTR pszName, 
                                                __reserved ULONG *pchEaten, 
                                                __out PIDLIST_RELATIVE *ppidl, 
                                                __inout_opt ULONG *pdwAttributes)
{
    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////
//
//  Allows a client to determine the contents of a folder by 
//  creating an item identifier enumeration object and returning 
//  its IEnumIDList interface. The methods supported by that 
//  interface can then be used to enumerate the folder's contents. 
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::EnumObjects(HWND hwnd, DWORD grfFlags, __out IEnumIDList **ppenumIDList)
{
    HRESULT hr;
    
    if (m_nLevel >= g_nMaxLevel)
    {
        *ppenumIDList = NULL;
        hr = S_FALSE;
    }
    else
    {
        *ppenumIDList = new CFolderViewImplEnumIDList(m_nLevel + 1, this);   
        hr = *ppenumIDList ? S_OK : E_OUTOFMEMORY;   
    }
    
    return hr;
}


/////////////////////////////////////////////////////////////////////////
//
//  Retrieves the IShellFolder interface for the specified subfolder.
//
/////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::BindToObject(__in PCUIDLIST_RELATIVE pidl, 
                                            __in_opt IBindCtx *pbc, 
                                            __in REFIID riid, 
                                            __deref_out void **ppv)
{
    *ppv = NULL;
    HRESULT hr = _ValidatePidl(pidl);
    if (SUCCEEDED(hr))
    {    
        //  We have no junctions so we can always just assume that all children
        //  are ours if we had any other IShellFolder implementations we would
        //  have to bind and recurse for level of pidl
        int nLevel;
        _GetLevel(ILFindLastID(pidl), &nLevel); // If this fails, nLevel == 0;

        CFolderViewImplFolder* pCFolderViewImplFolder = new CFolderViewImplFolder(nLevel);
        
        hr = pCFolderViewImplFolder ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            // Initialize it.
            PIDLIST_ABSOLUTE pidlBind = ILCombine(m_pidl, pidl);  
            hr = pidlBind ? S_OK : E_OUTOFMEMORY;
            if (SUCCEEDED(hr))
            {
                hr = pCFolderViewImplFolder->Initialize(pidlBind);
                if (SUCCEEDED(hr))
                {
                    hr = pCFolderViewImplFolder->QueryInterface(riid, ppv);
                }
                
                CoTaskMemFree(pidlBind);
            }
            pCFolderViewImplFolder->Release();
        }
    }
    
    return hr;
}

HRESULT CFolderViewImplFolder::BindToStorage(__in PCUIDLIST_RELATIVE pidl, 
                                             __in_opt IBindCtx *pbc, 
                                             __in REFIID riid, 
                                             __deref_out void **ppv)
{
    return BindToObject(pidl, pbc, riid, ppv);
}


/////////////////////////////////////////////////////////////
//
//  Helper function to help compare relative IDs.
//
/////////////////////////////////////////////////////////////
HRESULT _ILCompareRelIDs(IShellFolder *psfParent, 
                         PCUIDLIST_RELATIVE pidl1, 
                         PCUIDLIST_RELATIVE pidl2, 
                         LPARAM lParam)
{
    HRESULT hr;
    PCUIDLIST_RELATIVE pidlRel1 = ILNext(pidl1);
    PCUIDLIST_RELATIVE pidlRel2 = ILNext(pidl2);
    
    if (ILIsEmpty(pidlRel1))
    {
        if (ILIsEmpty(pidlRel2))
            hr = ResultFromShort(0);  // Both empty
        else
            hr = ResultFromShort(-1);   // 1 is empty, 2 is not.
    }
    else
    {
        if (ILIsEmpty(pidlRel2))
        {
            hr = ResultFromShort(1);  // 2 is empty, 1 is not
        }
        else
        {
            // pidlRel1 and pidlRel2 point to something, so:
            //  (1) Bind to the next level of the IShellFolder
            //  (2) Call its CompareIDs to let it compare the rest of IDs.
            LPITEMIDLIST pidlNext = ILCloneFirst(pidl1);    // pidl2 would work as well

            hr = pidlNext ? S_OK : E_OUTOFMEMORY;
            if (pidlNext)
            {
                IShellFolder *psfNext;
            
                hr = psfParent->BindToObject(pidlNext, NULL, IID_PPV_ARGS(&psfNext));
                if (SUCCEEDED(hr))
                {
                    // We do not want to pass the lParam is IShellFolder2 isn't supported.
                    // Although it isn't important for this example it shoud be considered
                    // if you are implementing this for other situations.
                    IShellFolder2 *psf2;
                    
                    if (SUCCEEDED(psfNext->QueryInterface(IID_PPV_ARGS(&psf2))))
                    {
                        psf2->Release();  // We can use the lParam
                    }
                    else
                    {
                        lParam = 0;       // We can't use the lParam
                    }

                    // Also, the column mask will not be relevant and should never be passed.
                    hr = psfNext->CompareIDs((lParam & ~SHCIDS_COLUMNMASK), pidlRel1, pidlRel2);
                    psfNext->Release();
                }
                
                CoTaskMemFree(pidlNext);
            }
        }
    }
    
    return hr;
}

/////////////////////////////////////////////////////
//
//  Called to determine the sorting of two pidls. 
//
/////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::CompareIDs(LPARAM lParam, 
                                          PCUIDLIST_RELATIVE pidl1, 
                                          PCUIDLIST_RELATIVE pidl2)
{
    // Here, we are just comparing the strings in the last PIDLs.
    HRESULT hr = S_OK;
    
    switch (LOWORD(lParam))
    {
    case 0: // Column one, Name.
        {
            // Load the strings that represent the names
            if (!m_rgNames[0])
            {
                hr = LoadFolderViewImplDisplayStrings(m_rgNames, ARRAYSIZE(m_rgNames));
            }

            if (SUCCEEDED(hr))
            {
                PWSTR psz1;
                hr = _GetName(pidl1, &psz1);
                if (SUCCEEDED(hr))
                {
                    PWSTR psz2;
                    hr = _GetName(pidl2, &psz2);
                    if (SUCCEEDED(hr))
                    {
                        // Find their place in the array.
                        int nPidlOne = 0, nPidlTwo = 0;
                        for (int i = 0; i < ARRAYSIZE(m_rgNames); i++)
                        {
                            if (!StrCmpW(psz1, m_rgNames[i]))
                            {
                                nPidlOne = i;
                            }

                            if (!StrCmpW(psz2, m_rgNames[i]))
                            {
                                nPidlTwo = i;
                            }
                        }

                        if (nPidlOne == nPidlTwo)
                        {
                            hr = _ILCompareRelIDs(this, pidl1, pidl2, lParam);
                        }
                        else
                        {
                            hr = ResultFromShort(nPidlOne - nPidlTwo);
                        }
                        
                        CoTaskMemFree(psz2);
                    }
                    
                    CoTaskMemFree(psz1);
                }
            }
            break;
        }
        case 1: // Column two, Size.
        {
            int nSize1 = 0, nSize2 = 0;
            hr = _GetSize(pidl1, &nSize1);
            if (SUCCEEDED(hr))
            {
                hr = _GetSize(pidl2, &nSize2);
                if (SUCCEEDED(hr))
                {
                    hr = ResultFromShort(nSize1 - nSize2);
                }
            }
            break;
        }
        case 2: // Column Three, Sides.
        {
            int nSides1 = 0, nSides2 = 0;
            hr = _GetSides(pidl1, &nSides1);
            if (SUCCEEDED(hr))
            {
                hr = _GetSides(pidl2, &nSides2);
                if (SUCCEEDED(hr))
                {
                    hr = ResultFromShort(nSides1 - nSides2);
                }
            }
            break;
        }
        case 3: // Column four, Level.
        {
            int cLevel1 = 0, cLevel2 = 0;
            hr = _GetLevel(pidl1, &cLevel1);
            if (SUCCEEDED(hr))
            {
                hr = _GetLevel(pidl2, &cLevel2);
                if (SUCCEEDED(hr))
                {
                    hr = ResultFromShort(cLevel1 - cLevel2);
                }
            }
            break;
        }
        default:
        {
            hr = ResultFromShort(1);
        }
    }
    
    return hr;
}

////////////////////////////////////////////////////////////////////
//
//  Called by the Shell to create the View Object and return it.
//
////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::CreateViewObject(__in_opt HWND hwndOwner, __in REFIID riid, __deref_out void **ppv)
{
    *ppv = NULL;

    HRESULT  hr = E_NOINTERFACE;
    if (riid == IID_IShellView)
    {             
        SFV_CREATE csfv = { sizeof(csfv), 0 };

        hr = QueryInterface(IID_PPV_ARGS(&csfv.pshf));
        if (SUCCEEDED(hr))
        {
            // No callbacks are required to have a fully functional System Folder View Object!
            csfv.psfvcb = NULL;  
            hr = SHCreateShellFolderView(&csfv, (IShellView**)ppv);
            csfv.pshf->Release();
        }   
    }
    else if (riid == IID_ICategoryProvider)
    {
        CFolderViewImplCategoryProvider* pCatProvider = new CFolderViewImplCategoryProvider(this);
        hr = pCatProvider ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        { 
            hr = pCatProvider->QueryInterface(riid, ppv);
            pCatProvider->Release();
        } 
    }
    else if (riid == IID_IContextMenu)
    {
        DEFCONTEXTMENU dcm = { hwndOwner, NULL, m_pidl, static_cast<IShellFolder2 *>(this), 0, NULL, NULL, 0, NULL };
        hr = SHCreateDefaultContextMenu(&dcm, riid, ppv);
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////////
//
//  Retrieves the attributes of one or more file objects or subfolders.
//
///////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetAttributesOf(UINT cidl, __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY apidl, __inout ULONG *rgfInOut)
{
    HRESULT hr = E_INVALIDARG;
    if (1 == cidl)
    {
        int nLevel = 0;
        hr = _GetLevel(apidl[0], &nLevel);
        if (SUCCEEDED(hr))
        {
            if (nLevel < g_nMaxLevel)
            {
                *rgfInOut &= SFGAO_FOLDER | SFGAO_HASSUBFOLDER;
            }
            else
            {
                *rgfInOut &= SFGAO_FOLDER;
            }
        }
    }
    
    return hr;
}

////////////////////////////////////////////////////////////////
//
//  Retrieves an OLE interface that can be used to carry out 
//  actions on the specified file objects or folders.
//
////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetUIObjectOf(HWND hwnd, 
                                             UINT cidl, 
                                             __in_ecount_opt(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                             REFIID riid, 
                                             __reserved UINT *prgfInOut, 
                                             __out void **ppv)
{
    *ppv = NULL;
    HRESULT hr;

    if (riid == IID_IContextMenu)
    {
        DEFCONTEXTMENU dcm = { hwnd, NULL, m_pidl, static_cast<IShellFolder2 *>(this), 
                               cidl, apidl, NULL, 0, NULL };
        hr = SHCreateDefaultContextMenu(&dcm, riid, ppv);
    }
    else if (riid == IID_IExtractIconW)
    {
        IDefaultExtractIconInit *pdxi;
        hr = SHCreateDefaultExtractIcon(IID_PPV_ARGS(&pdxi));
        if (SUCCEEDED(hr))
        {
            hr = pdxi->SetNormalIcon(L"shell32.dll", 4);
            if (SUCCEEDED(hr))
            {
                hr = pdxi->QueryInterface(riid, ppv);
            }
            
            pdxi->Release();
        }
    }
    else if (riid == IID_IDataObject)
    {
        hr = SHCreateDataObject(m_pidl, cidl, apidl, NULL, riid, ppv);
    }
    else
    {
        hr = E_NOINTERFACE;
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////////
//
//  Retrieves the display name for the specified file object or subfolder. 
//
//////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetDisplayNameOf(__in PCUITEMID_CHILD pidl, __in SHGDNF shgdnFlags, __out STRRET *pName)
{
    HRESULT hr = S_OK;
    if (shgdnFlags & SHGDN_FORPARSING)
    {
        WCHAR szDisplayName[MAX_PATH];
        
        if (shgdnFlags & SHGDN_INFOLDER)
        {
            hr = _GetName(pidl, szDisplayName, ARRAYSIZE(szDisplayName));
        }
        else
        {
            PWSTR pszThisFolder;
            hr = SHGetNameFromIDList(m_pidl, (shgdnFlags & SHGDN_FORADDRESSBAR) ? SIGDN_DESKTOPABSOLUTEEDITING : SIGDN_DESKTOPABSOLUTEPARSING, &pszThisFolder);
            if (SUCCEEDED(hr))
            {
                StringCchCopyW(szDisplayName, ARRAYSIZE(szDisplayName), pszThisFolder);
                StringCchCatW(szDisplayName, ARRAYSIZE(szDisplayName), L"\\");

                WCHAR szName[MAX_PATH];
                hr = _GetName(pidl, szName, ARRAYSIZE(szName));
                if (SUCCEEDED(hr))
                {
                    StringCchCatW(szDisplayName, ARRAYSIZE(szDisplayName), szName);
                }

                CoTaskMemFree(pszThisFolder);
            }
        }
        if (SUCCEEDED(hr))
        {
            hr = StringToStrRetW(szDisplayName, pName);
        }
    }
    else
    {
        PWSTR pszName;
        hr = _GetName(pidl, &pszName);
        if (SUCCEEDED(hr))
        {
            hr = StringToStrRetW(pszName, pName);
            CoTaskMemFree(pszName);
        }
    }
    
    return hr;
}

//////////////////////////////////////////////////////////////////////
//
//  Sets the display name of a file object or subfolder, changing
//  the item identifier in the process.
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::SetNameOf(HWND hwnd, 
                                         PCUITEMID_CHILD pidl, 
                                         PCWSTR pszName, 
                                         DWORD uFlags, 
                                         __out_opt PITEMID_CHILD * ppidlOut)
{
    HRESULT hr = E_NOTIMPL;
    *ppidlOut = NULL;
    return hr;
}
 
////////////////////////
//
//  IPersist method
//
////////////////////////
HRESULT CFolderViewImplFolder::GetClassID(__out CLSID *pClassID)
{
    *pClassID = CLSID_FolderViewImpl;
    return S_OK;
}

//////////////////////////////
//
//  IPersistFolder method
//
//////////////////////////////
HRESULT CFolderViewImplFolder::Initialize(PCIDLIST_ABSOLUTE pidl)
{
    HRESULT hr = pidl ? S_OK : E_INVALIDARG;
    
    if (SUCCEEDED(hr))
    {
        m_pidl = ILClone(pidl);
        hr = m_pidl ? S_OK : E_FAIL;
    }
  
    return hr;
}

//////////////////////////////
//
//  IShellFolder2 methods
//
//////////////////////////////
HRESULT CFolderViewImplFolder::EnumSearches(__out IEnumExtraSearch **ppEnum)
{
    *ppEnum = NULL;
    return E_NOINTERFACE;
}

////////////////////////////////////////////////////////////
//
//  Retrieves the default sorting and display columns.
//
////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetDefaultColumn (DWORD dwRes, 
                                                 __out ULONG *pSort, 
                                                 __out ULONG *pDisplay)
{
    *pSort = 0;
    *pDisplay = 0;
    return S_OK;
}

////////////////////////////////////////////////////////////
//
//  Retrieves the default state for a specified column.
//
////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetDefaultColumnState (UINT iColumn, __out SHCOLSTATEF *pcsFlags)
{
    HRESULT hr = (iColumn < 3) ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        *pcsFlags = SHCOLSTATE_ONBYDEFAULT | SHCOLSTATE_TYPE_STR;
    }

    return hr;
}

/////////////////////////////////////////////////////////////////////
//
//  Requests the GUID of the default search object for the folder.
//
/////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetDefaultSearchGUID(__out GUID *pguid)
{
    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////
//
//  Helper function for getting the display name for a column.
//  IMPORTANT: If cch is set to 0 the value is returned in the VARIANT.
//
//////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::_GetColumnDisplayName(PCUITEMID_CHILD pidl, 
                                                     const PROPERTYKEY* pkey,
                                                     VARIANT* pv, 
                                                     PWSTR pszRet, 
                                                     UINT cch)
{
    HRESULT hr = S_OK;
    if (IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_AreaSize))
    {
        int nSize = 0;
        hr = _GetSize(pidl, &nSize);
        if (SUCCEEDED(hr))
        {
            //This property is declared as "String" type.  See:  FolderViewImpl.propdesc
            WCHAR szFormattedSize[12];
            hr = StringCchPrintfW(szFormattedSize, ARRAYSIZE(szFormattedSize), L"%d Sq. Ft.", nSize);
            if (cch)
            {
                hr = StringCchCopyW(pszRet, cch, szFormattedSize);
            }
            else
            {
                pv->vt = VT_BSTR;
                pv->bstrVal = SysAllocString(szFormattedSize);
                hr = pv->bstrVal ? S_OK : E_OUTOFMEMORY;   
            }   
        }
    }
    else if (IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_NumberOfSides))
    {
        int nSides = 0;
        hr = _GetSides(pidl, &nSides);
        if (SUCCEEDED(hr))
        {
            if (cch)
            {
                hr = StringCchPrintfW(pszRet, cch, L"%d", nSides);
            }
            else
            {
                pv->vt = VT_I4;
                pv->lVal = nSides;
            }   
        }
    }
    else if (IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_DirectoryLevel))
    {
        int nLevel = 0;
        hr = _GetLevel(pidl, &nLevel);
        if (SUCCEEDED(hr))
        {
            if (cch)
            {
                hr = StringCchPrintfW(pszRet, cch, L"%d", nLevel);
            }
            else
            {
                pv->vt = VT_I4;
                pv->lVal = nLevel;
            }   
        }
    }

    return hr;
}

////////////////////////////////////////////////////////
//
//  Retrieves detailed information, identified by a 
//  property set ID (FMTID) and property ID (PID), 
//  on an item in a Shell folder.
//
////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetDetailsEx(PCUITEMID_CHILD pidl, 
                                            const PROPERTYKEY *pkey, 
                                            __out VARIANT *pv)
{
    HRESULT hr = S_OK;
    if (IsEqualPropertyKey(*pkey, PKEY_PropList_PreviewDetails))
    {
        pv->vt = VT_BSTR;
        pv->bstrVal = SysAllocString(L"prop:Microsoft.SDKSample.AreaSize;Microsoft.SDKSample.NumberOfSides;Microsoft.SDKSample.DirectoryLevel");
        hr = pv->bstrVal ? S_OK : E_OUTOFMEMORY;
    }
    else
    {
        hr = _GetColumnDisplayName(pidl, pkey, pv, NULL, 0);
    }
    
    return hr;
}

///////////////////////////////////////////////////////
//
//  Retrieves detailed information, identified by a 
//  column index, on an item in a Shell folder.
//
///////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetDetailsOf(__in_opt PCUITEMID_CHILD pidl, 
                                            UINT iColumn, 
                                            __out SHELLDETAILS *pDetails)
{
    PROPERTYKEY key;
    HRESULT hr = MapColumnToSCID(iColumn, &key);
    pDetails->cxChar = 24;
    WCHAR szRet[MAX_PATH];
    
    if (!pidl)
    { 
        switch (iColumn)
        {
        case 0:
            pDetails->fmt = LVCFMT_LEFT;
            hr = StringCchCopyW(szRet, ARRAYSIZE(szRet), L"Name");
            break;
        case 1:
            pDetails->fmt = LVCFMT_CENTER;
            hr = StringCchCopyW(szRet, ARRAYSIZE(szRet), L"Size");
            break;
        case 2:
            pDetails->fmt = LVCFMT_CENTER;
            hr = StringCchCopyW(szRet, ARRAYSIZE(szRet), L"Sides");
            break;
        case 3:
            pDetails->fmt = LVCFMT_CENTER;
            hr = StringCchCopyW(szRet, ARRAYSIZE(szRet), L"Level");
            break;
        default:
            hr = E_FAIL;
            break;
        }
    }
    else
    {
        if (SUCCEEDED(hr))
        {
            hr = _GetColumnDisplayName(pidl, &key, NULL, szRet, ARRAYSIZE(szRet));
        }
    }
                
    if (SUCCEEDED(hr))
    {
        hr = StringToStrRetW(szRet, &pDetails->str);
    }
        
    return hr;
}

//////////////////////////////////////////////////////
//
//  Converts a column name to the appropriate  
//  property set ID (FMTID) and property ID (PID).
//
//////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::MapColumnToSCID(UINT iColumn, __out PROPERTYKEY *pkey)
{
    // The property keys returned here are used by the categorizer.
    HRESULT hr = S_OK;
    switch (iColumn)
    {
    case 0:
        *pkey = PKEY_ItemNameDisplay;
        break;
    case 1:
        *pkey = PKEY_Microsoft_SDKSample_AreaSize;
        pkey->pid = 3;
        break;
    case 2:
        *pkey = PKEY_Microsoft_SDKSample_NumberOfSides;
        break;
     case 3:
        *pkey = PKEY_Microsoft_SDKSample_DirectoryLevel;
        break;
    default:
        hr = E_FAIL;
    }
    
    return hr;
}

//IPersistFolder2 methods
////////////////////////////////////////////////////////////
//
//  Retrieves the PIDLIST_ABSOLUTE for the folder object. 
//
////////////////////////////////////////////////////////////
HRESULT CFolderViewImplFolder::GetCurFolder(__out PIDLIST_ABSOLUTE *ppidl)
{
    *ppidl = NULL;
    
    HRESULT hr = m_pidl ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        *ppidl = ILClone(m_pidl);
        hr = *ppidl ? S_OK : E_OUTOFMEMORY;
    }
    
    return hr; 
}

PCFVITEMID CFolderViewImplFolder::_IsValid(PCUIDLIST_RELATIVE pidl)
{
    PCFVITEMID pidmine = NULL;
    if (pidl)
    {
        pidmine = (PCFVITEMID)pidl;
        if (!(pidmine->cb && MYOBJID == pidmine->MyObjID && pidmine->nLevel <= g_nMaxLevel))
        {
            pidmine = NULL;
        }
    }

    return pidmine;
}

HRESULT CFolderViewImplFolder::_GetName(PCUIDLIST_RELATIVE pidl, PWSTR pszName, int cchMax)
{
    PCFVITEMID pMyObj = _IsValid(pidl);

    HRESULT hr = pMyObj ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        int i = 0;
        for ( ; i < cchMax; i++)
        {
            pszName[i] = pMyObj->szName[i];
            if (0 == pszName[i])
            {
                break;
            }
        }

        // Make sure the string is null-terminated.
        if (i == cchMax)
        {
            pszName[cchMax - 1] = 0;
        }
    }

    return hr;
}

HRESULT CFolderViewImplFolder::_GetName(PCUIDLIST_RELATIVE pidl, PWSTR *ppsz)
{
    *ppsz = 0;
    PCFVITEMID pMyObj = _IsValid(pidl);
    
    HRESULT hr = pMyObj ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        int cch = pMyObj->cchName;
        *ppsz = (PWSTR)CoTaskMemAlloc(cch * sizeof(**ppsz));
    
        hr = *ppsz ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = _GetName(pidl, *ppsz, cch);
        }
    }
    
    return hr;
}


HRESULT CFolderViewImplFolder::_GetSides(PCUIDLIST_RELATIVE pidl, int* pSides)
{
    PCFVITEMID pMyObj = _IsValid(pidl);

    HRESULT hr = pMyObj ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        *pSides = pMyObj->nSides;
    }

    return hr;
}

HRESULT CFolderViewImplFolder::_GetLevel(PCUIDLIST_RELATIVE pidl, int* pLevel)
{
    PCFVITEMID pMyObj = _IsValid(pidl);

    HRESULT hr = pMyObj ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        *pLevel = pMyObj->nLevel;
    }
    else  // If this fails we are at level zero.
    {
        *pLevel = 0;
    }
    
    return hr;
}

HRESULT CFolderViewImplFolder::_GetSize(PCUIDLIST_RELATIVE pidl, int* pSize)
{
    PCFVITEMID pMyObj = _IsValid(pidl);
    
    HRESULT hr = pMyObj ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        *pSize = pMyObj->nSize;
    }
    
    return hr;
}

HRESULT CFolderViewImplFolder::_ValidatePidl(PCUIDLIST_RELATIVE pidl)
{
    PCFVITEMID pMyObj = _IsValid(pidl);
    return pMyObj ? S_OK : E_INVALIDARG;
}

HRESULT CFolderViewImplFolder::CreateChildID(int nLevel, int nCurrent, PITEMID_CHILD *pidl)
{    
    if (!m_rgNames[0])
    {
        LoadFolderViewImplDisplayStrings(m_rgNames, ARRAYSIZE(m_rgNames));
    }

    HRESULT hr = nCurrent < ARRAYSIZE(m_rgNames) ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr))
    {
        // Sizeof an object plus the next cb plus the characters in the string.
        UINT nSize = sizeof(FVITEMID) + 
                     sizeof(USHORT) + 
                     (lstrlenW(m_rgNames[nCurrent]) * sizeof(WCHAR)) + 
                     sizeof(WCHAR);

        // Allocate and zero the memory.
        FVITEMID *lpMyObj = (FVITEMID *)CoTaskMemAlloc(nSize);
        
        hr = lpMyObj ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            ZeroMemory(lpMyObj, nSize);
            lpMyObj->cb = static_cast<short>(nSize - sizeof(lpMyObj->cb));
            lpMyObj->MyObjID = MYOBJID;
            lpMyObj->cchName    = (BYTE)(lstrlenW(m_rgNames[nCurrent]) + 1);
            lpMyObj->nLevel     = (BYTE)nLevel;
            lpMyObj->nSize      = (BYTE)(rand() % MAXBYTE);
            lpMyObj->nSides     = (BYTE)(rand() % 6);
                                
            while (1 == lpMyObj->nSides || 2 == lpMyObj->nSides)
            {
                lpMyObj->nSides = (BYTE)(rand() % 6);
            }
            
            hr = StringCchCopyW(lpMyObj->szName, lpMyObj->cchName,  m_rgNames[nCurrent]);
            if (SUCCEEDED(hr))
            {
                *pidl = (PITEMID_CHILD)lpMyObj;
            }
        }
    }
    
    return hr;
}
