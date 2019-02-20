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

// SAFE_RELEASE macro.
// Releases a COM pointer if the pointer is not NULL, and sets the pointer to NULL.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif


namespace MediaFoundationSamples
{
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


#ifdef _DEBUG

    //--------------------------------------------------------------------------------------
    // Debug logging functions
    // Description: Contains debug logging functions.
    //
    // The TRACE_INIT, TRACE, and TRACE_CLOSE macros are mapped to the logging functions
    // in debug builds, and defined as nothing in retail builds.
    //--------------------------------------------------------------------------------------


    HRESULT DebugLog_Initialize(const WCHAR *sFileName);
    void    DebugLog_Trace(const WCHAR *sFormatString, ...);
    void    DebugLog_Close();

    #define TRACE_INIT(x) DebugLog_Initialize(x)
    #define TRACE(x) DebugLog_Trace x
    #define TRACE_CLOSE() DebugLog_Close()

    // Log HRESULTs on failure.
    #define LOG_IF_FAILED(x,hr) if (FAILED(hr)) { TRACE((L"%s hr=0x%X", x, hr)); }

#else

    #define TRACE_INIT(x) 
    #define TRACE(x) 
    #define TRACE_CLOSE() 

    #define LOG_IF_FAILED(x, hr)

#endif


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




    //////////////////////////////////////////////////////////////////////////
    //  CAsyncCallback [template]
	//
    //  Description: 
	//  Helper class that routes IMFAsyncCallback::Invoke calls to a class
	//  method on the parent class.
	//
	//  Usage:
	//  Add this class as a member variable. In the parent class constructor,
	//  initialize the CAsyncCallback class like this:
	//  	m_cb(this, &CYourClass::OnInvoke)
	//  where
	//      m_cb       = CAsyncCallback object
	//      CYourClass = parent class
	//      OnInvoke   = Method in the parent class to receive Invoke calls.
	//
	//  The parent's OnInvoke method (you can name it anything you like) must
	//  have a signature that matches the InvokeFn typedef below.
    //////////////////////////////////////////////////////////////////////////

	// T: Type of the parent object
	template<class T>
	class CAsyncCallback : public IMFAsyncCallback
	{
	public: 
		typedef HRESULT (T::*InvokeFn)(IMFAsyncResult *pAsyncResult);

		CAsyncCallback(T *pParent, InvokeFn fn) : m_pParent(pParent), m_pInvokeFn(fn)
		{
		}

		// IUnknown
		STDMETHODIMP_(ULONG) AddRef() { 
			// Delegate to parent class.
			return m_pParent->AddRef(); 
		}
		STDMETHODIMP_(ULONG) Release() { 
			// Delegate to parent class.
			return m_pParent->Release(); 
		}
		STDMETHODIMP QueryInterface(REFIID iid, void** ppv)
		{
			if (!ppv)
			{
				return E_POINTER;
			}
			if (iid == __uuidof(IUnknown))
			{
				*ppv = static_cast<IUnknown*>(static_cast<IMFAsyncCallback*>(this));
			}
			else if (iid == __uuidof(IMFAsyncCallback))
			{
				*ppv = static_cast<IMFAsyncCallback*>(this);
			}
			else
			{
				return E_NOINTERFACE;
			}
			AddRef();
			return S_OK;
		}


		// IMFAsyncCallback methods
		STDMETHODIMP GetParameters(DWORD*, DWORD*)
		{
			// Implementation of this method is optional.
			return E_NOTIMPL;
		}

		STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult)
		{
			return (m_pParent->*m_pInvokeFn)(pAsyncResult);
		}

		T *m_pParent;
		InvokeFn m_pInvokeFn;
	};


} // namespace MediaFoundationSamples