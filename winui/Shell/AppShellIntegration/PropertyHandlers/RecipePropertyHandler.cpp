//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//

#include <shobjidl.h>    // IInitializeWithStream, IDestinationStreamFactory
#include <propsys.h>     // Property System APIs and interfaces
#include <propkey.h>     // System PROPERTYKEY definitions
#include <propvarutil.h> // PROPVARIANT and VARIANT helper APIs
#include <msxml6.h>      // DOM interfaces
#include <comutil.h>     // _bstr_t
#include <wincrypt.h>    // CryptBinaryToString, CryptStringToBinary
#include <strsafe.h>     // StringCchPrintf


//
// Releases the specified pointer if not NULL
//
#define SAFE_RELEASE(p) \
if (p)                  \
{                       \
    (p)->Release();     \
    (p) = NULL;         \
}


//
// Define PROPERTYKEYs for any custom properties defined in RecipeProperties.propdesc
//
#define INITGUID
#include <propkeydef.h>
// {1794C9FE-74A9-497f-9C69-B31F03CE7EF9}
DEFINE_PROPERTYKEY(PKEY_Microsoft_SampleRecipe_Difficulty, 0x1794c9fe, 0x74a9, 0x497f, 0x9c, 0x69, 0xb3, 0x1f, 0x3, 0xce, 0x7e, 0xf9, 100);
#undef INITGUID


//
// Map of property keys to the locations of their value(s) in the .recipe XML schema
//
struct PROPERTYMAP
{
    REFPROPERTYKEY key;
    PCWSTR pszXPathParent;
    PCWSTR pszValueNodeName;
};

PROPERTYMAP g_rgPROPERTYMAP[] =
{
    { PKEY_Title,                             L"Recipe",                L"Title" },
    { PKEY_Comment,                           L"Recipe",                L"Comments" },
    { PKEY_Author,                            L"Recipe/Background",     L"Author" },
    { PKEY_Keywords,                          L"Recipe/RecipeKeywords", L"Keyword" },
    { PKEY_Microsoft_SampleRecipe_Difficulty, L"Recipe/RecipeInfo",     L"Difficulty" },
};


//
// Helper functions to opaquely serialize and deserialize PROPVARIANT values to and from string form
//
HRESULT SerializePropVariantAsString(REFPROPVARIANT propvar, PWSTR *pszOut);
HRESULT DeserializePropVariantFromString(PCWSTR pszIn, PROPVARIANT *ppropvar);


//
// DLL lifetime management functions
//
void DllAddRef();
void DllRelease();


