//////////////////////////////////////////////////////////////////////////
// Tooltip.h: Tooltip control class.
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


class ToolTip : public Control
{
public:
    HRESULT Create(HWND hParent);

    BOOL    AddTool(HWND hControl, LPTSTR szText);
};