//////////////////////////////////////////////////////////////////////////
// DSUtil.h: DirectShow helper functions.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

// Conventions:
//
// Functions named "IsX" return true or false.
//
// Functions named "FindX" enumerate over a collection and return the first
// matching instance. 

#pragma once

#include <dshow.h>

#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

/*
    Pin and Filter Functions
    -------------------
    GetPinCategory
    GetPinMediaType
    FindUnconnectedPin
    FindConnectedPin
	FindPinByCategory
	FindPinByIndex
    FindPinByName
    FindPinInterface
    FindMatchingPin
    IsPinConnected
    IsPinDirection
    IsPinUnconnected
    IsSourceFilter
    IsRenderer
	SupportsPropertyPage

    Graph Building Functions 
    -----------------------
    AddFilterByCLSID
	AddFilterFromMoniker
    AddSourceFilter
    AddWriterFilter
    ConnectFilters
    CreateKernelFilter
    DisconnectPin
    FindFilterInterface
    FindGraphInterface
    GetNextFilter
    GetConnectedFilter
    RemoveFilter
    RemoveFiltersDownstream

    Media Type Functions
    --------------------
    CreatePCMAudioType
    CreateRGBVideoType
    DeleteMediaType
    FreeMediaType
    CopyFormatBlock

    GraphEdit Functions
    -------------------
    AddGraphToRot
    LoadGraphFile
    RemoveGraphFromRot
    SaveGraphFile

    Misc Functions
    --------------
    FramesPerSecToFrameLength
    LetterBoxRect
    MsecToRefTime
    RectWidth
    RectHeight
    RefTimeToMsec
    RefTimeToSeconds
    SecondsToRefTime

*/



const LONGLONG ONE_SECOND = 10000000;  // One second, in 100-nanosecond units.
const LONGLONG ONE_MSEC = ONE_SECOND / 10000;  // One millisecond, in 100-nanosecond units

// Directions for filter graph data flow.
enum GraphDirection
{ 
    UPSTREAM, DOWNSTREAM
};


/**********************************************************************

    Pin Query Functions - Test pins for various things

**********************************************************************/


///////////////////////////////////////////////////////////////////////
// Name: GetPinCategory
// Desc: Returns the category of a pin
//
// Note: Pin categories are used by some kernel-mode filters to
//       distinguish different outputs. (e.g, capture and preview)
///////////////////////////////////////////////////////////////////////

HRESULT GetPinCategory(
    IPin *pPin,             // Pointer to the pin.
    GUID *pPinCategory      // Receives the pin category.
    );


///////////////////////////////////////////////////////////////////////
// Name: GetPinMediaType
// Desc: Given a pin, find a preferred media type 
//
// pPin         Pointer to the pin
// majorType    Preferred major type (GUID_NULL = don't care)
// subType      Preferred subtype (GUID_NULL = don't care)
// formatType   Preferred format type (GUID_NULL = don't care)
// ppmt         Receives a pointer to the media type. Can be NULL.
//
// Note: If you want to check whether a pin supports a desired media type,
//       but do not need the format details, set ppmt to NULL.
//
//       If ppmt is not NULL and the method succeeds, the caller must
//       delete the media type, including the format block. (Use the
//       DeleteMediaType function.)
///////////////////////////////////////////////////////////////////////

HRESULT GetPinMediaType(
    IPin *pPin, 
    REFGUID majorType, 
    REFGUID subType,
    REFGUID formatType,
    AM_MEDIA_TYPE **ppmt
    );



///////////////////////////////////////////////////////////////////////
// Name: IsPinConnected
// Desc: Query whether a pin is connected to another pin.
//
// Note: If you need to get the other pin, use IPin::ConnectedTo.
///////////////////////////////////////////////////////////////////////

HRESULT IsPinConnected(
    IPin *pPin,             // Pointer to the pin.
    BOOL *pbResult
    );




///////////////////////////////////////////////////////////////////////
// Name: IsPinUnconnected
// Desc: Query whether a pin is NOT connected to another pin.
//
///////////////////////////////////////////////////////////////////////

