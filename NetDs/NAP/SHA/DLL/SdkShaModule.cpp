// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

// PARTICULAR PURPOSE.

//

// Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include "NapUtil.h"
#include "SdkShaModule.h"


/// Registers the SDK SHA with the NAP Server.
HRESULT CSdkShaModule::RegisterSdkSha()
{
	HRESULT hr = S_OK;
	///// Pointer to the INapServerManagement interface
	//CComPtr<INapSystemHealthAgentRegistration> m_pSHAMgmt = NULL;
	/// Pointer to the INapServerManagement interface
	CComPtr<INapClientManagement> m_pNAPClientMgmt = NULL;
	/// Registration Information
	NapComponentRegistrationInfo m_shaInfo;

	ZeroMemory (&m_shaInfo, sizeof(m_shaInfo));

    //hr = m_pNAPClientMgmt.CoCreateInstance(CLSID_NapSystemHealthAgentRegistration,
    //                       NULL,
    //                       CLSCTX_INPROC_SERVER);
    hr = m_pNAPClientMgmt.CoCreateInstance(CLSID_NapClientManagement,
                                            NULL,
                                            CLSCTX_INPROC_SERVER);
    if (FAILED(hr))
	{
		DebugPrintfW(L"RegisterSdkSha: CoCreateInstance Failed with %#x",hr);
		goto Cleanup;
	}

	hr = FillShaComponentRegistrationInfo(&m_shaInfo);
	if(FAILED(hr))
	{
		DebugPrintfW(L"RegisterSdkSha:: FillShaComponentRegistrationInfo Failed with %#x",hr);
		goto Cleanup;
	}

	hr = m_pNAPClientMgmt->RegisterSystemHealthAgent(&m_shaInfo);
	if (FAILED(hr))
	{
		DebugPrintfW(L"RegisterSdkSha:: RegisterSystemHealthAgent failed %#x", hr);
		goto Cleanup;
	}

Cleanup:
	FreeComponentRegistration(&m_shaInfo);
	return hr;
}

/// Unregisters the SDK SHA with the NAP Server.
HRESULT CSdkShaModule::UnregisterSdkSha()
{
	HRESULT hr = S_OK;
    //CComPtr<INapSystemHealthAgentRegistration> m_pSHAMgmt = NULL;
    CComPtr<INapClientManagement> m_pNAPClientMgmt = NULL;

	//hr = m_pNAPClientMgmt.CoCreateInstance(CLSID_NapSystemHealthAgentRegistration,
 //                          NULL,
 //                          CLSCTX_INPROC_SERVER);
	hr = m_pNAPClientMgmt.CoCreateInstance(CLSID_NapClientManagement,
                                            NULL,
                                            CLSCTX_INPROC_SERVER);
    if (FAILED(hr))
	{
		DebugPrintfW(L"UnregisterSdkSha: CoCreateInstance Failed with %#x",hr);
		goto Cleanup;
	}

	hr = m_pNAPClientMgmt->UnregisterSystemHealthAgent(QuarSampleSystemHealthId);
	if (FAILED(hr))
	{
		DebugPrintfW(L"UnregisterSdkSha: UnregisterSystemHealthAgent Failed with %#x",hr);
		goto Cleanup;
	}

Cleanup:
	return hr;
}

/// Fill the NapComponentRegistrationInfo structure that needs to be passed during registration.
HRESULT CSdkShaModule::FillShaComponentRegistrationInfo (NapComponentRegistrationInfo *agentInfo)
{
    HRESULT hr = S_OK;
    agentInfo->id = QuarSampleSystemHealthId;
    agentInfo->infoClsid = CLSID_INFO; 

    hr = ConstructCountedString(SHA_FRIENDLY_NAME, sizeof(SHA_FRIENDLY_NAME)/sizeof(WCHAR), &(agentInfo->friendlyName));
    if (FAILED(hr))
    {
        DebugPrintfW(L"SdkSha::FillShaComponentRegistrationInfo(): ConstructCountedString for friendlyName returned error %#x!",hr);
        goto Cleanup;
    }

    hr = ConstructCountedString(SHA_DESCRIPTION, sizeof(SHA_DESCRIPTION)/sizeof(WCHAR), &(agentInfo->description));
    if (FAILED(hr))
    {
        DebugPrintfW(L"SdkSha::FillShaComponentRegistrationInfo(): ConstructCountedString for description returned error %#x!",hr);
        goto Cleanup;
    }

    hr = ConstructCountedString(SHA_VERSION, sizeof(SHA_VERSION)/sizeof(WCHAR), &(agentInfo->version));
    if (FAILED(hr))
    {
        DebugPrintfW(L"SdkSha::FillShaComponentRegistrationInfo(): ConstructCountedString for version returned error %#x!",hr);
        goto Cleanup;
    }

    hr = ConstructCountedString(SHA_VENDOR_NAME, sizeof(SHA_VENDOR_NAME)/sizeof(WCHAR), &(agentInfo->vendorName));
    if (FAILED(hr))
    {
        DebugPrintfW(L"SdkSha::FillShaComponentRegistrationInfo(): ConstructCountedString for vendorName returned error %#x!",hr);
        goto Cleanup;
    }

Cleanup:
    return hr;
}


// Helper Function for releaseing ShaComponentRegistrationInfo members
void CSdkShaModule::FreeComponentRegistration(NapComponentRegistrationInfo *agentInfo)
{
    FreeMemory(agentInfo->friendlyName.string);
    FreeMemory(agentInfo->description.string);
    FreeMemory(agentInfo->version.string);
    FreeMemory(agentInfo->vendorName.string);
}

