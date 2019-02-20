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
#include "Guid.h"
#include "Utils.h"
#include "ShellFolder.h"
#include "EnumIDList.h"
#include "Category.h"
#include "resource.h"
#include "FolderViewImpl.h"

#define MAX_CATEGORIES      1  // These are additional categories beyond the columns

// Name
// {77559890-8E11-48e2-9B72-585FBA9CFFF4}
DEFINE_GUID(CAT_GUID_NAME,0x77559890, 0x8e11, 0x48e2, 0x9b, 0x72, 0x58, 0x5f, 0xba, 0x9c, 0xff, 0xf4);

// Size
// {0210C647-A9B8-41fb-ACB7-3C57D27C5BC1}
DEFINE_GUID(CAT_GUID_SIZE, 0x210c647, 0xa9b8, 0x41fb, 0xac, 0xb7, 0x3c, 0x57, 0xd2, 0x7c, 0x5b, 0xc1);

// Type
// {D1873FB3-76BA-474d-BE69-ED52476DD7E3}
DEFINE_GUID(CAT_GUID_SIDES,0xd1873fb3, 0x76ba, 0x474d, 0xbe, 0x69, 0xed, 0x52, 0x47, 0x6d, 0xd7, 0xe3);

// Level
// {F5A38A1F-3093-4dd9-9E6C-8762E4224F10}
DEFINE_GUID(CAT_GUID_LEVEL, 0xf5a38a1f, 0x3093, 0x4dd9, 0x9e, 0x6c, 0x87, 0x62, 0xe4, 0x22, 0x4f, 0x10);

// The next category guid is NOT based on a column
// {5DE8798F-9E55-4a78-804E-274A906BC3B3}
DEFINE_GUID(CAT_GUID_VALUE, 0x5de8798f, 0x9e55, 0x4a78, 0x80, 0x4e, 0x27, 0x4a, 0x90, 0x6b, 0xc3, 0xb3);

CFolderViewImplCategoryProvider::CFolderViewImplCategoryProvider(IShellFolder2* psf) : m_cRef(1), m_psf(psf)
{
    m_psf->AddRef();
}

CFolderViewImplCategoryProvider::~CFolderViewImplCategoryProvider()
{
    m_psf->Release();
}

HRESULT CFolderViewImplCategoryProvider::QueryInterface(__in REFIID riid, 
                                                        __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplCategoryProvider, ICategoryProvider),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplCategoryProvider::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplCategoryProvider::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
   
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

// ICategoryProvider methods
HRESULT CFolderViewImplCategoryProvider::CanCategorizeOnSCID(const PROPERTYKEY* pkey)
{
    HRESULT hr = S_FALSE;

    if (IsEqualPropertyKey(*pkey, PKEY_ItemNameDisplay) ||
        IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_AreaSize) ||
        IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_NumberOfSides) ||
        IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_DirectoryLevel))
    {        
        hr = S_OK;  
    }
    
    return hr;
}

//  Creates a category object.
HRESULT CFolderViewImplCategoryProvider::CreateCategory(const GUID* pguid, REFIID riid, __deref_out void** ppv)
{
    HRESULT hr;
    *ppv = NULL;
    
    if (riid == IID_ICategorizer)
    {
        if (IsEqualGUID(*pguid, CAT_GUID_NAME))
        {
            *ppv = new CFolderViewImplCategorizer_Name(m_psf);
            hr = *ppv ? S_OK : E_OUTOFMEMORY;
        }
        else if (IsEqualGUID(*pguid, CAT_GUID_SIDES))
        {
            *ppv = new CFolderViewImplCategorizer_Sides(m_psf);
            hr = *ppv ? S_OK : E_OUTOFMEMORY;
        }
        else if (IsEqualGUID(*pguid, CAT_GUID_SIZE))
        {
            *ppv = new CFolderViewImplCategorizer_Size(m_psf);
            hr = *ppv ? S_OK : E_OUTOFMEMORY;
        }
        else if (IsEqualGUID(*pguid, CAT_GUID_VALUE))
        {
            *ppv = new CFolderViewImplCategorizer_Value(m_psf);
            hr = *ppv ? S_OK : E_OUTOFMEMORY;
        }
        else if (IsEqualGUID(*pguid, CAT_GUID_LEVEL))
        {
            *ppv = new CFolderViewImplCategorizer_Level(m_psf);
            hr = *ppv ? S_OK : E_OUTOFMEMORY;
        }
        else
        {
            hr = E_INVALIDARG;
        }
    }
    else
    {
        hr = E_NOINTERFACE;
    }
    
    return hr;
}

