//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
//
//

#include "CSampleCredential.h"
#include "guid.h"

// CSampleCredential ////////////////////////////////////////////////////////

CSampleCredential::CSampleCredential():
    _cRef(1),
    _pCredProvCredentialEvents(NULL)
{
    ZeroMemory(_rgCredProvFieldDescriptors, sizeof(_rgCredProvFieldDescriptors));
    ZeroMemory(_rgFieldStatePairs, sizeof(_rgFieldStatePairs));
    ZeroMemory(_rgFieldStrings, sizeof(_rgFieldStrings));
    DllAddRef();
}

CSampleCredential::~CSampleCredential()
{
    if (_rgFieldStrings[SFI_PASSWORD])
    {
        //CoTaskMemFree (below) deals with NULL, but wcslen does not.
        SecureZeroMemory(_rgFieldStrings[SFI_PASSWORD], wcslen(_rgFieldStrings[SFI_PASSWORD]) * sizeof(*_rgFieldStrings[SFI_PASSWORD]));
    }
    for (int i = 0; i < ARRAYSIZE(_rgFieldStrings); i++)
    {
        CoTaskMemFree(_rgFieldStrings[i]);
        CoTaskMemFree(_rgCredProvFieldDescriptors[i].pszLabel);
    }
    DllRelease();
}


//
// Initializes one credential with the field information passed in.
// Set the value of the SFI_USERNAME field to pwzUsername.
//
HRESULT CSampleCredential::Initialize(
    const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* rgcpfd,
    const FIELD_STATE_PAIR* rgfsp,
    PCWSTR pwzUsername   
    )
{
    HRESULT hr = S_OK;

    // Copy the field descriptors for each field. This is useful if you want to vary the field
    // descriptors based on what Usage scenario the credential was created for.
    for (DWORD i = 0; SUCCEEDED(hr) && i < ARRAYSIZE(_rgCredProvFieldDescriptors); i++)
    {
        _rgFieldStatePairs[i] = rgfsp[i];
        hr = FieldDescriptorCopy(rgcpfd[i], &_rgCredProvFieldDescriptors[i]);
    }

    // Initialize the String value of all the fields
    if (SUCCEEDED(hr))
    {
        hr = SHStrDupW(pwzUsername, &_rgFieldStrings[SFI_USERNAME]);
    }
    if (SUCCEEDED(hr))
    {
        hr = SHStrDupW(L"", &_rgFieldStrings[SFI_PASSWORD]);
    }
    if (SUCCEEDED(hr))
    {
        hr = SHStrDupW(L"Submit", &_rgFieldStrings[SFI_SUBMIT_BUTTON]);
    }

    return S_OK;
}

//LogonUI calls this in order to give us a callback in case we need to notify it of anything
HRESULT CSampleCredential::Advise(
    ICredentialProviderCredentialEvents* pcpce
    )
{
    if (_pCredProvCredentialEvents != NULL)
    {
        _pCredProvCredentialEvents->Release();
    }
    _pCredProvCredentialEvents = pcpce;
    _pCredProvCredentialEvents->AddRef();
    return S_OK;
}

//LogonUI calls this to tell us to release the callback
HRESULT CSampleCredential::UnAdvise()
{
    if (_pCredProvCredentialEvents)
    {
        _pCredProvCredentialEvents->Release();
    }
    _pCredProvCredentialEvents = NULL;
    return S_OK;
}

//LogonUI calls this function when our tile is selected (zoomed)
//If you simply want fields to show/hide based on the selected state,
//there's no need to do anything here - you can set that up in the 
//field definitions.  But if you want to do something
//more complicated, like change the contents of a field when the tile is
//selected, you would do it here.
HRESULT CSampleCredential::SetSelected(BOOL* pbAutoLogon)  
{
    *pbAutoLogon = FALSE;  

    return S_OK;
}

