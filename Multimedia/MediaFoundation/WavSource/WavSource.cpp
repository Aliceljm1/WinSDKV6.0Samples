//////////////////////////////////////////////////////////////////////
//
// WavSource.cpp : Sample media source for Media Foundation
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////


// Misc implementation notes.
//
// Locking: 
//     The source and stream objects both have critical sections. If you 
//     hold both locks, the source lock must be held FIRST, to avoid 
//     deadlocks. 
//
// Shutdown: 
//     Most methods start by calling CheckShutdown(). This method
//     fails if the source was shut down.
//

#include "WavSource.h"

#include <assert.h>


// FOURCCs that we need
const FOURCC ckid_WAVE_FILE = FCC('WAVE');    // RIFF type for .wav files
const FOURCC ckid_WAVE_FMT =  FCC('fmt ');    // WAVEFORMATEX chunk
const FOURCC ckid_WAVE_DATA = FCC('data');    // Audio data chunk


template <class T>
T AlignUp(T num, T mult)
{
    assert(num >= 0);
    T tmp = num + mult - 1;
    return tmp - (tmp % mult);
}



// Helper Functions

HRESULT QueueEventWithIUnknown(
    IMFMediaEventGenerator *pMEG,
    MediaEventType meType,
    HRESULT hrStatus,
    IUnknown *pUnk);

HRESULT ValidateWaveFormat(const WAVEFORMATEX *pWav, DWORD cbSize);

LONGLONG AudioDurationFromBufferSize(const WAVEFORMATEX *pWav, DWORD cbAudioDataSize);
LONGLONG BufferSizeFromAudioDuration(const WAVEFORMATEX *pWav, LONGLONG duration);


//-------------------------------------------------------------------
// Name: CreateInstance
// Description: Static method to create an instance of the source.
//
// iid:         IID of the requested interface on the source.
// ppSource:    Receives a ref-counted pointer to the source.
//-------------------------------------------------------------------

HRESULT WavSource::CreateInstance(REFIID iid, void **ppSource)
{
    if (ppSource == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    WavSource *pSource = new WavSource(hr);
    if (pSource == NULL)
    {
        return E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        hr = pSource->QueryInterface(iid, ppSource);
    }

    if (FAILED(hr))
    {
        delete pSource;
    }
    return hr;
}


//-------------------------------------------------------------------
// WavSource constructor.
//
// hr: If the constructor fails, this value is set to a failure code.
//-------------------------------------------------------------------

WavSource::WavSource(HRESULT& hr) 
  : m_nRefCount(0),
    m_pEventQueue(NULL),
    m_pPresentationDescriptor(NULL),
    m_IsShutdown(FALSE),
    m_state(STATE_STOPPED),
    m_pStream(NULL),
    m_pRiff(NULL)
{
    // Create the media event queue.
    hr = MFCreateEventQueue(&m_pEventQueue);

}


//-------------------------------------------------------------------
// WavSource destructor.
//-------------------------------------------------------------------


WavSource::~WavSource()
{
    assert(m_IsShutdown);
    assert(m_nRefCount == 0);
}


// IUnknown methods

ULONG WavSource::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG  WavSource::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT WavSource::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IMFMediaEventGenerator))
    {
        *ppv = static_cast<IMFMediaEventGenerator*>(this);
    }
    else if (iid == __uuidof(IMFMediaSource))
    {
        *ppv = static_cast<IMFMediaSource*>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// IMFMediaEventGenerator methods
//
// All of the IMFMediaEventGenerator methods do the following:
// 1. Check for shutdown status.
// 2. Call the event generator helper object.

HRESULT WavSource::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    HRESULT hr = S_OK;

    CAutoLock lock(m_critSec);
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->BeginGetEvent(pCallback, punkState);
    }

    return hr;
}

HRESULT WavSource::EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    CAutoLock lock(m_critSec);
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->EndGetEvent(pResult, ppEvent);
    }

    return hr;
}

HRESULT WavSource::GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent)
{
    // NOTE: GetEvent can block indefinitely, so we don't hold the
    //       WavSource lock. This requires some juggling with the
    //       event queue pointer.

    HRESULT hr = S_OK;

    IMFMediaEventQueue *pQueue = NULL;

    { // scope for lock
      
        CAutoLock lock(m_critSec);

        // Check shutdown
        hr = CheckShutdown();

        if (SUCCEEDED(hr))
        {
            pQueue = m_pEventQueue;
            pQueue->AddRef();
        }

    }   // release lock

    if (SUCCEEDED(hr))
    {
        hr = pQueue->GetEvent(dwFlags, ppEvent);
    }

    SAFE_RELEASE(pQueue);

    return hr;
}

HRESULT WavSource::QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue)
{
    HRESULT hr = S_OK;

    CAutoLock lock(m_critSec);
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    }

    return hr;
}


// IMFMediaSource methods


//-------------------------------------------------------------------
// Name: CreatePresentationDescriptor
// Description: Returns a copy of the default presentation descriptor.
//-------------------------------------------------------------------

HRESULT WavSource::CreatePresentationDescriptor(IMFPresentationDescriptor** ppPresentationDescriptor)
{
    CAutoLock lock(m_critSec);

    if (ppPresentationDescriptor == NULL)
    {
        return E_POINTER;
    }


    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (m_pPresentationDescriptor == NULL)
        {
            hr = CreatePresentationDescriptor();
        }
    }

    if (SUCCEEDED(hr))
    {
        // Clone our default presentation descriptor.
        hr = m_pPresentationDescriptor->Clone(ppPresentationDescriptor);
    }    

    return hr;
}


