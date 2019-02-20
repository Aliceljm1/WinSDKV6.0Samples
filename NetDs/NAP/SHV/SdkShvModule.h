// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

// PARTICULAR PURPOSE.

//

// Copyright (c) Microsoft Corporation. All rights reserved.

#ifndef SDK_SHV_MODULE_H
#define SDK_SHV_MODULE_H

#include "stdafx.h"
#include "DebugHelper.h"
#include <NapTypes.h>
#include <NapServerManagement.h>
#include "SampleShv.h"
#include <Strsafe.h>
#include "SdkCommon.h"

using namespace SDK_SAMPLE_COMMON;
using namespace SDK_SAMPLE_SHV;

static const wchar_t SDK_SHV_friendlyName[] = L"SDK SHV Sample";
static const wchar_t SDK_SHV_version[] = L"1.0.0.1";
static const wchar_t SDK_SHV_description[] = L"System Health Validator(SHV)";
static const wchar_t SDK_SHV_vendorName[] = L"Microsoft";

// {fc0a5b5d-2a73-4cf9-aaa7-c6bf5860ac1e}
static const GUID SHV_CLSID_INFO =
{ 0xfc0a5b5d, 0x2a73, 0x4cf9, 0xaa, 0xa7, 0xc6, 0xbf, 0x58, 0x60, 0xac, 0x1e };

class CSdkShvModule : public CAtlDllModuleT< CSdkShvModule >
{
public:

    /// Registers the SDK SHV with the NAP Server.
    HRESULT RegisterSdkShv() throw();

    /// Unregisters the SDK SHV with the NAP Server.
    HRESULT UnregisterSdkShv() throw();

private:

    /// Fill the NapComponentRegistrationInfo structure that needs to be passed during registration.
    HRESULT FillShvComponentRegistrationInfo (NapComponentRegistrationInfo *shvInfo) throw();

    // Helper Function for releaseing ShvComponentRegistrationInfo members
    void FreeComponentRegistration(NapComponentRegistrationInfo *shvInfo) throw();
};

#endif