// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include <strsafe.h>

// This number controls how many object identifiers are requested during each call
// to IEnumPortableDeviceObjectIDs::Next()
#define NUM_OBJECTS_TO_REQUEST  10

// Recursively called function which enumerates using the specified
// object identifier as the parent.
void RecursiveEnumerate(
    LPCWSTR                 wszObjectID,
    IPortableDeviceContent* pContent)
{
    HRESULT                               hr = S_OK;
    CComPtr<IEnumPortableDeviceObjectIDs> pEnumObjectIDs;

    if ((wszObjectID == NULL) ||
        (pContent    == NULL))
    {
        return;
    }

    // Print the object identifier being used as the parent during enumeration.
    printf("%ws\n",wszObjectID);

    // Get an IEnumPortableDeviceObjectIDs interface by calling EnumObjects with the
    // specified parent object identifier.
    hr = pContent->EnumObjects(0,               // Flags are unused
                               wszObjectID,     // Starting from the passed in object
                               NULL,            // Filter is unused
                               &pEnumObjectIDs);
    if (FAILED(hr))
    {
        printf("! Failed to get IEnumPortableDeviceObjectIDs from IPortableDeviceContent, hr = 0x%lx\n",hr);
    }

    // Loop calling Next() while S_OK is being returned.
    while(hr == S_OK)
    {
        DWORD  cFetched = 0;
        LPWSTR szObjectIDArray[NUM_OBJECTS_TO_REQUEST] = {0};
        hr = pEnumObjectIDs->Next(NUM_OBJECTS_TO_REQUEST,   // Number of objects to request on each NEXT call
                                  szObjectIDArray,          // Array of LPWSTR array which will be populated on each NEXT call
                                  &cFetched);               // Number of objects written to the LPWSTR array
        if (SUCCEEDED(hr))
        {
            // Traverse the results of the Next() operation and recursively enumerate
            // Remember to free all returned object identifiers using CoTaskMemFree()
            for (DWORD dwIndex = 0; dwIndex < cFetched; dwIndex++)
            {
                RecursiveEnumerate(szObjectIDArray[dwIndex],pContent);

                // Free allocated LPWSTRs after the recursive enumeration call has completed.
                CoTaskMemFree(szObjectIDArray[dwIndex]);
                szObjectIDArray[dwIndex] = NULL;
            }
        }
    }
}

// Enumerate all content on the device starting with the
// "DEVICE" object
void EnumerateAllContent(
    IPortableDevice* pDevice)
{
    HRESULT                         hr = S_OK;
    CComPtr<IPortableDeviceContent> pContent;

    if (pDevice == NULL)
    {
        printf("! A NULL IPortableDevice interface pointer was received\n");
        return;
    }

    // Get an IPortableDeviceContent interface from the IPortableDevice interface to
    // access the content-specific methods.
    hr = pDevice->Content(&pContent);
    if (FAILED(hr))
    {
        printf("! Failed to get IPortableDeviceContent from IPortableDevice, hr = 0x%lx\n",hr);
    }

    // Enumerate content starting from the "DEVICE" object.
    if (SUCCEEDED(hr))
    {
        printf("\n");
        RecursiveEnumerate(WPD_DEVICE_OBJECT_ID, pContent);
    }
}

