//////////////////////////////////////////////////////////////////////////
//
// winmain.cpp : Defines the entry point for the application.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////


#include "MFT_Grayscale.h"
#include "Grayscale.h"

#include <uuids.h>      // DirectShow GUIDs
#include <assert.h>
#include <evr.h>


// This sample implements a Media Foundation transform (MFT) that 
// converts YUV video frames to grayscale. The conversion is done
// simply by setting all of the U and V bytes to zero (0x80).

// NOTES:
// 1-in, 1-out
// Fixed streams
// Formats: UYVY, YUY2, NV12

// Assumptions:
// 1. If the MFT is holding an input sample, SetInputType and SetOutputType 
//    return MF_E_UNSUPPORTED_MEDIATYPE
// 2. If the input type is set, the output type must match (and vice versa).
// 3. If both types are set, no type can be set until the current type is 
//    cleared.
// 4. Preferred input types: 
//    (a) If the output type is set, that's the preferred type.
//    (b) Otherwise. the preferred types are partial types, constructed from 
//        a list of supported video subtypes. 
// 5. Preferred output types: As above.
 
// Video FOURCC codes.
const FOURCC FOURCC_YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2');
const FOURCC FOURCC_UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y');
const FOURCC FOURCC_NV12 = MAKEFOURCC('N', 'V', '1', '2');


#ifdef _DEBUG

// TraceSubtype: Log the name of a video subtype FOURCC.
// Note: Not guaranteed to produce a readable string, but it's only for debug
// tracing.
void TraceSubtype(const GUID& guid)
{
    DWORD fcc = guid.Data1;

	char str[5];  // Four characters plus NULL

	str[0] = (char)(fcc & 0xff);
	str[1] = (char)((fcc >> 8) & 0xff);
	str[2] = (char)((fcc >> 16) & 0xff);
	str[3] = (char)(fcc >> 24);
	str[4] = 0;

    TRACE((L"Subtype: %S", str));
};

#else

#define TraceSubtype(x)

#endif


// Static array of media types (preferred and accepted).
const GUID* g_MediaSubtypes[] = 
{
	& MEDIASUBTYPE_NV12,
	& MEDIASUBTYPE_YUY2,
	& MEDIASUBTYPE_UYVY
};

// Number of media types in the aray.
DWORD g_cNumSubtypes = ARRAY_SIZE(g_MediaSubtypes);

// GetImageSize: Returns the size of a video frame, in bytes.
HRESULT GetImageSize(FOURCC fcc, UINT32 width, UINT32 height, DWORD* pcbImage);



//-------------------------------------------------------------------
// Name: TransformImage_UYVY
// Description: Converts an image in UYVY format to grayscale.
//
// The image conversion functions take the following parameters:
//
// pDest:            Pointer to the destination buffer.
// lDestStride:      Stride of the destination buffer, in bytes.
// pSrc:             Pointer to the source buffer.
// lSrcStride:       Stride of the source buffer, in bytes.
// dwWidthInPixels:  Frame width in pixels.
// dwHeightInPixels: Frame height, in pixels.
//-------------------------------------------------------------------

void TransformImage_UYVY(
    BYTE*		pDest,
    LONG		lDestStride,
    const BYTE*	pSrc,
    LONG		lSrcStride,
    DWORD		dwWidthInPixels,
    DWORD		dwHeightInPixels
    )
{
	for (DWORD y = 0; y < dwHeightInPixels; y++)
	{
		WORD *pSrc_Pixel = (WORD*)pSrc;
		WORD *pDest_Pixel = (WORD*)pDest;
		
		for (DWORD x = 0; x < dwWidthInPixels; x++)
		{
            // Byte order is U0 Y0 V0 Y1
            // Each WORD is a byte pair (U/V, Y)
            // Windows is little-endian so the order appears reversed.
            
            WORD pixel = pSrc_Pixel[x] & 0xFF00;
			pixel |= 0x0080;
			pDest_Pixel[x] = pixel;
		}
		pDest += lDestStride;
		pSrc += lSrcStride;
	}
}


//-------------------------------------------------------------------
// Name: TransformImage_YUY2
// Description: Converts an image in YUY2 format to grayscale.
//-------------------------------------------------------------------

void TransformImage_YUY2(
    BYTE*		pDest,
    LONG		lDestStride,
    const BYTE*	pSrc,
    LONG		lSrcStride,
    DWORD		dwWidthInPixels,
    DWORD		dwHeightInPixels
    )
{
	for (DWORD y = 0; y < dwHeightInPixels; y++)
	{
		WORD *pSrc_Pixel = (WORD*)pSrc;
		WORD *pDest_Pixel = (WORD*)pDest;
		
		for (DWORD x = 0; x < dwWidthInPixels; x++)
		{
            // Byte order is Y0 U0 Y1 V0 
            // Each WORD is a byte pair (Y, U/V)
            // Windows is little-endian so the order appears reversed.
            
            WORD pixel = pSrc_Pixel[x] & 0x00FF;
			pixel |= 0x8000;
			pDest_Pixel[x] = pixel;
		}
		pDest += lDestStride;
		pSrc += lSrcStride;
	}
}



