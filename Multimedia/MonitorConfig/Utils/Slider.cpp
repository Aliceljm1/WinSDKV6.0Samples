//-----------------------------------------------------------------------------
// File: slider.cpp
// Desc: Slider control class
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "dialog.h"
#include <strsafe.h>


//-----------------------------------------------------------------------------
// Name: Create
// Desc: Create a new instance of a slider control.
//
// hParent: Parent window
// rcSize:  Bounding rectangle
// dwStyle: Additional styles
//-----------------------------------------------------------------------------

HRESULT Slider::Create(HWND hParent, const Rect& rcSize, DWORD dwStyle)
{
    CreateStruct create;
    create.lpszClass = TRACKBAR_CLASS;
    create.SetBoundingRect(rcSize);
    create.hwndParent = hParent;
    create.style = dwStyle;

    return Control::Create(create);
}



//-----------------------------------------------------------------------------
// Name: GetPosition
// Desc: Gets the current position of the slider.
//-----------------------------------------------------------------------------

DWORD Slider::GetPosition() const
{
    return (DWORD)(m_scale * (LONG)SendMessage(TBM_GETPOS, 0, 0));
}



//-----------------------------------------------------------------------------
// Name: SetPosition
// Desc: Sets the position of the slider.
//-----------------------------------------------------------------------------

void Slider::SetPosition(DWORD pos)
{
    SendMessage(TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos / m_scale);
}



//-----------------------------------------------------------------------------
// Name: SetRange
// Desc: Sets the slider range.
//-----------------------------------------------------------------------------

void Slider::SetRange(DWORD min, DWORD max)
{
    if (min > max)
    {
        if (min > MAXLONG)
        {
            m_scale = -2;
            SendMessage(TBM_SETRANGEMIN, TRUE, - (LONG)(min / 2));
            SendMessage(TBM_SETRANGEMAX, TRUE, - (LONG)(max / 2));
        }
        else
        {
            m_scale = -1;
            SendMessage(TBM_SETRANGEMIN, TRUE, - (LONG)min);
            SendMessage(TBM_SETRANGEMAX, TRUE, - (LONG)max);
        }

    }
    else
    {
        if (max > MAXLONG)
        {
            m_scale = 2;
        }
        else
        {
            m_scale = 1;
        }
        SendMessage(TBM_SETRANGEMIN, TRUE, (LONG)(min / m_scale));
        SendMessage(TBM_SETRANGEMAX, TRUE, (LONG)(max / m_scale));
    }
}

//-----------------------------------------------------------------------------
// Name: SetRangeAndPosition
// Desc: Sets the slider range and the slider position.
//-----------------------------------------------------------------------------

void Slider::SetRangeAndPosition(DWORD min, DWORD max, DWORD pos)
{
    SetRange(min, max);
    SetPosition(pos);
}


//-----------------------------------------------------------------------------
// Name: SetThumbLength
// Desc: Sets the size of the slider, in pixels.
//-----------------------------------------------------------------------------

void Slider::SetThumbLength(int iLen)
{
    // Intuitively, if you want to change the size of the slider,
    // you must set the TBS_FIXEDLENGTH style.
    AddStyle(TBS_FIXEDLENGTH);
    SendMessage(TBM_SETTHUMBLENGTH, (WPARAM)iLen, 0);
}

//-----------------------------------------------------------------------------
// Name: SetTickFrequency
// Desc: Sets the tick frequency.
//-----------------------------------------------------------------------------

void Slider::SetTickFrequency(DWORD dwFreq)
{
    // Add the AUTOTICKS style to display the ticks.
    AddStyle(TBS_AUTOTICKS);
    SendMessage(TBM_SETTICFREQ, dwFreq, 0);
}

//-----------------------------------------------------------------------------
// Name: ShowTip
// Desc: Show the tool tip for the slider.
//
// toolTip: Tool tip control.
//-----------------------------------------------------------------------------