HRESULT IsPinUnconnected(
    IPin *pPin,             // Pointer to the pin.
    BOOL *pbResult
    );


///////////////////////////////////////////////////////////////////////
// Name: IsPinDirection
// Desc: Query whether a pin has a specified direction (input / output)
//
///////////////////////////////////////////////////////////////////////

HRESULT IsPinDirection(
    IPin *pPin,             // Pointer to the pin.
    PIN_DIRECTION dir,      // The direction to test for.
    BOOL *pbResult
    );


/**************************************************************************

    Pin Searching Functions

    These functions search a filter for a pin that matches some set of
    criteria. They return the first pin that matches, or VFW_E_NOT_FOUND.

**************************************************************************/


///////////////////////////////////////////////////////////////////////
// Name: FindPinByIndex
// Desc: Return the Nth pin with the specified pin direction.
///////////////////////////////////////////////////////////////////////

HRESULT FindPinByIndex(
	IBaseFilter *pFilter,
	PIN_DIRECTION PinDir,
	UINT nIndex,
	IPin **ppPin
	);


///////////////////////////////////////////////////////////////////////
// Name: FindUnconnectedPin
// Desc: Return the first unconnected input pin or output pin.
///////////////////////////////////////////////////////////////////////

HRESULT FindUnconnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin            // Receives a pointer to the pin.
    );


///////////////////////////////////////////////////////////////////////
// Name: FindConnectedPin
// Desc: Return the first connected input pin or output pin
///////////////////////////////////////////////////////////////////////


HRESULT FindConnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin            // Receives a pointer to the pin.
    );


///////////////////////////////////////////////////////////////////////
// Name: FindPinByCategory
// Desc: Find the first pin that matches a specified pin category
//       and direction.
///////////////////////////////////////////////////////////////////////

HRESULT FindPinByCategory(
	IBaseFilter *pFilter, 
	REFGUID guidCategory,
	PIN_DIRECTION PinDir,
	IPin **ppPin
	);



///////////////////////////////////////////////////////////////////////
// Name: FindPinByName
// Desc: Find a pin by name.
//
// Note: Generally, you should find pins by direction, media type,
//       and/or pin category. However, in some cases you may need to
//       find a pin by name.
///////////////////////////////////////////////////////////////////////

HRESULT FindPinByName(IBaseFilter *pFilter, const WCHAR *wszName, IPin **ppPin);


///////////////////////////////////////////////////////////////////////
// Name: FindPinInterface
// Desc: Search a filter for a pin that exposes a specified interface.
//       (Returns the first instance found.)
// 
// pFilter  Pointer to the filter to search.
// iid      IID of the interface.
// ppUnk    Receives the interface pointer.
// Q        Address of an ATL smart pointer.
//
// Note:    This function returns the first instance that it finds. 
//          If no pin is found, the function returns VFW_E_NOT_FOUND.
//          The templated version deduces the IID.
///////////////////////////////////////////////////////////////////////

HRESULT FindPinInterface(IBaseFilter *pFilter, REFGUID iid, void **ppUnk);

template <class Q>
HRESULT FindPinInterface(IGraphBuilder *pGraph, Q** pp)
{
    return FindPinInterface(pGraph, __uuidof(Q), (void**)pp);
}



///////////////////////////////////////////////////////////////////////
// Name: FindMatchingPin (template)
// Desc: Return the first pin on a filter that matches a caller-supplied 
//       function or function object
//
// FN must be either
//   (a) A function with the signature HRESULT (IPin*, BOOL *)
//   (b) A class that implements HRESULT operator()(IPin*, BOOL *)
//
// FindMatchingPin halts if FN fails or returns TRUE
///////////////////////////////////////////////////////////////////////


template <class PinPred>
HRESULT FindMatchingPin(IBaseFilter *pFilter, PinPred FN, IPin **ppPin)
{
    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    BOOL bFound = FALSE;
    while (S_OK == pEnum->Next(1, &pPin, NULL))
    {
        hr = FN(pPin, &bFound);

        if (FAILED(hr))
        {
            pPin->Release();
            break;
        }
        if (bFound)
        {
            *ppPin = pPin;
            break;
        }

        pPin->Release();
    }

    pEnum->Release();

    if (!bFound)
    {
        hr = VFW_E_NOT_FOUND;
    }

    return hr;
}