//-------------------------------------------------------------------
// Name: GetCharacteristics
// Description: Returns flags the describe the source.
//-------------------------------------------------------------------

HRESULT WavSource::GetCharacteristics(DWORD* pdwCharacteristics)
{
    CAutoLock lock(m_critSec);

    if (pdwCharacteristics == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        *pdwCharacteristics =  MFMEDIASOURCE_CAN_PAUSE | MFMEDIASOURCE_CAN_SEEK;
    }

    return hr;

}

//-------------------------------------------------------------------
// Name: Start
// Description: Switches to running state.
//-------------------------------------------------------------------

HRESULT WavSource::Start(
                   IMFPresentationDescriptor* pPresentationDescriptor,
                   const GUID* pguidTimeFormat,
                   const PROPVARIANT* pvarStartPosition
                   )
{
    CAutoLock lock(m_critSec);

    PROPVARIANT var;
    LONGLONG llStartOffset = 0;
	BOOL bIsSeek = FALSE;    
	BOOL bIsRestartFromCurrentPosition = FALSE;     

    PropVariantInit(&var);

    // Fail if the source is shut down.
    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        // Check parameters. 
        // Start position and presentation descriptor cannot be NULL.
        if (pvarStartPosition == NULL || pPresentationDescriptor == NULL)
        {
            hr = E_INVALIDARG;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Check the time format. Must be "reference time" units.
        if ((pguidTimeFormat != NULL) && (*pguidTimeFormat != GUID_NULL))
        {
            // Unrecognized time format GUID.
            hr = MF_E_UNSUPPORTED_TIME_FORMAT;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Check the start position.
        if (pvarStartPosition->vt == VT_I8)
        {
            // Start position is given in pvarStartPosition in 100-ns units.
            llStartOffset = pvarStartPosition->hVal.QuadPart;

            if (m_state != STATE_STOPPED)
            {
                // Source is running or paused, so this is a seek.
                bIsSeek = TRUE;
            }

        }
        else if (pvarStartPosition->vt == VT_EMPTY)
        {
            // Start position is "current position". 
            // For stopped, that means 0. Otherwise, use the current position.
            if (m_state == STATE_STOPPED)
            {
                llStartOffset = 0;
            }
            else
            {
                llStartOffset = GetCurrentPosition();
				bIsRestartFromCurrentPosition = TRUE;
            }
        }
        else
        {
            // We don't support this time format.
            hr =  MF_E_UNSUPPORTED_TIME_FORMAT;
        }
    }


    if (SUCCEEDED(hr))
    {
        assert(pPresentationDescriptor != NULL);  // Checked this earlier.

        // Validate the caller's presentation descriptor.
        hr = ValidatePresentationDescriptor(pPresentationDescriptor);
    }

    if (SUCCEEDED(hr))
    {
        // Sends the MENewStream or MEUpdatedStream event.
        hr = QueueNewStreamEvent(pPresentationDescriptor);
    }

    if (SUCCEEDED(hr))
    {
        // Notify the stream of the new start time.
        hr = m_pStream->SetPosition(llStartOffset);
    }

    if (SUCCEEDED(hr))
    {
        // Send Started or Seeked events. We will send them 
        // 1. from the media source
        // 2. from each stream

        var.vt = VT_I8;
        var.hVal.QuadPart = llStartOffset;

        // (1) Send the source event.
        if (bIsSeek)
        {
            hr = QueueEvent(MESourceSeeked, GUID_NULL, hr, &var);
        }
        else
        {
            // For starting, if we are RESTARTING from the current position and our 
            // previous state was running/paused, then we need to add the 
            // MF_EVENT_SOURCE_ACTUAL_START attribute to the event. This requires
            // creating the event object first.

			IMFMediaEvent *pEvent = NULL;

            // Create the event.
			hr = MFCreateMediaEvent(
				MESourceStarted,
				GUID_NULL,
				hr, 
				&var,
				&pEvent
				);

            // For restarts, set the actual start time as an attribute.
			if (SUCCEEDED(hr))
			{
                if (bIsRestartFromCurrentPosition)
                {
    				hr = pEvent->SetUINT64(MF_EVENT_SOURCE_ACTUAL_START, llStartOffset);
                }
			}
            
            // Now  queue the event.
			if (SUCCEEDED(hr))
			{
				hr = m_pEventQueue->QueueEvent(pEvent);
			}

            SAFE_RELEASE(pEvent);
        }
    }

    if (SUCCEEDED(hr))
    {
        // 2. Send the stream event.
        if (m_pStream)
        {
            if (bIsSeek)
            {
                hr = m_pStream->QueueEvent(MEStreamSeeked, GUID_NULL, hr, &var);
            }
            else
            {
                hr = m_pStream->QueueEvent(MEStreamStarted, GUID_NULL, hr, &var);
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        // Update our state. 
        m_state = STATE_STARTED;
    }


    // NOTE: If this method were implemented as an asynchronous operation 
    // and the operation failed asynchronously, the media source would need
    // to send an MESourceStarted event with the failure code. For this
    // sample, all operations are synchronous (which is allowed), so any
    // failures are also synchronous.


    PropVariantClear(&var);

    return hr;
}


//-------------------------------------------------------------------
// Name: Pause
// Description: Switches to paused state.
//-------------------------------------------------------------------

HRESULT WavSource::Pause()
{
    CAutoLock lock(m_critSec);

    HRESULT hr = CheckShutdown();

    // Pause is only allowed from started state.
    if (SUCCEEDED(hr))
    {
        if (m_state != STATE_STARTED)
        {
            hr = MF_E_INVALID_STATE_TRANSITION;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Send the appropriate events.
        if (m_pStream)
        {
            hr = m_pStream->QueueEvent(MEStreamPaused, GUID_NULL, S_OK, NULL);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = QueueEvent(MESourcePaused, GUID_NULL, S_OK, NULL);
    }

    if (SUCCEEDED(hr))
    {
        // Update our state. 
        m_state = STATE_PAUSED;
    }

    // Nothing else for us to do.

    return hr;
}


//-------------------------------------------------------------------
// Name: Stop
// Description: Switches to stopped state.
//-------------------------------------------------------------------

HRESULT WavSource::Stop()
{
    CAutoLock lock(m_critSec);

    HRESULT hr = CheckShutdown();


    if (SUCCEEDED(hr))
    {
        // Queue events.
        if (m_pStream)
        {
            hr = m_pStream->QueueEvent(MEStreamStopped, GUID_NULL, S_OK, NULL);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = QueueEvent(MESourceStopped, GUID_NULL, S_OK, NULL);
    }

    if (SUCCEEDED(hr))
    {
        // Update our state. 
        m_state = STATE_STOPPED;
    }

    return hr;
}


//-------------------------------------------------------------------
// Name: Shutdown
// Description: Releases resources.
//
// The source and stream objects hold reference counts on each other.
// To avoid memory leaks caused by circular ref. counts, the Shutdown
// method releases the pointer to the stream.
//-------------------------------------------------------------------

HRESULT WavSource::Shutdown()
{
    CAutoLock lock(m_critSec);

    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        // Shut down the stream object.
        if (m_pStream)
        {
            hr = m_pStream->Shutdown();
        }

        // Shut down the event queue.
        if (m_pEventQueue)
        {
            m_pEventQueue->Shutdown();
        }

        // Release objects. (Even if Shutdown failed for some reason.)
        SAFE_RELEASE(m_pStream);
        SAFE_RELEASE(m_pEventQueue);
        SAFE_RELEASE(m_pPresentationDescriptor);
        SAFE_RELEASE(m_pRiff);

        // Set our shutdown flag.
        m_IsShutdown = TRUE;
    }

    return hr;
}


/// Other public methods


//-------------------------------------------------------------------
// Name: Open
// Description: Opens the source from a bytestream.
//
// The bytestream handler calls this method after it creates the 
// source. 
//
// Note: This method is not a public API. It is a custom method on 
// for our bytestream class to use.
//-------------------------------------------------------------------

HRESULT WavSource::Open(IMFByteStream *pStream)
{
    CAutoLock lock(m_critSec);

    if (m_pRiff != NULL)
    {
        // The media source has already been opened.
        return MF_E_INVALIDREQUEST;
    }

    HRESULT hr = S_OK;

    // Create a new WAVE RIFF parser object to parse the stream.
    hr = CWavRiffParser::Create(pStream, &m_pRiff);

    // Parse the WAVE header. This fails if the header is not
    // well-formed.
    if (SUCCEEDED(hr))
    {
        hr = m_pRiff->ParseWAVEHeader();
    }

    // Validate the WAVEFORMATEX structure from the file header.
    if (SUCCEEDED(hr))
    {
        hr = ValidateWaveFormat(m_pRiff->Format(), m_pRiff->FormatSize());
    }

    if (FAILED(hr))
    {
        Shutdown();
    }

    return hr;
}



/////////////// Private WavSource methods

// NOTE: These private methods do not hold the source's critical
// section. The caller must ensure the critical section is held.
// Also, these methods do not check for shut-down.



//-------------------------------------------------------------------
// Name: WaveFormat
// Description: 
// Returns a pointer to the WAVEFORMATEX structure that describes the
// audio format. Returns NULL if no format is set.
//-------------------------------------------------------------------

const WAVEFORMATEX* WavSource::WaveFormat() const 
{ 
    if (m_pRiff)
    {
        return m_pRiff->Format();
    }
    else
    {
        return NULL;
    }
}

//-------------------------------------------------------------------
// Name: WaveFormatSize
// Description: 
// Returns the size of the WAVEFORMATEX structure.
//-------------------------------------------------------------------

DWORD WavSource::WaveFormatSize() const 
{  
    if (m_pRiff)
    {
        return m_pRiff->FormatSize();
    }
    else
    {
        return 0;
    }
}



//-------------------------------------------------------------------
// Name: CreatePresentationDescriptor
// Description: Creates the default presentation descriptor.
//-------------------------------------------------------------------

HRESULT WavSource::CreatePresentationDescriptor()
{
    HRESULT hr = S_OK;

    IMFMediaType *pMediaType = NULL;
    IMFStreamDescriptor *pStreamDescriptor = NULL;
    IMFMediaTypeHandler *pHandler = NULL;

    assert(WaveFormat() != NULL);

    // Create an empty media type.
    hr = MFCreateMediaType(&pMediaType);

    // Initialize the media type from the WAVEFORMATEX structure.
    if (SUCCEEDED(hr))
    {
        hr = MFInitMediaTypeFromWaveFormatEx(pMediaType, WaveFormat(), WaveFormatSize());
    }

    // Create the stream descriptor.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateStreamDescriptor(
            0,          // stream identifier
            1,          // Number of media types.
            &pMediaType, // Array of media types
            &pStreamDescriptor
            );
    }

    // Set the default media type on the media type handler.
    if (SUCCEEDED(hr))
    {
        hr = pStreamDescriptor->GetMediaTypeHandler(&pHandler);
    }

    if (SUCCEEDED(hr))
    {
        hr = pHandler->SetCurrentMediaType(pMediaType);
    }

    // Create the presentation descriptor.
    if (SUCCEEDED(hr))
    {
        hr = MFCreatePresentationDescriptor(
            1,      // Number of stream descriptors
            &pStreamDescriptor, // Array of stream descriptors
            &m_pPresentationDescriptor
            );
    }

    // Select the first stream
    if (SUCCEEDED(hr))
    {
        hr = m_pPresentationDescriptor->SelectStream(0);
    }

    // Set the file duration as an attribute on the presentation descriptor.
    if (SUCCEEDED(hr))
    {
        MFTIME duration = m_pRiff->FileDuration();
        hr = m_pPresentationDescriptor->SetUINT64(MF_PD_DURATION, (UINT64)duration);
    }


    SAFE_RELEASE(pMediaType);
    SAFE_RELEASE(pStreamDescriptor);
    SAFE_RELEASE(pHandler);

    return hr;

}

//-------------------------------------------------------------------
// Name: ValidatePresentationDescriptor
// Description: Validates the caller's presentation descriptor.
//
// This method is called when Start() is called with a non-NULL
// presentation descriptor. The caller is supposed to give us back
// the same PD that we gave out in CreatePresentationDescriptor().
// This method performs a sanity check on the caller's PD to make 
// sure it matches ours.
//
// Note: Because this media source has one stream with single, fixed 
//       media type, there is not much for the caller to decide. In
//       a more complicated source, the caller might select different
//       streams, or select from a list of media types.
//-------------------------------------------------------------------

HRESULT WavSource::ValidatePresentationDescriptor(IMFPresentationDescriptor *pPD)
{
    HRESULT hr;

    assert(pPD != NULL);

    IMFStreamDescriptor *pStreamDescriptor = NULL;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFMediaType        *pMediaType = NULL;
    IMFAudioMediaType   *pAudioType = NULL;
    const WAVEFORMATEX  *pFormat = NULL;

    DWORD cStreamDescriptors = 0;
    BOOL fSelected = FALSE;

    // Make sure there is only one stream.
    hr = pPD->GetStreamDescriptorCount(&cStreamDescriptors);

    if (SUCCEEDED(hr))
    {
        if (cStreamDescriptors != 1)
        {
            hr = MF_E_UNSUPPORTED_REPRESENTATION;
        }
    }

    // Get the stream descriptor.
    if (SUCCEEDED(hr))
    {
        hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pStreamDescriptor);
    }

    // Make sure it's selected. (This media source has only one stream, so it
    // is not useful to deselect the only stream.)
    if (SUCCEEDED(hr))
    {
        if (!fSelected)
        {
            hr = MF_E_UNSUPPORTED_REPRESENTATION;
        }
    }


    // Get the media type handler, so that we can get the media type.
    if (SUCCEEDED(hr))
    {
        hr = pStreamDescriptor->GetMediaTypeHandler(&pHandler);
    }
    if (SUCCEEDED(hr))
    {
        hr = pHandler->GetCurrentMediaType(&pMediaType);
    }


    if (SUCCEEDED(hr))
    {
        hr = pMediaType->QueryInterface(IID_IMFAudioMediaType, (void**)&pAudioType);
    }

    if (SUCCEEDED(hr))
    {
        pFormat = pAudioType->GetAudioFormat();
        if ((pFormat == NULL) || (this->WaveFormat() == NULL))
        {
            hr = MF_E_INVALIDMEDIATYPE;
        }
    }

    if (SUCCEEDED(hr))
    {
        assert(this->WaveFormat() != NULL);

        if (memcmp(pFormat, WaveFormat(), WaveFormatSize()) != 0)
        {
            hr = MF_E_INVALIDMEDIATYPE;
        }
    }


    SAFE_RELEASE(pStreamDescriptor);
    SAFE_RELEASE(pHandler);
    SAFE_RELEASE(pMediaType);
    SAFE_RELEASE(pAudioType);
    
    return hr;

}


//-------------------------------------------------------------------
// Name: QueueNewStreamEvent
// Description: 
// Queues an MENewStream or MEUpdatedStream event during Start.
//
// pPD: The presentation descriptor.
//
// Precondition: The presentation descriptor is assumed to be valid. 
// Call ValidatePresentationDescriptor before calling this method.
//-------------------------------------------------------------------

HRESULT WavSource::QueueNewStreamEvent(IMFPresentationDescriptor *pPD)
{
    assert(pPD != NULL);

    HRESULT hr = S_OK;
    IMFStreamDescriptor *pSD = NULL;

    BOOL fSelected = FALSE;
    hr = pPD->GetStreamDescriptorByIndex(0, &fSelected, &pSD);

    if (SUCCEEDED(hr))
    {
        // The stream must be selected, because we don't allow the app
        // to de-select the stream. See ValidatePresentationDescriptor.
        assert(fSelected);

        if (m_pStream)
        {
            // The stream already exists, and is still selected.
            // Send the MEUpdatedStream event.

            hr = QueueEventWithIUnknown(this, MEUpdatedStream, S_OK, m_pStream); 
        }
        else
        {
            // The stream does not exist, and is now selected.
            // Create a new stream.

            hr = CreateWavStream(pSD);

            // CreateWavStream creates the stream, so m_pStream is no longer NULL.
            assert(m_pStream != NULL);

            if (SUCCEEDED(hr))
            {
                // Send the MENewStream event.
                hr = QueueEventWithIUnknown(this, MENewStream, S_OK, m_pStream);   
            }
        }
    }


    SAFE_RELEASE(pSD);

    return hr;
}

//-------------------------------------------------------------------
// Name: CreateWavStream
// Description: Creates the source's media stream object.
//-------------------------------------------------------------------

HRESULT WavSource::CreateWavStream(IMFStreamDescriptor *pSD)
{
    HRESULT hr = S_OK;
    m_pStream = new WavStream(this, m_pRiff, pSD, hr);

    if (m_pStream == NULL)
    {
        hr = E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        m_pStream->AddRef();
    }
    return hr;
}



//-------------------------------------------------------------------
// Name: GetCurrentPosition
// Description: Returns the current playback position.
//-------------------------------------------------------------------

LONGLONG WavSource::GetCurrentPosition() const
{
    if (m_pStream)
    {
        return m_pStream->GetCurrentPosition();
    }
    else
    {
        // If no stream is selected, we are at time 0 by definition.
        return 0;
    }
}



////////// AUDIO STREAM

//-------------------------------------------------------------------
// WavStream constructor.
//
// pSource: Parent media source.
// pSD: Stream descriptor that describes this stream.
// hr: If the constructor fails, this value is set to a failure code.
//-------------------------------------------------------------------


WavStream::WavStream(WavSource *pSource,  CWavRiffParser *pRiff, IMFStreamDescriptor *pSD, HRESULT& hr) :
    m_nRefCount(0),
    m_pEventQueue(NULL),
    m_IsShutdown(FALSE),
    m_rtCurrentPosition(0),
    m_discontinuity(FALSE),
    m_EOS(FALSE)
{
    m_pSource = pSource;
    m_pSource->AddRef();

    m_pStreamDescriptor = pSD;
    m_pStreamDescriptor->AddRef();

    m_pRiff = pRiff;
    m_pRiff->AddRef();


    // Create the media event queue.
    hr = MFCreateEventQueue(&m_pEventQueue);
  
}


//-------------------------------------------------------------------
// WavStream destructor.
//-------------------------------------------------------------------

WavStream::~WavStream()
{
    assert(m_IsShutdown);
    assert(m_nRefCount == 0);
}


// IUnknown methods

ULONG WavStream::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG  WavStream::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT WavStream::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IMFMediaEventGenerator))
    {
        *ppv = static_cast<IMFMediaEventGenerator*>(this);
    }
    else if (iid == __uuidof(IMFMediaStream))
    {
        *ppv = static_cast<IMFMediaStream*>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// IMFMediaEventGenerator methods
// [See note for WavSource class]

HRESULT WavStream::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState)
{
    HRESULT hr = S_OK;

    CAutoLock lock(m_critSec);
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->BeginGetEvent(pCallback, punkState);

    };

    return hr;
}

