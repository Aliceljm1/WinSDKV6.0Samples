/*************************************************************************************************
*
* File: CustomControl.h
*
* Description: Declarations for the custom list control.
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


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <UIAutomationCore.h>
#include <UIAutomationCoreAPI.h>
#include "resource.h"

#include <deque>
using namespace std;

#include <assert.h>

enum STATUS 
{
    offline,
    online
};


// Custom message type for adding new contacts.
#define CUSTOMLB_ADDITEM WM_USER + 1

void RegisterListControl(HINSTANCE hInstance);
LRESULT CALLBACK ControlWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HFONT GetFont(LONG height);

// Forward declarations.
class CustomListItemControl;
class ListProvider;
class ListItemProvider;

// CustomList control class -- the list box itself.
//
class CustomListControl
{
    // Dimensions of list item.
#define ITEMHEIGHT 15 
#define ITEMWIDTH  70
    // Dimensions of image that signifies item status.
#define IMAGEWIDTH 10
#define IMAGEHEIGHT 10
    // Offset of text to allow room for image.
#define TEXTINDENT 15
    // Constraints on number of items in list.
#define MAXITEMS   10
#define MINITEMS    1

private:
    bool   m_gotFocus;
    int    m_selectedIndex; 
    HWND   m_controlHwnd;
    deque<CustomListItemControl*> m_itemCollection;
    ListProvider*   m_pListProvider;

public:
    CustomListControl(HWND hwnd);
    virtual ~CustomListControl();
    ListProvider* GetListProvider();
    int IndexFromY(HWND hwnd, int y);
    void SelectItem(int index);
    int GetSelectedIndex(); 
    void SetSelectedIndex(int index);
    bool GetIsFocused();
    void SetIsFocused(bool isFocused);
    bool AddItem(STATUS status, WCHAR* name);
    CustomListItemControl* GetItemAt(int index);
    bool RemoveAt(int index);
    int GetCount();
    int CreateUniqueId();
    HWND GetHwnd();
};

// CustomListItem control class -- an item in the list.
//
class CustomListItemControl
{
private:
    int m_Id; 
    WCHAR* m_name;
    STATUS m_status;
    ListItemProvider* m_pListItemProvider;
    CustomListControl* m_pOwnerControl;

public:
    CustomListItemControl(CustomListControl* owner, int id, WCHAR* name);
    virtual ~CustomListItemControl();
    int GetItemIndex();
    CustomListControl* GetOwnerList();
    ListItemProvider* GetListItemProvider();
    STATUS GetStatus();
    void SetStatus(STATUS status);
    WCHAR* GetName();
    int GetId();
};