// Recursively called function which enumerates using the specified
// object identifier as the parent and populates the returned object
// identifiers into an IPortableDevicePropVariantCollection object.
void RecursiveEnumerateAndCopyToCollection(
    LPCWSTR                               wszObjectID,
    IPortableDeviceContent*               pContent,
    IPortableDevicePropVariantCollection* pObjectIDs)
{
    HRESULT                               hr = S_OK;
    CComPtr<IEnumPortableDeviceObjectIDs> pEnumObjectIDs;

    if ((wszObjectID == NULL) ||
        (pContent    == NULL) ||
        (pObjectIDs  == NULL))
    {
        return;
    }

    // Add the object identifier being used as the parent during enumeration
    // to the collection.
    PROPVARIANT pv = {0};
    PropVariantInit(&pv);

    // Allocated a new string in a PROPVARIANT so we can add it to our
    // collection object.
    pv.vt      = VT_LPWSTR;
    pv.pwszVal = AtlAllocTaskWideString(wszObjectID);

    // Add the object identifer...
    hr = pObjectIDs->Add(&pv);

    // Free the allocated string in the PROPVARIANT
    PropVariantClear(&pv);

    // If we failed to add the object identifier, return immediately.
    if (FAILED(hr))
    {
        printf("! Failed to add object identifier '%ws' to the IPortableDevicePropVariantCollection, hr = 0x%lx\n",wszObjectID, hr);
        return;
    }

    // Get an IEnumPortableDeviceObjectIDs interface by calling EnumObjects with the
    // specified parent object identifier.
    hr = pContent->EnumObjects(0,               // Flags are unused
                               wszObjectID,     // Starting from the passed in object
                               NULL,            // Filter is unused
                               &pEnumObjectIDs);
    if (FAILED(hr))
    {
        printf("! Failed to get IEnumPortableDeviceObjectIDs from IPortableDeviceContent, hr = 0x%lx\n",hr);
    }

    // Loop calling Next() while S_OK is being returned.
    while(hr == S_OK)
    {
        DWORD  cFetched = 0;
        LPWSTR szObjectIDArray[NUM_OBJECTS_TO_REQUEST] = {0};
        hr = pEnumObjectIDs->Next(NUM_OBJECTS_TO_REQUEST,   // Number of objects to request on each NEXT call
                                  szObjectIDArray,          // Array of LPWSTR array which will be populated on each NEXT call
                                  &cFetched);               // Number of objects written to the LPWSTR array
        if (SUCCEEDED(hr))
        {
            // Traverse the results of the Next() operation and recursively enumerate
            // Remember to free all returned object identifiers using CoTaskMemFree()
            for (DWORD dwIndex = 0; dwIndex < cFetched; dwIndex++)
            {
                RecursiveEnumerateAndCopyToCollection(szObjectIDArray[dwIndex], pContent, pObjectIDs);

                // Free allocated LPWSTRs after the recursive enumeration call has completed.
                CoTaskMemFree(szObjectIDArray[dwIndex]);
                szObjectIDArray[dwIndex] = NULL;
            }
        }
    }
}

// Enumerate all content on the device starting with the
// "DEVICE" object and populates the returned object identifiers
// into an IPortableDevicePropVariantCollection
HRESULT CreateIPortableDevicePropVariantCollectionWithAllObjectIDs(
    IPortableDevice*        pDevice,
    IPortableDeviceContent* pContent,
    IPortableDevicePropVariantCollection** ppObjectIDs)
{
    HRESULT hr = S_OK;
    if ((pDevice     == NULL) ||
        (pContent    == NULL) ||
        (ppObjectIDs == NULL))
    {
        hr = E_POINTER;
        return hr;
    }

    CComPtr<IPortableDevicePropVariantCollection> pObjectIDs;

    // CoCreate an IPortableDevicePropVariantCollection interface to hold the the object identifiers
    hr = CoCreateInstance(CLSID_PortableDevicePropVariantCollection,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IPortableDevicePropVariantCollection,
                          (VOID**) &pObjectIDs);
    if (SUCCEEDED(hr))
    {
        if (pObjectIDs != NULL)
        {
            RecursiveEnumerateAndCopyToCollection(WPD_DEVICE_OBJECT_ID, pContent, pObjectIDs);
        }
    }

    hr = pObjectIDs->QueryInterface(IID_PPV_ARGS(ppObjectIDs));
    if (FAILED(hr))
    {
        printf("! Failed to QueryInterface for IPortableDevicePropVariantCollection, hr = 0x%lx\n",hr);
    }

    return hr;
}


