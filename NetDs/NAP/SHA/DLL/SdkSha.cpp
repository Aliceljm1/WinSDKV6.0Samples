// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

// PARTICULAR PURPOSE.

//

// Copyright (c) Microsoft Corporation. All rights reserved.


#include "stdafx.h"
#include "resource.h"
#include "SdkShaModule.h"

CSdkShaModule _AtlModule;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = S_OK;

    // The default implementation, _AtlModule.DllRegisterServer(), assumes
    // that this DLL contains a type library.  Since this DLL does not
    // contain a typelib, this method needs to be expanded slightly.

    hr = _AtlModule.DllRegisterServer(FALSE);
    if (FAILED(hr))
        goto Cleanup;


    // Import all registry keys associated with this DLL. (ATL automatically
    // uses %MODULE% replacement.)
    hr = _AtlModule.UpdateRegistryFromResource(IDR_REGISTRY, TRUE, 0);
    if (FAILED(hr))
        goto Cleanup;

    // Regsiter the SDK SHV with SHA Host using the Registration API.
    hr = _AtlModule.RegisterSdkSha();

    Cleanup:
        return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = S_OK;

    // Unregsiter the SDK SHV with SHA Host using the Registration API.
    hr = _AtlModule.UnregisterSdkSha();

    hr = _AtlModule.DllUnregisterServer(FALSE);


    // Whether or not DllUnregisterServer() succeeded, this COM application
    // should remove itself from the registry.

    hr = _AtlModule.UpdateRegistryFromResource(IDR_REGISTRY, false, 0);

    return hr;
}

