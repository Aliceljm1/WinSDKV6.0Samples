/*************************************************************************************************
 *
 * File: Control.h
 *
 * Description: Declaration of the CustomButton class.
 * 
 * See Window.cpp for a full description of this sample.
 *   
 *
 *  Copyright (C) Microsoft Corporation.  All rights reserved.
 * 
 * This source code is intended only as a supplement to Microsoft
 * Development Tools and/or on-line documentation.  See these other
 * materials for detailed information regarding Microsoft code samples.
 * 
 * THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 * 
 *************************************************************************************************/
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>

#include <ole2.h>
#include <UIAutomationCore.h>
#include <UIAutomationCoreAPI.h>

#include "Provider.h"


class CustomButton //: IUnknown
{
public:
    CustomButton();
    virtual ~CustomButton();
    IRawElementProviderSimple* GetUIAutomationProvider(HWND hwnd);
    bool IsButtonOn();
    void InvokeButton(HWND hwnd);
    static void RegisterControl(HINSTANCE hInstance);

private:
    bool m_buttonOn;
    IRawElementProviderSimple* m_provider;
    int m_InvokedEventId;
    HWND controlHwnd;



};