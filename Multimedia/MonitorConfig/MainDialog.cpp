//-----------------------------------------------------------------------------
// File: MainDialog.cpp
// Desc: Main dialog for the application.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "MainDialog.h"
#include "trace.h"
#include <strsafe.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif 

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) { delete x; x = NULL; } }
#endif

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(x)  { if (x) { delete [] x; x = NULL; } }
#endif

#define TRACE_IF_FAILED(FnName, bSuccess) if (!bSuccess) { TRACE((L"%s FAILED (error %x)", FnName, GetLastError())); }


// Application instance handle.
extern HINSTANCE g_hInstance;

// First color temperature value. (Index zero in the array of radio buttons for color temperature.)
const MC_COLOR_TEMPERATURE BaseColorTemp = MC_COLOR_TEMPERATURE_4000K;


BOOL CALLBACK MonitorEnumProc(
  HMONITOR hMonitor,  // handle to display monitor
  HDC hdcMonitor,     // handle to monitor DC
  LPRECT lprcMonitor, // monitor intersection rectangle
  LPARAM dwData       // data
);



/// MainDialog

//-----------------------------------------------------------------------------
// MainDialog constructor
//-----------------------------------------------------------------------------

MainDialog::MainDialog() 
  : CBaseDialog(IDD_MAINDIALOG), 
    m_caps(0), 
    m_colorTemps(0), 
    m_hMonitor((HANDLE)-1),
    m_LastColorTemperature(MC_COLOR_TEMPERATURE_UNKNOWN), 
    m_pPhysicalMonitors(NULL),
    m_NumPhysicalMonitors(0)
{
}


MainDialog::~MainDialog()
{
    FreeMonitorHandles();
}

//-----------------------------------------------------------------------------
// Name: OnInitDialog
// Description: Called when the dialog is created.
//-----------------------------------------------------------------------------

HRESULT MainDialog::OnInitDialog()
{
    BOOL bSuccess = TRUE;

    // Set up the control helper classes.
    InitializeControls();

    // Allocate the handles to the physical monitors.
    bSuccess = AllocateMonitorHandles();

    // Update the UI for the selected monitor.
    if (bSuccess)
    {
        bSuccess = OnSelectMonitor();
    }

    if (!bSuccess)
    {
        ShowLastError(m_hDlg);
    }

    return bSuccess ? S_OK : E_FAIL;
}


//-----------------------------------------------------------------------------
// Name: OnReceiveMsg
// Description: Handles dialog messages.
//
// Note: WM_COMMAND, WM_NOTIFY, and WM_INITDIALOG are handled elsewhere.
//-----------------------------------------------------------------------------

