// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "StdAfx.h"
#include "PictureContent.h"
#include "PictureEvents.h"

CPictureEvents::CPictureEvents(CPictureContent* pContent)
{
    m_pContent = pContent;
}

CPictureEvents::~CPictureEvents()
{
}

//
// ISideShowEvents methods
//
HRESULT CPictureEvents::ContentMissing(
        const CONTENT_ID contentId,
        ISideShowContent** ppIContent)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pContent)
    {
        ISideShowContent* pIContent = NULL;

        //
        // Get the content for this content ID.
        //
        pIContent = m_pContent->GetContent(contentId);
        if (NULL != pIContent)
        {
            //
            // QueryInterface to return an AddRef'ed pointer to the platform.
            // The platform will call Release automatically.
            //
            hr = pIContent->QueryInterface(IID_ISideShowContent, (void**)ppIContent);
        }
    }
    return hr;
}

HRESULT CPictureEvents::ApplicationEvent(
        ISideShowCapabilities* pICapabilities,
        const DWORD dwEventId,
        const DWORD dwEventSize,
        const BYTE* pbEventData)
{
    return S_OK;
}

HRESULT CPictureEvents::DeviceAdded(
        ISideShowCapabilities* pIDevice)
{
    return S_OK;
}

HRESULT CPictureEvents::DeviceRemoved(
        ISideShowCapabilities* pIDevice)
{
    return S_OK;
}