//////////////////////////////////////////////////////
//
//  Retrieves the enumerator for the categories.
//
//////////////////////////////////////////////////////
HRESULT CFolderViewImplCategoryProvider::EnumCategories(__out IEnumGUID** penum)
{
    *penum = new CFolderViewImplEnumGUID();    
    return *penum ? S_OK : E_OUTOFMEMORY;
}

///////////////////////////////////////////////////////////////////////
//
//  Retrieves a globally unique identifier (GUID) that represents
//  the categorizer to use for the specified Shell column.
//
///////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategoryProvider::GetCategoryForSCID(const PROPERTYKEY* pkey, 
                                                            GUID* pguid)
{
    HRESULT hr = S_OK;
    
    if (IsEqualPropertyKey(*pkey, PKEY_ItemNameDisplay))
    {
        *pguid = CAT_GUID_NAME;
    }
    else if (IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_AreaSize))
    {
        *pguid = CAT_GUID_SIZE;
    }
    else if (IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_NumberOfSides))
    {
        *pguid = CAT_GUID_SIDES;
    }
    else if (IsEqualPropertyKey(*pkey, PKEY_Microsoft_SDKSample_DirectoryLevel))
    {
        *pguid = CAT_GUID_LEVEL;
    }
    else if (IsEqualGUID(pkey->fmtid, GUID_NULL))
    {
        /* This can be called with a NULL
        format ID. This will happen if you have a category,
        not based on a column, that gets stored in the
        property bag. When a return is made to this item,
        it will call this function with a NULL format id. */
        
        *pguid = CAT_GUID_VALUE;
    }
    else
    {
        hr = E_INVALIDARG;
    }
    
    return hr;
}

////////////////////////////////////////////////////////////////////
//
//  Retrieves the name of the specified category. This is where
//  additional categories that appear under the column
//  related categories in the UI, get their display names.
//
////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategoryProvider::GetCategoryName(const GUID* pguid, 
                                                         __out_ecount(cch) PWSTR pszName, 
                                                         UINT cch)
{
    HRESULT hr = E_FAIL;

    if (IsEqualGUID(*pguid, CAT_GUID_VALUE))
    {
        hr = LoadStringW(g_hInst, IDS_VALUE, pszName, cch) ? S_OK : E_FAIL;
    }
   
    return hr;
}

////////////////////////////////////////////////////////////
//
//  Enables the folder to override the default grouping.
//
////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategoryProvider::GetDefaultCategory(__out GUID* pguid, 
                                                            __out PROPERTYKEY* pkey)
{
    HRESULT  hr = S_OK;
    *pguid = CAT_GUID_LEVEL;
    pkey = NULL;
    return hr;
}

CFolderViewImplEnumGUID::CFolderViewImplEnumGUID() : m_cRef(1), m_ulCurrentIndex(0)
{
    
    DllAddRef();
}

CFolderViewImplEnumGUID::~CFolderViewImplEnumGUID()
{
    DllRelease();
}

HRESULT CFolderViewImplEnumGUID::QueryInterface(__in REFIID riid, 
                                                __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplEnumGUID, IEnumGUID),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplEnumGUID::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplEnumGUID::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    
    if (0 == cRef)
    {
        delete this;
    }
    
    return cRef;
}


