/*************************************************************************************************
 *
 * File: Provider.h
 *
 * Description: Declaration of the Provider class.
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


class Provider : public IRawElementProviderSimple, 
        public IInvokeProvider
{
public:
    Provider(HWND hwnd);
    virtual ~Provider();

    void InitIds();

    // IUnknown methods
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void**);
    
    // IRawElementProviderSimple methods
    HRESULT STDMETHODCALLTYPE get_ProviderOptions(ProviderOptions * pRetVal);
    HRESULT STDMETHODCALLTYPE GetPatternProvider(PATTERNID patternId, IUnknown ** pRetVal);
    HRESULT STDMETHODCALLTYPE GetPropertyValue(PROPERTYID propertyId, VARIANT * pRetVal);
    HRESULT STDMETHODCALLTYPE get_HostRawElementProvider(IRawElementProviderSimple ** pRetVal);

    // IInvokeProvider methods
    HRESULT STDMETHODCALLTYPE Invoke();

    // Ref Counter for this COM object
    ULONG m_refCount;

    HWND controlHWnd; // The HWND for the control.
};
