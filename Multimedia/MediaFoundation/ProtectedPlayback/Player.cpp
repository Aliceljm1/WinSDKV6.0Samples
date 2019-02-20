//////////////////////////////////////////////////////////////////////////
// 
// player.cpp : Playback helper class.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include "ProtectedPlayback.h"
#include "ContentEnabler.h"



///////////////////////////////////////////////////////////////////////
//  Name: CreateInstance
//  Description:  Static class method to create the CPlayer object.
//  
//  hVideo:   Handle to the video window.
//  hEvent:   Handle to the window to receive notifications.
//  ppPlayer: Receives an AddRef's pointer to the CPlayer object. 
//            The caller must release the pointer.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateInstance(HWND hVideo, HWND hEvent, CPlayer **ppPlayer)
{
    TRACE((L"CPlayer::Create"));

    assert(hVideo != NULL);
    assert(hEvent != NULL);

    HRESULT hr = S_OK;

    // Note: CPlayer constructor sets the ref count to zero.
    // Create method calls AddRef.

    CPlayer *pPlayer = new CPlayer(hVideo, hEvent, hr);

    if (pPlayer == NULL)
    {
        hr = E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        hr = pPlayer->Initialize();
    }

    if (SUCCEEDED(hr))
    {
        *ppPlayer = pPlayer;
        (*ppPlayer)->AddRef();
    }
    else
    {
        delete pPlayer;
    }

    return hr;
}


///////////////////////////////////////////////////////////////////////
//  CPlayer constructor
/////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer(HWND hVideo, HWND hEvent, HRESULT &hr) : 
    m_pSession(NULL),
    m_pSource(NULL),
    m_pVideoDisplay(NULL),
    m_hwndVideo(hVideo),
    m_hwndEvent(hEvent),
    m_state(Ready),
    m_hCloseEvent(NULL),
    m_nRefCount(0),
    m_pContentProtectionManager(NULL)
{
}


//////////////////////////////////////////////////////////////////////
//  Name: Initialize
//  Initializes the CPlayer object. This method is called by the
//  CreateInstance method.
/////////////////////////////////////////////////////////////////////////
HRESULT CPlayer::Initialize()
{
    HRESULT hr = S_OK;

    m_hCloseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hCloseEvent == NULL)
    {
        hr = __HRESULT_FROM_WIN32(GetLastError());
    }

    // Start up Media Foundation platform.
    if (SUCCEEDED(hr))
    {
        hr = MFStartup(MF_VERSION);
        LOG_IF_FAILED(L"MFStartup", hr);
    }

    return hr;
}



///////////////////////////////////////////////////////////////////////
//  CPlayer destructor
/////////////////////////////////////////////////////////////////////////

CPlayer::~CPlayer()
{
    assert(m_pSession == NULL);  // If FALSE, the app did not call Shutdown().
}


///////////////////////////////////////////////////////////////////////
//  AddRef
/////////////////////////////////////////////////////////////////////////

ULONG CPlayer::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}


///////////////////////////////////////////////////////////////////////
//  Release
/////////////////////////////////////////////////////////////////////////

ULONG CPlayer::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}



///////////////////////////////////////////////////////////////////////
//  QueryInterface
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == IID_IMFAsyncCallback)
    {
        *ppv = static_cast<IMFAsyncCallback*>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}



