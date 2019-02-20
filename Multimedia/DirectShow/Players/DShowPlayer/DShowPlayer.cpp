//////////////////////////////////////////////////////////////////////////
// DShowPlayer.cpp: Implements DirectShow playback functionality.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DShowPlayer.h"
#include "DshowUtil.h"


HRESULT RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer, BOOL *pbRemoved);
HRESULT InitWindowlessVMR(IBaseFilter *pVMR, HWND hwnd, IVMRWindowlessControl** ppWc); 

//-----------------------------------------------------------------------------
// DShowPlayer constructor.
//-----------------------------------------------------------------------------

DShowPlayer::DShowPlayer(HWND hwndVideo) :
	m_state(STATE_CLOSED),
	m_hwndVideo(hwndVideo),
	m_hwndEvent(NULL),
	m_EventMsg(0),
	m_pGraph(NULL),
	m_pControl(NULL),
	m_pEvent(NULL),
	m_pSeek(NULL),
	m_pWindowless(NULL),
	m_pAudio(NULL),
	m_seekCaps(0),
	m_bMute(FALSE),
	m_lVolume(MAX_VOLUME)
{


}

//-----------------------------------------------------------------------------
// DShowPlayer destructor.
//-----------------------------------------------------------------------------

DShowPlayer::~DShowPlayer()
{
	TearDownGraph();
}



//-----------------------------------------------------------------------------
// DShowPlayer::SetEventWindow
// Description: Set the window to receive graph events.
//
// hwnd: Window to receive the events.
// msg: Private window message that window will receive whenever a 
//      graph event occurs. (Must be in the range WM_APP through 0xBFFF.)
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::SetEventWindow(HWND hwnd, UINT msg)
{
	m_hwndEvent = hwnd;
	m_EventMsg = msg;
	return S_OK;
}


//-----------------------------------------------------------------------------
// DShowPlayer::OpenFile
// Description: Open a new file for playback.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::OpenFile(const WCHAR* sFileName)
{
	HRESULT hr = S_OK;

	IBaseFilter *pSource = NULL;

	// Create a new filter graph. (This also closes the old one, if any.)
	hr = InitializeGraph();
	
	// Add the source filter to the graph.
	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->AddSourceFilter(sFileName, NULL, &pSource);
	}

	// Try to render the streams.
	if (SUCCEEDED(hr))
	{
		hr = RenderStreams(pSource);
	}

	// Get the seeking capabilities.
	if (SUCCEEDED(hr))
	{
		hr = m_pSeek->GetCapabilities(&m_seekCaps);
	}

    // Set the volume.
	if (SUCCEEDED(hr))
	{
		hr = UpdateVolume();
	}

	// Update our state.
	if (SUCCEEDED(hr))
	{
		m_state = STATE_STOPPED;
	}

	SAFE_RELEASE(pSource);

	return hr;
}



//-----------------------------------------------------------------------------
// DShowPlayer::HandleGraphEvent
// Description: Respond to a graph event.
//
// The owning window should call this method when it receives the window
// message that the application specified when it called SetEventWindow.
//
// pCB: Pointer to the GraphEventCallback callback, implemented by 
//      the application. This callback is invoked once for each event
//      in the queue. 
//
// Caution: Do not tear down the graph from inside the callback.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::HandleGraphEvent(GraphEventCallback *pCB)
{
	if (pCB == NULL)
	{
		return E_POINTER;
	}

	if (!m_pEvent)
	{
		return E_UNEXPECTED;
	}

	long evCode = 0;
	LONG_PTR param1 = 0, param2 = 0;

	HRESULT hr = S_OK;

    // Get the events from the queue.
	while (SUCCEEDED(m_pEvent->GetEvent(&evCode, &param1, &param2, 0)))
	{
        // Invoke the callback.
		pCB->OnGraphEvent(evCode, param1, param2);

        // Free the event data.
		hr = m_pEvent->FreeEventParams(evCode, param1, param2);
		if (FAILED(hr))
		{
			break;
		}
	}

	return hr;
}


// state changes

HRESULT DShowPlayer::Play()
{
	if (m_state != STATE_PAUSED && m_state != STATE_STOPPED)
	{
		return VFW_E_WRONG_STATE;
	}

	assert(m_pGraph); // If state is correct, the graph should exist.

	HRESULT hr = m_pControl->Run();

	if (SUCCEEDED(hr))
	{
		m_state = STATE_RUNNING;
	}

	return hr;
}

