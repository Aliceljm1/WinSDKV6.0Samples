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
    public IMediaObject,
    public IWMPPluginEnable,
    public ISpecifyPropertyPages,
    public IPropertyBag,
    public I[!output Safe_root]
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
    COM_INTERFACE_ENTRY(IPropertyBag)
END_COM_MAP()

    // CComCoClass Overrides
    HRESULT FinalConstruct();
    void    FinalRelease();

    // I[!output Safe_root] methods
    STDMETHOD(get_scale)(double *pVal);
    STDMETHOD(put_scale)(double newVal);
    STDMETHOD(LoadResourceImage)(IStream **ppStream);

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

    // IPropertyBag methods
    STDMETHOD ( Read )(
                    LPCOLESTR pszPropName,
                    VARIANT *pVar,
                    IErrorLog *pErrorLog
                    );

    STDMETHOD ( Write )(
                    LPCOLESTR pszPropName,
                    VARIANT *pVar
                    );

    static const GUID* k_guidValidSubtypes[];
    static const DWORD k_dwValidSubtypesCount;
    
private:
    HRESULT DoProcessOutput(
                BYTE *pbOutputData,             // Pointer to the output buffer
                const BYTE *pbInputData,        // Pointer to the input buffer
                DWORD *cbBytesProcessed);       // Number of bytes processed
    HRESULT ValidateMediaType(
                const DMO_MEDIA_TYPE *pmtTarget,    // target media type to verify
                const DMO_MEDIA_TYPE *pmtPartner);  // partner media type to verify
    DWORD GetSampleSize( const DMO_MEDIA_TYPE* pmt ); // Get the sample size from the media type.
    HRESULT GetVideoInfoParameters(             // Get the important information out of a VIDEOINFOHEADER
                BYTE  * const pbData,        
                DWORD *pdwWidth,         
                DWORD *pdwHeight,        
                LONG  *plStrideInBytes,  
                BYTE **ppbTop,           
                bool bYuv);
    HRESULT GetBitmapInfoHeader(
                const DMO_MEDIA_TYPE *pmt,
                BITMAPINFOHEADER *pbmih);
    HRESULT GetTargetRECT(
                const DMO_MEDIA_TYPE *pmt,
                RECT *prcTarget);
    HRESULT ProcessYUY2(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT ProcessNV12(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT ProcessYV12(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT ProcessUYVY(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process24Bit(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process32Bit(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process565(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process555(
                BYTE *pbIputData, 
                BYTE *pbOutputData);

    DMO_MEDIA_TYPE          m_mtInput;          // Stores the input format structure
    DMO_MEDIA_TYPE          m_mtOutput;         // Stores the output format structure

    CComPtr<IMediaBuffer>   m_spInputBuffer;    // Smart pointer to the input buffer object
    DWORD                   m_dwBufferSize;

    bool                    m_bValidTime;       // Is timestamp valid?
    bool                    m_bValidLength;
    REFERENCE_TIME          m_rtTimestamp;      // Stores the input buffer timestamp
    REFERENCE_TIME          m_rtTimelength;     // Stores the input buffer timelength.

    double                  m_fScaleFactor;     // Scale factor
    BOOL                    m_bEnabled;         // TRUE if enabled
};

#endif //__C[!output SAFE_ROOT]_H_
