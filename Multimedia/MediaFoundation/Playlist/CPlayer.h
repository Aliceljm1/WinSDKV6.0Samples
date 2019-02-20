#include "MF_SeqSrc_Playback.h"
#include "Utils.h"
#include "CSegmentList.h"


//////////////////////////////////////////////////////////////////////////
//  CPlayer
//  Description: 
//    Implements the a playlist scenario using the sequencer source.
//    This enables the application to playback multiple sources and provide 
//    seamless transition from one media source to the other.
//    The application creates topologies for the media sources and adds them to 
//    the sequencer source which is queued on the media session. The media session
//    provides transport control such as play, pause, and stop.
//////////////////////////////////////////////////////////////////////////

class CPlayer : public IMFAsyncCallback
{
private:
    IMFSequencerSource      *m_pSequencerSource;
    IMFPresentationClock    *m_pPresentationClock;
    IMFMediaSession         *m_pMediaSession;
    IMFActivate             *m_pAudioRendererActivate;

    // Application's window handle.
    HWND    m_hWnd;

    //Player State
    PlayerState    m_State;

    // SegmentList object
    CSegmentList   m_Segments;

    // Timing data from the MESessionNotifyPresentationTime event.
    MFTIME    m_PresentationTimeOffset;

	TimePair *m_phnsTimePairStart;

	TimePair *m_phnsTimePairEnd;

    long    m_nRefCount;

    // Event to wait on while closing

    HANDLE    m_hCloseEvent;

	TopologyStatusInfo topostat;

	MFSequencerElementId   m_ActiveSegment;


    HRESULT AddSegment (
        const WCHAR *sURL, 
        MFSequencerElementId *pSegmentId);
    
    HRESULT CreateMediaSource ( 
        const WCHAR *sURL, 
        IMFMediaSource **ppMediaSource);

    HRESULT CreateTopology(
        IMFMediaSource *pMediaSource, 
        IMFTopology *pTopology);
    
    HRESULT CreateNodesForStream (
        IMFPresentationDescriptor *const pPresentationDescriptor,
        IMFMediaSource *const pMediaSource, 
        IMFTopology *const pTopology);

    HRESULT CreateSourceNode (
        IMFPresentationDescriptor *const pPresentationDescriptor,
        IMFStreamDescriptor *const pStreamDescriptor, 
        IMFMediaSource *const pMediaSource, 
        IMFTopologyNode **const ppSourceNode);

    HRESULT CreateOutputNode ( 
        IMFStreamDescriptor *const pStreamDescriptor, 
        IMFTopologyNode **const ppOutputNode);

    HRESULT AddTopologyToSequencer(
        const WCHAR *sURL, 
        IMFMediaSource *pMediaSource, 
        IMFTopology *pTopology, 
        MFSequencerElementId *pSegmentId);

    HRESULT QueueNextSegment (
        IMFPresentationDescriptor *pPresentationDescriptor,
        MFSequencerElementId *pSegmentId);

    HRESULT HandleNotifyPresentationTime(IMFMediaEvent *const pEvent);


public:
    CPlayer(HWND hWnd);
    virtual ~CPlayer(void);

    // Implementation of CAsyncCallback::Invoke.

    HRESULT Initialize(void);
    
    HRESULT AddToPlaylist(const WCHAR *sURL);
    
    HRESULT DeleteSegment(MFSequencerElementId SegmentID);
    
    HRESULT Play(void);
    
    HRESULT Pause(void);
    
    HRESULT Stop(void);
    
    HRESULT Skip (const MFSequencerElementId SegmentID);
    
    HRESULT ShutDown(void);

    HRESULT GetCurrentSegmentTime(MFTIME *phnsCurrentTime);

    HRESULT GetPresentationTime(MFTIME *phnsPresentationTime);

    PlayerState GetState() const { return m_State; }
    
    HRESULT GetSegmentInfo(
        MFSequencerElementId SegmentID, 
        MFTIME *SegmentDuration, 
        WCHAR *szSegmentURL, 
        DWORD dwSize);
    
    int GetSegmentCount(){return m_Segments.GetCount();};

    //IMFAsyncCallback methods

    STDMETHODIMP_(ULONG) AddRef();

    STDMETHODIMP_(ULONG) Release();

    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

    STDMETHODIMP Invoke(IMFAsyncResult*);

    // Implementation of this method is optional.
    STDMETHODIMP GetParameters(DWORD*, DWORD*) { return E_NOTIMPL; }

};
