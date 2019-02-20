//////////////////////////////////////////////////////////////////////////
// DSUtil.cpp: DirectShow helper functions.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

/*********************************************************************************
/
/  DSUtil.cpp 
/  DirectShow helper functions.
/
/  Copyright (C) Microsoft Corporation. All rights reserved.
/
/**********************************************************************************/

#include <dshow.h>
#include "DshowUtil.h"

#include <strsafe.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#endif

/******* Helper functions not declared in DShowUtil.h *******/

// MatchPinName: 
// Function object to match a pin by name.
struct MatchPinName
{
    const WCHAR *m_wszName;

    MatchPinName(const WCHAR *wszName)
    {
        m_wszName = wszName;
    }

    HRESULT operator()(IPin *pPin, BOOL *pResult)
    {
        assert(pResult != NULL);

        PIN_INFO PinInfo;
        HRESULT hr = pPin->QueryPinInfo(&PinInfo);
        if (SUCCEEDED(hr))
        {
            PinInfo.pFilter->Release();

            // TODO: Use Strsafe
            if (wcscmp(m_wszName, PinInfo.achName) == 0)
            {
                *pResult = TRUE;
            }
            else
            {
                *pResult = FALSE;
            }
        }
        return hr;
    }
};

// MatchPinDirectionAndConnection
// Function object to match a pin by direction and connection

struct MatchPinDirectionAndConnection
{
    BOOL            m_bShouldBeConnected;
    PIN_DIRECTION   m_direction;


    MatchPinDirectionAndConnection(PIN_DIRECTION direction, BOOL bShouldBeConnected) 
        : m_bShouldBeConnected(bShouldBeConnected), m_direction(direction)
    {
    }

    HRESULT operator()(IPin *pPin, BOOL *pResult)
    {
        assert(pResult != NULL);

        BOOL bMatch = FALSE;
        BOOL bIsConnected = FALSE;

        HRESULT hr = IsPinConnected(pPin, &bIsConnected);
        if (SUCCEEDED(hr))
        {
            if (bIsConnected == m_bShouldBeConnected)
            {
                hr = IsPinDirection(pPin, m_direction, &bMatch);
            }
        }

        if (SUCCEEDED(hr))
        {
            *pResult = bMatch;
        }
        return hr;
    }
};


struct MatchPinDirectionAndCategory
{
	const GUID*		m_pCategory;
    PIN_DIRECTION   m_direction;

	MatchPinDirectionAndCategory(PIN_DIRECTION direction, REFGUID guidCategory)
		: m_direction(direction), m_pCategory(&guidCategory)
	{
	}

    HRESULT operator()(IPin *pPin, BOOL *pResult)
    {
        assert(pResult != NULL);

        BOOL bMatch = FALSE;
		GUID category;

        HRESULT hr = IsPinDirection(pPin, m_direction, &bMatch);
			
        if (SUCCEEDED(hr) && bMatch)
        {
			hr = GetPinCategory(pPin, &category);
			if (SUCCEEDED(hr))
			{
				bMatch = (category == *m_pCategory);
			}
        }

        if (SUCCEEDED(hr))
        {
            *pResult = bMatch;
        }
        return hr;
    }
};


/******* Functions declared in DSUtil.h *******/




HRESULT AddFilterByCLSID(
    IGraphBuilder *pGraph,  // Pointer to the Filter Graph Manager.
    const GUID& clsid,      // CLSID of the filter to create.
    IBaseFilter **ppF,      // Receives a pointer to the filter.
    LPCWSTR wszName)        // A name for the filter.
{
    if (!pGraph || ! ppF) 
    {
        return E_POINTER;
    }

    *ppF = 0;

    IBaseFilter *pFilter = NULL;
    HRESULT hr = CoCreateInstance(
        clsid, 
        NULL, 
        CLSCTX_INPROC_SERVER, 
        IID_IBaseFilter, 
        (void**)&pFilter);

    if (SUCCEEDED(hr))
    {
        hr = pGraph->AddFilter(pFilter, wszName);
    }

    if (SUCCEEDED(hr))
    {
        *ppF = pFilter;
        (*ppF)->AddRef();
    }

    SAFE_RELEASE(pFilter);

    return hr;
}


HRESULT AddFilterFromMoniker(
    IGraphBuilder *pGraph,          // Pointer to the Filter Graph Manager.
	IMoniker *pFilterMoniker,		// Pointer to the moniker.
    IBaseFilter **ppF,              // Receives a pointer to the filter.
    LPCWSTR wszName		            // A name for the filter (can be NULL).
	)
{
	if (!pGraph || !pFilterMoniker || !ppF)
	{
		return E_POINTER;
	}

	IBaseFilter *pFilter = NULL;

	// Use the moniker to create the filter
    HRESULT hr = pFilterMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pFilter);   
    
	// Add the capture filter to the filter graph
	if (SUCCEEDED(hr))
	{
		hr = pGraph->AddFilter(pFilter, wszName);
	}

	// Return to the caller.
	if (SUCCEEDED(hr))
	{
		*ppF = pFilter;
		(*ppF)->AddRef();
	}

	SAFE_RELEASE(pFilter);

	return hr;
}



// ConnectFilters: Filter to pin

HRESULT ConnectFilters(
    IGraphBuilder *pGraph, // Filter Graph Manager.
    IPin *pOut,            // Output pin on the upstream filter.
    IBaseFilter *pDest)    // Downstream filter.
{
    if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
    {
        return E_POINTER;
    }

    // Find an input pin on the downstream filter.
    IPin *pIn = NULL;
    HRESULT hr = FindUnconnectedPin(pDest, PINDIR_INPUT, &pIn);

	// Try to connect them.
	if (SUCCEEDED(hr))
    {
        hr = pGraph->Connect(pOut, pIn);
	}

	SAFE_RELEASE(pIn);

	return hr;
}


