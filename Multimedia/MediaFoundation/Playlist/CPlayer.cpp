#include "CPlayer.h"

// ----- Public Methods -----------------------------------------------
//////////////////////////////////////////////////////////////////////////
//  Name: CPlayer
//  Description: Constructor
//
/////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer(HWND hWnd)
:   m_hWnd (hWnd),
    m_nRefCount(1),
    m_pMediaSession (NULL), 
    m_pSequencerSource (NULL),
    m_pAudioRendererActivate (NULL),
    m_pPresentationClock (NULL),
    m_PresentationTimeOffset (0),
    m_phnsTimePairStart (0),
    m_phnsTimePairEnd (0),
    m_State(PlayerCreated),
	m_ActiveSegment(0)
{
}


//////////////////////////////////////////////////////////////////////////
//  Name: ~CPlayer
//  Description: Destructor
//
//  -Calls Shutdown
/////////////////////////////////////////////////////////////////////////

CPlayer::~CPlayer()
{
    this->ShutDown();
}


//////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::Initialize
//  Description: 
//      Intializes Media Foundation
//      Creates a media session
//      Creates a sequencer source
//      Creates a presentation clock
//      Creates an audio renderer
//      Starts the event queue
//
/////////////////////////////////////////////////////////////////////////
HRESULT CPlayer::Initialize()
{
    TRACE((L"\nCPlayer::Initialize"));

    HRESULT hr = S_OK;

    IMFClock *pClock = NULL;

    // Initialize Media Foundation.
    hr = MFStartup(MF_VERSION);

    // Create the media session.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateMediaSession(NULL, &m_pMediaSession);
        LOG_IF_FAILED(L"MFCreateMediaSession", hr);
    }

    // Start the event queue. 
    if (SUCCEEDED(hr))
    {
        hr  = m_pMediaSession->BeginGetEvent(this, NULL);
    }

    // Create a sequencer Source.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateSequencerSource(NULL, &m_pSequencerSource);
        LOG_IF_FAILED(L"MFCreateSequencerSource", hr);
    }

    //setup clock
    if (SUCCEEDED(hr))
    {
        hr = m_pMediaSession->GetClock(&pClock);
    }

    if (SUCCEEDED(hr))
    {
        hr = pClock->QueryInterface( IID_IMFPresentationClock,
                        (void**) &m_pPresentationClock);
    }
    // Create an IMFActivate object for the audio renderer.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateAudioRendererActivate(&m_pAudioRendererActivate);
        LOG_IF_FAILED(L"MFCreateAudioRendererActivate", hr);
    }

    //Set the player state to Initialized
    m_State = Initialized;
    
    // Notify the app that the player is initialized.
    PostMessage(m_hWnd, WM_NOTIFY_APP, PLAYER_INITIALIZED, m_State);

    //Clean up.

    SAFE_RELEASE( pClock);
    
    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::AddToPlaylist (Public)
