/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All Rights Reserved.

Abstract:
    This C++ file includes sample code for reading Windows Firewall Settings per profile using
    the Microsoft Windows Firewall APIs.

--********************************************************************/


#include <objbase.h>
#include <windows.h>
#include <stdio.h>
#include <comutil.h>
#include <strsafe.h>
#include <netfw.h>

#import "netfw.tlb"

void 
Get_FirewallSettings_PerProfileType(
    NetFwPublicTypeLib::NET_FW_PROFILE_TYPE2_ ProfileTypePassed, 
    NetFwPublicTypeLib::INetFwPolicy2Ptr sipFwPolicy2
    )
{
    printf("******************************************\n");    
    printf ("Firewall is %s\n",  
            sipFwPolicy2->FirewallEnabled[ProfileTypePassed] ?
            "enabled" : "disabled");
    printf ("Inbound traffic is %sallowed\n",  
        sipFwPolicy2->BlockAllInboundTraffic[ProfileTypePassed] ?
            "not " : "");
    printf ("Notifications are %s\n",  
            sipFwPolicy2->NotificationsDisabled[ProfileTypePassed] ?
            "disabled" : "enabled");
    printf ("UnicastResponsesToMulticastBroadcast is %s\n",  
            sipFwPolicy2->UnicastResponsesToMulticastBroadcastDisabled[ProfileTypePassed] ?
            "disabled" : "enabled");
    printf ("Default inbound action is %s\n",
            sipFwPolicy2->DefaultInboundAction[ProfileTypePassed] != NetFwPublicTypeLib::NET_FW_ACTION_BLOCK ?
            "Allow" : "Block");
    printf ("Default Outbound action is %s\n",
            sipFwPolicy2->DefaultOutboundAction[ProfileTypePassed] != NetFwPublicTypeLib::NET_FW_ACTION_BLOCK ?
            "Allow" : "Block");
    printf("\n");
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
        
        hr = sipFwPolicy2.CreateInstance("HNetCfg.FwPolicy2");
        if (FAILED(hr))
        {
            _com_issue_error(hr);
        }

        printf("Setting for the firewall domain profile:\n");
        Get_FirewallSettings_PerProfileType(NetFwPublicTypeLib::NET_FW_PROFILE2_DOMAIN, sipFwPolicy2);

        printf("Setting for the firewall private profile:\n");
        Get_FirewallSettings_PerProfileType(NetFwPublicTypeLib::NET_FW_PROFILE2_PRIVATE, sipFwPolicy2);

        printf("Setting for the firewall public profile:\n");
        Get_FirewallSettings_PerProfileType(NetFwPublicTypeLib::NET_FW_PROFILE2_PUBLIC, sipFwPolicy2);
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