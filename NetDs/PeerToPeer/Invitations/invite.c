/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) 1985-2005 Microsoft Corporation. All Rights Reserved.

Abstract:

    This C file includes sample code for sending an invitation
    with the Microsoft Peer-to-Peer Collaboration APIs.

Feedback:
    If you have any questions or feedback, please contact us using
    any of the mechanisms below:

    Email: peerfb@microsoft.com
    Newsgroup: Microsoft.public.win32.programmer.networks
    Website: http://www.microsoft.com/p2p

Notes:
    * This peer to peer application requires global IPv6 connectivity.

    * This sample relies on having existing contacts who are actively
      signed in.  This sample does not offer the ability to add new
      contacts.  To do this, the Contacts samples may be run.

--********************************************************************/

#include <p2p.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>

#define ERRBUFSIZE 1024
#define HRESULTTOWIN32(hres)                                \
            ((HRESULT_FACILITY(hres) == FACILITY_WIN32)     \
                ? HRESULT_CODE(hres)                        \
                : (hres))

// Note: Every application should create a new guid for their applications.
//       Never copy and paste the guid below for use in your own application.
//
const GUID SampleAppGuid = {0x12341234, 0x1234, 0x1234, {0x12,0x34,0x12,0x34,0x12,0x34,0x12,0x34}};



//----------------------------------------------------------------------------
// Function:   DisplayLocalApplicationInfo
// Purpose:    Prints out locally registered applications
// Parameters:
//   pApplication : [in] pointer to PEER_APPLICATION_REGISTRATION_INFO object
//
VOID DisplayLocalApplicationInfo(__in PCPEER_APPLICATION_REGISTRATION_INFO pApplication)
{
    PWSTR   pwzApplicationId = NULL;

    wprintf(L"Application:\n");

    if (UuidToString((UUID* )&pApplication->application.id, &pwzApplicationId) == RPC_S_OK)
    {
        wprintf(L"\tId: %s\n", pwzApplicationId);
        RpcStringFree(&pwzApplicationId);
    }

    if (pApplication->application.pwzDescription != NULL)
    {
        wprintf(L"\tDescription: %s\n", pApplication->application.pwzDescription);
    }

    if (pApplication->pwzApplicationToLaunch != NULL)
    {
        wprintf(L"\tApplication Path: %s\n", pApplication->pwzApplicationToLaunch);
    }

    if (pApplication->pwzApplicationArguments != NULL)
    {
        wprintf(L"\tApplication Arguments: %s\n", pApplication->pwzApplicationArguments);
    }
}

//-----------------------------------------------------------------------------
// Function:    DisplayLocallyRegisteredApplications
// Purpose:     Retrieves and displays locally registered applications
// Parameters:  None
//
HRESULT DisplayLocallyRegisteredApplications()
{
    PPEER_APPLICATION_REGISTRATION_INFO*  ppApplications = NULL;
    HPEERENUM           hApplicationEnum = NULL;
    ULONG               cApplications = 0;
    ULONG               i = 0;
    HRESULT             hr = S_OK;

    printf("Displaying locally registered applications:\n");

    // Get a list of locally registgered applications 
    hr = PeerCollabEnumApplicationRegistrationInfo(PEER_APPLICATION_CURRENT_USER, &hApplicationEnum);

    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hApplicationEnum, &cApplications);

        if (SUCCEEDED(hr))
        {
            if (cApplications == 0)
            {
                wprintf(L"There are no locally registered applications for the current user.\n");
            }
            else
            {
                hr = PeerGetNextItem(hApplicationEnum, &cApplications, (PVOID **) &ppApplications);
                if (SUCCEEDED(hr))
                {
                    for (i = 0; i < cApplications; i++)
                    {
                        DisplayLocalApplicationInfo(ppApplications[i]);
                    }

                    PeerFreeData(ppApplications);
                }
                else
                {
                    wprintf(L"Error retrieving application info\nHRESULT=0x%x\n", hr);
                }
            }
        }

        PeerEndEnumeration(hApplicationEnum);
    }
    else
    {
        wprintf(L"Error retrieving application info\nHRESULT=0x%x\n", hr);
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function:   GetContacts
// Purpose:    Retrieves all contacts for the currently logged in user
// Parameters:
//   pcContacts  :  [out] number of contacts retrieved
//   pppContacts :  [out] pointer to array of contact pointers
//

void PrintError(HRESULT hrError)
{
    DWORD dwCch=0;
    WCHAR wszBuffer[ERRBUFSIZE] = {0};  

    if (HRESULT_FACILITY(hrError) == FACILITY_P2P)
    {
        HMODULE hResDll = GetModuleHandle(L"p2p.dll");
        if (NULL != hResDll)
        {
            dwCch = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE,
                                  hResDll,
                                  HRESULTTOWIN32(hrError),
                                  0,
                                  wszBuffer,
                                  ERRBUFSIZE,
                                  NULL);
            FreeLibrary(hResDll);
           
            if (dwCch > 0)
            {
                wprintf(L"Error Description: %s\n", wszBuffer);
            }
        }
    }
    else
    {
        dwCch = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                              NULL,
                              HRESULTTOWIN32(hrError),
                              0,
                              wszBuffer,
                              ERRBUFSIZE,
                              NULL);

            if (dwCch > 0)
            {
                wprintf(L"Error Description: %s\n", wszBuffer);
            }
    }
}  


