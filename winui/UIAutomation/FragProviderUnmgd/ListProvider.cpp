
/*************************************************************************************************
 *
 * File: ListProvider.cpp
 *
 * Description: Implementation of the ListProvider class, which implements which implements a 
 * UI Automation provider for a custom list control.
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

#define INITGUID
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <ole2.h>
#include "UIAProviders.h"
#include "CustomControl.h"

HFONT GetFont(LONG ht);
UiaIds AutoIds;

// Constructor.
//
ListProvider::ListProvider(CustomListControl* pControl): m_refCount(1)
{
    m_pControl = pControl;
    m_controlHwnd = pControl->GetHwnd();
    m_elementSelectedEventListenerCount = 0;

    // Initialize identifiers from GUIDs.
    InitIds();
}

// Destructor.
//
ListProvider::~ListProvider()
{
}


// Retrieves the UI Automation provider for a list item.
//
ListItemProvider* ListProvider::GetItemProviderByIndex(int index)
{
    if ((index < 0) || (index >= m_pControl->GetCount()))
    {
        return NULL;
    }
    CustomListItemControl* pItem = m_pControl->GetItemAt(index);
    if (pItem == NULL)
    {
        return NULL;
    }
    return pItem->GetListItemProvider();  
}

// Looks up identifiers. To use UiaLookupId, you must link to UIAutomationcore.lib.
//
void ListProvider::InitIds()
{
    static bool inited = false;
    if (!inited)
    {
        inited = true;
        AutoIds.LocalizedControlTypeProperty = UiaLookupId(AutomationIdentifierType_Property, &LocalizedControlType_Property_GUID);
        AutoIds.AutomationIdProperty = UiaLookupId(AutomationIdentifierType_Property, &AutomationId_Property_GUID);
        AutoIds.NameProperty = UiaLookupId(AutomationIdentifierType_Property, &Name_Property_GUID);
        AutoIds.HasKeyboardFocusProperty = UiaLookupId(AutomationIdentifierType_Property, &HasKeyboardFocus_Property_GUID);
        AutoIds.IsControlElementProperty = UiaLookupId(AutomationIdentifierType_Property, &IsControlElement_Property_GUID);
        AutoIds.IsContentElementProperty = UiaLookupId(AutomationIdentifierType_Property, &IsContentElement_Property_GUID);
        AutoIds.IsKeyboardFocusableProperty = UiaLookupId(AutomationIdentifierType_Property, &IsKeyboardFocusable_Property_GUID);
        AutoIds.ItemStatusProperty = UiaLookupId(AutomationIdentifierType_Property, &ItemStatus_Property_GUID);

        AutoIds.ControlTypeProperty = UiaLookupId(AutomationIdentifierType_Property, &ControlType_Property_GUID);
        AutoIds.SelectionPattern = UiaLookupId(AutomationIdentifierType_Pattern, &Selection_Pattern_GUID);
        AutoIds.SelectionItemPattern = UiaLookupId(AutomationIdentifierType_Pattern, &SelectionItem_Pattern_GUID);
        AutoIds.ListControlType = UiaLookupId(AutomationIdentifierType_ControlType, &List_Control_GUID);
        AutoIds.ListItemControlType = UiaLookupId(AutomationIdentifierType_ControlType, &ListItem_Control_GUID);
        AutoIds.ElementSelectedEvent = UiaLookupId(AutomationIdentifierType_Event, &SelectionItem_ElementSelectedEvent_Event_GUID);
    }
}

// Raises an event when a list item is selected.
//

// IUnknown implementation.
//
ULONG STDMETHODCALLTYPE ListProvider::AddRef()
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE ListProvider::Release()
{
    if (--m_refCount <= 0)
    {
        delete this;
        return 0;             
    }
    return m_refCount;
}

HRESULT STDMETHODCALLTYPE ListProvider::QueryInterface(REFIID riid, void** ppInterface)
{
    if(riid == __uuidof(IUnknown))                              *ppInterface =(IUnknown*)((IRawElementProviderSimple*)this);
    else if(riid == __uuidof(IRawElementProviderSimple))        *ppInterface =(IRawElementProviderSimple*)this;
    else if(riid == __uuidof(IRawElementProviderFragment))      *ppInterface =(IRawElementProviderFragment*)this;
    else if(riid == __uuidof(IRawElementProviderFragmentRoot))  *ppInterface =(IRawElementProviderFragmentRoot*)this;
    else if(riid == __uuidof(IRawElementProviderAdviseEvents))  *ppInterface =(IRawElementProviderAdviseEvents*)this;
    else if(riid == __uuidof(ISelectionProvider))               *ppInterface =(ISelectionProvider*)this;
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    ((IUnknown*)(*ppInterface))->AddRef();
    return S_OK;
}


// IRawElementProviderSimple implementation
//
// Implementation of IRawElementProviderSimple::get_ProviderOptions.
// Gets UI Automation provider options.
//
HRESULT STDMETHODCALLTYPE ListProvider::get_ProviderOptions(ProviderOptions* pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    *pRetVal = ProviderOptions_ServerSideProvider;
    return S_OK;
}

// Implementation of IRawElementProviderSimple::get_PatternProvider.
// Gets the object that supports ISelectionPattern.
//
HRESULT STDMETHODCALLTYPE ListProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    *pRetVal = NULL;
    if (patternId == AutoIds.SelectionPattern)
    {
        *pRetVal =(IUnknown*)(ISelectionProvider*)this;   
        AddRef();  
    }
    else
    {
        *pRetVal = NULL;   
    }
    return S_OK;
}

// Implementation of IRawElementProviderSimple::get_PropertyValue.
// Gets custom properties.
//
HRESULT STDMETHODCALLTYPE ListProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    if (propertyId == AutoIds.LocalizedControlTypeProperty)
    {
        pRetVal->vt = VT_BSTR;
        pRetVal->bstrVal = SysAllocString(L"Contact List");
    }
    else if (propertyId == AutoIds.ControlTypeProperty)
    {
        pRetVal->vt = VT_I4;
        pRetVal->lVal = AutoIds.ListControlType;
    }
    else if (propertyId == AutoIds.IsKeyboardFocusableProperty)
    {
        pRetVal->vt = VT_BOOL;
        pRetVal->boolVal = VARIANT_TRUE;
    }
    // else pRetVal is empty, and UI Automation will attempt to get the property from
    //  the HostRawElementProvider, which is the default provider for the HWND.
	// Note that the Name property comes from the Caption property of the control window, 
	//  if it has one.
    else
    {
        pRetVal->vt = VT_EMPTY;
    }
    return S_OK;
}

// Implementation of IRawElementProviderSimple::get_HostRawElementProvider.
// Gets the default UI Automation provider for the host window. This provider 
// supplies many properties.
//
HRESULT STDMETHODCALLTYPE ListProvider::get_HostRawElementProvider(IRawElementProviderSimple** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    if (m_controlHwnd == NULL)
    {
        return UIA_E_ELEMENTNOTAVAILABLE;
    }
    HRESULT hr = UiaHostProviderFromHwnd(m_controlHwnd, pRetVal); 
    return hr;
}


// IRawElementProviderFragment implementation
//
// Implementation of IRawElementProviderFragment::Navigate.
// Enables UI Automation to locate the element in the tree.
// Navigation to the parent is handled by the host window provider.
//
HRESULT STDMETHODCALLTYPE ListProvider::Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    CustomListControl* pListControl = this->m_pControl;
    CustomListItemControl* destination;
    IRawElementProviderFragment* pFrag = NULL;
    switch(direction)
    {
      case NavigateDirection_FirstChild:  
          destination = pListControl->GetItemAt(0);
          pFrag = (IRawElementProviderFragment*)destination->GetListItemProvider();
          break;
      case NavigateDirection_LastChild:  
          destination = pListControl->GetItemAt(pListControl->GetCount()-1);
          pFrag = (IRawElementProviderFragment*)destination->GetListItemProvider();
          break;
    }
    if (pFrag != NULL) pFrag->AddRef();   
    *pRetVal = pFrag;
    return S_OK;
}

// Implementation of IRawElementProviderFragment::GetRuntimeId.
// UI Automation gets this value from the host window provider, so supply NULL here.
//
HRESULT STDMETHODCALLTYPE ListProvider::GetRuntimeId(SAFEARRAY** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    *pRetVal = NULL;
    return S_OK;
}

// Implementation of IRawElementProviderFragment::get_BoundingRectangle.
//
// Retrieves the screen location and size of the control. Controls hosted in
// Win32 windows can return an empty rectangle; UI Automation will
// retrieve the rectangle from the HWND provider. However, the method is
// implemented here so that it can be used by the list items to calculate
// their own bounding rectangles.
//
// UI Spy uses the bounding rectangle to draw a red border around the element.
//
HRESULT STDMETHODCALLTYPE ListProvider::get_BoundingRectangle(UiaRect* pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    RECT rect;
    GetClientRect(m_controlHwnd, &rect);
    InflateRect(&rect, -2, -2);
    POINT upperLeft;
    upperLeft.x = rect.left;  
    upperLeft.y = rect.top;
    ClientToScreen(m_controlHwnd, &upperLeft);

    pRetVal->left = upperLeft.x;
    pRetVal->top = upperLeft.y;
    pRetVal->width = rect.right - rect.left;
    pRetVal->height = rect.bottom - rect.top;
    return S_OK;
}

// Implementation of IRawElementProviderFragment::GetEmbeddedFragmentRoots.
// Retrieves other fragment roots that may be hosted in this one.
//
HRESULT STDMETHODCALLTYPE ListProvider::GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    *pRetVal = NULL;
    return S_OK;
}

// Implementation of IRawElementProviderFragment::SetFocus.
// Responds to the control receiving focus through a UI Automation request.
// For HWND-based controls, this is handled by the host window provider.
//
HRESULT STDMETHODCALLTYPE ListProvider::SetFocus()
{
    return S_OK;
}

// Implementation of IRawElementProviderFragment::get_FragmentRoot.
// Retrieves the root element of this fragment.
//
HRESULT STDMETHODCALLTYPE ListProvider::get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    *pRetVal = (IRawElementProviderFragmentRoot*)this;
    AddRef();  
    return S_OK;
}

// IRawElementProviderFragmentRoot implementation
//
// Implementation of IRawElementProviderFragmentRoot::ElementProviderFromPoint.
// Retrieves the IRawElementProviderFragment interface for the item at the specified 
// point (in client coordinates).
// UI Spy uses this to determine what element is under the cursor when Ctrl is pressed.
//
HRESULT STDMETHODCALLTYPE ListProvider::ElementProviderFromPoint(double x, double y, IRawElementProviderFragment** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    POINT pt;
    pt.x = (LONG)x;
    pt.y = (LONG)y;
    ScreenToClient(m_controlHwnd, &pt);
    int itemIndex = m_pControl->IndexFromY(m_controlHwnd, pt.y);
    ListItemProvider* pItem = GetItemProviderByIndex(itemIndex);  
    if (pItem != NULL)
    {
        *pRetVal = (IRawElementProviderFragment*)pItem;
        pItem->AddRef();
    }
    else *pRetVal = NULL;
    return S_OK;
}

// Implementation of IRawElementProviderFragmentRoot::GetFocus.
// Retrieves the provider for the list item that is selected when the control gets focus.
//
HRESULT STDMETHODCALLTYPE ListProvider::GetFocus(IRawElementProviderFragment** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    ListItemProvider* pItem = GetItemProviderByIndex(m_pControl->GetSelectedIndex()); 
    pItem->AddRef();
    *pRetVal = (IRawElementProviderFragment*)pItem;
    return S_OK;
}


// IRawElementProviderAdviseEvents methods

// Implementation of IRawElementProviderAdviseEvents::AdviseEventAdded.
// This and the following method can be used to track what events are being listened for,
// so that efficiency can be improved by raising only those events.
//
HRESULT STDMETHODCALLTYPE ListProvider::AdviseEventAdded(EVENTID eventId, SAFEARRAY* propertyIDs)
{
    if (eventId == AutoIds.ElementSelectedEvent)
    {
        m_elementSelectedEventListenerCount++;
    }
    return S_OK;
}

// Implementation of IRawElementProviderAdviseEvents::AdviseEventRemoved
//
HRESULT STDMETHODCALLTYPE ListProvider::AdviseEventRemoved(EVENTID eventId, SAFEARRAY* propertyIDs)
{
    if (eventId == AutoIds.ElementSelectedEvent)
    {
       if (m_elementSelectedEventListenerCount > 0) m_elementSelectedEventListenerCount--;
    }
    return S_OK;
}

// ISelectionProvider implementation
//
// Implementation of ISelectionProvider::GetSelection.
// Gets the provider(s) for the items(s) selected in the list box. 
// In this case, only a single item can be selected.
//
HRESULT STDMETHODCALLTYPE ListProvider::GetSelection(SAFEARRAY** pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;

    SAFEARRAY *psa = SafeArrayCreateVector(VT_UNKNOWN, 0, 1);
    int index = m_pControl->GetSelectedIndex(); 
    ListItemProvider* pItem = GetItemProviderByIndex(index); 
    pItem->AddRef();
    LONG i = 0;
    SafeArrayPutElement(psa, &i, pItem);
    *pRetVal = psa;
	return S_OK;
}

// Implementation of ISelectionProvider::get_CanSelectMultiple.
//
HRESULT STDMETHODCALLTYPE ListProvider::get_CanSelectMultiple(BOOL *pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
    *pRetVal = FALSE;
	return S_OK;
}
// Implementation of ISelectionProvider::get_IsSelectionRequired.
//
HRESULT STDMETHODCALLTYPE ListProvider::get_IsSelectionRequired(BOOL *pRetVal)
{
    if (pRetVal == NULL) return E_INVALIDARG;
   *pRetVal = TRUE;
   return S_OK;
}