//Similarly to SetSelected, LogonUI calls this when your tile was selected
//and now no longer is.  The most common thing to do here (which we do below)
//is to clear out the password field.
HRESULT CSampleCredential::SetDeselected()
{
    if (_rgFieldStrings[SFI_PASSWORD])
    {
        SecureZeroMemory(_rgFieldStrings[SFI_PASSWORD], wcslen(_rgFieldStrings[SFI_PASSWORD]) * sizeof(*_rgFieldStrings[SFI_PASSWORD]));
        CoTaskMemFree(_rgFieldStrings[SFI_PASSWORD]);
        HRESULT    hr = SHStrDupW(L"", &_rgFieldStrings[SFI_PASSWORD]);

        if (SUCCEEDED(hr) && _pCredProvCredentialEvents)
        {
            _pCredProvCredentialEvents->SetFieldString(this, SFI_PASSWORD, _rgFieldStrings[SFI_PASSWORD]);
        }
        return hr;
    }
    else
    {
        return S_OK;
    }
}

//
// Get info for a particular field of a tile. Called by logonUI to get information to display the tile.
//
HRESULT CSampleCredential::GetFieldState(
    DWORD dwFieldID,
    CREDENTIAL_PROVIDER_FIELD_STATE* pcpfs,
    CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE* pcpfis
    )
{
    HRESULT hr;
    
    if (dwFieldID < ARRAYSIZE(_rgFieldStatePairs) && pcpfs && pcpfis)
    {
        *pcpfs = _rgFieldStatePairs[dwFieldID].cpfs;
        *pcpfis = _rgFieldStatePairs[dwFieldID].cpfis;

        hr = S_OK;
    }
    else
    {
        hr = E_INVALIDARG;
    }
    return hr;
}

//
// Sets ppwz to the string value of the field at the index dwFieldID
//
HRESULT CSampleCredential::GetStringValue(
    DWORD dwFieldID, 
    PWSTR* ppwz
    )
{
    HRESULT hr;

    // Check to make sure dwFieldID is a legitimate index
    if (dwFieldID < ARRAYSIZE(_rgCredProvFieldDescriptors) && ppwz) 
    {
        // Make a copy of the string and return that. The caller
        // is responsible for freeing it.
        hr = SHStrDupW(_rgFieldStrings[dwFieldID], ppwz);
    }
    else
    {
        hr = E_INVALIDARG;
    }

    return hr;
}

//
// Get the image to show in the user tile
//
HRESULT CSampleCredential::GetBitmapValue(
    DWORD dwFieldID, 
    HBITMAP* phbmp
    )
{
    HRESULT hr;
    if (SFI_TILEIMAGE == dwFieldID && phbmp)
    {
        HBITMAP hbmp = LoadBitmap(HINST_THISDLL, MAKEINTRESOURCE(IDB_TILE_IMAGE));
        hr = HRESULT_FROM_WIN32(GetLastError());
        if (SUCCEEDED(hr))
        {
            hr = S_OK;
            *phbmp = hbmp;
        }
    }
    else
    {
        hr = E_INVALIDARG;
    }

    return hr;
}


//
// Sets pdwAdjacentTo to the index of the field the submit button should be 
// adjacent to. We recommend that the submit button is placed next to the last
// field which the user is required to enter information in. Optional fields
// should be below the submit button.
//
HRESULT CSampleCredential::GetSubmitButtonValue(
    DWORD dwFieldID,
    DWORD* pdwAdjacentTo
    )
{
    HRESULT hr;

    if (SFI_SUBMIT_BUTTON == dwFieldID && pdwAdjacentTo)
    {
        // pdwAdjacentTo is a pointer to the fieldID you want the submit button to appear next to.
        *pdwAdjacentTo = SFI_PASSWORD;
        hr = S_OK;
    }
    else
    {
        hr = E_INVALIDARG;
    }
    return hr;
}

//
// Sets the value of a field which can accept a string as a value.
// This is called on each keystroke when a user types into an edit field
//
HRESULT CSampleCredential::SetStringValue(
    DWORD dwFieldID, 
    PCWSTR pwz      
    )
{
    HRESULT hr;

    if (dwFieldID < ARRAYSIZE(_rgCredProvFieldDescriptors) && 
       (CPFT_EDIT_TEXT == _rgCredProvFieldDescriptors[dwFieldID].cpft || 
		CPFT_PASSWORD_TEXT == _rgCredProvFieldDescriptors[dwFieldID].cpft)) 
    {
        PWSTR* ppwzStored = &_rgFieldStrings[dwFieldID];
        CoTaskMemFree(*ppwzStored);

        hr = SHStrDupW(pwz, ppwzStored);
    }
    else
    {
        hr = E_INVALIDARG;
    }

    return hr;
}