//  IEnumGUID
/////////////////////////////////////////////////////////////////
//
//  Get next catagory GUID. The categories enumerated here 
//  appear under the categories that are not releated to the 
//  columns but can be categorized.
//
/////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplEnumGUID::Next(ULONG celt, 
                                      __out_ecount_part(celt, *pceltFetched) GUID *rgelt, 
                                      __out_opt ULONG *pceltFetched)
{
    HRESULT hr = (celt != 1) ? E_INVALIDARG : S_OK;
    
    if (SUCCEEDED(hr))
    {
        hr = (m_ulCurrentIndex < MAX_CATEGORIES) ? S_OK : S_FALSE;
        if (S_OK == hr)
        {
            switch (m_ulCurrentIndex++)
            {
            case 0:
                *rgelt = CAT_GUID_VALUE;
                break;
            }
        }
    }
    
    return hr;
}

//  Skip cch number of categories.
HRESULT CFolderViewImplEnumGUID::Skip(ULONG cch)
{
    m_ulCurrentIndex += cch;
    return S_OK;
}

//  Reset the count to zero.
HRESULT CFolderViewImplEnumGUID::Reset ()
{
    m_ulCurrentIndex = 0;
    return S_OK;
}

HRESULT CFolderViewImplEnumGUID::Clone(__deref_out IEnumGUID** ppEnum)
{
    *ppEnum = NULL;
    return E_NOTIMPL;
}

CFolderViewImplCategorizer_Name::CFolderViewImplCategorizer_Name(IShellFolder2* psf) : m_cRef(1), m_psf(psf)
{
    m_psf->AddRef();
}

CFolderViewImplCategorizer_Name::~CFolderViewImplCategorizer_Name()
{
    m_psf->Release();
}

HRESULT CFolderViewImplCategorizer_Name::QueryInterface(__in REFIID riid, 
                                                        __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplCategorizer_Name, ICategorizer),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplCategorizer_Name::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplCategorizer_Name::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    
    if (0 == cRef)
    {
        delete this;
    }
    
    return cRef;
}

///////////////////////////////////////////////////////////////////////////////////
//
//  Determines the relative order of two items in their item identifier lists.
//
///////////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Name::CompareCategory(CATSORT_FLAGS csfFlags, 
                                                         DWORD dwCategoryId1, 
                                                         DWORD dwCategoryId2)
{
    return ResultFromShort((short)(dwCategoryId1 - dwCategoryId2));
}

//////////////////////////////////////////////////////////////////////////////
//
//  Retrieves a list of categories associated with a list of identifiers.
//
//////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Name::GetCategory(UINT cidl, 
                                                     __in_ecount(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                                     __out_ecount(cidl) DWORD* rgCategoryIds)
{
    HRESULT hr = E_INVALIDARG; // cidl == 0
    
    for (UINT i = 0; i < cidl; i++)
    {
        VARIANT v = {0};
    
        hr = m_psf->GetDetailsEx(apidl[i], &PKEY_ItemNameDisplay, &v);
        if (SUCCEEDED(hr))
        {
            rgCategoryIds[i] = (DWORD)v.bstrVal[0];
            VariantClear(&v);
        }
        else
        {
            break;
        }
    }

    return hr;
}

/////////////////////////////////////////////////////////////////////
//
//  Retrieves information about a category, such as the default
//  display and the text to display in the user interface.
//
/////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Name::GetCategoryInfo(DWORD dwCategoryId, 
                                                         __out CATEGORY_INFO* pci)
{
    return StringCchPrintfW(pci->wszName, 260, L"%C", (WCHAR)dwCategoryId);
}

//////////////////////////////////////////////////////////////////////
//
//  Retrieves the name of a categorizer, such as "Group By Device 
//  Type", that can be displayed in the user interface.
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Name::GetDescription(__out_ecount(cch) PWSTR pszDesc, UINT cch)
{
   return LoadStringW(g_hInst, IDS_GROUPBYALPHA, pszDesc, cch) ? S_OK : E_FAIL;
}


CFolderViewImplCategorizer_Size::CFolderViewImplCategorizer_Size(IShellFolder2* psf) : m_cRef(1), m_psf(psf)
{
    m_psf->AddRef();
}

CFolderViewImplCategorizer_Size::~CFolderViewImplCategorizer_Size()
{
    m_psf->Release();
}

