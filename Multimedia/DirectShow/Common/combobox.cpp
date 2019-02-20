#include "dialog.h"


BOOL ComboBox::AddString(LPCTSTR sItem)
{
    if (!sItem)
    {
        return FALSE;
    }

    LRESULT result = SendMessage(CB_ADDSTRING, 0, (LPARAM)sItem);
    if (result == CB_ERR || result == CB_ERRSPACE)
    {
        return FALSE;
    }

    return TRUE;
}


BOOL ComboBox::InsertString(int index, LPCTSTR sItem)
{
    if (!sItem)
    {
        return FALSE;
    }

    LRESULT result = SendMessage(CB_INSERTSTRING, (WPARAM)index, (LPARAM)sItem);
    if (result == CB_ERR || result == CB_ERRSPACE)
    {
        return FALSE;
    }
    return TRUE;
}


BOOL ComboBox::GetCurrentSelection(int *pindex)
{
    if (!pindex)
    {
        return FALSE;
    }

    LRESULT res = SendMessage(CB_GETCURSEL, 0, 0);
    if (res == CB_ERR)
    {
        return FALSE;
    }
    else
    {
        *pindex = (int)res;
        return TRUE;
    }
}

BOOL ComboBox::Select(int index)
{
    LRESULT res = SendMessage(CB_SETCURSEL, (WPARAM)index, 0);
    if (res == CB_ERR)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


BOOL ComboBox::GetString(int index, TCHAR **ppBuffer)
{
    if (ppBuffer == NULL)
    {
        return FALSE;
    }

    LRESULT res = SendMessage(CB_GETLBTEXTLEN, (WPARAM)index, 0);
    if (res == CB_ERR)
    {
        return FALSE;
    }

    DWORD count = (DWORD)(res + 1);

    TCHAR *pBuffer = (TCHAR*)CoTaskMemAlloc(sizeof(TCHAR) * count);
    if (pBuffer == NULL)
    {
        return FALSE;
    }

    res = SendMessage(CB_GETLBTEXT, (WPARAM)index, (LPARAM)pBuffer);

    if (res == CB_ERR)
    {
        CoTaskMemFree(pBuffer);
        return FALSE;
    }
    else
    {
        *ppBuffer = pBuffer;
        return TRUE;
    }
}


BOOL ComboBox::GetSelectedString(TCHAR **ppBuffer)
{
    int index = 0;
    if (GetCurrentSelection(&index))
    {
        return GetString(index, ppBuffer);
    }
    else
    {
        return FALSE;
    }
}

UINT ComboBox::Count()
{
    LRESULT res = SendMessage(CB_GETCOUNT, 0, 0);
    return (res == CB_ERR ? 0 : (UINT)res);
}


void ComboBox::ClearItems()
{
    SendMessage(CB_RESETCONTENT, 0, 0);
}



BOOL ComboBox::SetItemData(int nIndex, DWORD_PTR pData)
{
    LRESULT result = SendMessage(CB_SETITEMDATA,  nIndex, (LPARAM) pData);
	if (result == CB_ERR)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BOOL ComboBox::GetItemData(int nIndex, DWORD_PTR *pData)
{

	LRESULT result = SendMessage(CB_GETITEMDATA, nIndex, 0);
	if (result == CB_ERR)
	{
		return FALSE;
	}
	else
	{
		*pData = (DWORD_PTR)result;
		return TRUE;
	}
}

BOOL ComboBox::GetCurrentSelectionItemData(DWORD_PTR *pData)
{
	int selection;
	BOOL bResult = GetCurrentSelection(&selection);
	if (bResult)
	{
		bResult = GetItemData(selection, pData);
	}
	return bResult;
}