HRESULT DShowPlayer::Pause()
{
	if (m_state != STATE_RUNNING)
	{
		return VFW_E_WRONG_STATE;
	}

	assert(m_pGraph); // If state is correct, the graph should exist.

	HRESULT hr = m_pControl->Pause();

	if (SUCCEEDED(hr))
	{
		m_state = STATE_PAUSED;
	}

	return hr;
}


HRESULT DShowPlayer::Stop()
{
	if (m_state != STATE_RUNNING && m_state != STATE_PAUSED)
	{
		return VFW_E_WRONG_STATE;
	}

	assert(m_pGraph); // If state is correct, the graph should exist.

	HRESULT hr = m_pControl->Stop();

	if (SUCCEEDED(hr))
	{
		m_state = STATE_STOPPED;
	}

	return hr;
}


// VMR functionality



//-----------------------------------------------------------------------------
// DShowPlayer::UpdateVideoWindow
// Description: Sets the destination rectangle for the video.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::UpdateVideoWindow(const LPRECT prc)
{
	if (m_pWindowless == NULL)
	{
		return S_OK; // no-op
	}

	if (prc)
	{
		return m_pWindowless->SetVideoPosition(NULL, prc);
	}
	else
	{

		RECT rc;
		GetClientRect(m_hwndVideo, &rc);
		return m_pWindowless->SetVideoPosition(NULL, &rc);
	}

}

//-----------------------------------------------------------------------------
// DShowPlayer::Repaint
// Description: Repaints the video.
//
// Call this method when the application receives WM_PAINT.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::Repaint(HDC hdc)
{
	if (m_pWindowless)
	{
		return m_pWindowless->RepaintVideo(m_hwndVideo, hdc);
	}
	else
	{
		return S_OK;
	}
}


//-----------------------------------------------------------------------------
// DShowPlayer::DisplayModeChanged
// Description: Notifies the VMR that the display mode changed.
//
// Call this method when the application receives WM_DISPLAYCHANGE.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::DisplayModeChanged()
{
	if (m_pWindowless)
	{
		return m_pWindowless->DisplayModeChanged();
	}
	else
	{
		return S_OK;
	}
}

// Seeking


//-----------------------------------------------------------------------------
// DShowPlayer::CanSeek
// Description: Returns TRUE if the current file is seekable.
//-----------------------------------------------------------------------------

BOOL DShowPlayer::CanSeek() const
{
	const DWORD caps = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
	return ((m_seekCaps & caps) == caps);
}


//-----------------------------------------------------------------------------
// DShowPlayer::SetPosition
// Description: Seeks to a new position.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::SetPosition(REFERENCE_TIME pos)
{
	if (m_pControl == NULL || m_pSeek == NULL)
	{
		return E_UNEXPECTED;
	}

	HRESULT hr = S_OK;

	hr = m_pSeek->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning);

	if (SUCCEEDED(hr))
	{
		// If playback is stopped, we need to put the graph into the paused
		// state to update the video renderer with the new frame, and then stop 
		// the graph again. The IMediaControl::StopWhenReady does this.
		if (m_state == STATE_STOPPED)
		{
			hr = m_pControl->StopWhenReady();
		}
	}

	return hr;
}

//-----------------------------------------------------------------------------
// DShowPlayer::GetDuration
// Description: Gets the duration of the current file.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::GetDuration(LONGLONG *pDuration)
{
	if (m_pSeek == NULL)
	{
		return E_UNEXPECTED;
	}

	return m_pSeek->GetDuration(pDuration);
}

//-----------------------------------------------------------------------------
// DShowPlayer::GetCurrentPosition
// Description: Gets the current playback position.
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::GetCurrentPosition(LONGLONG *pTimeNow)
{
	if (m_pSeek == NULL)
	{
		return E_UNEXPECTED;
	}

	return m_pSeek->GetCurrentPosition(pTimeNow);
}


// Audio

//-----------------------------------------------------------------------------
// DShowPlayer::Mute
// Description: Mutes or unmutes the audio.
//-----------------------------------------------------------------------------

HRESULT	DShowPlayer::Mute(BOOL bMute)
{
	m_bMute = bMute;
	return UpdateVolume();
}

