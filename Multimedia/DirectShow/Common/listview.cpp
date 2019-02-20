//-----------------------------------------------------------------------------
// File: ListView.cpp
// Desc: List view control class
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "dialog.h"

//-----------------------------------------------------------------------------
// CListView ctor
//-----------------------------------------------------------------------------

CListView::CListView() 
: m_cNumColumns(0), m_cNumItems(0)
{
}

//-----------------------------------------------------------------------------
// Name: SetView
// Desc: Sets the current view (icon, list, report, etc).
//-----------------------------------------------------------------------------

void CListView::SetView(ViewType view)
{
    DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE); 
    
    // (taken from MSDN)
    // Only set the window style if the view bits have changed. 
    if ((dwStyle & LVS_TYPEMASK) != view) 
    {
        SetWindowLong(m_hwnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | view); 
    }
}

//-----------------------------------------------------------------------------
// Name: AddColumn
// Desc: Adds a new column.
//
// sText: Column heading
// width: Column width
//-----------------------------------------------------------------------------

BOOL CListView::AddColumn(TCHAR *sText, int width)
{
    LVCOLUMN lvc; 
    ZeroMemory(&lvc, sizeof(LVCOLUMN)); // not strictly necessary

    int iCol = m_cNumColumns + 1; 
 
    // Initialize the LVCOLUMN structure.
    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM; 
     
    // Add the column. 
    lvc.iSubItem = iCol;
    lvc.pszText = sText;    
    lvc.cx = width;           // width of column in pixels
    int result = ListView_InsertColumn(m_hwnd, iCol, &lvc);
    if (result == -1)
    {
        return FALSE; 
    } 
    else
    {
        m_cNumColumns++;
        return TRUE; 
    }
}

//-----------------------------------------------------------------------------
// Name: DeleteAllItems
// Desc: Delete everything in the list.
//-----------------------------------------------------------------------------

void CListView::DeleteAllItems()
{
    ListView_DeleteAllItems(m_hwnd);
    m_cNumItems = 0;
}

//-----------------------------------------------------------------------------
// Name: AddItem
// Desc: Add a new item to the list.
//
// sText:  Item text
// lParam: User data
// pIndex: Receives the index of the item.
//-----------------------------------------------------------------------------

BOOL CListView::AddItem(TCHAR *sText, LPARAM lParam, int *pIndex)
{
    LVITEM item;
    ZeroMemory(&item, sizeof(LVITEM));

    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.iItem = m_cNumItems;
    item.pszText = sText;
    item.lParam = lParam;

    int result = ListView_InsertItem(m_hwnd, &item);

    if (pIndex)
    {
        *pIndex = result;
    }

    if (result == -1)
    {
        return FALSE;
    }
    else
    {
        m_cNumItems++;
        return TRUE;
    }
}

//-----------------------------------------------------------------------------
// Name: AddSubItem
// Desc: Add a subitem to an existing item in the list.
//
// iItem:  Index of the item.
// iCol:   Index of the column for the subitem
// sText:  Subitem text
//-----------------------------------------------------------------------------

BOOL CListView::AddSubItem(unsigned int iItem, unsigned int iCol, TCHAR *sText)
{
    if (iItem >= m_cNumItems)
    {
        assert(false);
        return FALSE;
    }

    if (iCol < 1 || iCol >= m_cNumColumns)
    {
        assert(false);
        return FALSE;
    }


    LVITEM item;
    ZeroMemory(&item, sizeof(LVITEM));

    item.mask = LVIF_TEXT;
    item.iItem = iItem;
    item.iSubItem = iCol;
    item.pszText = sText;

    int result = ListView_SetItem(m_hwnd, &item);

    return (result == -1 ? FALSE : TRUE);
}

//-----------------------------------------------------------------------------
// Name: GetSelectedItem
// Desc: Return the index of the selected item. [or first selected item]
//-----------------------------------------------------------------------------

int CListView::GetSelectedItem()
{
    return ListView_GetNextItem(m_hwnd, -1, LVNI_SELECTED);
}

//-----------------------------------------------------------------------------
// Name: GetItemData
// Desc: Returns the user data for an item.
//
// index: Index of the list view item
// pData: Receives the data, as an LPARAM value
//-----------------------------------------------------------------------------

BOOL CListView::GetItemData(int index, LPARAM *pData)
{
    assert(pData != NULL);
    if (pData == NULL)
    {
        return FALSE;
    }

    LVITEM item;
    item.iItem = index;
    item.mask = LVIF_PARAM;
    BOOL result = ListView_GetItem(m_hwnd, &item);
    if (result)
    {
        *pData = item.lParam;
    }
    return result;
}