//
// Recipe property handler class definition
//
class CRecipePropertyStore :
    public IPropertyStore,
    public IPropertyStoreCapabilities,
    public IInitializeWithStream
{
public:
    static HRESULT CreateInstance(REFIID riid, void **ppv);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        static const QITAB qit[] = {
            QITABENT(CRecipePropertyStore, IPropertyStore),
            QITABENT(CRecipePropertyStore, IPropertyStoreCapabilities),
            QITABENT(CRecipePropertyStore, IInitializeWithStream),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        DllAddRef();
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        DllRelease();
        ULONG cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }

    // IPropertyStore
    IFACEMETHODIMP GetCount(__out DWORD *pcProps);
    IFACEMETHODIMP GetAt(DWORD iProp, __out PROPERTYKEY *pkey);
    IFACEMETHODIMP GetValue(REFPROPERTYKEY key, __out PROPVARIANT *pPropVar);
    IFACEMETHODIMP SetValue(REFPROPERTYKEY key, REFPROPVARIANT propVar);
    IFACEMETHODIMP Commit();

    // IPropertyStoreCapabilities
    IFACEMETHODIMP IsPropertyWritable(REFPROPERTYKEY key);

    // IInitializeWithStream
    IFACEMETHODIMP Initialize(IStream *pStream, DWORD grfMode);

protected:
    CRecipePropertyStore() :
        _cRef(1),
        _pStream(NULL),
        _grfMode(0),
        _pDomDoc(NULL),
        _pCache(NULL)
    {
        DllAddRef();
    }

    ~CRecipePropertyStore()
    {
        SAFE_RELEASE(_pStream);
        SAFE_RELEASE(_pDomDoc);
        SAFE_RELEASE(_pCache);
    }

    IStream*             _pStream; // data stream passed in to Initialize, and saved to on Commit
    DWORD                _grfMode; // STGM mode passed to Initialize
    IXMLDOMDocument*     _pDomDoc; // DOM object representing the .recipe file
    IPropertyStoreCache* _pCache;  // internal value cache to abstract IPropertyStore operations from the DOM back-end

    // helpers to load data from the DOM
    HRESULT _LoadCacheFromDom();
    HRESULT _LoadPropertyValues(IXMLDOMNode *pNodeParent, PCWSTR pszNodeValues, PROPVARIANT *ppropvar);
    HRESULT _LoadProperty(const PROPERTYMAP &map);
    HRESULT _LoadExtendedProperties();
    HRESULT _LoadSearchContent();

    // helpers to save data to the DOM
    HRESULT _SaveCacheToDom();
    HRESULT _SavePropertyValues(IXMLDOMNode *pNodeParent, PCWSTR pszNodeValues, REFPROPVARIANT propvar);
    HRESULT _SaveProperty(REFPROPVARIANT propvar, const PROPERTYMAP &map);
    HRESULT _SaveExtendedProperty(REFPROPERTYKEY key, REFPROPVARIANT propvar);
    HRESULT _EnsureChildNodeExists(IXMLDOMNode *pNodeParent, PCWSTR pszQuery, PCWSTR pszName, IXMLDOMNode **ppNodeChild);

private:
    long _cRef;
};


//
// Instantiates a recipe property store object
//
HRESULT CRecipePropertyStore::CreateInstance(REFIID riid, void **ppv)
{
    HRESULT hr = E_OUTOFMEMORY;
    CRecipePropertyStore *pNew = new CRecipePropertyStore;
    if (pNew)
    {
        hr = pNew->QueryInterface(riid, ppv);
        SAFE_RELEASE(pNew);
    }

    return hr;
}

HRESULT CRecipePropertyStore_CreateInstance(REFIID riid, void **ppv)
{
    return CRecipePropertyStore::CreateInstance(riid, ppv);
}


//
// Accessor methods forward directly to internal value cache
//
HRESULT CRecipePropertyStore::GetCount(__out DWORD *pcProps)
{
    HRESULT hr = E_UNEXPECTED;
    if (_pCache)
    {
        hr = _pCache->GetCount(pcProps);
    }

    return hr;
}

HRESULT CRecipePropertyStore::GetAt(DWORD iProp, __out PROPERTYKEY *pkey)
{
    HRESULT hr = E_UNEXPECTED;
    if (_pCache)
    {
        hr = _pCache->GetAt(iProp, pkey);
    }

    return hr;
}

HRESULT CRecipePropertyStore::GetValue(REFPROPERTYKEY key, __out PROPVARIANT *pPropVar)
{
    HRESULT hr = E_UNEXPECTED;
    if (_pCache)
    {
        hr = _pCache->GetValue(key, pPropVar);
    }

    return hr;
}


//
// SetValue just updates the internal value cache
//
HRESULT CRecipePropertyStore::SetValue(REFPROPERTYKEY key, REFPROPVARIANT propVar)
{
    HRESULT hr = E_UNEXPECTED;
    if (_pCache)
    {
        // check grfMode to ensure writes are allowed
        hr = STG_E_ACCESSDENIED;
        if (_grfMode & STGM_READWRITE &&
            !IsEqualPropertyKey(key, PKEY_Search_Contents))  // this property is read-only
        {
            hr = _pCache->SetValueAndState(key, &propVar, PSC_DIRTY);
        }
    }

    return hr;
}


