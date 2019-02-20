/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All Rights Reserved.

Abstract:
    This C++ file includes sample code to read if a group is
    enabled in the current profile and if the APIs can be used to modify 
    the effective state using the Microsoft Windows Firewall APIs.

--********************************************************************/

#include <objbase.h>
#include <windows.h>
#include <stdio.h>
#include <comutil.h>
#include <strsafe.h>
#include <netfw.h>

#import "netfw.tlb"

HRESULT CoCreateInstanceAsAdmin(HWND hwnd, REFCLSID rclsid, REFIID riid, __out void ** ppv)
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
    long Currentprofiletypes = 0;

    try
    {
        NetFwPublicTypeLib::INetFwPolicy2Ptr sipFwPolicy2;

        //
        // Initialize the COM library on the current thread.
        //
        hr = CoInitialize(NULL);
        if (FAILED(hr))
        {
            _com_issue_error(hr);
        }
        fComInitialized = TRUE;

        hr = sipFwPolicy2.CreateInstance("HNetCfg.FwPolicy2");
        if (FAILED(hr))
        {
            _com_issue_error(hr);            
        }

        if(sipFwPolicy2->IsRuleGroupCurrentlyEnabled["File and Printer Sharing"])
            printf("File and Printer Sharing is currently enabled.\n");
        else
            printf("File and Printer Sharing is currently not enabled.\n");

        switch(sipFwPolicy2->LocalPolicyModifyState)
        {
        case NetFwPublicTypeLib::NET_FW_MODIFY_STATE_OK:
            wprintf(L"Changing or adding a firewall rule (or group) to the current profile will take effect.\n");
            break;
        case NetFwPublicTypeLib::NET_FW_MODIFY_STATE_GP_OVERRIDE:
            wprintf(L"Changing or adding a firewall rule (or group) to the current profile will not take effect because group policy overrides it.\n");
            break;
        case NetFwPublicTypeLib::NET_FW_MODIFY_STATE_INBOUND_BLOCKED:
            wprintf(L"Changing or adding an inbound firewall rule (or group) to the current profile will not take effect because inbound exceptions are not allowed.\n");
            break;
        default:
            break;
        }        
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