#include <objbase.h>
#include <shlwapi.h>
#include <thumbcache.h> // For IThumbnailProvider.
#include <stierr.h>

extern HRESULT CRecipeThumbHandler_CreateInstance(REFIID riid, void **ppv);

static HMODULE g_hModule    = NULL ;

#define SZ_CLSID_RECIPETHUMBHANDLER     L"{50d9450f-2a80-4f08-93b9-2eb526477d1b}"
#define SZ_SHELLEXTAPPROVEDPATH         L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"
#define SZ_RECIPETHUMBHANDLER           L"Recipe Thumbnail Handler"
#define SZ_RECIPETHUMBHANDLER_PROG_ID   L"Recipe.Thumbnail.Handler"
#define SZ_RECIPETHUMBHANDLER_CUR_VER   L"Recipe.Thumbnail.Handler.1"

const CLSID CLSID_RecipeThumbHandler    = {0x50d9450f, 0x2a80, 0x4f08, {0x93, 0xb9, 0x2e, 0xb5, 0x26, 0x47, 0x7d, 0x1b}};

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = (HINSTANCE)hModule ;
        DisableThreadLibraryCalls(hModule);
        break;
    }
    return TRUE;
}

LONG g_cLocks = 0;

STDAPI DllCanUnloadNow()
{
    return (g_cLocks == 0) ? S_OK : S_FALSE;
}
 
STDAPI_(void) DllAddRef(void)
{
    InterlockedIncrement(&g_cLocks);
}

STDAPI_(void) DllRelease(void)
{
    InterlockedDecrement(&g_cLocks);
}

typedef HRESULT (*PfnCreate)(REFIID, void**);

class CClassFactory : public IClassFactory
{
public:
    CClassFactory(PfnCreate pfnCreate) : _pfnCreate(pfnCreate) { }

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        *ppv = NULL;
        HRESULT hr = E_NOINTERFACE;
        if ((riid == IID_IClassFactory) || (riid == IID_IUnknown))
        {
            *ppv = static_cast<IClassFactory *>(this);
            AddRef();
            hr = S_OK;
        }

        return hr;
    }

    STDMETHODIMP_(ULONG) AddRef()  
    {
        DllAddRef();
        return 2;
    }

    STDMETHODIMP_(ULONG) Release() 
    {
        DllRelease(); 
        return 1;
    }

    // IClassFactory
    STDMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
    {
        return pUnkOuter ? CLASS_E_NOAGGREGATION : _pfnCreate(riid, ppv);
    }

    STDMETHODIMP LockServer(BOOL fLock)
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

private:
    PfnCreate _pfnCreate;
};

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **ppv)
{
    *ppv = NULL;
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
    PfnCreate pfnCreate = NULL;

    if (clsid == CLSID_RecipeThumbHandler)
    {
        pfnCreate = CRecipeThumbHandler_CreateInstance;
    }
    
    if (pfnCreate)
    {
        CClassFactory *pcf = new CClassFactory(pfnCreate);
        if (pcf)
        {
            hr = pcf->QueryInterface(riid, ppv);
            pcf->Release();
        }
        else
        {
            hr = E_OUTOFMEMORY;
        }
    }

    return hr;
}


//
// A struct to hold the information required for a registry entry
//
struct REGISTRY_ENTRY
{
    HKEY    hkeyRoot;
    LPCWSTR pszKeyName;
    LPCWSTR pszValueName;
    LPCWSTR pszData;
};

//
// Creates a registry key (if needed) and sets the default value of the key
//
HRESULT CreateRegKeyAndSetValue(REGISTRY_ENTRY *pRegistryEntry)
{
    HRESULT hr = E_INVALIDARG;

    if (pRegistryEntry != NULL)
    {
        HKEY hKey;

        LONG lRet = RegCreateKeyExW(pRegistryEntry->hkeyRoot,
                                    pRegistryEntry->pszKeyName, 
                                    0, 
                                    NULL, 
                                    REG_OPTION_NON_VOLATILE,
                                    KEY_ALL_ACCESS, 
                                    NULL, 
                                    &hKey, 
                                    NULL);

        if (lRet != ERROR_SUCCESS)
        {
            hr = HRESULT_FROM_WIN32(lRet);
        }
        else
        {
            lRet = RegSetValueExW(hKey,
                                pRegistryEntry->pszValueName,
                                0,
                                REG_SZ,
                                (LPBYTE) pRegistryEntry->pszData,
                                ((DWORD) wcslen(pRegistryEntry->pszData) + 1) * sizeof(WCHAR));

            hr = HRESULT_FROM_WIN32(lRet);

            RegCloseKey(hKey);
        }
    }

    return hr;
}

