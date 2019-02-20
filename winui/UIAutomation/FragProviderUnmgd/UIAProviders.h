/*************************************************************************************************
*
* File: UIAProviders.h
*
* Description: Declarations for the sample UI Autoamtion provider implementations.
* 
* See EntryPoint.cpp for a full description of this sample.
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
#pragma once

#include <UIAutomationCore.h>
#include <UIAutomationCoreAPI.h>
#include <assert.h>
#include "CustomControl.h"

class ListItemProvider;

// Various identifiers that have to be looked up.
typedef struct UiaIdentifiers
{
    PROPERTYID    LocalizedControlTypeProperty;
    PROPERTYID    AutomationIdProperty;
    PROPERTYID    NameProperty;
    PROPERTYID    ControlTypeProperty;
    PROPERTYID    HasKeyboardFocusProperty;
    PROPERTYID    IsControlElementProperty;
    PROPERTYID    IsContentElementProperty;
    PROPERTYID    IsKeyboardFocusableProperty;
    PROPERTYID    ItemStatusProperty;
    PATTERNID     SelectionPattern;
    PATTERNID     SelectionItemPattern;
    CONTROLTYPEID ListControlType;
    CONTROLTYPEID ListItemControlType;
    EVENTID       ElementSelectedEvent;
} UiaIds;

class ListProvider : public IRawElementProviderSimple, 
    public IRawElementProviderFragment, 
    public IRawElementProviderFragmentRoot, 
    public IRawElementProviderAdviseEvents,
    public ISelectionProvider
{
public:

    // IUnknown methods
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void**ppInterface);

    // IRawElementProviderSimple methods
    HRESULT STDMETHODCALLTYPE get_ProviderOptions(ProviderOptions * pRetVal);
    HRESULT STDMETHODCALLTYPE GetPatternProvider(PATTERNID iid,IUnknown * * pRetVal );
    HRESULT STDMETHODCALLTYPE GetPropertyValue(PROPERTYID idProp,VARIANT * pRetVal );
    HRESULT STDMETHODCALLTYPE get_HostRawElementProvider(IRawElementProviderSimple ** pRetVal );

    // IRawElementProviderFragment methods
    HRESULT STDMETHODCALLTYPE Navigate(NavigateDirection direction, IRawElementProviderFragment ** pRetVal );
    HRESULT STDMETHODCALLTYPE GetRuntimeId(SAFEARRAY ** pRetVal );
    HRESULT STDMETHODCALLTYPE get_BoundingRectangle(UiaRect * pRetVal );
    HRESULT STDMETHODCALLTYPE GetEmbeddedFragmentRoots(SAFEARRAY ** pRetVal );
    HRESULT STDMETHODCALLTYPE SetFocus();
    HRESULT STDMETHODCALLTYPE get_FragmentRoot( IRawElementProviderFragmentRoot * * pRetVal );

    // IRawElementProviderFragmenRoot methods
    HRESULT STDMETHODCALLTYPE ElementProviderFromPoint(double x, double y, IRawElementProviderFragment ** pRetVal );
    HRESULT STDMETHODCALLTYPE GetFocus(IRawElementProviderFragment ** pRetVal );

    // IRawElementProviderAdviseEvents methods
    HRESULT STDMETHODCALLTYPE AdviseEventAdded(EVENTID eventId, SAFEARRAY * propertyIDs);
    HRESULT STDMETHODCALLTYPE AdviseEventRemoved(EVENTID eventId, SAFEARRAY * propertyIDs);

    // ISelectionProvider methods
    HRESULT STDMETHODCALLTYPE GetSelection(SAFEARRAY * *pRetVal);
    HRESULT STDMETHODCALLTYPE get_CanSelectMultiple(BOOL *pRetVal);
    HRESULT STDMETHODCALLTYPE get_IsSelectionRequired(BOOL *pRetVal);

    // Ref counter for this COM object.
    ULONG m_refCount;

    // Count of clients listening for element-selected events.
    int m_elementSelectedEventListenerCount;

    // Parent control.
    HWND m_controlHwnd;
    CustomListControl* m_pControl;

    // Constructor/destructor.
    ListProvider(CustomListControl* pControl);
    virtual ~ListProvider();

    // Various methods.
    void InitIds();
    ListItemProvider* GetItemProviderByIndex(int index);
};

class ListItemProvider : public IRawElementProviderSimple, 
    public IRawElementProviderFragment, 
    public ISelectionItemProvider
{
public:

    // IUnknown methods
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void**ppInterface);

    // IRawElementProviderSimple methods
    HRESULT STDMETHODCALLTYPE get_ProviderOptions(ProviderOptions * pRetVal);
    HRESULT STDMETHODCALLTYPE GetPatternProvider(PATTERNID iid,IUnknown * * pRetVal );
    HRESULT STDMETHODCALLTYPE GetPropertyValue(PROPERTYID idProp,VARIANT * pRetVal );
    HRESULT STDMETHODCALLTYPE get_HostRawElementProvider(IRawElementProviderSimple ** pRetVal );

    // IRawElementProviderFragment methods
    HRESULT STDMETHODCALLTYPE Navigate(NavigateDirection direction, IRawElementProviderFragment ** pRetVal );
    HRESULT STDMETHODCALLTYPE GetRuntimeId(SAFEARRAY ** pRetVal );
    HRESULT STDMETHODCALLTYPE get_BoundingRectangle(UiaRect * pRetVal );
    HRESULT STDMETHODCALLTYPE GetEmbeddedFragmentRoots(SAFEARRAY ** pRetVal );
    HRESULT STDMETHODCALLTYPE SetFocus();
    HRESULT STDMETHODCALLTYPE get_FragmentRoot( IRawElementProviderFragmentRoot * * pRetVal );

    // ISelectionItemProvider methods
    HRESULT STDMETHODCALLTYPE Select();
    HRESULT STDMETHODCALLTYPE AddToSelection();
    HRESULT STDMETHODCALLTYPE RemoveFromSelection();
    HRESULT STDMETHODCALLTYPE get_IsSelected(BOOL *pRetVal);
    HRESULT STDMETHODCALLTYPE get_SelectionContainer(IRawElementProviderSimple **pRetVal);

    // Ref Counter for this COM object
    ULONG m_refCount;

    // Pointers to the owning item control and list control.
    CustomListItemControl* m_pListItemControl;
    CustomListControl* m_pListControl;

    // Constructor / destructor
    ListItemProvider(CustomListItemControl* pControl); 
    virtual ~ListItemProvider();

    // Various methods
    void NotifyItemAdded();
    void NotifyItemRemoved();
    void NotifyElementSelected();

};
