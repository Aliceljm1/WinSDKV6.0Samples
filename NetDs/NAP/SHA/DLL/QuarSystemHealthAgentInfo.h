// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

// PARTICULAR PURPOSE.

//

// Copyright (c) Microsoft Corporation. All rights reserved.


#include "stdafx.h"
#ifndef __QuarSystemHealthAgentInfo_H__
#define __QuarSystemHealthAgentInfo_H__

#pragma once

#include "resource.h"       // main symbols
#include "napcommon.h"


namespace SDK_SAMPLE_SHA
{

// Error messages should be defined
static const HRESULT E_WRONG_MSG_ID = E_INVALIDARG;


// Message Ids for Friendly names

// SDK_FRIENDLY_NAME_MSG_ID is defined at Messages.mc


// The English friendly name that will be used with GetFriendlyName()
static const WCHAR SDK_ENGLISH_FRIENDLY_NAME_STRING[] = L"SDK Sample";


static const DWORD SDK_ENGLISH_FRIENDLY_NAME_STRING_SIZE =
    sizeof(SDK_ENGLISH_FRIENDLY_NAME_STRING);


static const size_t SDK_ENGLISH_FRIENDLY_NAME_STRING_LEN =
    (SDK_ENGLISH_FRIENDLY_NAME_STRING_SIZE/sizeof(WCHAR))-1;


// The dll file name
static const WCHAR SHA_SDK_SAMPLE_DLL_FILE_NAME[] = L"SdkSha.dll";


// Handle to the dll
static const HINSTANCE SHA_SDK_SAMPLE_DLL_FILE_HANDLE = GetModuleHandleW(
    SHA_SDK_SAMPLE_DLL_FILE_NAME);


static const WORD US_ENGLISH = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);


}


// CQuarSystemHealthAgentInfo

class __declspec(novtable)
          __declspec(uuid("E19DDEC2-3FBE-4c3b-9317-679760C13AAE"))
        CQuarSystemHealthAgentInfo :
            public CComObjectRootEx<CComMultiThreadModelNoCS>,
            public CComCoClass<CQuarSystemHealthAgentInfo, &__uuidof(CQuarSystemHealthAgentInfo)>,
            public INapComponentInfo
{


public:


    // Constructor
    CQuarSystemHealthAgentInfo() throw();


    // Destructor
    ~CQuarSystemHealthAgentInfo() throw();


    DECLARE_NO_REGISTRY()

    DECLARE_NOT_AGGREGATABLE(CQuarSystemHealthAgentInfo)

    BEGIN_COM_MAP(CQuarSystemHealthAgentInfo)
        COM_INTERFACE_ENTRY(INapComponentInfo)
    END_COM_MAP()


    STDMETHOD(GetFriendlyName)(OUT MessageId* pFriendlyName) ;

    STDMETHOD(GetDescription)(OUT MessageId* description);

    STDMETHOD(GetVendorName)(OUT MessageId* pCompanyName) ;

    STDMETHOD(GetVersion)(OUT MessageId* pVersionInfo) ;

    STDMETHOD(GetIcon)(
        OUT CountedString** ppDllFilePath,
        OUT UINT* pIconResourceId) ;

    STDMETHOD(ConvertErrorCodeToMessageId)(
        IN HRESULT errorCode,
        OUT MessageId* pMsgId) ;

    STDMETHOD(GetLocalizedString)(
        IN MessageId msgId,
        OUT CountedString** ppString) ;


private:


    // Copy constructor not implemented. This explicitly prevents the
    // compiler from automatically providing a default implementation
    // of the copy constructor, which isn't guaranteed to handle member
    // variables correctly.
    CQuarSystemHealthAgentInfo(const CQuarSystemHealthAgentInfo &rhs) throw();


    // Assignment operator not implemented. This explicitly prevents the
    // compiler from automatically providing a default implementation of
    // the assignment operator, which isn't guaranteed to handle member
    // variables correctly.
    CQuarSystemHealthAgentInfo& operator=(const CQuarSystemHealthAgentInfo&rhs) throw();


};

OBJECT_ENTRY_AUTO(__uuidof(CQuarSystemHealthAgentInfo), CQuarSystemHealthAgentInfo)

#endif