//-----------------------------------------------------------------------------
// DShowPlayer::SetVolume
// Description: Sets the volume. 
//-----------------------------------------------------------------------------

HRESULT	DShowPlayer::SetVolume(long lVolume)
{
	m_lVolume = lVolume;
	return UpdateVolume();
}


//-----------------------------------------------------------------------------
// DShowPlayer::UpdateVolume
// Description: Update the volume after a call to Mute() or SetVolume().
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::UpdateVolume()
{
	HRESULT hr = S_OK;

	if (m_bAudioStream && m_pAudio)
	{
        // If the audio is muted, set the minimum volume. 
		if (m_bMute)
		{
			hr = m_pAudio->put_Volume(MIN_VOLUME);
		}
		else
		{
			// Restore previous volume setting
			hr = m_pAudio->put_Volume(m_lVolume);
		}
	}

	return hr;
}




// Graph building


//-----------------------------------------------------------------------------
// DShowPlayer::InitializeGraph
// Description: Create a new filter graph. (Tears down the old graph.) 
//-----------------------------------------------------------------------------

HRESULT DShowPlayer::InitializeGraph()
{
	HRESULT hr = S_OK;

	TearDownGraph();

	// Create the Filter Graph Manager.
	hr = CoCreateInstance(
		CLSID_FilterGraph, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&m_pGraph
		);

	// Query for graph interfaces.
	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (void**)&m_pEvent);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->QueryInterface(IID_IBasicAudio, (void**)&m_pAudio);
	}


	// Set up event notification.
	if (SUCCEEDED(hr))
	{
		hr = m_pEvent->SetNotifyWindow((OAHWND)m_hwndEvent, m_EventMsg, NULL);
	}

	return hr;
}

//-----------------------------------------------------------------------------
// DShowPlayer::TearDownGraph
// Description: Tear down the filter graph and release resources. 
//-----------------------------------------------------------------------------

void DShowPlayer::TearDownGraph()
{
	// Stop sending event messages
	if (m_pEvent)
	{
		m_pEvent->SetNotifyWindow((OAHWND)NULL, NULL, NULL);
	}

	SAFE_RELEASE(m_pGraph);
	SAFE_RELEASE(m_pControl);
	SAFE_RELEASE(m_pEvent);
	SAFE_RELEASE(m_pSeek);
	SAFE_RELEASE(m_pWindowless);
	SAFE_RELEASE(m_pAudio);

	m_state = STATE_CLOSED;
	m_seekCaps = 0;

    m_bAudioStream = FALSE;
}

//-----------------------------------------------------------------------------
// DShowPlayer::RenderStreams
// Description: Render the streams from a source filter. 
//-----------------------------------------------------------------------------

