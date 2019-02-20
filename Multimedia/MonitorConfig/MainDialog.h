//-----------------------------------------------------------------------------
// File: MainDialog.h
// Desc: Main dialog for the application.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once 

#include <windows.h>

#include "dialog.h"
#include "resource.h"

#include <PhysicalMonitorEnumerationAPI.h>
#include <HighLevelMonitorConfigurationAPI.h>

const DWORD cColorTemps = 8;  // Number of color temperatures.

class MainDialog : public CBaseDialog
{

public:
	MainDialog();
	~MainDialog();

	HRESULT OnInitDialog(); 

    INT_PTR OnReceiveMsg(UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR OnCommand(HWND hControl, WORD idControl, WORD msg);

    BOOL    OnEnumMonitor(HMONITOR hMonitor);

private:

	void    InitializeControls();
    BOOL    OnSelectMonitor();
    void    OnScroll(HWND hControl, BOOL bThumbTrack);
    void    OnDegauss();
    void    OnRestoreDefaults();
    void    OnSelectColorTemp(MC_COLOR_TEMPERATURE temp);

    void    DisableAllMonitorSettingControls();
    
    BOOL    AllocateMonitorHandles();
    BOOL    FreeMonitorHandles();

    BOOL    UpdateAllMonitorSettings();
	BOOL	UpdateBrightness();
	BOOL	UpdateContrast();
	BOOL	UpdateGain();
    BOOL    UpdateColorTemps();
    BOOL    UpdateDisplayAreaSize();
    BOOL    UpdateDisplayAreaPosition();

    
    HANDLE          m_hMonitor;     // Current monitor handle
    DWORD           m_caps;         // Capability flags for current monitor.
    DWORD           m_colorTemps;   // Supported color temperature for current monitor.

    MC_COLOR_TEMPERATURE    m_LastColorTemperature; // Most recent color temperature

    DWORD                   m_NumPhysicalMonitors;  // Number of physical monitors.
    PHYSICAL_MONITOR        *m_pPhysicalMonitors;   // Array of physical monitor structures.


    // UI controls
	ComboBox	    comboMonitors;

    Button		    buttonDegauss;
    Button		    buttonRestoreDefaults;

    Slider          sliderBrightness;
    Slider          sliderContrast;
	Slider          sliderGainRed;
    Slider          sliderGainGreen;
    Slider          sliderGainBlue;

    Button          buttonColorTemp[cColorTemps];

    UpDownControl   spinHeight;
    UpDownControl   spinWidth;
    UpDownControl   spinVerticalPos;
    UpDownControl   spinHorizontalPos;

    ToolTip         toolTip;

};