//
// Registers this COM server
//
STDAPI DllRegisterServer()
{
    HRESULT hr = E_FAIL;
 
    WCHAR szModuleName[MAX_PATH];
      
    if (!GetModuleFileNameW(g_hModule, szModuleName, ARRAYSIZE(szModuleName)))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    else
    {
        WCHAR szIID[MAX_PATH];

        hr = StringFromGUID2(IID_IThumbnailProvider, szIID, ARRAYSIZE(szIID));

        if (SUCCEEDED(hr))
        {
            WCHAR szTemp[MAX_PATH] = {0};

            //
            // List of registry entries we want to create
            //
            REGISTRY_ENTRY rgRegistryEntries[] = {// RootKey            KeyName                                                                ValueName                     Data
                                                  {HKEY_CLASSES_ROOT,   L"CLSID\\" SZ_CLSID_RECIPETHUMBHANDLER,                                 NULL,                           SZ_RECIPETHUMBHANDLER},
                                                  {HKEY_CLASSES_ROOT,   L"CLSID\\" SZ_CLSID_RECIPETHUMBHANDLER L"\\InProcServer32",             NULL,                           szModuleName},
                                                  {HKEY_CLASSES_ROOT,   L"CLSID\\" SZ_CLSID_RECIPETHUMBHANDLER L"\\InProcServer32",             L"ThreadingModel",              L"Apartment"},
                                                  {HKEY_CLASSES_ROOT,   L"CLSID\\" SZ_CLSID_RECIPETHUMBHANDLER L"\\ProgID",                     NULL,                           SZ_RECIPETHUMBHANDLER_CUR_VER},
                                                  {HKEY_CLASSES_ROOT,   L"CLSID\\" SZ_CLSID_RECIPETHUMBHANDLER L"\\VersionIndependentProgID",   NULL,                           SZ_RECIPETHUMBHANDLER_PROG_ID},
                                                  {HKEY_CLASSES_ROOT,   SZ_RECIPETHUMBHANDLER_PROG_ID,                                          NULL,                           SZ_RECIPETHUMBHANDLER},
                                                  {HKEY_CLASSES_ROOT,   SZ_RECIPETHUMBHANDLER_PROG_ID L"\\CLSID",                               NULL,                           SZ_CLSID_RECIPETHUMBHANDLER},
                                                  {HKEY_CLASSES_ROOT,   SZ_RECIPETHUMBHANDLER_PROG_ID L"\\CurVer",                              NULL,                           SZ_RECIPETHUMBHANDLER_CUR_VER},
                                                  {HKEY_CLASSES_ROOT,   SZ_RECIPETHUMBHANDLER_CUR_VER,                                          NULL,                           SZ_RECIPETHUMBHANDLER},
                                                  {HKEY_CLASSES_ROOT,   SZ_RECIPETHUMBHANDLER_CUR_VER L"\\CLSID",                               NULL,                           SZ_CLSID_RECIPETHUMBHANDLER},
                                                  {HKEY_CLASSES_ROOT,   PathCombineW(szTemp, L".recipe\\ShellEx", szIID),                       NULL,                           SZ_CLSID_RECIPETHUMBHANDLER},
                                                  {HKEY_LOCAL_MACHINE,  SZ_SHELLEXTAPPROVEDPATH,                                                SZ_CLSID_RECIPETHUMBHANDLER,    SZ_RECIPETHUMBHANDLER}};

            for (int i = 0; i < ARRAYSIZE(rgRegistryEntries) && SUCCEEDED(hr); i++)
            {
                hr = CreateRegKeyAndSetValue(&rgRegistryEntries[i]);
            }
        }
    }
 
    return hr;
}
 
//
// Unregisters this COM server
//
STDAPI DllUnregisterServer()
{
    WCHAR szIID[MAX_PATH];
    DWORD dwError;
    
    HRESULT hr = StringFromGUID2(IID_IThumbnailProvider, szIID, ARRAYSIZE(szIID));
 
    if (SUCCEEDED(hr))
    {
        WCHAR szRecipeKey[MAX_PATH];
        LPCWSTR rgpszKeys[] = {L"CLSID\\" SZ_CLSID_RECIPETHUMBHANDLER,
                               SZ_RECIPETHUMBHANDLER_PROG_ID, 
                               SZ_RECIPETHUMBHANDLER_CUR_VER,
                               PathCombineW(szRecipeKey, L".recipe\\ShellEx", szIID)};

        //
        // Delete the different CLSID/PROGID entries
        //
        for (int i = 0; i < ARRAYSIZE(rgpszKeys) && SUCCEEDED(hr); i++)
        {
            dwError = SHDeleteKeyW(HKEY_CLASSES_ROOT, rgpszKeys[i]);
            
            hr = HRESULT_FROM_WIN32(dwError);
            if (hr == STIERR_OBJECTNOTFOUND)
            {
                // If the registry entry has already been deleted, say S_OK.
                hr = S_OK;
            }
        }

        if (SUCCEEDED(hr))
        {
            //
            // Delete the approved shell extension value
            //
            dwError = SHDeleteValueW(HKEY_LOCAL_MACHINE, SZ_SHELLEXTAPPROVEDPATH, SZ_CLSID_RECIPETHUMBHANDLER);

            hr = HRESULT_FROM_WIN32(dwError);
            if (hr == STIERR_OBJECTNOTFOUND)
            {
                // If the registry entry has already been deleted, say S_OK.
                hr = S_OK;
            }
        } 
    }
    
    return hr;
}