//
// Commit writes the internal value cache back out to the stream passed to Initialize
//
HRESULT CRecipePropertyStore::Commit()
{
    HRESULT hr = E_UNEXPECTED;
    if (_pCache)
    {
        // check grfMode to ensure writes are allowed
        hr = STG_E_ACCESSDENIED;
        if (_grfMode & STGM_READWRITE)
        {
            // save the internal value cache to XML DOM object
            hr = _SaveCacheToDom();
            if (SUCCEEDED(hr))
            {
                // reset the output stream
                LARGE_INTEGER liZero = {0};
                hr = _pStream->Seek(liZero, STREAM_SEEK_SET, NULL);
                if (SUCCEEDED(hr))
                {
                    // obtain a temporary destination stream for manual safe-save
                    IDestinationStreamFactory *pSafeCommit;
                    hr = _pStream->QueryInterface(IID_PPV_ARGS(&pSafeCommit));
                    if (SUCCEEDED(hr))
                    {
                        IStream *pStreamCommit;
                        hr = pSafeCommit->GetDestinationStream(&pStreamCommit);
                        if (SUCCEEDED(hr))
                        {
                            // write the XML out to the temprorary stream and commit it
                            VARIANT varStream = {0};
                            V_VT(&varStream) = VT_UNKNOWN;
                            V_UNKNOWN(&varStream) = pStreamCommit;
                            hr = _pDomDoc->save(varStream);
                            if (SUCCEEDED(hr))
                            {
                                hr = pStreamCommit->Commit(STGC_DEFAULT);
                                if (SUCCEEDED(hr))
                                {
                                    // commit the real output stream
                                    _pStream->Commit(STGC_DEFAULT);
                                }
                            }

                            SAFE_RELEASE(pStreamCommit);
                        }

                        SAFE_RELEASE(pSafeCommit);
                    }
                }
            }
        }
    }

    return hr;
}


//
// Indicates whether the users should be able to edit values for the given property key
//
HRESULT CRecipePropertyStore::IsPropertyWritable(REFPROPERTYKEY key)
{
    // System.Search.Contents is the only property not supported for writing
    return IsEqualPropertyKey(key, PKEY_Search_Contents) ? S_FALSE : S_OK;
}


//
// Initialize populates the internal value cache with data from the specified stream
//
HRESULT CRecipePropertyStore::Initialize(IStream *pStream, DWORD grfMode)
{
    HRESULT hr = E_UNEXPECTED;
    if (!_pStream)
    {
        hr = E_POINTER;
        if (pStream)
        {
            // instantiate the DOM object
            hr = CoCreateInstance(CLSID_DOMDocument,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&_pDomDoc));
            if (SUCCEEDED(hr))
            {
                // load the DOM object's contents from the stream
                VARIANT_BOOL vfSuccess = VARIANT_FALSE;
                VARIANT varStream = {0};
                V_VT(&varStream) = VT_UNKNOWN;
                V_UNKNOWN(&varStream) = pStream;
                hr = _pDomDoc->load(varStream, &vfSuccess);
                if (hr == S_OK && vfSuccess == VARIANT_TRUE)
                {
                    // load the internal value cache from the DOM object
                    hr = _LoadCacheFromDom();
                    if (SUCCEEDED(hr))
                    {
                        // save a reference to the stream as well as the grfMode
                        hr = pStream->QueryInterface(IID_PPV_ARGS(&_pStream));
                        if (SUCCEEDED(hr))
                        {
                            _grfMode = grfMode;
                        }
                    }
                }
                else
                {
                    hr = E_FAIL;
                }

                if (FAILED(hr))
                {
                    SAFE_RELEASE(_pDomDoc);
                }
            }
        }
    }

    return hr;
}


//
// Populates the internal value cache from the internal DOM object
//
HRESULT CRecipePropertyStore::_LoadCacheFromDom()
{
    HRESULT hr = S_OK;

    if (!_pCache)
    {
        // create the internal value cache
        hr = PSCreateMemoryPropertyStore(IID_PPV_ARGS(&_pCache));
        if (SUCCEEDED(hr))
        {
            // populate native properties directly from the XML
            for (UINT i = 0; i < ARRAYSIZE(g_rgPROPERTYMAP); ++i)
            {
                _LoadProperty(g_rgPROPERTYMAP[i]);
            }

            // load extended properties and search content
            _LoadExtendedProperties();
            _LoadSearchContent();
        }
    }

    return hr;
}


