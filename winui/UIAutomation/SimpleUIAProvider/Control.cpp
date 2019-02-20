/*************************************************************************************************
 *
 * File: Control.cpp
 *
 * Description: Implements a simple custom control that supports UI Automation.
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
#include "Control.h"


// Forward declarations.
LRESULT CALLBACK ControlWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

/**** CustomButton methods ***/

// Constructor.

CustomButton::CustomButton()
{
    CustomButton::m_buttonOn = false;
    CustomButton::m_provider = NULL;
    m_InvokedEventId = UiaLookupId(AutomationIdentifierType_Event, &Invoke_Invoked_Event_GUID);
}

// Destructor. Release the provider interface.

CustomButton::~CustomButton()
{
    IRawElementProviderSimple* pRootProvider = this->GetUIAutomationProvider(NULL);
    if (pRootProvider != NULL)
    {
        pRootProvider->Release();  
    }
}

IRawElementProviderSimple* CustomButton::GetUIAutomationProvider(HWND hwnd)
{
    // If it exists, or if being called from destructor, don't create.
    if ((m_provider == NULL) && (hwnd != NULL))
    {
        m_provider = new Provider(hwnd);
    }
    return m_provider;
}


// Handle button click or invoke.

void CustomButton::InvokeButton(HWND hwnd)
{
    m_buttonOn = ! m_buttonOn;
    SetFocus(hwnd);
    if (UiaClientsAreListening())
    {
        // Retrieve the provider for this instance.
        IRawElementProviderSimple* pProvider = (IRawElementProviderSimple*)(LONG_PTR)GetWindowLongPtr(hwnd, sizeof(LONG_PTR));
        // Raise an event.
        UiaRaiseAutomationEvent(pProvider, m_InvokedEventId);
    }
    InvalidateRect(hwnd, NULL, true);
}

// Ascertain whether button is in the "on" state.

bool CustomButton::IsButtonOn()
{
    return m_buttonOn;
}

// Register the control class.

void CustomButton::RegisterControl(HINSTANCE hInstance)
{
    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = ControlWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof(LONG_PTR) * 2;  // Make room for two pointers.
    wc.hInstance        = hInstance;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = L"COLORBUTTON";

    RegisterClass(&wc);
}

// Control window procedure.

LRESULT CALLBACK ControlWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
    case WM_CREATE:
        {
            CustomButton* pControl = new CustomButton();
            if (pControl == NULL)
            {
                PostQuitMessage(-1);
            }
            // Save the class instance as extra window data so that members can be accessed
            //  from within this function.
            SetWindowLongPtr(hwnd, 0, (LONG)(LONG_PTR)pControl);
            break;
        }

    case WM_DESTROY:
        {
            CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            delete pControl;
            break;
        }


        // Register with UI Automation.
    case WM_GETOBJECT:
        {
            // If the lParam matches the RootObjectId, send back the RawElementProvider
            if ( (long)lParam == (long)UiaRootObjectId )
            {
                CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
                IRawElementProviderSimple* pRootProvider = pControl->GetUIAutomationProvider(hwnd);
                return UiaReturnRawElementProvider(hwnd, wParam, lParam, pRootProvider);
            }
            return 0;
        }

    case WM_PAINT:
        {
            CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            COLORREF color;
            if (pControl->IsButtonOn())
            {
                color = RGB(128, 0, 0);
            }
            else
            {
                color = RGB(0, 128, 0);
            }
            HDC hdc = GetDC(hwnd);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Shrink the colored rectangle so the focus rectangle will be outside it.
            InflateRect(&clientRect, -4, -4);

            // Paint the rectangle.
            HBRUSH brush = CreateSolidBrush(color);
            if (brush == NULL) 
            {
                ReleaseDC(hwnd, hdc);
                break;
            }
            FillRect(hdc, &clientRect, brush);

            // Clean up.
            DeleteObject(brush);
            ReleaseDC(hwnd, hdc);
            break;
        }

    case WM_SETFOCUS:
        {
            CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            HDC hdc = GetDC(hwnd);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            DrawFocusRect(hdc, &clientRect); 
            ReleaseDC(hwnd, hdc);
            break;
        }
    case WM_KILLFOCUS:
        {
            CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            HDC hdc = GetDC(hwnd);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            DrawFocusRect(hdc, &clientRect); // Erases focus rect if there's one there.
            ReleaseDC(hwnd, hdc);
            break;
        }

    case WM_LBUTTONDOWN:
        {
            CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
            pControl->InvokeButton(hwnd);
            break;
        }

    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_SPACE:
                CustomButton* pControl = (CustomButton*)(LONG_PTR)GetWindowLongPtr(hwnd, 0); 
                pControl->InvokeButton(hwnd);
                break;
            }
            break;
        }
        break;
    }  // switch (message)

    return DefWindowProc(hwnd, message, wParam, lParam);
}