//------------- 
// The following methods are for logonUI to get the values of various UI elements and then communicate
// to the credential about what the user did in that field.  However, these methods are not implemented
// because our tile doesn't contain these types of UI elements
HRESULT CSampleCredential::GetCheckboxValue(
    DWORD dwFieldID, 
    BOOL* pbChecked,
    PWSTR* ppwzLabel
    )
{
    UNREFERENCED_PARAMETER(dwFieldID);
    UNREFERENCED_PARAMETER(pbChecked);
    UNREFERENCED_PARAMETER(ppwzLabel);

    return E_NOTIMPL;
}

HRESULT CSampleCredential::GetComboBoxValueCount(
    DWORD dwFieldID, 
    DWORD* pcItems, 
    DWORD* pdwSelectedItem
    )
{
    UNREFERENCED_PARAMETER(dwFieldID);
    UNREFERENCED_PARAMETER(pcItems);
    UNREFERENCED_PARAMETER(pdwSelectedItem);
    return E_NOTIMPL;
}

HRESULT CSampleCredential::GetComboBoxValueAt(
    DWORD dwFieldID, 
    DWORD dwItem,
    PWSTR* ppwzItem
    )
{
    UNREFERENCED_PARAMETER(dwFieldID);
    UNREFERENCED_PARAMETER(dwItem);
    UNREFERENCED_PARAMETER(ppwzItem);
    return E_NOTIMPL;
}

HRESULT CSampleCredential::SetCheckboxValue(
    DWORD dwFieldID, 
    BOOL bChecked
    )
{
    UNREFERENCED_PARAMETER(dwFieldID);
    UNREFERENCED_PARAMETER(bChecked);

    return E_NOTIMPL;
}

HRESULT CSampleCredential::SetComboBoxSelectedValue(
    DWORD dwFieldId,
    DWORD dwSelectedItem
    )
{
    UNREFERENCED_PARAMETER(dwFieldId);
    UNREFERENCED_PARAMETER(dwSelectedItem);
    return E_NOTIMPL;
}

HRESULT CSampleCredential::CommandLinkClicked(DWORD dwFieldID)
{
    UNREFERENCED_PARAMETER(dwFieldID);
    return E_NOTIMPL;
}
//------ end of methods for controls we don't have in our tile ----//