//
// Loads specified values from given parent node and creates a PROPVARIANT from them
//
HRESULT CRecipePropertyStore::_LoadPropertyValues(IXMLDOMNode *pNodeParent,
                                                  PCWSTR pszValueNodeName,
                                                  PROPVARIANT *ppropvarValues)
{
    // intialize the outparam
    PropVariantInit(ppropvarValues);

    // select the value nodes
    IXMLDOMNodeList *pValueList = NULL;
    HRESULT hr = pNodeParent->selectNodes(_bstr_t(pszValueNodeName), &pValueList);
    if (hr == S_OK)
    {
        // get the count of values
        long cValues = 0;
        hr = pValueList->get_length(&cValues);
        if (SUCCEEDED(hr))
        {
            // create an array to hold the values
            hr = E_OUTOFMEMORY;
            BSTR *pbstrValues = new BSTR[cValues];
            if (pbstrValues)
            {
                // load the text of each value node into the array
                hr = S_OK;
                IXMLDOMNode *pValue = NULL;
                for (long iValue = 0; iValue < cValues; iValue++)
                {
                    if (hr == S_OK)
                    {
                        hr = pValueList->get_item(iValue, &pValue);
                        if (hr == S_OK)
                        {
                            hr = pValue->get_text(&pbstrValues[iValue]);

                            SAFE_RELEASE(pValue);
                        }
                    }
                    else
                    {
                        // NULL out remaining elements on failure
                        pbstrValues[iValue] = NULL;
                    }
                }

                if (hr == S_OK)
                {
                    // package the list of values up in a PROPVARIANT
                    hr = InitPropVariantFromStringVector((PCWSTR*)pbstrValues, cValues, ppropvarValues);
                }

                // clean up array of values
                for (long iValue = 0; iValue < cValues; iValue++)
                {
                    SysFreeString(pbstrValues[iValue]);
                }
                delete[] pbstrValues;
            }
        }
    }

    return hr;
}


//
// Loads the data for the property specified in the given map into the internal value cache
//
HRESULT CRecipePropertyStore::_LoadProperty(const PROPERTYMAP &map)
{
    // select the property's parent node and load the property's value(s)
    IXMLDOMNode *pNodeParent = NULL;
    HRESULT hr = _pDomDoc->selectSingleNode(_bstr_t(map.pszXPathParent), &pNodeParent);
    if (hr == S_OK)
    {
        PROPVARIANT propvarValues = {0};
        hr = _LoadPropertyValues(pNodeParent, map.pszValueNodeName, &propvarValues);
        if (hr == S_OK)
        {
            // coerce the value(s) to the appropriate type for the property key
            hr = PSCoerceToCanonicalValue(map.key, &propvarValues);
            if (SUCCEEDED(hr))
            {
                // cache the value(s) loaded
                hr = _pCache->SetValueAndState(map.key, &propvarValues, PSC_NORMAL);
            }

            PropVariantClear(&propvarValues);
        }

        SAFE_RELEASE(pNodeParent);
    }

    return hr;
}


