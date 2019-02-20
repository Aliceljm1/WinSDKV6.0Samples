/**************************************************************************
    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
 
   (c) Microsoft Corporation. All Rights Reserved.
**************************************************************************/

#include <windows.h>
#include <shlwapi.h>
#include <objbase.h>
#include <Shlobj.h>
#include <olectl.h>
#include <strsafe.h>
#include "Utils.h"

// The GUID for the FolderViewImpl
#include "GUID.h"

// Implementation of ClassFactory
#include "ClassFactory.h"

#pragma comment(lib, "propsys.lib")

#define MYCOMPUTER_NAMESPACE_GUID L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace\\%s"
#define SHELL_EXT_APPROVED        L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved\\%s"

HINSTANCE g_hInst = NULL;
LONG g_cRefModule = 0;

void DllAddRef()
{
    InterlockedIncrement(&g_cRefModule);
}

void DllRelease()
{
    InterlockedDecrement(&g_cRefModule);
}

const WCHAR g_szExtTitle[] =  L"FolderView SDK Sample";

STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, void* lpReserved)
{ 
    if (DLL_PROCESS_ATTACH == dwReason)
    {
        g_hInst = hInstance;
    }
    return TRUE;
}                              

STDAPI DllCanUnloadNow(void)
{
    return g_cRefModule ? S_FALSE : S_OK;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    *ppv = NULL;

    HRESULT hr = E_OUTOFMEMORY;
    CFolderViewImplClassFactory* pClassFactory = new CFolderViewImplClassFactory();
    if (pClassFactory)
    {
        hr = pClassFactory->QueryInterface(riid, ppv);
        pClassFactory->Release();
    }
    return hr;
}

void RefreshFolderViews(UINT csidl)
{
    PIDLIST_ABSOLUTE pidl;
    if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, csidl, &pidl)))
    {
        SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_IDLIST, pidl, 0);
        CoTaskMemFree(pidl);
    }
}

// Structure to hold data for individual keys to register.
typedef struct
{
    HKEY  hRootKey;
    PCWSTR pszSubKey;
    PCWSTR pszValueName;
    BYTE *pszData;
    DWORD dwType;
} REGSTRUCT;

