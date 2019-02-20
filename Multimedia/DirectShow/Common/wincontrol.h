//-----------------------------------------------------------------------------
// File: WinControl.h
// Desc: Base control class
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#include <assert.h>

class Control
{
public:
	Control() : m_hwnd(0) {}
    
	void SetWindow(HWND hwnd) { m_hwnd = hwnd; }
    HWND Window() const { return m_hwnd; }

	void Enable(BOOL bEnable) 
    { 
        if (!bEnable &&  m_hwnd == GetFocus())
        {
            // If we're being disabled and this control has focus,
            // set the focus to the next control.

            // For more info, see Raymond Chen's blog:
            // http://weblogs.asp.net/oldnewthing/archive/2004/08/04/208005.aspx
            ::SendMessage(GetParent(m_hwnd), WM_NEXTDLGCTL, 0, FALSE);
        }

        EnableWindow(m_hwnd, bEnable); 
    }

    // SendMessage: Send a message to the control.
    LRESULT SendMessage(UINT msg, WPARAM wParam, LPARAM lParam) const
    {
        assert(m_hwnd);
        return ::SendMessage(m_hwnd, msg, wParam, lParam);
    }

    bool HasStyle(LONG style) const
    {
        return (GetWindowLong(m_hwnd, GWL_STYLE) & style) == style;
    }

    LONG AddStyle(LONG style)
    {
        LONG old_style = GetWindowLong(m_hwnd, GWL_STYLE);
        return SetWindowLong(m_hwnd, GWL_STYLE,  old_style | style);
    }

    LONG RemoveStyle(LONG style)
    {
        LONG old_style = GetWindowLong(m_hwnd, GWL_STYLE);
        old_style = old_style & (~style);
        return SetWindowLong(m_hwnd, GWL_STYLE, old_style);
    }

    LONG EnableStyle(bool bEnable, LONG style)
    {
        return (bEnable ? AddStyle(style) : RemoveStyle(style));
    }

protected:
	HRESULT Create(const CREATESTRUCT& create);
    HWND m_hwnd;
};