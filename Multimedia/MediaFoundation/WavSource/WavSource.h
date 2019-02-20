//////////////////////////////////////////////////////////////////////
//
// WavSource.h : Sample audio media source for Media Foundation
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Notes:
// This sample implements a relatively simple audio source
// to parse .wav files. 
// 
// Design decisions:
//
// - For simplicity, the source performs all methods synchronously.
// - Also for simplicity, the source only accepts uncompressed PCM audio
//   formats.
// - It does not support rate control. (This feature would be fairly 
//   easy to add to the sample.)
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include <windows.h>
#include <assert.h>

#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mferror.h>

#include "utils.h"
#include "RiffParser.h"

// SAFE_RELEASE macro.
// Releases a COM pointer if the pointer is not NULL, and sets the pointer to NULL.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }
#endif

const LONGLONG ONE_SECOND = 10000000;


// Forward declares

class WavStream;
class WavSource;
class CWavRiffParser;


//////////////////////////////////////////////////////////////////////////
//  WavSource
//  Description: Media source object.
//////////////////////////////////////////////////////////////////////////

class WavSource : public IMFMediaSource
{
    friend class WavStream;

public:
    static HRESULT CreateInstance(REFIID iid, void **ppSource);

    // IUnknown
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

    // IMFMediaEventGenerator
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback,IUnknown* punkState);
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent);
    STDMETHODIMP GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent);
    STDMETHODIMP QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue);

    // IMFMediaSource
    STDMETHODIMP CreatePresentationDescriptor(IMFPresentationDescriptor** ppPresentationDescriptor);
    STDMETHODIMP GetCharacteristics(DWORD* pdwCharacteristics);
    STDMETHODIMP Pause();
    STDMETHODIMP Shutdown();
    STDMETHODIMP Start(
        IMFPresentationDescriptor* pPresentationDescriptor,
        const GUID* pguidTimeFormat,
        const PROPVARIANT* pvarStartPosition
    );
    STDMETHODIMP Stop();


    // Other methods
    HRESULT Open(IMFByteStream *pStream);



private:

    enum State
    {
        STATE_STOPPED,
        STATE_PAUSED,
        STATE_STARTED
    };


    // Constructor is private - client should use static CreateInstance method.
    WavSource(HRESULT &hr);
    virtual ~WavSource();

    HRESULT CheckShutdown() const 
    {
        if (m_IsShutdown)
        {
            return MF_E_SHUTDOWN;
        }
        else
        {
            return S_OK;
        }
    }

    HRESULT     CreatePresentationDescriptor();
    HRESULT     QueueNewStreamEvent(IMFPresentationDescriptor *pPD);
    HRESULT     CreateWavStream(IMFStreamDescriptor *pSD);
    HRESULT     ValidatePresentationDescriptor(IMFPresentationDescriptor *pPD);

    LONGLONG    GetCurrentPosition() const;
    State       GetState() const { return m_state; }

    const WAVEFORMATEX*     WaveFormat() const;             // Returns a pointer to the format.
    DWORD                   WaveFormatSize() const;         // Returns the size of the format, in bytes.

    IMFMediaEventQueue          *m_pEventQueue;             // Event generator helper
    IMFPresentationDescriptor   *m_pPresentationDescriptor; // Default presentation

    WavStream                   *m_pStream;                 // Media stream. Can be NULL is no stream is selected.

    long                        m_nRefCount;                // reference count
    CCritSec                    m_critSec;                  // critical section for thread safety
    BOOL                        m_IsShutdown;               // Flag to indicate if Shutdown() method was called.
    State                       m_state;                    // Current state (running, stopped, paused)

    CWavRiffParser              *m_pRiff;
};


//////////////////////////////////////////////////////////////////////////
//  WavStream
//  Description: Media stream object.
//////////////////////////////////////////////////////////////////////////


class WavStream : public IMFMediaStream 
{
    friend class WavSource;

public:

    // IUnknown
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

    // IMFMediaEventGenerator
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback,IUnknown* punkState);
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent);
    STDMETHODIMP GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent);
    STDMETHODIMP QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue);

    // IMFMediaStream
    STDMETHODIMP GetMediaSource(IMFMediaSource** ppMediaSource);
    STDMETHODIMP GetStreamDescriptor(IMFStreamDescriptor** ppStreamDescriptor);
    STDMETHODIMP RequestSample(IUnknown* pToken);
    
private:

    WavStream(WavSource *pSource, CWavRiffParser *pRiff, IMFStreamDescriptor *pSD, HRESULT& hr);
    ~WavStream();


    HRESULT CheckShutdown() const
    {
        if (m_IsShutdown)
        {
            return MF_E_SHUTDOWN;
        }
        else
        {
            return S_OK;
        }
    }

    HRESULT     Shutdown();
    HRESULT     CreateAudioSample(IMFSample **pSample);
    LONGLONG    GetCurrentPosition() const { return m_rtCurrentPosition; }
    HRESULT     SetPosition(LONGLONG rtNewPosition);
    HRESULT     CheckEndOfStream();


    long                        m_nRefCount;            // reference count
    CCritSec                    m_critSec;              // critical section for thread safety
    BOOL                        m_IsShutdown;           // Flag to indicate if source's Shutdown() method was called.
    LONGLONG                    m_rtCurrentPosition;    // Current position in the stream, in 100-ns units 
    BOOL                        m_discontinuity;        // Is the next sample a discontinuity?
    BOOL                        m_EOS;                  // Did we reach the end of the stream?

    CWavRiffParser              *m_pRiff;
    IMFMediaEventQueue          *m_pEventQueue;         // Event generator helper.
    WavSource                   *m_pSource;             // Parent media source
    IMFStreamDescriptor         *m_pStreamDescriptor;   // Stream descriptor for this stream.

};


//////////////////////////////////////////////////////////////////////////
//  CWavRiffParser
//  Description: Parses the RIFF file structure.
//////////////////////////////////////////////////////////////////////////

class CWavRiffParser : public CRiffParser
{
public:
    static HRESULT      Create(IMFByteStream *pStream, CWavRiffParser **ppParser);

    HRESULT             ParseWAVEHeader();

    const WAVEFORMATEX* Format() const { return m_pWaveFormat; }
    DWORD               FormatSize() const { return m_cbWaveFormat; }

    MFTIME              FileDuration() const { return m_rtDuration; } 

private:
    
    CWavRiffParser(IMFByteStream *pStream, HRESULT& hr);
    ~CWavRiffParser();

    HRESULT             ReadFormatBlock();

    WAVEFORMATEX        *m_pWaveFormat;
    DWORD               m_cbWaveFormat;

    MFTIME              m_rtDuration;               // File duration.

};