//
// Loads data for any external properties (e.g. those not explicitly mapped to the XML schema) into the internal value cache
//
HRESULT CRecipePropertyStore::_LoadExtendedProperties()
{
    // select the list of extended property nodes
    IXMLDOMNodeList *pList = NULL;
    HRESULT hr = _pDomDoc->selectNodes(_bstr_t(L"Recipe/ExtendedProperties/Property"), &pList);
    if (hr == S_OK)
    {
        long cElems = 0;
        hr = pList->get_length(&cElems);
        if (hr == S_OK)
        {
            // iterate over the list and cache each value
            for (long iElem = 0; iElem < cElems; ++iElem)
            {
                IXMLDOMNode *pNode = NULL;
                hr = pList->get_item(iElem, &pNode);
                if (hr == S_OK)
                {
                    IXMLDOMElement *pElement = NULL;
                    hr = pNode->QueryInterface(IID_PPV_ARGS(&pElement));
                    if (SUCCEEDED(hr))
                    {
                        // get the name of the property and convert it to a PROPERTYKEY
                        VARIANT varPropName = {0};
                        hr = pElement->getAttribute(_bstr_t(L"Name"), &varPropName);
                        if (hr == S_OK)
                        {
                            PROPERTYKEY key;
                            hr = PSPropertyKeyFromString(V_BSTR(&varPropName), &key);
                            if (SUCCEEDED(hr))
                            {
                                // get the encoded value and deserialize it into a PROPVARIANT
                                VARIANT varEncodedValue = {0};
                                hr = pElement->getAttribute(_bstr_t(L"EncodedValue"), &varEncodedValue);
                                if (hr == S_OK)
                                {
                                    PROPVARIANT propvarValue = {0};
                                    hr = DeserializePropVariantFromString(V_BSTR(&varEncodedValue), &propvarValue);
                                    if (SUCCEEDED(hr))
                                    {
                                        // cache the value loaded
                                        hr = _pCache->SetValueAndState(key, &propvarValue, PSC_NORMAL);
                                        PropVariantClear(&propvarValue);
                                    }

                                    VariantClear(&varEncodedValue);
                                }
                            }

                            VariantClear(&varPropName);
                        }

                        SAFE_RELEASE(pElement);
                    }

                    SAFE_RELEASE(pNode);
                }
            }
        }

        SAFE_RELEASE(pList);
    }

    return hr;
}


//
// Populates the System.Search.Contents property in the internal value cache
//
HRESULT CRecipePropertyStore::_LoadSearchContent()
{
    // XSLT to generate a space-delimited list of Items, Steps, Yield, Difficulty, and Keywords
    HRESULT hr = E_OUTOFMEMORY;
    BSTR bstrContentXSLT = SysAllocString(L"<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\n"
                                          L"<xsl:output method=\"text\" version=\"1.0\" encoding=\"UTF-8\" indent=\"no\"/>\n"
                                          L"  <xsl:template match=\"/\">\n"
                                          L"    <xsl:apply-templates select=\"Recipe/Ingredients/Item\"/>\n"
                                          L"    <xsl:apply-templates select=\"Recipe/Directions/Step\"/>\n"
                                          L"    <xsl:apply-templates select=\"Recipe/RecipeInfo/Yield\"/>\n"
                                          L"    <xsl:apply-templates select=\"Recipe/RecipeInfo/Difficulty\"/>\n"
                                          L"    <xsl:apply-templates select=\"Recipe/RecipeKeywords/Keyword\"/>\n"
                                          L"  </xsl:template>\n"
                                          L"  <xsl:template match=\"*\">\n"
                                          L"    <xsl:value-of select=\".\"/>\n"
                                          L"    <xsl:text> </xsl:text>\n"
                                          L"  </xsl:template>\n"
                                          L"</xsl:stylesheet>");
    if (bstrContentXSLT)
    {
        // create a DOM object to hold the XSLT
        IXMLDOMDocument *pContentXSLT = NULL;
        hr = CoCreateInstance(CLSID_DOMDocument,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&pContentXSLT));
        if (SUCCEEDED(hr))
        {
            // load the XSLT
            VARIANT_BOOL vfSuccess = VARIANT_FALSE;
            hr = pContentXSLT->loadXML(bstrContentXSLT, &vfSuccess);
            if (!vfSuccess)
            {
                hr = FAILED(hr) ? hr : E_FAIL; // keep failed hr
            }

            if (SUCCEEDED(hr))
            {
                // get the root node of the XSLT
                IXMLDOMNode* pContentXSLTNode = NULL;
                hr = pContentXSLT->QueryInterface(IID_PPV_ARGS(&pContentXSLTNode));
                if (SUCCEEDED(hr))
                {
                    // transform the internal DOM object using the XSLT to generate the content string
                    BSTR bstrContent = NULL;
                    hr = _pDomDoc->transformNode(pContentXSLTNode, &bstrContent);
                    if (SUCCEEDED(hr))
                    {
                        // initialize a PROPVARIANT from the string, and store it in the internal value cache
                        PROPVARIANT propvarContent = {0};
                        hr = InitPropVariantFromString(bstrContent, &propvarContent);
                        if (SUCCEEDED(hr))
                        {
                            hr = _pCache->SetValueAndState(PKEY_Search_Contents, &propvarContent, PSC_NORMAL);
                            PropVariantClear(&propvarContent);
                        }

                        SysFreeString(bstrContent);
                    }

                    SAFE_RELEASE(pContentXSLT);
                }
            }

            SAFE_RELEASE(pContentXSLT);
        }

        SysFreeString(bstrContentXSLT);
    }

    return hr;
}