HRESULT CFolderViewImplCategorizer_Size::QueryInterface(__in REFIID riid, 
                                                        __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplCategorizer_Size, ICategorizer),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplCategorizer_Size::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplCategorizer_Size::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);

    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

///////////////////////////////////////////////////////////////////////////////////
//
//  Determines the relative order of two items in their item identifier lists.
//
///////////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Size::CompareCategory(CATSORT_FLAGS csfFlags, 
                                                         DWORD dwCategoryId1, 
                                                         DWORD dwCategoryId2)
{
    return ResultFromShort((short)(dwCategoryId1 - dwCategoryId2));
}

/////////////////////////////////////////////////////////////////////////////
//
//  Retrieves a list of categories associated with a list of identifiers.
//
/////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Size::GetCategory(UINT cidl, 
                                                     __in_ecount(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                                     __out_ecount(cidl) DWORD* rgCategoryIds)
{
    HRESULT hr = E_INVALIDARG; //cidl == 0
    
    for (UINT i = 0; i < cidl; i++)
    {
        VARIANT v = {0};
    
        hr = m_psf->GetDetailsEx(apidl[0], &PKEY_Microsoft_SDKSample_AreaSize, &v);
        if (SUCCEEDED(hr))
        {
            rgCategoryIds[i] = IDS_SMALL;

            if (VT_BSTR == v.vt)
            {
                int nSize = _wtoi(v.bstrVal);
                
                if (nSize < MAXBYTE / 3)
                {
                    rgCategoryIds[i] = IDS_SMALL;
                }
                else if (nSize < 2 * MAXBYTE / 3)
                {
                    rgCategoryIds[i] = IDS_MEDIUM;
                }
                else
                {
                    rgCategoryIds[i] = IDS_LARGE;
                }
            }
            
            VariantClear(&v);
        }
        else
        {
            break;
        }
    }
    
    return hr;
}

///////////////////////////////////////////////////////////////////
//
//  Retrieves information about a category, such as the default
//  display and the text to display in the user interface.
//
///////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Size::GetCategoryInfo(DWORD dwCategoryId, 
                                                         __out CATEGORY_INFO* pci)
{
   return LoadStringW(g_hInst, dwCategoryId, pci->wszName, 260) ? S_OK : E_FAIL;
}

//////////////////////////////////////////////////////////////////////
//
//  Retrieves the name of a categorizer, such as "Group By Device 
//  Type", that can be displayed in the user interface.
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Size::GetDescription(__out_ecount(cch) PWSTR pszDesc, UINT cch)
{
     return LoadStringW(g_hInst, IDS_GROUPBYSIZE, pszDesc, cch) ? S_OK : E_FAIL;
}

CFolderViewImplCategorizer_Sides::CFolderViewImplCategorizer_Sides(IShellFolder2* psf) : m_cRef(1), m_psf(psf)
{
    m_psf->AddRef();
}

CFolderViewImplCategorizer_Sides::~CFolderViewImplCategorizer_Sides()
{
    m_psf->Release();
}

HRESULT CFolderViewImplCategorizer_Sides::QueryInterface(__in REFIID riid, 
                                                         __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplCategorizer_Sides, ICategorizer),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplCategorizer_Sides::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplCategorizer_Sides::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);

    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

///////////////////////////////////////////////////////////////////////////////////
//
//  Determines the relative order of two items in their item identifier lists.
//
///////////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Sides::CompareCategory(CATSORT_FLAGS csfFlags,
                                                          DWORD dwCategoryId1, 
                                                          DWORD dwCategoryId2)
{
   return ResultFromShort((short)(dwCategoryId1 - dwCategoryId2));
}

