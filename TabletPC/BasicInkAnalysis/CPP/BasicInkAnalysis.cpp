// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Module:       
//      BasicInkAnalysis.cpp
//
// Description:
//      This sample demonstrates such advanced features of the 
//      Microsoft Tablet PC Automation API used for handwriting
//      recognition, as 
//          - dynamic background recognition
//
//      This application is discussed in the Getting Started guide.
//
//      (NOTE: For code simplicity, returned HRESULT is not checked 
//             on failure in the places where failures are not critical 
//             for the application or very unexpected)
//
//      The interfaces used are: 
//      IInkRecognizers, IInkRecognizer, IInkRecoContext, 
//      IInkCollector, IInkDisp, IInkRenderer, IInkStrokes, IInkStroke
//      IInkAnalyzer, IAnalysisStatus
//
// Requirements:
//      One or more handwriting recognizer must be installed on the system;
//      Appropriate Asian fonts need to be installed to output the results 
//      of the Asian recognizers.
//      
//--------------------------------------------------------------------------

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
CComModule _Module;
#include <atlwin.h>         // defines CWindowImpl
#include <atlcom.h>         // defines IDispEventSimpleImpl

// Headers for Tablet PC Automation interfaces
#include <msinkaut.h>
#include <msinkaut_i.c>
#include <tpcerror.h>

// Headers for the Tablet PC Ink Analysis interfaces
#include <IACom.h>
#include <IACom_i.c>


// The application header files
#include "resource.h"       // main symbols, including command ID's 
#include "EventSinks.h"     // defines the IInkEventsImpl and IInkRecognitionEventsImpl 
#include "ChildWnds.h"      // definitions of the CInkInputWnd and CRecoOutputWnd
#include "BasicInkAnalysis.h"        // contains the definition of CAddRecoApp


const _ATL_FUNC_INFO IInkOverlayEventsImpl<CBasicInkAnalysis>::mc_AtlFuncInfo[5] = {
        {CC_STDCALL, VT_EMPTY, 3, {VT_UNKNOWN, VT_UNKNOWN, VT_BOOL|VT_BYREF}}, 
        {CC_STDCALL, VT_EMPTY, 4, {VT_UNKNOWN, VT_UNKNOWN, VT_VARIANT, VT_BOOL|VT_BYREF}},
        {CC_STDCALL, VT_EMPTY, 1, {VT_UNKNOWN}},
        {CC_STDCALL, VT_EMPTY, 1, {VT_UNKNOWN}},
        {CC_STDCALL, VT_EMPTY, 1, {VT_UNKNOWN}}
};
const TCHAR gc_szAppName[] = TEXT("Basic Ink Analysis");

/////////////////////////////////////////////////////////
//                                          
// WinMain
// 
// The WinMain function is called by the system as the 
// initial entry point for a Win32-based application. 
//
// Parameters:
//        HINSTANCE hInstance,      : [in] handle to current instance
//        HINSTANCE hPrevInstance,  : [in] handle to previous instance
//        LPSTR lpCmdLine,          : [in] command line
//        int nCmdShow              : [in] show state
//
// Return Values (int):
//        0 : The function terminated before entering the message loop.
//        non zero: Value of the wParam when receiving the WM_QUIT message
//
/////////////////////////////////////////////////////////
int APIENTRY WinMain(
        HINSTANCE hInstance,
        HINSTANCE /*hPrevInstance*/,   // not used here 
        LPSTR     /*lpCmdLine*/,       // not used here 
        int       nCmdShow
        )
{
    int iRet = 0;

    // Initialize the COM library and the application module
    if (S_OK == ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))
    {
        _Module.Init(NULL, hInstance);

        // Register the common control classes used by the application
        INITCOMMONCONTROLSEX icc;
        icc.dwSize = sizeof(icc);
        icc.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
        if (TRUE == ::InitCommonControlsEx(&icc))
        {
            // Call the boilerplate function of the application
            iRet = CBasicInkAnalysis::Run(nCmdShow);
        }
        else
        {
            ::MessageBox(NULL, TEXT("Error initializing the common controls."), 
                         gc_szAppName, MB_ICONERROR | MB_OK);
        }

        // Release the module and the COM library
        _Module.Term();
        ::CoUninitialize();
    }
    
    return iRet;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::Run