HRESULT GetContacts(__out ULONG *pcContacts, __out_ecount(*pcContacts) PEER_CONTACT ***pppContacts)
{
    HPEERENUM hEnum;
    HRESULT hr = S_OK;

    *pcContacts = 0;

    // Begin an enumeration of all contacts
    //
    hr = PeerCollabEnumContacts(&hEnum);

    if (SUCCEEDED(hr))
    {
        // Retrieve the number of contacts
        //
        hr = PeerGetItemCount(hEnum, pcContacts);

        // Retrieve all the contacts
        //
        if (SUCCEEDED(hr))
        {
            hr = PeerGetNextItem(hEnum, pcContacts, (void***) pppContacts);
        }

        // Close the enumeration handle
        //
        PeerEndEnumeration(hEnum);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function:   HasApplication
// Purpose:    Utility routine to check whether a contact/endpoint pair has
//             the desired application
// Parameters:
//   pContact  : [in] the contact to check for the application
//   pEndpoint : [in] the endpoint to check for the application
//
BOOL HasApplication(PCPEER_CONTACT pContact, PCPEER_ENDPOINT pEndpoint)
{
    BOOL fHasApplication = FALSE;
    HRESULT hr = S_OK;
    HPEERENUM hEnum;
    ULONG cApplications = 0;
    PEER_APPLICATION **ppApplications = NULL;

    // Begin an enumeration of all Capabilites that have the SampleAppGuid
    //
    hr = PeerCollabEnumApplications(pEndpoint, (GUID*) &SampleAppGuid, &hEnum);

    if (SUCCEEDED(hr))
    {
        // Retrieve the number of applications found in the enumeration
        //
        hr = PeerGetItemCount(hEnum, &cApplications);

        // If there is more than 1... the contact has that application
        //
        if (SUCCEEDED(hr) && cApplications > 0)
        {
            fHasApplication = TRUE;
        }

        // Close the enumeration handle
        //
        PeerEndEnumeration(hEnum);
    }

    return fHasApplication;
}

//-----------------------------------------------------------------------------
// Function:   DisplayContacts
// Purpose:    Utility routine to display an array of contacts
// Parameters:
//   cContacts  : [in] number of contacts to display
//   ppContacts : [in] array of contact pointers
//
HRESULT DisplayContacts(ULONG cContacts, __in_ecount(cContacts) PEER_CONTACT **ppContacts)
{
    HRESULT hr = S_OK;
    ULONG iContact;

    if (cContacts > 0)
        printf("Contacts:\n");
    else
        printf("No contacts found.\n");

    for (iContact = 0; iContact < cContacts; iContact++)
    {
        HPEERENUM hEnum;

        // Print out the contacts name (or PeerName if no Nickname)
        //
        printf(" %d: ", iContact);
        if (ppContacts[iContact]->pwzNickName != NULL)
            printf("%S\t", ppContacts[iContact]->pwzNickName);
        printf("%S\n", ppContacts[iContact]->pwzPeerName);

        // Begin an enumeration of all endpoints for this contact
        //
        hr = PeerCollabEnumEndpoints(ppContacts[iContact], &hEnum);

        if (SUCCEEDED(hr))
        {
            ULONG cEndpoints;

            // Retrieve the number of endpoints in the enumeration
            //
            hr = PeerGetItemCount(hEnum, &cEndpoints);

            if (SUCCEEDED(hr) && cEndpoints > 0)
            {
                PEER_ENDPOINT **ppEndpoints = NULL;

                // Retrieve all the endpoints in the enumeration
                //
                hr = PeerGetNextItem(hEnum, &cEndpoints, (void***) &ppEndpoints);

                if (SUCCEEDED(hr))
                {
                    ULONG iEndpoint;

                    // Display information about each endpoint
                    //
                    for (iEndpoint = 0; iEndpoint < cEndpoints; iEndpoint++)
                    {
                        printf("\tEndpoint %d", iEndpoint);
                        if (ppEndpoints[iEndpoint]->pwzEndpointName != NULL)
                            printf(" [%S]", ppEndpoints[iEndpoint]->pwzEndpointName);

                        if (HasApplication(ppContacts[iContact], ppEndpoints[iEndpoint]))
                            printf(" Has SampleApp application.\n");
                        else
                            printf(" Does not have SampleApp application.\n");
                    }
                    printf("\n");

                    // Free the data retrieved from PeerGetNextItem
                    //
                    PeerFreeData(ppEndpoints);
                }
            }
            else
            {
                printf("\tNo endpoints.\n");
            }

            // Close the enumeration handle
            //
            PeerEndEnumeration(hEnum);
        }
        else
        {
            printf("\tFailed to enum this contact's endpoints\n\t");
            PrintError(hr);
        }
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function:   GetContactEndpoints
// Purpose:    Retrieve the endpoints for a given contact
// Parameters:
//   pContact     : [in] contact to retrieve endpoints for
//   pcEndpoints  : [out] number of endpoints retrieved
//   pppEndpoints : [out] pointer to array of endpoint pointers for the specified contact
//
HRESULT GetContactEndpoints(const PEER_CONTACT *pContact,
                            __out ULONG *pcEndpoints,
                            __out_ecount(*pcEndpoints) PEER_ENDPOINT ***pppEndpoints)
{
    HRESULT hr = S_OK;
    HPEERENUM hEnum;

    // Enumerate all endpoints for this contact
    //
    hr = PeerCollabEnumEndpoints(pContact, &hEnum);

    if (SUCCEEDED(hr))
    {
        // Get the count of endpoints for this contact
        //
        hr = PeerGetItemCount(hEnum, pcEndpoints);

        // If there are any endpoints, retrieve them all
        //
        if (SUCCEEDED(hr) && *pcEndpoints > 0)
        {
            hr = PeerGetNextItem(hEnum, pcEndpoints, (void***) pppEndpoints);
        }

        // Close the enumeration handle
        //
        PeerEndEnumeration(hEnum);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function:   SendAsyncInvite
// Purpose:    Utility routine to send an invitation
// Parameters:
//   pContact   :  [in] contact to send an invite to
//   pEndpoint  :  [in] endpoint to send an invite to
//   pInvite    :  [in] the invitation
//
HRESULT SendAsyncInvite(__in const PEER_CONTACT *pContact,
                        __in const PEER_ENDPOINT *pEndpoint,
                        __in const PEER_INVITATION *pInvite)
{
    HANDLE hInvite = NULL;
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    HRESULT hr = S_OK;
    PPEER_INVITATION_RESPONSE pResponse = NULL;

    if (hEvent == NULL)
        return E_OUTOFMEMORY;

    // Send an async invite
    //
    hr = PeerCollabAsyncInviteContact(pContact, pEndpoint, pInvite, hEvent, &hInvite);

    if (SUCCEEDED(hr))
    {
        // For simplicity we block and wait on the handle.  More sophisticated applications
        // would continue processing their message loops, and/or do this work in another thread.
        //
        WaitForSingleObject(hEvent, INFINITE);

        // Event has been signalled, get the response
        //
        hr = PeerCollabGetInvitationResponse(hInvite, &pResponse);

        if (SUCCEEDED(hr))
        {
            if (pResponse->action == PEER_INVITATION_RESPONSE_ACCEPTED)
                printf("Invitation accepted.\n");
            else
                printf("Invitation not accepted.\n");

            PeerFreeData(pResponse);
        }

        PeerCollabCloseHandle(hInvite);
    }

    CloseHandle(hEvent);
    return hr;
}

//-----------------------------------------------------------------------------
// Function:   SendSyncInvite
// Purpose:    Utility routine to send an invitation
// Parameters:
//   pContact   :  [in] contact to send an invite to
//   pEndpoint  :  [in] endpoint to send an invite to
//   pInvite    :  [in] the invitation
//

HRESULT SendSyncInvite(__in const PEER_CONTACT *pContact,
                       __in const PEER_ENDPOINT *pEndpoint,
                       __in const PEER_INVITATION *pInvite)
{
    PPEER_INVITATION_RESPONSE pResponse = NULL;

    HRESULT hr = PeerCollabInviteContact(pContact, pEndpoint, pInvite, &pResponse);

    if (SUCCEEDED(hr))
    {
        if (pResponse->action == PEER_INVITATION_RESPONSE_ACCEPTED)
            printf("Invitation accepted.\n");
        else
            printf("Invitation not accepted.\n");

        PeerFreeData(pResponse);
    }
    else
    {
        printf("Failed to send invite.\n");
        PrintError(hr);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function:   SendInvite
// Purpose:    Sends an invite to given contact and endpoint
// Parameters:
//   pContact     : [in] contact to send invite to
//   fSynchronous : [in] flag which is true if the function should be synchronous
//
HRESULT SendInvite(__in const PEER_CONTACT *pContact, BOOL fSynchronous)
{
    HRESULT hr = S_OK;
    PEER_ENDPOINT **ppEndpoints = NULL;
    ULONG cEndpoints = 0;
    BOOL fInvitationSent = FALSE;
    PPEER_INVITATION_RESPONSE pResponse = NULL;
    PEER_INVITATION InviteRequest = {0};
    CHAR AppDataExample[] = "I am extra data to send as part of invite";

    InviteRequest.applicationId = SampleAppGuid;
    InviteRequest.applicationData.cbData = sizeof(AppDataExample);
    InviteRequest.applicationData.pbData = (PBYTE) AppDataExample;
    InviteRequest.pwzMessage = L"Let's run the sample app together!";

    //
    // Retrieve all the endpoints for the specified contact
    hr = GetContactEndpoints(pContact, &cEndpoints, &ppEndpoints);

    if (SUCCEEDED(hr))
    {
        //
        // Find an endpoint with the desired application
        ULONG iEndpoint;

        for (iEndpoint = 0; iEndpoint < cEndpoints; iEndpoint++)
        {
            // Check if specified endpoint has the sampleapp application
            //
            if (HasApplication(pContact, ppEndpoints[iEndpoint]))
            {
                printf("Sending invite to %S\n", pContact->pwzPeerName);

                if (fSynchronous)
                    hr = SendSyncInvite(pContact, ppEndpoints[iEndpoint], &InviteRequest);
                else
                    hr = SendAsyncInvite(pContact, ppEndpoints[iEndpoint], &InviteRequest);

                fInvitationSent = TRUE;
                break;
            }
        }

        PeerFreeData(ppEndpoints);
    }
    else
    {
        printf("Failed to get endpoint data.\n");
    }

    if (!fInvitationSent)
    {
        printf("No invitations sent.\n");
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function:   RegisterApplication
// Purpose:    Register a new application for the current user
// Parameters: None
//
void RegisterApplication()
{
    WCHAR wzPath[MAX_PATH];
    HRESULT hr = S_OK;
    PEER_APPLICATION_REGISTRATION_INFO RegInfo = {0};

    GetWindowsDirectory(wzPath, MAX_PATH);
    StringCchCat(wzPath, MAX_PATH, L"\\notepad.exe");

    // Setup the application registration
    //
    RegInfo.application.id = SampleAppGuid;
    RegInfo.application.pwzDescription = L"Sample Application";
    RegInfo.pwzApplicationToLaunch = wzPath;
    RegInfo.dwPublicationScope = PEER_PUBLICATION_SCOPE_ALL;

    // Register the sample app as a new application for all users
    //
    hr = PeerCollabRegisterApplication(&RegInfo, PEER_APPLICATION_CURRENT_USER);

    if (SUCCEEDED(hr))
    {
        printf("Successfully registered application\n");
    }
    else
    {
        printf("Failed to register application, likely already registered\n");
    }
}

//-----------------------------------------------------------------------------
// Function:   main
// Purpose:    entry point
// Parameters:
//
int __cdecl main(int argc, __in_ecount(argc) char *argv[])
{
    HRESULT hr = S_OK;
    PEER_CONTACT **ppContacts = NULL;
    ULONG cContacts = 0;
    BOOL fSynchronous = TRUE;

    if (argc > 1 && strcmp(argv[1], "/async") == 0)
        fSynchronous = FALSE;

    hr =  PeerCollabStartup(PEER_COLLAB_VERSION);

    if (SUCCEEDED(hr))
    {
        hr = PeerCollabSignin(NULL, PEER_SIGNIN_ALL);

        if (SUCCEEDED(hr))
        {
            RegisterApplication();
            DisplayLocallyRegisteredApplications();

            // Retrieve the list of contacts
            //
            hr = GetContacts(&cContacts, &ppContacts);

            // Display the contacts
            //
            if (SUCCEEDED(hr))
                DisplayContacts(cContacts, ppContacts);
            else
            {
                printf("Failed to retrieve contacts.\n", hr);
                PrintError(hr);
            }

            // Send an invite to a user specified contact
            //
            if (SUCCEEDED(hr) && cContacts > 0)
            {
                CHAR szBuff[5];
                UINT Selection = 0;

                // Get a contact from the user
                //
                printf("Contact number which you will send invite to: ");
                hr = StringCbGetsA(szBuff, sizeof(szBuff));

                if (szBuff[0] != L'\0')
                {
                    Selection = atoi(szBuff);

                    // Send the invitation
                    //
                    if (SUCCEEDED(hr) && Selection < cContacts)
                    {
                        hr = SendInvite(ppContacts[Selection], fSynchronous);
                    }
                }

                // Free the contacts
                //
                PeerFreeData(ppContacts);
            }

            PeerCollabShutdown();
        }
        else
        {
            printf("PeerCollabSignin failed.\n");
            PrintError(hr);
        }
    }
    else
    {
        printf("PeerCollabStartup failed.\n");
        PrintError(hr);
    }
}
