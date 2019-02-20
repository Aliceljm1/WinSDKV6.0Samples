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
// Note: This source file implements the class factory for the transform,
//       plus the following DLL functions:
//       - DllMain
//       - DllCanUnloadNow
//       - DllRegisterServer
//       - DllUnregisterServer
//       - DllGetClassObject
//
//////////////////////////////////////////////////////////////////////////

#include "MFT_Grayscale.h"
#include "Grayscale.h"

#include <assert.h>

#include <initguid.h>
#include "GrayscaleGuids.h"

HMODULE g_hModule = NULL;    // DLL module handle
long    g_cObjects = 0;      // Number of active COM objects

const DWORD CHARS_IN_GUID = 39;


// Forward declares

// Register and unregister objects by CLSID

HRESULT RegisterComObjectCLSID(const GUID& guid, const TCHAR *sDescription, const TCHAR *sThreadingModel);
HRESULT UnregisterComObjectCLSID(const GUID& guid);

HRESULT CreateObjectKeyName(const GUID& guid, TCHAR *sName, DWORD cchMax);
HRESULT SetKeyValue(HKEY hKey, const TCHAR *sName, const TCHAR *sValue);

// ClassFactory:
// Implements the class factory for the DMO

class ClassFactory : public IClassFactory
{
private:
    volatile long m_refCount; // Reference count.
    static volatile long m_serverLocks; // Number of server locks

public:

    ClassFactory() : m_refCount(1)
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

        return CGrayscale::CreateInstance(riid, ppv);
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
        TRACE_INIT(L"mftlog.txt");
        break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		TRACE_CLOSE();
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
    assert(g_hModule != NULL); 

	HRESULT hr = S_OK;

    hr = RegisterComObjectCLSID(CLSID_GrayscaleMFT, TEXT("Grayscale Video Effect"), TEXT("Both"));

	if (SUCCEEDED(hr))
	{
		hr = MFTRegister(
			CLSID_GrayscaleMFT,         // CLSID
			MFT_CATEGORY_VIDEO_EFFECT,  // Category
			L"Grayscale Video Effect",  // Friendly name
			0,                          // Reserved, must be zero.
			0,
			NULL,
			0,
			NULL,
			NULL
			);
	}
    return hr;
}

STDAPI DllUnregisterServer()
{
    MFTUnregister(CLSID_GrayscaleMFT);
    UnregisterComObjectCLSID(CLSID_GrayscaleMFT);

    return S_OK;
}

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
{
    if (clsid != CLSID_GrayscaleMFT)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    ClassFactory *pFactory = new ClassFactory();
    if (pFactory == NULL)
    {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();

    return hr;
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
    HKEY hSubkey = NULL;

    TCHAR achTemp[MAX_PATH];

    // Create the name of the key from the object's CLSID
    HRESULT hr = CreateObjectKeyName(guid, achTemp, MAX_PATH);

    // Create the new key.
    if (SUCCEEDED(hr))
    {
        LONG lreturn = RegCreateKeyEx(
            HKEY_CLASSES_ROOT,
            (LPCTSTR)achTemp,     // subkey
            0,                    // reserved
            NULL,                 // class string (can be NULL)
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            NULL,                 // security attributes
            &hKey,
            NULL                  // receives the "disposition" (is it a new or existing key)
            );

        hr = __HRESULT_FROM_WIN32(lreturn);
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

        LONG lreturn = RegCreateKeyEx(hKey, sServer, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubkey, NULL);

        hr = __HRESULT_FROM_WIN32(lreturn);
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
        hr = SetKeyValue(hSubkey, NULL, achTemp);
    }

    // Add a new value to the subkey, for "ThreadingModel" = <threading model>
    if (SUCCEEDED(hr))
    {
        hr = SetKeyValue(hSubkey, TEXT("ThreadingModel"), sThreadingModel);
    }

    // close hkeys

    if (hSubkey != NULL)
    {
        RegCloseKey( hSubkey );
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