// 
// The static CBasicInkAnalysis::Run is the boilerplate of the application.
// It instantiates and initializes an CBasicInkAnalysis object and runs the 
// application's message loop.
//
// Parameters:
//      int nCmdShow              : [in] show state
//
// Return Values (int):
//      0 : The function terminated before entering the message loop.
//      non zero: Value of the wParam when receiving the WM_QUIT message
//
/////////////////////////////////////////////////////////
int CBasicInkAnalysis::Run(
        int nCmdShow
        )
{

    CBasicInkAnalysis theApp;

    // Load and update the menu before creating the main window.
    // Create menu items for the installed recognizers and for the 
    // supported input scopes.
    HMENU hMenu = theApp.LoadMenu();
    if (NULL == hMenu)
        return 0;

    int iRet;

    // Load the icon from the resource and associate it with the window class
    WNDCLASSEX& wc = CBasicInkAnalysis::GetWndClassInfo().m_wc;
    wc.hIcon = wc.hIconSm = ::LoadIcon(_Module.GetResourceInstance(), 
                                       MAKEINTRESOURCE(IDR_APPICON));
    
    // Create the application's main window
    if (theApp.Create(NULL, CWindow::rcDefault, gc_szAppName,
                      WS_OVERLAPPEDWINDOW, 0, (UINT)hMenu) != NULL)
    {
        // Set the collection mode to ICM_InkOnly 
        theApp.SendMessage(WM_COMMAND, ID_MODE_INK);  
        
        // Show and update the main window
        theApp.ShowWindow(nCmdShow);
        theApp.UpdateWindow();

        // Run the boilerplate message loop
        MSG msg;
        while (::GetMessage(&msg, NULL, 0, 0) > 0)
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }   
        iRet = msg.wParam;
    }
    else
    {
        ::MessageBox(NULL, TEXT("Error creating the window"), 
                     gc_szAppName, MB_ICONERROR | MB_OK);
        ::DestroyMenu(hMenu);
        iRet = 0;
    }

    return iRet;
}

// Window message handlers //////////////////////////////

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnCreate
//  
// This WM_CREATE message handler creates and obtains interface
// pointers to the required Automation objects, sets their 
// attributes, creates the child windows and enables pen input.
//
// Parameters:
//      defined in the ATL's macro MESSAGE_HANDLER,
//      none of them is used here
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnCreate(
        UINT /*uMsg*/, 
        WPARAM /*wParam*/, 
        LPARAM /*lParam*/, 
        BOOL& /*bHandled*/
        )
{
    // Create child windows for ink input and recognition output, 
    // listview controls for the lists of gestures, and a status bar
    if (false == CreateChildWindows())
        return -1;

    HRESULT hr;

    // Create an ink collector object.
    hr = m_spIInkCollector.CoCreateInstance(CLSID_InkOverlay);
    if (FAILED(hr)) 
        return -1;

    // Get a pointer to the ink object interface.
    hr = m_spIInkCollector->get_Ink(&m_spIInkDisp);
    if (FAILED(hr)) 
        return -1;

    // Get an empty ink strokes collection from the ink object.
    // It'll be used as a storage for the ink
    hr = m_spIInkDisp->get_Strokes(&m_spIInkStrokes);
    if (FAILED(hr)) 
        return -1;

    // Establish a connection to the collector's event source. 
    // Depending on the selected collection mode, the application will be 
    // listening to either Stroke or Gesture events, or both.
    hr = IInkOverlayEventsImpl<CBasicInkAnalysis>::DispEventAdvise(m_spIInkCollector);
    // There is nothing interesting the application can do without events
    // from the ink collector
    if (FAILED(hr))
        return -1;

    // Enable ink input in the m_wndInput window
    hr = m_spIInkCollector->put_hWnd((long)m_wndInput.m_hWnd);
    if (FAILED(hr)) 
        return -1;
    hr = m_spIInkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) 
        return -1;

    // Create an ink analyzer object.
    hr = m_spIInkAnalyzer.CoCreateInstance(CLSID_InkAnalyzer);
    if (FAILED(hr)) 
        return -1;

    // Associate the ink analyzer with the Input Window
    m_wndInput.SetInkAnalyzer( m_spIInkAnalyzer );


    CComObject<InkAnalyzerEvents>::CreateInstance(&m_pInkAnalyzerEvents);
    if (m_pInkAnalyzerEvents == NULL)
        return -1;
    m_pInkAnalyzerEvents->AttachObj(this);

    // Establish a connection to the Analyzer's event source. 
    // We will listen to events from both Event Sources supported by
    // the InkAnalyzer ie. _IAnalysisEvents
    // These will enable listening to the events when analysis is run in the
    // background and updating the applications based on the events
    // from the analyzer
    hr = this->m_pInkAnalyzerEvents->AdviseInkAnalyzer(
        m_spIInkAnalyzer);

    if (FAILED(hr))
    {
        return hr;
    }

    // Set the analysis mode to do personalization
    hr = m_spIInkAnalyzer->SetAnalysisModes((AnalysisModes) AnalysisModes_Personalization );
    if (FAILED(hr))
    {
        return hr;
    }

    // Get the renderer from the ink collector's ink object
    CComPtr<IInkRenderer> spInkRenderer;
    hr = m_spIInkCollector->get_Renderer(&spInkRenderer);
    m_wndInput.SetInkRenderer( spInkRenderer );

    // Set the menu with the right items selected
    UpdateMenuRadioItems(
        mc_iSubmenuModes, 
        ID_MODE_INK, 
        ID_MODE_ERASE);
    UpdateMenuRadioItems(
        mc_iSubmenuModes, 
        ID_MODE_AUTOMATICLAYOUTANALYSIS, 
        0);

    return 0;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnDestroy
