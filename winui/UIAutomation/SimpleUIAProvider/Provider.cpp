/*************************************************************************************************
 *
 * File: Provider.cpp
 *
 * Description: Implementation of the Provider class, which implements IRawElementProviderSimple 
 * and IInvokeProvider for a simple custom control.
 * 
 * See Window.cpp for a full description of this sample.
 *   
 * 
 *  Copyright (C) Microsoft Corporation.  All rights reserved.
 * 
 * This source code is intended only as a supplement to Microsoft
 * Development Tools and/or on-line documentation.  See these other
 * materials for detailed information regarding Microsoft code samples.
 * 
 * THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 * 
 *************************************************************************************************/

#define INITGUID
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <ole2.h>
#include <UIAutomationCore.h>
#include <UIAutomationCoreAPI.h>

#include "Provider.h"


// Various identifiers that have to be looked up.
struct UiaIdentifiers
{
    PROPERTYID NameProperty;
    PROPERTYID ControlTypeProperty;
    PATTERNID  InvokePattern;
    CONTROLTYPEID ButtonControlType;
} UiaIds;

// Look up identifiers.
void Provider::InitIds()
{
    static bool inited = false;
    if (!inited)
    {
        inited = true;
        UiaIds.NameProperty = UiaLookupId(AutomationIdentifierType_Property, &Name_Property_GUID);
        UiaIds.ControlTypeProperty = UiaLookupId(AutomationIdentifierType_Property, &ControlType_Property_GUID);
        UiaIds.InvokePattern = UiaLookupId(AutomationIdentifierType_Pattern, &Invoke_Pattern_GUID);
        UiaIds.ButtonControlType = UiaLookupId(AutomationIdentifierType_ControlType, &Button_Control_GUID);
    }
}

// Constructor.
Provider::Provider(HWND hwnd): m_refCount(1)
{
    controlHWnd = hwnd;
    InitIds();
}

// Destructor.
Provider::~Provider()
{
    // Nothing to do.
}

// IUnknown implementation.

ULONG STDMETHODCALLTYPE Provider::AddRef()
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE Provider::Release()
{
    if (--m_refCount <= 0)
    {
        delete this;
        return 0;
    }
    return m_refCount;
}

HRESULT STDMETHODCALLTYPE Provider::QueryInterface(REFIID riid, void** ppInterface)
{
    if (riid == __uuidof(IUnknown))
    {
        *ppInterface =(IUnknown*)((IRawElementProviderSimple*)this);
    }
    else if (riid == __uuidof(IRawElementProviderSimple))
    {
        *ppInterface =(IRawElementProviderSimple*)this;
    }
    else if (riid == __uuidof(IInvokeProvider))
    {
        *ppInterface =(IInvokeProvider*)this;
    }
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }

    ((IUnknown*)(*ppInterface))->AddRef();
    return S_OK;
}


// IRawElementProviderSimple implementation

// Get provider options.

HRESULT STDMETHODCALLTYPE Provider::get_ProviderOptions( ProviderOptions* pRetVal )
{
    *pRetVal = ProviderOptions_ServerSideProvider;
    return S_OK;
}

// Get the object that supports IInvokePattern.

HRESULT STDMETHODCALLTYPE Provider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
    if (patternId == UiaIds.InvokePattern)
    {
        AddRef();
        *pRetVal =(IUnknown *)(IRawElementProviderSimple*)this;
    }
    else
    {
        *pRetVal = NULL;   
    }
    return S_OK;
}

// Gets custom properties.

HRESULT STDMETHODCALLTYPE Provider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
    if (propertyId == UiaIds.ControlTypeProperty)
    {
        pRetVal->vt = VT_I4;
        pRetVal->lVal = UiaIds.ButtonControlType;
    }

    // The Name property comes from the Caption property of the control window, if it has one.
    // The Name is overridden here for the sake of illustration. 
    else if (propertyId == UiaIds.NameProperty)
    {
        pRetVal->vt = VT_BSTR;
        pRetVal->bstrVal = SysAllocString(L"ColorButton");
    }
    else
    {
        pRetVal->vt = VT_EMPTY;
       // UI Automation will attempt to get the property from the host window provider.
    }
    return S_OK;
}

// Gets the UI Automation provider for the host window. This provider supplies most properties.

HRESULT STDMETHODCALLTYPE Provider::get_HostRawElementProvider(IRawElementProviderSimple** pRetVal)
{
    return UiaHostProviderFromHwnd(controlHWnd, pRetVal); 
}


// IInvokeProvider implementation.

HRESULT STDMETHODCALLTYPE Provider::Invoke()
{
    PostMessage(controlHWnd,  WM_LBUTTONDOWN, NULL, NULL);
    return S_OK;
}