//////////////////////////////////////////////////////////////////////////////
//
//  Retrieves a list of categories associated with a list of identifiers.
//
//////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Sides::GetCategory(UINT cidl, 
                                                     __in_ecount(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                                     __out_ecount(cidl) DWORD* rgCategoryIds)
{
    HRESULT hr = E_INVALIDARG;  //cidl == 0
    
    for (UINT i = 0; i < cidl; i++)
    {
        VARIANT v = {0};
    
        hr = m_psf->GetDetailsEx(apidl[0], &PKEY_Microsoft_SDKSample_NumberOfSides, &v);
        if (SUCCEEDED(hr))
        {
            rgCategoryIds[i] = IDS_POLYGON;
            if (VT_I4 == v.vt)
            {
                if (0 == v.lVal)
                {
                    rgCategoryIds[i] = IDS_CIRCLE;
                }
                else if (3 == v.lVal)
                {
                    rgCategoryIds[i] = IDS_TRIANGLE;
                }
                else if (4 == v.lVal)
                {
                    rgCategoryIds[i] = IDS_SQUARE;
                }
                else
                {
                    rgCategoryIds[i] = IDS_POLYGON;
                }
            }

            VariantClear(&v);
        }
        else
        {
            break;
        }
    }
    
    return hr;
}

////////////////////////////////////////////////////////////////////
//
//  Retrieves information about a category, such as the default
//  display and the text to display in the user interface.
//
////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Sides::GetCategoryInfo(DWORD dwCategoryId, 
                                                         __out CATEGORY_INFO* pci)
{
   return LoadStringW(g_hInst, dwCategoryId, pci->wszName, 260) ? S_OK : E_FAIL;
}

//////////////////////////////////////////////////////////////////////
//
//  Retrieves the name of a categorizer, such as "Group By Device 
//  Type", that can be displayed in the user interface.
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Sides::GetDescription(__out_ecount(cch) PWSTR pszDesc, UINT cch)
{
    return LoadStringW(g_hInst, IDS_GROUPBYSIDES, pszDesc, cch) ? S_OK : E_FAIL;
}


CFolderViewImplCategorizer_Level::CFolderViewImplCategorizer_Level(IShellFolder2* psf) : m_cRef(1), m_psf(psf)
{
    m_psf->AddRef();
}

CFolderViewImplCategorizer_Level::~CFolderViewImplCategorizer_Level()
{
    m_psf->Release();
}

HRESULT CFolderViewImplCategorizer_Level::QueryInterface(__in REFIID riid, 
                                                         __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplCategorizer_Level, ICategorizer),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplCategorizer_Level::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplCategorizer_Level::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);

    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}


//////////////////////////////////////////////////////////////////////////////////
//
//  Determines the relative order of two items in their item identifier lists.
//
//////////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Level::CompareCategory(CATSORT_FLAGS csfFlags,
                                                          DWORD dwCategoryId1, 
                                                          DWORD dwCategoryId2)
{
    return ResultFromShort((short)(dwCategoryId1 - dwCategoryId2));
}

//////////////////////////////////////////////////////////////////////////////
//
//  Retrieves a list of categories associated with a list of identifiers.
//
//////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Level::GetCategory(UINT cidl, 
                                                     __in_ecount(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                                     __out_ecount(cidl) DWORD* rgCategoryIds)
{
    HRESULT hr = E_INVALIDARG;  // cidl == 0
    
    for (UINT i = 0; i < cidl; i++)
    {
        VARIANT v = {0};
    
        hr = m_psf->GetDetailsEx(apidl[0], &PKEY_Microsoft_SDKSample_DirectoryLevel, &v);
        if (SUCCEEDED(hr))
        {
            rgCategoryIds[0] = VT_I4 == v.vt ? (DWORD)v.lVal : 0;
            VariantClear(&v);
        }
        else
        {
            break;
        }
    }
    
    return hr;
}

///////////////////////////////////////////////////////////////////
//
//  Retrieves information about a category, such as the default
//  display and the text to display in the user interface.
//
///////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Level::GetCategoryInfo(DWORD dwCategoryId, 
                                                         __out CATEGORY_INFO* pci)
{
    return StringCchPrintfW(pci->wszName, 260, L"%d", dwCategoryId);
}

//////////////////////////////////////////////////////////////////////
//
//  Retrieves the name of a categorizer, such as "Group By Device 
//  Type", that can be displayed in the user interface.
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Level::GetDescription(__out_ecount(cch) PWSTR pszDesc, UINT cch)
{
    return LoadStringW(g_hInst, IDS_GROUPBYLEVEL, pszDesc, cch) ? S_OK : E_FAIL;
}