/**********************************************************************

    Filter Query Functions  - Test filters for various conditions.

**********************************************************************/



///////////////////////////////////////////////////////////////////////
// Name: IsSourceFilter
// Desc: Query whether a filter is a source filter.
///////////////////////////////////////////////////////////////////////

HRESULT IsSourceFilter(
    IBaseFilter *pFilter , // Pointer to the filter.
    BOOL *pbResult
    );


///////////////////////////////////////////////////////////////////////
// Name: IsRenderer
// Desc: Query whether a filter is a renderer filter.
///////////////////////////////////////////////////////////////////////

HRESULT IsRenderer(
    IBaseFilter *pFilter,   // Pointer to the filter.
    BOOL *pbResult
    );


///////////////////////////////////////////////////////////////////////
// Name: SupportsPropertyPage
// Desc: Query whether a filter has a property page.
///////////////////////////////////////////////////////////////////////

BOOL SupportsPropertyPage(IBaseFilter *pFilter);


///////////////////////////////////////////////////////////////////////
// Name: ShowFilterPropertyPage
// Desc: Show a filter's property page.
//
// hwndParent: Owner window for the property page.
///////////////////////////////////////////////////////////////////////

HRESULT ShowFilterPropertyPage(IBaseFilter *pFilter, HWND hwndParent);


/**************************************************************************

    Graph Building Helper Functions

**************************************************************************/



///////////////////////////////////////////////////////////////////////
// Name: AddFilterByCLSID
// Desc: Create a filter by CLSID and add it to the graph.
///////////////////////////////////////////////////////////////////////

HRESULT AddFilterByCLSID(
    IGraphBuilder *pGraph,          // Pointer to the Filter Graph Manager.
    const GUID& clsid,              // CLSID of the filter to create.
    IBaseFilter **ppF,              // Receives a pointer to the filter.
    LPCWSTR wszName = NULL          // A name for the filter (can be NULL).
    );


///////////////////////////////////////////////////////////////////////
// Name: AddFilterFromMoniker
// Desc: Create a filter from an IMoniker pointer and add it to the graph.
///////////////////////////////////////////////////////////////////////

HRESULT AddFilterFromMoniker(
    IGraphBuilder *pGraph,          // Pointer to the Filter Graph Manager.
	IMoniker *pFilterMoniker,		// Pointer to the moniker.
    IBaseFilter **ppF,              // Receives a pointer to the filter.
    LPCWSTR wszName = NULL          // A name for the filter (can be NULL).
	);

///////////////////////////////////////////////////////////////////////
// Name: AddSourceFilter
// Desc: Loads a source filter.
//
// pGraph: Pointer to the filter graph manager.
// szFile: File name.
// clsid:  CLSID of the source filter to use.
// ppSourceFilter: Receives a pointer to the source filter's IBaseFilter 
//                 interface.
//
// This function creates the filter, adds it to the graph, queries for
// IFileSourceFilter, and calls IFileSourceFilter::Load. 
//
// You can use this function if you want to specify the source filter by
// CLSID. Otherwise, just use IGraphBuilder::AddSourceFilter.
///////////////////////////////////////////////////////////////////////

HRESULT AddSourceFilter(
    IGraphBuilder *pGraph,          
    const WCHAR *szFile,            
    const GUID& clsid,              
    IBaseFilter **ppSourceFilter    
    );


///////////////////////////////////////////////////////////////////////
// Name: AddWriterFilter
// Desc: Adds a file-writing filter. (ie, a filter that supports
//       IFileSinkFilter.)
//
// pGraph: Pointer to the filter graph manager.
// szFile: File name.
// clsid:  CLSID of the filter.
// bOverwrite: If TRUE, overwrite the file.
// ppSourceFilter: Receives the filter's IBaseFilter interface.
//
// This function creates the filter, adds it to the graph, sets the 
// file name, and set the overwrite mode. 
///////////////////////////////////////////////////////////////////////
HRESULT AddWriterFilter(
    IGraphBuilder *pGraph,
    const WCHAR *szFile,
    const GUID& clsid,
	BOOL bOverwrite,
    IBaseFilter **ppFilter
    );


