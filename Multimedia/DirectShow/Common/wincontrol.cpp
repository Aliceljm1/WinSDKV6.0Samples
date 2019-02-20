//-----------------------------------------------------------------------------
// File: Control.cpp
// Desc: Base control class
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "dialog.h"

//-----------------------------------------------------------------------------
// Name: Create
// Desc: Create a new instance of the control.
//
// create: Creation parameters.
//-----------------------------------------------------------------------------

HRESULT Control::Create(const CREATESTRUCT& create)
{
    if (m_hwnd != NULL)
    {
        // The control was already created.
        return E_FAIL;
    }

    if (create.hwndParent == NULL)
    {
        return E_INVALIDARG;
    }

    HINSTANCE hinst = create.hInstance;

    if (hinst == NULL)
    {
        hinst = GetInstance();
    }

    if (hinst == NULL)
    {
        return E_INVALIDARG;
    }


    HWND hwnd = CreateWindowEx(
        create.dwExStyle, create.lpszClass, create.lpszName, 
        create.style | WS_CHILD | WS_VISIBLE,
        create.x, create.y, create.cx, create.cy, create.hwndParent, create.hMenu,
        hinst, create.lpCreateParams);

    if (hwnd== 0)
    {
        return __HRESULT_FROM_WIN32(GetLastError());
    }

    SetWindow(hwnd);
    return S_OK;
};