/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All Rights Reserved.

Abstract:
    This C++ file includes sample code for disabling Windows Firewall per profile
    using the Microsoft Windows Firewall APIs.

--********************************************************************/

#include <objbase.h>
#include <windows.h>
#include <stdio.h>
#include <comutil.h>
#include <strsafe.h>
#include <netfw.h>

#import "netfw.tlb"

HRESULT 
CoCreateInstanceAsAdmin(
    HWND hwnd, 
    REFCLSID rclsid, 
    REFIID riid, 
    __out void ** ppv
    )
{
    BIND_OPTS3 bo;
    WCHAR  wszCLSID[50];
    WCHAR  wszMonikerName[300];

    StringFromGUID2(rclsid, wszCLSID, sizeof(wszCLSID)/sizeof(wszCLSID[0])); 
    HRESULT hr = StringCchPrintf(wszMonikerName, sizeof(wszMonikerName)/sizeof(wszMonikerName[0]), L"Elevation:Administrator!new:%s", wszCLSID);
    if (FAILED(hr))
        return hr;
    memset(&bo, 0, sizeof(bo));
    bo.cbStruct = sizeof(bo);
    bo.hwnd = hwnd;
    bo.dwClassContext  = CLSCTX_LOCAL_SERVER;
    return CoGetObject(wszMonikerName, &bo, riid, ppv);
}

int __cdecl main()
{
    HRESULT hr;
    BOOL fComInitialized = FALSE;

    try
    {
        //
        // Initialize the COM library on the current thread
        //
        hr = CoInitialize(NULL); 
        if (FAILED(hr))
        {
            _com_issue_error(hr);
        }
        fComInitialized = TRUE;
        NetFwPublicTypeLib::INetFwPolicy2Ptr sipFwPolicy2;

        hr = CoCreateInstanceAsAdmin(GetDesktopWindow(), __uuidof(NetFwPolicy2), IID_PPV_ARGS(&sipFwPolicy2));
        if (FAILED(hr))
        {
            _com_issue_error(hr);
        }
        sipFwPolicy2->FirewallEnabled[NetFwPublicTypeLib::NET_FW_PROFILE2_DOMAIN] = FALSE;
        sipFwPolicy2->FirewallEnabled[NetFwPublicTypeLib::NET_FW_PROFILE2_PRIVATE] = FALSE;
        sipFwPolicy2->FirewallEnabled[NetFwPublicTypeLib::NET_FW_PROFILE2_PUBLIC] = FALSE;
    }
    catch (_com_error& e)
    {
        printf ("Error. HRESULT message is: %s (0x%08lx)\n", e.ErrorMessage(), e.Error());
        if (e.ErrorInfo())
        {
            printf ("Description: %s\n", (char *)e.Description());
        }
    }
    if (fComInitialized)
    {
        CoUninitialize();
    }
    return 0;
}