void Slider::ShowTip(ToolTip& toolTip)
{

    RECT rcSlider;
    RECT rcThumb;
    SIZE sizeTip;

    TCHAR msg[16];      // Enough to show MAXDWORD in decimal

    StringCchPrintf(msg, 32, TEXT("%u"), GetPosition());

    SendMessage(TBM_GETTHUMBRECT, 0, (LPARAM)&rcThumb);

    GetWindowRect(Window(), &rcSlider);

    toolTip.Activate(TRUE);

    // Convert thumb to screen coordinates
    OffsetRect(&rcThumb, rcSlider.left, rcSlider.top);

    toolTip.GetSize(&sizeTip);

    OffsetRect(&rcThumb, (rcThumb.right - rcThumb.left), -sizeTip.cy);

    toolTip.SetPosition((WORD)rcThumb.left, (WORD)rcThumb.top);
    toolTip.SetText(msg);


}

//-----------------------------------------------------------------------------
// Name: InitSlider
// Desc: Initializes all of the slider properties.
//-----------------------------------------------------------------------------

void InitSlider(Slider& slider, HWND hwnd, LONG min, LONG max, WORD pos)
{
    slider.SetWindow(hwnd);
    slider.SetRange(min, max);
    slider.SetPosition(pos);
}


/// TOOLTIP class

//-----------------------------------------------------------------------------
// Name: Initialize
// Description: Creates the tool tip window.
//-----------------------------------------------------------------------------

BOOL ToolTip::Initialize(HWND hParent)
{
    BOOL bResult = TRUE;

    m_hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hParent, NULL, GetInstance(),
        NULL);

    if (m_hwnd)
    {
        SetWindowPos(m_hwnd, HWND_TOPMOST,0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        bResult = AddTool(SLIDER_TOOLTIP);
    }
    else
    {
        bResult = FALSE;
    }

    return bResult;
}


//-----------------------------------------------------------------------------
// Name: AddTool [private]
// Description: Add a new tool.
//-----------------------------------------------------------------------------

BOOL ToolTip::AddTool(UINT_PTR id)
{
    TOOLINFO tinfo;
    ZeroMemory(&tinfo, sizeof(tinfo));


    tinfo.cbSize = sizeof(TOOLINFO); 
    tinfo.uFlags = TTF_TRACK | TTF_ABSOLUTE;
    tinfo.hwnd = GetParent(m_hwnd);
    tinfo.uId = id;

    LRESULT result = ::SendMessage(m_hwnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&tinfo);

    return (BOOL)result;
}

//-----------------------------------------------------------------------------
// Name: Activate
// Description: Activate or deactivate the tool tip.
//-----------------------------------------------------------------------------

BOOL ToolTip::Activate(BOOL bActivate)
{
    TOOLINFO tinfo;
    ZeroMemory(&tinfo, sizeof(tinfo));

    tinfo.cbSize = sizeof(tinfo);
    tinfo.hwnd = GetParent(m_hwnd);
    tinfo.uId = SLIDER_TOOLTIP;

    SendMessage(TTM_TRACKACTIVATE, (WPARAM)bActivate, (LPARAM)&tinfo);
    return TRUE;
}


//-----------------------------------------------------------------------------
// Name: SetPosition
// Description: Set the tool tip position.
//-----------------------------------------------------------------------------

BOOL ToolTip::SetPosition(WORD x, WORD y)
{
    SendMessage(TTM_TRACKPOSITION, 0, MAKELONG(x,y));
    return TRUE;
}


//-----------------------------------------------------------------------------
// Name: SetText
// Description: Set the text of the tool tip.
//-----------------------------------------------------------------------------

BOOL ToolTip::SetText(LPTSTR szText)
{
    TOOLINFO tinfo;
    ZeroMemory(&tinfo, sizeof(tinfo));

    tinfo.cbSize = sizeof(tinfo);
    tinfo.hwnd = GetParent(m_hwnd);
    tinfo.uId = SLIDER_TOOLTIP;
    tinfo.lpszText = szText;

    SendMessage(TTM_UPDATETIPTEXT, 0, (LPARAM)&tinfo);
    return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetSize
// Description: Get the size of the tool tip window.
//-----------------------------------------------------------------------------

BOOL ToolTip::GetSize(SIZE *psz)
{
    TOOLINFO tinfo;
    ZeroMemory(&tinfo, sizeof(tinfo));

    tinfo.cbSize = sizeof(tinfo);
    tinfo.hwnd = GetParent(m_hwnd);
    tinfo.uId = SLIDER_TOOLTIP;

    LRESULT result = SendMessage(TTM_GETBUBBLESIZE, 0, (LPARAM)&tinfo);

    psz->cx = LOWORD(result);
    psz->cy = HIWORD(result);

    return TRUE;
}