//-------------------------------------------------------------------
// Name: TransformImage_NV12
// Description: Converts an image in NV12 format to grayscale.
//-------------------------------------------------------------------

void TransformImage_NV12(
    BYTE*		pDest,
    LONG		lDestStride,
    const BYTE*	pSrc,
    LONG		lSrcStride,
    DWORD		dwWidthInPixels,
    DWORD		dwHeightInPixels
    )
{
    // NV12 is planar: Y plane, followed by packed U-V plane.

	// Y plane
	for (DWORD y = 0; y < dwHeightInPixels; y++)
	{
		CopyMemory(pDest, pSrc, dwWidthInPixels);
		pDest += lDestStride;
		pSrc += lSrcStride;
	}

	// U-V plane
	for (DWORD y = 0; y < dwHeightInPixels/2; y++)
	{
		FillMemory(pDest, dwWidthInPixels, 0x80);
		pDest += lDestStride;
	}
}


//-------------------------------------------------------------------
// Name: CreateInstance
// Description: Static method to create an instance of the source.
//
// iid:         IID of the requested interface on the source.
// ppSource:    Receives a ref-counted pointer to the source.
//-------------------------------------------------------------------

HRESULT CGrayscale::CreateInstance(REFIID iid, void **ppMFT)
{
    if (ppMFT == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    CGrayscale *pMFT = new CGrayscale(hr);
    if (pMFT == NULL)
    {
        return E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        hr = pMFT->QueryInterface(iid, ppMFT);
    }

    if (FAILED(hr))
    {
        delete pMFT;
    }
    return hr;
}


//-------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------

CGrayscale::CGrayscale(HRESULT& hr) :
    m_nRefCount(0),     // Ref count starts at zero, CreateInstance calls QI. Returned to caller with ref count = 1.
    m_pSample(NULL),
    m_pInputType(NULL),
    m_pOutputType(NULL),
    m_pTransformFn(NULL),
    m_videoFOURCC(0),
    m_imageWidthInPixels(0),
    m_imageHeightInPixels(0),
    m_cbImageSize(0)
{
}

//-------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------

CGrayscale::~CGrayscale()
{
    assert(m_nRefCount == 0);
}

// IUnknown methods

ULONG CGrayscale::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG CGrayscale::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT CGrayscale::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IMFTransform))
    {
        *ppv = static_cast<IMFTransform*>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// IMFTransform methods. Refer to the Media Foundation SDK documentation for details.

//-------------------------------------------------------------------
// Name: GetStreamLimits
//-------------------------------------------------------------------

HRESULT CGrayscale::GetStreamLimits( 
    DWORD   *pdwInputMinimum,
    DWORD   *pdwInputMaximum,
    DWORD   *pdwOutputMinimum,
    DWORD   *pdwOutputMaximum
)
{
    // Fixed stream limits.
    if (pdwInputMinimum)
    {
        *pdwInputMinimum = 1;
    }
    if (pdwInputMaximum)
    {
        *pdwInputMaximum = 1;
    }
    if (pdwOutputMinimum)
    {
        *pdwOutputMinimum = 1;
    }
    if (pdwOutputMaximum)
    {
        *pdwOutputMaximum = 1;
    }

    return S_OK;
}


//-------------------------------------------------------------------
// Name: GetStreamCount
//-------------------------------------------------------------------

HRESULT CGrayscale::GetStreamCount(
    DWORD   *pcInputStreams,
    DWORD   *pcOutputStreams
)
{
    // Fixed stream count.
    if (pcInputStreams)
    {
        *pcInputStreams = 1;
    }
    if (pcOutputStreams)
    {
        *pcOutputStreams = 1;
    }
    return S_OK;
}



//-------------------------------------------------------------------
// Name: GetStreamIDs
//-------------------------------------------------------------------

HRESULT CGrayscale::GetStreamIDs(
    DWORD	dwInputIDArraySize,
    DWORD   *pdwInputIDs,
    DWORD	dwOutputIDArraySize,
    DWORD	*pdwOutputIDs
)
{
    // Do not need to implement, because this MFT has a fixed number of 
    // streams and the stream IDs match the stream indexes.
    return E_NOTIMPL;   
}


//-------------------------------------------------------------------
// Name: GetInputStreamInfo
//-------------------------------------------------------------------

HRESULT CGrayscale::GetInputStreamInfo(
    DWORD                     dwInputStreamID,
    MFT_INPUT_STREAM_INFO *   pStreamInfo
)
{
	TRACE((L"GetInputStreamInfo"));

	CAutoLock lock(m_critSec);
	
	if (!IsValidInputStream(dwInputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

	if (m_pInputType == NULL)
	{
		return MF_E_TRANSFORM_TYPE_NOT_SET;
	}

	pStreamInfo->hnsMaxLatency = 0;
	pStreamInfo->dwFlags = MFT_INPUT_STREAM_WHOLE_SAMPLES | MFT_INPUT_STREAM_SINGLE_SAMPLE_PER_BUFFER ;
	pStreamInfo->cbSize = m_cbImageSize;
	pStreamInfo->cbMaxLookahead = 0;
	pStreamInfo->cbAlignment = 0;

    return S_OK;
}



//-------------------------------------------------------------------
// Name: GetOutputStreamInfo
//-------------------------------------------------------------------

HRESULT CGrayscale::GetOutputStreamInfo(
    DWORD                     dwOutputStreamID, 
    MFT_OUTPUT_STREAM_INFO *  pStreamInfo      
)
{
	CAutoLock lock(m_critSec);


	if (dwOutputStreamID != 0)
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

	if (m_pOutputType == NULL)
	{
		return MF_E_TRANSFORM_TYPE_NOT_SET;
	}

	pStreamInfo->dwFlags = 
		MFT_OUTPUT_STREAM_WHOLE_SAMPLES | 
		MFT_OUTPUT_STREAM_SINGLE_SAMPLE_PER_BUFFER |
		MFT_OUTPUT_STREAM_FIXED_SAMPLE_SIZE ;
	pStreamInfo->cbSize = m_cbImageSize;
	pStreamInfo->cbAlignment = 0;

    return S_OK;
}



//-------------------------------------------------------------------
// Name: GetAttributes
//-------------------------------------------------------------------

HRESULT CGrayscale::GetAttributes(IMFAttributes** pAttributes)
{
    return E_NOTIMPL;   
}



//-------------------------------------------------------------------
// Name: GetInputStreamAttributes
//-------------------------------------------------------------------

HRESULT CGrayscale::GetInputStreamAttributes(
    DWORD           dwInputStreamID,
    IMFAttributes   **ppAttributes
)
{
    return E_NOTIMPL;
}



//-------------------------------------------------------------------
// Name: GetOutputStreamAttributes
//-------------------------------------------------------------------

HRESULT CGrayscale::GetOutputStreamAttributes(
    DWORD           dwOutputStreamID,
    IMFAttributes   **ppAttributes
)
{
    return E_NOTIMPL;
}



//-------------------------------------------------------------------
// Name: DeleteInputStream
//-------------------------------------------------------------------

HRESULT CGrayscale::DeleteInputStream(DWORD dwStreamID)
{
    return E_NOTIMPL; // Fixed streams.
}



//-------------------------------------------------------------------
// Name: AddInputStreams
//-------------------------------------------------------------------

HRESULT CGrayscale::AddInputStreams( 
    DWORD   cStreams,
    DWORD   *adwStreamIDs
)
{
    return E_NOTIMPL; // Fixed streams.
}



//-------------------------------------------------------------------
// Name: GetInputAvailableType
// Description: Return a preferred input type.
//-------------------------------------------------------------------

HRESULT CGrayscale::GetInputAvailableType(
    DWORD           dwInputStreamID,
    DWORD           dwTypeIndex, // 0-based
    IMFMediaType    **ppType
)
{
	TRACE((L"GetInputAvailableType (stream = %d, type index = %d)", dwInputStreamID, dwTypeIndex));

	CAutoLock lock(m_critSec);

	if (ppType == NULL)
	{
		return E_INVALIDARG;
	}

	if (!IsValidInputStream(dwInputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    HRESULT hr = S_OK;

    // 
	if (this->m_pOutputType)
	{
		*ppType = m_pOutputType;
		(*ppType)->AddRef();
	}
    else
    {
        // Create a partial media type.
	    hr = OnGetPartialType(dwTypeIndex, ppType);
    }
    return hr;
}



//-------------------------------------------------------------------
// Name: GetOutputAvailableType
// Description: Return a preferred output type.
//-------------------------------------------------------------------

HRESULT CGrayscale::GetOutputAvailableType(
    DWORD           dwOutputStreamID,
    DWORD           dwTypeIndex, // 0-based
    IMFMediaType    **ppType
)
{
	TRACE((L"GetOutputAvailableType (stream = %d, type index = %d)", dwOutputStreamID, dwTypeIndex));

	CAutoLock lock(m_critSec);

	if (ppType == NULL)
	{
		return E_INVALIDARG;
	}

	if (!IsValidInputStream(dwOutputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    HRESULT hr = S_OK;

	if (this->m_pInputType)
	{
		*ppType = m_pInputType;
		(*ppType)->AddRef();
	}
    else
    {
	    hr = OnGetPartialType(dwTypeIndex, ppType);
    }

    return hr;
}



//-------------------------------------------------------------------
// Name: SetInputType
//-------------------------------------------------------------------

HRESULT CGrayscale::SetInputType(
    DWORD           dwInputStreamID,
    IMFMediaType    *pType,
    DWORD           dwFlags 
)
{
    TRACE((L"CGrayscale::SetInputType"));

	CAutoLock lock(m_critSec);

	if (!IsValidInputStream(dwInputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    HRESULT hr = S_OK;

    // Does the caller want us to set the type, or just test it?
    BOOL bReallySet = ((dwFlags & MFT_SET_TYPE_TEST_ONLY) == 0);

    // If we have an input sample, the client cannot change the type now.
    if (HasPendingOutput())
    {
        hr = MF_E_INVALIDMEDIATYPE;
    }

    // Validate the type.
    if (SUCCEEDED(hr))
    {
        hr = OnCheckInputType(pType);
    }

    if (SUCCEEDED(hr))
    {
        // The type is OK. 
        // Set the type, unless the caller was just testing.
        if (bReallySet)
        {
            hr = OnSetInputType(pType);
        }
    }
            

    return hr;
}



//-------------------------------------------------------------------
// Name: SetOutputType
//-------------------------------------------------------------------

HRESULT CGrayscale::SetOutputType(
    DWORD           dwOutputStreamID,
    IMFMediaType    *pType,
    DWORD           dwFlags 
)
{

    TRACE((L"CGrayscale::SetOutputType"));

    CAutoLock lock(m_critSec);

    if (!IsValidOutputStream(dwOutputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    HRESULT hr = S_OK;


    // Does the caller want us to set the type, or just test it?
     BOOL bReallySet = ((dwFlags & MFT_SET_TYPE_TEST_ONLY) == 0);

    // If we have an input sample, the client cannot change the type now.
    if (HasPendingOutput())
    {
        hr = MF_E_INVALIDMEDIATYPE;
    }

    // Validate the type.
    if (SUCCEEDED(hr))
    {
        hr = OnCheckOutputType(pType);
    }

    if (SUCCEEDED(hr))
    {
        if (bReallySet)
        {
            // The type is OK. 
            // Set the type, unless the caller was just testing.
            hr = OnSetOutputType(pType);
        }
    }

    return hr;
}



//-------------------------------------------------------------------
// Name: GetInputCurrentType
// Description: Returns the current input type.
//-------------------------------------------------------------------

HRESULT CGrayscale::GetInputCurrentType(
    DWORD           dwInputStreamID,
    IMFMediaType    **ppType
)
{
	CAutoLock lock(m_critSec);

    if (ppType == NULL)
    {
        return E_POINTER;
    }

    if (!IsValidInputStream(dwInputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    if (!m_pInputType)
    {
        return MF_E_TRANSFORM_TYPE_NOT_SET;
    }

    *ppType = m_pInputType;
    (*ppType)->AddRef();

    return S_OK;

}



//-------------------------------------------------------------------
// Name: GetOutputCurrentType
// Description: Returns the current output type.
//-------------------------------------------------------------------

HRESULT CGrayscale::GetOutputCurrentType(
    DWORD           dwOutputStreamID,
    IMFMediaType    **ppType
)
{
	CAutoLock lock(m_critSec);

    if (ppType == NULL)
    {
        return E_POINTER;
    }

    if (!IsValidOutputStream(dwOutputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    if (!m_pOutputType)
    {
        return MF_E_TRANSFORM_TYPE_NOT_SET;
    }

    *ppType = m_pOutputType;
    (*ppType)->AddRef();

    return S_OK;

}



//-------------------------------------------------------------------
// Name: GetInputStatus
// Description: Query if the MFT is accepting more input.
//-------------------------------------------------------------------

HRESULT CGrayscale::GetInputStatus(
    DWORD			dwInputStreamID,
    DWORD           *pdwFlags 
)
{
	TRACE((L"GetInputStatus"));

	CAutoLock lock(m_critSec);

    if (pdwFlags == NULL)
    {
        return E_POINTER;
    }

    if (!IsValidOutputStream(dwInputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    // If we already have an input sample, we don't accept
    // another one until the client calls ProcessOutput or Flush.
    if (m_pSample == NULL)
    {
        *pdwFlags = MFT_INPUT_STATUS_ACCEPT_DATA;
    }
    else
    {
        *pdwFlags = 0;
    }

    return S_OK;
}



//-------------------------------------------------------------------
// Name: GetOutputStatus
// Description: Query if the MFT can produce output.
//-------------------------------------------------------------------

HRESULT CGrayscale::GetOutputStatus(DWORD *pdwFlags)
{
	TRACE((L"GetOutputStatus"));

	CAutoLock lock(m_critSec);

    if (pdwFlags == NULL)
    {
        return E_POINTER;
    }

    // We can produce an output sample if (and only if)
    // we have an input sample.
    if (m_pSample != NULL)
    {
        *pdwFlags = MFT_OUTPUT_STATUS_SAMPLE_READY;
    }
    else
    {
        *pdwFlags = 0;
    }

    return S_OK;
}



//-------------------------------------------------------------------
// Name: SetOutputBounds
//-------------------------------------------------------------------

HRESULT CGrayscale::SetOutputBounds(
    LONGLONG        hnsLowerBound,
    LONGLONG        hnsUpperBound
)
{
    return E_NOTIMPL;
}



//-------------------------------------------------------------------
// Name: ProcessEvent
//-------------------------------------------------------------------

HRESULT CGrayscale::ProcessEvent(
    DWORD              dwInputStreamID,
    IMFMediaEvent      *pEvent 
)
{
    return E_NOTIMPL;
}



//-------------------------------------------------------------------
// Name: ProcessMessage
//-------------------------------------------------------------------

HRESULT CGrayscale::ProcessMessage(
    MFT_MESSAGE_TYPE	eMessage,
    ULONG_PTR			ulParam
)
{
	CAutoLock lock(m_critSec);

    HRESULT hr = S_OK;

    switch (eMessage)
    {
    case MFT_MESSAGE_COMMAND_FLUSH:
        // Flush the MFT.
        hr = OnFlush();
        break;

    // The remaining messages do not require any action from this MFT.

    case MFT_MESSAGE_COMMAND_DRAIN:
        // Drain: Tells the MFT not to accept any more input until 
        // all of the pending output has been processed. That is our 
        // default behevior already, so there is nothing to do.

    case MFT_MESSAGE_SET_D3D_MANAGER:
    case MFT_MESSAGE_NOTIFY_BEGIN_STREAMING:
    case MFT_MESSAGE_NOTIFY_END_STREAMING:
    case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
    case MFT_MESSAGE_NOTIFY_START_OF_STREAM: 
        break;
    }

    return hr;
}
    


//-------------------------------------------------------------------
// Name: ProcessInput
// Description: Process an input sample.
//-------------------------------------------------------------------

HRESULT CGrayscale::ProcessInput(
    DWORD				dwInputStreamID,
    IMFSample           *pSample, 
    DWORD				dwFlags 
)
{
	CAutoLock lock(m_critSec);

    if (pSample == NULL)
    {
        return E_POINTER;
    }

    if (!IsValidOutputStream(dwInputStreamID))
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    if (dwFlags != 0)
    {
        return E_INVALIDARG;
    }

    if (!m_pInputType || !m_pOutputType)
    {
        return MF_E_NOTACCEPTING;   // Client must set input and output types.
    }

    if (m_pSample != NULL)
    {
        return MF_E_NOTACCEPTING;   // We already have an input sample.
    }

    // Cache the sample. We do the actuual work in ProcessOutput.
    m_pSample = pSample;
    pSample->AddRef();  // Hold a reference count on the sample.

    return S_OK;
}



//-------------------------------------------------------------------
// Name: ProcessOutput
// Description: Process an output sample.
//-------------------------------------------------------------------

HRESULT CGrayscale::ProcessOutput(
    DWORD					dwFlags, 
    DWORD					cOutputBufferCount,
    MFT_OUTPUT_DATA_BUFFER  *pOutputSamples, // one per stream
    DWORD                   *pdwStatus  
)
{
	CAutoLock lock(m_critSec);

    // Check input parameters...

    // There are no flags that we accept in this MFT.
    // The only defined flag is MFT_PROCESS_OUTPUT_DISCARD_WHEN_NO_BUFFER. This 
    // flag only applies when the MFT marks an output stream as lazy or optional.
    // However there are no lazy or optional streams on this MFT, so the flag is
    // not valid.
    if (dwFlags != 0)
    {
        return E_INVALIDARG;
    }

    if (pOutputSamples == NULL || pdwStatus == NULL)
    {
        return E_POINTER;
    }

    // Must be exacly one output buffer.
    if (cOutputBufferCount != 1)
    {
        return E_INVALIDARG;
    }

    // It must contain a sample.
    if (pOutputSamples[0].pSample == NULL)
    {
        return E_INVALIDARG;
    }

    // If we don't have an input sample, we need some input before
    // we can generate any output.
    if (m_pSample == NULL)
    {
        return MF_E_TRANSFORM_NEED_MORE_INPUT;
    }

    HRESULT hr = S_OK;

    IMFMediaBuffer *pInput = NULL;
    IMFMediaBuffer *pOutput = NULL;

    // Get the input buffer.
    hr = m_pSample->GetBufferByIndex(0, &pInput);

    // Get the output buffer.
    if (SUCCEEDED(hr))
    {
        hr = pOutputSamples[0].pSample->GetBufferByIndex(0, &pOutput);
    }

    if (SUCCEEDED(hr))
    {
        hr = OnProcessOutput(pInput, pOutput);
    }

    // Set status flags.
    if (SUCCEEDED(hr))
    {
        pOutputSamples[0].dwStatus = 0; 
        *pdwStatus = 0;
    }


    // Copy the duration and time stamp from the input sample,
    // if present.
     
	LONGLONG hnsDuration = 0;
	LONGLONG hnsTime = 0;

    if (SUCCEEDED(hr))
    {
    	if (SUCCEEDED(m_pSample->GetSampleDuration(&hnsDuration)))
        {
        	hr = pOutputSamples[0].pSample->SetSampleDuration(hnsDuration);
        }
    }

    if (SUCCEEDED(hr))
    {
        if (SUCCEEDED(m_pSample->GetSampleTime(&hnsTime)))
        {
    	    hr = pOutputSamples[0].pSample->SetSampleTime(hnsTime);
        }
    }

   
	// Release our input sample.
	if (SUCCEEDED(hr))
	{
		SAFE_RELEASE(m_pSample);
	}

    SAFE_RELEASE(pInput);
    SAFE_RELEASE(pOutput);

    return hr;
}



/// PRIVATE METHODS

//-------------------------------------------------------------------
// Name: OnGetPartialType
// Description: Returns a partial media type from our list.
//
// dwTypeIndex: Index into the list of peferred media types.
// ppmt: Receives a pointer to the media type.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnGetPartialType(DWORD dwTypeIndex, IMFMediaType **ppmt)
{
	HRESULT hr = S_OK;

	if (dwTypeIndex >= g_cNumSubtypes)
	{
		return MF_E_NO_MORE_TYPES;
	}

	IMFMediaType *pmt;

	hr = MFCreateMediaType(&pmt);

	if (SUCCEEDED(hr))
	{
		hr = pmt->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	}

	if (SUCCEEDED(hr))
	{
		hr = pmt->SetGUID(MF_MT_SUBTYPE, *g_MediaSubtypes[dwTypeIndex]);
	}

	if (SUCCEEDED(hr))
	{
		*ppmt = pmt;
		(*ppmt)->AddRef();
	}

	SAFE_RELEASE(pmt);

	return hr;
}





//-------------------------------------------------------------------
// Name: OnCheckInputType
// Description: Validate an input media type.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnCheckInputType(IMFMediaType *pmt)
{
	TRACE((L"OnCheckInputType"));
	assert(pmt != NULL);

	HRESULT hr = S_OK;

	// If the output type is set, see if they match.
	if (m_pOutputType != NULL)
	{
		DWORD flags = 0;
		hr = pmt->IsEqual(m_pOutputType, &flags);

		// IsEqual can return S_FALSE. Treat this as failure.

		if (hr != S_OK)
		{
			hr = MF_E_INVALIDTYPE;
		}
	}
	else
	{
        // Output type is not set. Just check this type.
		hr = OnCheckMediaType(pmt);
	}

    return hr;
}




//-------------------------------------------------------------------
// Name: OnCheckOutputType
// Description: Validate an output media type.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnCheckOutputType(IMFMediaType *pmt)
{
	TRACE((L"OnCheckOutputType"));
	assert(pmt != NULL);

	HRESULT hr = S_OK;

	// If the input type is set, see if they match.
	if (m_pInputType != NULL)
	{
		DWORD flags = 0;
		hr = pmt->IsEqual(m_pInputType, &flags);

		// IsEqual can return S_FALSE. Treat this as failure.

		if (hr != S_OK)
		{
			hr = MF_E_INVALIDTYPE;
		}

	}
	else
	{
        // Input type is not set. Just check this type.
		hr = OnCheckMediaType(pmt);
	}

    return hr;    
}



//-------------------------------------------------------------------
// Name: OnCheckMediaType
// Description: Validates a media type for this transform.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnCheckMediaType(IMFMediaType *pmt)
{
	GUID major_type = GUID_NULL;
	GUID subtype = GUID_NULL;
	MFVideoInterlaceMode interlace = MFVideoInterlace_Unknown;
    UINT32 val = 0;

	HRESULT hr = S_OK;

	// Major type must be video.
	hr = pmt->GetGUID(MF_MT_MAJOR_TYPE, &major_type);
	if (SUCCEEDED(hr))
	{
		if (major_type != MFMediaType_Video)
		{
			hr = MF_E_INVALIDTYPE;
		}
	}

	// Subtype must be one of the subtypes in our global list.
	if (SUCCEEDED(hr))
	{
        // Get the subtype GUID.
		hr = pmt->GetGUID(MF_MT_SUBTYPE, &subtype);
	}
	if (SUCCEEDED(hr))
	{
	    TraceSubtype(subtype);

        // Look for the subtype in our list of accepted types.
		BOOL bFoundMatchingSubtype = FALSE;
		for (DWORD i = 0; i < g_cNumSubtypes; i++)
		{
			if (subtype == *g_MediaSubtypes[i])
			{
				bFoundMatchingSubtype = TRUE;
				break;
			}
		}

		if (!bFoundMatchingSubtype)
		{
			hr = MF_E_INVALIDTYPE;
		}		
	}

	// Video must be progressive frames.
	if (SUCCEEDED(hr))
	{
		hr = pmt->GetUINT32(MF_MT_INTERLACE_MODE, (UINT32*)&interlace);
	}
	if (SUCCEEDED(hr))
	{
		if (interlace != MFVideoInterlace_Progressive)
		{
			hr = MF_E_INVALIDTYPE;
		}
	}

	return hr;
}



//-------------------------------------------------------------------
// Name: OnSetInputType
// Description: Sets the input media type.
//
// Prerequisite:
// The input type has already been validated.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnSetInputType(IMFMediaType *pmt)
{
    TRACE((L"CGrayscale::OnSetInputType"));

    // Set the type.
    
    SAFE_RELEASE(m_pInputType);
    m_pInputType = pmt;
    if (m_pInputType)
    {
        m_pInputType->AddRef();
    }

    // Update the format information.
	UpdateFormatInfo();

    return S_OK;
}




//-------------------------------------------------------------------
// Name: OnSetOutputType
// Description: Sets the output media type.
//
// Prerequisite:
// The output type has already been validated.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnSetOutputType(IMFMediaType *pmt)
{
    TRACE((L"CGrayscale::OnSetOutputType"));
	SAFE_RELEASE(m_pOutputType);

    m_pOutputType = pmt;
    if (m_pOutputType)
    {
        m_pOutputType->AddRef();
    }

    return S_OK;
  
}




//-------------------------------------------------------------------
// Name: OnProcessOutput
// Description: Generates output data.
//-------------------------------------------------------------------


HRESULT CGrayscale::OnProcessOutput(IMFMediaBuffer *pIn, IMFMediaBuffer *pOut)
{
	DWORD cb;
	HRESULT hr = S_OK;

	BYTE *pDest = NULL;			// Destination buffer.
	LONG lDestStride = 0;		// Destination stride.

	BYTE *pSrc = NULL;			// Source buffer.
	LONG lSrcStride = 0;		// Source stride.

	BOOL bLockedInputBuffer = FALSE;
	BOOL bLockedOutputBuffer = FALSE;

	// Stride if the buffer does not support IMF2DBuffer
	LONG lStrideIfContiguous;
    
    hr = m_pInputType->GetUINT32(MF_MT_DEFAULT_STRIDE, (UINT32*)&lStrideIfContiguous);

	IMF2DBuffer *pOut2D = NULL;
	IMF2DBuffer *pIn2D = NULL;

	// Lock the output buffer. Use IMF2DBuffer if available.
	hr = pOut->QueryInterface(IID_IMF2DBuffer, (void**)&pOut2D);
	if (SUCCEEDED(hr))
	{
        TRACE((L"output buffer: 2D"));
		hr = pOut2D->Lock2D(&pDest, &lDestStride);
	}
	else if (hr == E_NOINTERFACE)
	{
		hr = pOut->Lock(&pDest, NULL, &cb);
		lDestStride = lStrideIfContiguous;
	}
	if (SUCCEEDED(hr))
	{
		bLockedOutputBuffer = TRUE;
	}

	// Lock the input buffer. Use IMF2DBuffer if available.
	hr = pIn->QueryInterface(IID_IMF2DBuffer, (void**)&pIn2D);
	if (SUCCEEDED(hr))
	{
        TRACE((L"input buffer: 2D"));
		hr = pIn2D->Lock2D(&pSrc, &lSrcStride);
	}
	else if (hr == E_NOINTERFACE)
	{
		hr = pIn->Lock(&pSrc, NULL, &cb);
		lSrcStride = lStrideIfContiguous;

		assert (cb == m_cbImageSize); 

	}
	if (SUCCEEDED(hr))
	{
		bLockedInputBuffer = TRUE;
	}


    // Invoke the image transform function.
	if (SUCCEEDED(hr))
	{
        assert (m_pTransformFn != NULL); 
        if (m_pTransformFn)
        {
            (*m_pTransformFn)( pDest, lDestStride, pSrc, lSrcStride, 
				m_imageWidthInPixels, m_imageHeightInPixels);
        }
        else
        {
            hr = E_UNEXPECTED;
        }
	}

    // Unlock the buffers.

	if (bLockedOutputBuffer)
	{
		if (pOut2D != NULL)
		{
			pOut2D->Unlock2D();
		}
		else
		{
			pOut->Unlock();
		}
	}

	if (bLockedInputBuffer)
	{
		if (pIn2D != NULL)
		{
			pIn2D->Unlock2D();
		}
		else
		{
			pIn->Unlock();
		}
	}


    // Set the data size on the output buffer.
	if (SUCCEEDED(hr))
	{
		pOut->SetCurrentLength(m_cbImageSize);
	}

	SAFE_RELEASE(pOut2D);
	SAFE_RELEASE(pIn2D);

    return S_OK;
}




//-------------------------------------------------------------------
// Name: OnFlush
// Description: Flush the MFT.
//-------------------------------------------------------------------

HRESULT CGrayscale::OnFlush()
{
    // For this MFT, flushing just means releasing the input sample.
    SAFE_RELEASE(m_pSample);
    return S_OK;
}




//-------------------------------------------------------------------
// Name: UpdateFormatInfo
// Description: After the input type is set, update our format 
//              information.
//-------------------------------------------------------------------

HRESULT CGrayscale::UpdateFormatInfo()
{
	HRESULT hr = S_OK;

	GUID subtype = GUID_NULL;

	m_imageWidthInPixels = 0;
	m_imageHeightInPixels = 0;
	m_videoFOURCC = 0;
	m_cbImageSize = 0;

    m_pTransformFn = NULL;

	if (m_pInputType != NULL)
	{
		hr = m_pInputType->GetGUID(MF_MT_SUBTYPE, &subtype);

		if (SUCCEEDED(hr))
		{
            TraceSubtype(subtype);
            m_videoFOURCC = subtype.Data1;

            switch (m_videoFOURCC)
            {
            case FOURCC_YUY2:
                m_pTransformFn = TransformImage_YUY2;
                break;

            case FOURCC_UYVY:
                m_pTransformFn = TransformImage_UYVY;
                break;

            case FOURCC_NV12:
                m_pTransformFn = TransformImage_NV12;
                break;

            default:
                hr = E_UNEXPECTED;
            }
        }

		if (SUCCEEDED(hr))
		{
			hr = MFGetAttributeSize(
					m_pInputType, 
					MF_MT_FRAME_SIZE, 
					&m_imageWidthInPixels, 
					&m_imageHeightInPixels
					);
		}

		if (SUCCEEDED(hr))
		{
            TRACE((L"Frame size: %d x %d", m_imageWidthInPixels, m_imageHeightInPixels));

            // Calculate the image size (not including padding)
            hr = GetImageSize(m_videoFOURCC, m_imageWidthInPixels, m_imageHeightInPixels, &m_cbImageSize);
		}
	}

	return hr;
}


//-------------------------------------------------------------------
// Name: GetImageSize
// Description: 
// Calculates the buffer size needed, based on the video format.
//-------------------------------------------------------------------

HRESULT GetImageSize(FOURCC fcc, UINT32 width, UINT32 height, DWORD* pcbImage)
{
    HRESULT hr = S_OK;

    switch (fcc)
    {
    case FOURCC_YUY2:
    case FOURCC_UYVY:
        // check overflow
        if ((width > MAXDWORD / 2) ||
            (width * 2 > MAXDWORD / height))
        {
            hr = E_INVALIDARG;
        }
        else
        {
            // 16 bpp
            *pcbImage = width * height * 2;
        }
        break;
        

	case FOURCC_NV12:
        // check overflow
        if ((height/2 > MAXDWORD - height) ||
            ((height + height/2) > MAXDWORD / width))
        {
            hr = E_INVALIDARG;
        }
        else
        {
            // 12 bpp
    		*pcbImage = width * (height + (height/2));
        }
		break;

    default:
        hr = E_FAIL;    // Unsupported type.
    }

    return hr;
}