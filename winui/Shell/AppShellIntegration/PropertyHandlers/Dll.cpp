#include <shlwapi.h> // Registry helper APIs
#include <shlobj.h>  // SHChangeNotify
#include <propsys.h> // PSRegisterPropertySchema, PSUnregisterPropertySchema


//
// CLSID of the recipe property handler object
//
const CLSID CLSID_RecipePropertyStore = { 0x50d9450f, 0x2a80, 0x4f08, {0x93, 0xb9, 0x2e, 0xb5, 0x26, 0x47, 0x7d, 0x1a} };


//
// Registry path strings
//
#define SZ_PROPERTYHANDLERSPATH           L"Software\\Microsoft\\Windows\\CurrentVersion\\PropertySystem\\PropertyHandlers"
#define SZ_APPROVEDSHELLEXTENSIONS        L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"
#define SZ_RECIPEPROPERTYHANDLER          L"Recipe Property Handler"
#define SZ_CLSID_RECIPEPROPERTYHANDLER    L"{50D9450F-2A80-4F08-93B9-2EB526477D1A}"
#define SZ_FILE_EXTENSION                 L".recipe"


//
// Reference count for the DLL
//
LONG g_cLocks = 0;


//
// Handle the the DLL's module
//
HMODULE g_hmodThis;


//
// Standard DLL functions
//
BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    g_hmodThis = (HMODULE)hModule;
    return TRUE;
}

STDAPI DllCanUnloadNow()
{
    // Only allow the DLL to be unloaded after all outstanding references have been released
    return (g_cLocks == 0) ? S_OK : S_FALSE;
}

void DllAddRef()
{
    // Increment the reference count on the DLL
    InterlockedIncrement(&g_cLocks);
}

void DllRelease()
{
    // Decrement the reference count on the DLL
    InterlockedDecrement(&g_cLocks);
}


//
// Constructor for CRecipePropertyStore
//
HRESULT CRecipePropertyStore_CreateInstance(REFIID riid, void **ppv);


