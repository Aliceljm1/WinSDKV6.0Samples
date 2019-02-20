//////////////////////////////////////////////////////////////////////////
//
// dllmain.cpp : Implements DLL exports and COM class factory
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Note: This source file implements the class factory for the sample
//       media source, plus the following DLL functions:
//       - DllMain
//       - DllCanUnloadNow
//       - DllRegisterServer
//       - DllUnregisterServer
//       - DllGetClassObject
//
//////////////////////////////////////////////////////////////////////////

#include "WavSource.h"
#include "WavByteStreamHandler.h"

#include <assert.h>

#include <strsafe.h>

#include <initguid.h>
#include "WavSourceGuids.h"

HMODULE g_hModule = NULL;    // DLL module handle
long    g_cObjects = 0;      // Number of active COM objects


const DWORD CHARS_IN_GUID = 39;

// Function pointer for creating COM objects. (Used by the class factory.)
typedef HRESULT (*CreateInstanceFn)(REFIID iid, void **ppMEG);

// g_ClassFactories: Array of class factory data.
// Defines a look-up table of CLSIDs and corresponding creation functions.

struct ClassFactoryData
{
    const GUID          *pclsid;
    CreateInstanceFn    pfnCreate;
};

ClassFactoryData g_ClassFactories[] =
{
    {   &CLSID_MFSampleWavSource,                   WavSource::CreateInstance },
    {   &CLSID_MFSampleWavSourceByteStreamHandler,  CWavByteStreamHandler::CreateInstance }
};
      
DWORD g_numClassFactories = ARRAY_SIZE(g_ClassFactories);

// Text strings

const TCHAR* sByteStreamHandlerDescription = TEXT("WAVE Source ByteStreamHandler");
const TCHAR* sWavFileExtension = TEXT(".wav");
const TCHAR* REGKEY_MF_BYTESTREAM_HANDLERS 
                = TEXT("Software\\Microsoft\\Windows Media Foundation\\ByteStreamHandlers");




// Forward declares

// Functions to register and unregister our COM objects.

HRESULT RegisterComObjectCLSID(const GUID& guid, const TCHAR *sDescription, const TCHAR *sThreadingModel);
HRESULT UnregisterComObjectCLSID(const GUID& guid);
HRESULT CreateObjectKeyName(const GUID& guid, TCHAR *sName, DWORD cchMax);

// Functions to register and unregister the byte stream handler.

HRESULT RegisterByteStreamHandler(const GUID& guid, const TCHAR *sFileExtension, const TCHAR *sDescription);
HRESULT UnregisterByteStreamHandler(const GUID& guid, const TCHAR *sFileExtension);

// Misc Registry helpers
HRESULT SetKeyValue(HKEY hKey, const TCHAR *sName, const TCHAR *sValue);



// ClassFactory:
// Implements the class factory for the COM objects.

class ClassFactory : public IClassFactory
{
private:
    volatile long           m_refCount;     // Reference count.
    static volatile long    m_serverLocks;  // Number of server locks

    CreateInstanceFn        m_pfnCreation;  // Function to create an instance of the object.

public:

    ClassFactory(CreateInstanceFn pfnCreation) : m_pfnCreation(pfnCreation), m_refCount(1)
    {
    }

    static bool IsLocked()
    {
        return (m_serverLocks != 0);
    }

    // IUnknown methods
    STDMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&m_refCount);
    }
    STDMETHODIMP_(ULONG) Release()
    {
        assert(m_refCount >= 0);
        ULONG uCount = InterlockedDecrement(&m_refCount);
        if (uCount == 0)
        {
            delete this;
        }
        // Return the temporary variable, not the member
        // variable, for thread safety.
        return uCount;
    }
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if (NULL == ppv)
        {
            return E_POINTER;
        }
        else if (riid == __uuidof(IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }
        else if (riid == __uuidof(IClassFactory))
        {
            *ppv = static_cast<IClassFactory*>(this);
        }
        else 
        {
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }


    STDMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
    {
        // We do not support aggregation. 
        if (pUnkOuter != NULL)
        {
            return CLASS_E_NOAGGREGATION;
        }

        return m_pfnCreation(riid, ppv);
    }

    STDMETHODIMP LockServer(BOOL lock)
    {   
        if (lock)
        {
            InterlockedIncrement(&m_serverLocks);
        }
        else
        {
            InterlockedDecrement(&m_serverLocks);
        }
        return S_OK;
    }

};


volatile long ClassFactory::m_serverLocks = 0;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = (HMODULE)hModule;
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

STDAPI DllCanUnloadNow()
{
    if (!ClassFactory::IsLocked() && g_cObjects == 0)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}