//  Description: 
//      Adds a new segment to the playlist.
//      If the new segment is the first one to be added to the sequencer:
//          Queues it on the media session.
//      Otherwise:
//          Resets the last topology flag and sets it to the newly added segment
//          
//  Parameter:  
//      sURL: [in] File URL
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::AddToPlaylist(const WCHAR *sURL)
{
    TRACE((L"\nCPlayer::AddToPlaylist"));

    if(!sURL)
        return E_POINTER;
    
    HRESULT hr = S_OK;

    IMFPresentationDescriptor *pPresentationDescriptor = NULL;
    IMFMediaSource *pMediaSource = NULL;


    MFSequencerElementId SegmentId = 0;

    if (m_Segments.GetCount()!=0)
    {
        //Get the last segment id
        m_Segments.GetLastSegmentId(&SegmentId);

        //reset the last topology in the sequencer
        hr = this->m_pSequencerSource->UpdateTopologyFlags(SegmentId, 0);
    }

    //Create media source and topology, and add it to the sequencer
    if (SUCCEEDED(hr))
    {
        hr = AddSegment(sURL, &SegmentId);
    }

    //Set the last topology
    if (SUCCEEDED(hr))
    {
        hr = this->m_pSequencerSource->UpdateTopologyFlags(SegmentId, SequencerTopologyFlags_Last);
    }

    //If this is the first segment in the sequencer, queue it on the session
    if (m_Segments.GetCount() == 1)
    {
        
        hr = m_pSequencerSource->QueryInterface(
                                __uuidof(IMFMediaSource), 
                                (void**)&pMediaSource);

        if (SUCCEEDED(hr))
        {
            hr = pMediaSource->CreatePresentationDescriptor(&pPresentationDescriptor);
        }

        hr = this->QueueNextSegment (pPresentationDescriptor, &SegmentId);
    }

    //Notify the app
    if (SUCCEEDED(hr))
    {
        PostMessage(m_hWnd, WM_NOTIFY_APP, PLAYER_SEGMENT_ADDED, SegmentId);
    }


    SAFE_RELEASE(pMediaSource);
    SAFE_RELEASE(pPresentationDescriptor);


    return hr;
    
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::DeleteSegment (Public)
//  Description: 
//      Deletes the corresponding topology from the sequencer source
//  Parameter:
//      SegmentID: [in] The segment identifier
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::DeleteSegment(MFSequencerElementId SegmentID)
{
    TRACE((L"\nCPlayer::DeleteSegment: %d", SegmentID));

	if(m_ActiveSegment == SegmentID)
	{
		return E_ACCESSDENIED;
	}

    HRESULT hr = S_OK;

    MFSequencerElementId SegId = 0;

    hr  = m_pSequencerSource->DeleteTopology(SegmentID);

    if(SUCCEEDED(hr))
    {
        hr = m_Segments.GetLastSegmentId(&SegId);
    }

    //Delete the segment entry from the list.
    if(SUCCEEDED(hr))
    {
        hr =  m_Segments.DeleteSegmentEntry(SegmentID);
    }
    
    //Is the deleted topology the last one?
    if((SUCCEEDED(hr)) && (SegId == SegmentID))
    {
        //Get the new last segment id
        hr = m_Segments.GetLastSegmentId(&SegId);

        //set this topology as the last in the sequencer
        hr = this->m_pSequencerSource->UpdateTopologyFlags(SegId, SequencerTopologyFlags_Last);
    }

    if(SUCCEEDED(hr))
    {
        PostMessage(m_hWnd, WM_NOTIFY_APP, PLAYER_SEGMENT_DELETED, SegmentID);
    }

    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::Play (Public)
//  Description: 
//      Starts the media session with the current topology
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Play()
{
    TRACE((L"\nCPlayer::Play"));


    HRESULT hr = S_OK;

    // Create the starting position parameter
    PROPVARIANT var;
    PropVariantInit(&var );

    var.vt = VT_EMPTY;

    hr = m_pMediaSession->Start(&GUID_NULL, &var );
    
	LOG_IF_FAILED(L"IMFMediaSession::Start", hr);

    PropVariantClear( &var );

    return hr;

}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::Pause (Public)
//  Description: 
//      Pauses the media session with the current topology
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Pause(void)
{
    TRACE((L"\nCPlayer::Pause"));

    HRESULT hr = S_OK;

     // pause the media session.
    hr = m_pMediaSession->Pause();
    LOG_IF_FAILED(L"IMFMediaSession::Pause", hr);

    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::Stop (Public)
//  Description: 
//      Stops the media session with the current topology
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Stop(void)
{
    TRACE((L"CPlayer::Stop"));

    HRESULT hr = S_OK;

    hr = m_pMediaSession->Stop();
    LOG_IF_FAILED(L"IMFMediaSession::Start from skip", hr);

    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::Skip (Public)
//  Description: 
//      Skips to the specified segment in the sequencer source
//  Parameter:
//      SegmentID: [in] The segment identifier
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::Skip (const MFSequencerElementId SegmentID)
{
    TRACE((L"\nCPlayer::Skip"));

    HRESULT hr = S_OK;

    PROPVARIANT var;
    PropVariantInit (&var);
   
	this->m_pMediaSession->Stop();

    hr = MFCreateSequencerSegmentOffset(SegmentID, NULL, &var);
    
    if (SUCCEEDED(hr))
    {
        hr = m_pMediaSession->Start(&MF_TIME_FORMAT_SEGMENT_OFFSET, &var);
        LOG_IF_FAILED(L"IMFMediaSession::Start from skip", hr);
    }

    PropVariantClear(&var);

    return hr;
}



////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::GetCurrentSegmentTime (Public)
//  Description: 
//      Gets the lastest play time of the current segment
//  Parameter:
//      phnsCurrentTime: [out] Playtime of the current segment
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::GetCurrentSegmentTime(MFTIME *phnsCurrentTime)
{
    if (!phnsCurrentTime)
    {
        return E_POINTER;
    }
    
    HRESULT hr = S_OK;

    hr = m_pPresentationClock->GetTime(phnsCurrentTime);
    
    if (m_phnsTimePairStart)
    {
        if(*phnsCurrentTime >= this->m_phnsTimePairStart->hnsStartPresentationTime)
        {
            //update m_PresentationTimeOffset
            m_PresentationTimeOffset = this->m_phnsTimePairStart->hnsPresentationTimeOffset;

            TimePair *temp = this->m_phnsTimePairStart;
            
            this->m_phnsTimePairStart = this->m_phnsTimePairStart->pNextTimePair; 
    
            delete temp;
        }

    }

    *phnsCurrentTime -= m_PresentationTimeOffset;

    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::GetSegmentInfo (Public)
//  Description: 
//      Gets the segment info based on segment identifier: Duration, segment name.
//  Parameter:  
//      SegmentID: [in] Segment identifier
//      phnsSegmentDuration: [out] Receives the segment duration
//      szSegmentURL: [out] Receives segment name
//      dwSize: [in] Size of szSegmentURL
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::GetSegmentInfo (
                    MFSequencerElementId SegmentID, 
                    MFTIME *phnsSegmentDuration, 
                    WCHAR *szSegmentURL, 
                    DWORD dwSize)
{
    HRESULT hr = S_OK;
    
    hr = m_Segments.GetSegmentEntryInfo(SegmentID, phnsSegmentDuration, szSegmentURL, dwSize);

    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::ShutDown (Public)
//  Description: 
//      Releases all resources and shuts down Media Foundation
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::ShutDown()
{
    TRACE((L"\nCPlayer::ShutDown"));

    HRESULT hr = S_OK;

    IMFMediaSource *pMediaSource = NULL;

    //Call shutdown on the sequencer source
    hr = m_pSequencerSource->QueryInterface(
                                __uuidof(IMFMediaSource), 
                                (void**)&pMediaSource);

    if (SUCCEEDED(hr))
    {
        hr = pMediaSource->Shutdown();
        LOG_IF_FAILED(L"IMFMediaSource::Shutdown on the sequencer source", hr);
    }


   //Close media session
    if (m_pMediaSession)
    {
        hr = m_pMediaSession->Close();
        LOG_IF_FAILED(L"IMFMediaSession::Close", hr);
    }

    // Wait for the close operation to complete
    if (SUCCEEDED(hr))
    {
        DWORD res = WaitForSingleObject(m_hCloseEvent, 5000);
        if (res == WAIT_TIMEOUT)
        {
            TRACE((L"WaitForSingleObject timed out!"));
        }
    }

    //Shutdown media session

    if (SUCCEEDED(hr))
    {
        hr = m_pMediaSession->Shutdown();
        LOG_IF_FAILED(L"IMFMediaSession::Shutdown", hr);
    }
    
    // Shut down Media Foundation.
    if SUCCEEDED(hr)
    {
        hr = MFShutdown();
    }
    
    SAFE_RELEASE(pMediaSource);
    SAFE_RELEASE(m_pMediaSession);
    SAFE_RELEASE(m_pSequencerSource);
    SAFE_RELEASE(m_pPresentationClock);
    SAFE_RELEASE(m_pAudioRendererActivate);
    
    return hr;
}

 //IMFAsyncCallback methods
    
STDMETHODIMP_(ULONG) CPlayer::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}
    
STDMETHODIMP_(ULONG) CPlayer::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}
    
STDMETHODIMP CPlayer::QueryInterface(REFIID iid, void** ppv)
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
    
       

//////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::Invoke
//  Description: 
//      Implementation of CAsyncCallback::Invoke.
//      Callback for asynchronous BeginGetEvent method.
//  Parameter:
//      pAsyncResult: Pointer to the result.        
//
/////////////////////////////////////////////////////////////////////////

STDMETHODIMP CPlayer::Invoke(IMFAsyncResult *pAsyncResult)
{
    HRESULT hr = S_OK;
    BOOL fGetAnotherEvent = TRUE;

    IMFMediaEvent* pEvent = NULL;

    MediaEventType eventType = MEUnknown;   // Event type
    PROPVARIANT eventData;                  // Event data
    HRESULT eventStatus = S_OK;             // Event status


    PropVariantInit(&eventData);

    // Get the event from the event queue.
    hr = this->m_pMediaSession->EndGetEvent(pAsyncResult, &pEvent);
    LOG_IF_FAILED(L"IMediaEventGenerator::EndGetEvent", hr);

    if (SUCCEEDED(hr))
    {
        // Get the event type.
        hr = pEvent->GetType(&eventType);
        LOG_IF_FAILED(L"IMFMediaEvent::GetType", hr);

    }
    if (SUCCEEDED(hr))
    {
        // Get the event data
        hr = pEvent->GetValue(&eventData);
        LOG_IF_FAILED(L"IMFMediaEvent::GetValue", hr);

    }
    if (SUCCEEDED(hr))
    {
        // Get the event status. If the operation that triggered the event
        // did not succeed, the status is a failure code.
        hr = pEvent->GetStatus(&eventStatus);
    }

    if (SUCCEEDED(hr))
    {
        // Switch on the event type. Update the internal state of the CPlayer
        // as needed.

        switch(eventType)
        {
            // Session events
            case MESessionStarted:
                {
                    TRACE((L"%d: MESessionStarted, Status: 0x%X", eventType, eventStatus ));

                    m_State = Playing;

                    PostMessage(m_hWnd, WM_NOTIFY_APP, eventType, m_State);
                    
                    break;
                }
            case MESessionPaused:
                {
                    TRACE((L"%d: MESessionPaused, Status: 0x%X", eventType, eventStatus ));

                    m_State = Paused;
                    PostMessage(m_hWnd, WM_NOTIFY_APP, eventType, m_State);
                    
                    break;
                }
            case MESessionStopped:
                {
                    TRACE((L"%d: MESessionStopped, Status: 0x%X", eventType, eventStatus ));

                    m_State = Stopped;
                    PostMessage(m_hWnd, WM_NOTIFY_APP, eventType, m_State);
                    
                    break;
                }
            
            case MESessionTopologyStatus:
                {
                    TRACE((L"%d: MESessionTopologyStatus, Status: 0x%X", eventType, eventStatus ));
					
					UINT value = 0;
					
					hr = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &value);
					MFSequencerElementId SegmentID = 0;
					TOPOID ID;
					
					//Get information about the new segment
					IMFTopology *pTopology = NULL;
                    
					hr = eventData.punkVal->QueryInterface(
							__uuidof(IMFTopology),
							(void**)&pTopology);

					if (SUCCEEDED(hr))
					{
						pTopology->GetTopologyID(&ID);
					}
					if (SUCCEEDED(hr))
					{
						m_Segments.GetSegmentIDByTopoID(ID, &SegmentID);
					}
                    
					topostat.iTopologyStatusType = value;
					topostat.iSegmentId = SegmentID;

					switch(value)
					{
					case MF_TOPOSTATUS_STARTED_SOURCE:
							m_ActiveSegment = SegmentID;
							break;
						
						case MF_TOPOSTATUS_ENDED:
							m_ActiveSegment = 0;
							break;

					}
				
					PostMessage(m_hWnd, WM_NOTIFY_APP, MESessionTopologyStatus, (LPARAM)&topostat);
					
					SAFE_RELEASE(pTopology);

                    break;
                }
            case MENewPresentation:
                {
                    TRACE((L"%d: MENewPresentation, Status: 0x%X", eventType, eventStatus ));

                    IMFPresentationDescriptor *pPresentationDescriptor = NULL;

                    MFSequencerElementId SegmentId = 0;
                    
                    hr = eventData.punkVal->QueryInterface(
                            __uuidof(IMFPresentationDescriptor),
                            (void**)&pPresentationDescriptor);  

                    if (SUCCEEDED(hr))
                    {
                        //Queue the next segment on the media session
                        this->QueueNextSegment(pPresentationDescriptor, &SegmentId);
                    }

                    SAFE_RELEASE(pPresentationDescriptor);

                    PostMessage(m_hWnd, WM_NOTIFY_APP, eventType, SegmentId);
    
                    break;
                }

            case MEEndOfPresentation:
                {
                    TRACE((L"%d: MEEndOfPresentation, Status: 0x%X", eventType, eventStatus ));

					UINT value = 0;
					
					hr = pEvent->GetUINT32(MF_EVENT_SOURCE_TOPOLOGY_CANCELED, &value);

                    PostMessage(m_hWnd, WM_NOTIFY_APP, eventType, (LPARAM)value);

                    break;
                }

            case MEEndOfPresentationSegment:
                {
                    TRACE((L"%d: MEEndOfPresentationSegment, Status: 0x%X", eventType, eventStatus ));

					UINT value = 0;
					
					hr = pEvent->GetUINT32(MF_EVENT_SOURCE_TOPOLOGY_CANCELED, &value);

                    PostMessage(m_hWnd, WM_NOTIFY_APP, eventType, value);

                    break;
                }
    
            case MESessionNotifyPresentationTime:
                {
                    TRACE((L"%d: MESessionNotifyPresentationTime, Status: 0x%X", eventType, eventStatus ));

                    hr = HandleNotifyPresentationTime( pEvent );
                    break;
                }
            case MESessionClosed:
                {
                    TRACE((L"%d: MESessionClosed, Status: 0x%X", eventType, eventStatus ));

                    SetEvent(m_hCloseEvent);

                    break;
                }
            
            default:
                TRACE((L"%d: Event", eventType ));
                break;
                
        }

    }
    else
    {
        TRACE((L"Event %d failed IMFMediaEvent::GetStatus 0x%X", eventType, eventStatus));
    }
    
    
    // Request another event.
    if (eventType != MESessionClosed)
    {
        hr = m_pMediaSession->BeginGetEvent(this, NULL);
        LOG_IF_FAILED(L"IMFMediaSession::BeginGetEvent", hr);
    }
    // Clean up.
    PropVariantClear(&eventData);

    return S_OK;
} 

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::GetPresentationTime (Public)
//  Description: 
//      Gets the unadjusted presentation time
//  Parameter:
//      phnsPresentationTime: [out] presentation time
///////////////////////////////////////////////////////////////////////////////////////////
HRESULT CPlayer::GetPresentationTime(MFTIME *phnsPresentationTime)
{
    if (!phnsPresentationTime)
    {
        return E_POINTER;
    }
    
    HRESULT hr = S_OK;

    hr = m_pPresentationClock->GetTime(phnsPresentationTime);
    
    return hr;
}



//Private methods
//-------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::CreateNodesForStream (Private)
//  Description: 
//      Creates the source and output nodes for a stream and 
//      Adds them to the topology
//      Connects the source node to the output node
//      
//  Parameter:
//      pPresentationDescriptor: [in] Pointer to the presentation descriptor for the media source
//      pMediaSource: [in] Pointer to the media source
//      pTopology: [in] Pointer to the topology
//
//  Notes: For each stream, the app must:
//      1. Create a source node associated with the stream. 
//      2. Create an output node for the renderer. 
//      3. Connect the two nodes.
//      The media session will resolve the topology, transform nodes are not required
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateNodesForStream(
                    IMFPresentationDescriptor *pPresentationDescriptor,
                    IMFMediaSource *const pMediaSource,
                    IMFTopology *const pTopology)
{
    if (!pPresentationDescriptor || !pMediaSource || !pTopology)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    IMFStreamDescriptor *pStreamDescriptor = NULL;
    IMFTopologyNode *pSourceNode = NULL;
    IMFTopologyNode *pOutputNode = NULL;
    
    BOOL fSelected = FALSE;

    // Get the stream descriptor for the only stream index =0.
    if (SUCCEEDED(hr))
    {
        hr = pPresentationDescriptor->GetStreamDescriptorByIndex(0, &fSelected, &pStreamDescriptor );
        LOG_IF_FAILED(L"IMFPresentationDescriptor::GetStreamDescriptorByIndex", hr);
    }

    if(fSelected)
    {
        // Create a source node for this stream and add it to the topology.
        if (SUCCEEDED(hr))
        {
            hr = CreateSourceNode(pPresentationDescriptor, pStreamDescriptor, pMediaSource, &pSourceNode);
        }

        if (SUCCEEDED(hr))
        {
            hr = pTopology->AddNode(pSourceNode);
            LOG_IF_FAILED(L"IMFTopology::AddNode for source node", hr);
        }
        
        // Create the output node for the renderer and add it to the topology.
        if (SUCCEEDED(hr))
        {
            hr  = CreateOutputNode(pStreamDescriptor, &pOutputNode );
        }

        if (SUCCEEDED(hr))
        {
            hr = pTopology->AddNode(pOutputNode);
            LOG_IF_FAILED(L"IMFTopology::AddNode for output node", hr);
        }
        
        // Connect the source node to the output node.
        if (SUCCEEDED(hr))
        {
            hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
            LOG_IF_FAILED(L"IMFTopologyNode::ConnectOutput", hr);

        }

    }
    
    //clean up
    SAFE_RELEASE(pStreamDescriptor);
    SAFE_RELEASE(pSourceNode);
    SAFE_RELEASE(pOutputNode);

    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::CreateSourceNode (Private)
//  Description: 
//      Creates the source node for a stream 
//  Parameter:
//      pPresentationDescriptor: [in] Pointer to the presentation descriptor for the media source
//      pStreamDescriptor: [in] Stream descriptor for the stream
//      pMediaSource: [in] Pointer to the media source
//      ppSourceNode: [out] Receives a pointer to the new node
///////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateSourceNode(
    IMFPresentationDescriptor *const pPresentationDescriptor,
    IMFStreamDescriptor *const pStreamDescriptor,
    IMFMediaSource *const pMediaSource,
    IMFTopologyNode **const ppSourceNode)
{

    if (!pPresentationDescriptor || !pMediaSource || !pStreamDescriptor || !ppSourceNode)
    {
        return E_POINTER;
    }

    IMFTopologyNode *pNewNode = NULL;

    HRESULT hr = S_OK;

    // Create the source-stream node. 
    if (SUCCEEDED(hr))
    {
        hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNewNode);
        LOG_IF_FAILED(L"MFCreateTopologyNode for source node", hr);
    }

    // Set attribute: Pointer to the media source. Necessary.
    if (SUCCEEDED(hr))
    {
        hr = pNewNode->SetUnknown( MF_TOPONODE_SOURCE, pMediaSource);
    }

    // Set attribute: Pointer to the presentation descriptor. Necessary.
    if (SUCCEEDED(hr))
    {    
        hr = pNewNode->SetUnknown( MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPresentationDescriptor);
    }

    // Set attribute: Pointer to the stream descriptor. Necessary.
    if (SUCCEEDED(hr))
    {
        hr = pNewNode->SetUnknown( MF_TOPONODE_STREAM_DESCRIPTOR, pStreamDescriptor);
    }

    // Return the IMFTopologyNode pointer to the caller.
    if (SUCCEEDED(hr))
    {
        *ppSourceNode = pNewNode;
        (*ppSourceNode)->AddRef();
    }

    // Clean up.
    SAFE_RELEASE(pNewNode);

    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::CreateOutputNode (Private)
//  Description: 
//      Creates an output node for a stream
//      Sets the IActivate pointer on the node
//  Parameter:
//      pStreamDescriptor: [in] Stream descriptor for the stream
//      ppSourceNode: [out] Receives a pointer to the new node
////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateOutputNode(
                    IMFStreamDescriptor *const pStreamDescriptor,
                    IMFTopologyNode     **const ppOutputNode)
{
    if (!pStreamDescriptor)
    {
        return E_POINTER;
    }

    if (!ppOutputNode)
    {
        return E_HANDLE;
    }

    IMFTopologyNode *pNewNode = NULL;
    IMFMediaTypeHandler *pHandler = NULL;

    GUID guidMajorType = GUID_NULL;

    HRESULT hr = S_OK;

    // Create a downstream node.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNewNode);
        LOG_IF_FAILED(L"MFCreateTopologyNode for output node", hr);
    }

    // Get the media type handler for the stream.
    if (SUCCEEDED(hr))
    {
        hr = pStreamDescriptor->GetMediaTypeHandler(&pHandler);
        LOG_IF_FAILED(L"IMFStreamDescriptor::GetMediaTypeHandler", hr);
    }

    // Get the major media type.
    if (SUCCEEDED(hr))
    {
        hr = pHandler->GetMajorType(&guidMajorType);
        LOG_IF_FAILED(L"IMFMediaTypeHandler::GetMajorType", hr);
    }
    if (SUCCEEDED(hr))
    {
        // Set the IActivate object on the output node.
        if (MFMediaType_Audio == guidMajorType)
        {
            hr = pNewNode->SetObject(m_pAudioRendererActivate);
            TRACE((L"Audio stream"));

        }
        //Only audio is implemented, if guidMajorType is any other type, return E_NOTIMPL
        else
        {
            hr = E_NOTIMPL;
            TRACE((L"Unsupported stream"));
        }
    }

    // Return the IMFTopologyNode pointer to the caller.
    if (SUCCEEDED(hr))
    {
        *ppOutputNode = pNewNode;
        (*ppOutputNode)->AddRef();
    }

    // Clean up.
    SAFE_RELEASE(pNewNode);
    SAFE_RELEASE(pHandler);

    return hr;
}



////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::HandleNotifyPresentationTime (Private)
//  Description: 
//      Handles the media session's MESessionNotifyPresentationTime event
//  Parameter:
//      pEvent: [in] MESessionNotifyPresentationTime event
///////////////////////////////////////////////////////////////////////////////////////////
HRESULT CPlayer::HandleNotifyPresentationTime(IMFMediaEvent *const pEvent)
{
    if (!pEvent)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;


    if (!m_phnsTimePairStart)
    {
        m_phnsTimePairStart = new TimePair;
        m_phnsTimePairStart ->pNextTimePair = NULL;
        m_phnsTimePairEnd = m_phnsTimePairStart;
    }
    else
    {
        m_phnsTimePairEnd ->pNextTimePair = new TimePair;
        m_phnsTimePairEnd = m_phnsTimePairEnd->pNextTimePair;
        m_phnsTimePairEnd->pNextTimePair = NULL;
    }
    
    hr = pEvent->GetUINT64(
                MF_EVENT_START_PRESENTATION_TIME,
                (UINT64 *) &(this->m_phnsTimePairEnd->hnsStartPresentationTime));   

    if (SUCCEEDED(hr))
        hr = pEvent->GetUINT64(
                    MF_EVENT_PRESENTATION_TIME_OFFSET,
                    (UINT64 *) &(this->m_phnsTimePairEnd->hnsPresentationTimeOffset));
    
    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::CreateMediaSource (Private)
//  Description: 
//      Creates a media source from URL
//  Parameter:
//      sURL: [in] File URL
//      ppMediaSource: [out] Receives the media source
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateMediaSource (
                    const WCHAR     *sURL, 
                    IMFMediaSource  **ppMediaSource)
{
    TRACE((L"CPlayer::CreateMediaSource"));

    if (!sURL)
    {
        return E_POINTER;
    }

    if (!ppMediaSource)
    {
        return E_HANDLE;
    }

    IMFSourceResolver   *pSourceResolver = NULL;
    IUnknown            *pSourceUnk = NULL;

    HRESULT hr = S_OK;
    
    hr = MFCreateSourceResolver(&pSourceResolver);
    LOG_IF_FAILED(L"MFCreateSourceResolver", hr);


    // Use the source resolver to create the media source.
    if (SUCCEEDED(hr))
    {
        MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

        hr = pSourceResolver->CreateObjectFromURL(
                sURL,                       // URL of the source.
                MF_RESOLUTION_MEDIASOURCE,  // Create a source object.
                NULL,                       // Optional property store.
                &ObjectType,                // Receives the created object type. 
                &pSourceUnk                 // Receives a pointer to the media source.
            );
            LOG_IF_FAILED(L"IMFSourceResolver::CreateObjectFromURL", hr);

    }

   // Get the IMFMediaSource interface from the media source.
    if (SUCCEEDED(hr))
    {
        hr = pSourceUnk->QueryInterface(IID_IMFMediaSource, (void**)ppMediaSource);
    }

    SAFE_RELEASE(pSourceResolver);
    SAFE_RELEASE(pSourceUnk);

    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::CreateTopology (Private)
//  Description: 
//      Creates a topology for the media source
//  Parameter:
//      pMediaSource: [in] Pointer to the media source
//      pTopology: [in] Receives the partial topology
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::CreateTopology(
                    IMFMediaSource  *pMediaSource,
                    IMFTopology     *pTopology)
{
    TRACE((L"CPlayer::CreateTopology"));

    //The caller needs to pass a valid media source
    //We need the media source because to set the source node attribute, media source is needed
    
    if(!pMediaSource || !pTopology)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    IMFPresentationDescriptor *pPresentationDescriptor = NULL;

    //Create Presentation Descriptor for the media source
    if (SUCCEEDED(hr))
    {
        hr = pMediaSource->CreatePresentationDescriptor(&pPresentationDescriptor);
        LOG_IF_FAILED(L"IMFMediaSource::CreatePresentationDescriptor", hr);
    }
    
    if (SUCCEEDED(hr))
    {
        hr = CreateNodesForStream(pPresentationDescriptor, pMediaSource, pTopology );
    }

    SAFE_RELEASE(pPresentationDescriptor);

    return hr;
}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::AddTopologyToSequencer (Private)
//  Description: 
//      Adds the topology to the sequencer
//  Parameter:
//      sURL: [in] File URL
//      pMediaSource: [in] Pointer to the media source
//      pTopology: [in] Pointer to the topology
//      pSegmentId: [out] Receives the segment id returned by the sequencer source
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::AddTopologyToSequencer (
                    const WCHAR     *sURL,
                    IMFMediaSource  *pMediaSource, 
                    IMFTopology     *pTopology,
                    MFSequencerElementId *pSegmentId)
{
    TRACE((L"CPlayer::AddTopologyToSequencer"));

    if (!sURL || !pMediaSource || !pTopology)
    {
        return E_POINTER;
    }
    
    HRESULT hr = S_OK;

    MFTIME hnsSegmentDuration = 0;
    
    TOPOID TopologyID = 0;

    IMFPresentationDescriptor *pPresentationDescriptor = NULL;
    
    hr = m_pSequencerSource->AppendTopology(pTopology, 0, pSegmentId);

    if (SUCCEEDED(hr))
    {
        pTopology->GetTopologyID(&TopologyID);
    }
        
    //create a presentation descriptor
    if (SUCCEEDED(hr))
    {
        hr = pMediaSource->CreatePresentationDescriptor(&pPresentationDescriptor);

    }
    
    //get the segment duration
    if (SUCCEEDED(hr))
    {
        hr = pPresentationDescriptor->GetUINT64(MF_PD_DURATION, (UINT64*)&hnsSegmentDuration);
    }

    if (SUCCEEDED(hr))
    {
        assert( hnsSegmentDuration > 0 );
    }

    //store the segment info: SegmentId, SegmentDuration, TopoID in the linked list.
    if (SUCCEEDED(hr))
    {
        m_Segments.AddNewSegmentEntry(*pSegmentId, hnsSegmentDuration, TopologyID, sURL);   
    }

    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::QueueNextSegment (Private)
//  Description: 
//      Queues the next topology on the session.
//  
//  Parameter:
//      pPresentationDescriptor: [in] Presentation descriptor for the next topology
//      pSegmentId: [out] Receives the corresponding segment identifier for the topology
//  
//  Note: The presentation descriptor is received from the MENewPresentation event.
//          This event tells the session about the next topology in the sequencer.
//          If NULL is passed, this method queues the first topology on the media session.
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::QueueNextSegment (
                     IMFPresentationDescriptor *pPresentationDescriptor,
                     MFSequencerElementId *pSegmentId)
{

    HRESULT hr = S_OK;
    
    IMFMediaSourceTopologyProvider *pMediaSourceTopologyProvider = NULL;
    IMFTopology *pTopology = NULL;

    MFSequencerElementId SegId = 0;

    if (SUCCEEDED(hr))
    {
        // Get the Segment ID.
        hr = this->m_pSequencerSource->GetPresentationContext(
            pPresentationDescriptor, &SegId,
            &pTopology);
    
        TRACE((L"CPlayer::QueueNextSegment: %d", SegId));

    }

    //Get the topology for the presentation descriptor
    if (SUCCEEDED(hr))
    {
        hr = m_pSequencerSource->QueryInterface(
                        __uuidof(IMFMediaSourceTopologyProvider), 
                        (void**)&pMediaSourceTopologyProvider);
    }

    if (SUCCEEDED(hr))
    {
        hr = pMediaSourceTopologyProvider->GetMediaSourceTopology(
                            pPresentationDescriptor, 
                            &pTopology);
    }

    //Set the topology on the media session
    if (SUCCEEDED(hr))
    {
        m_pMediaSession->SetTopology(NULL, pTopology);
        LOG_IF_FAILED(L"IMFMediaSession::SetTopology", hr);

        *pSegmentId = SegId;

    }

    //clean up
    SAFE_RELEASE(pMediaSourceTopologyProvider);
    SAFE_RELEASE(pTopology);

    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CPlayer::AddSegment (Private)
//  Description: 
//      Adds a segment to the sequencer.
//  Parameter:  
//      sURL: [in]File URL
//      pSegmentId: [out] receives the segment identifier of the segment returned by AppendTopology
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CPlayer::AddSegment(const WCHAR *sURL, MFSequencerElementId *pSegmentId)
{
    TRACE((L"CPlayer::AddSegment"));
    TRACE((L"URL = %s", sURL));

    if (!sURL || !pSegmentId)
    {
        return E_POINTER;
    }

    IMFMediaSource *pMediaSource = NULL;
    IMFTopology *pTopology = NULL;

    HRESULT hr = S_OK;

    hr = this->CreateMediaSource(sURL, &pMediaSource);

    if (SUCCEEDED(hr))
    {
        hr = MFCreateTopology(&pTopology);
    }

    if (SUCCEEDED(hr))
    {
        hr = this->CreateTopology(pMediaSource, pTopology);     
    }

    if (SUCCEEDED(hr))
    {
        hr = this->AddTopologyToSequencer(sURL, pMediaSource, pTopology, pSegmentId);
    }

    //clean up

    SAFE_RELEASE(pMediaSource);
    SAFE_RELEASE(pTopology);


    return hr;
}



 