CFolderViewImplCategorizer_Value::CFolderViewImplCategorizer_Value(IShellFolder2* psf) : m_cRef(1), m_psf(psf)
{
    m_psf->AddRef();
}

CFolderViewImplCategorizer_Value::~CFolderViewImplCategorizer_Value()
{
    m_psf->Release();
}

HRESULT CFolderViewImplCategorizer_Value::QueryInterface(__in REFIID riid, 
                                                         __deref_out void **ppv)
{
     static const QITAB qit[] = {
        QITABENT (CFolderViewImplCategorizer_Value, ICategorizer),
        { 0 },
    };
     
    return QISearch(this, qit, riid, ppv);
}

ULONG CFolderViewImplCategorizer_Value::AddRef() 
{
    return InterlockedIncrement(&m_cRef);
}

ULONG CFolderViewImplCategorizer_Value::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);

    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

///////////////////////////////////////////////////////////////////////////////////
//
//  Determines the relative order of two items in their item identifier lists.
//
///////////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Value::CompareCategory(CATSORT_FLAGS csfFlags,
                                                          DWORD dwCategoryId1, 
                                                          DWORD dwCategoryId2)
{
    return ResultFromShort((short)(dwCategoryId1 - dwCategoryId2));
}

//////////////////////////////////////////////////////////////////////////////
//
//  Retrieves a list of categories associated with a list of identifiers.
//
//////////////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Value::GetCategory(UINT cidl, 
                                                      __in_ecount(cidl) PCUITEMID_CHILD_ARRAY apidl, 
                                                      __out_ecount(cidl) DWORD* rgCategoryIds)
{
    HRESULT hr = S_OK;
    
    for (UINT i = 0; i < cidl; i++)
    {
        VARIANT v = {0};
    
        hr = m_psf->GetDetailsEx(apidl[i], &PKEY_ItemNameDisplay, &v);
        if (SUCCEEDED(hr))
        {
            PWSTR rgNames[MAX_OBJS];

            hr = LoadFolderViewImplDisplayStrings(rgNames, ARRAYSIZE(rgNames));
            if (SUCCEEDED(hr))
            {
                // Find their place in the array.
                int p = 0;
                for ( ; p < ARRAYSIZE(rgNames); p++)
                {
                    if (!StrCmpW(v.bstrVal, rgNames[p]))
                    {
                        break;
                    }
                }

                if (p < 5)
                {
                    rgCategoryIds[i] = IDS_LESSTHAN5;
                }
                else
                {
                    rgCategoryIds[i] = IDS_5ORGREATER;
                }
                // Free the strings
                for (int i = 0; i < ARRAYSIZE(rgNames); i++)
                {
                    CoTaskMemFree(rgNames[i]);
                }
            }
            
            VariantClear(&v);
         }
        else
        {
            break;
        }
    }

    return hr;
}

////////////////////////////////////////////////////////////////////
//
//  Retrieves information about a category, such as the default
//  display and the text to display in the user interface.
//
////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Value::GetCategoryInfo(DWORD dwCategoryId, 
                                                         __out CATEGORY_INFO* pci)
{
    HRESULT hr = S_OK;
    
    switch (dwCategoryId)
    {
    case IDS_LESSTHAN5:
        hr = LoadStringW(g_hInst, IDS_LESSTHAN5, pci->wszName, 260) ? S_OK : E_FAIL; 
        break;
    case IDS_5ORGREATER:
        hr = LoadStringW(g_hInst, IDS_5ORGREATER, pci->wszName, 260) ? S_OK : E_FAIL; 
        break;
    }
    
    return hr;
}

//////////////////////////////////////////////////////////////////////
//
//  Retrieves the name of a categorizer, such as "Group By Device 
//  Type", that can be displayed in the user interface.
//
//////////////////////////////////////////////////////////////////////
HRESULT CFolderViewImplCategorizer_Value::GetDescription(__out_ecount(cch) PWSTR pszDesc, UINT cch)
{
    return LoadStringW(g_hInst, IDS_GROUPBYVALUE, pszDesc, cch) ? S_OK : E_FAIL; 
}