HRESULT WavStream::EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    CAutoLock lock(m_critSec);
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->EndGetEvent(pResult, ppEvent);
    };

    return hr;
}

HRESULT WavStream::GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    IMFMediaEventQueue *pQueue = NULL;
    
    { // scope for lock
    
        CAutoLock lock(m_critSec);

        hr = CheckShutdown();

        if (SUCCEEDED(hr))
        {
            pQueue = m_pEventQueue;
            pQueue->AddRef();
        }

    }

    if (SUCCEEDED(hr))
    {
        hr = pQueue->GetEvent(dwFlags, ppEvent);
    };

    SAFE_RELEASE(pQueue);

    return hr;
}

HRESULT WavStream::QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue)
{
    HRESULT hr = S_OK;

    CAutoLock lock(m_critSec);
    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    };

    return hr;
}


// IMFMediaStream methods.


//-------------------------------------------------------------------
// Name: GetMediaSource
// Description: Returns a pointer to the media source.
//-------------------------------------------------------------------

HRESULT WavStream::GetMediaSource(IMFMediaSource** ppMediaSource)
{
    CAutoLock lock(m_critSec);

    if (ppMediaSource == NULL)
    {
        return E_POINTER;
    }

    if (m_pSource == NULL)
    {
        return E_UNEXPECTED;
    }

    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = m_pSource->QueryInterface(IID_IMFMediaSource, (void**)ppMediaSource);
    };

    return hr;
}


