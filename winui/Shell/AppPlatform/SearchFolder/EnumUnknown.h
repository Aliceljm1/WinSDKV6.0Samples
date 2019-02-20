
//
//  This is an example of a class that extends from IEnumUnknown
//  It is needed to create complicated conditions
//

class CEnumUnknownWrapper : public IEnumUnknown
{
public:
    //
    // IUnknown methods
    //
    IFACEMETHODIMP QueryInterface(REFIID riid,  void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
        
    //
    // IEnumUnknown methods
    //
    IFACEMETHODIMP Next(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched);
    IFACEMETHODIMP Skip(ULONG celt);
    IFACEMETHODIMP Reset();
    IFACEMETHODIMP Clone(IEnumUnknown **ppenum);

    static HRESULT CreateInstance(IUnknown** _ppUnk, ULONG iCount, REFIID riid, void **ppv);

protected:
    CEnumUnknownWrapper();
    CEnumUnknownWrapper(IUnknown **_ppUnkIn, ULONG iCount);
    ~CEnumUnknownWrapper();
    IUnknown **_ppUnk;
    ULONG _iCurrent;
    ULONG _iCapacity;
    LONG _iRef;
};

//
//  QueryInterface Method.  Supports IEnumUnknown and IUnknown
//
IFACEMETHODIMP CEnumUnknownWrapper::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CEnumUnknownWrapper, IEnumUnknown),
        QITABENT(CEnumUnknownWrapper, IUnknown),
        { 0 },
    };

    return QISearch(this, qit, riid, ppv);
}

//
//  Adds another reference to the object
//
IFACEMETHODIMP_(ULONG) CEnumUnknownWrapper::AddRef()
{
    return InterlockedIncrement(&_iRef);
}

//
//  Releases a reference from the object
//
IFACEMETHODIMP_(ULONG) CEnumUnknownWrapper::Release()
{
    LONG cRef = InterlockedDecrement(&_iRef);

    if (!cRef)
    {
        delete this;
    }

    return cRef;
}

//
//  If celt results are available they will be put in rgelt and S_OK will be returned
//  Otherwise as many results as are available up to celt will be put in rgelt, and S_FALSE will be returned
//  pceltFetched is an out param that will tell how many items were successfully fetched
//
IFACEMETHODIMP CEnumUnknownWrapper::Next(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched)
{
    HRESULT hr = rgelt != NULL ? S_OK : E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        ULONG i = 0;
        
        *rgelt = NULL;
    
        for (i; i < celt; i++)
        {
            if (i + _iCurrent < _iCapacity)
            {
                //
                // We have more items to give, insert it in the out array
                //
                IUnknown_Set(&rgelt[i], _ppUnk[i + _iCurrent]);
            }
            else
            {
                //
                // We are all out of items
                //
                hr = S_FALSE;
                break;
            }
        }

        if (pceltFetched != NULL)
        {
            *pceltFetched = i;
        }

        _iCurrent += i;
    }

    return hr;
}

//
//  This tells the enumorator to skip the next celt items
//  If celt were skipped S_OK is returned, otherwise S_FALSE
//
IFACEMETHODIMP CEnumUnknownWrapper::Skip(ULONG celt)
{
    _iCurrent = (celt + _iCurrent) < _iCapacity ? _iCurrent + celt : _iCapacity - 1; 

    return (celt + _iCurrent) < _iCapacity ? S_OK : S_FALSE;
}

//
//  This resets the enumorator
//
IFACEMETHODIMP CEnumUnknownWrapper::Reset()
{
    _iCurrent = 0; 

    return S_OK;
}

//
//  This clones the current object and puts it in ppenum
//
IFACEMETHODIMP CEnumUnknownWrapper::Clone(IEnumUnknown **ppenum)
{ 
    return CreateInstance(_ppUnk, _iCapacity, IID_PPV_ARGS(ppenum)); 
}


//
// Creates a new instance of CEnumUnknownWrapper
//
HRESULT CEnumUnknownWrapper::CreateInstance(IUnknown **ppUnknown, ULONG iCount, REFIID riid, void **ppv)
{
    HRESULT hr = ppUnknown != NULL ? S_OK : E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        CEnumUnknownWrapper *penum = new CEnumUnknownWrapper(ppUnknown, iCount);
     
        if (penum != NULL)
        {
            hr = penum->QueryInterface(riid, ppv);
            penum->Release();
        }
        else
        {
            hr = E_OUTOFMEMORY;
        }
    }

    return hr;
}

//
// Constructor to create the object from an array of IUnknowns
//
CEnumUnknownWrapper::CEnumUnknownWrapper(IUnknown **ppUnknown, ULONG iCount) : 
    _iRef(1),
    _ppUnk(NULL),
    _iCurrent(0),
    _iCapacity(0)
{
    _ppUnk = new IUnknown *[iCount];
    
    if (_ppUnk)
    {
        ZeroMemory(_ppUnk, sizeof(IUnknown*) * iCount);
        _iCurrent = 0;
        _iCapacity = iCount;

        for (ULONG i = 0; i < iCount; i++)
        {
            IUnknown_Set(&_ppUnk[i], ppUnknown[i]);
        }
    }
}

//
//  Destructor that destroys the object
//
CEnumUnknownWrapper::~CEnumUnknownWrapper()
{ 
    for (ULONG i = 0; i < _iCapacity; i++)
    {
        if (_ppUnk[i] != NULL)
        {
            _ppUnk[i]->Release();
        }
    }
    delete [] _ppUnk; 
}
