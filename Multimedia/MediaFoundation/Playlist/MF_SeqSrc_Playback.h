//////////////////////////////////////////////////////////////////////////
//
// MF_SeqSrc_Playback.h : Global header.
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////


#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.

#ifndef WINVER              // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400       // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400     // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif                      

#ifndef _WIN32_WINDOWS      // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE           // Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400    // Change this to the appropriate value to target IE 5.0 or later.
#endif

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <cderr.h>
#include <assert.h>

#include <tchar.h>
#include <commdlg.h> // OpenFile dialog
#include <strsafe.h>
#include <commctrl.h>


#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mftransform.h>
#include <mferror.h>
//#include <mfpropkey.h>

#define USE_LOGGING

#include "Utils.h"
using namespace MediaFoundationSamples;


enum PlayerState
{
    PlayerCreated,
    Initialized,
    Playing,
    Paused,
    Stopped,
};

struct TimePair
{
	MFTIME hnsStartPresentationTime;
	MFTIME hnsPresentationTimeOffset;
	TimePair *pNextTimePair;
};

struct TopologyStatusInfo
{
	int iTopologyStatusType;
	int iSegmentId;
};

//  Application events.
#define WM_NOTIFY_APP   WM_APP + 1                              // Notify the application

//Application notifications. 
#define PLAYER_INITIALIZED          WM_NOTIFY_APP + 1           // Player initialized.
#define PLAYER_SEGMENT_DELETED      PLAYER_INITIALIZED + 1      //Topology deleted from the sequencer
#define PLAYER_SEGMENT_ADDED        PLAYER_SEGMENT_DELETED + 1  //Topology added to the sequencer

//UI Strings
#define MAX_STRING_SIZE     260
#define MAX_TIME_STRING     15
#define MAX_STATE_STRING    50

