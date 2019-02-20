

struct SegmentInfo
{
	SegmentInfo				*pNextSegment;
	MFSequencerElementId    SegmentID;
	WCHAR					SegmentName[MAX_STRING_SIZE];
	MFTIME					hnsSegmentDuration;
	TOPOID					TopologyID;
};

class CSegmentList
{
	SegmentInfo *m_pStart;
	SegmentInfo *m_pEnd;

	unsigned int m_Count;

public:
	CSegmentList();

	virtual ~CSegmentList(void);

	HRESULT AddNewSegmentEntry(MFSequencerElementId 
		SegId, MFTIME SegDuration, 
		TOPOID TopoId, 
		const WCHAR *szSegmentName);
	
	HRESULT DeleteSegmentEntry(MFSequencerElementId SegId);
	
	HRESULT GetSegmentEntryInfo(
		MFSequencerElementId SegmentID, 
		MFTIME *phnsSegmentDuration, 
		WCHAR *szSegmentURL, 
		DWORD dwSize);
	
	HRESULT GetSegmentIDByTopoID(
		TOPOID TopologyID, 
		MFSequencerElementId *SegmentID);
	
	int GetCount(){return m_Count;};
	
	HRESULT GetLastSegmentId(MFSequencerElementId *SegmentId);
};
