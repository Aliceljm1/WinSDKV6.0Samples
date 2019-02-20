//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
//
// CSampleProvider implements ICredentialProvider, which is the main
// interface that logonUI uses to decide which tiles to display.
// In this sample, we have decided to show two tiles, one for
// Administrator and one for Guest.  You will need to decide what
// tiles make sense for your situation.  Can you enumerate the
// users who will use your method to log on?  Or is it better
// to provide a tile where they can type in their username?
// Does the user need to interact with something other than the
// keyboard before you can recognize which user it is (such as insert 
// a smartcard)?  We call these "event driven" credential providers.  
// We suggest that such credential providers first provide one basic tile which
// tells the user what to do ("insert your smartcard").  Once the
// user performs the action, then you can callback into LogonUI to
// tell it that you have new tiles, and include a tile that is specific
// to the user that the user can then interact with if necessary.

#include <credentialprovider.h>
#include "CSampleCredential.h"
#include "guid.h"
#define NUM_CREDENTIALS 2

class CSampleProvider : public ICredentialProvider
{
  public:
    // IUnknown
    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&_cRef);
    }
    
    STDMETHOD_(ULONG, Release)()
    {
        LONG cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }

    STDMETHOD (QueryInterface)(REFIID riid, void** ppv)
    {
		HRESULT hr;
        if (ppv)
        {
            if (IID_IUnknown == riid || 
                IID_ICredentialProvider == riid)
            {
                *ppv = static_cast<IUnknown*>(this);
                reinterpret_cast<IUnknown*>(*ppv)->AddRef();
                hr = S_OK;
            }
            else
            {
                *ppv = NULL;
                hr = E_NOINTERFACE;
            }
        }
        else
        {
            hr = E_INVALIDARG;
        }
        return hr;
    }

  public:
    IFACEMETHODIMP SetUsageScenario(CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus, DWORD dwFlags);
    IFACEMETHODIMP SetSerialization(const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs);

    IFACEMETHODIMP Advise(ICredentialProviderEvents* pcpe, UINT_PTR upAdviseContext);
    IFACEMETHODIMP UnAdvise();

    IFACEMETHODIMP GetFieldDescriptorCount(DWORD* pdwCount);
    IFACEMETHODIMP GetFieldDescriptorAt(DWORD dwIndex,  CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd);

    IFACEMETHODIMP GetCredentialCount(DWORD* pdwCount,
                                      DWORD* pdwDefault,
                                      BOOL* pbAutoLogonWithDefault);
    IFACEMETHODIMP GetCredentialAt(DWORD dwIndex, 
                                   ICredentialProviderCredential** ppcpc);

    friend HRESULT CSampleProvider_CreateInstance(REFIID riid, void** ppv);

  protected:
    CSampleProvider();
    __override ~CSampleProvider();
    
  private:
    
    HRESULT _EnumerateOneCredential(DWORD dwCredientialIndex,
                                    PCWSTR pwzUsername
                                    );

    // Create our array of credentials.
    HRESULT _EnumerateCredentials();

private:
    LONG                                    _cRef;
    CSampleCredential*                       _rgpCredentials[NUM_CREDENTIALS]; // Pointers to the credentials which will be enumerated by this Provider.
};

// CSampleProvider ////////////////////////////////////////////////////////

CSampleProvider::CSampleProvider():
    _cRef(1)
{
    ZeroMemory(_rgpCredentials, sizeof(_rgpCredentials));
    DllAddRef();
}

CSampleProvider::~CSampleProvider()
{
    for (int i = 0; i < ARRAYSIZE(_rgpCredentials); i++)
    {
        if (_rgpCredentials[i])
        {
            _rgpCredentials[i]->Release();
        }
    }
    DllRelease();
}

//
// SetUsageScenario is the provider's cue that it's going to be asked for tiles
// in a subsequent call.  In this sample we have chosen to precreate the credentials 
// for the usage scenario passed in cpus instead of saving off cpus and only creating
// the credentials when we're asked to.
// This sample only handles the logon and unlock scenarios as those are the most common.
//
HRESULT CSampleProvider::SetUsageScenario(
    CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus,
    DWORD dwFlags
    )
{
    UNREFERENCED_PARAMETER(dwFlags);
    HRESULT hr;

    switch (cpus)
    {
    case CPUS_LOGON:
    case CPUS_UNLOCK_WORKSTATION:       
        hr = _EnumerateCredentials();
        break;

    case CPUS_CHANGE_PASSWORD:
    case CPUS_CREDUI:
		hr = E_NOTIMPL;
		break;

    default:
        hr = E_INVALIDARG;
        break;
    }

    return hr;
}

//
// SetSerialization takes the kind of buffer that you would normally return to LogonUI for
// an authentication attempt.  It's the opposite of ICredentialProviderCredential::GetSerialization.
// GetSerialization is implement by a credential and serializes that credential.  Instead,
// SetSerialization takes the serialization and uses it to create a tile.
//
// SetSerialization is called for two main scenarios.  The first scenario is in the credui case
// where it is prepopulating a tile with credentials that the user chose to store in the OS.
// The second situation is in a remote logon case where the remote client may wish to 
// prepopulate a tile with a username, or in some cases, completely populate the tile and
// use it to logon without showing any UI.
//
// SetSerialization is currently optional, which is why it's not implemented in this sample.  
// If that changes in the future, we will update the sample.
// 
STDMETHODIMP CSampleProvider::SetSerialization(
    const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs
    )
{
    UNREFERENCED_PARAMETER(pcpcs);
    return E_NOTIMPL;
}

