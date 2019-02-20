// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

// PARTICULAR PURPOSE.

//

// Copyright (c) Microsoft Corporation. All rights reserved.


#include "stdafx.h"
#include <windows.h>
#include <new>

#include "DebugHelper.h"

#include "NapUtil.h"
#include "QuarSystemHealthAgentInfo.h"

#include "SDKCommon.h"
using namespace SDK_SAMPLE_COMMON;
using namespace SDK_SAMPLE_SHA;

#include <strsafe.h>
#include <assert.h>


// Constructor
CQuarSystemHealthAgentInfo::CQuarSystemHealthAgentInfo() throw()
{
}



// Destructor
CQuarSystemHealthAgentInfo::~CQuarSystemHealthAgentInfo() throw()
{
}





STDMETHODIMP CQuarSystemHealthAgentInfo::
GetFriendlyName(OUT MessageId* pFriendlyName)
{
    HRESULT hr = S_OK;

    // Check parameters make sure the pointer is not NULL
    // The caller should pass a valid pointer to pFriendlyName
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert(pFriendlyName != NULL);

    // Put here the exact UINT32 MessageId for the friendly name
    *pFriendlyName = SDK_SAMPLE_FRIENDLY_NAME_MSG_ID;


    return hr;
}



STDMETHODIMP CQuarSystemHealthAgentInfo::
GetDescription(OUT MessageId* pDescription)
{
    HRESULT hr = S_OK;

    // Check parameters make sure the pointer is not NULL
    // The caller should pass a valid pointer to description
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert(pDescription != NULL);

    // Put here the exact UINT32 MessageId for the friendly name
    *pDescription = SDK_SAMPLE_SHA_DESCRIPTION_MSG_ID;


    return hr;
}





STDMETHODIMP CQuarSystemHealthAgentInfo::
GetVendorName (OUT MessageId* pCompanyName)
{
    HRESULT hr = S_OK;

    // Check parameters make sure the pointer is not NULL
    // The caller should pass a valid pointer to pCompanyName
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert(pCompanyName != NULL);

    // Put here the UINT32 Message ID for the company name for this SHA
    *pCompanyName = SDK_SAMPLE_COMPANY_NAME_MSG_ID;


    return hr;
}




STDMETHODIMP CQuarSystemHealthAgentInfo::
GetVersion(OUT MessageId* pVersionInfo)
{
    HRESULT hr = S_OK;

    // Check parameters make sure the pointer is not NULL
    // The caller should pass a valid pointer to pVersionInfo
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert(pVersionInfo != NULL);

    // Put here the UINT32 Message ID for the version info
    *pVersionInfo = SDK_SAMPLE_VERSION_INFO_MSG_ID;


    return hr;
}




STDMETHODIMP CQuarSystemHealthAgentInfo::
GetIcon(
    OUT CountedString** ppDllFilePath,
    OUT UINT* pIconResourceId)
{
    HRESULT hr = S_OK;

    // The caller should pass valid pointers to ppDllFilePath* and pIconResourceId
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert((ppDllFilePath != NULL) && (pIconResourceId != NULL));

    hr = AllocateMemory(*ppDllFilePath, MAX_PATH_WCHAR_SIZE);
//	hr = AllocCountedString(ppDllFilePath, (const WCHAR *)SHA_SDK_SAMPLE_DLL_FILE_NAME);
    if (FAILED(hr))
    {
        goto Cleanup;
    }

    DWORD fileNameLength = 0;
    fileNameLength = GetModuleFileNameW(
                                        SHA_SDK_SAMPLE_DLL_FILE_HANDLE,
                                        (*ppDllFilePath)->string,
                                        (MAX_PATH+1));

    if (fileNameLength == (MAX_PATH+1))
    {
        hr = E_OUTOFMEMORY;
        DebugPrintfW(L" ---          buffer is too small to hold the module name (error = 0x%08x)",
            hr);
        // if failed goto cleanup to free the allocated memory
        goto Cleanup;
    }
    else if (fileNameLength == 0)
    {
        // Failed to get the module name
        hr =HRESULT_FROM_WIN32(GetLastError());
        DebugPrintfW(L" ---          GetModuleFileNameW failed to get the module name (error = 0x%08x)",
            hr);
        goto Cleanup;
    }

    (*ppDllFilePath)->length = (UINT16) fileNameLength;

    *pIconResourceId = IDI_ICON1; // Defined in Resource.h


    Cleanup:
        if (FAILED(hr) && (ppDllFilePath))
        {
            FreeMemory(*ppDllFilePath);
        }
        return hr;
}




STDMETHODIMP CQuarSystemHealthAgentInfo::
ConvertErrorCodeToMessageId(
    IN HRESULT errorCode,
    OUT MessageId* pMsgId)
{
    HRESULT hr = S_OK;

    // Check parameters make sure the pointer is not NULL
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert(pMsgId != NULL);


    // SDKNote:
    // The following cases are examples, add more error codes here
    switch (errorCode)
    {
        case QUAR_E_NOTPATCHED:
            *pMsgId = SDK_SAMPLE_CLIENT_NOT_PATCHED_MSG_ID;
            break;
        case S_OK:
            *pMsgId = SDK_SAMPLE_COMPLIANT_CLIENT_MSG_ID;
            break;
        default:
            // return an error
            hr = E_UNEXPECTED;
            DebugPrintfW(L" ---          Unknown error code (error = 0x%08x)",
            hr);
            break;

    }


    return hr;
}




STDMETHODIMP CQuarSystemHealthAgentInfo::
GetLocalizedString(
    IN MessageId msgId,
    OUT CountedString** ppString)
{
    HRESULT hr = S_OK;

    // The caller should pass valid pointers to ppString* and pRetrievedLangId
    // MIDL protects this on Retail builds
    // whereas the assert() will fire on Debug builds
    assert(ppString != NULL);

    // Allocate memory for string
    hr = AllocateMemory(*ppString, MAX_PATH_WCHAR_SIZE);
//	hr = AllocCountedString(ppString, (const WCHAR *)SHA_SDK_SAMPLE_DLL_FILE_NAME);
    if (FAILED(hr))
    {
        goto Cleanup;
    }

    // Read from the resource file
    DWORD stringLength = 0;
    stringLength = FormatMessageW(FORMAT_MESSAGE_FROM_HMODULE,
                            SHA_SDK_SAMPLE_DLL_FILE_HANDLE,
                            msgId,
                            US_ENGLISH,
                            (*ppString)->string,
                            (MAX_PATH+1) ,
                            NULL);

    // if failed to read from the mc file check if the Language not found
    // return the English string else return the error
    if (stringLength == 0)   // failed to get from the mc file
    {
        DWORD error = GetLastError();
        DebugPrintfW(L" ---          Error getting FormatMessageW (error = 0x%08x)", hr);
        hr =HRESULT_FROM_WIN32(error);
        goto Cleanup;
    }

    (*ppString)->length = (UINT16) stringLength;


Cleanup:
    if (FAILED(hr) &&(ppString))
    {
        FreeMemory(*ppString);
    }
    return hr;
}




