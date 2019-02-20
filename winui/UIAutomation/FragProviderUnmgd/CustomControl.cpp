/*************************************************************************************************
*
* File: CustomControl.cpp
*
* Description: Implementation of the custom list control.
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
#include "CustomControl.h"
#include "UIAProviders.h"


// CustomListControl class.
//
CustomListControl::CustomListControl(HWND hwnd)
{
    m_selectedIndex = -1;
    m_pListProvider = NULL;
    m_controlHwnd = hwnd;

    // Initialize the list items.
    AddItem(online, L"Fred");
    AddItem(offline, L"Prakash");
    AddItem(online, L"Kim");
    AddItem(online, L"Sandra");
    AddItem(offline, L"Silvio");
    SelectItem(0);
}

// Destructor.
//
CustomListControl::~CustomListControl()
{
    if (m_pListProvider != NULL)
    {
        m_pListProvider->Release();
    }
    m_itemCollection.clear();
}

// Adds an item to the end of the list.
//
bool CustomListControl::AddItem(STATUS status, WCHAR* name)
{
    if ((wcslen(name) == 0) || (GetCount() >= MAXITEMS) || (name == NULL))
    {
        return false;
    }
    int id = CreateUniqueId();
    CustomListItemControl* newItem = new CustomListItemControl(this, id, name);
    if (newItem == NULL)
    {
        return false;
    }
    newItem->SetStatus(status);

    // Add to collection.
    m_itemCollection.push_back(newItem);  
    SelectItem(0);

    // Raise UI Automation event. // TODO ONLY IF CLIENTS ARE LISTENING
    // SHOULD THIS BE DONE BY PROVIDER, AS WITH NOTIFYELEMENTSELECTED???

    // BUGBUG  This doesn't seem right as we are getting the "raw" runtime ID from the provider, not the one
    //  constructed by UIA.
    ListItemProvider* itemProvider = newItem->GetListItemProvider();
    itemProvider->NotifyItemAdded();
    return true;
}

// Removes the selected item.
//
bool CustomListControl::RemoveAt(int index)
{
    CustomListItemControl* deletedItem = GetItemAt(index);

    // Raise UI Automation event. Do this before the object disappears.      // TODO ONLY IF CLIENTS ARE LISTENING

    ListItemProvider* itemProvider = deletedItem->GetListItemProvider();
    itemProvider->NotifyItemRemoved();

    m_itemCollection.erase(m_itemCollection.begin() + index);
    SelectItem(GetSelectedIndex());

    return true;
}

CustomListItemControl* CustomListControl::GetItemAt(int index)
{
    if (index < (int)m_itemCollection.size())
    {
        return m_itemCollection.at(index);
    }
    return NULL;
}


// Gets the UI Automation provider for the list; creates it if necessary.
//
ListProvider* CustomListControl::GetListProvider()
{
    if (m_pListProvider == NULL)
    {
        m_pListProvider = new ListProvider(this);   
    }
    return m_pListProvider;
}

// Gets the index of the item at a point on the Y coordinate within the list.
//
int CustomListControl::IndexFromY(HWND hwnd, int y)
{
    int index = (int)(y / ITEMHEIGHT);
    if ((index < 0) || (GetCount() <= index))
    {
        index = -1;
    }
    return index;
}

// Sets the selected item and forces refresh.
//
void CustomListControl::SelectItem(int index)
{
    m_selectedIndex = index;
    if (m_selectedIndex >= (int)m_itemCollection.size())
    {
        m_selectedIndex = (int)m_itemCollection.size() - 1;  
    }
    InvalidateRect(m_controlHwnd, NULL, false);

    // Raise UI Automation event

    if (m_pListProvider != NULL)
    {
        ListItemProvider* itemProvider = m_pListProvider->GetItemProviderByIndex(m_selectedIndex);
        if (itemProvider != NULL)
        {
            itemProvider->NotifyElementSelected();
        }
    }
}

// Gets the index of the selected item.
//
int CustomListControl::GetSelectedIndex()
{
    return m_selectedIndex;
}

// Sets the index of the selected item.
//
void CustomListControl::SetSelectedIndex(int index)
{
    m_selectedIndex = index;
}

// Gets the focused state.
//
bool CustomListControl::GetIsFocused()
{
    return m_gotFocus;
}

// Sets the focused state.
//
void CustomListControl::SetIsFocused(bool isFocused)
{
    m_gotFocus = isFocused;
}

// Gets the count of items in the list.
//
int CustomListControl::GetCount()
{
    return (int)m_itemCollection.size();
}

// Creates a unique identifier within this instance of the control.
//
int CustomListControl::CreateUniqueId()
{
    static int uniqueId;
    return uniqueId++;
};

// Gets the HWND of the control.
//
HWND CustomListControl::GetHwnd()
{
    return m_controlHwnd;
}


// Registers the control class.
//
void RegisterListControl(HINSTANCE hInstance)
{
    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = ControlWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof(LONG_PTR);  // Make room for a pointer.
    wc.hInstance        = hInstance;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = L"CONTACTLIST";
    RegisterClass(&wc);
}


// Handles window messages for the HWND that contains the custom control.
//
LRESULT CALLBACK ControlWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
    case WM_CREATE:
        {
            // Create the control object.
            CustomListControl* pCustomList = new CustomListControl(hwnd);

            // Save the class instance as extra window data so that its members 
            // can be accessed from within this function.
            SetWindowLongPtr(hwnd, 0, (LONG)(LONG_PTR)pCustomList);
            break;
        }

    case WM_DESTROY:
        {
            // Destroy the control so interfaces are released.
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            delete pCustomList;
            // Remove the data stored with the window.
            SetWindowLongPtr(hwnd, 0, NULL);    
            break;
        }

    case WM_GETOBJECT:
        {
            // Register the control with UI Automation.
            // If the lParam matches the RootObjectId, send back the list provider.
            if ((long)lParam == (long)UiaRootObjectId)
            {
                // Get the control.
                CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
                // Return its associated UI Automation provider.
                LRESULT lresult = UiaReturnRawElementProvider(
                    hwnd, wParam, lParam, pCustomList->GetListProvider());
                return lresult;
            }
            return 0;
        }

    case WM_PAINT:
        {
            // Get the control.
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);

            // Set up graphics context.
            COLORREF color = RGB(255, 255, 200);
            HDC hdc = GetDC(hwnd);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Draw control rectangle.
            SelectObject(hdc, GetStockObject(BLACK_PEN)); 
            Rectangle(hdc, clientRect.left, clientRect.top, clientRect.right, 
                ITEMHEIGHT * MAXITEMS + 2);

            // Draw items.
            // Create and select a null pen so the rectangle isn't outlined.
            HPEN nullPen = CreatePen(PS_NULL, 1, RGB(0,0,0));
            SelectObject(hdc, nullPen);

            // Set up a brush for the background of the focused item.
            LOGBRUSH bgBrushParams;
            bgBrushParams.lbStyle = BS_SOLID;
            bgBrushParams.lbHatch = 0;

            // Create and select the font.
            HFONT font = GetFont(8);
            SelectObject(hdc, font);

            // Set transparency for text.
            SetBkMode(hdc, TRANSPARENT); 

            HBRUSH fillBrush;

            for (int i = 0; i < pCustomList->GetCount(); i++)              
            {
                // Get the rectangle for the item.
                RECT itemRect;
                itemRect.left = clientRect.left + 2;
                itemRect.top = clientRect.top + 2 + (ITEMHEIGHT * i);
                itemRect.right = clientRect.right - 2;
                itemRect.bottom = itemRect.top + ITEMHEIGHT;

                // Set the default text color.
                SetTextColor(hdc, RGB(0, 0, 0));

                // Set up the appearance of the focused item.
                // It's different depending on whether the list control has focus.
                if (i == pCustomList->GetSelectedIndex())
                {
                    if (pCustomList->GetIsFocused())
                    {
                        DrawFocusRect(hdc, &itemRect); 
                        bgBrushParams.lbColor = RGB(0,0,255);  // dark blue
                        fillBrush = CreateBrushIndirect(&bgBrushParams);
                        SelectObject(hdc, fillBrush);
                        Rectangle(hdc, itemRect.left+1, itemRect.top+1, itemRect.right, itemRect.bottom);
                        DeleteObject(fillBrush);
                        SetTextColor(hdc, RGB(255, 255, 255));
                    }
                    else
                    {
                        bgBrushParams.lbColor = RGB(200,200,200);  // light gray
                        fillBrush = CreateBrushIndirect(&bgBrushParams);
                        SelectObject(hdc, fillBrush);
                        Rectangle(hdc, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);
                        DeleteObject(fillBrush);
                    }
                }
                // Get the item.
                CustomListItemControl* item = pCustomList->GetItemAt(i);

                // Draw the text.
                TextOut(hdc, itemRect.left+IMAGEWIDTH+5, itemRect.top+2, 
                    item->GetName(), (int)wcslen(item->GetName()));

                // Draw the status icon.
                if (item->GetStatus() == online)
                {
                    bgBrushParams.lbColor = RGB(0, 192, 0);   // green
                    fillBrush = CreateBrushIndirect(&bgBrushParams);
                    SelectObject(hdc, fillBrush);
                }
                else
                {
                    bgBrushParams.lbColor = RGB(255, 0, 0);   // red
                    fillBrush = CreateBrushIndirect(&bgBrushParams);
                    SelectObject(hdc, fillBrush);
                }
                Rectangle(hdc, itemRect.left + 2, itemRect.top + 3,
                    itemRect.left + IMAGEWIDTH + 2, itemRect.top + 3 + IMAGEHEIGHT);
                DeleteObject(fillBrush);

            }  // for each item.

            ReleaseDC(hwnd, hdc);
            DeleteObject(font);
            DeleteObject(nullPen);
            break;
        }

    case WM_SETFOCUS:
        {
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            if (pCustomList != NULL)
            {
                pCustomList->SetIsFocused(true);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;        
        }
    case WM_KILLFOCUS:
        {
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            pCustomList->SetIsFocused(false); 
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

    case WM_DELETEITEM:
        {
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            // Don't allow deletion of the last remaining item. This is just to
            // simplify the logic of the sample.
            if (pCustomList->GetCount() > MINITEMS)
            {
                pCustomList->RemoveAt(pCustomList->GetSelectedIndex());
            }
            break;
        }

    case CUSTOMLB_ADDITEM:
        {
            CustomListControl* pCustomList = 
                (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            if (pCustomList->GetCount() < MAXITEMS)
            {
                pCustomList->AddItem((STATUS)wParam, (WCHAR*)lParam);
            }
            break;
        }

    case WM_GETDLGCODE:
        {
            // Trap arrow keys.
            return DLGC_WANTARROWS | DLGC_WANTCHARS;
            break;
        }

    case WM_LBUTTONDOWN:
        {
            // Retrieve the control.
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);

            // Get the item under the cursor. This is -1 if the user clicked on a blank space.
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            int newItem = pCustomList->IndexFromY(hwnd, y);

            // Set the focus to the control regardless of whether the selection is valid.
            SetFocus(hwnd);
            if (newItem >= 0)
            {
                pCustomList->SelectItem(newItem);
            }

            break;
        }

    case WM_KEYDOWN:
        // Move the selection with up/down arrows.
        {
            // Retrieve the control.
            CustomListControl* pCustomList = (CustomListControl*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);

            // Ignore keystrokes if listbox does not have focus.
            switch (wParam)
            {
            case VK_UP:
                if (pCustomList->GetSelectedIndex() > 0) 
                {
                    pCustomList->SelectItem(pCustomList->GetSelectedIndex() - 1);
                }
                return 0;
                break;

            case VK_DOWN:
                if (pCustomList->GetSelectedIndex() < pCustomList->GetCount() - 1)          
                {
                    pCustomList->SelectItem(pCustomList->GetSelectedIndex() + 1);  
                }
                return 0;
                break;
            }
            break; // WM_KEYDOWN
        }
    }  // switch (message)

    return DefWindowProc(hwnd, message, wParam, lParam);
}


// CustomListItemControl class 
//
// Constructor.
CustomListItemControl::CustomListItemControl(CustomListControl* pOwner, int id, WCHAR* name)
{
    m_pOwnerControl = pOwner;
    m_Id = id;
    size_t nameLen = wcslen(name);
    m_name = new WCHAR[nameLen +1 ];  
    wcscpy_s(m_name, nameLen + 1, name);
    m_pListItemProvider = NULL;
}

// Destructor.
CustomListItemControl::~CustomListItemControl()
{
    free(m_name);
    if (m_pListItemProvider != NULL)
    {
        m_pListItemProvider->Release();
    }
}

// Gets the index of this item in the collection.
//
int CustomListItemControl::GetItemIndex()
{
    for (int i = 0; i < m_pOwnerControl->GetCount(); i++)
    {
        if (m_pOwnerControl->GetItemAt(i) == this)
        {
            return i;
        }
    }
    // Item not found; shouldn't happen.
    return -1;
}

// Gets the UI Automation provider for the list item; creates it if necessary.
//
ListItemProvider* CustomListItemControl::GetListItemProvider()
{
    if (m_pListItemProvider == NULL)
    {
        m_pListItemProvider = new ListItemProvider(this);  
    }
    return m_pListItemProvider;
}

// Gets the custom list control that holds this item.
//
CustomListControl* CustomListItemControl::GetOwnerList()
{
    return m_pOwnerControl;
}

// Gets the status (online/offline) of this contact.
//
STATUS CustomListItemControl::GetStatus()
{
    return m_status;
}

// Sets the status (online/offline) of this contact.
//
void CustomListItemControl::SetStatus(STATUS status)
{
    m_status = status;
}

// Gets the name of the contact.
//
WCHAR* CustomListItemControl::GetName()
{
    return m_name;
}

// Gets the Id of the contact.
//
int CustomListItemControl::GetId()
{
    return m_Id;
}


// Helper functions. 
//
// Retrieves a font for list items.
//
HFONT GetFont(LONG height) 
{ 
    // Get a handle to the ANSI fixed-pitch font, and copy 
    // information about the font to a LOGFONT structure. 
    static LOGFONT lf;
    GetObject(GetStockObject(ANSI_VAR_FONT), sizeof(LOGFONT), &lf); 

    // Change the font size.
    lf.lfHeight = height;

    // Create the font and return its handle.  
    return CreateFont(lf.lfHeight, lf.lfWidth, 
        lf.lfEscapement, lf.lfOrientation, lf.lfWeight, 
        lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, 
        lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality, 
        lf.lfPitchAndFamily, lf.lfFaceName); 
} 