//
// Saves the values in the internal cache back to the internal DOM object
//
HRESULT CRecipePropertyStore::_SaveCacheToDom()
{
    // iterate over each property in the internal value cache
    DWORD cProps;
    HRESULT hr = _pCache->GetCount(&cProps);
    if (SUCCEEDED(hr))
    {
        for (UINT i = 0; SUCCEEDED(hr) && i < cProps; ++i)
        {
            PROPERTYKEY key;
            hr = _pCache->GetAt(i, &key);
            if (SUCCEEDED(hr))
            {
                // check the cache state; only save dirty properties
                PSC_STATE psc;
                hr = _pCache->GetState(key, &psc);
                if (SUCCEEDED(hr) && psc == PSC_DIRTY)
                {
                    // get the cached value
                    PROPVARIANT propvar = { 0 };
                    hr = _pCache->GetValue(key, &propvar);
                    if (SUCCEEDED(hr))
                    {
                        // save as a native property if the key is in the property map
                        BOOL fIsNativeProperty = FALSE;
                        for (UINT i = 0; i < ARRAYSIZE(g_rgPROPERTYMAP); ++i)
                        {
                            if (IsEqualPropertyKey(key, g_rgPROPERTYMAP[i].key))
                            {
                                fIsNativeProperty = TRUE;
                                hr = _SaveProperty(propvar, g_rgPROPERTYMAP[i]);
                                break;
                            }
                        }

                        // otherwise, save as an extended proeprty
                        if (!fIsNativeProperty)
                        {
                            hr = _SaveExtendedProperty(key, propvar);
                        }

                        PropVariantClear(&propvar);
                    }
                }
            }
        }
    }

    return hr;
}


//
// Saves the values in the given PROPVARIANT to the specified XML nodes
//
HRESULT CRecipePropertyStore::_SavePropertyValues(IXMLDOMNode* pNodeParent,
                                                  PCWSTR pszValueNodeName,
                                                  REFPROPVARIANT propvarValues)
{
    // iterate through each value in the PROPVARIANT
    HRESULT hr = S_OK;
    ULONG cValues = PropVariantGetElementCount(propvarValues);
    for (ULONG iValue = 0; SUCCEEDED(hr) && (iValue < cValues); iValue++)
    {
        PROPVARIANT propvarValue = {0};
        hr = PropVariantGetElem(propvarValues, iValue, &propvarValue);
        if (SUCCEEDED(hr))
        {
            // convert to a BSTR
            BSTR bstrValue = NULL;
            hr = PropVariantToBSTR(propvarValue, &bstrValue);
            if (SUCCEEDED(hr))
            {
                // create an element and set its text to the value
                IXMLDOMElement *pValue = NULL;
                hr = _pDomDoc->createElement(_bstr_t(pszValueNodeName), &pValue);
                if (SUCCEEDED(hr))
                {
                    hr = pValue->put_text(bstrValue);
                    if (SUCCEEDED(hr))
                    {
                        // append the value to its parent node
                        hr = pNodeParent->appendChild(pValue, NULL);
                    }

                    SAFE_RELEASE(pValue);
                }

                SysFreeString(bstrValue);
            }

            PropVariantClear(&propvarValue);
        }
    }

    return hr;
}