//-------------------------------------------------------------------
// Name: GetStreamDescriptor
// Description: Returns the stream descriptor for this stream.
//-------------------------------------------------------------------

HRESULT WavStream::GetStreamDescriptor(IMFStreamDescriptor** ppStreamDescriptor)
{
    CAutoLock lock(m_critSec);

    if (ppStreamDescriptor == NULL)
    {
        return E_POINTER;
    }

    if (m_pStreamDescriptor == NULL)
    {
        return E_UNEXPECTED;
    }

    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        *ppStreamDescriptor = m_pStreamDescriptor;
        (*ppStreamDescriptor)->AddRef();

    };

    return hr;
}



//-------------------------------------------------------------------
// Name: RequestSample
// Description: Requests a new sample.
// 
// pToken: Token object. Can be NULL.
//-------------------------------------------------------------------

HRESULT WavStream::RequestSample(IUnknown* pToken)
{
    if (m_pSource == NULL)
    {
        return E_UNEXPECTED;
    }

    HRESULT hr = S_OK;

    IMFSample *pSample = NULL;  // Sample to deliver.
    BOOL bReachedEOS = FALSE;   // TRUE if we hit end-of-stream during this method.

    {
        // Scope for critical section
        CAutoLock lock(m_critSec);

        // Check if we are shut down.
        hr = CheckShutdown();

        // Check if we already reached the end of the stream.
        if (SUCCEEDED(hr))
        {
            if (m_EOS)
            {
                hr = MF_E_END_OF_STREAM;
            }
        }

        // Check the source is stopped.
        if (SUCCEEDED(hr))
        {
            // GetState does not hold the source's critical section. Safe to call.
            if (m_pSource->GetState() == WavSource::STATE_STOPPED)
            {
                hr = MF_E_INVALIDREQUEST;
            }
        }

        // If we succeeded to here, we are able to deliver a sample.

        // Create a new audio sample.
        if (SUCCEEDED(hr))
        {
            hr = CreateAudioSample(&pSample);
        }

        // If the caller provided a token, attach it to the sample as
        // an attribute. 
        if (SUCCEEDED(hr))
        {
            // NOTE: If we processed sample requests asynchronously, we would
            // need to call AddRef on the token and put the token onto a FIFO
            // queue. See documenation for IMFMediaStream::RequestSample.
            if (pToken)
            {
                hr = pSample->SetUnknown(MFSampleExtension_Token, pToken);
            }
        }

        // Send the MEMediaSample event with the new sample.
        if (SUCCEEDED(hr))
        {
            hr = QueueEventWithIUnknown(this, MEMediaSample, hr, pSample); 
        };

        // See if we reached the end of the stream.
        if (SUCCEEDED(hr))
        {
            hr = CheckEndOfStream();    // This method sends MEEndOfStream if needed.
            bReachedEOS = m_EOS;        // Cache this flag in a local variable.
        }

        SAFE_RELEASE(pSample);
    
    }  // Scope for critical section lock.


    // We only have one stream, so the end of the stream is also the end of the
    // presentation. Therefore, when we reach the end of the stream, we need to 
    // queue the end-of-presentation event from the source. Logically we would do 
    // this inside the CheckEndOfStream method. However, we cannot hold the
    // source's critical section while holding the stream's critical section, at
    // risk of deadlock. 

    if (SUCCEEDED(hr))
    {   
        if (bReachedEOS)
        {
            hr = m_pSource->QueueEvent(MEEndOfPresentation, GUID_NULL, S_OK, NULL);
        }
    }

    return hr;
}


