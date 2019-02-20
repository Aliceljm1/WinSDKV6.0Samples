// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//  This source code is only intended as a supplement to the
//  Microsoft Tablet PC Platform SDK Reference and related electronic 
//  documentation provided with the Software Development Kit.
//  See these sources for more detailed information. 
//
// Module:       
//      BasicInkAnalysis.h
//
// Description:
//      The header file for the CBasicInkAnalysis class - the application window 
//      class of the BasicInkAnalysis sample.
//		The methods of the class are defined in the BasicInkAnalysis.cpp file.
//   
//--------------------------------------------------------------------------

#pragma once

#include "AnalysisEventSinks.h"

// Define a message for the analysis reconciliation
#define WM_READYTORECONCILE WM_APP

/////////////////////////////////////////////////////////////////////////////
// CBasicInkAnalysis

class CBasicInkAnalysis : 
    public CWindowImpl<CBasicInkAnalysis>,
    public IInkOverlayEventsImpl<CBasicInkAnalysis>
{
public:
    // Constants 

    enum { 
        // submenus indices
        mc_iSubmenuInk = 0, 
        mc_iSubmenuModes = 1,
    };

    enum {
        // child windows IDs
        mc_iInputWndId = 1, 
        mc_iOutputWndId = 2, 
        mc_iStatusWndId = 3,
    };

    // Automation API interface pointers
    CComPtr<IInkOverlay>            m_spIInkCollector;
    CComPtr<IInkDisp>               m_spIInkDisp;
    CComPtr<IInkStrokes>            m_spIInkStrokes;
    CComPtr<IInkAnalyzer>           m_spIInkAnalyzer;

    CComObject<InkAnalyzerEvents>  *m_pInkAnalyzerEvents;

    // Child windows
    CInkInputWnd    m_wndInput;
    CRecoOutputWnd  m_wndResults;
    HWND            m_hwndStatusBar;

    // Helper data members
    UINT            m_nCmdMode;
    bool            m_fAutomaticLayoutAnalysis;
    bool            m_fRotatedBoundingBox;

    // Static method that creates an object of the class
    static int Run(int nCmdShow);

    // Constructor
    CBasicInkAnalysis() :
    m_hwndStatusBar(NULL),
        m_nCmdMode(0),
        m_fAutomaticLayoutAnalysis(true),
        m_fRotatedBoundingBox(false)
    {
    }

    // Helper methods
    HMENU   LoadMenu();
    bool    CreateChildWindows();
    void    UpdateLayout();
    void    UpdateMenuRadioItems(UINT iSubMenu, UINT idCheck, UINT idUncheck);
    void    UpdateStatusBar();


    // Declare the class objects' window class with NULL background.
    // There's no need to paint CBasicInkAnalysis window background because
    // the entire client area is covered by the child windows.
    DECLARE_WND_CLASS_EX(NULL, 0, -1)

    // ATL macro's to declare which commands/messages the class is interested in.
    BEGIN_MSG_MAP(CBasicInkAnalysis)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        COMMAND_ID_HANDLER(ID_MODE_INK, OnMode)
        COMMAND_ID_HANDLER(ID_MODE_ERASE, OnMode)
        COMMAND_ID_HANDLER(ID_MODE_AUTOMATICLAYOUTANALYSIS, OnMode)
        COMMAND_ID_HANDLER(ID_MODE_ROTATEDBOUNDINGBOXES, OnMode)
        COMMAND_ID_HANDLER(ID_ANALYZE, OnAnalyze)
        COMMAND_ID_HANDLER(ID_EXIT, OnExit)
        MESSAGE_HANDLER(WM_READYTORECONCILE, OnReconcile)
    END_MSG_MAP()

public:

    // Window message handlers
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetFocus(UINT, WPARAM, LPARAM, BOOL& bHandled);
    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);

    // Command handlers
    LRESULT OnRecognizer(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnAnalyze(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    // Reconciliation handler
    LRESULT OnReconcile(UINT, WPARAM, LPARAM, BOOL& bHandled);

    // Ink collector event handler
    HRESULT OnStroke(IInkCursor* pIInkCursor, IInkStrokeDisp* pIInkStroke, 
        VARIANT_BOOL* pbCancel);
    HRESULT OnGesture(IInkCursor* pIInkCursor, IInkStrokes* pIInkStrokes, 
        VARIANT vGestures, VARIANT_BOOL* pbCancel);
    // Ink overlay event handler
    HRESULT OnStrokesDeleting(IInkStrokes *pIInkStrokesToDelete);
    HRESULT OnSelectionResized(IInkRectangle * pRect);
    HRESULT OnSelectionMoved(IInkRectangle * pRect);

    // InkAnalyzer event handler
    HRESULT OnIntermediateResultsUpdated(
        IInkAnalyzer *pInkAnalyzer,
        IAnalysisStatus *pAnalysisStatus);
    HRESULT OnResultsUpdated(
        IInkAnalyzer *pInkAnalyzer,
        IAnalysisStatus *pAnalysisStatus);
    HRESULT OnAnalyzerActivity();
    HRESULT OnReadyToReconcile();
    HRESULT OnUpdateStrokeCache(
        ULONG ulStrokeIdsCount,
        LONG *plStrokeIds );
};