// ConnectFilters: Filter to filter

HRESULT ConnectFilters(
    IGraphBuilder *pGraph, 
    IBaseFilter *pSrc, 
    IBaseFilter *pDest)
{
    if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
    {
        return E_POINTER;
    }

    // Find an output pin on the first filter.
    IPin *pOut = NULL;
    HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (SUCCEEDED(hr))
    {
        hr = ConnectFilters(pGraph, pOut, pDest);
    }


    SAFE_RELEASE(pOut);

    return hr;
}

// ConnectFilters: Filter to pin

HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IPin *pIn)
{
    if ((pGraph == NULL) || (pSrc == NULL) || (pIn == NULL))
    {
        return E_POINTER;
    }

    // Find an output pin on the upstream filter.
    IPin *pOut = NULL;
    HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (SUCCEEDED(hr))
    {
        // Try to connect them.
        hr = pGraph->Connect(pOut, pIn);
    }

	SAFE_RELEASE(pOut);

    return hr;
}


HRESULT CopyFormatBlock(AM_MEDIA_TYPE *pmt, const BYTE *pFormat, DWORD cbSize)
{
    if (pmt == NULL)
    {
        return E_POINTER;
    }

    if (cbSize == 0)
    {
        FreeMediaType(*pmt);
        return S_OK;
    }

    if (pFormat == NULL)
    {
        return E_INVALIDARG;
    }

    // reallocate the format block.
    pmt->pbFormat = (BYTE*)CoTaskMemRealloc(pmt->pbFormat, cbSize);  // OK if pmt->pbFormat is NULL on input.
    if (pmt->pbFormat == NULL)
    {
        return E_OUTOFMEMORY;
    }

    pmt->cbFormat = cbSize;
    CopyMemory(pmt->pbFormat, pFormat, cbSize);
    return S_OK;
}





HRESULT CreateKernelFilter(
    const GUID &guidCategory,  // Filter category.
    LPCOLESTR szName,          // The name of the filter.
    IBaseFilter **ppFilter     // Receives a pointer to the filter.
)
{
    HRESULT hr;

    if (!szName || !ppFilter) 
    {
        return E_POINTER;
    }

    ICreateDevEnum *pDevEnum = NULL;
    IEnumMoniker *pEnum = NULL;

    // Create the system device enumerator.
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, 
        NULL, 
        CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum,
        (void**)&pDevEnum);

    if (SUCCEEDED(hr))
    {
        // Create a class enumerator for the specified category.
        hr = pDevEnum->CreateClassEnumerator(guidCategory, &pEnum, 0);
        if (hr != S_OK) // S_FALSE means the category is empty.
        {
            hr = E_FAIL;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Enumerate devices within this category.
        bool bFound = false;
        IMoniker *pMoniker = NULL;
        while (!bFound && (S_OK == pEnum->Next(1, &pMoniker, 0)))
        {
            // Get the property bag
            IPropertyBag *pBag = NULL;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
            if (FAILED(hr))
            {
                pMoniker->Release();
                continue; // Maybe the next one will work.
            }

            // Check the friendly name.
            VARIANT var;
            VariantInit(&var);
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if (SUCCEEDED(hr) && (lstrcmpiW(var.bstrVal, szName) == 0))
            {
                // This is the right filter.
                hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter,
                    (void**)ppFilter);
                bFound = true;
            }
            VariantClear(&var);
            pBag->Release();
            pMoniker->Release();
        } // while
        if (!bFound) 
        {
            hr = E_FAIL;
        }
    }

    SAFE_RELEASE(pDevEnum);
    SAFE_RELEASE(pEnum);

    return hr;
}


