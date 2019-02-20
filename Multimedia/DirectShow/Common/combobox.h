#pragma once


class ComboBox : public Control
{


public:
    BOOL AddString(LPCTSTR sItem);
    BOOL InsertString(int index, LPCTSTR sItem);

    BOOL GetCurrentSelection(int *pindex);
    BOOL Select(int index);
    BOOL GetString(int index, TCHAR **ppBuffer);
    BOOL GetSelectedString(TCHAR **ppBuffer);

    void ClearItems();
    UINT Count();

	BOOL SetItemData(int nIndex, DWORD_PTR pData);
	BOOL GetItemData(int nIndex, DWORD_PTR *pData);
	BOOL GetCurrentSelectionItemData(DWORD_PTR *pData);

};