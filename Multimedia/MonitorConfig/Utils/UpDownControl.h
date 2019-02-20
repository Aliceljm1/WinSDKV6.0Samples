//////////////////////////////////////////////////////////////////////////
// UpDownControl.h: Up-down (spinner) control class
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#pragma once

class UpDownControl : public Control
{
private:
    int    m_scale;

public:

    UpDownControl() : m_scale(1)
    {
    }

    void    SetBuddyWindow(HWND hBuddy);
    void    SetRange(DWORD low, DWORD high);
    void    SetPosition(DWORD pos);
    DWORD   GetPosition();
};