HRESULT	DShowPlayer::RenderStreams(IBaseFilter *pSource)
{
	HRESULT hr = S_OK;

	BOOL bRenderedAnyPin = FALSE;

	IFilterGraph2 *pGraph2 = NULL;
	IEnumPins *pEnum = NULL;
	IBaseFilter *pVMR = NULL;
	IBaseFilter *pAudioRenderer = NULL;

	hr = m_pGraph->QueryInterface(IID_IFilterGraph2, (void**)&pGraph2);

	// Add the VMR-7 to the graph.
	if (SUCCEEDED(hr))
	{
		hr = AddFilterByCLSID(m_pGraph, CLSID_VideoMixingRenderer, &pVMR, L"VMR-7");
	}

	// Set windowless mode on the VMR. This must be done before the VMR is connected.
	if (SUCCEEDED(hr))
	{
		hr = InitWindowlessVMR(pVMR, m_hwndVideo, &m_pWindowless);
	}

	// Add the DSound Renderer to the graph.
	if (SUCCEEDED(hr))
	{
		hr = AddFilterByCLSID(m_pGraph, CLSID_DSoundRender, &pAudioRenderer, L"Audio Renderer");
	}

    // Enumerate the pins on the source filter.
	if (SUCCEEDED(hr))
	{
		hr = pSource->EnumPins(&pEnum);
	}

	if (SUCCEEDED(hr))
	{
		// Loop through all the pins
		IPin *pPin = NULL;

		while (S_OK == pEnum->Next(1, &pPin, NULL))
		{			
			// Try to render this pin. 
			// It's OK if we fail some pins, if at least one pin renders.
			HRESULT hr2 = pGraph2->RenderEx(pPin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, NULL);

			pPin->Release();

			if (SUCCEEDED(hr2))
			{
				bRenderedAnyPin = TRUE;
			}
		}
	}


	// Remove un-used renderers.

    // Try to remove the VMR.
	if (SUCCEEDED(hr))
	{
    	BOOL bRemoved = FALSE;
		hr = RemoveUnconnectedRenderer(m_pGraph, pVMR, &bRemoved);

		// If we removed the VMR, then we also need to release our 
		// pointer to the VMR's windowless control interface.
		if (bRemoved)
		{
			SAFE_RELEASE(m_pWindowless);
		}
	}

    // Try to remove the audio renderer.
	if (SUCCEEDED(hr))
	{
    	BOOL bRemoved = FALSE;
		hr = RemoveUnconnectedRenderer(m_pGraph, pAudioRenderer, &bRemoved);

        if (bRemoved)
        {
            m_bAudioStream = FALSE;
        }
        else
        {
            m_bAudioStream = TRUE;
        }
	}

	SAFE_RELEASE(pEnum);
	SAFE_RELEASE(pVMR);
	SAFE_RELEASE(pAudioRenderer);
	SAFE_RELEASE(pGraph2);

	// If we succeeded to this point, make sure we rendered at least one 
	// stream.
	if (SUCCEEDED(hr))
	{
		if (!bRenderedAnyPin)
		{
			hr = VFW_E_CANNOT_RENDER;
		}
	}

	return hr;
}


//-----------------------------------------------------------------------------
// DShowPlayer::RemoveUnconnectedRenderer
// Description: Remove a renderer filter from the graph if the filter is
//              not connected. 
//-----------------------------------------------------------------------------

HRESULT RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer, BOOL *pbRemoved)
{
	IPin *pPin = NULL;

	BOOL bRemoved = FALSE;

	// Look for a connected input pin on the renderer.

	HRESULT hr = FindConnectedPin(pRenderer, PINDIR_INPUT, &pPin);
	SAFE_RELEASE(pPin);

	// If this function succeeds, the renderer is connected, so we don't remove it.
	// If it fails, it means the renderer is not connected to anything, so
	// we remove it.

	if (FAILED(hr))
	{
		hr = pGraph->RemoveFilter(pRenderer);
		bRemoved = TRUE;
	}

	if (SUCCEEDED(hr))
	{
		*pbRemoved = bRemoved;
	}

	return hr;
}

//-----------------------------------------------------------------------------
// DShowPlayer::InitWindowlessVMR
// Description: Initialize the VMR-7 for windowless mode. 
//-----------------------------------------------------------------------------

HRESULT InitWindowlessVMR( 
    IBaseFilter *pVMR,				// Pointer to the VMR
	HWND hwnd,						// Clipping window
	IVMRWindowlessControl** ppWC	// Receives a pointer to the VMR.
    ) 
{ 

    IVMRFilterConfig* pConfig = NULL; 
	IVMRWindowlessControl *pWC = NULL;

	HRESULT hr = S_OK;

	// Set the rendering mode.  
    hr = pVMR->QueryInterface(IID_IVMRFilterConfig, (void**)&pConfig); 
    if (SUCCEEDED(hr)) 
    { 
        hr = pConfig->SetRenderingMode(VMRMode_Windowless); 
    }

	// Query for the windowless control interface.
    if (SUCCEEDED(hr))
    {
        hr = pVMR->QueryInterface(IID_IVMRWindowlessControl, (void**)&pWC);
	}

	// Set the clipping window.
	if (SUCCEEDED(hr))
	{
		hr = pWC->SetVideoClippingWindow(hwnd);
	}

	// Preserve aspect ratio by letter-boxing
	if (SUCCEEDED(hr))
	{
		hr = pWC->SetAspectRatioMode(VMR_ARMODE_LETTER_BOX);
	}

	// Return the IVMRWindowlessControl pointer to the caller.
	if (SUCCEEDED(hr))
	{
		*ppWC = pWC;
		(*ppWC)->AddRef();
	}

	SAFE_RELEASE(pConfig);
	SAFE_RELEASE(pWC);

	return hr; 
} 