///////////////////////////////////////////////////////////////////////
// Name: ConnectFilters
// Desc: Connect two filters.
//
// There are 3 overrides on this method:
// (1) From output pin to filter
// (2) From filter to filter
// (3) From filter to input pin
// 
// Note: The 4th combination (output pin to input pin) is already
// provided by the IGraphBuilder::Connect method. 
//
// All of these methods search the filter(s) for the first unconnected
// pin. If that pin fails, the method fails. In some cases, it may be 
// better to try every pin until one succeeds, or loop through the pins 
// and examine the preferred media types. 
// 
///////////////////////////////////////////////////////////////////////

HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest);  
HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest);
HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IPin *pIn);  


///////////////////////////////////////////////////////////////////////
// Name: CreateKernelFilter
// Desc: Create a kernel-mode filter by name.
///////////////////////////////////////////////////////////////////////

HRESULT CreateKernelFilter(
    const GUID &guidCategory,  // Filter category.
    LPCOLESTR szName,          // The name of the filter.
    IBaseFilter **ppFilter     // Receives a pointer to the filter.
);


///////////////////////////////////////////////////////////////////////
// Name: DisconnectPin
// Desc: Disconnect a pin from its peer.
//
// Note: If the pin is not connected, the function returns S_OK (no-op).
///////////////////////////////////////////////////////////////////////

HRESULT DisconnectPin(
    IGraphBuilder *pGraph,  // Pointer to the filter graph manager
    IPin *pPin              // Pointer to the pin to disconnect.
    );


///////////////////////////////////////////////////////////////////////
// Name: FindFilterInterface
// Desc: Search the graph for a filter that exposes a specified interface.
// 
// pGraph   Pointer to the Filter Graph Manager.
// iid      IID of the interface to retrieve.
// ppUnk    Receives the interface pointer.
// Q        Address of an ATL smart pointer.
//
// Note:    This function returns the first instance that it finds. 
//          If no filter is found, the function returns VFW_E_NOT_FOUND.
//          The templated version takes an ATL smart pointer and deduces the IID.
///////////////////////////////////////////////////////////////////////

HRESULT FindFilterInterface(IGraphBuilder *pGraph, REFGUID iid, void **ppUnk);  

template <class Q>
HRESULT FindFilterInterface(IGraphBuilder *pGraph, Q** pp)
{
    return FindFilterInterface(pGraph, __uuidof(Q), (void**)pp);
}




///////////////////////////////////////////////////////////////////////
// Name: FindGraphInterface
// Desc: Search the graph for a filter OR pin that exposes a specified 
//       interface. 
// 
// pGraph   Pointer to the Filter Graph Manager.
// iid      IID of the interface.
// ppUnk    Receives the interface pointer.
// Q        Address of an ATL smart pointer.
//
// Note:    This function returns the first instance that it finds. 
//          If no match is found, the function returns VFW_E_NOT_FOUND.
//          The templated version takes an ATL smart pointer and deduces the IID.
///////////////////////////////////////////////////////////////////////

HRESULT FindGraphInterface(IGraphBuilder *pGraph, REFGUID iid, void **ppUnk);

template <class Q>
HRESULT FindGraphInterface(IGraphBuilder *pGraph, Q** pp)
{
    return FindGraphInterface(pGraph, __uuidof(Q), (void**)pp);
}



///////////////////////////////////////////////////////////////////////
// Name: GetNextFilter
// Desc: Find a filter's upstream or downstream neighbor. 
//       (Returns the first instance found.)
// 
// If there is no matching filter, the function returns VFW_E_NOT_CONNECTED.
///////////////////////////////////////////////////////////////////////

HRESULT GetNextFilter(
    IBaseFilter *pFilter,   // Pointer to the filter
    GraphDirection Dir,     // Direction to search (upstream or downstream)
    IBaseFilter **ppNext    // Receives a pointer to the peer filter.
    );