///////////////////////////////////////////////////////////////////////
//  Name: OpenURL
//  Description:  Opens a URL for playback.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::OpenURL(const WCHAR *sURL)
{
    TRACE((L"CPlayer::OpenURL"));
    TRACE((L"URL = %s", sURL));

    // 1. Create a new media session.
    // 2. Create the media source.
    // 3. Create the topology.
    // 4. Queue the topology [asynchronous]
    // 5. Start playback [asynchronous - does not happen in this method.]

    HRESULT hr = S_OK;
    IMFTopology *pTopology = NULL;

    // Create the media session.
    hr = CreateSession();

    // Create the media source.
    if (SUCCEEDED(hr))
    {
        hr = CreateMediaSource(sURL);
    }

    // Create a partial topology.
    if (SUCCEEDED(hr))
    {
        hr = CreateTopologyFromSource(&pTopology);
    }

    // Set the topology on the media session.
    if (SUCCEEDED(hr))
    {
        hr = m_pSession->SetTopology(0, pTopology);
        LOG_IF_FAILED(L"IMFMediaSession::SetTopology", hr);
    }

    if (SUCCEEDED(hr))
    {
        // Set our state to "open pending"
        m_state = OpenPending;
        NotifyState();
    }
    else
    {
        NotifyError(hr);
        m_state = Ready;
    }

    SAFE_RELEASE(pTopology);

    // If SetTopology succeeded, the media session will queue an 
    // MESessionTopologySet event.

    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: Play
//  Description:  Starts playback from paused state.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Play()
{
    TRACE((L"CPlayer::Pause"));

    if (m_state != Paused)
    {
        return E_FAIL;
    }
    if (m_pSession == NULL || m_pSource == NULL)
    {
        return E_UNEXPECTED;
    }

    HRESULT hr = StartPlayback();

    if (SUCCEEDED(hr))
    {
        m_state = StartPending;
        NotifyState();
    }
    else
    {
        NotifyError(hr);
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////
//  Name: Pause
//  Description:  Paused playback.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Pause()
{
    TRACE((L"CPlayer::Pause"));

    if (m_state != Started)
    {
        return E_FAIL;
    }
    if (m_pSession == NULL || m_pSource == NULL)
    {
        return E_UNEXPECTED;
    }

    HRESULT hr = m_pSession->Pause();
    LOG_IF_FAILED(L"IMFMediaSession::Pause", hr);

    if (SUCCEEDED(hr))
    {
        m_state = PausePending;
        NotifyState();
    }
    else
    {
        NotifyError(hr);
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////
//  Name: Repaint
//  Description:  Repaint the video window.
//
//  Note: The application should call this method when it receives a
//        WM_PAINT message.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Repaint()
{
    HRESULT hr = S_OK;

    if (m_pVideoDisplay)
    {
        hr = m_pVideoDisplay->RepaintVideo();
    }
    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: ResizeVideo
//  Description:  Repaint the video window.
//
//  Note: The application should call this method when it receives a
//        WM_SIZE message.
/////////////////////////////////////////////////////////////////////////

HRESULT	CPlayer::ResizeVideo(WORD width, WORD height)
{
    HRESULT hr = S_OK;

    if (m_pVideoDisplay)
    {
        MFVideoNormalizedRect nRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        RECT rcDest = { 0, 0, width, height };

        hr = m_pVideoDisplay->SetVideoPosition(&nRect, &rcDest);
    }
    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: Invoke
//  Description:  Callback for asynchronous BeginGetEvent method.
//  
//  pAsyncResult: Pointer to the result.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Invoke(IMFAsyncResult *pResult)
{
    HRESULT hr = S_OK;
    IMFMediaEvent* pEvent = NULL;
    MediaEventType meType = MEUnknown;  // Event type
    HRESULT hrStatus = S_OK;	        // Event status

    MF_TOPOSTATUS TopoStatus = MF_TOPOSTATUS_INVALID; // Used with MESessionTopologyStatus event.    

    // Get the event from the event queue.
    hr = m_pSession->EndGetEvent(pResult, &pEvent);
    LOG_IF_FAILED(L"IMediaEventGenerator::EndGetEvent", hr);

    // Get the event type.
    if (SUCCEEDED(hr))
    {
        hr = pEvent->GetType(&meType);
        LOG_IF_FAILED(L"IMFMediaEvent::GetType", hr);
    }

    // Get the event status. If the operation that triggered the event did
    // not succeed, the status is a failure code.
    if (SUCCEEDED(hr))
    {
        hr = pEvent->GetStatus(&hrStatus);
        LOG_IF_FAILED(L"IMFMediaEvent::GetStatus", hr);
    }

    if (SUCCEEDED(hr))
    {
        TRACE((L"Media event: %s", EventName(meType)));


        // Check if the async operation succeeded.
        if (SUCCEEDED(hrStatus))
        {
            // Switch on the event type. Update the internal state of the CPlayer as needed.
            switch(meType)
            {
            case MESessionTopologyStatus:
                // Get the status code.
                hr = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, (UINT32*)&TopoStatus);
                if (SUCCEEDED(hr))
                {
                    switch (TopoStatus)
                    {
                    case MF_TOPOSTATUS_READY: 
                        hr = OnTopologyReady(pEvent);
                        break;
                    default: 
                        // Nothing to do.
		        break;  
                    }
                }
                break;

            case MESessionStarted:
                hr = OnSessionStarted(pEvent);
                break;

            case MESessionPaused:
                hr = OnSessionPaused(pEvent);
                break;

            case MESessionClosed:
                hr = OnSessionClosed(pEvent);
                break;

            case MEEndOfPresentation:
                hr = OnPresentationEnded(pEvent);
                break;
            }

        }
        else
        {
            // The async operation failed. Notify the application
            NotifyError(hrStatus);
        }
    }

    // Request another event.
    if (meType != MESessionClosed)
    {
        hr = m_pSession->BeginGetEvent(this, NULL);
        LOG_IF_FAILED(L"IMFMediaSession::BeginGetEvent", hr);
    }

    SAFE_RELEASE(pEvent);

    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: ShutDown
//  Description:  Releases all resources held by this object.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Shutdown()
{
    TRACE((L"CPlayer::ShutDown"));

    HRESULT hr = S_OK;

    // Close the session
    hr = CloseSession();

    // Shutdown the Media Foundation platform
    MFShutdown();

    CloseHandle(m_hCloseEvent);


    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: GetContentProtectionManager
//  Description:  Returns the content protection manager object.
//
//  This is a helper object for handling IMFContentEnabler operations.
/////////////////////////////////////////////////////////////////////////

HRESULT  CPlayer::GetContentProtectionManager(ContentProtectionManager **ppManager)
{
    if (ppManager == NULL)
    {
        return E_INVALIDARG;
    }

    if (m_pContentProtectionManager == NULL)
    {
        return E_FAIL; // Session wasn't created yet. No helper object;
    }

    *ppManager = m_pContentProtectionManager;
    (*ppManager)->AddRef();

    return S_OK;

}


///
///
/// Protected methods
///
/// All methods that follow are private to the CPlayer class.
///
///


///////////////////////////////////////////////////////////////////////
//  Name: OnTopologyReady
//  Description:  Handler for MESessionTopologyReady event.
//
//  Note: 
//  - The MESessionTopologySet event means the session queued the 
//    topology, but the topology is not ready yet. Generally, the 
//    applicationno need to respond to this event unless there is an
//    error.
//  - The MESessionTopologyReady event means the new topology is
//    ready for playback. After this event is received, any calls to
//    IMFGetService will get service interfaces from the new topology.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::OnTopologyReady(IMFMediaEvent *pEvent)
{
    TRACE((L"CPlayer::OnTopologyReady"));

    // Ask for the IMFVideoDisplayControl interface.
    // This interface is implemented by the EVR and is
    // exposed by the media session as a service.

    // Note: This call is expected to fail if the source
    // does not have video.

    MFGetService(
        m_pSession,
        MR_VIDEO_RENDER_SERVICE,
        __uuidof(IMFVideoDisplayControl),
        (void**)&m_pVideoDisplay
        );

    HRESULT hr = StartPlayback();
    if (FAILED(hr))
    {
        NotifyError(hr);
    }

    // If we succeeded, the Start call is pending. Don't notify the app yet.

    return S_OK;
}


///////////////////////////////////////////////////////////////////////
//  Name: OnSessionStarted
//  Description:  Handler for MESessionStarted event.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::OnSessionStarted(IMFMediaEvent *pEvent)
{
    TRACE((L"CPlayer::OnSessionStarted"));

    m_state = Started;
    NotifyState();

    return S_OK;
}


///////////////////////////////////////////////////////////////////////
//  Name: OnSessionPaused
//  Description:  Handler for MESessionPaused event.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::OnSessionPaused(IMFMediaEvent *pEvent)
{
    TRACE((L"CPlayer::OnSessionPaused"));

    m_state = Paused;
    NotifyState();

    return S_OK;
}


///////////////////////////////////////////////////////////////////////
//  Name: OnSessionClosed
//  Description:  Handler for MESessionClosed event.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::OnSessionClosed(IMFMediaEvent *pEvent)
{
    TRACE((L"CPlayer::OnSessionClosed"));

    // The application thread is waiting on this event, inside the 
    // CPlayer::CloseSession method. 
    SetEvent(m_hCloseEvent);
    return S_OK;
}



HRESULT CPlayer::OnPresentationEnded(IMFMediaEvent *pEvent)
{
    TRACE((L"CPlayer::OnPresentationEnded"));

    // The session puts itself into the stopped state autmoatically.

    m_state = Ready;
    NotifyState();

    return S_OK;
}


///////////////////////////////////////////////////////////////////////
//  Name: CreateSession
//  Description:  Creates a new instance of the media session.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateSession()
{
    TRACE((L"CPlayer::CreateSession"));

    IMFAttributes *pAttributes = NULL;
    IMFActivate   *pEnablerActivate = NULL;

    // Close the old session, if any.
    HRESULT hr = CloseSession();

    // Create a new attribute store.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateAttributes(&pAttributes, 1);
    }

    // Create the content protection manager.
    if (SUCCEEDED(hr))
    {
        assert(m_pContentProtectionManager == NULL); // Was released in CloseSession

        hr = ContentProtectionManager::CreateInstance(
            m_hwndEvent, 
            &m_pContentProtectionManager
            );
    }

    // Set the MF_SESSION_CONTENT_PROTECTION_MANAGER attribute with a pointer
    // to the content protection manager.
    if (SUCCEEDED(hr))
    {
        hr = pAttributes->SetUnknown(
            MF_SESSION_CONTENT_PROTECTION_MANAGER, 
            (IMFContentProtectionManager*)m_pContentProtectionManager
            );
    }

    // Create the PMP media session.
    if (SUCCEEDED(hr))
    {
        hr = MFCreatePMPMediaSession(
            0, // Can use this flag: MFPMPSESSION_UNPROTECTED_PROCESS
            pAttributes, 
            &m_pSession,
            &pEnablerActivate
            );
        LOG_IF_FAILED(L"MFCreatePMPMediaSession", hr);
    }

    // Start pulling events from the media session
    if (SUCCEEDED(hr))
    {
        hr = m_pSession->BeginGetEvent((IMFAsyncCallback*)this, NULL);
    }
    else
    {
		// TODO:

        // If MFCreatePMPMediaSession fails it might return an IMFActivate pointer.
		// This indicates that a trusted binary failed to load in the protected process.
        // An application can use the IMFActivate pointer to create an enabler object, which 
		// provides revocation and renewal information for the component that failed to
		// load. 

		// This sample does not demonstrate that feature. Instead, we simply treat this
		// case as a playback failure. 
    }

    SAFE_RELEASE(pAttributes);
    SAFE_RELEASE(pEnablerActivate);
    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: CloseSession
//  Description:  Closes the media session. 
//
//  Note: The IMFMediaSession::Close method is asynchronous, but the
//        CPlayer::CloseSession method waits on the MESessionClosed event.
//        The MESessionClosed event is guaranteed to be the last event 
//        that the media session fires.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CloseSession()
{
    HRESULT hr = S_OK;

    SAFE_RELEASE(m_pVideoDisplay);

    if (m_pSession)
    {
        hr = m_pSession->Close();
        LOG_IF_FAILED(L"IMFMediaSession::Close", hr);

        // Wait for the close operation to complete
        if (SUCCEEDED(hr))
        {
            DWORD res = WaitForSingleObject(m_hCloseEvent, 5000);
            if (res == WAIT_TIMEOUT)
            {
                TRACE((L"WaitForSingleObject timed out!"));
            }
        }
    }

    // Complete shutdown operations

    // 1. Shut down the media source
    if (m_pSource)
    {
        m_pSource->Shutdown();
    }

    // 2. Shut down the media session. (Synchronous operation, no events.)
    if (m_pSession)
    {
        m_pSession->Shutdown();
    }

    SAFE_RELEASE(m_pSource);
    SAFE_RELEASE(m_pSession);
    SAFE_RELEASE(m_pContentProtectionManager);

    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: StartPlayback
//  Description:  Starts playback from the current position. 
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::StartPlayback()
{
    TRACE((L"CPlayer::StartPlayback"));

    assert(m_pSession != NULL);

    HRESULT hr = S_OK;

    PROPVARIANT varStart;
    PropVariantInit(&varStart);

    varStart.vt = VT_EMPTY;

    hr = m_pSession->Start(&GUID_NULL, &varStart);
    LOG_IF_FAILED(L"IMFMediaSession::Start", hr);

    // Not strictly needed here but good form...
    PropVariantClear(&varStart);

    return hr;
}


///////////////////////////////////////////////////////////////////////
//  Name: CreateMediaSource
//  Description:  Create a media source from a URL.
//
//  sURL: The URL to open.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateMediaSource(const WCHAR *sURL)
{
    TRACE((L"CPlayer::CreateMediaSource"));

    HRESULT hr = S_OK;

    IMFSourceResolver* pSourceResolver = NULL;
    IUnknown* pSource = NULL;

    // Create the source resolver.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateSourceResolver(&pSourceResolver);
        LOG_IF_FAILED(L"MFCreateSourceResolver", hr);
    }

    // Use the source resolver to create the media source.
    if (SUCCEEDED(hr))
    {
        MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

        hr = pSourceResolver->CreateObjectFromURL(
                sURL,						// URL of the source.
                MF_RESOLUTION_MEDIASOURCE,	// Create a source object.
                NULL, 				// Optional property store.
                &ObjectType,				// Receives the created object type. 
                &pSource					// Receives a pointer to the media source.
            );
        LOG_IF_FAILED(L"IMFSourceResolver::CreateObjectFromURL", hr);
    }

    // Get the IMFMediaSource interface from the media source.
    if (SUCCEEDED(hr))
    {
        hr = pSource->QueryInterface(__uuidof(IMFMediaSource), (void**)&m_pSource);
    }

    // Clean up
    SAFE_RELEASE(pSourceResolver);
    SAFE_RELEASE(pSource);

    return hr;
}



///////////////////////////////////////////////////////////////////////
//  CreateTopologyFromSource
//  Description:  Create a playback topology from the media source.
//
//  Pre-condition: The media source must be created already.
//                 Call CreateMediaSource() before calling this method.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateTopologyFromSource(IMFTopology **ppTopology)
{
    TRACE((L"CPlayer::CreateTopologyFromSource"));

    assert(m_pSession != NULL);
    assert(m_pSource != NULL);

    HRESULT hr = S_OK;

    IMFTopology *pTopology = NULL;
    IMFPresentationDescriptor* pSourcePD = NULL;
    DWORD cSourceStreams = 0;

    // Create a new topology.
    hr = MFCreateTopology(&pTopology);
    LOG_IF_FAILED(L"MFCreateTopology", hr);

    // Create the presentation descriptor for the media source.
    if (SUCCEEDED(hr))
    {
        hr = m_pSource->CreatePresentationDescriptor(&pSourcePD);
        LOG_IF_FAILED(L"IMFMediaSource::CreatePresentationDescriptor", hr);
    }

    // Get the number of streams in the media source.
    if (SUCCEEDED(hr))
    {
        hr = pSourcePD->GetStreamDescriptorCount(&cSourceStreams);
        LOG_IF_FAILED(L"IMFPresentationDescriptor::GetStreamDescriptorCount", hr);
    }

    TRACE((L"Stream count: %d", cSourceStreams));

    // For each stream, create the topology nodes and add them to the topology.
    if (SUCCEEDED(hr))
    {
        for (DWORD i = 0; i < cSourceStreams; i++)
        {
            hr = AddBranchToPartialTopology(pTopology, pSourcePD, i);
            if (FAILED(hr))
            {
                break;
            }
        }
    }

    // Return the IMFTopology pointer to the caller.
    if (SUCCEEDED(hr))
    {
        *ppTopology = pTopology;
        (*ppTopology)->AddRef();
    }

    SAFE_RELEASE(pTopology);
    SAFE_RELEASE(pSourcePD);

    return hr;
}





///////////////////////////////////////////////////////////////////////
//  Name:  AddBranchToPartialTopology 
//  Description:  Adds a topology branch for one stream.
//
//  pTopology: Pointer to the topology object.
//  pSourcePD: The source's presentation descriptor.
//  iStream: Index of the stream to render.
//
//  Pre-conditions: The topology must be created already.
//
//  Notes: For each stream, we must do the following:
//    1. Create a source node associated with the stream. 
//    2. Create an output node for the renderer. 
//    3. Connect the two nodes.
//  The media session will resolve the topology, so we do not have
//  to worry about decoders or other transforms.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::AddBranchToPartialTopology(IMFTopology *pTopology, IMFPresentationDescriptor *pSourcePD, DWORD iStream)
{
    TRACE((L"CPlayer::AddBranchToPartialTopology"));

    assert(pTopology != NULL);

    IMFStreamDescriptor* pSourceSD = NULL;
    IMFTopologyNode* pSourceNode = NULL;
    IMFTopologyNode* pOutputNode = NULL;
    BOOL fSelected = FALSE;

    HRESULT hr = S_OK;

    // Get the stream descriptor for this stream.
    hr = pSourcePD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSourceSD);
    LOG_IF_FAILED(L"IMFPresentationDescriptor::GetStreamDescriptorByIndex", hr);

    if (SUCCEEDED(hr))
    {
        // Create the topology branch only if the stream is selected.
        // Otherwise, do nothing.
        if (fSelected)
        {
            // Create a source node for this stream.
            hr = CreateSourceStreamNode(pSourcePD, pSourceSD, &pSourceNode);

            // Create the output node for the renderer.
            if (SUCCEEDED(hr))
            {
                hr = CreateOutputNode(pSourceSD, &pOutputNode);
            }

            // Add both nodes to the topology.
            if (SUCCEEDED(hr))
            {
                hr = pTopology->AddNode(pSourceNode);
                LOG_IF_FAILED(L"IMFTopology::AddNode", hr);
            }

            if (SUCCEEDED(hr))
            {
                hr = pTopology->AddNode(pOutputNode);
                LOG_IF_FAILED(L"IMFTopology::AddNode", hr);
            }

            // Connect the source node to the output node.
            if (SUCCEEDED(hr))
            {
                hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
                LOG_IF_FAILED(L"IMFTopologyNode::ConnectOutput", hr);
            }
        }
    }

    // Clean up.
    SAFE_RELEASE(pSourceSD);
    SAFE_RELEASE(pSourceNode);
    SAFE_RELEASE(pOutputNode);

    return hr;

}



///////////////////////////////////////////////////////////////////////
//  Name: CreateSourceStreamNode
//  Description:  Creates a source-stream node for a stream.
// 
//  pSourcePD: Presentation descriptor for the media source.
//  pSourceSD: Stream descriptor for the stream.
//  ppNode: Receives a pointer to the new node.
//
//  Pre-conditions: Create the media source.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateSourceStreamNode(
    IMFPresentationDescriptor *pSourcePD, 
    IMFStreamDescriptor *pSourceSD,
    IMFTopologyNode **ppNode
    )
{
    assert(m_pSource != NULL);

    IMFTopologyNode *pNode = NULL;
    HRESULT hr = S_OK;

    // Create the source-stream node. 
    hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
    LOG_IF_FAILED(L"MFCreateTopologyNode", hr);

    // Set attribute: Pointer to the media source.
    if (SUCCEEDED(hr))
    {
        hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, m_pSource);
    }

    // Set attribute: Pointer to the presentation descriptor.
    if (SUCCEEDED(hr))
    {
        hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pSourcePD);
    }

    // Set attribute: Pointer to the stream descriptor.
    if (SUCCEEDED(hr))
    {
        hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSourceSD);
    }

    // Return the IMFTopologyNode pointer to the caller.
    if (SUCCEEDED(hr))
    {
        *ppNode = pNode;
        (*ppNode)->AddRef();
    }

    // Clean up.
    SAFE_RELEASE(pNode);

    return hr;
}



///////////////////////////////////////////////////////////////////////
//  Name: CreateOutputNode
//  Description:  Create an output node for a stream.
//
//  pSourceSD: Stream descriptor for the stream.
//  ppNode: Receives a pointer to the new node.
//
//  Notes:
//  This function does the following:
//  1. Chooses a renderer based on the media type of the stream.
//  2. Creates an IActivate object for the renderer.
//  3. Creates an output topology node.
//  4. Sets the IActivate pointer on the node.
/////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateOutputNode(
    IMFStreamDescriptor *pSourceSD, 
    IMFTopologyNode **ppNode
    )
{	

    IMFTopologyNode *pNode = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFActivate *pRendererActivate = NULL;

    GUID guidMajorType = GUID_NULL;
    HRESULT hr = S_OK;

    // Get the stream ID.
    DWORD streamID = 0;
    pSourceSD->GetStreamIdentifier(&streamID); // Just for debugging, ignore any failures.
    LOG_IF_FAILED(L"IMFStreamDescriptor::GetStreamIdentifier", hr);

    // Get the media type handler for the stream.
    hr = pSourceSD->GetMediaTypeHandler(&pHandler);
    LOG_IF_FAILED(L"IMFStreamDescriptor::GetMediaTypeHandler", hr);
    
    // Get the major media type.
    if (SUCCEEDED(hr))
    {
        hr = pHandler->GetMajorType(&guidMajorType);
        LOG_IF_FAILED(L"IMFMediaTypeHandler::GetMajorType", hr);
    }

    // Create a downstream node.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
        LOG_IF_FAILED(L"MFCreateTopologyNode", hr);
    }

    // Create an IMFActivate object for the renderer, based on the media type.
    if (SUCCEEDED(hr))
    {
        if (MFMediaType_Audio == guidMajorType)
        {
            // Create the audio renderer.
            TRACE((L"Stream %d: audio stream", streamID));
            hr = MFCreateAudioRendererActivate(&pRendererActivate);
            LOG_IF_FAILED(L"MFCreateAudioRendererActivate", hr);
        }
        else if (MFMediaType_Video == guidMajorType)
        {
            // Create the video renderer.
            TRACE((L"Stream %d: video stream", streamID));
            hr = MFCreateVideoRendererActivate(m_hwndVideo, &pRendererActivate);
            LOG_IF_FAILED(L"MFCreateVideoRendererActivate", hr);
        }
        else
        {
            TRACE((L"Stream %d: Unknown format", streamID));
            hr = E_FAIL;
        }
    }

    // Set the IActivate object on the output node.
    if (SUCCEEDED(hr))
    {
        hr = pNode->SetObject(pRendererActivate);
        LOG_IF_FAILED(L"IMFTopologyNode::SetObject", hr);
    }

    // Return the IMFTopologyNode pointer to the caller.
    if (SUCCEEDED(hr))
    {
        *ppNode = pNode;
        (*ppNode)->AddRef();
    }

    // Clean up.
    SAFE_RELEASE(pNode);
    SAFE_RELEASE(pHandler);
    SAFE_RELEASE(pRendererActivate);

    return hr;
}