HRESULT CreatePCMAudioType(AM_MEDIA_TYPE& mt, WORD nChannels, 
    DWORD nSamplesPerSec, WORD wBitsPerSample)
{
    FreeMediaType(mt);


    mt.pbFormat = (BYTE*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
    if (!mt.pbFormat)
    {
        return E_OUTOFMEMORY;
    }
    mt.cbFormat = sizeof(WAVEFORMATEX);

    mt.majortype = MEDIATYPE_Audio;
    mt.subtype = MEDIASUBTYPE_PCM;
    mt.formattype = FORMAT_WaveFormatEx;

    WAVEFORMATEX *pWav = (WAVEFORMATEX*)mt.pbFormat;
    pWav->wFormatTag = WAVE_FORMAT_PCM;
    pWav->nChannels  = nChannels;
    pWav->nSamplesPerSec = nSamplesPerSec;
    pWav->wBitsPerSample = wBitsPerSample;
    pWav->cbSize = 0;

    // Derived values
    pWav->nBlockAlign = nChannels * (wBitsPerSample  / 8);
    pWav->nAvgBytesPerSec = nSamplesPerSec * pWav->nBlockAlign;

    return S_OK;
}




//----------------------------------------------------------------------------
// CreateRGBVideoType
//
// Create a media type for an uncompressed RGB format
//
// pMediaType: The method fills this with the media type
// iBitDepth:  Bits per pixel. Must be 1, 4, 8, 16, 24, or 32
// Width:      Width in pixels
// Height:     Height in pixels. Use > 0 for bottom-up DIBs, < 0 for top-down DIB
// fps:        Frame rate, in frames per second
// 
//-----------------------------------------------------------------------------

HRESULT CreateRGBVideoType(AM_MEDIA_TYPE &mt, WORD iBitDepth, long Width, long Height, 
                           double fps)
{
    DWORD color_mask_565[] = { 0x00F800, 0x0007E0, 0x00001F };


    if ((iBitDepth != 1) && (iBitDepth != 4) && (iBitDepth != 8) &&
        (iBitDepth != 16) && (iBitDepth != 24) && (iBitDepth != 32))
    {
        return E_INVALIDARG;
    }

    if (Width < 0)
    {
        return E_INVALIDARG;
    }

    FreeMediaType(mt);


    mt.pbFormat = (BYTE*)CoTaskMemAlloc(sizeof(VIDEOINFO));
    if (!mt.pbFormat)
    {
        return E_OUTOFMEMORY;
    }
    mt.cbFormat = sizeof(VIDEOINFO);


    VIDEOINFO *pvi = (VIDEOINFO*)mt.pbFormat;
    ZeroMemory(pvi, sizeof(VIDEOINFO));

    pvi->AvgTimePerFrame = FramesPerSecToFrameLength(fps);

    BITMAPINFOHEADER *pBmi = &(pvi->bmiHeader);
    pBmi->biSize = sizeof(BITMAPINFOHEADER);
    pBmi->biWidth = Width;
    pBmi->biHeight = Height;
    pBmi->biPlanes = 1;
    pBmi->biBitCount = iBitDepth;

    if (iBitDepth == 16)
    {
        pBmi->biCompression = BI_BITFIELDS;
        memcpy(pvi->dwBitMasks, color_mask_565, sizeof(DWORD) * 3);
    }
    else
    {
        pBmi->biCompression = BI_RGB;
    }

    if (iBitDepth <= 8)
    {
        // Palettized format.
        pBmi->biClrUsed = PALETTE_ENTRIES(pvi);

        HDC hdc = GetDC(NULL);  // hdc for the current display.
        if (hdc == NULL)
        {
            FreeMediaType(mt);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        GetSystemPaletteEntries(hdc, 0, pBmi->biClrUsed, (PALETTEENTRY*)pvi->bmiColors);

        ReleaseDC(NULL, hdc);
    }

    pvi->bmiHeader.biSizeImage = DIBSIZE(pvi->bmiHeader);

    mt.majortype == MEDIATYPE_Video;
    mt.subtype = FORMAT_VideoInfo;

    switch (iBitDepth)
    {
    case 1:
        mt.subtype = MEDIASUBTYPE_RGB1;
        break;
    case 4:
        mt.subtype = MEDIASUBTYPE_RGB4;
        break;
    case 8:
        mt.subtype = MEDIASUBTYPE_RGB8;
        break;
    case 16:
        mt.subtype = MEDIASUBTYPE_RGB565;
        break;
    case 24:
        mt.subtype = MEDIASUBTYPE_RGB24;
        break;
    case 32:
        mt.subtype = MEDIASUBTYPE_RGB32;
    }


    mt.lSampleSize = pvi->bmiHeader.biSizeImage;
    mt.bTemporalCompression = FALSE;
    mt.bFixedSizeSamples = TRUE;
    return S_OK;
}



HRESULT DisconnectPin(IGraphBuilder *pGraph, IPin *pPin)
{
    if (!pGraph || !pPin)
    {
        return E_POINTER;
    }

    IPin *pPinTo = NULL;
    HRESULT hr = pPin->ConnectedTo(&pPinTo);
    if (hr == VFW_E_NOT_CONNECTED)
    {
        // This pin is not connected.
        return S_OK; // no-op
    }

    // Disconnect the first pin.

	if (SUCCEEDED(hr))
	{
	    hr = pGraph->Disconnect(pPin);
	}
    if (SUCCEEDED(hr))
    {
        // Disconnect the other pin.
        pGraph->Disconnect(pPinTo);
    }

    SAFE_RELEASE(pPinTo);

	return hr;
}



HRESULT FindConnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin            // Receives a pointer to the pin.
    )
{
    return FindMatchingPin(pFilter, MatchPinDirectionAndConnection(PinDir, TRUE), ppPin);
}

HRESULT FindPinByCategory(
	IBaseFilter *pFilter, 
	REFGUID guidCategory,
	PIN_DIRECTION PinDir,
	IPin **ppPin
	)
{
	return FindMatchingPin(pFilter, MatchPinDirectionAndCategory(PinDir, guidCategory), ppPin);
}




HRESULT FindFilterInterface(
    IGraphBuilder *pGraph, // Pointer to the Filter Graph Manager.
    REFGUID iid,           // IID of the interface to retrieve.
    void **ppUnk)          // Receives the interface pointer.
{
    if (!pGraph || !ppUnk) 
    {
        return E_POINTER;
    }

    bool bFound = false;

    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter = NULL;

    HRESULT hr = pGraph->EnumFilters(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    // Query every filter for the interface.
    while (S_OK == pEnum->Next(1, &pFilter, 0))
    {
        hr = pFilter->QueryInterface(iid, ppUnk);
        pFilter->Release();
        if (SUCCEEDED(hr))
        {
            bFound = true;
            break;
        }
    }

    SAFE_RELEASE(pEnum);
    SAFE_RELEASE(pFilter);

    return (bFound ? S_OK : VFW_E_NOT_FOUND);
}




HRESULT FindGraphInterface(
    IGraphBuilder *pGraph, 
    REFGUID iid, 
    void **ppUnk)
{
    if (!pGraph || !ppUnk) 
    {
        return E_POINTER;
    }

    bool bFound = false;

    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter = NULL;

    HRESULT hr = pGraph->EnumFilters(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    // Loop through every filter in the graph.
    while (S_OK == pEnum->Next(1, &pFilter, 0))
    {
        hr = pFilter->QueryInterface(iid, ppUnk);
        if (FAILED(hr))
        {
            // The filter does not expose the interface, but maybe
            // one of its pins does.
            hr = FindPinInterface(pFilter, iid, ppUnk);
        }

        pFilter->Release();
        if (SUCCEEDED(hr))
        {
            bFound = true;
            break;
        }
    }

    SAFE_RELEASE(pEnum);
    SAFE_RELEASE(pFilter);

    return (bFound ? S_OK : VFW_E_NOT_FOUND);
}


// FindPinByIndex: Get the Nth pin with a specified direction.
HRESULT FindPinByIndex(IBaseFilter *pFilter, PIN_DIRECTION PinDir,
	UINT nIndex, IPin **ppPin)
{
	if (!pFilter || !ppPin)
	{
		return E_POINTER;
	}

	IEnumPins *pEnum = NULL;
	IPin *pPin = NULL;

	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}

	bool bFound = false;
	UINT count = 0;
	while (S_OK == (hr = pEnum->Next(1, &pPin, NULL)))
	{
		PIN_DIRECTION ThisDir;
		hr = pPin->QueryDirection(&ThisDir);

		if (FAILED(hr))
		{
			pPin->Release();
			break;
		}

		if (ThisDir == PinDir)
		{
			if (nIndex == count)
			{
				*ppPin = pPin;			// return to caller
				bFound = true;
				break;
			}
			count++;
		}
		pPin->Release();
	}

    SAFE_RELEASE(pPin);
	SAFE_RELEASE(pEnum);

    return (bFound ? S_OK : VFW_E_NOT_FOUND);
}

HRESULT FindPinByName(IBaseFilter *pFilter, const WCHAR *wszName, IPin **ppPin)
{
    if (!pFilter || !wszName || !ppPin)
    {
        return E_POINTER;
    }

    // Verify that wszName is not longer than MAX_PIN_NAME
    size_t cch;
    HRESULT hr = StringCchLengthW(wszName, MAX_PIN_NAME, &cch);

    if (SUCCEEDED(hr))
    {
        hr = FindMatchingPin(pFilter, MatchPinName(wszName), ppPin);
    }

    return hr;
}


HRESULT FindPinInterface(
    IBaseFilter *pFilter,  // Pointer to the filter to search.
    REFGUID iid,           // IID of the interface.
    void **ppUnk)          // Receives the interface pointer.
{
    if (!pFilter || !ppUnk) 
    {
        return E_POINTER;
    }

    bool bFound = false;

    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);

    if (FAILED(hr))
    {
        return hr;
    }
    
    // Query every pin for the interface.
    while (S_OK == pEnum->Next(1, &pPin, 0))
    {
        hr = pPin->QueryInterface(iid, ppUnk);
        pPin->Release();
        if (SUCCEEDED(hr))
        {
            bFound = true;
            break;
        }
    }

    SAFE_RELEASE(pEnum);
    SAFE_RELEASE(pPin);

    return (bFound ? S_OK : VFW_E_NOT_FOUND);
}




HRESULT FindUnconnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin            // Receives a pointer to the pin.
    )
{
    return FindMatchingPin(pFilter, MatchPinDirectionAndConnection(PinDir, FALSE), ppPin);
}