///////////////////////////////////////////////////////////////////////
// Name: GetConnectedFilter
// Desc: Returns the filter on the other side of a pin. 
//
// ie, Given a pin, get the pin connected to it, and return that pin's filter.
// 
// pPin     Pointer to the pin.
// ppFilter Receives a pointer to the filter.
//
///////////////////////////////////////////////////////////////////////

HRESULT GetConnectedFilter(IPin *pPin, IBaseFilter **ppFilter);


///////////////////////////////////////////////////////////////////////
// Name: RemoveFilter
// Desc: Removes a filter from the graph.
//
// Note: The function first disconnects the filter's pins. If the method
//       fails, some pins may be disconnected, so the graph is in an
//       unknown state.
///////////////////////////////////////////////////////////////////////

HRESULT RemoveFilter(
    IGraphBuilder *pGraph,  // Pointer to the filter graph manager
    IBaseFilter *pFilter    // Pointer to the filter to remove.
    );



///////////////////////////////////////////////////////////////////////
// Name: RemoveFiltersDownstream
// Desc: Remove a filter from the graph, including all filters 
//       downstream from that filter.
//
// Note: This function removes pFilter from the graph and 
//       removes every filter that is downstream from pFilter.
///////////////////////////////////////////////////////////////////////

HRESULT RemoveFiltersDownstream(
    IGraphBuilder *pGraph,  // Pointer to the filter graph manager
    IBaseFilter *pFilter    // Pointer to the filter to remove.
    );



/**************************************************************************

    Misc. Helper Functions

**************************************************************************/

// CopyFormatBlock: 
// Allocates memory for the format block in the media type and copies the 
// buffer into the format block. Also releases the previous format block.
HRESULT CopyFormatBlock(AM_MEDIA_TYPE *pmt, const BYTE *pFormat, DWORD cbSize);



// RectWidth: Returns the width of a rectangle.
inline LONG RectWidth(const RECT& rc) { return rc.right - rc.left; }

// RectHeight: Returns the height of a rectangle.
inline LONG RectHeight(const RECT& rc) { return rc.bottom - rc.top; }


/********************* Time conversion functions *********************/

///////////////////////////////////////////////////////////////////////
// FramesPerSecToFrameLength
// Converts from frames-to-second to frame duration.
///////////////////////////////////////////////////////////////////////

inline REFERENCE_TIME FramesPerSecToFrameLength(double fps) 
{ 
    return (REFERENCE_TIME)((double)ONE_SECOND / fps);
}

///////////////////////////////////////////////////////////////////////
// RefTimeToMsec
// Convert REFERENCE_TIME units to milliseconds (taken from CRefTime)
///////////////////////////////////////////////////////////////////////

inline LONG RefTimeToMsec(const REFERENCE_TIME& time)
{
	return (LONG)(time / (ONE_SECOND / ONE_MSEC));
}

///////////////////////////////////////////////////////////////////////
// RefTimeToSeconds
// Converts reference time (100 nanosecond units) to floating-point seconds.
///////////////////////////////////////////////////////////////////////

inline double RefTimeToSeconds(const REFERENCE_TIME& rt)
{
    return double(rt) / double(ONE_SECOND);
}

///////////////////////////////////////////////////////////////////////
// SecondsToRefTime
// Converts seconds to reference time.
///////////////////////////////////////////////////////////////////////

inline REFERENCE_TIME SecondsToRefTime(const double& sec)
{
    return (REFERENCE_TIME)(sec * double(ONE_SECOND));
}

///////////////////////////////////////////////////////////////////////
// MsecToRefTime
// Converts milliseconds to reference time.
///////////////////////////////////////////////////////////////////////

inline REFERENCE_TIME MsecToRefTime(const LONG& msec)
{
    return (REFERENCE_TIME)(msec * ONE_MSEC);
}


/********************* GraphEdit functions *********************/


///////////////////////////////////////////////////////////////////////
// Name: SaveGraphFile
// Desc: Save a filter graph to a .grf (GraphEdit) file.
///////////////////////////////////////////////////////////////////////

HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);

///////////////////////////////////////////////////////////////////////
// Name: LoadGraphFile
// Desc: Load a .grf (GraphEdit) file.
///////////////////////////////////////////////////////////////////////

