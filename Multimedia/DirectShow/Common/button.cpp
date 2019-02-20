//-----------------------------------------------------------------------------
// File: Button.cpp
// Desc: Button control classes
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "dialog.h"

/****************************** Button class ****************************/

//-----------------------------------------------------------------------------
// Name: CreateText
// Desc: Creates a simple text button
//
// hParent:   Parent window
// szCaption: Text on the button
// nID:       Command ID.
// rcBound:   Bounding rectangle.
//-----------------------------------------------------------------------------

HRESULT Button::CreateText(HWND hParent, const TCHAR *szCaption, int nID, 
                               const RECT& rcBound)
{
    CREATESTRUCT create;
	ZeroMemory(&create, sizeof(CREATESTRUCT));

    create.x = rcBound.left;
    create.y = rcBound.top;
    create.cx = rcBound.right - create.x;
    create.cy = rcBound.bottom - create.y;

    create.hwndParent = hParent;
    create.lpszName = szCaption;
    create.hMenu = (HMENU)(INT_PTR)nID;
    create.lpszClass = TEXT("BUTTON");
    create.style = BS_PUSHBUTTON | BS_FLAT;
    return Control::Create(create);
}

//-----------------------------------------------------------------------------
// Name: CreateBitmap
// Desc: Creates a simple bitmap button
//
// hParent: Parent window
// nImgID:  Resource ID of the bitmap
// nID:     Command ID.
// rcBound: Bounding rectangle.
//-----------------------------------------------------------------------------

HRESULT Button::CreateBitmap(HWND hParent, int nImgID, int nID, const RECT& rcSize)
{
    HRESULT hr = CreateText(hParent, NULL, nID, rcSize);
    if (SUCCEEDED(hr))
    {
        SetImage(nImgID);
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Name: SetImage
// Desc: Set a bitmap for the button
//
// nImgID:  Resource ID of the bitmap
//-----------------------------------------------------------------------------

BOOL Button::SetImage(WORD nImgId)
{
    AddStyle(BS_BITMAP);
    HBITMAP hBitmap = SetBitmapImg(GetInstance(), nImgId, m_hwnd);
    return (hBitmap ? TRUE : FALSE);
}