HRESULT GetConnectedFilter(IPin *pPin, IBaseFilter **ppFilter)
{
    if (!pPin || !ppFilter)
    {
        return E_POINTER;
    }

    IPin *pPeer = NULL;
    PIN_INFO info;

    ZeroMemory(&info, sizeof(info));

    HRESULT hr = pPin->ConnectedTo(&pPeer);
    if (SUCCEEDED(hr))
    {
        hr = pPeer->QueryPinInfo(&info);
    }

    if (SUCCEEDED(hr))
    {
        assert(info.pFilter != NULL);
        if (info.pFilter)
        {
            *ppFilter = info.pFilter;   // Return pointer to caller.
            (*ppFilter)->AddRef();
            hr = S_OK;
        }
        else
        {
            hr = E_UNEXPECTED;  // Pin does not have an owning filter! That's weird! 
        }
    }

    SAFE_RELEASE(pPeer);
    SAFE_RELEASE(info.pFilter);

    return hr;
}




// Get the first upstream or downstream filter
HRESULT GetNextFilter(
    IBaseFilter *pFilter, // Pointer to the starting filter
    GraphDirection Dir,    // Direction to search (upstream or downstream)
    IBaseFilter **ppNext) // Receives a pointer to the next filter.
{

    PIN_DIRECTION PinDirection = (Dir == UPSTREAM ? PINDIR_INPUT : PINDIR_OUTPUT); 

    if (!pFilter || !ppNext) 
    {
        return E_POINTER;
    }

    IPin *pPin = NULL;
    HRESULT hr = FindConnectedPin(pFilter, PinDirection, &pPin);
    if (SUCCEEDED(hr))
    {
        hr = GetConnectedFilter(pPin, ppNext);
    }

    SAFE_RELEASE(pPin);

    return hr;
}



// GetPinCategory: Return the pin category

HRESULT GetPinCategory(IPin *pPin, GUID *pPinCategory)
{
    if (pPin == NULL)
    {
        return E_POINTER;
    }
    if (pPinCategory == NULL)
    {
        return E_POINTER;
    }

    IKsPropertySet *pKs = NULL;
    HRESULT hr = pPin->QueryInterface(IID_IKsPropertySet, (void**)&pKs);
    if (SUCCEEDED(hr))
    {
        // Try to retrieve the pin category.
        DWORD cbReturned;
        hr = pKs->Get(AMPROPSETID_Pin, AMPROPERTY_PIN_CATEGORY, NULL, 0, 
            pPinCategory, sizeof(GUID), &cbReturned);
    
        // If this succeeded, pPinCategory now contains the category GUID.

    }

    SAFE_RELEASE(pKs);

    return hr;
}




