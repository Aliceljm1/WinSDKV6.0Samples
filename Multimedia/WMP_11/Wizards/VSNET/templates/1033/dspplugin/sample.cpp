/////////////////////////////////////////////////////////////////////////////
//
// [!output root].cpp : Implementation of C[!output Safe_root]
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "[!output root].h"
#include "[!output Root]PropPage.h"

#include <mediaerr.h>   // DirectX SDK media errors
#include <dmort.h>      // DirectX SDK DMO runtime support
#include <uuids.h>      // DirectX SDK media types and subtyes
#include <ks.h>         // required for WAVEFORMATEXTENSIBLE
#include <mmreg.h>
#include <ksmedia.h>

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::C[!output Safe_root]
//
// Constructor
/////////////////////////////////////////////////////////////////////////////

C[!output Safe_root]::C[!output Safe_root]()
{
    m_fScaleFactor = 1.0;   // default to a scale factor of 1.0
    m_cbInputLength = 0;
    m_pbInputData = NULL;
    m_bValidTime = false;
    m_rtTimestamp = 0;
    m_bEnabled = TRUE;

    ::ZeroMemory(&m_mtInput, sizeof(m_mtInput));
    ::ZeroMemory(&m_mtOutput, sizeof(m_mtOutput));
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::~C[!output Safe_root]
//
// Destructor
/////////////////////////////////////////////////////////////////////////////

C[!output Safe_root]::~C[!output Safe_root]()
{
    ::MoFreeMediaType(&m_mtInput);
    ::MoFreeMediaType(&m_mtOutput);
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::FinalConstruct
//
// Called when an plug-in is first loaded. Use this function to do one-time
// intializations that could fail instead of doing this in the constructor,
// which cannot return an error.
/////////////////////////////////////////////////////////////////////////////

HRESULT C[!output Safe_root]::FinalConstruct()
{
    CRegKey key;
    LONG    lResult;
    DWORD   dwValue = 0;

    // read DWORD scale factor from registry and convert to double
    lResult = key.Open(HKEY_CURRENT_USER, kszPrefsRegKey, KEY_READ);
    if (ERROR_SUCCESS == lResult)
    {
[!if VSNET]
        DWORD dwType = 0;
        ULONG uLength = sizeof(dwValue);
        lResult = key.QueryValue(kszPrefsScaleFactor, &dwType, &dwValue, &uLength);
[!else]
        lResult = key.QueryValue(dwValue, kszPrefsScaleFactor );
[!endif]
    if (ERROR_SUCCESS == lResult)
        {
            m_fScaleFactor = dwValue / 65536.0;
        }
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]:::FinalRelease
//
// Called when an plug-in is unloaded. Use this function to free any
// resources allocated.
/////////////////////////////////////////////////////////////////////////////

void C[!output Safe_root]::FinalRelease()
{
    FreeStreamingResources();  // In case client does not call this.        
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetStreamCount
//
// Implementation of IMediaObject::GetStreamCount
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::GetStreamCount( 
               DWORD *pcInputStreams,
               DWORD *pcOutputStreams)
{
    HRESULT hr = S_OK;

    if ( NULL == pcInputStreams )
    {
        return E_POINTER;
    }

    if ( NULL == pcOutputStreams )
    {
        return E_POINTER;
    }

    // The plug-in uses one stream in each direction.
    *pcInputStreams = 1;
    *pcOutputStreams = 1;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetInputStreamInfo
//
// Implementation of IMediaObject::GetInputStreamInfo
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetInputStreamInfo( 
               DWORD dwInputStreamIndex,
               DWORD *pdwFlags)
{    
    if ( NULL == pdwFlags )
    {
        return E_POINTER;
    }

    // The stream index must be zero.
    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    // Use the default input stream configuration (a single stream). 
    *pdwFlags = 0;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetOutputStreamInfo
//
// Implementation of IMediaObject::GetOutputStreamInfo
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetOutputStreamInfo( 
               DWORD dwOutputStreamIndex,
               DWORD *pdwFlags)
{
    if ( NULL == pdwFlags )
    {
        return E_POINTER;
    }

    // The stream index must be zero.
    if ( 0 != dwOutputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    // Use the default output stream configuration (a single stream).
    *pdwFlags = 0;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetInputType
//
// Implementation of IMediaObject::GetInputType
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetInputType ( 
               DWORD dwInputStreamIndex,
               DWORD dwTypeIndex,
               DMO_MEDIA_TYPE *pmt)
{
    HRESULT hr = S_OK;

    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX ;
    }

    // only support one preferred type
    if ( 0 != dwTypeIndex )
    {
        return DMO_E_NO_MORE_ITEMS;
    }

    if ( NULL == pmt )
    {
       return E_POINTER;
    
    }

    // if output type has been defined, use that as input type
    if (GUID_NULL != m_mtOutput.majortype)
    {
        hr = ::MoCopyMediaType( pmt, &m_mtOutput );
    }
    else // otherwise use default for this plug-in
    {
        ::ZeroMemory( pmt, sizeof( DMO_MEDIA_TYPE ) );
        pmt->majortype = MEDIATYPE_Audio;
        pmt->subtype = MEDIASUBTYPE_PCM;
    }

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetOutputType
//
// Implementation of IMediaObject::GetOutputType
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetOutputType( 
               DWORD dwOutputStreamIndex,
               DWORD dwTypeIndex,
               DMO_MEDIA_TYPE *pmt)
{
    HRESULT hr = S_OK;

    if ( 0 != dwOutputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    // only support one preferred type
    if ( 0 != dwTypeIndex )
    {
        return DMO_E_NO_MORE_ITEMS;
    
    }

    if ( NULL == pmt )
    {
       return E_POINTER;
    
    }

    // if input type has been defined, use that as output type
    if (GUID_NULL != m_mtInput.majortype)
    {
        hr = ::MoCopyMediaType( pmt, &m_mtInput );
    }
    else // other use default for this plug-in
    {
        ::ZeroMemory( pmt, sizeof( DMO_MEDIA_TYPE ) );
        pmt->majortype = MEDIATYPE_Audio;
        pmt->subtype = MEDIASUBTYPE_PCM;
    }

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::SetInputType
//
// Implementation of IMediaObject::SetInputType
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::SetInputType( 
               DWORD dwInputStreamIndex,
               const DMO_MEDIA_TYPE *pmt,
               DWORD dwFlags)
{
    HRESULT hr = S_OK;

    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( DMO_SET_TYPEF_CLEAR & dwFlags ) 
    {
        ::MoFreeMediaType(&m_mtInput);
        ::ZeroMemory(&m_mtInput, sizeof(m_mtInput));

        return S_OK;
    }

    if ( NULL == pmt )
    {
       return E_POINTER;
    }

    // validate that the input media type matches our requirements and
    // and matches our output type (if set)
    hr = ValidateMediaType(pmt, &m_mtOutput);

    if (FAILED(hr))
    {
        if( DMO_SET_TYPEF_TEST_ONLY & dwFlags )
        {
            hr = S_FALSE;
        }
    }
    else if ( 0 == dwFlags )
    {
        // free existing media type
        ::MoFreeMediaType(&m_mtInput);
        ::ZeroMemory(&m_mtInput, sizeof(m_mtInput));

        // copy new media type
        hr = ::MoCopyMediaType( &m_mtInput, pmt );
    }

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::SetOutputType
//
// Implementation of IMediaObject::SetOutputType
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::SetOutputType( 
               DWORD dwOutputStreamIndex,
               const DMO_MEDIA_TYPE *pmt,
               DWORD dwFlags)
{ 
    HRESULT hr = S_OK;

    if ( 0 != dwOutputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if( DMO_SET_TYPEF_CLEAR & dwFlags )
    {
        ::MoFreeMediaType( &m_mtOutput );
        ::ZeroMemory(&m_mtOutput, sizeof(m_mtOutput));

        return S_OK;
    }

    if ( NULL == pmt )
    {
        return E_POINTER;
    }

    // validate that the output media type matches our requirements and
    // and matches our input type (if set)
    hr = ValidateMediaType(pmt, &m_mtInput);

    if (FAILED(hr))
    {
        if( DMO_SET_TYPEF_TEST_ONLY & dwFlags )
        {
            hr = S_FALSE;
        }
    }
    else if ( 0 == dwFlags )
    {
        // free existing media type
        ::MoFreeMediaType(&m_mtOutput);
        ::ZeroMemory(&m_mtOutput, sizeof(m_mtOutput));

        // copy new media type
        hr = ::MoCopyMediaType( &m_mtOutput, pmt );
    }

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetInputCurrentType
//
// Implementation of IMediaObject::GetInputCurrentType
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::GetInputCurrentType( 
               DWORD dwInputStreamIndex,
               DMO_MEDIA_TYPE *pmt)
{
    HRESULT hr = S_OK;

    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( NULL == pmt )
    {
        return E_POINTER;
    }

    if (GUID_NULL == m_mtInput.majortype)
    {
        return DMO_E_TYPE_NOT_SET;
    }

    hr = ::MoCopyMediaType( pmt, &m_mtInput );

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetOutputCurrentType
//
// Implementation of IMediaObject::GetOutputCurrentType
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetOutputCurrentType( 
               DWORD dwOutputStreamIndex,
               DMO_MEDIA_TYPE *pmt)
{
    HRESULT hr = S_OK;

    if ( 0 != dwOutputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( NULL == pmt )
    {
        return E_POINTER;
    }

    if (GUID_NULL == m_mtOutput.majortype)
    {
        return DMO_E_TYPE_NOT_SET;
    }

    hr = ::MoCopyMediaType( pmt, &m_mtOutput );

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetInputSizeInfo
//
// Implementation of IMediaObject::GetInputSizeInfo
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetInputSizeInfo( 
               DWORD dwInputStreamIndex,
               DWORD *pcbSize,
               DWORD *pcbMaxLookahead,
               DWORD *pcbAlignment)
{
    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( NULL == pcbSize )
    {
       return E_POINTER;
    }

    if ( NULL == pcbMaxLookahead )
    {
        return E_POINTER;
    }

    if ( NULL == pcbAlignment )
    {
       return E_POINTER;
    }

    if (GUID_NULL == m_mtInput.majortype)
    {
        return DMO_E_TYPE_NOT_SET;
    }

    // Return the input sample size, in bytes.
    *pcbSize = m_mtInput.lSampleSize;

    // This plug-in doesn't perform lookahead. Return zero.
    *pcbMaxLookahead = 0;

    // Get the pointer to the input format structure.
    WAVEFORMATEX *pWave = ( WAVEFORMATEX * ) m_mtInput.pbFormat;

    // Return the input buffer alignment, in bytes.
    *pcbAlignment = pWave->nBlockAlign;
  
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetOutputSizeInfo
//
// Implementation of IMediaObject::GetOutputSizeInfo
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetOutputSizeInfo( 
               DWORD dwOutputStreamIndex,
               DWORD *pcbSize,
               DWORD *pcbAlignment)
{
    if ( 0 != dwOutputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( NULL == pcbSize )
    {
        return E_POINTER;
    }

    if ( NULL == pcbAlignment )
    {
        return E_POINTER;
    }

    if (GUID_NULL == m_mtOutput.majortype)
    {
        return DMO_E_TYPE_NOT_SET;
    }

    // Return the output sample size, in bytes.
    *pcbSize = m_mtOutput.lSampleSize;

    // Get the pointer to the output format structure.
    WAVEFORMATEX *pWave = ( WAVEFORMATEX * ) m_mtOutput.pbFormat;

    // Return the output buffer aligment, in bytes.
    *pcbAlignment = pWave->nBlockAlign;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetInputMaxLatency
//
// Implementation of IMediaObject::GetInputMaxLatency
/////////////////////////////////////////////////////////////////////////////
   
STDMETHODIMP C[!output Safe_root]::GetInputMaxLatency( 
               DWORD dwInputStreamIndex,
               REFERENCE_TIME *prtMaxLatency)
{
    return E_NOTIMPL; // Not dealing with latency in this plug-in.
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::SetInputMaxLatency
//
// Implementation of IMediaObject::SetInputMaxLatency
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::SetInputMaxLatency( 
               DWORD dwInputStreamIndex,
               REFERENCE_TIME rtMaxLatency)
{
    return E_NOTIMPL; // Not dealing with latency in this plug-in.
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::Flush
//
// Implementation of IMediaObject::Flush
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::Flush( void )
{
    m_spInputBuffer = NULL;  // release smart pointer
    m_cbInputLength = 0;
    m_pbInputData = NULL;
    m_bValidTime = false;
    m_rtTimestamp = 0;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::Discontinuity
//
// Implementation of IMediaObject::Discontinuity
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::Discontinuity( 
               DWORD dwInputStreamIndex)
{
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::AllocateStreamingResources
//
// Implementation of IMediaObject::AllocateStreamingResources
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::AllocateStreamingResources ( void )
{
    // Allocate any buffers need to process the stream. This plug-in does
    // all processing in-place, so it requires no extra buffers.

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::FreeStreamingResources
//
// Implementation of IMediaObject::FreeStreamingResources
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::FreeStreamingResources( void )
{
    m_spInputBuffer = NULL; // release smart pointer
    m_cbInputLength = 0;
    m_pbInputData = NULL;
    m_bValidTime = false;
    m_rtTimestamp = 0;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetInputStatus
//
// Implementation of IMediaObject::GetInputStatus
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::GetInputStatus( 
           DWORD dwInputStreamIndex,
           DWORD *pdwFlags)
{ 
    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( NULL == pdwFlags )
    {
        return E_POINTER;
    }

    if ( m_spInputBuffer )
    {
        *pdwFlags = 0; //The buffer still contains data; return zero.
    }
    else
    {
        *pdwFlags = DMO_INPUT_STATUSF_ACCEPT_DATA; // OK to call ProcessInput.
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::ProcessInput
//
// Implementation of IMediaObject::ProcessInput
/////////////////////////////////////////////////////////////////////////////
    
STDMETHODIMP C[!output Safe_root]::ProcessInput( 
               DWORD dwInputStreamIndex,
               IMediaBuffer *pBuffer,
               DWORD dwFlags,
               REFERENCE_TIME rtTimestamp,
               REFERENCE_TIME rtTimelength)
{ 
    HRESULT hr = S_OK;

    if ( 0 != dwInputStreamIndex )
    {
        return DMO_E_INVALIDSTREAMINDEX;
    }

    if ( NULL == pBuffer )
    {
        return E_POINTER;
    }

    if (GUID_NULL == m_mtInput.majortype)
    {
        return DMO_E_TYPE_NOT_SET;
    }

    // Get a pointer to the actual data and length information.
    BYTE    *pbInputData = NULL;
    DWORD   cbInputLength = 0;
    hr = pBuffer->GetBufferAndLength(&pbInputData, &cbInputLength);
    if (FAILED(hr))
    {
        return hr;
    }

    // Hold on to the buffer using a smart pointer.
    m_spInputBuffer = pBuffer;
    m_pbInputData = pbInputData;
    m_cbInputLength = cbInputLength;

    //Verify that buffer's time stamp is valid.
    if (dwFlags & DMO_INPUT_DATA_BUFFERF_TIME)
    {
        m_bValidTime = true;
        m_rtTimestamp = rtTimestamp;
    }
    else
    {
        m_bValidTime = false;
    }

    return S_OK;
}
    
/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::ProcessOutput
//
// Implementation of IMediaObject::ProcessOutput
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::ProcessOutput( 
               DWORD dwFlags,
               DWORD cOutputBufferCount,
               DMO_OUTPUT_DATA_BUFFER *pOutputBuffers,
               DWORD *pdwStatus)
{
    HRESULT hr = S_OK;

    if ( NULL == pOutputBuffers )
    {
        return E_POINTER;
    }

    // this plug-in only supports one output buffer
    if (1 != cOutputBufferCount)
    {
        return E_INVALIDARG;
    }

    if (GUID_NULL == m_mtOutput.majortype)
    {
        return DMO_E_TYPE_NOT_SET;
    }

    if (pdwStatus)
    {
        *pdwStatus = 0;
    }

    // make sure input and output buffer exist
    IMediaBuffer *pOutputBuffer = pOutputBuffers[0].pBuffer;

    if ((!m_spInputBuffer) || (!pOutputBuffer))
    {
        if (pOutputBuffer)
        {
            pOutputBuffer->SetLength(0);
        }

        pOutputBuffers[0].dwStatus = 0;

        return S_FALSE;
    }
    
    BYTE         *pbOutputData = NULL;
    DWORD        cbOutputMaxLength = 0;
    DWORD        cbBytesProcessed = 0;

    // Get current length of output buffer
    hr = pOutputBuffer->GetBufferAndLength(&pbOutputData, &cbOutputMaxLength);
    if (FAILED(hr))
    {
        return hr;
    }

    // Get max length of output buffer
    hr = pOutputBuffer->GetMaxLength(&cbOutputMaxLength);
    if (FAILED(hr))
    {
        return hr;
    }

    // Calculate how many bytes we can process
    bool bComplete = false; // The entire buffer is not yet processed.

    if (m_cbInputLength > cbOutputMaxLength)
    {
        cbBytesProcessed = cbOutputMaxLength; // only process as much of the input as can fit in the output
    }
    else
    {
        cbBytesProcessed = m_cbInputLength; // process entire input buffer
        bComplete = true;                   // the entire input buffer has been processed. 
    }

    // Call the internal processing method, which returns the no. bytes processed
    hr = DoProcessOutput(pbOutputData, m_pbInputData, &cbBytesProcessed);
    if (FAILED(hr))
    {
        return hr;
    }

    // Set the size of the valid data in the output buffer.
    hr = pOutputBuffer->SetLength(cbBytesProcessed);
    if (FAILED(hr))
    {
        return hr;
    }

    // Update the DMO_OUTPUT_DATA_BUFFER information for the output buffer.
    pOutputBuffers[0].dwStatus = 0;

    if (m_bValidTime)
    {
        // store start time of output buffer
        pOutputBuffers[0].dwStatus |= DMO_OUTPUT_DATA_BUFFERF_TIME;
        pOutputBuffers[0].rtTimestamp = m_rtTimestamp;
    
        // Get the pointer to the output format structure.
        WAVEFORMATEX *pWave = ( WAVEFORMATEX * ) m_mtOutput.pbFormat;

        // estimate time length of output buffer
        pOutputBuffers[0].dwStatus |= DMO_OUTPUT_DATA_BUFFERF_TIMELENGTH;
        pOutputBuffers[0].rtTimelength = ::MulDiv(cbBytesProcessed, UNITS, pWave->nAvgBytesPerSec);

        // this much time has been consumed, so move the time stamp accordingly
        m_rtTimestamp += pOutputBuffers[0].rtTimelength;
    }

    if (bComplete) 
    {
        m_spInputBuffer = NULL;   // Release smart pointer
        m_cbInputLength = 0;
        m_pbInputData = NULL;
        m_bValidTime = false;
        m_rtTimestamp = 0;
    }
    else 
    {
        // Let the client know there is still data that needs processing 
        // in the input buffer.
        pOutputBuffers[0].dwStatus |= DMO_OUTPUT_DATA_BUFFERF_INCOMPLETE;
        m_pbInputData += cbBytesProcessed;
        m_cbInputLength -= cbBytesProcessed;
    }
 
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::Lock
//
// Implementation of IMediaObject::Lock
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::Lock( LONG bLock )
{
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::SetEnable
//
// Implementation of IWMPPluginEnable::SetEnable
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::SetEnable( BOOL fEnable )
{
    // This function is called when the plug-in is being enabled or disabled,
    // typically by user action. Once a plug-in is disabled, it will still be
    // loaded into the graph but ProcessInput and ProcessOutput will not be called.

    // This function allows any state or UI associated with the plug-in to reflect the
    // enabled/disable state of the plug-in

    m_bEnabled = fEnable;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetEnable
//
// Implementation of IWMPPluginEnable::GetEnable
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::GetEnable( BOOL *pfEnable )
{
    if ( NULL == pfEnable )
    {
        return E_POINTER;
    }

    *pfEnable = m_bEnabled;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::GetPages
//
// Implementation of ISpecifyPropertyPages::GetPages
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::GetPages(CAUUID *pPages)
{
    // Only one property page is required for the plug-in.
    pPages->cElems = 1;
    pPages->pElems = (GUID *) (CoTaskMemAlloc(sizeof(GUID)));

    // Make sure memory is allocated for pPages->pElems
    if (NULL == pPages->pElems)
    {
        return E_OUTOFMEMORY;
    }

    // Return the property page's class ID
    *(pPages->pElems) = CLSID_[!output Safe_root]PropPage;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::get_scale
//
// Property get to retrieve the scale value via the public interface.
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::get_scale(double *pVal)
{
    if ( NULL == pVal )
    {
        return E_POINTER;
    }

    *pVal = m_fScaleFactor;

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::put_scale
//
// Property put to store the scale value via the public interface.
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP C[!output Safe_root]::put_scale(double newVal)
{
    m_fScaleFactor = newVal;
    
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::DoProcessOutput
//
// Convert the input buffer to the output buffer
/////////////////////////////////////////////////////////////////////////////

HRESULT C[!output Safe_root]::DoProcessOutput(
                            BYTE *pbOutputData,
                            const BYTE *pbInputData,
                            DWORD *cbBytesProcessed)
{
    // see if the plug-in has been disabled by the user
    if (!m_bEnabled)
    {
        // if so, just copy the data without changing it. You should
        // also do any neccesary format conversion here.
        memcpy(pbOutputData, pbInputData, *cbBytesProcessed);

        return S_OK;
    }

    WAVEFORMATEX *pWave = ( WAVEFORMATEX * ) m_mtInput.pbFormat;

    DWORD dwSamplesToProcess = (*cbBytesProcessed / pWave->nBlockAlign) * pWave->nChannels;

    // Note: for 8 and 16-bit samples, we assume the container is the same size as
    // the samples. For > 16-bit samples, we need to use the WAVEFORMATEXTENSIBLE
    // structure to determine the valid bits per sample.

    switch (pWave->wBitsPerSample)
    {
    case 8:
        {
            // return no. bytes actually copied to output buffer
            *cbBytesProcessed = dwSamplesToProcess * sizeof(BYTE);

            // 8-bit sound is 0..255 with 128 == silence
            while (dwSamplesToProcess--)
            {
                // Get the input sample and normalize to -128 .. 127
                int i = (*pbInputData++) - 128;

                // Apply scale factor to sample
                i = int( ((double) i) * m_fScaleFactor );
            
                // Truncate if exceeded full scale.
                if (i > 127)
                    i = 127;
                if (i < -128)
                    i = -128;

                // Convert back to 0..255 and write to output buffer.
                *pbOutputData++ = (BYTE)(i + 128);
            }
        }
        break;

    case 16:
        {
            // return no. bytes actually copied to output buffer
            *cbBytesProcessed = dwSamplesToProcess * sizeof(short);

            // 16-bit sound is -32768..32767 with 0 == silence
            short   *pwInputData = (short *) pbInputData;
            short   *pwOutputData = (short *) pbOutputData;
 
            while (dwSamplesToProcess--)
            {
                // Get the input sample
                int i = *pwInputData++;

                // Apply scale factor to sample
                i = int( ((double) i) * m_fScaleFactor );
            
                // Truncate if exceeded full scale.
                if (i > 32767)
                    i = 32767;
                if (i < -32768)
                    i = -32768;

                // Write to output buffer.
                *pwOutputData++ = i;
            }
        }
        break;

    case 24:
        {
            // return no. bytes actually copied to output buffer
            *cbBytesProcessed = dwSamplesToProcess * 3;

            WAVEFORMATEXTENSIBLE *pWaveXT = (WAVEFORMATEXTENSIBLE *) pWave;

            while (dwSamplesToProcess--)
            {
                // Get the input sample
                int i = (char) pbInputData[2];
                i = (i << 8) | pbInputData[1];

                int iClip = 0;
                switch (pWaveXT->Samples.wValidBitsPerSample)
                {
                case 16:
                    iClip = (1 << 15);
                    break;
                case 20:
                    i = (i << 4) | (pbInputData[0] >> 4);
                    iClip = (1 << 19);
                    break;
                case 24:
                    i = (i << 8) | pbInputData[0];
                    iClip = (1 << 23);
                    break;
                }

                pbInputData += 3;

                // Apply scale factor to sample
                i = int( ((double) i) * m_fScaleFactor );
            
                // Truncate if exceeded full scale.
                if (i > (iClip - 1))
                    i = iClip - 1;
                if (i < -iClip)
                    i = -iClip;

                // Write to output buffer.
                *pbOutputData++ = i & 0xFF;
                *pbOutputData++ = (i >> 8) & 0xFF;
                *pbOutputData++ = (i >> 16) & 0xFF;
            }
        }
        break;

    case 32:
        {
            // return no. bytes actually copied to output buffer
            *cbBytesProcessed = dwSamplesToProcess * 4;

            switch (pWave->wFormatTag)
            {
            case WAVE_FORMAT_IEEE_FLOAT:
                {
                    float  *pfInputData = (float *) pbInputData;
                    float  *pfOutputData = (float *) pbOutputData;

                    while (dwSamplesToProcess--)
                    {
                        // Do sample value scaling
                        *pfOutputData++ = *pfInputData++ * (float)m_fScaleFactor;
                    }
                }
                break;

            case WAVE_FORMAT_EXTENSIBLE:
                {
                    WAVEFORMATEXTENSIBLE *pWaveXT = (WAVEFORMATEXTENSIBLE *) pWave;

                    if (pWaveXT->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) 
                    {
                        float  *pfInputData = (float *) pbInputData;
                        float  *pfOutputData = (float *) pbOutputData;

                        while (dwSamplesToProcess--)
                        {
                            // Do sample value scaling
                            *pfOutputData++ = *pfInputData++ * (float)m_fScaleFactor;
                        }
                    }
                    else if (pWaveXT->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) 
                    {
                        long   *plInputData = (long *) pbInputData;
                        long   *plOutputData = (long *) pbOutputData;

                        while (dwSamplesToProcess--)
                        {
                            // Get the input sample
                            int i = *plInputData++;

                            int iClip = 0;
                            switch (pWaveXT->Samples.wValidBitsPerSample)
                            {
                            case 16:
                                i >>= 16;
                                iClip = (1 << 15);
                                break;
                            case 20:
                                i >>= 12;
                                iClip = (1 << 19);
                                break;
                            case 24:
                                i >>= 8;
                                iClip = (1 << 23);
                                break;
                            case 32:
                                iClip = (1 << 31);
                                break;
                            }

                            // Apply scale factor to sample
                            double f = ((double) i) * m_fScaleFactor;
                        
                            // Truncate if exceeded full scale.
                            if (f > (iClip - 1))
                                f = iClip - 1;
                            if (f < -iClip)
                                f = -iClip;

                            // Write to output buffer.
                            *plOutputData++ = (int) f;
                        }
                    }
                }
                break;

            default:
                // should never happen
                _ASSERT(false);
                *cbBytesProcessed = 0;
                return E_FAIL;
                break;
            }
        }
        break;

    default:
        // return no. bytes actually copied to output buffer
        *cbBytesProcessed = 0;
        return E_FAIL;
        break;
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::ValidateMediaType
//
// Validate that the media type is acceptable
/////////////////////////////////////////////////////////////////////////////

HRESULT C[!output Safe_root]::ValidateMediaType(const DMO_MEDIA_TYPE *pmtTarget, const DMO_MEDIA_TYPE *pmtPartner)
{
    // make sure the target media type has the fields we require
    if( ( MEDIATYPE_Audio != pmtTarget->majortype ) || 
        ( FORMAT_WaveFormatEx != pmtTarget->formattype ) ||
        ( pmtTarget->cbFormat < sizeof( WAVEFORMATEX )) ||
        ( NULL == pmtTarget->pbFormat) )
    {
        return DMO_E_TYPE_NOT_ACCEPTED;
    }

    // make sure the wave header has the fields we require
    WAVEFORMATEX *pWave = (WAVEFORMATEX *) pmtTarget->pbFormat;

    if ((0 == pWave->nChannels) ||
        (0 == pWave->nSamplesPerSec) ||
        (0 == pWave->nAvgBytesPerSec) ||
        (0 == pWave->nBlockAlign) ||
        (0 == pWave->wBitsPerSample))
    {
        return DMO_E_TYPE_NOT_ACCEPTED;
    }

    // make sure this is a supported container size
    if ((8  != pWave->wBitsPerSample) &&
        (16 != pWave->wBitsPerSample) &&
        (24 != pWave->wBitsPerSample) &&
        (32 != pWave->wBitsPerSample))
    {
        return DMO_E_TYPE_NOT_ACCEPTED;
    }

    // make sure the wave format is acceptable
    switch (pWave->wFormatTag)
    {
    case WAVE_FORMAT_PCM:

        // make sure sample size is 8 or 16-bit
        if ((8  != pWave->wBitsPerSample) &&
            (16 != pWave->wBitsPerSample))
        {
            return DMO_E_TYPE_NOT_ACCEPTED;
        }
        break;

    case WAVE_FORMAT_IEEE_FLOAT:

        // make sure the input is sane
        if (32 != pWave->wBitsPerSample)
        {
            return DMO_E_TYPE_NOT_ACCEPTED;
        }
        break;

    case WAVE_FORMAT_EXTENSIBLE:
        {
            WAVEFORMATEXTENSIBLE *pWaveXT = (WAVEFORMATEXTENSIBLE *) pWave;

            // make sure the wave format extensible has the fields we require
            if ((KSDATAFORMAT_SUBTYPE_PCM != pWaveXT->SubFormat &&
                 KSDATAFORMAT_SUBTYPE_IEEE_FLOAT != pWaveXT->SubFormat) ||
                (0 == pWaveXT->Samples.wSamplesPerBlock) ||
                (pWaveXT->Samples.wValidBitsPerSample > pWave->wBitsPerSample))
            {
                return DMO_E_TYPE_NOT_ACCEPTED;
            }

            // for 8 or 16-bit, the container and sample size must match
            if ((8  == pWave->wBitsPerSample) ||
                (16 == pWave->wBitsPerSample))
            {
                if (pWave->wBitsPerSample != pWaveXT->Samples.wValidBitsPerSample)
                {
                    return DMO_E_TYPE_NOT_ACCEPTED;
                }
            }
            else 
            {
                // for any other container size, make sure the valid
                // bits per sample is a value we support
                if ((16 != pWaveXT->Samples.wValidBitsPerSample) &&
                    (20 != pWaveXT->Samples.wValidBitsPerSample) &&
                    (24 != pWaveXT->Samples.wValidBitsPerSample) &&
                    (32 != pWaveXT->Samples.wValidBitsPerSample))
                {
                    return DMO_E_TYPE_NOT_ACCEPTED;
                }
            }
        }
        break;

    default:
        return DMO_E_TYPE_NOT_ACCEPTED;
        break;
    }

    // if the partner media type is configured, make sure it matches the target.
    // this is done because this plug-in requires the same input and output types
    if (GUID_NULL != pmtPartner->majortype)
    {
        if ((pmtTarget->majortype != pmtPartner->majortype) ||
            (pmtTarget->subtype   != pmtPartner->subtype))
        {
            return DMO_E_TYPE_NOT_ACCEPTED;
        }

        // make sure the wave headers for the target and the partner match
        WAVEFORMATEX *pPartnerWave = (WAVEFORMATEX *) pmtPartner->pbFormat;

        if ((pWave->nChannels != pPartnerWave->nChannels) ||
            (pWave->nSamplesPerSec != pPartnerWave->nSamplesPerSec) ||
            (pWave->nAvgBytesPerSec != pPartnerWave->nAvgBytesPerSec) ||
            (pWave->nBlockAlign != pPartnerWave->nBlockAlign) ||
            (pWave->wBitsPerSample != pPartnerWave->wBitsPerSample) ||
            (pWave->wFormatTag != pPartnerWave->wFormatTag))
        {
            return DMO_E_TYPE_NOT_ACCEPTED;
        }
       
        // make sure the waveformatextensible types are the same
        if (pWave->wFormatTag == WAVE_FORMAT_EXTENSIBLE) 
        {
            WAVEFORMATEXTENSIBLE *pWaveXT = (WAVEFORMATEXTENSIBLE *) pWave;
            WAVEFORMATEXTENSIBLE *pPartnerWaveXT = (WAVEFORMATEXTENSIBLE *) pPartnerWave;
            if (pWaveXT->SubFormat != pPartnerWaveXT->SubFormat)
            {
                return DMO_E_TYPE_NOT_ACCEPTED;
            }
        }
    }

    // media type is valid
    return S_OK;
}
