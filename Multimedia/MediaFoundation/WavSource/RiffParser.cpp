//////////////////////////////////////////////////////////////////////////
//
// RiffParser.cpp : RIFF file parsing.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////


#include "WavSource.h"
#include "RiffParser.h"


// Convert FOURCCs to readable strings. (For debugging purposes.)
void FourccString::Fcc2Str(DWORD fcc)
{
    str[0] = (char)(fcc & 0xff);
    str[1] = (char)((fcc >> 8) & 0xff);
    str[2] = (char)((fcc >> 16) & 0xff);
    str[3] = (char)(fcc >> 24);
    str[4] = 0;
}


///

CRiffChunk::CRiffChunk()
{
    this->fcc = 0;
    this->cb = 0;
}

CRiffChunk::CRiffChunk(const RIFFCHUNK& c)
{
    this->fcc = c.fcc;
    this->cb = c.cb;
}




//-------------------------------------------------------------------
// Name: CreateInstance
// Description: Static method to create an instance of the parser.
//
// pStream:         Bytestream for the RIFF file.
// ppRiffParser:    Receives a ref-counted pointer to the parser.
//-------------------------------------------------------------------

/* static */ HRESULT CRiffParser::Create(IMFByteStream *pStream, CRiffParser **ppRiffParser)
{
    if (ppRiffParser == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    // Create a riff parser for the 'RIFF' container
    CRiffParser *pParser = new CRiffParser(
        pStream, FOURCC_RIFF, 0, hr);

    if (pParser == NULL)
    {
        return E_OUTOFMEMORY;
    }

    if (FAILED(hr))
    {
        delete pParser;
    }
    else
    {
        *ppRiffParser = pParser;
        (*ppRiffParser)->AddRef();
    }

    return hr;
}



ULONG CRiffParser::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}


ULONG CRiffParser::Release()
{
    assert(m_nRefCount >= 0);
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // Return the temporary variable, not the member
    // variable, for thread safety.
    return uCount;
}

//-------------------------------------------------------------------
// CRiffParser constructor
//
// pStream: Stream to read from RIFF file
// id:  FOURCC of the RIFF container. Should be 'RIFF' or 'LIST'.
// cbStartofContainer:  Start of the container, as an offset into the stream.
// hr:  Receives the success or failure of the constructor
//-------------------------------------------------------------------

CRiffParser::CRiffParser(IMFByteStream *pStream, FOURCC id, LONGLONG cbStartOfContainer, HRESULT& hr) :
    m_fccID(id), m_fccType(0), m_nRefCount(0),
    m_llContainerOffset(cbStartOfContainer), m_dwContainerSize(0), m_llCurrentChunkOffset(0),
    m_dwBytesRemaining(0)
{
    if (pStream == NULL)
    {
        hr = E_POINTER;
    }
    else
    {
        m_pStream = pStream;
        m_pStream->AddRef();

        hr = ReadRiffHeader();
    }
}

CRiffParser::~CRiffParser()
{
    SAFE_RELEASE(m_pStream);
}


//-------------------------------------------------------------------
// Name: ReadRiffHeader
// Description: 
// Read the container header section. (The 'RIFF' or 'LIST' header.)
//
// This method verifies the header is well-formed and caches the
// container's FOURCC type.
//-------------------------------------------------------------------