///// Private WavStream methods

// NOTE: Some of these methods hold the stream's critical section
// because they are called by the media source object.

//-------------------------------------------------------------------
// Name: CreateAudioSample
// Description: Creates a new audio sample.
//-------------------------------------------------------------------

HRESULT WavStream::CreateAudioSample(IMFSample **ppSample)
{
    HRESULT hr = S_OK;

    IMFMediaBuffer *pBuffer = NULL;
    IMFSample *pSample = NULL;

    DWORD		cbBuffer = 0;
    BYTE		*pData = NULL;
    LONGLONG    duration = 0;
    BOOL		bBufferLocked = FALSE;

    // Start with one second of data, rounded up to the nearest block.
    cbBuffer = AlignUp<DWORD>(m_pRiff->Format()->nAvgBytesPerSec, m_pRiff->Format()->nBlockAlign);

    // Don't request any more than what's left.
    cbBuffer = min(cbBuffer, m_pRiff->BytesRemainingInChunk());

    // Create the buffer.
    hr = MFCreateMemoryBuffer(cbBuffer, &pBuffer);

    if (SUCCEEDED(hr))
    {
        // Get a pointer to the buffer memory.
        hr = pBuffer->Lock(&pData, NULL, NULL);

        // Set this flag so that we're sure to unlock the buffer, even if
        // the next call fails.
        bBufferLocked = SUCCEEDED(hr);
    }

    // Fill the buffer
    if (SUCCEEDED(hr))
    {
        hr = m_pRiff->ReadDataFromChunk(pData, cbBuffer);
    }

    // Unlock the buffer.
    if (bBufferLocked)
    {
        hr = pBuffer->Unlock();
    }

    // Set the size of the valid data in the buffer.
    if (SUCCEEDED(hr))
    {
        hr = pBuffer->SetCurrentLength(cbBuffer);
    }

    // Create a new sample and add the buffer to it.
    if (SUCCEEDED(hr))
    {
        hr = MFCreateSample(&pSample);
    }

    if (SUCCEEDED(hr))
    {
        hr = pSample->AddBuffer(pBuffer);
    }

    // Set the time stamps, duration, and sample flags.
    if (SUCCEEDED(hr))
    {
        hr = pSample->SetSampleTime(m_rtCurrentPosition);
    }

    if (SUCCEEDED(hr))
    {
        duration = AudioDurationFromBufferSize(m_pRiff->Format(), cbBuffer);
        hr = pSample->SetSampleDuration(duration);
    }

    if (SUCCEEDED(hr))
    {
        // Set the discontinuity flag.
        if (m_discontinuity)
        {
            hr = pSample->SetUINT32(MFSampleExtension_Discontinuity, TRUE);
        }
    }

    if (SUCCEEDED(hr))
    {
        // Update our current position.
        m_rtCurrentPosition += duration;

        // Give the pointer to the caller.
        *ppSample = pSample;
        (*ppSample)->AddRef();
    }


    SAFE_RELEASE(pBuffer);
    SAFE_RELEASE(pSample);

    return hr;

}

