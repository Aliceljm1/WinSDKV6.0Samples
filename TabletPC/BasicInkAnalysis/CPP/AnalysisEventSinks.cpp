// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Module:       
//        AnalysisEventSinks.cpp
//
// Description:
//        Implementation for the Analysis Event Sinks
//-----------------------------------------------------------------------
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

// Windows header files
#include <windows.h>
#include <commctrl.h>       // need it to call CreateStatusWindow

// The following definitions may be not found in the old headers installed with VC6, 
// so they're copied from the newer headers found in the Microsoft Platform SDK


#ifndef MIIM_STRING
#define MIIM_STRING      0x00000040
#endif
#ifndef MIIM_FTYPE
#define MIIM_FTYPE       0x00000100
#endif

// A useful macro to determine the number of elements in the array
#define countof(array)  (sizeof(array)/sizeof(array[0]))

// ATL header files
#include <atlbase.h>        // defines CComModule, CComPtr, CComVariant
extern CComModule _Module;
#include <atlwin.h>         // defines CWindowImpl
#include <atlcom.h>         // defines IDispEventSimpleImpl

// Headers for Tablet PC Automation interfaces
#include <msinkaut.h>
#include <tpcerror.h>

// Headers for the Tablet PC Ink Analysis interfaces
#include <IACom.h>

// The application header files
#include "resource.h"       // main symbols, including command ID's 
#include "EventSinks.h"     // defines the IInkEventsImpl and IInkRecognitionEventsImpl 
#include "ChildWnds.h"      // definitions of the CInkInputWnd and CRecoOutputWnd

#include "AnalysisEventSinks.h"
#include "BasicInkAnalysis.h"

STDMETHODIMP InkAnalyzerEvents:: FinalConstruct()
{
    m_pIConnectionPointAnalysisEvents = NULL;
    m_pBasicInkAnalysisApp = NULL;
    return S_OK;
}

 
void InkAnalyzerEvents::FinalRelease()
{
    m_pIConnectionPointAnalysisEvents = NULL;
    m_pBasicInkAnalysisApp = NULL;
}

 
STDMETHODIMP InkAnalyzerEvents::ResultsUpdated(
    IInkAnalyzer *pInkAnalyzer,
    IAnalysisStatus *pAnalysisStatus)
{

    return m_pBasicInkAnalysisApp->OnResultsUpdated(
        pInkAnalyzer,
        pAnalysisStatus);
}

 
STDMETHODIMP InkAnalyzerEvents::IntermediateResultsUpdated(
    IInkAnalyzer *pInkAnalyzer,
    IAnalysisStatus *pAnalysisStatus)
{
    
    return m_pBasicInkAnalysisApp->OnIntermediateResultsUpdated(
        pInkAnalyzer,
        pAnalysisStatus);
}

 
STDMETHODIMP InkAnalyzerEvents::Activity()
{
   
    return m_pBasicInkAnalysisApp->OnAnalyzerActivity();
}

STDMETHODIMP InkAnalyzerEvents::ReadyToReconcile()
{   
    return m_pBasicInkAnalysisApp->OnReadyToReconcile();
}
 
STDMETHODIMP InkAnalyzerEvents::UpdateStrokesCache(
    ULONG ulStrokeIdsCount,
    LONG *plStrokeIds )
{  
    return m_pBasicInkAnalysisApp->OnUpdateStrokeCache(
        ulStrokeIdsCount,
        plStrokeIds );
}