HRESULT CRiffParser::ReadRiffHeader()
{
    // Riff chunks must be WORD aligned
    if (!IsAligned(m_llContainerOffset, 2))
    {
        return E_INVALIDARG;
    }

    // Offset must be positive.
    if (m_llContainerOffset < 0)
    {
        return E_INVALIDARG;
    }

    // Offset + the size of header must not overflow.
    if (MAXLONGLONG - m_llContainerOffset <= sizeof(RIFFLIST))
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;
    RIFFLIST header;
    ULONG cbRead = 0;


    // Seek to the start of the container.
    hr = m_pStream->SetCurrentPosition(m_llContainerOffset);
    
    // Read the header. 

    if (SUCCEEDED(hr))
    {
        hr = m_pStream->Read((BYTE*)&header, sizeof(header), &cbRead); 
    };

    if (SUCCEEDED(hr))
    {
        // Make sure we read the number of bytes we expected.
        if (cbRead != sizeof(header))
        {
            hr = E_INVALIDARG;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Make sure the header ID matches what the caller expected.
        if (header.fcc != m_fccID)
        {
            hr = E_INVALIDARG;
        }
    }

    if (SUCCEEDED(hr))
    {
        // The size given in the RIFF header does not include the 8-byte header.
        // However, our m_llContainerOffset is the offset from the start of the
        // header. Therefore our container size = listed size + size of header.

        m_dwContainerSize = header.cb + sizeof(RIFFCHUNK); 
        m_fccType = header.fccListType;

        // Start of the first chunk = start of container + size of container header
        m_llCurrentChunkOffset = m_llContainerOffset + sizeof(RIFFLIST);

    }

    if (SUCCEEDED(hr))
    {
        hr = ReadChunkHeader();
    }

    return hr;
}

//-------------------------------------------------------------------
// Name: ReadChunkHeader
// Description: 
// Reads the chunk header. Caller must ensure that the current file 
// pointer is located at the start of the chunk header.
//-------------------------------------------------------------------

HRESULT CRiffParser::ReadChunkHeader()
{
    // Offset + the size of header must not overflow.
    if (MAXLONGLONG - m_llCurrentChunkOffset <= sizeof(RIFFCHUNK))
    {
        return E_INVALIDARG;
    }

    ULONG cbRead;
    HRESULT hr = m_pStream->Read((BYTE*)&m_chunk, sizeof(RIFFCHUNK), &cbRead); 
    
    if (SUCCEEDED(hr))
    {
        // Make sure we got the number of bytes we expected.
        if (cbRead != sizeof(RIFFCHUNK))
        {
            hr = E_INVALIDARG;
        }
    }

    if (SUCCEEDED(hr))
    {
        m_dwBytesRemaining = m_chunk.DataSize();
    }


    return hr;
}

//-------------------------------------------------------------------
// Name: MoveToNextChunk
// Description: 
// Advance to the start of the next chunk and read the chunk header. 
//-------------------------------------------------------------------

HRESULT CRiffParser::MoveToNextChunk()
{
    // chunk offset is always bigger than container offset,
    // and both are always non-negative.
    assert(m_llCurrentChunkOffset > m_llContainerOffset);
    assert(m_llCurrentChunkOffset >= 0);
    assert(m_llContainerOffset >= 0);


    HRESULT hr = S_OK;
    ULONG cbRead = 0;

    // Update current chunk offset to the start of the next chunk
    m_llCurrentChunkOffset = m_llCurrentChunkOffset + ChunkActualSize();


    // Are we at the end?
    if ((m_llCurrentChunkOffset - m_llContainerOffset) >= m_dwContainerSize)
    {
        return E_FAIL;
    }

    // Current chunk offset + size of current chunk
    if (MAXLONGLONG - m_llCurrentChunkOffset <= ChunkActualSize())
    {
        return E_INVALIDARG;
    }
    
    // Seek to the start of the chunk.
    hr = m_pStream->SetCurrentPosition(m_llCurrentChunkOffset);

    // Read the header. 
    if (SUCCEEDED(hr))
    {
        hr = ReadChunkHeader();
    };


    // This chunk cannot be any larger than (container size - (chunk offset - container offset) )

    if (SUCCEEDED(hr))
    {
        LONGLONG maxChunkSize = (LONGLONG)m_dwContainerSize - (m_llCurrentChunkOffset - m_llContainerOffset);

        if (maxChunkSize < ChunkActualSize())
        {
            hr = E_INVALIDARG;
        }
    }

    if (SUCCEEDED(hr))
    {
        m_dwBytesRemaining = m_chunk.DataSize();
    }

    return hr;
}


//-------------------------------------------------------------------
// Name: EnumerateChunksInList
// Description: Return a parser for a LIST. 
//-------------------------------------------------------------------

HRESULT CRiffParser::EnumerateChunksInList( CRiffParser** ppParser )
{
    if (ppParser == NULL)
    {
        return E_POINTER;
    }

    if (!m_chunk.IsList())
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;

    CRiffParser *pRiff = new CRiffParser(m_pStream, FOURCC_LIST, m_llCurrentChunkOffset, hr);
    if (pRiff == NULL)
    {
        hr = E_OUTOFMEMORY;
    }


    if (SUCCEEDED(hr))
    {
        *ppParser = pRiff;
        (*ppParser)->AddRef();
    }
    else
    {
        SAFE_DELETE(pRiff);
    }


    return hr;
}


//-------------------------------------------------------------------
// Name: MoveToChunkOffset
// Description: 
// Move the file pointer to a byte offset from the start of the 
// current chunk. 
//-------------------------------------------------------------------

HRESULT CRiffParser::MoveToChunkOffset(DWORD dwOffset)
{
    if (dwOffset > m_chunk.DataSize())
    {
        return E_INVALIDARG;
    }

    HRESULT hr = m_pStream->SetCurrentPosition(m_llCurrentChunkOffset + dwOffset + sizeof(RIFFCHUNK));
    if (SUCCEEDED(hr))
    {
        m_dwBytesRemaining = m_chunk.DataSize() - dwOffset;
    }
    return hr;
}


//-------------------------------------------------------------------
// Name: MoveToChunkOffset
// Description: 
// Move the file pointer to the start of the current chunk. 
//-------------------------------------------------------------------

HRESULT CRiffParser::MoveToStartOfChunk()
{
    return MoveToChunkOffset(0);
}


//-------------------------------------------------------------------
// Name: ReadDataFromChunk
// Description: 
// Read data from the current chunk. (Starts at the current file ptr.) 
//-------------------------------------------------------------------

HRESULT CRiffParser::ReadDataFromChunk( BYTE* pData, DWORD dwLengthInBytes )
{
    HRESULT hr = S_OK;

    if (dwLengthInBytes > m_dwBytesRemaining)
    {
        return E_INVALIDARG;
    }

    ULONG cbRead = 0;
    hr = m_pStream->Read(pData, dwLengthInBytes, &cbRead);

    if (SUCCEEDED(hr))
    {
        m_dwBytesRemaining -= cbRead;
    }

    return hr;
}
