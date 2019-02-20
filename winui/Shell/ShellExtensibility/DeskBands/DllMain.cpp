#include <windows.h>
#include <strsafe.h> // for StringCchXXX functions
#include <olectl.h> // for SELFREG_E_CLASS
#include <shlobj.h> // for ICatRegister
#include "ClassFactory.h" // for the class factory

// {46B3D3EF-071A-4b7e-8AA2-E560810DAB35}
CLSID CLSID_DeskBandSample = {0x46b3d3ef, 0x71a, 0x4b7e, {0x8a, 0xa2, 0xe5, 0x60, 0x81, 0xd, 0xab, 0x35}};

HINSTANCE   g_hInst     = NULL;
LONG        g_cDllRef   = 0;

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, void *)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hInst = hInst;
        break;
    }

    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if (IsEqualCLSID(CLSID_DeskBandSample, rclsid))
    {
        hr = E_OUTOFMEMORY;

        CClassFactory *pClassFactory = new CClassFactory();
        if (pClassFactory)
        {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }

    return hr;
}

STDAPI DllCanUnloadNow()
{
    return g_cDllRef > 0 ? S_FALSE : S_OK;
}

HRESULT RegisterServer()
{
    LPOLESTR psz;
    WCHAR szCLSID[MAX_PATH];
    HRESULT hr = StringFromCLSID(CLSID_DeskBandSample, &psz);
    if (SUCCEEDED(hr))
    {
        hr = StringCchCopyW(szCLSID, ARRAYSIZE(szCLSID), psz);
        CoTaskMemFree(psz);
    }

    WCHAR szSubkey[MAX_PATH];
    HKEY hKey;
    DWORD dwDisp;

    if (SUCCEEDED(hr))
    {
        hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
        if (SUCCEEDED(hr))
        {
            hr = E_FAIL;
            if (ERROR_SUCCESS == RegCreateKeyExW(HKEY_CLASSES_ROOT, 
                                                 szSubkey, 
                                                 0, 
                                                 NULL, 
                                                 REG_OPTION_NON_VOLATILE, 
                                                 KEY_WRITE, 
                                                 NULL, 
                                                 &hKey, 
                                                 &dwDisp))
            {
                WCHAR szName[] = L"DeskBand Sample";
                if (ERROR_SUCCESS == RegSetValueExW(hKey, 
                                                    NULL, 
                                                    0, 
                                                    REG_SZ, 
                                                    (LPBYTE) szName, 
                                                    sizeof(szName)))
                {
                    hr = S_OK;
                }

                RegCloseKey(hKey);
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s\\InprocServer32", szCLSID);
        if (SUCCEEDED(hr))
        {
            hr = E_FAIL;
            if (ERROR_SUCCESS == RegCreateKeyExW(HKEY_CLASSES_ROOT, 
                                                 szSubkey, 
                                                 0, 
                                                 NULL, 
                                                 REG_OPTION_NON_VOLATILE, 
                                                 KEY_WRITE, 
                                                 NULL, 
                                                 &hKey, 
                                                 &dwDisp))
            {
                WCHAR szModule[MAX_PATH];
                if (GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule)))
                {
                    DWORD cch;
                    hr = StringCchLengthW(szModule, ARRAYSIZE(szModule), (size_t *)(&cch));
                    if (SUCCEEDED(hr))
                    {
                        if (ERROR_SUCCESS != RegSetValueExW(hKey, 
                                                            NULL, 
                                                            0, 
                                                            REG_SZ, 
                                                            (LPBYTE) szModule, 
                                                            cch * sizeof(WCHAR)))
                        {
                            hr = E_FAIL;
                        }
                    }
                }

                if (SUCCEEDED(hr))
                {
                    WCHAR szModel[] = L"Apartment";
                    if (ERROR_SUCCESS != RegSetValueExW(hKey, 
                                                        L"ThreadingModel", 
                                                        0, 
                                                        REG_SZ, 
                                                        (LPBYTE) szModel, 
                                                        sizeof(szModel)))
                    {
                        hr = E_FAIL;
                    }
                }

                RegCloseKey(hKey);
            }
        }
    }

    return hr;
}

HRESULT RegisterComCat()
{
    CoInitialize(NULL);

    ICatRegister *pcr;
    HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
                                  NULL, 
                                  CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pcr));
    if (SUCCEEDED(hr))
    {
        CATID catid = CATID_DeskBand;
        hr = pcr->RegisterClassImplCategories(CLSID_DeskBandSample, 1, &catid);
        pcr->Release();
    }

    CoUninitialize();

    return hr;
}

STDAPI DllRegisterServer()
{
    // Register the deskband object.
    HRESULT hr = RegisterServer();
    if (SUCCEEDED(hr))
    {
        // Register the component category for the deskband object.
        hr = RegisterComCat();
    }

    return SUCCEEDED(hr) ? S_OK : SELFREG_E_CLASS;
}

STDAPI DllUnregisterServer()
{
    LPOLESTR psz;
    WCHAR szCLSID[MAX_PATH];
    HRESULT hr = StringFromCLSID(CLSID_DeskBandSample, &psz);
    if (SUCCEEDED(hr))
    {
        hr = StringCchCopyW(szCLSID, ARRAYSIZE(szCLSID), psz);
        CoTaskMemFree(psz);
    }

    if (SUCCEEDED(hr))
    {
        WCHAR szSubkey[MAX_PATH];
        hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
        if (SUCCEEDED(hr))
        {
            if (ERROR_SUCCESS != RegDeleteTreeW(HKEY_CLASSES_ROOT, szSubkey))
            {
                hr = E_FAIL;
            }
        }
    }

    return SUCCEEDED(hr) ? S_OK : SELFREG_E_CLASS;
}