//
// Saves the given PROPVARIANT value to the XML nodes specified by the specified map
//
HRESULT CRecipePropertyStore::_SaveProperty(REFPROPVARIANT propvar, const PROPERTYMAP &map)
{
    // obtain the parent node of the value
    IXMLDOMNode *pNodeParent = NULL;
    HRESULT hr = _pDomDoc->selectSingleNode(_bstr_t(map.pszXPathParent), &pNodeParent);
    if (hr == S_OK)
    {
        // remove existing value nodes
        IXMLDOMNodeList *pNodeListValues = NULL;
        hr = pNodeParent->selectNodes(_bstr_t(map.pszValueNodeName), &pNodeListValues);
        if (hr == S_OK)
        {
            IXMLDOMSelection *pSelectionValues = NULL;
            hr = pNodeListValues->QueryInterface(IID_PPV_ARGS(&pSelectionValues));
            if (SUCCEEDED(hr))
            {
                hr = pSelectionValues->removeAll();
                SAFE_RELEASE(pSelectionValues);
            }

            SAFE_RELEASE(pNodeListValues);
        }

        if (SUCCEEDED(hr))
        {
            // save the new values to the parent node
            hr = _SavePropertyValues(pNodeParent, map.pszValueNodeName, propvar);
        }

        SAFE_RELEASE(pNodeParent);
    }

    return hr;
}


//
// Saves an extended property with the given key and value
//
HRESULT CRecipePropertyStore::_SaveExtendedProperty(REFPROPERTYKEY key, REFPROPVARIANT propvar)
{
    // convert the key to string form; don't use canonical name, since it may not be registered on other systems
    WCHAR szKey[MAX_PATH] = {0};
    HRESULT hr = PSStringFromPropertyKey(key, szKey, ARRAYSIZE(szKey));
    if (SUCCEEDED(hr))
    {
        // serialize the value to string form
        PWSTR pszValue;
        hr = SerializePropVariantAsString(propvar, &pszValue);
        
        if (SUCCEEDED(hr))
        {
            // obtain or create the ExtendedProperties node in the document
            IXMLDOMElement *pRecipe = NULL;
            hr = _pDomDoc->get_documentElement(&pRecipe);
            if (hr == S_OK)
            {
                IXMLDOMNode *pExtended = NULL;
                hr = _EnsureChildNodeExists(pRecipe, L"ExtendedProperties", L"ExtendedProperties", &pExtended);

                if (SUCCEEDED(hr))
                {
                    // query for the Property node with the given name, or create a new Property node
                    WCHAR szPropertyPath[MAX_PATH];
                    hr = StringCchPrintfW(szPropertyPath, ARRAYSIZE(szPropertyPath), L"Property[@Name = '%s']", szKey);
                    if (SUCCEEDED(hr))
                    {
                        IXMLDOMNode *pPropNode = NULL;
                        hr = _EnsureChildNodeExists(pExtended, szPropertyPath, L"Property", &pPropNode);
                        if (SUCCEEDED(hr))
                        {
                            IXMLDOMElement *pPropNodeElem = NULL;
                            hr = pPropNode->QueryInterface(IID_PPV_ARGS(&pPropNodeElem));
                            if (SUCCEEDED(hr))
                            {
                                // set the attributes of the node with the name and value of the property
                                VARIANT varKey = {0};
                                hr = InitVariantFromString(szKey, &varKey);
                                if (SUCCEEDED(hr))
                                {
                                    VARIANT varValue = {0};
                                    hr = InitVariantFromString(pszValue, &varValue);
                                    if (SUCCEEDED(hr))
                                    {
                                        hr = pPropNodeElem->setAttribute(_bstr_t(L"Name"), varKey);
                                        if (SUCCEEDED(hr))
                                        {
                                            hr = pPropNodeElem->setAttribute(_bstr_t(L"EncodedValue"), varValue);
                                        }

                                        VariantClear(&varValue);
                                    }

                                    VariantClear(&varKey);
                                }

                                SAFE_RELEASE(pPropNodeElem);
                            }

                            SAFE_RELEASE(pPropNode);
                        }
                    }

                    SAFE_RELEASE(pExtended);
                }

                SAFE_RELEASE(pRecipe);
            }
            else
            {
                hr = E_UNEXPECTED;
            }
            
            CoTaskMemFree(pszValue);
        }
    }

    return hr;
}


