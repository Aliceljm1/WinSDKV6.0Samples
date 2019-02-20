//-----------------------------------------------------------------------------
// File: ListView.h
// Desc: List view control class
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

class CListView : public Control
{

public:
    enum ViewType
    {
        LISTVIEW_ICON       = LVS_ICON,
        LISTVIEW_SMALL_ICON = LVS_SMALLICON,
        LISTVIEW_LIST       = LVS_LIST,
        LISTVIEW_REPORT     = LVS_REPORT
    };

    CListView();
    void SetView(ViewType view);
    BOOL AddColumn(TCHAR *sText, int width);
    BOOL AddItem(TCHAR *sText, LPARAM lParam, int *pIndex);
    BOOL AddSubItem(unsigned int iItem, unsigned int iCol, TCHAR *sText);

    int  GetSelectedItem();
    BOOL GetItemData(int index, LPARAM *pData);

    void DeleteAllItems();

private:
    ULONG m_cNumColumns;
    ULONG m_cNumItems;
};