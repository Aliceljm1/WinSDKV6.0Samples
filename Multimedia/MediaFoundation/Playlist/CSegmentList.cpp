#include "MF_SeqSrc_Playback.h"
#include "CSegmentList.h"

//Constructor
CSegmentList::CSegmentList()
{
    m_pStart = NULL;
    m_pEnd = NULL;

    m_Count = 0;
}


//Destructor
CSegmentList::~CSegmentList()
{
    while (m_pStart)
    {
        SegmentInfo *current = m_pStart;

        m_pStart = m_pStart->pNextSegment;
        
        delete current;

        --m_Count;
    }

}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CSegmentList::AddNewSegmentEntry (Public)
//  Description: 
//      Add a segment node.
//  Parameters:
//      SegId: [in] Segment identifier
//      hnsSegDuration: [in] Segment Duration
//      TopoId: [in] Topology identifier
//      szSegmentName: [in] Segment Name
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CSegmentList::AddNewSegmentEntry(
                        MFSequencerElementId SegId, 
                        MFTIME hnsSegDuration, 
                        TOPOID TopoId, 
                        const WCHAR *szSegmentName)
{
    HRESULT hr = S_OK;
    
    SegmentInfo *pNewSegment = new SegmentInfo();

    if (!pNewSegment)
    {
        return E_OUTOFMEMORY;
    }

    pNewSegment->pNextSegment=NULL;

    pNewSegment->SegmentID = SegId;

    pNewSegment->hnsSegmentDuration = hnsSegDuration;

    pNewSegment->TopologyID = TopoId;

    hr = StringCchCopy(pNewSegment->SegmentName, MAX_STRING_SIZE, (STRSAFE_LPCWSTR)szSegmentName);

    if (SUCCEEDED(hr))
    {
        if (!m_pStart)
        {
            m_pStart = pNewSegment;
        }
        else
        {
            m_pEnd->pNextSegment = pNewSegment;
        }
        m_pEnd = pNewSegment;
        
        m_Count++;
    }
    
    return hr;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CSegmentList::DeleteSegmentEntry (Public)
//  Description: 
//      Delete a segment node based on segment identifier.
//  Parameters:
//      SegId: [in] Segment identifier
/////////////////////////////////////////////////////////////////////////////////////////
HRESULT CSegmentList::DeleteSegmentEntry(MFSequencerElementId SegId)
{
    
    if (!m_pStart)
    {
        return E_FAIL;
    }

    if (m_pStart == m_pEnd)
    {
        if (m_pStart->SegmentID == SegId)
        {
            //delete the only node
            delete m_pStart;

            m_Count--;

            //reset the start and the end
            m_pStart = NULL;
            m_pEnd = NULL;

            return S_OK;
        }
        else
        {
            return E_FAIL;
        }
    }


    HRESULT hr = S_OK;

    SegmentInfo *current = m_pStart, *prev = m_pStart;

    while (current)
    {
    
        if(current->SegmentID == SegId)
        {
            //if node is at the beginning of the list
            if (current == m_pStart)
            {
                m_pStart = m_pStart->pNextSegment;
            }   

            //if node is at the end of the list
            else if (current == m_pEnd)
            {
                prev->pNextSegment = NULL;
                
                m_pEnd = prev;
            }

            else
            {
                prev->pNextSegment = current->pNextSegment;
            }
        
            delete current;
            m_Count--;

            return hr;
        }
        
        prev = current; 
        current = current->pNextSegment;

    }

    //segment node not found
    return E_FAIL;
	
}



////////////////////////////////////////////////////////////////////////////////////////
//  Name: CSegmentList::GetSegmentEntryInfo (Public)
//  Description: 
//      Gets segment name and duration based on segment identifier.
//  Parameters:
//      SegID: [in] Segment identifier
//      phnsSegDuration: [out] Segment Duration
//      szSegURL: [out] Segment name
//      dwSize: [in] size of szSegURL
/////////////////////////////////////////////////////////////////////////////////////////

HRESULT CSegmentList::GetSegmentEntryInfo(
                        MFSequencerElementId SegID, 
                        MFTIME *phnsSegDuration, 
                        WCHAR *szSegName, 
                        DWORD dwSize)
{
    if (!m_pStart)
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;
    
    SegmentInfo *current = m_pStart;

    while (current)
    {
        if (current->SegmentID == SegID)
        {
            *phnsSegDuration = current->hnsSegmentDuration;
            hr = StringCchCopy(szSegName, dwSize, current->SegmentName);
            return hr;
        }
        current = current->pNextSegment;
    }
    
    return E_FAIL;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CSegmentList::GetSegmentIDByTopoID (Public)
//  Description: 
//      Gets the segment identifier based on the topology identifier.
//  Parameter:  
//      TopologyID: [in] Topology identifier
//      SegmentID: [out] Receives the segment identifier
/////////////////////////////////////////////////////////////////////////////////////////
HRESULT CSegmentList::GetSegmentIDByTopoID(
                        TOPOID TopologyID, 
                        MFSequencerElementId *SegmentID)
{
    HRESULT hr = S_OK;
    
    SegmentInfo *current = m_pStart;

    while (current)
    {
        if (current->TopologyID == TopologyID)
        {
            *SegmentID = current->SegmentID;
            return hr;
        }
        current = current->pNextSegment;
    }
    //SegmentID not found
    return E_FAIL;
}


////////////////////////////////////////////////////////////////////////////////////////
//  Name: CSegmentList::GetLastSegmentId (Public)
//  Description: 
//      Gets the segment identifier for the last node.
//  Parameter:  
//      pSegmentID: [out] Receives the segment identifier
/////////////////////////////////////////////////////////////////////////////////////////
HRESULT CSegmentList::GetLastSegmentId(MFSequencerElementId *pSegmentId)
{
    HRESULT hr = S_OK;

    if (!m_pEnd)
    {
        return E_FAIL;
    }

    *pSegmentId = m_pEnd->SegmentID;

    return hr;
}
