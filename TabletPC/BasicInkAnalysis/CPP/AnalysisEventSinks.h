// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Module:       
//      AnalysisEventSinks.h
//
// Description:
//      This file contains the definitions of the event sink templates,
//      instantiated in the CBasicInkAnalysis class.
//
//      The event source interfaces used are: 
//      _IInkEvents, _IInkRecognitionEvent, _IInkOverlayEvents
//   
//--------------------------------------------------------------------------

#pragma once

class CBasicInkAnalysis;
/////////////////////////////////////////////////////////
//                                          
// IInkAnalyzerEvents
// 
// 
/////////////////////////////////////////////////////////

class ATL_NO_VTABLE InkAnalyzerEvents : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<InkAnalyzerEvents>,
    public _IAnalysisEvents
{

    public:
    DECLARE_NO_REGISTRY()

    BEGIN_COM_MAP(InkAnalyzerEvents)
        COM_INTERFACE_ENTRY(_IAnalysisEvents)
    END_COM_MAP()
    
    // Constructor: initialize memory to null.
    InkAnalyzerEvents()
    {
        m_pIConnectionPointAnalysisEvents = NULL;
        m_pBasicInkAnalysisApp = NULL;
    }
    
    void AttachObj(CBasicInkAnalysis *pBasicInkAnalysisApp)
    {
        this->m_pBasicInkAnalysisApp = pBasicInkAnalysisApp;
    }
    public:
    // _IAnalysisEvents
    STDMETHOD (ResultsUpdated)(
        IInkAnalyzer *pInkAnalyzer,
        IAnalysisStatus *pAnalysisStatus);

    STDMETHOD (IntermediateResultsUpdated)(
        IInkAnalyzer *pInkAnalyzer,
        IAnalysisStatus *pAnalysisStatus);

    STDMETHOD (Activity)();

    STDMETHOD (ReadyToReconcile)();

    STDMETHOD (UpdateStrokesCache)(
        ULONG ulStrokeIdsCount,
        LONG *plStrokeIds );

    //
    // Methods
    //
    
    // Destructor: free resources
    ~InkAnalyzerEvents()
    {
       // UnadviseInkAnalyzer();
    }
    
    // Set up connection between sink and Ink Collector
    HRESULT AdviseInkAnalyzer(
        IInkAnalyzer *pIInkAnalyzer)
    {
        HRESULT hr = S_OK;
        

        // Check to ensure that the sink is not currently connected
        // with another Ink Collector...
        if (NULL == m_pIConnectionPointAnalysisEvents)
        {

            IUnknown *pUnknown = NULL;
            hr = this->QueryInterface(
                IID_IUnknown,
                (void **)&pUnknown);

            if( FAILED(hr) )
            {
                return hr;
            }
            // Get the connection point container
            CComPtr<IConnectionPointContainer> spIConnectionPointContainer;
            hr = pIInkAnalyzer->QueryInterface(
                IID_IConnectionPointContainer, 
                (void **) &spIConnectionPointContainer);
            
            if (FAILED(hr))
            {
                return hr;
            }
            
            // Find the connection point for Ink Collector events
            hr = spIConnectionPointContainer->FindConnectionPoint(
                __uuidof(_IAnalysisEvents), &m_pIConnectionPointAnalysisEvents);
            
            if (SUCCEEDED(hr))
            {
                // Hook up sink to connection point
                hr = m_pIConnectionPointAnalysisEvents->Advise(
                    pUnknown, 
                    &m_dwCookieAnalysisEvents);
            }
            
            if (FAILED(hr))
            {
                // Clean up after an error.
                if (m_pIConnectionPointAnalysisEvents)
                {
                    m_pIConnectionPointAnalysisEvents->Release();
                    m_pIConnectionPointAnalysisEvents = NULL;
                }
            }

        }
        // If the sink is already connected to an _IAnalysisEvents, return a 
        // failure;
        else
        {
            hr = E_FAIL;
        }

        return hr;
    }
    
    // Remove the connection of the sink to the Ink Collector
    HRESULT UnadviseInkAnalyzer()
    {
        HRESULT hr = S_OK;

        // If there the ink collector is connected to the sink,
        // remove it.  Otherwise, do nothing (there is nothing
        // to unadvise).
        if (m_pIConnectionPointAnalysisEvents != NULL)
        {
            hr = m_pIConnectionPointAnalysisEvents->Unadvise(m_dwCookieAnalysisEvents);
            m_pIConnectionPointAnalysisEvents->Release();
            m_pIConnectionPointAnalysisEvents = NULL;
        }

        return hr;
    }
    
    STDMETHOD( FinalConstruct )();

    void FinalRelease();

private:

    //
    //  Data Members
    //
    
    // Connection point on _IAnalysisEvents
    IConnectionPoint *m_pIConnectionPointAnalysisEvents;

    // Cookie returned from advise
    DWORD m_dwCookieAnalysisEvents;

    CBasicInkAnalysis *m_pBasicInkAnalysisApp;
};
