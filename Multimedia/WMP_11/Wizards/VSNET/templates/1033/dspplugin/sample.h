/////////////////////////////////////////////////////////////////////////////
//
// [!output root].h : Declaration of C[!output Safe_root]
//
// Note: Requires DirectX 8 SDK or later.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////
  
#ifndef __C[!output SAFE_ROOT]_H_
#define __C[!output SAFE_ROOT]_H_

#include "resource.h"
#include <mediaobj.h>       // The IMediaObject header from the DirectX SDK.
#include "wmpservices.h"    // The header containing the WMP interface definitions.
#include "./[!output root]PS/[!output root]_h.h"  // Generated from the IDL file during proxy/stub compilation.

const DWORD UNITS = 10000000;  // 1 sec = 1 * UNITS
const DWORD MAXSTRING = 1024;

// registry location for preferences
const TCHAR kszPrefsRegKey[] = _T("Software\\[!output root]\\DSP Plugin");
const TCHAR kszPrefsScaleFactor[] = _T("ScaleFactor");

// {[!output CLASSID]}
DEFINE_GUID(CLSID_[!output Safe_root], [!output DEFINEGUID]);

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE C[!output Safe_root] : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<C[!output Safe_root], &CLSID_[!output Safe_root]>,
    public I[!output Safe_root],
    public IMediaObject,
    public IWMPPluginEnable,
    public ISpecifyPropertyPages
{
public:
    C[!output Safe_root]();
    virtual ~C[!output Safe_root]();

DECLARE_REGISTRY_RESOURCEID(IDR_[!output SAFE_ROOT])

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(C[!output Safe_root])
    COM_INTERFACE_ENTRY(I[!output Safe_root])
    COM_INTERFACE_ENTRY(IMediaObject)
    COM_INTERFACE_ENTRY(IWMPPluginEnable)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
END_COM_MAP()

    // CComCoClass Overrides
    HRESULT FinalConstruct();
    void    FinalRelease();

    // I[!output Safe_root] methods
    STDMETHOD(get_scale)(double *pVal);
    STDMETHOD(put_scale)(double newVal);

    // IMediaObject methods
    STDMETHOD( GetStreamCount )( 
                   DWORD *pcInputStreams,
                   DWORD *pcOutputStreams
                   );
    
    STDMETHOD( GetInputStreamInfo )( 
                   DWORD dwInputStreamIndex,
                   DWORD *pdwFlags
                   );
    
    STDMETHOD( GetOutputStreamInfo )( 
                   DWORD dwOutputStreamIndex,
                   DWORD *pdwFlags
                   );
    
    STDMETHOD( GetInputType )( 
                   DWORD dwInputStreamIndex,
                   DWORD dwTypeIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( GetOutputType )( 
                   DWORD dwOutputStreamIndex,
                   DWORD dwTypeIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( SetInputType )( 
                   DWORD dwInputStreamIndex,
                   const DMO_MEDIA_TYPE *pmt,
                   DWORD dwFlags
                   );
    
    STDMETHOD( SetOutputType )( 
                   DWORD dwOutputStreamIndex,
                   const DMO_MEDIA_TYPE *pmt,
                   DWORD dwFlags
                   );
    
    STDMETHOD( GetInputCurrentType )( 
                   DWORD dwInputStreamIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( GetOutputCurrentType )( 
                   DWORD dwOutputStreamIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( GetInputSizeInfo )( 
                   DWORD dwInputStreamIndex,
                   DWORD *pcbSize,
                   DWORD *pcbMaxLookahead,
                   DWORD *pcbAlignment
                   );
    
    STDMETHOD( GetOutputSizeInfo )( 
                   DWORD dwOutputStreamIndex,
                   DWORD *pcbSize,
                   DWORD *pcbAlignment
                   );
    
    STDMETHOD( GetInputMaxLatency )( 
                   DWORD dwInputStreamIndex,
                   REFERENCE_TIME *prtMaxLatency
                   );
    
    STDMETHOD( SetInputMaxLatency )( 
                   DWORD dwInputStreamIndex,
                   REFERENCE_TIME rtMaxLatency
                   );
    
    STDMETHOD( Flush )( void );
    
    STDMETHOD( Discontinuity )( 
                   DWORD dwInputStreamIndex
                   );
    
    STDMETHOD( AllocateStreamingResources )( void );
    
    STDMETHOD( FreeStreamingResources )( void );
    
    STDMETHOD( GetInputStatus )( 
                   DWORD dwInputStreamIndex,
                   DWORD *pdwFlags
                   );
    
    STDMETHOD( ProcessInput )( 
                   DWORD dwInputStreamIndex,
                   IMediaBuffer *pBuffer,
                   DWORD dwFlags,
                   REFERENCE_TIME rtTimestamp,
                   REFERENCE_TIME rtTimelength
                   );
    
    STDMETHOD( ProcessOutput )( 
                   DWORD dwFlags,
                   DWORD cOutputBufferCount,
                   DMO_OUTPUT_DATA_BUFFER *pOutputBuffers,
                   DWORD *pdwStatus
                   );

    STDMETHOD( Lock )( LONG bLock );

    // Note: need to override CComObjectRootEx::Lock to avoid
    // ambiguity with IMediaObject::Lock. The override just
    // calls through to the base class implementation.

    // CComObjectRootEx overrides
    void Lock()
    {
        CComObjectRootEx<CComMultiThreadModel>::Lock();
    }

    // IWMPPluginEnable methods
    STDMETHOD( SetEnable )( BOOL fEnable );
    STDMETHOD( GetEnable )( BOOL *pfEnable );

    // ISpecifyPropertyPages methods
    STDMETHOD( GetPages )(CAUUID *pPages);
    
private:
    HRESULT DoProcessOutput(
                BYTE *pbOutputData,             // Pointer to the output buffer
                const BYTE *pbInputData,        // Pointer to the input buffer
                DWORD *cbBytesProcessed);       // Number of bytes processed
    HRESULT ValidateMediaType(
                const DMO_MEDIA_TYPE *pmtTarget,    // target media type to verify
                const DMO_MEDIA_TYPE *pmtPartner);  // partner media type to verify

    DMO_MEDIA_TYPE          m_mtInput;          // Stores the input format structure
    DMO_MEDIA_TYPE          m_mtOutput;         // Stores the output format structure

    CComPtr<IMediaBuffer>   m_spInputBuffer;    // Smart pointer to the input buffer object
    BYTE*                   m_pbInputData;      // Pointer to the data in the input buffer
    DWORD                   m_cbInputLength;    // Length of the data in the input buffer
   
    bool                    m_bValidTime;       // Is timestamp valid?
    REFERENCE_TIME          m_rtTimestamp;      // Stores the input buffer timestamp

    double                  m_fScaleFactor;     // Scale factor
    BOOL                    m_bEnabled;         // TRUE if enabled
};

#endif //__C[!output SAFE_ROOT]_H_
