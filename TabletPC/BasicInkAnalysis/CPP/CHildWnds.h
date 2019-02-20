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
//      ChildWnds.h
//
// Description:
//      This file contains the definitions of the classes CInkInputWnd 
//      and CRecoOutputWnd, which are derived from the ATL's CWindowImpl
//      and used for creating the sample's child windows.
//		The methods of the classes are defined in the ChildWnds.cpp file.
//--------------------------------------------------------------------------

#pragma once

/////////////////////////////////////////////////////////
//                                          
// class CInkInputWnd
// 
// The CInkInputWnd class allows to create a simple window, that 
// may draw itself with lined or boxed guides or with no guides at all.
//
// An object of the class is used in the CBasicInkAnalysis for pen input.
//
/////////////////////////////////////////////////////////

class CInkInputWnd :
    public CWindowImpl<CInkInputWnd>
{

public:

	// Constructor
    CInkInputWnd();

// Declare the objects' window class with NULL background (-1) to avoid flicking
// that happens because of delays between WM_ERASEBKGND and WM_PAINT messages 
// sent to the window. The background will be painted in the WM_PAINT handler.
DECLARE_WND_CLASS_EX(NULL, 0, -1)

// ATL macro's to declare which commands/messages the class is interested in
BEGIN_MSG_MAP(CInkInputWnd)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
END_MSG_MAP()
     
    // WM_PAINT message handler
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void SetInkAnalyzer ( IInkAnalyzer *pInkAnalyzer)
    {
        m_spInkAnalyzer = pInkAnalyzer;
    }
    void SetInkRenderer ( IInkRenderer *pInkRenderer)
    {
        m_spInkRenderer = pInkRenderer;
    }

    HRESULT CInkInputWnd::GetBoxForNode(
        IContextNode *pIContextNode,
        POINT pts[5]);

    void DrawBoundingBoxes(
        HDC hdc,
        IInkAnalyzer *pAnalyzer,
        IInkRenderer *pRenderer);

    void PaintTreeElement(
        HDC hdc,
        IContextNode *pIContextNode, 
        IInkRenderer* pInkRenderer);

    void ConvertHMMToDP(
        HDC hdc,
        IInkRenderer *pRenderer,
        POINT pts[5]);

    void ToggleBoundingBoxStyle();

    void InflatePolygon(LPPOINT pPts, int inflateBy);

private:
    CComPtr<IInkRenderer>   m_spInkRenderer;
    CComPtr<IInkAnalyzer>   m_spInkAnalyzer;
    bool                    m_fRotatedBoundingBox;

};  // class CInkInputWnd



/////////////////////////////////////////////////////////
//                                          
// class CRecoOutputWnd
// 
// The CRecoOutputWnd class allows to create a window, that draws 
// a few lines of text in it.
// 
// An object of the class is used in the CBasicInkAnalysis to output 
// results of the recognition. 
//
/////////////////////////////////////////////////////////

class CRecoOutputWnd :
    public CWindowImpl<CRecoOutputWnd>
{
public:

// Data members

    // Declare the class-wide constants
    enum { 
        mc_iMarginX = 10,		// left offset for text output
        mc_iMarginY = 10, 		// top offset for text output
        mc_iFontHeight = 20 	// height of the fonts for text output
    };
    
    // Array of strings to draw out
    CComBSTR    m_bstrResults;

    // The current font object to draw text with
    HFONT   m_hFont;
    int     m_iFontName;
    UINT    m_nGesture;
    bool    m_bNewGesture;

// Constructor and destructor
    
	CRecoOutputWnd();
    ~CRecoOutputWnd();

// Helper methods

    void ResetResults();
    int GetBestHeight();
    bool UpdateFont(LANGID wLangId);

// Declare the class objects' window class with NULL background to avoid flicking.
DECLARE_WND_CLASS_EX(NULL, 0, -1)

// ATL macro's to declare which commands/messages the class is interested in.
BEGIN_MSG_MAP(CRecoOutputWnd)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
END_MSG_MAP()

	// WM_PAINT message handler
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};  // class CRecoOutputWnd

