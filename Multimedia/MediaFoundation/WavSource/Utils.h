//////////////////////////////////////////////////////////////////////////
//
// Utils.h : Misc. helper classes.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

//////////////////////////////////////////////////////////////////////////
//  CCritSec
//  Description: Wraps a critical section.
//////////////////////////////////////////////////////////////////////////

class CCritSec
{
private:
    CRITICAL_SECTION m_criticalSection;
public:
    CCritSec();
    ~CCritSec();

    void Lock();
    void Unlock();
};


//////////////////////////////////////////////////////////////////////////
//  CAutoLock
//  Description: Provides automatic locking and unlocking of a 
//               of a critical section.
//////////////////////////////////////////////////////////////////////////

class CAutoLock
{
private:
    CCritSec *m_pCriticalSection;
public:
    CAutoLock(CCritSec& crit);
    ~CAutoLock();
};



///////////////////////////////////////////////////////////////////////
// Name: AreCOMObjectsEqual [template]
// Desc: Tests two COM pointers for equality.
//
// Return values:
//   S_OK: They point to the same object.
//   S_FALSE: They do not point to the same object.
//   error code
///////////////////////////////////////////////////////////////////////

template <class T1, class T2>
HRESULT AreCOMObjectsEqual(T1 *p1, T2 *p2)
{
    HRESULT hr = E_FAIL;

    if (p1 == NULL && p2 == NULL)
    {
        hr = S_OK;  // Both are NULL
    }
    else if (p1 != NULL && p2 != NULL)
    {
        // Both are not NULL. Compare IUnknowns.
        IUnknown *pUnk1 = NULL;
        IUnknown *pUnk2 = NULL;

        hr = p1->QueryInterface(IID_IUnknown, (void**)&pUnk1);
        if (SUCCEEDED(hr))
        {
            hr = p2->QueryInterface(IID_IUnknown, (void**)&pUnk2);
            if (SUCCEEDED(hr))
            {
                hr = (pUnk1 == pUnk2 ? S_OK : S_FALSE);
                pUnk2->Release();
            }
            pUnk1->Release();
        }
    }
    else 
    {
        // One is NULL, the other is non-NULL
        hr = S_FALSE;
    }
    return hr;
}