INT_PTR MainDialog::OnReceiveMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    // Scroll message from a slider or up-down control
    case WM_VSCROLL:

        switch (LOWORD(wParam))
        {
        case SB_PAGEUP:
        case SB_PAGEDOWN:
            OnScroll((HWND)lParam, FALSE);  // Scroll, not dragging
            break;

        case SB_THUMBTRACK:
            OnScroll((HWND)lParam, TRUE);   // Scroll with dragging
            break;

        case SB_THUMBPOSITION:
            OnScroll((HWND)lParam, FALSE);
            // The user has released the slider, so deactivate the tool tip.
            toolTip.Activate(FALSE);
            break;

        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// Name: OnCommand
// Description: Handles WM_COMMAND messages.
//-----------------------------------------------------------------------------

INT_PTR MainDialog::OnCommand(HWND hControl, WORD idControl, WORD msg)
{
	switch (idControl)
	{
	case IDC_MONITORS:
		if (msg == CBN_SELCHANGE)
		{
            // User selected a new monitor from the list.
			OnSelectMonitor();
		}
		break;

    case IDC_DEGAUSS:
        OnDegauss();
        break;

    case IDC_RESTORE_DEFAULTS:
        OnRestoreDefaults();
        break;

    // Color temparature radio buttons.
    case IDC_COLORTEMP_4000:
    case IDC_COLORTEMP_5000:
    case IDC_COLORTEMP_6500:
    case IDC_COLORTEMP_7500:
    case IDC_COLORTEMP_8200:
    case IDC_COLORTEMP_9300:
    case IDC_COLORTEMP_10000:
    case IDC_COLORTEMP_11500:
        OnSelectColorTemp((MC_COLOR_TEMPERATURE)(idControl - IDC_COLORTEMP_BASE + 1));
        break;
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////
// Name: InitializeControls
//
// Description: Associates the control helper classes with the 
// correct control windows.
///////////////////////////////////////////////////////////////////////

void MainDialog::InitializeControls()
{
    // Set windows for all the controls.
    SetControlWindow(comboMonitors, IDC_MONITORS);
    SetControlWindow(buttonDegauss, IDC_DEGAUSS);
    SetControlWindow(buttonRestoreDefaults, IDC_RESTORE_DEFAULTS);
    SetControlWindow(sliderBrightness, IDC_BRIGHTNESS);
    SetControlWindow(sliderContrast, IDC_CONTRAST);
    SetControlWindow(sliderGainRed, IDC_GAIN_RED);
    SetControlWindow(sliderGainGreen, IDC_GAIN_GREEN);
    SetControlWindow(sliderGainBlue, IDC_GAIN_BLUE);
    SetControlWindow(spinHeight, IDC_SPIN_VSIZE);
    SetControlWindow(spinWidth, IDC_SPIN_HSIZE);
    SetControlWindow(spinVerticalPos, IDC_SPIN_VPOS);
    SetControlWindow(spinHorizontalPos, IDC_SPIN_HPOS);

    for (DWORD i = 0; i < cColorTemps; i++)
    {
        SetControlWindow(buttonColorTemp[i], i + IDC_COLORTEMP_BASE);
    }

    // Set buddy windows for the spinners.
    spinHeight.SetBuddyWindow(GetDlgItem(IDC_EDIT_VSIZE));
    spinWidth.SetBuddyWindow(GetDlgItem(IDC_EDIT_HSIZE));

    spinVerticalPos.SetBuddyWindow(GetDlgItem(IDC_EDIT_VPOS));
    spinHorizontalPos.SetBuddyWindow(GetDlgItem(IDC_EDIT_HPOS));

    // Initialize the tool tip for the sliders.
    toolTip.Initialize(m_hDlg);


    // Disable all monitor setting controls by default. We will 
    // re-enable the controls for the settings that the monitor 
    // supports, if any.
    DisableAllMonitorSettingControls();
}


///////////////////////////////////////////////////////////////////////
// Name: DisableAllControls
//
// Description: Disables all of the controls that relate to monitor 
// settings.
///////////////////////////////////////////////////////////////////////

void MainDialog::DisableAllMonitorSettingControls()
{
    sliderBrightness.Enable(FALSE);
    sliderContrast.Enable(FALSE);
    sliderGainRed.Enable(FALSE);
    sliderGainGreen.Enable(FALSE);
    sliderGainBlue.Enable(FALSE);
    spinHeight.Enable(FALSE);
    spinWidth.Enable(FALSE);
    spinVerticalPos.Enable(FALSE);
    spinHorizontalPos.Enable(FALSE);

    for (DWORD i = 0; i < cColorTemps; i++)
    {
        buttonColorTemp[i].Enable(FALSE);
    }
}



///////////////////////////////////////////////////////////////////////
// Name: UpdateAllMonitorSettings
//
// Description: 
// Update the UI to reflect the current monitor settings.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateAllMonitorSettings()
{

    // NOTE: If any of these sub-tasks fails, we log the failure but
    // otherwise ignore the error. That way, if one setting fails,
    // we still attempt the other settings. 

    UpdateBrightness();
    UpdateContrast();
    UpdateDisplayAreaSize();
    UpdateDisplayAreaPosition();
	UpdateGain();
    UpdateColorTemps();

    return true;
}



///////////////////////////////////////////////////////////////////////
// Name: UpdateBrightness
//
// Description: Update the UI to reflect the monitor brightness.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateBrightness()
{
    DWORD minVal = 0, maxVal = 0, current = 0;
    BOOL bSuccess = TRUE;

    if (m_caps & MC_CAPS_BRIGHTNESS)
    {
        bSuccess = GetMonitorBrightness(m_hMonitor, &minVal, &current, &maxVal);
        TRACE_IF_FAILED(L"GetMonitorBrightness", bSuccess);

        if (bSuccess)
        {
            TRACE((L"Brightness: %d (%d - %d)", current, minVal, maxVal));

            sliderBrightness.Enable(TRUE);
            sliderBrightness.SetRangeAndPosition(maxVal, minVal, current);
        }
    }
    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: UpdateContrast
//
// Description: Update the UI to reflect the monitor contrast.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateContrast()
{
    DWORD minVal = 0, maxVal = 0, current = 0;
    BOOL bSuccess = TRUE;

    if (m_caps & MC_CAPS_CONTRAST)
    {
        bSuccess = GetMonitorContrast(m_hMonitor, &minVal, &current, &maxVal);
        TRACE_IF_FAILED(L"GetMonitorContrast", bSuccess);

        if (bSuccess)
        {
            TRACE((L"Contrast: %d (%d - %d)", current, minVal, maxVal));

            sliderContrast.Enable(TRUE);
            sliderContrast.SetRangeAndPosition(maxVal, minVal, current);
        }
    }
    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: UpdateGain
//
// Description: Update the UI to reflect the monitor gain settings.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateGain()
{
    DWORD minVal = 0, maxVal = 0, current = 0;
    BOOL bSuccess = TRUE;

    MC_GAIN_TYPE gain[] = { MC_RED_GAIN, MC_GREEN_GAIN, MC_BLUE_GAIN };

    Slider* slider[] = { &sliderGainRed, &sliderGainGreen, &sliderGainBlue };

    if (m_caps & MC_CAPS_RED_GREEN_BLUE_GAIN)
    {
        for (DWORD i = 0; i < ARRAY_SIZE(gain); i++)
        {
            bSuccess = GetMonitorRedGreenOrBlueGain(m_hMonitor, gain[i], 
                &minVal, &current, &maxVal);

            TRACE_IF_FAILED(L"GetMonitorRedGreenOrBlueGain", bSuccess);

            if (bSuccess)
            {
                slider[i]->Enable(TRUE);
                slider[i]->SetRangeAndPosition(maxVal, minVal, current);
            }
            else
            {
                break;
            }
        }
    }

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: UpdateColorTemps
//
// Description: Update the UI to reflect the monitor color temperature.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateColorTemps()
{
    BOOL bSuccess = TRUE;
    MC_COLOR_TEMPERATURE colorTemp = MC_COLOR_TEMPERATURE_UNKNOWN;

    DWORD ColorTemps[] = 
    {
        MC_SUPPORTED_COLOR_TEMPERATURE_4000K,
        MC_SUPPORTED_COLOR_TEMPERATURE_5000K, 
        MC_SUPPORTED_COLOR_TEMPERATURE_6500K, 
        MC_SUPPORTED_COLOR_TEMPERATURE_7500K, 
        MC_SUPPORTED_COLOR_TEMPERATURE_8200K, 
        MC_SUPPORTED_COLOR_TEMPERATURE_9300K, 
        MC_SUPPORTED_COLOR_TEMPERATURE_10000K,
        MC_SUPPORTED_COLOR_TEMPERATURE_11500K 
    };

    if (m_caps & MC_CAPS_COLOR_TEMPERATURE)
    {
        // Enable the radio buttons that match the supported color temperatures,
        // and disable all the others. Uncheck all of the buttons.

        for (DWORD i = 0; i < cColorTemps; i++)
        {
            buttonColorTemp[i].SetCheck(BST_UNCHECKED);

            if (m_colorTemps & ColorTemps[i])
            {
                buttonColorTemp[i].Enable(TRUE);
            }
            else
            {
                buttonColorTemp[i].Enable(FALSE);
            }
        }

        // Get the current color temperature.
        bSuccess = GetMonitorColorTemperature(m_hMonitor, &colorTemp);
        TRACE_IF_FAILED(L"GetMonitorColorTemperature", bSuccess);

        if (bSuccess)
        {
            if (colorTemp != MC_COLOR_TEMPERATURE_UNKNOWN)
            {
                assert(colorTemp - BaseColorTemp >= 0);
                assert(colorTemp - BaseColorTemp <= cColorTemps);

                // Check the button that matches the new color temperature. 

                buttonColorTemp[colorTemp - BaseColorTemp].SetCheck(BST_CHECKED);

                m_LastColorTemperature = colorTemp; // Cache this value.
            }
        }

    }

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: UpdateDisplayAreaSize
//
// Description: Update the UI to reflect the monitor display area size.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateDisplayAreaSize()
{
    DWORD minVal = 0, maxVal = 0, current = 0;
    BOOL bSuccess = TRUE;

    if (m_caps & MC_CAPS_DISPLAY_AREA_SIZE)
    {
        // Get display area height.
        bSuccess = GetMonitorDisplayAreaSize(m_hMonitor, MC_HEIGHT, &minVal, &current, &maxVal);
        TRACE_IF_FAILED(L"GetMonitorDisplayAreaSize(MC_HEIGHT)", bSuccess);
        
        // Set the spin control.
        if (bSuccess)
        {
            spinHeight.Enable(TRUE);
            spinHeight.SetRange(minVal, maxVal);
            spinHeight.SetPosition(current);
        }


        // Get the display area width.
        if (bSuccess)
        {
            bSuccess = GetMonitorDisplayAreaSize(m_hMonitor, MC_WIDTH, &minVal, &current, &maxVal);
            TRACE_IF_FAILED(L"GetMonitorDisplayAreaSize(MC_WIDTH)", bSuccess);
        }

        // Set the spin control.
        if (bSuccess)
        {
            spinWidth.Enable(TRUE);
            spinWidth.SetRange(minVal, maxVal);
            spinWidth.SetPosition(current);
        }
    }

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: UpdateDisplayAreaPosition
//
// Description: Update the UI to reflect the monitor display area 
//              position.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::UpdateDisplayAreaPosition()
{
    DWORD minVal = 0, maxVal = 0, current = 0;
    BOOL bSuccess = TRUE;

    if (m_caps & MC_CAPS_DISPLAY_AREA_POSITION )
    {
        // Get vertical position.
        bSuccess = GetMonitorDisplayAreaPosition(m_hMonitor, MC_VERTICAL_POSITION, &minVal, &current, &maxVal);
        TRACE_IF_FAILED(L"GetMonitorDisplayAreaPosition(MC_VERTICAL_POSITION)", bSuccess);
        
        // Set the spin control.
        if (bSuccess)
        {
            spinVerticalPos.Enable(TRUE);
            spinVerticalPos.SetRange(minVal, maxVal);
            spinVerticalPos.SetPosition(current);
        }

        // Get the horizontal position.
        if (bSuccess)
        {
            bSuccess = GetMonitorDisplayAreaPosition(m_hMonitor, MC_HORIZONTAL_POSITION, &minVal, &current, &maxVal);
            TRACE_IF_FAILED(L"GetMonitorDisplayAreaPosition(MC_HORIZONTAL_POSITION)", bSuccess);
        }

        // Set the spin control.
        if (bSuccess)
        {
            spinHorizontalPos.Enable(TRUE);
            spinHorizontalPos.SetRange(minVal, maxVal);
            spinHorizontalPos.SetPosition(current);
        }
    }

    return bSuccess;
}




///////////////////////////////////////////////////////////////////////
// Name: AllocateMonitorHandles
// Description: Get handles to the physical monitors.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::AllocateMonitorHandles()
{
    // Enumerate the display monitors. The callback will do the work.
    return EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
}


///////////////////////////////////////////////////////////////////////
// Name: MonitorEnumProc
// Description: Callback for EnumDisplayMonitors function.
//
// The callback data is a pointer to the MainDialog class.
///////////////////////////////////////////////////////////////////////

BOOL CALLBACK MonitorEnumProc(
  HMONITOR hMonitor,  // handle to display monitor
  HDC hdcMonitor,     // handle to monitor DC
  LPRECT lprcMonitor, // monitor intersection rectangle
  LPARAM dwData       // data
)
{
    MainDialog *pDialog = (MainDialog*)dwData;
    BOOL bContinue = FALSE;

    if (pDialog)
    {
        bContinue = pDialog->OnEnumMonitor(hMonitor);
    }
    return bContinue;
}




///////////////////////////////////////////////////////////////////////
// Name: OnEnumMonitor
// Description: Callback for EnumDisplayMonitors function.
//
// This function is where we get the physical monitor handles.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::OnEnumMonitor(HMONITOR hMonitor)
{
    TRACE((L"OnEnumMonitor: Handle = 0x%X", hMonitor));

    BOOL bSuccess = FALSE;
    DWORD cMonitors = 0;
    PHYSICAL_MONITOR *pPhysicalMonitors = NULL;

    // Free any monitor handles that we allocated previously.
    FreeMonitorHandles();

    // Also clear the list of monitors displayed in the UI.
    comboMonitors.ClearItems();

    // Get the number of physical monitors.
    bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(
        hMonitor, 
        &cMonitors
        );

    TRACE_IF_FAILED(L"GetNumberOfPhysicalMonitorsFromHMONITOR", bSuccess);

    // Allocate the array of PHYSICAL_MONITOR structs.
    if (bSuccess)
    {
        pPhysicalMonitors = new PHYSICAL_MONITOR[cMonitors];

        if (pPhysicalMonitors == NULL)
        {
            bSuccess = FALSE;
        }
    }

    // Get the array of monior handles.
    if (bSuccess)
    {
        bSuccess = GetPhysicalMonitorsFromHMONITOR(
            hMonitor, cMonitors, pPhysicalMonitors);

        TRACE_IF_FAILED(L"GetPhysicalMonitorsFromHMONITOR", bSuccess);
    }

    if (bSuccess)
    {
        m_pPhysicalMonitors = pPhysicalMonitors;
        m_NumPhysicalMonitors = cMonitors;
    }

    // Add the display string to the combo box.
    // Also add the index of the array as user data. 
    if (bSuccess)
    {
        for (DWORD i = 0; i < cMonitors; i++)
        {
            TRACE((L"Physical monitor: %s (handle = 0x%X)", 
                pPhysicalMonitors[i].szPhysicalMonitorDescription,
                pPhysicalMonitors[i].hPhysicalMonitor
                ));

            bSuccess = comboMonitors.AddString(
                pPhysicalMonitors[i].szPhysicalMonitorDescription,  // string 
                (DWORD_PTR)i    // user data = array index
                );

            if (!bSuccess)
            {
                break;
            }
        }
    }

    // Select the first one in the list.
    if (bSuccess)
    {
        comboMonitors.Select(0);
    }

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: FreeMonitorHandles
//
// Description: Release the array of monitor handles.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::FreeMonitorHandles()
{
    BOOL bSuccess = TRUE;

    if (m_NumPhysicalMonitors > 0)
    {
        assert(m_pPhysicalMonitors != NULL);

        bSuccess = DestroyPhysicalMonitors(m_NumPhysicalMonitors, m_pPhysicalMonitors);
    }

    SAFE_ARRAY_DELETE(m_pPhysicalMonitors);

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: OnSelectMonitor
//
// Description: Called when the user selects a new monitor from the
//              combo box.
///////////////////////////////////////////////////////////////////////

BOOL MainDialog::OnSelectMonitor()
{
    BOOL bSuccess = TRUE;
    HANDLE hMonitor = NULL;
    DWORD index = 0;

    TRACE((L"Current combo selection: %d", (int)comboMonitors.SendMessage(CB_GETCURSEL, 0, 0)));


    // The user data for the combo box is the index into the array of monitor handles.
    bSuccess = comboMonitors.GetCurrentSelectionItemData((DWORD_PTR*)&index);
    if (bSuccess)
    {
        assert(index < m_NumPhysicalMonitors);

        if (index < m_NumPhysicalMonitors)
        {
            hMonitor = m_pPhysicalMonitors[index].hPhysicalMonitor;
        }
        else
        {
            TRACE((L"Error: The selected monitor handle was alreadt released (index = %d)", index));
            bSuccess = FALSE;
        }
    }

    if (bSuccess)
    {
        TRACE((L"Selected monitor: 0x%X", hMonitor));

        // If the selection changed, update the settings.

        if (hMonitor != m_hMonitor)
        {
            m_hMonitor = hMonitor;

            //// Get the monitor capabilities.
            bSuccess = GetMonitorCapabilities(m_hMonitor, &m_caps, &m_colorTemps);
            TRACE_IF_FAILED(L"GetMonitorCapabilities", bSuccess);

            if (!bSuccess)
            {
                // If GetMonitorCapabilities fails, it probably means the monitor
                // does not support DDC/CI. In that case, display an error to
                // the user now, but do not treat it as a fatal error. (For example,
                // maybe there is another physical monitor that does support DDC/CI.)

                ShowLastError(m_hDlg);

                m_caps = 0;
                m_colorTemps = 0;
                bSuccess = TRUE;
            }
            else
            {
                bSuccess = UpdateAllMonitorSettings();
            }
        }
    }
    return bSuccess;
}

///////////////////////////////////////////////////////////////////////
// Name: OnDegauss
// Description: Degausses the monitor.
///////////////////////////////////////////////////////////////////////

void MainDialog::OnDegauss()
{
    BOOL bSuccess = TRUE;

    bSuccess = DegaussMonitor(m_hMonitor);

    TRACE_IF_FAILED(L"DegaussMonitor", bSuccess);

    // Result of a degauss operation may not be obvious, so give some
    // feedback...
    if (bSuccess)
    {
        MessageBox(m_hDlg, L"Your monitor has been degaussed.", L"Success", MB_OK );
    }
    else
    {
        ShowLastError(m_hDlg);
    }

}

///////////////////////////////////////////////////////////////////////
// Name: OnRestoreDefaults
// Description: Restores the monitor's default settings.
///////////////////////////////////////////////////////////////////////

void MainDialog::OnRestoreDefaults()
{
    BOOL bSuccess = TRUE;

    bSuccess = RestoreMonitorFactoryDefaults(m_hMonitor);
    TRACE_IF_FAILED(L"RestoreMonitorFactoryDefaults", bSuccess);

    if (bSuccess)
    {
        UpdateAllMonitorSettings();
    }
}


///////////////////////////////////////////////////////////////////////
// Name: OnSelectColorTemp
// Description: Sets the monitor color temperature.
///////////////////////////////////////////////////////////////////////

void MainDialog::OnSelectColorTemp(MC_COLOR_TEMPERATURE temperature)
{
    BOOL bSuccess = TRUE;

    bSuccess = SetMonitorColorTemperature(m_hMonitor, temperature);
    TRACE_IF_FAILED(L"SetMonitorColorTemperature", bSuccess);

    if (bSuccess)
    {
        // Uncheck the previous temperature
        if (m_LastColorTemperature > MC_COLOR_TEMPERATURE_UNKNOWN)
        {
            buttonColorTemp[m_LastColorTemperature - BaseColorTemp].SetCheck(BST_UNCHECKED);
        }

        m_LastColorTemperature = temperature;

        // Changing the color temperature can affect the gain setting.
        UpdateGain();
    }
}



///////////////////////////////////////////////////////////////////////
// Name: OnScroll
//
// Description: Called when the user changes the position of a
//              trackbar control or up-down control.
///////////////////////////////////////////////////////////////////////


void MainDialog::OnScroll(HWND hControl, BOOL bThumbTrack)
{
    BOOL bSuccess = TRUE;
    BOOL bColorChanged = FALSE;

    // Match the control window to the monitor setting.
    if (hControl == sliderContrast.Window())
    {
        sliderContrast.ShowTip(toolTip);
        bSuccess = SetMonitorContrast(m_hMonitor, sliderContrast.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorContrast", bSuccess);
    }
    else if (hControl == sliderBrightness.Window())
    {
        if (bThumbTrack)
        {
            sliderBrightness.ShowTip(toolTip);
        }

        bSuccess = SetMonitorBrightness(m_hMonitor, sliderBrightness.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorBrightness", bSuccess);
    }
    else if (hControl == sliderGainRed.Window())
    {
        if (bThumbTrack)
        {
            sliderGainRed.ShowTip(toolTip);
        }

        bSuccess = SetMonitorRedGreenOrBlueGain(m_hMonitor, MC_RED_GAIN, sliderGainRed.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorRedGreenOrBlueGain", bSuccess);

        if (bSuccess)
        {
            bColorChanged = TRUE;
        }
    }
    else if (hControl == sliderGainGreen.Window())
    {
        if (bThumbTrack)
        {
            sliderGainGreen.ShowTip(toolTip);
        }
        
        bSuccess = SetMonitorRedGreenOrBlueGain(m_hMonitor, MC_GREEN_GAIN, sliderGainGreen.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorRedGreenOrBlueGain", bSuccess);

        if (bSuccess)
        {
            bColorChanged = TRUE;
        }
    }
    else if (hControl == sliderGainBlue.Window())
    {
        if (bThumbTrack)
        {
            sliderGainBlue.ShowTip(toolTip);
        }

        bSuccess = SetMonitorRedGreenOrBlueGain(m_hMonitor, MC_BLUE_GAIN, sliderGainBlue.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorRedGreenOrBlueGain", bSuccess);

        if (bSuccess)
        {
            bColorChanged = TRUE;
        }
    }
    else if (hControl == spinHeight.Window())
    {
        bSuccess = SetMonitorDisplayAreaSize(m_hMonitor, MC_HEIGHT, spinHeight.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorDisplayAreaSize", bSuccess);
    }
    else if (hControl == spinWidth.Window())
    {
        bSuccess = SetMonitorDisplayAreaSize(m_hMonitor, MC_WIDTH, spinWidth.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorDisplayAreaSize", bSuccess);
    }
    else if (hControl == spinVerticalPos.Window())
    {
        bSuccess = SetMonitorDisplayAreaPosition(m_hMonitor, MC_VERTICAL_POSITION, spinVerticalPos.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorDisplayAreaPosition", bSuccess);
    }
    else if (hControl == spinHorizontalPos.Window())
    {
        TRACE((L"SetMonitorDisplayAreaPosition %d", spinHorizontalPos.GetPosition()));
        bSuccess = SetMonitorDisplayAreaPosition(m_hMonitor, MC_HORIZONTAL_POSITION, spinHorizontalPos.GetPosition());
        TRACE_IF_FAILED(L"SetMonitorDisplayAreaPosition", bSuccess);
    }

    if (bSuccess && bColorChanged && !bThumbTrack)
    {
        // Changing the gain can affect the color temperature. Update the UI to reflect the new color.
        // (However, don't update it while the user is still dragging the scroll bar (bThumbTrack) because it
        // will make the UI jumpy....)
        UpdateColorTemps();
    }

}