//
//  1. The classID must be created under HKCR\CLSID.
//     a. DefaultIcon must be set to <Path and Module>,0.
//     b. InprocServer32 set to path and module.
//        i. Threading model specified as Apartment.
//     c. ShellFolder attributes must be set.
//  2. If the extension in non-rooted, its GUID is entered at the desired folder.
//  3. It must be registered as approved for Windows NT or XP.
//
STDAPI DllRegisterServer()
{
    // Convert the IID to a string.
    WCHAR szClassID[64], szSubKey[MAX_PATH], szData[MAX_PATH];
    StringFromGUID2(CLSID_FolderViewImpl, szClassID, ARRAYSIZE(szClassID)); // Convert the IID to a string.

    // Get the path and module name.
    WCHAR szModulePathAndName[MAX_PATH];
    GetModuleFileNameW(g_hInst, szModulePathAndName, ARRAYSIZE(szModulePathAndName));

    // This will setup and register the basic ClassIDs. 
    DWORD dwData = SFGAO_FOLDER | SFGAO_HASSUBFOLDER  | SFGAO_CANDELETE;
    REGSTRUCT rgRegEntries[] = 
    {
        HKEY_CLASSES_ROOT,   L"CLSID\\%s",                  NULL,                   (LPBYTE)g_szExtTitle,   REG_SZ,    
        HKEY_CLASSES_ROOT,   L"CLSID\\%s\\InprocServer32",  NULL,                   (LPBYTE)L"%s",          REG_SZ,
        HKEY_CLASSES_ROOT,   L"CLSID\\%s\\InprocServer32",  L"ThreadingModel",      (LPBYTE)L"Apartment",   REG_SZ,
        HKEY_CLASSES_ROOT,   L"CLSID\\%s\\DefaultIcon",     NULL,                   (LPBYTE)L"%s,0",        REG_SZ,
        HKEY_CLASSES_ROOT,   L"CLSID\\%s\\ShellFolder",     L"Attributes",          (LPBYTE)&dwData,        REG_DWORD
    };

    HKEY hKey = NULL;
    HRESULT hr = S_OK;
    
    for (int i = 0; SUCCEEDED(hr) && (i < ARRAYSIZE(rgRegEntries)); i++)
    {
        // Create the sub key string.
        hr = StringCchPrintfW(szSubKey, ARRAYSIZE(szSubKey), rgRegEntries[i].pszSubKey, szClassID);
        if (SUCCEEDED(hr))
        {
            LONG lResult = RegCreateKeyExW(rgRegEntries[i].hRootKey, szSubKey, 0, NULL, 
                                           REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
            if (S_OK == lResult)
            {
                // If this is a string entry, create the string.
                if (REG_SZ == rgRegEntries[i].dwType)
                { 
                    hr = StringCchPrintfW(szData, ARRAYSIZE(szData), (LPWSTR)rgRegEntries[i].pszData, szModulePathAndName);
                    if (SUCCEEDED(hr))
                    {
                        RegSetValueExW(hKey, 
                                       rgRegEntries[i].pszValueName, 
                                       0,
                                       rgRegEntries[i].dwType, 
                                       (LPBYTE)szData, 
                                       (lstrlenW(szData) + 1) * sizeof(WCHAR));
                    }
                }
                else if (REG_DWORD == rgRegEntries[i].dwType)
                {
                    RegSetValueExW(hKey, 
                                   rgRegEntries[i].pszValueName, 
                                   0, rgRegEntries[i].dwType,
                                   rgRegEntries[i].pszData, 
                                   sizeof(DWORD));
                }
                        
                RegCloseKey(hKey);
            }
            else
            {
                hr = SELFREG_E_CLASS;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = SELFREG_E_CLASS;

        // Now we are ready to register the namespace extension.
        // This will put our extension in My Computer.
        if (SUCCEEDED(StringCchPrintfW(szSubKey, ARRAYSIZE(szSubKey), MYCOMPUTER_NAMESPACE_GUID, szClassID)))
        {
            LONG lResult = RegCreateKeyExW(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, 
                                           REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
            if (S_OK == lResult)
            {
                // Copy our name into the string.
                hr = StringCchCopyW(szData, ARRAYSIZE(szData), g_szExtTitle);
                if (SUCCEEDED(hr))
                {
                    // Set the name of our extension.
                    lResult = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szData, (lstrlenW(szData) + 1) * sizeof(szData[0]));
                    RegCloseKey(hKey);
                
                    // It is assumed at this point that we are running on Windows XP or later
                    // and therefore the extension needs to be registered as approved.
                    hr = StringCchPrintfW(szSubKey, ARRAYSIZE(szSubKey), SHELL_EXT_APPROVED, szClassID);
                    if (SUCCEEDED(hr))
                    {
                        lResult = RegCreateKeyExW(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
                                                  KEY_WRITE, NULL, &hKey, NULL);
                        if (S_OK == lResult)
                        {
                            // Create the value string.
                            hr = StringCchCopyW(szData, ARRAYSIZE(szData), g_szExtTitle);
                            if (SUCCEEDED(hr))
                            {
                                lResult = RegSetValueExW(hKey, 
                                                         NULL, 
                                                         0, 
                                                         REG_SZ, 
                                                         (LPBYTE)szData, 
                                                         (lstrlenW(szData) + 1) * sizeof(WCHAR));
                                    
                                // The Shell has to be notified that the change has been made.
                                RefreshFolderViews(CSIDL_DRIVES);

                                hr = S_OK;
                            }
                            
                            RegCloseKey(hKey);
                        }
                    }
                }
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        //Note:  The property description file needs to be in the same
        // directory as FolderViewImpl.dll when registering the DLL.
        hr = PSRegisterPropertySchema(L"FolderViewImpl.propdesc");
    }

    return hr;
}


//
// Registry keys are removed here.
//
STDAPI DllUnregisterServer()
{
    WCHAR szSubKey[MAX_PATH], szClassID[MAX_PATH];
    
    StringFromGUID2(CLSID_FolderViewImpl, szClassID, ARRAYSIZE(szClassID)); // Convert the IID to a string.
    // Delete the namespace extension entries
    HRESULT hr = StringCchPrintfW(szSubKey, ARRAYSIZE(szSubKey), MYCOMPUTER_NAMESPACE_GUID, szClassID);
    if (SUCCEEDED(hr))
    {
        SHDeleteKeyW(HKEY_LOCAL_MACHINE, szSubKey);

        // Delete the object's registry entries
        if (SUCCEEDED(StringCchPrintfW(szSubKey, ARRAYSIZE(szSubKey), L"CLSID\\%s", szClassID)))
        {
            SHDeleteKeyW(HKEY_CLASSES_ROOT, szSubKey);

            if (SUCCEEDED(StringCchPrintfW(szSubKey, ARRAYSIZE(szSubKey), SHELL_EXT_APPROVED, szClassID)))
            {
                SHDeleteKeyW(HKEY_LOCAL_MACHINE, szSubKey);
       
                // Refresh the folder views that might be open
                RefreshFolderViews(CSIDL_DRIVES);
                
                hr = S_OK;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        //Note:  The property description file needs to be in the same
        // directory as FolderViewImpl.dll when unregistering the DLL.
        hr = PSUnregisterPropertySchema(L"FolderViewImpl.propdesc");
    }
    
    return hr;
}