// GetPinMediaType: Look for a preferred media type on a pin

HRESULT GetPinMediaType(
    IPin *pPin,             // pointer to the pin
    REFGUID majorType,      // desired major type, or GUID_NULL = don't care
    REFGUID subType,        // desired subtype, or GUID_NULL = don't care
    REFGUID formatType,     // desired format type, of GUID_NULL = don't care
    AM_MEDIA_TYPE **ppmt    // Receives a pointer to the media type. (Can be NULL)
    )
{
    if (!pPin)
    {
        return E_POINTER;
    }

    IEnumMediaTypes *pEnum = NULL;
    HRESULT hr = pPin->EnumMediaTypes(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    bool bFound = false;

    AM_MEDIA_TYPE *pmt = NULL;
    while (hr = pEnum->Next(1, &pmt, NULL), hr == S_OK)
    {
        if ((majorType == GUID_NULL) || (majorType == pmt->majortype))
        {
            if ((subType == GUID_NULL) || (subType == pmt->subtype))
            {
                if ((formatType == GUID_NULL) || (formatType == pmt->formattype))
                {
                    // Found a match. 
                    if (ppmt)
                    {
                        *ppmt = pmt;  // Return it to the caller
                    }
                    else
                    {
                        DeleteMediaType(pmt);
                    }
                    bFound = true;
                    break;
                }
            }
        }
        DeleteMediaType(pmt);
    }

    SAFE_RELEASE(pEnum);

    if (SUCCEEDED(hr))
    {
        if (!bFound)
        {
            hr = VFW_E_NOT_FOUND;
        }
    }
    return hr;
}


HRESULT IsPinConnected(IPin *pPin, BOOL *pResult)
{
    if (pPin == NULL || pResult == NULL)
    {
        return E_POINTER;
    }

    IPin *pTmp = NULL;
    HRESULT hr = pPin->ConnectedTo(&pTmp);
    if (SUCCEEDED(hr))
    {
        *pResult = TRUE;
    }
    else if (hr == VFW_E_NOT_CONNECTED)
    {
        // The pin is not connected. This is not an error for our purposes.
        *pResult = FALSE;
        hr = S_OK;
    }

    SAFE_RELEASE(pTmp);
    return hr;
}


HRESULT IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult)
{
    if (pPin == NULL || pResult == NULL)
    {
        return E_POINTER;
    }

    PIN_DIRECTION pinDir;
    HRESULT hr = pPin->QueryDirection(&pinDir);
    if (SUCCEEDED(hr))
    {
        *pResult = (pinDir == dir);
    }
    return hr;
}



HRESULT IsPinUnconnected(IPin *pPin, BOOL *pResult)
{
    HRESULT hr = IsPinConnected(pPin, pResult);
    if (SUCCEEDED(hr))
    {
        *pResult = !(*pResult);
    }
    return hr;
}