//-------------------------------------------------------------------
// Name: Shutdown
// Description: Notifies the stream that the source was shut down.
//-------------------------------------------------------------------

HRESULT WavStream::Shutdown()
{
    CAutoLock lock(m_critSec);

    // Shut down the event queue.
    if (m_pEventQueue)
    {
        m_pEventQueue->Shutdown();
    }

    // Release objects
    SAFE_RELEASE(m_pEventQueue);
    SAFE_RELEASE(m_pSource);
    SAFE_RELEASE(m_pStreamDescriptor);
    SAFE_RELEASE(m_pRiff);

    m_IsShutdown = TRUE;

    return S_OK;
}

//-------------------------------------------------------------------
// Name: SetPosition
// Description: Updates the new stream position.
//-------------------------------------------------------------------

HRESULT WavStream::SetPosition(LONGLONG rtNewPosition)
{
    CAutoLock lock(m_critSec);

    // Check if the requested position is beyond the end of the stream.
    LONGLONG duration = AudioDurationFromBufferSize(m_pRiff->Format(), m_pRiff->Chunk().DataSize());

    if (rtNewPosition > duration)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;

    if (m_rtCurrentPosition != rtNewPosition)
    {
        LONGLONG offset = BufferSizeFromAudioDuration(m_pRiff->Format(), rtNewPosition);

        // The chunk size is a DWORD. So if our calculations are correct, there is no
        // way that the maximum valid seek position can be larger than a DWORD. 
        assert(offset <= MAXDWORD);

        hr = m_pRiff->MoveToChunkOffset((DWORD)offset);
        if (SUCCEEDED(hr))
        {
            m_rtCurrentPosition = rtNewPosition;
            m_discontinuity = TRUE;
            m_EOS = FALSE;
        }
    }

    return hr;
}

