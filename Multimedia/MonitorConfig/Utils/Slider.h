//-----------------------------------------------------------------------------
// File: slider.h
// Desc: Slider control class
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

// NOTE: This class works around two limitations of the trackbar control:
//
// (1) The trackbar control has an effective numeric range of 31 bits (not 32)
//     For example, you can set the range to (0,MAXLONG) or (MINLONG, 0) but
//     not to (MINLONG,MAXLONG)
//
// (2) The trackbar control range cannot be reversed (i.e., min > max).
//
// These two issues are handled by applying a scaling factor to the position.
// However, this means the trackbar's default tooltip cannot be used, 
// because it displays the scaled position. Therefore we provide a custom 
// tooltip. 


class ToolTip;

const UINT_PTR SLIDER_TOOLTIP = 100;


//-----------------------------------------------------------------------------
// Name: Slider
// Description: Helper class for slider (trackbar) controls.
//-----------------------------------------------------------------------------

class Slider : public Control
{
private:
    int    m_scale; // Scaling factor for the slider values.

public:

    Slider() : m_scale(1)
    {
    }

	HRESULT Create(HWND hParent, const Rect& rcSize, DWORD dwStyle = 0);

    DWORD   GetPosition() const;
    void    SetPosition(DWORD pos);
    void    SetRange(DWORD min, DWORD max);

    void    SetRangeAndPosition(DWORD min, DWORD max, DWORD pos);

    void    SetThumbLength(int iLen);
	void    SetTickFrequency(DWORD dwFreq);

    void    ShowTip(ToolTip& toolTip);
};

// InitSlider
// Sets the window, range, and position in one call.
void InitSlider(Slider& slider, HWND hwnd, LONG min, LONG max, LONG pos);


//-----------------------------------------------------------------------------
// Name: ToolTip
// Description: Helper class for tool tip controls.
//-----------------------------------------------------------------------------

class ToolTip : public Control
{
private:
    BOOL    AddTool(UINT_PTR id);

public:

    ToolTip()
    {
    }

    BOOL    Initialize(HWND hParent);
    BOOL    UpdateText();
    BOOL    Activate(BOOL bActivate);
    BOOL    SetPosition(WORD x, WORD ys);
    BOOL    SetText(LPTSTR szText);
    BOOL    GetSize(SIZE *psz);
};
