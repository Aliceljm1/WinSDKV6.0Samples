//////////////////////////////////////////////////////////////////////////
//
// player.h : Playback helper class.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#pragma once

// Private window message to notify the application of playback events.
const UINT WM_APP_NOTIFY = WM_APP + 1;   // wparam = state

// Private window message to notify the application when an error occurs.
const UINT WM_APP_ERROR = WM_APP + 2;    // wparam = HRESULT

enum PlayerState
{
	Ready = 0,
	OpenPending,
	Started,
	PausePending,
	Paused,
	StartPending,
};

class CPlayer : public IMFAsyncCallback
{
public:
	static HRESULT CreateInstance(HWND hVideo, HWND hEvent, CPlayer **ppPlayer);

    // IUnknown methods
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

    // IMFAsyncCallback methods
    STDMETHODIMP GetParameters(DWORD*, DWORD*)
    {
        // Implementation of this method is optional.
        return E_NOTIMPL;
    }

	STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult);

	// Other methods

	HRESULT		OpenURL(const WCHAR *sURL);
	HRESULT		Play();
	HRESULT		Pause();
	HRESULT		Stop();
	HRESULT		Shutdown();

	// Video functionality
	HRESULT		Repaint();
	HRESULT		ResizeVideo(WORD width, WORD height);

	PlayerState GetState() const { return m_state; }
	BOOL		HasVideo() const { return (m_pVideoDisplay != NULL); }

protected:
	
	// Constructor is private. Use static CreateInstance method to instantiate.
	CPlayer(HWND hVideo, HWND hEvent, HRESULT &hr);

    // Destructor is private. Caller should call Release.
	virtual ~CPlayer();


    virtual HRESULT Initialize();


	// NotifyState: Notifies the application when the state changes.
	void NotifyState()
	{
		PostMessage(m_hwndEvent, WM_APP_NOTIFY, (WPARAM)m_state, (LPARAM)0);
	}

	// NotifyState: Notifies the application when an error occurs.
	void NotifyError(HRESULT hr)
	{
		TRACE((L"NotifyError: 0x%X", hr));
        m_state = Ready;
		PostMessage(m_hwndEvent, WM_APP_ERROR, (WPARAM)hr, 0);
	}

	HRESULT CreateSession();
	HRESULT CloseSession();
	HRESULT StartPlayback();
	HRESULT CreateMediaSource(const WCHAR *sURL);
    HRESULT CreateTopologyFromSource(IMFTopology **ppTopology);

    HRESULT AddBranchToPartialTopology(
		IMFTopology *pTopology, 
		IMFPresentationDescriptor *pSourcePD, 
		DWORD iStream
		);

	HRESULT CreateSourceStreamNode(
		IMFPresentationDescriptor *pSourcePD, 
		IMFStreamDescriptor *pSourceSD,
		IMFTopologyNode **ppNode
		);

	HRESULT CreateOutputNode(
		IMFStreamDescriptor *pSourceSD, 
		IMFTopologyNode **ppNode
		);

	// Media event handlers
	HRESULT OnTopologyReady(IMFMediaEvent *pEvent);
	HRESULT OnSessionStarted(IMFMediaEvent *pEvent);
	HRESULT OnSessionPaused(IMFMediaEvent *pEvent);
	HRESULT OnSessionClosed(IMFMediaEvent *pEvent);
	HRESULT OnPresentationEnded(IMFMediaEvent *pEvent);

    long                    m_nRefCount;        // Reference count.

	IMFMediaSession			*m_pSession;
	IMFMediaSource			*m_pSource;
	IMFVideoDisplayControl	*m_pVideoDisplay;

	HWND				    m_hwndVideo;		// Video window.
	HWND				    m_hwndEvent;		// App window to receive events.
	PlayerState			    m_state;			// Current state of the media session.
	HANDLE				    m_hCloseEvent;		// Event to wait on while closing
};