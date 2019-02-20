//-----------------------------------------------------------------------------
// File: Button.h
// Desc: Button control classes
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once




// PushButton: Simple text or bitmap button
class Button : public Control
{
public:
	HRESULT CreateText(HWND hParent, const TCHAR *szCaption, int nID, const RECT& rcBound);
	HRESULT CreateBitmap(HWND hParent, int nImgID, int nID, const RECT& rcBound);
	BOOL SetImage(WORD nImgId);

	void SetCheck(int nCheck)
	{
		SendMessage(BM_SETCHECK, (WPARAM)nCheck, 0L);
	}

	BOOL IsChecked()
	{
		return SendMessage(BM_GETCHECK, 0, 0 ) == BST_CHECKED;
	}
};

