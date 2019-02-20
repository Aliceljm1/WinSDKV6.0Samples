// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

// PARTICULAR PURPOSE.

//

// Copyright (c) Microsoft Corporation. All rights reserved.

#ifndef __SdkQecModule_H__
#define __SdkQecModule_H__

#include "stdafx.h"
#include "DebugHelper.h"
#include <NapTypes.h>
#include <NapManagement.h>
#include <Napenforcementclient.h>
#include <Strsafe.h>
#include "SdkCommon.h"


using namespace SDK_SAMPLE_COMMON;

static const wchar_t  QEC_FRIENDLY_NAME[] = L"SDKQec";
static const wchar_t  QEC_DESCRIPTION[] = L"Microsoft SDKQec";
static const wchar_t  QEC_VERSION[] = L"1.0.0.1";
static const wchar_t  QEC_VENDOR_NAME[] = L"Microsoft";

static const UINT32 NapSdkQecId = 0x000137F1;

// {E19DDEC2-3FBE-4c3b-9317-679760C13AAE}
static const GUID CLSID_INFO =
{ 0xe19ddec2, 0x3fbe, 0x4c3b, 0x93, 0x17, 0x67, 0x97, 0x60, 0xc1, 0x3a, 0xae };


class CSdkQecModule /* : public CAtlDllModuleT< CSdkQecModule > */
{
public:

    /// Registers the SDK QEC with the NAP Agent.
    HRESULT RegisterSdkQec() throw();

    /// Unregisters the SDK QEC with the NAP Agent.
    HRESULT UnregisterSdkQec() throw();

private:

    /// Fill the NapComponentRegistrationInfo structure that needs to be passed during registration.
    HRESULT FillQecComponentRegistrationInfo (NapComponentRegistrationInfo *agentInfo) throw();

    /// Helper Function to free memory used by SDK QEC.
    void FreeComponentRegistration(NapComponentRegistrationInfo *agentInfo) throw();

};

#endif