STDAPI DllRegisterServer()
{
    HRESULT hr;
    
    // Register the media source as a COM object.
    hr = RegisterComObjectCLSID(CLSID_MFSampleWavSource, TEXT("WAVE File Source"), TEXT("Both"));

    // Register the bytestream handler as a COM object.
    if (SUCCEEDED(hr))
    {
        hr = RegisterComObjectCLSID(
                CLSID_MFSampleWavSourceByteStreamHandler, 
                sByteStreamHandlerDescription, 
                TEXT("Both")
                );
    }

    // Register the bytestream handler as a Media Foundation bytestream handler for the ".wav" 
    // file extension.
    if (SUCCEEDED(hr))
    {
        RegisterByteStreamHandler(
            CLSID_MFSampleWavSourceByteStreamHandler, 
            sWavFileExtension, 
            sByteStreamHandlerDescription
            );
    }

    return hr;
}

STDAPI DllUnregisterServer()
{
    UnregisterComObjectCLSID(CLSID_MFSampleWavSource);
    UnregisterComObjectCLSID(CLSID_MFSampleWavSourceByteStreamHandler);
    UnregisterByteStreamHandler(CLSID_MFSampleWavSourceByteStreamHandler, sWavFileExtension);
    return S_OK;
}

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
{
    ClassFactory *pFactory = NULL;

    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE; // Default to failure

    // Find an entry in our look-up table for the specified CLSID.
    for (DWORD index = 0; index < g_numClassFactories; index++)
    {
        if (*g_ClassFactories[index].pclsid == clsid)
        {
            // Found an entry. Create a new class factory object.
            pFactory = new ClassFactory(g_ClassFactories[index].pfnCreate);
            if (pFactory)
            {
                hr = S_OK;
            }
            else
            {
                hr = E_OUTOFMEMORY;
            }
            break;
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = pFactory->QueryInterface(riid, ppv);
    }
    SAFE_RELEASE(pFactory);

    return hr;
}

///////////////////////////////////////////////////////////////////////
// Name: CreateRegistryKey
// Desc: Creates a new registry key. (Thin wrapper just to encapsulate
//       all of the default options.)
///////////////////////////////////////////////////////////////////////

HRESULT CreateRegistryKey(HKEY hKey, LPCTSTR subkey, HKEY *phKey)
{
    assert(phKey != NULL);

    LONG lreturn = RegCreateKeyEx(
        hKey,				  // parent key
        subkey,				  // name of subkey
        0,                    // reserved
        NULL,                 // class string (can be NULL)
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        NULL,                 // security attributes
        phKey,
        NULL                  // receives the "disposition" (is it a new or existing key)
        );

    return __HRESULT_FROM_WIN32(lreturn);
}


///////////////////////////////////////////////////////////////////////
// Name: RegisterComObjectCLSID
// Desc: Creates the registry entries for a COM object.
//
// guid: The object's CLSID
// sDescription: Description of the object
// sThreadingMode: Threading model. e.g., "Both"
///////////////////////////////////////////////////////////////////////

HRESULT RegisterComObjectCLSID(const GUID& guid, const TCHAR *sDescription, const TCHAR *sThreadingModel)
{
    assert(g_hModule != NULL); 

    HKEY hKey = NULL;
    HKEY hSubKey = NULL;

    TCHAR achTemp[MAX_PATH];

    // Create the name of the key from the object's CLSID
    HRESULT hr = CreateObjectKeyName(guid, achTemp, MAX_PATH);

    // Create the new key.
    if (SUCCEEDED(hr))
    {
        hr = CreateRegistryKey(HKEY_CLASSES_ROOT, achTemp, &hKey);
    }

    // The default key value is a description of the object.
    if (SUCCEEDED(hr))
    {
        hr = SetKeyValue(hKey, NULL, sDescription);
    }

    // Create the "InprocServer32" subkey
    if (SUCCEEDED(hr))
    {
        const TCHAR *sServer = TEXT("InprocServer32");

        hr = CreateRegistryKey(hKey, sServer, &hSubKey);
    }

    // The default value for this subkey is the path to the DLL.
    // Get the name of the module ...
    if (SUCCEEDED(hr))
    {
        DWORD res = GetModuleFileName(g_hModule, achTemp, MAX_PATH);
        if (res == 0)
        {
            hr = __HRESULT_FROM_WIN32(GetLastError());
        }
        if (res == MAX_PATH)
        {
            hr = E_FAIL; // buffer too small
        }
    }

    // ... and set the default key value.
    if (SUCCEEDED(hr))
    {
        hr = SetKeyValue(hSubKey, NULL, achTemp);
    }

    // Add a new value to the subkey, for "ThreadingModel" = <threading model>
    if (SUCCEEDED(hr))
    {
        hr = SetKeyValue(hSubKey, TEXT("ThreadingModel"), sThreadingModel);
    }

    // close hkeys

    if (hSubKey != NULL)
    {
        RegCloseKey( hSubKey );
    }

    if (hKey != NULL)
    {
        RegCloseKey( hKey );
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////
// Name: UnregisterComObjectCLSID
// Desc: Deletes the registry entries for a COM object.
//
// guid: The object's CLSID
///////////////////////////////////////////////////////////////////////

HRESULT UnregisterComObjectCLSID(const GUID& guid)
{
    TCHAR achTemp[MAX_PATH];

    HRESULT hr = CreateObjectKeyName(guid, achTemp, MAX_PATH);

    if (SUCCEEDED(hr))
    {
        DWORD res = RegDeleteKey(HKEY_CLASSES_ROOT, achTemp);
        if (res == ERROR_SUCCESS)
        {
            hr = S_OK;
        }
        else
        {
            hr = __HRESULT_FROM_WIN32(res);
        }
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////
// Name: CreateObjectKeyName
// Desc: Converts a CLSID into a string with the form "CLSID\{clsid}"
///////////////////////////////////////////////////////////////////////

HRESULT CreateObjectKeyName(const GUID& guid, TCHAR *sName, DWORD cchMax)
{
  // convert CLSID uuid to string 
  OLECHAR szCLSID[CHARS_IN_GUID];
  HRESULT hr = StringFromGUID2(guid, szCLSID, CHARS_IN_GUID);
  if (FAILED(hr))
  {
      return hr;
  }

  // Create a string of the form "CLSID\{clsid}"
  return StringCchPrintf(sName, cchMax, TEXT("CLSID\\%ls"), szCLSID);
}


///////////////////////////////////////////////////////////////////////
// Name: SetKeyValue
// Desc: Sets a string value (REG_SZ) for a registry key
//
// hKey:   Handle to the registry key.
// sName:  Name of the value. Use NULL for the default value.
// sValue: The string value.
///////////////////////////////////////////////////////////////////////

HRESULT SetKeyValue(HKEY hKey, const TCHAR *sName, const TCHAR *sValue)
{
    size_t cch = 0;

    HRESULT hr = StringCchLength(sValue, MAXLONG, &cch);
    if (SUCCEEDED(hr))
    {
        // Size must include NULL terminator, which is not counted in StringCchLength
        DWORD  cbData = ((DWORD)cch + 1) * sizeof(TCHAR);

        // set description string
        LONG ret = RegSetValueEx(hKey, sName, 0, REG_SZ, (BYTE*)sValue, cbData);
        if (ret == ERROR_SUCCESS)
        {
            hr = S_OK;
        }
        else
        {
            hr = HRESULT_FROM_WIN32(ret);
        }
    }
    return hr;
}

///////////////////////////////////////////////////////////////////////
// Name: RegisterByteStreamHandler
// Desc: Register a bytestream handler for the Media Foundation
//       source resolver.
//
// guid:            CLSID of the bytestream handler.
// sFileExtension:  File extension.
// sDescription:    Description.
//
// Note: sFileExtension can also be a MIME type although that is not
//       illustrated in this sample.
///////////////////////////////////////////////////////////////////////

HRESULT RegisterByteStreamHandler(const GUID& guid, const TCHAR *sFileExtension, const TCHAR *sDescription)
{
    HRESULT hr = S_OK;

    // Open HKCU/<byte stream handlers>/<file extension>
    
    // Create {clsid} = <description> key

    HKEY	hKey = NULL;
    HKEY	hSubKey = NULL;

    OLECHAR szCLSID[CHARS_IN_GUID];

    size_t	cchDescription = 0;
    hr = StringCchLength(sDescription, STRSAFE_MAX_CCH, &cchDescription);
    
    if (SUCCEEDED(hr))
    {
        hr = StringFromGUID2(guid, szCLSID, CHARS_IN_GUID);
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateRegistryKey(HKEY_LOCAL_MACHINE, REGKEY_MF_BYTESTREAM_HANDLERS, &hKey);
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateRegistryKey(hKey, sFileExtension, &hSubKey);
    }

    if (SUCCEEDED(hr))
    {
        hr = RegSetValueEx(
            hSubKey,
            szCLSID,
            0,
            REG_SZ,
            (BYTE*)sDescription,
            static_cast<DWORD>((cchDescription + 1) * sizeof(TCHAR))
            );
    }

    if (hSubKey != NULL)
    {
        RegCloseKey( hSubKey );
    }

    if (hKey != NULL)
    {
        RegCloseKey( hKey );
    }

    return hr;
}


HRESULT UnregisterByteStreamHandler(const GUID& guid, const TCHAR *sFileExtension)
{
    TCHAR achTemp[MAX_PATH];

    HRESULT hr = S_OK;
    
    hr = StringCchPrintf(achTemp, MAX_PATH, TEXT("%s\\%s"), REGKEY_MF_BYTESTREAM_HANDLERS, sFileExtension);

    if (SUCCEEDED(hr))
    {
        DWORD res = RegDeleteKey(HKEY_CLASSES_ROOT, achTemp);
        if (res == ERROR_SUCCESS)
        {
            hr = S_OK;
        }
        else
        {
            hr = __HRESULT_FROM_WIN32(res);
        }
    }

    return hr;
}