HRESULT WavStream::CheckEndOfStream()
{
    HRESULT hr = S_OK;

    if (m_pRiff->BytesRemainingInChunk() < m_pRiff->Format()->nBlockAlign)
    {
        // The remaining data is smaller than the audio block size. (In theory there shouldn't be
        // partial bits of data at the end, so we should reach an even zero bytes, but the file
        // might not be authored correctly.)
        m_EOS = TRUE;

        // Send the end-of-stream event,
        hr = QueueEvent(MEEndOfStream, GUID_NULL, S_OK, NULL);
    }
    return hr; 
}



/////////////////

// CWavRiffParser is a specialization of the generic RIFF parser object,
// and is designed to parse .wav files.

CWavRiffParser::CWavRiffParser(IMFByteStream *pStream, HRESULT& hr) :
    m_pWaveFormat(NULL), m_cbWaveFormat(0), m_rtDuration(0),
    CRiffParser(pStream, FOURCC_RIFF, 0, hr)
{

}

CWavRiffParser::~CWavRiffParser()
{
    CoTaskMemFree(m_pWaveFormat);
}


//-------------------------------------------------------------------
// Name: Create
// Description: Static creation function.
//-------------------------------------------------------------------

HRESULT CWavRiffParser::Create(IMFByteStream *pStream, CWavRiffParser **ppParser)
{
    if (ppParser == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    // Create a riff parser for the 'RIFF' container
    CWavRiffParser *pParser = new CWavRiffParser(pStream, hr);

    if (pParser == NULL)
    {
        return E_OUTOFMEMORY;
    }

    // Check the RIFF file type.
    if (SUCCEEDED(hr))
    {
        if (pParser->RiffType() != ckid_WAVE_FILE)
        {
            hr = MF_E_INVALID_FILE_FORMAT;
        }
    }

    if (FAILED(hr))
    {
        delete pParser;
    }
    else
    {
        *ppParser = pParser;
        (*ppParser)->AddRef();
    }

    return hr;

}


//-------------------------------------------------------------------
// Name: ParseWAVEHeader
// Description: Parsers the RIFF WAVE header.
// 
// Note:
// .wav files should look like this:
//
// RIFF ('WAVE'
//       'fmt ' = WAVEFORMATEX structure
//       'data' = audio data
//       )
//-------------------------------------------------------------------

HRESULT CWavRiffParser::ParseWAVEHeader()
{
    HRESULT hr = S_OK;
    BOOL bFoundData = FALSE;

    // Iterate through the RIFF chunks. Ignore chunks we don't recognize.
    while (SUCCEEDED(hr))
    {
        if (Chunk().FourCC() == ckid_WAVE_FMT)
        {
            // Read the WAVEFORMATEX structure allegedly contained in this chunk.
            // This method does NOT validate the contents of the structure.
            hr = ReadFormatBlock();
        }
        else if (Chunk().FourCC() == ckid_WAVE_DATA)
        {
            // Found the start of the audio data. The format chunk should precede the
            // data chunk. If we did not find the formt chunk yet, that is a failure 
            // case (see below)
            bFoundData = TRUE;
            break;
        }

        hr = MoveToNextChunk();
    }

    if (SUCCEEDED(hr))
    {
        // To be valid, the file must have a format chunk and a data chunk.
        // Fail if either of these conditions is not met.
        if (m_pWaveFormat == NULL || !bFoundData)
        {
            hr = MF_E_INVALID_FILE_FORMAT;
        }
    }

    if (SUCCEEDED(hr))
    {
        m_rtDuration =AudioDurationFromBufferSize(m_pWaveFormat, Chunk().DataSize());
    }

    return hr;

}

//-------------------------------------------------------------------
// Name: ReadFormatBlock
// Description: Reads the WAVEFORMATEX structure from the file header.
//-------------------------------------------------------------------

HRESULT CWavRiffParser::ReadFormatBlock()
{
    assert(Chunk().FourCC() == ckid_WAVE_FMT);
    assert(m_pWaveFormat == NULL);

    HRESULT hr = S_OK;

	// Some .wav files do not include the cbSize field of the WAVEFORMATEX 
    // structure. For uncompressed PCM audio, field is always zero. 
	const DWORD cbMinFormatSize = sizeof(WAVEFORMATEX) - sizeof(WORD);

	DWORD cbFormatSize = 0;		// Size of the actual format block in the file.

    // Validate the size
    if (Chunk().DataSize() < cbMinFormatSize)
    {
        hr = MF_E_INVALID_FILE_FORMAT;
    }

    // Allocate a buffer for the WAVEFORMAT structure.
    if (SUCCEEDED(hr))
    {
		cbFormatSize = Chunk().DataSize();
		
		// We store a WAVEFORMATEX structure, so our format block must be at 
		// least sizeof(WAVEFORMATEX) even if the format block in the file
		// is smaller. See note above about cbMinFormatSize.
		m_cbWaveFormat = max(cbFormatSize, sizeof(WAVEFORMATEX));

        m_pWaveFormat = (WAVEFORMATEX*)CoTaskMemAlloc(m_cbWaveFormat);
        if (m_pWaveFormat == NULL)
        {
            hr = E_OUTOFMEMORY;
        }
    }

    if (SUCCEEDED(hr))
    {
		// Zero our structure, in case cbFormatSize < m_cbWaveFormat.
		ZeroMemory(m_pWaveFormat, m_cbWaveFormat);

		// Now read cbFormatSize bytes from the file.
        hr = ReadDataFromChunk((BYTE*)m_pWaveFormat, cbFormatSize);
    }


    if (FAILED(hr))
    {
        CoTaskMemFree(m_pWaveFormat);
        m_pWaveFormat = NULL;
        m_cbWaveFormat = 0;
    }

    return hr;
}



//-------------------------------------------------------------------
// Name: QueueEventWithIUnknown
// Description: Helper function to queue an event with an IUnknown
//              pointer value.
//
// pMEG:        Media event generator that will queue the event.
// meType:      Media event type.
// hrStatus:    Status code for the event.
// pUnk:        IUnknown pointer value.
//
//-------------------------------------------------------------------


HRESULT QueueEventWithIUnknown(
    IMFMediaEventGenerator *pMEG,
    MediaEventType meType,
    HRESULT hrStatus,
    IUnknown *pUnk)
{

    // Create the PROPVARIANT to hold the IUnknown value.
    PROPVARIANT var;
    var.vt = VT_UNKNOWN;
    var.punkVal = pUnk;
    pUnk->AddRef();

    // Queue the event.
    HRESULT hr = pMEG->QueueEvent(meType, GUID_NULL, hrStatus, &var);

    // Clear the PROPVARIANT.
    PropVariantClear(&var);

    return hr;
}



//-------------------------------------------------------------------
// Name: ValidateWaveFormat
// Description: Validates a WAVEFORMATEX structure. 
//
// This method is called when the byte stream handler opens the
// source. The WAVEFORMATEX structure is copied directly from the 
// .wav file. Therefore the source should not trust any of the
// values in the format header.
//
// Just to keep the sample as simple as possible, we only accept 
// uncompressed PCM formats in this media source.
//-------------------------------------------------------------------


HRESULT ValidateWaveFormat(const WAVEFORMATEX *pWav, DWORD cbSize)
{
    if (pWav->wFormatTag != WAVE_FORMAT_PCM)
    {
        return MF_E_INVALIDMEDIATYPE;
    }

    if (pWav->nChannels != 1 && pWav->nChannels != 2)
    {
        return MF_E_INVALIDMEDIATYPE;
    }

    if (pWav->wBitsPerSample != 8 && pWav->wBitsPerSample != 16)
    {
        return MF_E_INVALIDMEDIATYPE;
    }

    if (pWav->cbSize != 0)
    {
        return MF_E_INVALIDMEDIATYPE;
    }

    // Make sure block alignment was calculated correctly.
    if (pWav->nBlockAlign != pWav->nChannels * (pWav->wBitsPerSample / 8))
    {   
        return MF_E_INVALIDMEDIATYPE;
    }

    // Check possible overflow...
    if (pWav->nSamplesPerSec  > (DWORD)(MAXDWORD / pWav->nBlockAlign))        // Is (nSamplesPerSec * nBlockAlign > MAXDWORD) ?
    {
        return MF_E_INVALIDMEDIATYPE;
    }

    // Make sure average bytes per second was calculated correctly.
    if (pWav->nAvgBytesPerSec != pWav->nSamplesPerSec * pWav->nBlockAlign)
    {
        return MF_E_INVALIDMEDIATYPE;
    }

    // Everything checked out.
    return S_OK;
}

LONGLONG AudioDurationFromBufferSize(const WAVEFORMATEX *pWav, DWORD cbAudioDataSize)
{
    assert(pWav != NULL);

    if (pWav->nAvgBytesPerSec == 0)
    {
        return 0;
    }
    return MulDiv(cbAudioDataSize, 10000000, pWav->nAvgBytesPerSec);
}

LONGLONG BufferSizeFromAudioDuration(const WAVEFORMATEX *pWav, LONGLONG duration)
{

    LONGLONG cbSize = duration * pWav->nAvgBytesPerSec / ONE_SECOND;

    ULONG ulRemainder = (ULONG)(cbSize % pWav->nBlockAlign);

    // Round up to the next block. 
    if(ulRemainder) 
    {
        cbSize += pWav->nBlockAlign - ulRemainder;
    }

    return cbSize;
}