HRESULT LoadGraphFile(IGraphBuilder *pGraph, const WCHAR* wszName);


///////////////////////////////////////////////////////////////////////
// Name: AddGraphToRot
// Desc: Adds a DirectShow filter graph to the Running Object Table,
//       allowing GraphEdit to load a remote filter graph if enabled.
//
// pUnkGraph: Pointer to the Filter Graph Manager
// pdwRegister: Receives a token. Pass this value to RemoveFromRot.
///////////////////////////////////////////////////////////////////////

HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);


///////////////////////////////////////////////////////////////////////
// Name: RemoveGraphFromRot
// Desc: Removes a DirectShow filter graph from the Running Object Table.
//
// dwRegister: Token returned by AddGraphToRot function.
///////////////////////////////////////////////////////////////////////

void RemoveGraphFromRot(DWORD dwRegister);



/********************* Audio and Video Format Functions *********************/


///////////////////////////////////////////////////////////////////////
// Name: CreatePCMAudioType
// Desc: Initialize a PCM audio type with a WAVEFORMATEX format block.
//       (This function does not handle WAVEFORMATEXTENSIBLE formats.)
//
// If the method succeeds, call FreeMediaType to free the format block.
///////////////////////////////////////////////////////////////////////

HRESULT CreatePCMAudioType(
    AM_MEDIA_TYPE& mt,
    WORD nChannels,
    DWORD nSamplesPerSec,
    WORD wBitsPerSample
    );



///////////////////////////////////////////////////////////////////////
// Name: CreateRGBVideoType
// Desc: Initialize an uncompressed RGB video media type.
//       (Allocates the palette table for palettized video)
//
// If the method succeeds, call FreeMediaType to free the format block.
///////////////////////////////////////////////////////////////////////

HRESULT CreateRGBVideoType(
    AM_MEDIA_TYPE& mt,      // Media type to initialize
    WORD iBitDepth,         // Bit depth in bits per pixel
    long Width,             // Width in pixels
    long Height,            // Height in pixels
    double fps              // frames per second
);


///////////////////////////////////////////////////////////////////////
// Name: LetterBoxRect
// Desc: Find the largest rectangle that fits inside rcDest and has
//       the specified aspect ratio.
// 
// aspectRatio: Desired aspect ratio
// rcDest:      Destination rectangle (defines the bounds)
// prcResult:   Pointer to a RECT struct. The method fills in the
//              struct with the letterboxed rectangle.
//
///////////////////////////////////////////////////////////////////////

HRESULT LetterBoxRect(const SIZE &aspectRatio, const RECT &rcDest, 
                      RECT *prcResult);


/********************* Media type functions *********************/


//----------------------------------------------------------------------------
// SetMediaTypeFormat: Sets the format block of a media type
// 
// pmt: Pointer to the AM_MEDIA_TYPE structure. Cannot be NULL
// pBuffer: Pointer to the format block. (Can be NULL)
// cbBuffer: Size of pBuffer in bytes
//
// This function clears the old format block and copies the new
// format block into the media type structure.
//----------------------------------------------------------------------------

HRESULT SetMediaTypeFormatBlock(AM_MEDIA_TYPE *pmt, BYTE *pBuffer, DWORD cbBuffer);



// The following functions are defined in the DirectShow base class library.
// They are redefined here for convenience, because many applications do not
// need to link to the base class library.

#ifndef __STREAMS__


// FreeMediaType: Release the format block for a media type.
inline void FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0)
    {
        CoTaskMemFree((PVOID)mt.pbFormat);
        mt.cbFormat = 0;
        mt.pbFormat = NULL;
    }
    if (mt.pUnk != NULL)
    {
        // Unecessary because pUnk should not be used, but safest.
        mt.pUnk->Release();
        mt.pUnk = NULL;
    }
}

// DeleteMediaType:
// Delete a media type structure that was created on the heap, including the
// format block)
inline void DeleteMediaType(AM_MEDIA_TYPE *pmt)
{
    if (pmt != NULL)
    {
        FreeMediaType(*pmt); 
        CoTaskMemFree(pmt);
    }
}


#endif


