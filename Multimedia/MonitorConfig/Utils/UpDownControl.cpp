//////////////////////////////////////////////////////////////////////////
// UpDownControl.cpp: Up-down (spinner) control class
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include "dialog.h"


//-----------------------------------------------------------------------------
// Name: SetBuddyWindow
// Description: Set the buddy window for the up-down control.
//-----------------------------------------------------------------------------

void UpDownControl::SetBuddyWindow(HWND hBuddy)
{
    SendMessage(UDM_SETBUDDY, (WPARAM)hBuddy, 0);
}

//-----------------------------------------------------------------------------
// Name: SetRange
// Description: Set the numeric range.
//
// Note: The up-down control uses a signed value for its 32-bit range. 
//       For values outside this range, we apply a scaling factor.
//-----------------------------------------------------------------------------

void UpDownControl::SetRange(DWORD low, DWORD high)
{
    assert(low <= high);

    if (high > MAXLONG)
    {
        m_scale = 2;
    }
    else
    {
        m_scale = 1;
    }

    SendMessage(UDM_SETRANGE32, (WPARAM)(low/m_scale), (LPARAM)(high/m_scale));

    // For very large ranges, adjust the acceleration. Otherwise it takes too
    // long to spin up and down the entire range.
    DWORD range = high - low;

    if (range > 12000)
    {
        UDACCEL accel[3] = { 
            { 0, 1 },
            { 1, range / 1600 },
            { 2, range / 400 }
        };

        SendMessage(UDM_SETACCEL, 3, (LPARAM)accel);
    }
}

//-----------------------------------------------------------------------------
// Name: SetPosition
// Description: Sets the position.
//-----------------------------------------------------------------------------

void UpDownControl::SetPosition(DWORD pos)
{
    SendMessage(UDM_SETPOS32, 0, (LPARAM)(pos/m_scale));
}

//-----------------------------------------------------------------------------
// Name: GetPosition
// Description: Gets the position.
//-----------------------------------------------------------------------------

DWORD UpDownControl::GetPosition()
{
    return (DWORD)(m_scale * (int)SendMessage(UDM_GETPOS32, 0, 0));
}

