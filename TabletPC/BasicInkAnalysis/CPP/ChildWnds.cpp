// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Module:       
//      ChildWnds.cpp
//
// Description:
//      The file contains the definitions of the methods of the classes
//		CInkInputWnd and CRecoOutputWnd. 
//		See the file ChildWnds.h for the definitions of the classes.
//--------------------------------------------------------------------------

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

// Windows header file
#include <windows.h>

// ATL header files:
#include <atlbase.h>        // defines CComModule, CComPtr, CComVariant
extern CComModule _Module;
#include <atlwin.h>         // defines CWindowImpl

// Tablet PC Automation interfaces header file
#include <msinkaut.h>

// Headers for the Tablet PC Ink Analysis interfaces
#include <IACom.h>
#include <initguid.h>
#include <IAGuid.h>


// The application header files
#include "resource.h"       // main symbols, including command ID's 
#include "ChildWnds.h"		// contains the CInkInputWnd and CRecoOutputWnd definitions

#define CLR_BLUE    RGB(0x00,0x00,0x80)
#define CLR_GRAY    RGB(0x80,0x80,0x80)

////////////////////////////////////////////////////////
// CInkInputWnd methods 
////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::CInkInputWnd
// 
// Constructor.
//
// Parameters: 
//		none
//
/////////////////////////////////////////////////////////
CInkInputWnd::CInkInputWnd():
    m_fRotatedBoundingBox(false)
{
}

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::OnPaint
// 
// The WM_PAINT message handler. The ATL calls this member 
// function when Windows or an application makes a request 
// to repaint a portion of the CInkInputWnd object's window.
// The method paints the window's background and draws 
// the guide if necessary. 
//
// Parameters:
//      defined in the ATL's MESSAGE_HANDLER macro,
//		none is used here
//
// Return Value (LRESULT):
//		always 0
//
/////////////////////////////////////////////////////////
LRESULT CInkInputWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, 
							  LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    RECT rcClip;
    if (FALSE == GetUpdateRect(&rcClip))
        return 0;   // there's no update region, so no painting is needed

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(&ps);
    if (hdc == NULL)
        return 0;

    // Get the rectangle to paint.
    GetClipBox(hdc, &rcClip);

    // Paint the background.
    ::FillRect(hdc, &rcClip, (HBRUSH)::GetStockObject(DC_BRUSH));

    DrawBoundingBoxes(
        hdc,
        m_spInkAnalyzer,
        m_spInkRenderer);
    EndPaint(&ps);
    return 0;
}

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::DrawBoundingBoxes
// 
// This method will paint all the bounding boxes for
// the elements in the analysis tree.
//
// Parameters:
//
// Return Value :
//		none
//
/////////////////////////////////////////////////////////
void CInkInputWnd::DrawBoundingBoxes(HDC hdc,
                                     IInkAnalyzer *pAnalyzer,
                                     IInkRenderer *pRenderer)
{
    HRESULT hr = S_OK;
    CComPtr<IContextNode> pRootNode = NULL;
    hr = pAnalyzer->GetRootNode(&pRootNode);
    if( SUCCEEDED(hr) )
    {
        IContextNodes *pSubNodes = NULL;
        hr = pRootNode->GetSubNodes(&pSubNodes);
        if( SUCCEEDED(hr) )
        {
            ULONG subNodeCount = 0;
            hr = pSubNodes->GetCount(&subNodeCount);
            if( SUCCEEDED(hr) &&
                subNodeCount > 0 )
            {
                for( ULONG l = 0; l < subNodeCount; l++ )
                {
                    CComPtr<IContextNode> spSubNode;
                    hr = pSubNodes->GetContextNode(
                        l,
                        &spSubNode);
                    
                    if( SUCCEEDED(hr) )
                    {
                        this->PaintTreeElement(
                            hdc,
                            spSubNode,
                            pRenderer);
                    }
                }
                
            }
        }
    }
}

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::ToggleBoundingBoxStyle
// 
// This toggles the bounding box style between
// Rotated and non rotated bounding box.
//
// Parameters:
//
// Return Value :
//		none
//
/////////////////////////////////////////////////////////
void CInkInputWnd::ToggleBoundingBoxStyle()
{
    m_fRotatedBoundingBox = !m_fRotatedBoundingBox;
}

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::GetBoxForNode
// 
// This method returns the bounding box
// of a node as five points
//
// Parameters:
//
// Return Value :
//		HRESULT
//
/////////////////////////////////////////////////////////
HRESULT CInkInputWnd::GetBoxForNode(
    IContextNode *pIContextNode,
    POINT pts[5])
{
    HRESULT hr = S_OK;
    RECT bounds;

    if (!m_fRotatedBoundingBox)
    {
        // Use the location's bounds
        CComPtr<IAnalysisRegion> spLocation;
        hr = pIContextNode->GetLocation(&spLocation);
        if( SUCCEEDED(hr) )
        {
            hr = spLocation->GetBounds(&bounds);
        }
        pts[0].x = bounds.left;
        pts[0].y = bounds.top;
        pts[1].x = bounds.right;
        pts[1].y = bounds.top;
        pts[2].x = bounds.right;
        pts[2].y = bounds.bottom;
        pts[3].x = bounds.left;
        pts[3].y = bounds.bottom;
        // To draw the complete polygon we need to point back to the
        // starting point
        pts[4].x = pts[0].x;
        pts[4].y = pts[0].y;
    }
    else
    {
        // Use the rotated bounding box if any
        LONG *plRBB = NULL;
        ULONG ulRBB = 0;
        hr = pIContextNode->GetPropertyData(
            (const GUID*)&GUID_CNP_ROTATEDBOUNDINGBOX,
            &ulRBB,
            (BYTE **)&plRBB );

        if ( plRBB == NULL )
        {
            hr = E_FAIL;
        }
        if( SUCCEEDED(hr) )
        {
            int ptCount = (int)(ulRBB / sizeof(LONG));
            if ( ptCount != 8 )
            {
                hr = E_FAIL;
            }

            for(int i = 0, j=0; i < ptCount && j < 4 && SUCCEEDED(hr) ; i++ )
            {
                pts[j].x = plRBB[i++];
                pts[j].y = plRBB[i];
                j++;
            }
            // To draw the complete polygon we need to point back to the
            // starting point
            pts[4].x = pts[0].x;
            pts[4].y = pts[0].y;

            // Free the memory allocated by the GetPropertyData method
            ::CoTaskMemFree(plRBB);
        }
    }
    return hr;
}


