// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

class CPictureEvents :
    public CBaseEvents
{
private:
    CPictureContent    *m_pContent;

public:
    CPictureEvents(CPictureContent* pContent);
    virtual ~CPictureEvents();

    //
    // ISideShowEvents methods
    //
    virtual HRESULT STDMETHODCALLTYPE ContentMissing(
        const CONTENT_ID contentId,
        ISideShowContent** ppIContent);

    virtual HRESULT STDMETHODCALLTYPE ApplicationEvent(
        ISideShowCapabilities* pICapabilities,
        const DWORD dwEventId,
        const DWORD dwEventSize,
        const BYTE* pbEventData);

    virtual HRESULT STDMETHODCALLTYPE DeviceAdded(
        ISideShowCapabilities* pIDevice);

    virtual HRESULT STDMETHODCALLTYPE DeviceRemoved(
        ISideShowCapabilities* pIDevice);
};