//  
// The WM_DESTROY message handler. 
// Used for clean up and also to post a quit message to 
// the application itself.
//
// Parameters:
//      defined in the ATL's macro MESSAGE_HANDLER,
//      none is used here
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnDestroy(
        UINT /*uMsg*/, 
        WPARAM /*wParam*/, 
        LPARAM /*lParam*/, 
        BOOL& /*bHandled*/
        )
{
    // Disable ink input and release the InkCollector object
    if (m_spIInkCollector != NULL)
    {
        IInkOverlayEventsImpl<CBasicInkAnalysis>::DispEventUnadvise(m_spIInkCollector);
        m_spIInkCollector->put_Enabled(VARIANT_FALSE);
        m_spIInkCollector.Release();
    }
    if (this->m_pInkAnalyzerEvents != NULL)
    {
        this->m_pInkAnalyzerEvents->UnadviseInkAnalyzer();
        this->m_pInkAnalyzerEvents = NULL;
    }

    // Release the other objects and collections
    m_spIInkStrokes.Release();

    // Post a WM_QUIT message to the application's message queue 
    ::PostQuitMessage(0);
    
    return 0;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnSize
//  
// The WM_SIZE message handler is needed to update 
// the layout of the child windows 
//
// Parameters:
//      defined in the ATL's macro MESSAGE_HANDLER,
//      wParam of the WN_SIZE message is the only used here.
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnSize(
        UINT /*uMsg*/, 
        WPARAM wParam, 
        LPARAM /*lParam*/, 
        BOOL& /*bHandled*/
        )
{
    if (wParam != SIZE_MINIMIZED)
    {
        UpdateLayout();
    }
    return 0;
}

// InkAnalyzer event handlers ////////////////////////////

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnResultsUpdated
//  
// The IInkAnalyzerEvents's ResultsUpdated event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//      IInkAnalyzer* pInkAnalyzer        : [in] the source of the event
//      IAnalysisStatus* pAnalysisStatus  : [in] the status of the analysis
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////

HRESULT CBasicInkAnalysis::OnResultsUpdated(
    IInkAnalyzer *pInkAnalyzer,
    IAnalysisStatus *pAnalysisStatus)
{
    HRESULT hr = S_OK;

    VARIANT_BOOL bResult = VARIANT_FALSE;
    CComBSTR bstr;

    // Was the analysis successful?
    hr = pAnalysisStatus->IsSuccessful(&bResult);

    if( SUCCEEDED(hr) && (VARIANT_TRUE == bResult) )
    {
        // Get the top result string
        hr = m_spIInkAnalyzer->GetRecognizedString(&bstr);

        // Get the results to the results window
        if (SUCCEEDED(hr))
        {
            m_wndResults.ResetResults();
            m_wndResults.m_bstrResults.Attach(bstr);
            m_wndResults.Invalidate();
        }
        if (SUCCEEDED(hr))
        {
            m_wndInput.Invalidate();
        }
    }

    return S_OK;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnResultsUpdated
//  
// The IInkAnalyzerEvents's IntermediateResultsUpdated event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//      IInkAnalyzer* pInkAnalyzer        : [in] the source of the event
//      IAnalysisStatus* pAnalysisStatus  : [in] the status of the analysis
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnIntermediateResultsUpdated(
    IInkAnalyzer *pInkAnalyzer,
    IAnalysisStatus *pAnalysisStatus)
{
    HRESULT hr = S_OK;

    VARIANT_BOOL bResult = VARIANT_FALSE;
    CComBSTR bstr;

    // Was the analysis successful?
    hr = pAnalysisStatus->IsSuccessful(&bResult);

    if( SUCCEEDED(hr) && (VARIANT_TRUE == bResult) )
    {
        // Get the top result string
        hr = m_spIInkAnalyzer->GetRecognizedString(&bstr);

        // Get the results to the results window
        if (SUCCEEDED(hr))
        {
            m_wndResults.ResetResults();
            m_wndResults.m_bstrResults.Attach(bstr);
            m_wndResults.Invalidate();
        }
        if (SUCCEEDED(hr))
        {
            m_wndInput.Invalidate();
        }
    }

    return S_OK;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnAnalyzerActivity
//  
// The IInkAnalyzerEvents's Activity event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnAnalyzerActivity()
{
    // There is nothing we want to do here.
    return S_OK;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnReadyToReconcile
//  
// The IInkAnalyzerEvents's ReadyToReconcile event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnReadyToReconcile()
{
    // Post a message to the UI thread to do the reconciliation
    // between the document and the analysis results.
    // This event is sent on the analysis background thread.
    // In this sample we take the approach of synchronizing
    // all operations on the document to theUI thread to avoid 
    // threading issues.
    PostMessage(WM_READYTORECONCILE, (WPARAM) 0, (LPARAM)0);
    return S_OK;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnUpdateStrokeCache
//  
// The IInkAnalyzerEvents's UpdateStrokeCache event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnUpdateStrokeCache(
        ULONG ulStrokeIdsCount,
        LONG *plStrokeIds )
{
    // We do not need to implement this function in this sample
    // because we did not set the analysis mode to 
    // AnalysisModes_StrokeCacheAutoCleanup and thus the ink analyzer
    // keeps a cache of all the stroke data that has been added to it.
    return S_OK;
}

// InkCollector event handlers ///////////////////////////


/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnSelectionMoved
//  
// The _IInkOverlyEvents's SelectionMoved event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
// The method is not implemented here since we do not have
// a UI for selection.
//
// Parameters:
//      IInkRectangle* pIRect     : [in] not used here
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnSelectionMoved(IInkRectangle * /*pRect*/)
{
    return S_OK;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::CCOMIA::OnSelectionResized
//  
// The _IInkOverlyEvents's SelectionMoved event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
// The method is not implemented here since we do not have
// a UI for selection.
//
// Parameters:
//      IInkRectangle* pIRect     : [in] not used here
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnSelectionResized(IInkRectangle * /*pRect*/)
{
    return S_OK;
}


/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnStrokesDeleting
//  
// The _IInkOverlayEvents's StrokesDeleting event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//      IInkCursor* pIInkStrokesToDelete     : [in] strokes being deleted
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnStrokesDeleting(
    IInkStrokes *pIInkStrokesToDelete)
{

    HRESULT hr = S_OK;
    if( pIInkStrokesToDelete != NULL )
    {
        long strokeCount = 0;
        hr = pIInkStrokesToDelete->get_Count(&strokeCount);
        
        if( SUCCEEDED(hr) && strokeCount > 0 )
        {
            LONG *plStrokeIds = new LONG[strokeCount];
            if( NULL != plStrokeIds )
            {
                LONG strokeIdsToDelete = 0;
                for ( long l = 0; l < strokeCount; l++ )
                {
                    CComPtr<IInkStrokeDisp> spStroke;

                    hr = pIInkStrokesToDelete->Item(
                        l,
                        &spStroke);
                    if( SUCCEEDED(hr) )
                    {
                        LONG id = 0;
                        hr = spStroke->get_ID(
                            &id);

                        if( SUCCEEDED(hr) )
                        {
                            plStrokeIds[strokeIdsToDelete++] = id;
                        }               
                    }
                }

                for( long l = 0; l < strokeIdsToDelete; l++ )
                {
                    m_spIInkAnalyzer->RemoveStroke(
                        plStrokeIds[l]);
                }
            }
        }
    }

    // Start a background analysis to update the analysis results
    if( SUCCEEDED(hr) && m_fAutomaticLayoutAnalysis)
    {
        hr = m_spIInkAnalyzer->BackgroundAnalyze();
    }

    return hr;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnStroke
//  
// The _IInkCollectorEvents's Stroke event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//      IInkCursor* pIInkCursor     : [in] not used here
//      IInkStrokeDisp* pInkStroke  : [in] 
//      VARIANT_BOOL* pbCancel      : [in,out] option to cancel the gesture, 
//                                    default value is FALSE, not modified here
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnStroke(
        IInkCursor* /*pIInkCursor*/, 
        IInkStrokeDisp* pIInkStroke, 
        VARIANT_BOOL* pbCancel
        )
{
    if (NULL == pIInkStroke)
        return E_INVALIDARG;

    if (m_spIInkStrokes == NULL)
        return S_OK;


    // Check that we are in inking mode.
    // The ink overlay still sends an OnStroke event when the mode is Erase or Select
    InkOverlayEditingMode mode = IOEM_Ink;
    HRESULT hr = S_OK;
    hr = m_spIInkCollector->get_EditingMode(&mode);
    if( FAILED(hr) )
    {
        return hr;
    }
    if( mode != IOEM_Ink )
    {
        return hr;
    }


    // Add the new stroke to the collection
    hr = m_spIInkStrokes->Add(pIInkStroke);

    // Get the stroke data
    if( SUCCEEDED(hr) )
    {
        VARIANT varPacketData;
        hr = pIInkStroke->GetPacketData(
            ISC_FirstElement,
            ISC_AllElements,
            &varPacketData);
        if( SUCCEEDED(hr) )
        {
            // Get the stroke id
            long id;
            hr = pIInkStroke->get_ID(&id);
            if( SUCCEEDED(hr) )
            {
                // Get the packet description
                VARIANT varPacketDesc;
                hr = pIInkStroke->get_PacketDescription(&varPacketDesc);
                if( SUCCEEDED(hr) )
                {
                    LONG * plPacketData = NULL;
                    BSTR * pbstrPacketDesc = NULL;

                    hr = ::SafeArrayAccessData(
                        varPacketData.parray,
                        (void **)&plPacketData);

                    if( SUCCEEDED(hr) )
                    {
                        hr = ::SafeArrayAccessData(
                            varPacketDesc.parray,
                            (void **)&pbstrPacketDesc);
                    }

                    if( SUCCEEDED(hr) )
                    {
                        ULONG guidCount = varPacketDesc.parray->rgsabound[0].cElements;
                        ULONG packetDataCount = varPacketData.parray->rgsabound[0].cElements;

                        GUID *pPacketDescGuids = new GUID[guidCount];

                        if( NULL == pPacketDescGuids )
                        {
                            hr = E_OUTOFMEMORY;
                        }

                        if( SUCCEEDED(hr) )
                        {
                            for( ULONG ul = 0; ul < guidCount; ul++ )
                            {
                                ::CLSIDFromString(
                                    pbstrPacketDesc[ul],
                                    &pPacketDescGuids[ul]);
                            }

                            CComPtr<IContextNode> spNode = NULL;
                            hr = m_spIInkAnalyzer->AddStroke(
                                id,
                                packetDataCount,
                                plPacketData,
                                guidCount,
                                pPacketDescGuids,
                                &spNode);
                        }
                        delete [] pPacketDescGuids;
                    }

                    ::SafeArrayUnaccessData(varPacketDesc.parray);
                    ::SafeArrayUnaccessData(varPacketData.parray);
                }
            }
        }
        ::VariantClear(&varPacketData);

        // Start a background analysis
        if( SUCCEEDED(hr) && m_fAutomaticLayoutAnalysis)
        {
            hr = m_spIInkAnalyzer->BackgroundAnalyze();
        }
    }

    return hr;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnGesture
//  
// The _IInkCollectorEvents's Gesture event handler.
// See the Tablet PC Automation API Reference for the 
// detailed description of the event and its parameters.
//
// Parameters:
//      IInkCursor* pIInkCursor  : [in] not used here
//      IInkStrokes* pInkStrokes : [in] the collection
//      VARIANT vGestures        : [in] safearray of IDispatch interface pointers
//                                 of the recognized  Gesture objects            
//      VARIANT_BOOL* pbCancel   : [in,out] option to cancel the gesture, 
//                                 default value is FALSE
//
// Return Values (HRESULT):
//      S_OK if succeeded, E_FAIL or E_INVALIDARG otherwise
//
/////////////////////////////////////////////////////////
HRESULT CBasicInkAnalysis::OnGesture(
        IInkCursor* /*pIInkCursor*/, 
        IInkStrokes* pInkStrokes, 
        VARIANT vGestures, 
        VARIANT_BOOL* pbCancel
        )
{
    return S_OK;
}



// Command handlers /////////////////////////////////////


/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnMode
//  
// This command handler is called when user selects 
// a different collection mode from the "Mode" submenu.
// NOTE: Changing collection mode has no effect on 
//       the recognition results of the existing strokes.
//
// Parameters:
//      defined in the ATL's macro COMMAND_RANGE_HANDLER
//      Only wID - the id of the command associated 
//      with the clicked menu item - is used here. 
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnMode(
        WORD /*wNotifyCode*/, 
        WORD wID, 
        HWND /*hWndCtl*/, 
        BOOL& /*bHandled*/
        )
{
    HRESULT hr = S_OK;
    switch (wID)
    {
    case ID_MODE_AUTOMATICLAYOUTANALYSIS:
        // toggle the automatic layout analysis flag
        m_fAutomaticLayoutAnalysis = !m_fAutomaticLayoutAnalysis;
        UpdateMenuRadioItems(
            mc_iSubmenuModes, 
            m_fAutomaticLayoutAnalysis ? ID_MODE_AUTOMATICLAYOUTANALYSIS : 0, 
            !m_fAutomaticLayoutAnalysis ? ID_MODE_AUTOMATICLAYOUTANALYSIS : 0);
        break;
    case ID_MODE_ERASE:
        hr = m_spIInkCollector->put_EditingMode(IOEM_Delete);
        if (FAILED(hr))
        {
            return -1;
        }
        UpdateMenuRadioItems(
            mc_iSubmenuModes, 
            ID_MODE_ERASE,
            ID_MODE_INK);
        break;
    case ID_MODE_INK:
        hr = m_spIInkCollector->put_EditingMode(IOEM_Ink);
        if (FAILED(hr))
        {
            return -1;
        }
        UpdateMenuRadioItems(
            mc_iSubmenuModes, 
            ID_MODE_INK, 
            ID_MODE_ERASE);
        break;
    case ID_MODE_ROTATEDBOUNDINGBOXES:
        // toggle the rotated bounding box flag
        m_fRotatedBoundingBox = !m_fRotatedBoundingBox;
        UpdateMenuRadioItems(
            mc_iSubmenuModes, 
            m_fRotatedBoundingBox ? ID_MODE_ROTATEDBOUNDINGBOXES : 0, 
            !m_fRotatedBoundingBox ? ID_MODE_ROTATEDBOUNDINGBOXES : 0);
        m_wndInput.ToggleBoundingBoxStyle();
        // We want to display the new bounding boxes around the tree elements
        m_wndInput.Invalidate();
        break;

    default:
        return 0;
    }
    return 0;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnRecognize
//  
// This command handler is called when user clicks on "Recognize" 
// in the Ink menu. 
//
// Parameters:
//      defined in the ATL's macro COMMAND_ID_HANDLER
//      none of them is used here
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnAnalyze(
        WORD /*wNotifyCode*/, 
        WORD /*wID*/, 
        HWND /*hWndCtl*/, 
        BOOL& /*bHandled*/
        )
{
    // Call the foregroung analysis of the ink
    HRESULT hr = S_OK;

    CComPtr<IAnalysisStatus> spIAnalysisStatus;
    CComBSTR bstr;
    hr = m_spIInkAnalyzer->Analyze(&spIAnalysisStatus);
    if (SUCCEEDED(hr))
    {
        // Get the top result string
        hr = m_spIInkAnalyzer->GetRecognizedString(&bstr);
    }
    // Get the results to the results window
    if (SUCCEEDED(hr))
    {
        m_wndResults.ResetResults();
        m_wndResults.m_bstrResults.Attach(bstr);
        m_wndResults.Invalidate();
    }
    if (SUCCEEDED(hr))
    {
        m_wndInput.Invalidate();
    }
    return 0;
}


/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnExit
//  
// This command handler is called when user clicks 
// on "Exit" in the Ink menu. 
//
// Parameters:
//      defined in the ATL's macro COMMAND_ID_HANDLER
//      none of them is used here
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnExit(
        WORD /*wNotifyCode*/, 
        WORD /*wID*/, 
        HWND /*hWndCtl*/, 
        BOOL& /*bHandled*/
        )
{
    // Close the application window
    SendMessage(WM_CLOSE);
    return 0;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::OnReconcile
//  
// This handler is called when user the analysis background
// thread is waiting for the application to reconcile
// the results with the current state of the document
//
// Parameters:
//      defined in the ATL's macro COMMAND_ID_HANDLER
//      none of them is used here
//
// Return Values (LRESULT):
//      always 0 
//
/////////////////////////////////////////////////////////
LRESULT CBasicInkAnalysis::OnReconcile(
        UINT, 
        WPARAM, 
        LPARAM, 
        BOOL& 
        )
{
    m_spIInkAnalyzer->Reconcile();
    return 0;
}


// Helper methods //////////////////////////////

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::LoadMenu
// 
// This method instantiates an enumerator object for the installed
// recognizers, loads the main menu resource and creates a menu item 
// for each recognizer from the collection.
// Also, it fills the Input Scope menu with the items for the supported 
// Input Scopes.
//
// Parameters:
//      none
//        
// Return Values (HMENU):
//      The return value is a handle of the menu 
//      that'll be used for the main window
//
/////////////////////////////////////////////////////////
HMENU CBasicInkAnalysis::LoadMenu()
{
    HRESULT hr = S_OK;
 
    // Load the menu of the main window
    HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU));
    if (NULL == hMenu)
        return NULL; // Not normal

    MENUITEMINFOW miinfo;
    memset(&miinfo, 0, sizeof(miinfo));
    miinfo.cbSize = sizeof(miinfo);
    miinfo.fMask = MIIM_ID | MIIM_STATE | MIIM_FTYPE | MIIM_STRING;
    miinfo.fType = MFT_RADIOCHECK | MFT_STRING;
    

    return hMenu;
}



/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::UpdateMenuRadioItems
// 
// As it follows from the name, this helper method updates 
// the specified radio items in the submenu.
// It's called for the appropriate items, whenever user selects
// a different recognizer, input scope, or guide mode.
//        
// Parameters:
//      UINT iSubMenu   : [in] the submenu to make updates in
//      UINT idCheck    : [in] the menu item to check
//      UINT idUncheck  : [in] the menu item to uncheck
//
// Return Values (void):
//      none
//
/////////////////////////////////////////////////////////
void CBasicInkAnalysis::UpdateMenuRadioItems(
        UINT iSubMenu, 
        UINT idCheck, 
        UINT idUncheck
        )
{
    // Update the menu
    HMENU hMenu = GetMenu();
    if (NULL != hMenu)
    {
        HMENU hSubMenu = ::GetSubMenu(hMenu, iSubMenu);
        if (NULL != hSubMenu)
        {
            MENUITEMINFO miinfo;
            miinfo.cbSize = sizeof(miinfo);
            miinfo.fMask = MIIM_STATE | MIIM_FTYPE;
            if (0 != idCheck )
            {
                ::GetMenuItemInfo(hSubMenu, idCheck, FALSE, &miinfo);
                miinfo.fType |= MFT_RADIOCHECK;
                miinfo.fState |= MFS_CHECKED;
                ::SetMenuItemInfo(hSubMenu, idCheck, FALSE, &miinfo);
            }
            if (0 != idUncheck)
            {
                ::GetMenuItemInfo(hSubMenu, idUncheck, FALSE, &miinfo);
                miinfo.fType |= MFT_RADIOCHECK;
                miinfo.fState &= ~MFS_CHECKED;
                ::SetMenuItemInfo(hSubMenu, idUncheck, FALSE, &miinfo);
            }
        }
    }
}


/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::CreateChildWindows
//  
// This helper method is called from WM_CREATE message handler.
// The child windows and controls are created and initialized here.
//
// Parameters:
//      none
//
// Return Values (bool):
//      true if the windows have been created successfully, 
//      false otherwise
//
/////////////////////////////////////////////////////////
bool CBasicInkAnalysis::CreateChildWindows()
{
    if ((m_wndInput.Create(m_hWnd, CWindow::rcDefault, NULL,
                           WS_CHILD, WS_EX_CLIENTEDGE, (UINT)mc_iInputWndId) == NULL) 
        || (m_wndResults.Create(m_hWnd, CWindow::rcDefault, NULL,
                                WS_CHILD, WS_EX_CLIENTEDGE, (UINT)mc_iOutputWndId) == NULL))
    {
        return false;
    }


    HINSTANCE hInst = _Module.GetResourceInstance();


    // Create a status bar (Ignore if it fails, the application can live without it).
    m_hwndStatusBar = ::CreateStatusWindow(
                        WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SBARS_SIZEGRIP,
                        NULL, m_hWnd, (UINT)mc_iStatusWndId);
    if (NULL != m_hwndStatusBar)
    {
        ::SendMessage(m_hwndStatusBar, 
                      WM_SETFONT, 
                      (LPARAM)::GetStockObject(DEFAULT_GUI_FONT), FALSE);
    }
    
    // Update the child windows' positions and sizes so that they cover 
    // entire client area of the main window.
    UpdateLayout();
    UpdateStatusBar();

    return true;
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::UpdateLayout
//  
// This helper method is called when the size of the main 
// window has been changed and the child windows' positions 
// need to be updated so that they cover entire client area
// of the main window.
//
// Parameters:
//      none
//
// Return Values (void):
//      none
//
/////////////////////////////////////////////////////////
void CBasicInkAnalysis::UpdateLayout()
{
    RECT rect;
    GetClientRect(&rect);
    
    // update the size and position of the status bar 
    if (::IsWindow(m_hwndStatusBar)
        && ((DWORD)::GetWindowLong(m_hwndStatusBar, GWL_STYLE) & WS_VISIBLE))
    {
        ::SendMessage(m_hwndStatusBar, WM_SIZE, 0, 0);
        RECT rectStatusBar;
        ::GetWindowRect(m_hwndStatusBar, &rectStatusBar);
        if (rect.bottom > rectStatusBar.bottom - rectStatusBar.top)
        {
            rect.bottom -= rectStatusBar.bottom - rectStatusBar.top;
        }
        else
        {
            rect.bottom = 0;
        }
    }


    // update the size and position of the output window
    if (::IsWindow(m_wndResults.m_hWnd))
    {
        int cyResultsWnd = m_wndResults.GetBestHeight();
        if (cyResultsWnd > rect.bottom)
        {
            cyResultsWnd = rect.bottom;
        }
        ::SetWindowPos(m_wndResults.m_hWnd, NULL, 
                       rect.left, rect.bottom - cyResultsWnd,
                       rect.right - rect.left, cyResultsWnd - rect.top,
                       SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        rect.bottom -= cyResultsWnd;
    }
    
    // update the size and position of the ink input window
    if (::IsWindow(m_wndInput.m_hWnd))
    {
        ::SetWindowPos(m_wndInput.m_hWnd, NULL, 
                       rect.left, rect.top,
                       rect.right - rect.left, rect.bottom - rect.top,
                       SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
}

/////////////////////////////////////////////////////////
//                                          
// CBasicInkAnalysis::UpdateStatusBar
//  
// This helper function outputs the colors used for
// showing the layout of the text.
//
// Parameters:
//      none
//
// Return Values (void):
//      none 
//
/////////////////////////////////////////////////////////
void CBasicInkAnalysis::UpdateStatusBar()
{
    if (NULL == m_hwndStatusBar)
        return;

    CComBSTR bstrStatus("Green: Word --- Magenta: Line --- Blue: Paragraph --- Yellow: Writing Region --- Red: Drawing --- Orange: Bullet");
    
    // Set the new text int the status bar
    ::SendMessage(m_hwndStatusBar, SB_SETTEXTW, NULL, (LPARAM)bstrStatus.m_str);
}