/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::InflatePolygon
// 
// This method inflates a polygon
// by an inflation factor.
//
// Parameters:
//
// Return Value :
//		None
//
/////////////////////////////////////////////////////////
void CInkInputWnd::InflatePolygon(LPPOINT pPts, int inflateBy)
{
    if (pPts[0].x < pPts[2].x)
    {
        pPts[0].x -= inflateBy;
        pPts[2].x += inflateBy;
    }
    else
    {
        pPts[0].x += inflateBy;
        pPts[2].x += inflateBy;
    }

    if ( pPts[0].y < pPts[2].y )
    {
        pPts[0].y -= inflateBy;
        pPts[2].y += inflateBy;
    }
    else
    {
        pPts[0].y += inflateBy;
        pPts[2].y -= inflateBy;
    }

    if ( pPts[1].x > pPts[3].x)
    {
        pPts[1].x += inflateBy;
        pPts[3].x -= inflateBy;
    }
    else
    {
        pPts[1].x -= inflateBy;
        pPts[3].x += inflateBy;
    }

    if ( pPts[1].y < pPts[3].y )
    {
        pPts[1].y -= inflateBy;
        pPts[3].y += inflateBy;
    }
    else
    {
        pPts[1].y += inflateBy;
        pPts[3].y -= inflateBy;
    }
    pPts[4].x = pPts[0].x;
    pPts[4].y = pPts[0].y;
}

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::PaintTreeElement
// 
// This recursive method draws the bounding
// box around the passed node, and calls itself
// to draw the bounding boxes around the node's
// children.
//
// Parameters:
//
// Return Value :
//		None
//
/////////////////////////////////////////////////////////
void CInkInputWnd::PaintTreeElement(
    HDC hdc,
    IContextNode *pIContextNode, 
    IInkRenderer *pInkRenderer)
{
    CComPtr<IAnalysisRegion> spLocation;
    POINT pts[5];
    HRESULT hr = GetBoxForNode(pIContextNode, pts);
    if( SUCCEEDED(hr) )
    {
        this->ConvertHMMToDP(hdc, pInkRenderer, pts);
        bool isParagraph = false;
        // Now get the node type
        GUID nodeType;
        hr = pIContextNode->GetType(&nodeType);
        if( SUCCEEDED(hr) )
        {
            if( nodeType == GUID_CNT_UNCLASSIFIEDINK )
            {
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0x00,0xff,0x00));
            }
            else if( nodeType == GUID_CNT_INKWORD )
            {                           
                InflatePolygon(pts, 2);
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0x00,0xff,0x00));
            }
            else if( nodeType == GUID_CNT_LINE )
            {
                InflatePolygon(pts, 6);
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0xff,0x00,0xff));
            }
            else if( nodeType == GUID_CNT_PARAGRAPH )
            {
                InflatePolygon(pts, 10);
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0x00,0x00,0xff));
            }
            else if( nodeType == GUID_CNT_WRITINGREGION )
            {
                InflatePolygon(pts, 14);
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0xff,0xff,0x00));
            }
            else if( nodeType == GUID_CNT_INKDRAWING )
            {
                InflatePolygon(pts, 2);
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0xff,0x00,0x00));
            }
            else if( nodeType == GUID_CNT_INKBULLET )
            {
                InflatePolygon(pts, 2);
                SelectObject(hdc,GetStockObject(DC_PEN));
                SetDCPenColor(hdc,RGB(0xff,0x88,0x88));
            }
        }
        // Draw the rectangle.
        MoveToEx(hdc, pts[0].x, pts[0].y, NULL); 
        LineTo(hdc, pts[1].x, pts[1].y);
        LineTo(hdc, pts[2].x, pts[2].y);
        LineTo(hdc, pts[3].x, pts[3].y);
        LineTo(hdc, pts[4].x, pts[4].y);
    }

    // Now draw the subnodes
    CComPtr<IContextNodes> spSubNodes;
    hr = pIContextNode->GetSubNodes(&spSubNodes);
    if( SUCCEEDED(hr) )
    {
        ULONG subNodeCount = 0;
        hr = spSubNodes->GetCount(&subNodeCount);
        if( SUCCEEDED(hr) &&
            subNodeCount > 0 )
        {
            for( ULONG l = 0; l < subNodeCount; l++ )
            {
                CComPtr<IContextNode> spSubNode;
                hr = spSubNodes->GetContextNode(
                    l,
                    &spSubNode);
                if( SUCCEEDED(hr) )
                {
                    this->PaintTreeElement(
                        hdc,
                        spSubNode,
                        pInkRenderer);
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////
//                                          
// CInkInputWnd::ConvertHMMToDP
// 
// This method converts points from Ink Space to
// pixels so that they can be drawn properly on the
// window with calls to DrawLine.
//
// Parameters:
//
// Return Value :
//		None
//
/////////////////////////////////////////////////////////
void CInkInputWnd::ConvertHMMToDP(
    HDC hdc,
    IInkRenderer *pRenderer,
    POINT pts[5])
{

    pRenderer->InkSpaceToPixel(
        (long)hdc,
        &pts[0].x,
        &pts[0].y);

    pRenderer->InkSpaceToPixel(
        (long)hdc,
        &pts[1].x,
        &pts[1].y);

    pRenderer->InkSpaceToPixel(
        (long)hdc,
        &pts[2].x,
        &pts[2].y);

    pRenderer->InkSpaceToPixel(
        (long)hdc,
        &pts[3].x,
        &pts[3].y);

    pRenderer->InkSpaceToPixel(
        (long)hdc,
        &pts[4].x,
        &pts[4].y);

}

////////////////////////////////////////////////////////
// CRecoOutputWnd methods 
////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//                                          
// CRecoOutputWnd::CRecoOutputWnd
// 
// Constructor.
//
// Parameters: 
//		none
//
/////////////////////////////////////////////////////////
CRecoOutputWnd::CRecoOutputWnd()
        : m_hFont(NULL), m_iFontName(-1), m_nGesture(0), m_bNewGesture(false)
{
    UpdateFont(::GetUserDefaultLangID());
}

/////////////////////////////////////////////////////////
//                                          
// CRecoOutputWnd::~CRecoOutputWnd
// 
// Destructor.
//
/////////////////////////////////////////////////////////
CRecoOutputWnd::~CRecoOutputWnd()
{
    if (NULL != m_hFont)
    {
        ::DeleteObject(m_hFont);
    }
}

// Message handlers 

/////////////////////////////////////////////////////////
//                                          
// CRecoOutputWnd::OnPaint
// 
// The WM_PAINT message handler. The ATL calls this member 
// function when Windows or an application makes a request 
// to repaint a portion of the CRecoOutputWnd object's window.
//
// Parameters:
//      defined in the ATL's MESSAGE_HANDLER macro,
//		none is used here
//
// Return Value (void):
//		none
//
/////////////////////////////////////////////////////////
LRESULT CRecoOutputWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, 
								LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    RECT rc;
    if (FALSE == GetUpdateRect(&rc))
        return 0;   // there's no update region, so no painting is needed

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(&ps);
    if (hdc == NULL)
        return 0;

    GetClientRect(&rc);

    // Paint the background
    ::FillRect(hdc, &rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));

    // Use blue color to draw the latest results, and gray for the old ones
    COLORREF clrGesture, clrText; 
    if (true == m_bNewGesture)
    {
        clrGesture = CLR_BLUE;
        clrText = CLR_GRAY;
    }
    else
    {
        clrGesture = CLR_GRAY;
        clrText = CLR_BLUE;
    }

    COLORREF clrOld = ::SetTextColor(hdc, clrGesture);

    // Output the name of the last gesture
    if (0 != m_nGesture)
    {
        TCHAR buffer[100];
        if (::LoadString(_Module.GetResourceInstance(), 
                         m_nGesture, buffer, sizeof(buffer)) != 0)
        {
            ::TextOut(hdc, mc_iMarginX, mc_iMarginY, buffer, _tcslen(buffer));
        }
    }
    
    // Output the handwriting recognition results
    HGDIOBJ hOldFont;
    if (NULL != m_hFont)
    {
        hOldFont = ::SelectObject(hdc, m_hFont);
    }
    ::SetTextColor(hdc, clrText);
    int iLength = m_bstrResults.Length();
    if (iLength)
    {
        ::TextOutW(hdc, mc_iMarginX, mc_iMarginY + mc_iFontHeight, 
                    m_bstrResults, iLength);
    }

    // Restore the dc
    ::SetTextColor(hdc, clrOld);
    if (NULL != m_hFont)
    {
        ::SelectObject(hdc, hOldFont);
    }

    EndPaint(&ps);
    return 0;
}

// Helper methods

/////////////////////////////////////////////////////////
//                                          
// CRecoOutputWnd::ResetResults
// 
// Empties the output strings.
//
// Parameters:
//      none
//
// Return Value (void):
//		none
//
/////////////////////////////////////////////////////////
void CRecoOutputWnd::ResetResults()
{
    m_bNewGesture = false;
    m_nGesture = 0;
    m_bstrResults.Empty();
}

/////////////////////////////////////////////////////////
//                                          
// CRecoOutputWnd::GetBestHeight
// 
//      Returns a height of the window the best to show all 
//      the strings.
// 
//      This method is called by CBasicInkAnalysis object when it updates 
//      the layouts of its child windows.
//
/////////////////////////////////////////////////////////
int CRecoOutputWnd::GetBestHeight() 
{ 
    return (mc_iMarginX * 2 + mc_iFontHeight * 2); 
}

/////////////////////////////////////////////////////////
//                                          
// CRecoOutputWnd::UpdateFont
//
//      The method creates an appropriate font for text output
//      based on the language id. It's called by the application 
//      when user selects a recognizer. 
// 
// Parameters:
//      LANGID wLangId      : [in] the id of the language of the text 
//
// Return Value (bool):
//		true, if succeded to create a font appropriate for the language
//      false otherwise
//
/////////////////////////////////////////////////////////
bool CRecoOutputWnd::UpdateFont(LANGID wLangId)
{
    static LPCTSTR pszFontNames[] = { TEXT("MS Shell Dlg"), TEXT("MS Mincho"),
                                      TEXT("Mingliu"), TEXT("Gullim") };
    bool bOk = true;
    int i;
    DWORD dwCharSet;
    // Select a font by the primary language id (the lower byte)
    switch(wLangId & 0xFF)
    {
        default:
            i = 0;
            dwCharSet = DEFAULT_CHARSET;
            break;

        case LANG_JAPANESE:
            i = 1;
            dwCharSet = SHIFTJIS_CHARSET;
            break;
        
        case LANG_CHINESE:
            i = 2;
            dwCharSet = CHINESEBIG5_CHARSET;
            break;
        
        case LANG_KOREAN:
            i = 3;
            dwCharSet = JOHAB_CHARSET;
            break;
    }
    if (i != m_iFontName)
    {
        HFONT hFont = ::CreateFont(mc_iFontHeight, 0, 0, 0, 0, 0, 0, 0, 
                                   dwCharSet, 
                                   0, 0, 0, 0, 
                                   pszFontNames[i] // typeface name
                                   );
        if (NULL != hFont)
        {
            if (NULL != m_hFont)
                ::DeleteObject(m_hFont);
            m_hFont = hFont;
            m_iFontName = i;
        }
        else
        {
            bOk = false;
        }
    }

    return bOk;
}