//
// Collect the username and password into a serialized credential for the correct usage scenario 
// (logon/unlock is what's demonstrated in this sample).  LogonUI then passes these credentials 
// back to the system to log on.
//
HRESULT CSampleCredential::GetSerialization(
    CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE* pcpgsr,
    CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs, 
    PWSTR* ppwzOptionalStatusText, 
    CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon
    )
{
    UNREFERENCED_PARAMETER(ppwzOptionalStatusText);
    UNREFERENCED_PARAMETER(pcpsiOptionalStatusIcon);

	KERB_INTERACTIVE_LOGON kil;
	ZeroMemory(&kil, sizeof(kil));

    HRESULT hr;

    WCHAR wsz[MAX_COMPUTERNAME_LENGTH+1];
    DWORD cch = ARRAYSIZE(wsz);
    if (GetComputerNameW(wsz, &cch))
    {
        //
        // Initialize the UNICODE_STRINGS to share our username and password strings
        //
        hr = UnicodeStringInitWithString(wsz, &kil.LogonDomainName);

        if (SUCCEEDED(hr))
        {
            hr = UnicodeStringInitWithString(_rgFieldStrings[SFI_USERNAME], &kil.UserName);

            if (SUCCEEDED(hr))
            {
                hr = UnicodeStringInitWithString(_rgFieldStrings[SFI_PASSWORD], &kil.Password);

                if (SUCCEEDED(hr))
                {
                    //
                    // Allocate copies of, and package, the strings in a binary blob
                    //
                    kil.MessageType = KerbInteractiveLogon;
                    hr = KerbInteractiveLogonPack(kil, &pcpcs->rgbSerialization, &pcpcs->cbSerialization);

                    if (SUCCEEDED(hr))
                    {
                        ULONG ulAuthPackage;
                        hr = RetrieveNegotiateAuthPackage(&ulAuthPackage);
                        if (SUCCEEDED(hr))
                        {
                            pcpcs->ulAuthenticationPackage = ulAuthPackage;
                            pcpcs->clsidCredentialProvider = CLSID_CSampleProvider;
 
                            // At this point the credential has created the serialized credential used for logon
                            // By setting this to CPGSR_RETURN_CREDENTIAL_FINISHED we are letting logonUI know
                            // that we have all the information we need and it should attempt to submit the 
                            // serialized credential.
                            *pcpgsr = CPGSR_RETURN_CREDENTIAL_FINISHED;
                        }
                    }
                }
            }
        }
    }
    else
    {
        DWORD dwErr = GetLastError();
        hr = HRESULT_FROM_WIN32(dwErr);
    }

    return hr;
}
struct REPORT_RESULT_STATUS_INFO
{
    NTSTATUS ntsStatus;
    NTSTATUS ntsSubstatus;
    PWSTR     pwzMessage;
    CREDENTIAL_PROVIDER_STATUS_ICON cpsi;
};

//these are currently defined in the ddk, but not the sdk
#ifndef STATUS_LOGON_FAILURE
#define STATUS_LOGON_FAILURE             (0xC000006DL)     // ntsubauth
#endif
#ifndef STATUS_ACCOUNT_RESTRICTION
#define STATUS_ACCOUNT_RESTRICTION       (0xC000006EL)     // ntsubauth
#endif
#ifndef STATUS_ACCOUNT_DISABLED
#define STATUS_ACCOUNT_DISABLED          (0xC0000072L)     // ntsubauth
#endif
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS                   (0x000000000)     // ntsubauth
#endif

static const REPORT_RESULT_STATUS_INFO s_rgLogonStatusInfo[] =
{
    { STATUS_LOGON_FAILURE, STATUS_SUCCESS, L"Incorrect password or username.", CPSI_ERROR, },
    { STATUS_ACCOUNT_RESTRICTION, STATUS_ACCOUNT_DISABLED, L"The account is disabled.", CPSI_WARNING },
};

// ReportResult is completely optional.  Its purpose is to allow a credential to customize the string
// and the icon displayed in the case of a logon failure.  For example, we have chosen to 
// customize the error shown in the case of bad username/password and in the case of the account
// being disabled.
HRESULT CSampleCredential::ReportResult(
    NTSTATUS ntsStatus, 
    NTSTATUS ntsSubstatus,
    PWSTR* ppwzOptionalStatusText, 
    CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon
    )
{
    *ppwzOptionalStatusText = NULL;
    *pcpsiOptionalStatusIcon = CPSI_NONE;

    DWORD dwStatusInfo = (DWORD)-1;

    // Look for a match on status and substatus.
    for (DWORD i = 0; i < ARRAYSIZE(s_rgLogonStatusInfo); i++)
    {
        if (s_rgLogonStatusInfo[i].ntsStatus == ntsStatus && s_rgLogonStatusInfo[i].ntsSubstatus == ntsSubstatus)
        {
            dwStatusInfo = i;
            break;
        }
    }

    if ((DWORD)-1 != dwStatusInfo)
    {
        if (SUCCEEDED(SHStrDupW(s_rgLogonStatusInfo[dwStatusInfo].pwzMessage, ppwzOptionalStatusText)))
        {
            *pcpsiOptionalStatusIcon = s_rgLogonStatusInfo[dwStatusInfo].cpsi;
        }
    }

    // Since NULL is a valid value for *ppwszOptionalStatusText and *pcpsiOptionalStatusIcon
    // this function can't fail.
    return S_OK;
}