//
// Queries for the specified child node, and creates and appends a new one if no such node exists
//
HRESULT CRecipePropertyStore::_EnsureChildNodeExists(IXMLDOMNode *pNodeParent, PCWSTR pszQuery, PCWSTR pszName, IXMLDOMNode **ppNodeChild)
{
    // query for the child node in case it already exists
    HRESULT hr = pNodeParent->selectSingleNode(_bstr_t(L"ExtendedProperties"), ppNodeChild);
    if (hr != S_OK)
    {
        // create an element with the specified name and append it to the given parent node
        IXMLDOMElement *pChildElem = NULL;
        hr = _pDomDoc->createElement(_bstr_t(pszName), &pChildElem);
        if (SUCCEEDED(hr))
        {
            hr = pNodeParent->appendChild(pChildElem, ppNodeChild);
            pChildElem->Release();
        }
    }

    return hr;
}


//
// Serializes a PROPVARIANT value to string form
//
HRESULT SerializePropVariantAsString(REFPROPVARIANT propvar, PWSTR *pszOut)
{
    SERIALIZEDPROPERTYVALUE *pBlob;
    ULONG cbBlob;

    // serialize PROPVARIANT to binary form
    HRESULT hr = StgSerializePropVariant(&propvar, &pBlob, &cbBlob);
    if (SUCCEEDED(hr))
    {
        // determine the required buffer size
        hr = E_FAIL;
        DWORD cchString;
        if (CryptBinaryToStringW((BYTE *)pBlob, cbBlob, CRYPT_STRING_BASE64, NULL, &cchString))
        {
            // allocate a sufficient buffer
            hr = E_OUTOFMEMORY;
            *pszOut = (PWSTR)CoTaskMemAlloc(sizeof(WCHAR) * cchString);
            if (*pszOut)
            {
                // convert the serialized binary blob to a string representation
                hr = E_FAIL;
                if (CryptBinaryToStringW((BYTE *)pBlob, cbBlob, CRYPT_STRING_BASE64, *pszOut, &cchString))
                {
                    hr = S_OK;
                }
                else
                {
                    CoTaskMemFree(*pszOut);
                }
            }
        }

        CoTaskMemFree(pBlob);
    }

    return S_OK;
}


//
// Deserializes a string value back into PROPVARIANT form
//
HRESULT DeserializePropVariantFromString(PCWSTR pszIn, PROPVARIANT *ppropvar)
{
    HRESULT hr = E_FAIL;
    DWORD dwFormatUsed, dwSkip, cbBlob;

    // compute and validate the required buffer size
    if (CryptStringToBinaryW(pszIn, 0, CRYPT_STRING_BASE64, NULL, &cbBlob, &dwSkip, &dwFormatUsed) &&
        dwSkip == 0 &&
        dwFormatUsed == CRYPT_STRING_BASE64)
    {
        // allocate a buffer to hold the serialized binary blob
        hr = E_OUTOFMEMORY;
        BYTE *pbSerialized = (BYTE *)CoTaskMemAlloc(cbBlob);
        if (pbSerialized)
        {
            // convert the string to a serialized binary blob
            hr = E_FAIL;
            if (CryptStringToBinaryW(pszIn, 0, CRYPT_STRING_BASE64, pbSerialized, &cbBlob, &dwSkip, &dwFormatUsed))
            {
                // deserialized the blob back into a PROPVARIANT value
                hr = StgDeserializePropVariant((SERIALIZEDPROPERTYVALUE *)pbSerialized, cbBlob, ppropvar);
            }

            CoTaskMemFree(pbSerialized);
        }
    }

    return hr;
}