HRESULT IsRenderer(IBaseFilter *pFilter, BOOL *pResult)
{
    if (pFilter == NULL || pResult == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    BOOL bIsRenderer = FALSE;

    IAMFilterMiscFlags *pFlags = NULL;
    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;


    // First try IAMFilterMiscFlags. 
    hr = pFilter->QueryInterface(IID_IAMFilterMiscFlags, (void**)&pFlags);
    if (SUCCEEDED(hr))
    {
        ULONG flags = pFlags->GetMiscFlags();
        if (flags & AM_FILTER_MISC_FLAGS_IS_RENDERER)
        {
            bIsRenderer = TRUE;
        }
    }

    if (!bIsRenderer)
    {
        // Look for the following conditions:

        // 1) Zero output pins AND at least 1 unmapped input pin
        // - or -
        // 2) At least 1 rendered input pin.

        // definitions:
        // unmapped input pin = IPin::QueryInternalConnections returns E_NOTIMPL
        // rendered input pin = IPin::QueryInternalConnections returns "0" slots

        // These cases are somewhat obscure and probably don't apply to many filters
        // that actually exist.

        hr = pFilter->EnumPins(&pEnum);
        if (SUCCEEDED(hr))
        {
            bool bFoundRenderedInputPin = false;
            bool bFoundUnmappedInputPin = false;
            bool bFoundOuputPin = false;

            while (pEnum->Next(1, &pPin, NULL) == S_OK)
            {
                BOOL bIsOutput = FALSE;
                hr = IsPinDirection(pPin, PINDIR_OUTPUT, &bIsOutput);
                if (FAILED(hr))
                {
                    break;
                }
                else if (bIsOutput)
                {
                    // This is an output pin.
                    bFoundOuputPin = true;
                }
                else
                {
                    // It's an input pin. Is it mapped to an output pin?
                    ULONG nPin = 0;
                    hr = pPin->QueryInternalConnections(NULL, &nPin);
                    if (hr == S_OK)
                    {
                        // The count (nPin) was zero, and the method returned S_OK, so
                        // this input pin is mapped to exactly zero ouput pins. 
                        // Therefore, it is a rendered input pin. 
                        bFoundRenderedInputPin = true;

                        // We have met condition (2) above, so we can stop looking.
                        break;

                        // Note: S_FALSE here means "the count (nPin) was not large
                        // enough", ie, this pin is mapped to one or more output pins.

                    }
                    else if (hr == E_NOTIMPL)
                    {
                        // This pin is not mapped to any particular output pin. 
                        bFoundUnmappedInputPin = true;
                        hr = S_OK;
                    }
                    else if (FAILED(hr))
                    {
                        // Unexpected error
                        break;
                    }

                }
                pPin->Release();  // Release for the next loop.
            }  // while

           
            if (bFoundRenderedInputPin)
            {
                bIsRenderer = TRUE; // condition (1) above
            }
            else if (bFoundUnmappedInputPin && !bFoundOuputPin)
            {
                bIsRenderer = TRUE;  // condition (2) above
            }
            else
            {
                hr = S_FALSE;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        *pResult = bIsRenderer;
    }


    SAFE_RELEASE(pFlags);
    SAFE_RELEASE(pEnum);
    SAFE_RELEASE(pPin);

    return hr;
}


HRESULT IsSourceFilter(IBaseFilter *pFilter, BOOL *pResult)
{
    if (pFilter == NULL || pResult == NULL)
    {
        return E_POINTER;
    }

    IAMFilterMiscFlags *pFlags = NULL;
    IFileSourceFilter *pFileSrc = NULL;

    BOOL bIsSource = FALSE;

    // If the filter exposes the IAMFilterMiscFlags interface, we use
    // IAMFilterMiscFlags::GetMiscFlags to test if this is a source filter.

    // Otherwise, it is a source filter if it exposes IFileSourceFilter.

    // First try IAMFilterMiscFlags
    HRESULT hr = pFilter->QueryInterface(IID_IAMFilterMiscFlags, (void**)&pFlags);
    if (SUCCEEDED(hr))
    {

        ULONG flags = pFlags->GetMiscFlags();
        if (flags &  AM_FILTER_MISC_FLAGS_IS_SOURCE)
        {
            bIsSource = TRUE;
        }
    }
    else
    {
        // Next, look for IFileSourceFilter. 
        hr = pFilter->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSrc);
        if (SUCCEEDED(hr))
        {
            bIsSource = TRUE;
        }
    }

    if (SUCCEEDED(hr))
    {
        *pResult = bIsSource;
    }

    SAFE_RELEASE(pFlags);
    SAFE_RELEASE(pFileSrc);

    return hr;
}





HRESULT LetterBoxRect(const SIZE &aspectRatio, const RECT &rcDest, RECT *prcResult)
{
    if (prcResult == NULL)
    {
        return E_POINTER;
    }

    // Avoid divide by zero (even though MulDiv handles this)
    if (aspectRatio.cx == 0 || aspectRatio.cy == 0)
    {
        return E_INVALIDARG;
    }

    LONG width, height;

    LONG SrcWidth = aspectRatio.cx;
    LONG SrcHeight = aspectRatio.cy;
    LONG DestWidth = rcDest.right - rcDest.left;
    LONG DestHeight = rcDest.bottom - rcDest.top;


    if (MulDiv(SrcWidth, DestHeight, SrcHeight) <= DestWidth)
    {
        // src width / src height <= dest width / dest height
        // Letterbox along the sides. ("pillarbox")
        width = MulDiv(DestHeight, SrcWidth, SrcHeight);
        height = DestHeight;
    }
    else
    {
        // src width / src height > dest width / dest height
        // Letterbox along the top and bottom.
        width = DestWidth;
        height = MulDiv(DestWidth, SrcHeight, SrcWidth);
    }

    if (width == -1 || height == -1)
    {
        // MulDiv caught an overflow or divide by zero)
        return E_FAIL;
    }

    assert(width <= DestWidth);
    assert(height <= DestHeight);

    // Fill in the rectangle
    prcResult->left = rcDest.left + ((DestWidth - width) / 2);
    prcResult->right = prcResult->left + width;
    prcResult->top = rcDest.top + ((DestHeight - height) / 2);
    prcResult->bottom = prcResult->top + height;

    return S_OK;
}




///////////////////////////////////////////////////////////////////////
//
// Name: AddSourceFilter
// Desc: Load a specified source filter for a file.
//
///////////////////////////////////////////////////////////////////////

HRESULT AddSourceFilter(
    IGraphBuilder *pGraph,          // Pointer to the filter graph manager.
    const WCHAR *szFile,            // File name
    const GUID& clsidSourceFilter,  // CLSID of the source filter to use
    IBaseFilter **ppSourceFilter)   // receives a pointer to the filter.
{
    if (!pGraph || !szFile || !ppSourceFilter)
    {
        return E_POINTER;
    }

    IBaseFilter *pSource = NULL;
    IFileSourceFilter *pFileSource = NULL;

    // Create the source filter and add it to the graph.
    HRESULT hr = CoCreateInstance(
        clsidSourceFilter, 
        NULL, 
        CLSCTX_INPROC_SERVER,
        IID_IBaseFilter,
        (void**)&pSource);

    if (SUCCEEDED(hr))
    {
        hr = pGraph->AddFilter(pSource, szFile);
    }

    if (SUCCEEDED(hr))
    {
        hr = pSource->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSource);
    }
    
    if (SUCCEEDED(hr))
    {
        hr = pFileSource->Load(szFile, NULL);
    }
    if (SUCCEEDED(hr))
    {
        // Return the filter pointer to the caller.
        *ppSourceFilter = pSource;
        (*ppSourceFilter)->AddRef();
    }
    else 
    {
        // FAILED, remove the filter
        if (pSource != NULL)
        {
            RemoveFilter(pGraph, pSource);
        }
    }

    SAFE_RELEASE(pSource);
    SAFE_RELEASE(pFileSource);

    return hr;
}


///////////////////////////////////////////////////////////////////////
//
// Name: AddSourceFilter
// Desc: Add a file-writer filter.
//
///////////////////////////////////////////////////////////////////////
HRESULT AddWriterFilter(
    IGraphBuilder *pGraph,
    const WCHAR *szFile,
    const GUID& clsid,
	BOOL bOverwrite,
    IBaseFilter **ppFilter
    )
{
    if (pGraph == NULL)
    {
        return E_POINTER;
    }
    if (szFile == NULL)
    {
        return E_POINTER;
    }
    if (ppFilter == NULL)
    {
        return E_POINTER;
    }

    IBaseFilter *pFilter = NULL;
    IFileSinkFilter *pSink = NULL;
	IFileSinkFilter2 *pSink2 = NULL;

    HRESULT hr = AddFilterByCLSID(pGraph, clsid, &pFilter, szFile);
    if (SUCCEEDED(hr))
    {
        hr = pFilter->QueryInterface(IID_IFileSinkFilter, (void**)&pSink);
    }
    if (SUCCEEDED(hr))
    {
        hr = pSink->SetFileName((LPCOLESTR)szFile, NULL);
    }

	if (SUCCEEDED(hr) && bOverwrite)
	{
		hr = pFilter->QueryInterface(IID_IFileSinkFilter2, (void**)&pSink2);
		if (SUCCEEDED(hr))
		{
			hr = pSink2->SetMode(AM_FILE_OVERWRITE);
		}
	}

    if (SUCCEEDED(hr))
    {
        *ppFilter = pFilter;
        (*ppFilter)->AddRef();
    }
    else
    {
        RemoveFilter(pGraph, pFilter);
    }

	SAFE_RELEASE(pFilter);
	SAFE_RELEASE(pSink);
	SAFE_RELEASE(pSink2);

    return hr;
}



HRESULT RemoveFilter(IGraphBuilder *pGraph, IBaseFilter *pFilter)
{
    if (!pGraph || !pFilter)
    {
        return E_POINTER;
    }

    IEnumPins *pEnum = NULL;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (SUCCEEDED(hr))
    {
        // Disconnect all the pins
        IPin *pPin = NULL;
        while (S_OK == pEnum->Next(1, &pPin, 0))
        {
            hr = DisconnectPin(pGraph, pPin);
            pPin->Release();

            if (FAILED(hr))
            {
                break;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = pGraph->RemoveFilter(pFilter);
    }

    SAFE_RELEASE(pEnum);

    return hr;
}


// RemoveFiltersDownstreamOfPin: 
// Removes any filters downstream from pPin, AND removes the filter that owns pPin.
// (This is a recursive function used by RemoveFiltersDownstream, see below.)

HRESULT RemoveFiltersDownstreamOfPin(IGraphBuilder *pGraph, IPin *pPin)
{
    // If the pin is an output pin, and the pin is connected, then
    //   1. Get the owning filter
    //   2. Call RemoveFiltersDownstream recursively
    //   3. Disconnect the pin
    //   4. Remove the owning filter

    BOOL bIsOutput = FALSE;
    HRESULT hr = IsPinDirection(pPin, PINDIR_OUTPUT, &bIsOutput);
    if (FAILED(hr))
    {
        return hr;
    }
    if (!bIsOutput)
    {
        // This pin is not an input pin, so there is nothing to do.
        return S_OK;
    }

    // Get the owning filter.

    IBaseFilter *pFilter = NULL;
    hr = GetConnectedFilter(pPin, &pFilter);
    if (hr == VFW_E_NOT_CONNECTED)
    {
        hr =  S_OK; // This pin was not connected, so there is nothing to do.
    }
    else if (SUCCEEDED(hr))
    {
        // Call recursively to remove the filters downstream from this pin.
        hr = RemoveFiltersDownstream(pGraph, pFilter);
        if (SUCCEEDED(hr))
        {
			FILTER_INFO fInfo;
			pFilter->QueryFilterInfo(&fInfo);
			OutputDebugString(fInfo.achName);
			OutputDebugString(L"\n");
			fInfo.pGraph->Release();


            // Now remove the owning filter.
            hr = RemoveFilter(pGraph, pFilter);
        }
    }

    SAFE_RELEASE(pFilter);

    return hr;
}


HRESULT RemoveFiltersDownstream(IGraphBuilder *pGraph, IBaseFilter *pFilter)
{
    if (!pGraph || !pFilter)
    {
        return E_POINTER;
    }

    IEnumPins *pEnum = NULL;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (SUCCEEDED(hr))
    {

        IPin *pPin = NULL;
        while (S_OK == pEnum->Next(1, &pPin, 0))
        {
            hr = RemoveFiltersDownstreamOfPin(pGraph, pPin);
            pPin->Release();
            if (FAILED(hr))
            {
                break;
            }
       }
    }

    SAFE_RELEASE(pEnum);

    return hr;
}


HRESULT ShowFilterPropertyPage(IBaseFilter *pFilter, HWND hwndParent)
{
    HRESULT hr;

    ISpecifyPropertyPages *pSpecify = NULL;
	FILTER_INFO FilterInfo;
	CAUUID caGUID;

    if (!pFilter)
	{
        return E_POINTER;
	}

	ZeroMemory(&FilterInfo, sizeof(FilterInfo));
	ZeroMemory(&caGUID, sizeof(caGUID));

    // Discover if this filter contains a property page
    hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpecify);
    if (SUCCEEDED(hr)) 
    {
		hr = pFilter->QueryFilterInfo(&FilterInfo);
	}

	if (SUCCEEDED(hr))
	{
		hr = pSpecify->GetPages(&caGUID);
	}

	// Display the filter's property page
	if (SUCCEEDED(hr))
	{
		hr = OleCreatePropertyFrame(
			hwndParent,             // Parent window
			0,                      // x (Reserved)
			0,                      // y (Reserved)
			FilterInfo.achName,     // Caption for the dialog box
			1,                      // Number of filters
			(IUnknown **)&pFilter,  // Pointer to the filter 
			caGUID.cElems,          // Number of property pages
			caGUID.pElems,          // Pointer to property page CLSIDs
			0,                      // Locale identifier
			0,                      // Reserved
			NULL                    // Reserved
			);

    }

	CoTaskMemFree(caGUID.pElems);
	
	SAFE_RELEASE(FilterInfo.pGraph); 
    SAFE_RELEASE(pSpecify);
    return hr;
}



//----------------------------------------------------------------------------
// SetMediaTypeFormat: Sets the format block of a media type
//----------------------------------------------------------------------------


HRESULT SetMediaTypeFormatBlock(AM_MEDIA_TYPE *pmt, BYTE *pBuffer, DWORD cbBuffer)
{
	if (!pmt)
	{
		return E_POINTER;
	}
	if (!pBuffer && cbBuffer > 0)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	pmt->pbFormat = (BYTE*)CoTaskMemRealloc(pmt->pbFormat, cbBuffer);
	pmt->cbFormat = cbBuffer;

	if (cbBuffer > 0)
	{
		if (pmt->pbFormat)
		{
			CopyMemory(pmt->pbFormat, pBuffer, cbBuffer);
		}
		else
		{
			pmt->cbFormat = 0;
			hr = E_OUTOFMEMORY;  // CoTaskMemRealloc failed
		}
	}		

	return hr;
}


BOOL SupportsPropertyPage(IBaseFilter *pFilter)
{
	if (pFilter == NULL)
	{
		return FALSE; 
	}

	ISpecifyPropertyPages *pProp = NULL;

	HRESULT hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, 
		(void**)&pProp);

	if (SUCCEEDED(hr))
	{
		pProp->Release();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
{
    if (pGraph == NULL)
    {
        return E_POINTER;
    }

    const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
    
    IStorage *pStorage = NULL;
    IStream *pStream = NULL;
    IPersistStream *pPersist = NULL;

    HRESULT hr = StgCreateDocfile(
        wszPath,
        STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        0, &pStorage);

    if(SUCCEEDED(hr)) 
    {
        hr = pStorage->CreateStream(
            wszStreamName,
            STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
            0, 
            0, 
            &pStream);
    }

    if (SUCCEEDED(hr))
    {
        hr = pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
    }

    if (SUCCEEDED(hr))
    {
        hr = pPersist->Save(pStream, TRUE);
    }

    if (SUCCEEDED(hr)) 
    {
        hr = pStorage->Commit(STGC_DEFAULT);
    }

    SAFE_RELEASE(pStorage);
    SAFE_RELEASE(pStream);
    SAFE_RELEASE(pPersist);

    return hr;
}





HRESULT LoadGraphFile(IGraphBuilder *pGraph, const WCHAR* wszName)
{
    if (pGraph == NULL)
    {
        return E_POINTER;
    }

    if (wszName == NULL)
    {
        return E_POINTER;
    }

    if (S_OK != StgIsStorageFile(wszName))
    {
        return E_FAIL;
    }

    IStorage *pStorage = NULL;
    IPersistStream *pPersistStream = NULL;
    IStream *pStream = NULL;

    HRESULT hr = StgOpenStorage(
        wszName, 
        0, 
        STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE, 
        0, 
        0, 
        &pStorage);

    if (SUCCEEDED(hr))
    {
        hr = pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersistStream);
    }

    if (SUCCEEDED(hr))
    {
        hr = pStorage->OpenStream(L"ActiveMovieGraph", 0, 
            STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
    }

    if(SUCCEEDED(hr))
    {
        hr = pPersistStream->Load(pStream);
    }

    SAFE_RELEASE(pStorage);
    SAFE_RELEASE(pPersistStream);
    SAFE_RELEASE(pStream);

    return hr;
}


// Adds a DirectShow filter graph to the Running Object Table,
// allowing GraphEdit to load a remote filter graph.
HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
    IMoniker * pMoniker = NULL;
    IRunningObjectTable *pROT = NULL;
    WCHAR wsz[128];
    HRESULT hr;

    if (!pUnkGraph || !pdwRegister)
    {
        return E_POINTER;
    }

    do
    {
        hr = GetRunningObjectTable(0, &pROT);

        if (FAILED(hr))
        {
            break;
        }

        // Format the string for the registration
        hr = StringCchPrintfW(
                wsz, NUMELMS(wsz), L"FilterGraph %08x pid %08x\0", 
                (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
        if (FAILED(hr))
        {
            break;
        }

        // Create the moniker
        hr = CreateItemMoniker(L"!", wsz, &pMoniker);
        if (FAILED(hr))
        {
            break;
        }

        // Use the ROTFLAGS_REGISTRATIONKEEPSALIVE to ensure a strong reference
        // to the object.  Using this flag will cause the object to remain
        // registered until it is explicitly revoked with the Revoke() method.
        //
        // Not using this flag means that if GraphEdit remotely connects
        // to this graph and then GraphEdit exits, this object registration 
        // will be deleted, causing future attempts by GraphEdit to fail until
        // this application is restarted or until the graph is registered again.
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, 
                            pMoniker, pdwRegister);
        if (FAILED(hr))
        {
            break;
        }
    } while (false);


    SAFE_RELEASE(pMoniker);
    SAFE_RELEASE(pROT);
    return hr;
}

// Removes a filter graph from the Running Object Table
void RemoveGraphFromRot(DWORD dwRegister)
{
    IRunningObjectTable *pROT = NULL;

    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) 
    {
        pROT->Revoke(dwRegister);
    }

    SAFE_RELEASE(pROT);
}