//
// Called by LogonUI to give you a callback.  Providers often use the callback if they
// some event would cause them to need to change the set of tiles that they enumerated
//
HRESULT CSampleProvider::Advise(
    ICredentialProviderEvents* pcpe,
    UINT_PTR upAdviseContext
    )
{
    UNREFERENCED_PARAMETER(pcpe);
    UNREFERENCED_PARAMETER(upAdviseContext);

    return E_NOTIMPL;
}

//
// Called by LogonUI when the ICredentialProviderEvents callback is no longer valid.
//
HRESULT CSampleProvider::UnAdvise()
{
    return E_NOTIMPL;
}

//
// Called by LogonUI to determine the number of fields in your tiles.  This
// does mean that all your tiles must have the same number of fields.
// This number must include both visible and invisible fields. If you want a tile
// to have different fields from the other tiles you enumerate for a given usage
// scenario you must include them all in this count and then hide/show them as desired 
// using the field descriptors.
//
HRESULT CSampleProvider::GetFieldDescriptorCount(
    DWORD* pdwCount
    )
{
    *pdwCount = SFI_NUM_FIELDS;

    return S_OK;
}

//
// Gets the field descriptor for a particular field
//
HRESULT CSampleProvider::GetFieldDescriptorAt(
    DWORD dwIndex, 
    CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
    )
{    
    HRESULT hr;

    // Verify dwIndex is a valid field.
    if (dwIndex < SFI_NUM_FIELDS && ppcpfd)
    {
        hr = FieldDescriptorCoAllocCopy(s_rgCredProvFieldDescriptors[dwIndex], ppcpfd);
    }
    else
    { 
        hr = E_INVALIDARG;
    }

    return hr;
}

//
// Sets pdwCount to the number of tiles that we wish to show at this time.
// Sets pdwDefault to the index of the tile which should be used as the default.
// The default tile is the tile which will be shown in the zoomed view by default. If 
// more than one provider specifies a default tile the behavior is undefined. 
// If *pbAutoLogonWithDefault is TRUE, LogonUI will immediately call GetSerialization
// on the credential you've specified as the default and will submit that credential
// for authentication without showing any further UI.
//
HRESULT CSampleProvider::GetCredentialCount(
    DWORD* pdwCount,
    DWORD* pdwDefault,
    BOOL* pbAutoLogonWithDefault
    )
{
    *pdwCount = ARRAYSIZE(_rgpCredentials); // This provider always has the same number of credentials.
    *pdwDefault = 0;
    *pbAutoLogonWithDefault = FALSE;

    return S_OK;
}

//
// Returns the credential at the index specified by dwIndex. This function is called by logonUI to enumerate
// the tiles.
// 
HRESULT CSampleProvider::GetCredentialAt(
    DWORD dwIndex, 
    ICredentialProviderCredential** ppcpc
    )
{
    HRESULT hr;
    if(dwIndex < ARRAYSIZE(_rgpCredentials) && ppcpc)
    {
        hr = _rgpCredentials[dwIndex]->QueryInterface(IID_ICredentialProviderCredential, reinterpret_cast<void**>(ppcpc));
    }
    else
    {
        hr = E_INVALIDARG;
    }
        
    return hr;
}

//
// Creates a Credential with the SFI_USERNAME field's value set to pwzUsername.
//
HRESULT CSampleProvider::_EnumerateOneCredential(
    DWORD dwCredentialIndex,
    PCWSTR pwzUsername
    )
{
    HRESULT hr;

    // Allocate memory for the new credential.
    CSampleCredential* ppc = new CSampleCredential();
    
    if (ppc)
    {
        // Set the Field State Pair and Field Descriptors for ppc's fields
        // to the defaults (s_rgCredProvFieldDescriptors, and s_rgFieldStatePairs) and the value of SFI_USERNAME
        // to pwzUsername
        hr = ppc->Initialize(s_rgCredProvFieldDescriptors, s_rgFieldStatePairs, pwzUsername);
        
        if (SUCCEEDED(hr))
        {
            _rgpCredentials[dwCredentialIndex] = ppc;
        }
        else
        {
            // release the pointer to account for the local reference.
            ppc->Release();
        }
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

//
// Sets up all the credentials for this provider.
// Since we always show the same tiles, we just set it up
// once.
//
HRESULT CSampleProvider::_EnumerateCredentials()
{
    HRESULT hr = _EnumerateOneCredential(0, L"Administrator");
    if (SUCCEEDED(hr))
    {
        hr = _EnumerateOneCredential(1, L"Guest");
    }
    return hr;
}

HRESULT CSampleProvider_CreateInstance(REFIID riid, void** ppv)
{
    HRESULT hr;

    CSampleProvider* pProvider = new CSampleProvider();

    if (pProvider)
    {
        hr = pProvider->QueryInterface(riid, ppv);
        pProvider->Release();
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }
    
    return hr;
}