//
// Class factory for CRecipePropertyHandler
//
class CRecipePropertyHandlerClassFactory  : public IClassFactory
{
public:
    //
    // IUnknown methods
    //
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        static const QITAB qit[] = {
            QITABENT(CRecipePropertyHandlerClassFactory, IClassFactory),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()  
    {
        DllAddRef();
        return 2; // Object's lifetime is tied to the DLL, so no need to track its refcount
    }

    IFACEMETHODIMP_(ULONG) Release() 
    {
        DllRelease();
        return 1; // Object's lifetime is tied to the DLL, so no need to track its refcount or manually delete it
    }

    //
    // IClassFactory methods
    //
    IFACEMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
    {
        // Aggregation is not supported
        return pUnkOuter ? CLASS_E_NOAGGREGATION : CRecipePropertyStore_CreateInstance(riid, ppv);
    }

    IFACEMETHODIMP LockServer(BOOL fLock)
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
};

//
// Single global instance of the class factory object
//
CRecipePropertyHandlerClassFactory g_cfRecipePropertyHandler;


//
// Export called by CoCreateInstance to obtain a class factory for the specified CLSID
//
STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    // we only support CLSID_RecipePropertyStore
    if (clsid == CLSID_RecipePropertyStore)
    {
        hr = g_cfRecipePropertyHandler.QueryInterface(riid, ppv);
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
    DWORD   dwType;
    LPCWSTR pszData;
    DWORD   dwData;
};


//
// Creates a registry key (if needed) and sets the default value of the key
//
HRESULT CreateRegKeyAndSetValue(REGISTRY_ENTRY *pRegistryEntry)
{
    HRESULT hr = E_INVALIDARG;

    if (pRegistryEntry != NULL)
    {
        // create the key, or obtain its handle if it already exists
        HKEY hKey;
        LONG lr = RegCreateKeyExW(pRegistryEntry->hkeyRoot,
                                  pRegistryEntry->pszKeyName, 
                                  0, 
                                  NULL, 
                                  REG_OPTION_NON_VOLATILE,
                                  KEY_ALL_ACCESS, 
                                  NULL, 
                                  &hKey, 
                                  NULL);
        hr = HRESULT_FROM_WIN32(lr);
        if (SUCCEEDED(hr))
        {
            // extract the data from the struct according to its type
            LPBYTE pData = NULL;
            DWORD cbData = 0;
            hr = S_OK;
            switch (pRegistryEntry->dwType)
            {
            case REG_SZ:
                pData = (LPBYTE) pRegistryEntry->pszData;
                cbData = ((DWORD) wcslen(pRegistryEntry->pszData) + 1) * sizeof(WCHAR);
                break;

            case REG_DWORD:
                pData = (LPBYTE) &pRegistryEntry->dwData;
                cbData = sizeof(pRegistryEntry->dwData);
                break;

            default:
                hr = E_INVALIDARG;
                break;
            }

            if (SUCCEEDED(hr))
            {
                // attempt to set the value
                lr = RegSetValueExW(hKey,
                                    pRegistryEntry->pszValueName,
                                    0,
                                    pRegistryEntry->dwType,
                                    pData,
                                    cbData);
                hr = HRESULT_FROM_WIN32(lr);
            }

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

    if (!GetModuleFileNameW(g_hmodThis, szModuleName, ARRAYSIZE(szModuleName)))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    else
    {
        //
        // List of property-handler specific registry entries we need to create
        //
        REGISTRY_ENTRY rgRegistryEntries[] =
            {
                //
                // COM information
                //
                {
                    HKEY_CLASSES_ROOT,
                    L"CLSID\\" SZ_CLSID_RECIPEPROPERTYHANDLER,
                    NULL,
                    REG_SZ,
                    SZ_RECIPEPROPERTYHANDLER,
                    0
                },
                {
                    HKEY_CLASSES_ROOT,
                    L"CLSID\\" SZ_CLSID_RECIPEPROPERTYHANDLER,
                    L"ManualSafeSave",
                    REG_DWORD,
                    NULL,
                    1
                },
                {
                    HKEY_CLASSES_ROOT,
                    L"CLSID\\" SZ_CLSID_RECIPEPROPERTYHANDLER L"\\InProcServer32",
                    NULL,
                    REG_SZ,
                    szModuleName,
                    0
                },
                {
                    HKEY_CLASSES_ROOT,
                    L"CLSID\\" SZ_CLSID_RECIPEPROPERTYHANDLER L"\\InProcServer32",
                    L"ThreadingModel",
                    REG_SZ,
                    L"Apartment",
                    0
                },

                //
                // Shell information
                //
                {
                    HKEY_LOCAL_MACHINE,
                    SZ_PROPERTYHANDLERSPATH L"\\" SZ_FILE_EXTENSION,
                    NULL,
                    REG_SZ,
                    SZ_CLSID_RECIPEPROPERTYHANDLER,
                    0
                },
                {
                    HKEY_LOCAL_MACHINE,
                    SZ_APPROVEDSHELLEXTENSIONS,
                    SZ_CLSID_RECIPEPROPERTYHANDLER,
                    REG_SZ,
                    SZ_RECIPEPROPERTYHANDLER,
                    0
                }
            };

        hr = S_OK;
        for (int i = 0; i < ARRAYSIZE(rgRegistryEntries) && SUCCEEDED(hr); i++)
        {
            hr = CreateRegKeyAndSetValue(&rgRegistryEntries[i]);
        }

        if (SUCCEEDED(hr))
        {
            // inform Explorer, et al of the new handler
            SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
        }
    }
 
    return hr;
}

 
//
// Unregisters this COM server
//
STDAPI DllUnregisterServer()
{
    HRESULT hr = S_OK;

    // attempt to delete everything, even if some operations fail
    DWORD dwCLSID =           SHDeleteKeyW(HKEY_CLASSES_ROOT, L"CLSID\\" SZ_CLSID_RECIPEPROPERTYHANDLER);
    DWORD dwPropertyHandler = SHDeleteKeyW(HKEY_LOCAL_MACHINE, SZ_PROPERTYHANDLERSPATH L"\\" SZ_FILE_EXTENSION);
    DWORD dwShellExtension =  SHDeleteValueW(HKEY_LOCAL_MACHINE, SZ_APPROVEDSHELLEXTENSIONS, SZ_CLSID_RECIPEPROPERTYHANDLER);

    // return first error encountered as HRESULT
    if (dwCLSID != ERROR_SUCCESS)
    {
        hr = HRESULT_FROM_WIN32(dwCLSID);
    }
    else if (dwPropertyHandler != ERROR_SUCCESS)
    {
        hr = HRESULT_FROM_WIN32(dwPropertyHandler);
    }
    else if (dwShellExtension != ERROR_SUCCESS)
    {
        hr = HRESULT_FROM_WIN32(dwShellExtension);
    }

    if (SUCCEEDED(hr))
    {
        // inform Explorer, et al that the handler has been unregistered
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
    }

    return hr;
}


//
// Gets the path to the schema file, assuming it is in the same directory as this DLL.
//
HRESULT GetSchemaPath(LPWSTR pwszSchemaPath, UINT cchSchemaPath)
{
    HRESULT hr = E_INVALIDARG;

    if (pwszSchemaPath && cchSchemaPath >= MAX_PATH)
    {
        // get the absolute path to this dll, remove the dll filename, and append the schema filename
        hr = E_FAIL;
        if (GetModuleFileNameW(g_hmodThis, pwszSchemaPath, cchSchemaPath) &&
            PathRemoveFileSpecW(pwszSchemaPath) &&
            PathAppendW(pwszSchemaPath, L"RecipeProperties.propdesc"))
        {
            hr = S_OK;
        }
    }

    return hr;
}


//
// Installs custom .recipe properties on the system; assumes RecipeProperties.propdesc is in the same directory as this DLL.
//
STDAPI InstallPropertySchema()
{
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        WCHAR wszSchemaPath[MAX_PATH] = {0};
        hr = GetSchemaPath(wszSchemaPath, ARRAYSIZE(wszSchemaPath));
        if (SUCCEEDED(hr))
        {
            hr = PSRegisterPropertySchema(wszSchemaPath);
        }

        CoUninitialize();
    }

    return hr;
}


//
// Uninstalls custom .recipe properties from the system; assumes RecipeProperties.propdesc is in the same directory as this DLL.
//
STDAPI UninstallPropertySchema()
{
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        WCHAR wszSchemaPath[MAX_PATH] = {0};
        hr = GetSchemaPath(wszSchemaPath, ARRAYSIZE(wszSchemaPath));
        if (SUCCEEDED(hr))
        {
            hr = PSUnregisterPropertySchema(wszSchemaPath);
        }

        CoUninitialize();
    }

    return hr;
}
