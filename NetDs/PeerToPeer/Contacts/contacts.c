/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) 1985-2005 Microsoft Corporation. All Rights Reserved.

Abstract:
    This C file includes sample code for working with contacts using
    the Microsoft Peer-to-Peer Collaboration APIs.

Feedback:
    If you have any questions or feedback, please contact us using
    any of the mechanisms below:

    Email: peerfb@microsoft.com
    Newsgroup: Microsoft.public.win32.programmer.networks
    Website: http://www.microsoft.com/p2p

Note:
    This peer to peer application requires global IPv6 connectivity.

--********************************************************************/
#include <p2p.h>
#include <stdio.h>
#include <strsafe.h>

#define celems(a)   (sizeof(a) / sizeof(a[0]))

#define MAX_PEERNAME      250

#define STRING_BUFSIZE    500
#define INPUT_BUFSIZE     10
#define LONG_TIMEOUT      (30 * 1000)
#define MAX_CONTACT_INFO  (8 * 1024)
#define ERRBUFSIZE 1024
#define HRESULTTOWIN32(hres)                                \
            ((HRESULT_FACILITY(hres) == FACILITY_WIN32)     \
                ? HRESULT_CODE(hres)                        \
                : (hres))

//-----------------------------------------------------------------------------
// Function:    PrintError
// Purpose:     Prints the error and the associated error string
// Parameters:  hr error obtained
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



//-----------------------------------------------------------------------------
// Function:    DisplayContacts
// Purpose:     Utility routine to display an array of contacts
// Parameters:
//    cContacts : count of contacts in ppContacts array
//    ppContacts: array of pointers to contacts to display
//
void DisplayContacts(ULONG cContacts, __in_ecount(cContacts) PEER_CONTACT **ppContacts)
{
    ULONG i;
    printf("Contacts:\n");

    for (i = 0; i < cContacts; i++)
    {
        printf("\tPeerName=%S\t FriendlyName=%S\n",
               ppContacts[i]->pwzPeerName,
               ppContacts[i]->pwzNickName);
    }
}

//-----------------------------------------------------------------------------
// Function:    DisplayApplications
// Purpose:     Utility routine to display the applications of an endpoint
// Parameters:
//    cApplications : count of applications in ppApplications array
//    ppApplications: array of pointers to applications to display
//
void DisplayApplications(ULONG cApplications, __in_ecount(cApplications) PEER_APPLICATION **ppApplications)
{
    ULONG i;
    printf("Applications:\n");

    for (i = 0; i < cApplications; i++)
    {
        printf("\tDescription=%S\n", ppApplications[i]->pwzDescription);
    }
}

//-----------------------------------------------------------------------------
// Function:    ExportContact
// Purpose:     Routine to export the "me" contact to a user specified file.
// Parameters:  None.
//
void ExportContact()
{
    WCHAR wzPath[MAX_PATH];
    PWSTR pwzXML = NULL;
    HRESULT hr = S_OK;

    // Get information about default contact
    //
    hr = PeerCollabExportContact(NULL, &pwzXML);

    if (SUCCEEDED(hr))
    {
        // Write the information out to a user specified file
        //
        printf("Enter the path to export to: ");
        hr = StringCbGets(wzPath, sizeof(wzPath));

        if (SUCCEEDED(hr))
        {
            FILE *pFile = NULL;
            errno_t err = 0;

            err = _wfopen_s(&pFile, wzPath, L"wb");
            if (err != 0)
            {
                hr = E_FAIL;
            }
            else
            {
                if (fputws(pwzXML, pFile) == WEOF)
                {
                    hr = E_FAIL;
                }
                fclose(pFile);
            }
        }

        PeerFreeData(pwzXML);
    }

    if (SUCCEEDED(hr))
    {
        printf("Contact successfully exported to %S\n", wzPath);
    }
    else
    {
        printf("Export failed\n");
        PrintError(hr);
    }
}

//-----------------------------------------------------------------------------
// Function:    ImportContact
// Purpose:     Routine to import a contact from a user specified file
// Parameters:  None.
//
void ImportContact()
{
    HRESULT hr = S_OK;
    WCHAR wzPath[MAX_PATH];
    WCHAR wzContactInfo[MAX_CONTACT_INFO] = {0};

    printf("Enter path to contact file: ");
    hr = StringCbGets(wzPath, sizeof(wzPath));

    if (SUCCEEDED(hr))
    {
        FILE *pFile = NULL;
        errno_t err = 0;

        err = _wfopen_s(&pFile, wzPath, L"rb");
        if (err != 0)
        {
            hr = E_FAIL;
        }
        else
        {
            if (fread(wzContactInfo, sizeof(WCHAR), MAX_CONTACT_INFO-1, pFile) == 0)
            {
                hr = E_FAIL;
            }
            fclose(pFile);
        }
    }

    if (FAILED(hr))
    {
        printf("unable to open and read from the contact file");
        return;
    }    

    // Add the contact information to the local machine
    //
    if (SUCCEEDED(hr))
    {
        hr = PeerCollabAddContact(wzContactInfo, NULL);
    }

    if (SUCCEEDED(hr))
    {
        printf("Contact successfully imported.\n");
    }
    else
    {
        printf("Import failed\n");
        PrintError(hr);
    }
}


//-----------------------------------------------------------------------------
// Function:    GiveWatchPermissions
// Purpose:     Routine to grant a user specified contact permission to watch
//              for our presence changes, and at the same time, start watching them
// Parameters:  None.
//
void GiveWatchPermissionsAndStartWatching()
{
    HRESULT hr = S_OK;
    PPEER_CONTACT pContact = {0};
    WCHAR wzPeerName[MAX_PEERNAME];

    printf("Enter the PeerName of the contact to give permissions: ");
    hr = StringCbGets(wzPeerName, sizeof(wzPeerName));

    if (SUCCEEDED(hr))
    {
        // Given a user specified PeerName, retrieve the contact
        //
        hr = PeerCollabGetContact(wzPeerName, &pContact);

        if (SUCCEEDED(hr))
        {
            // Grant this contact rights to watch us
            //
            pContact->WatcherPermissions = PEER_WATCH_ALLOWED;

            // Start watching them
            //
            pContact->fWatch = TRUE;

            hr = PeerCollabUpdateContact(pContact);

            if (SUCCEEDED(hr))
            {
                if (pContact->pwzNickName)
                    printf("Successfully gave watch permissions to %S.\n", pContact->pwzNickName);
                else
                    printf("Successfully gave watch permissions to %S.\n", pContact->pwzPeerName);
            }

            PeerFreeData(pContact);
        }
    }

    if (FAILED(hr))
    {
        printf("Failed to give watch permisions\n");
        PrintError(hr);
    }
}

//-----------------------------------------------------------------------------
// Function:    WaitForPresenceChange
// Purpose:     Waits for a period of time, for any contact to change presence status
// Parameters:  None
//
HRESULT WaitForPresenceChange()
{
    HPEEREVENT hPeerEvent;
    HRESULT hr = S_OK;
    PEER_COLLAB_EVENT_REGISTRATION event;
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (hEvent == NULL)
    {
        return E_OUTOFMEMORY;
    }

    event.eventType = PEER_EVENT_ENDPOINT_PRESENCE_CHANGED;
    event.pInstance = NULL;

    // Register to be notified when presence information for any of our contacts changes
    //
    hr = PeerCollabRegisterEvent(hEvent, 1, &event, &hPeerEvent);

    if (SUCCEEDED(hr))
    {
        // Wait 30 seconds and see if any of our contacts change presence
        //
        if (WaitForSingleObject(hEvent, LONG_TIMEOUT) == WAIT_OBJECT_0)
        {
            PEER_COLLAB_EVENT_DATA *pEventData = NULL;

            // Retrieve information about this presence change
            //
            hr = PeerCollabGetEventData(hPeerEvent, &pEventData);

            if (SUCCEEDED(hr))
            {
                if (pEventData->presenceChangedData.changeType == PEER_CHANGE_ADDED)
                {
                    printf("Received Presence Added Event...\n");
                    DisplayContacts(1, &pEventData->presenceChangedData.pContact);
                }
                else if (pEventData->presenceChangedData.changeType == PEER_CHANGE_DELETED)
                {
                    printf("Received Presence Deleted Event...\n");
                    DisplayContacts(1, &pEventData->presenceChangedData.pContact);
                }
                else if (pEventData->presenceChangedData.changeType == PEER_CHANGE_UPDATED)
                {
                    printf("Received Presence Updated Event...\n");
                    DisplayContacts(1, &pEventData->presenceChangedData.pContact);
                }

                PeerFreeData(pEventData);
            }
        }

        PeerCollabUnregisterEvent(hPeerEvent);
    }

    CloseHandle(hEvent);
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    EnumApplications
// Purpose:     Enumerates all the capabilites of a user supplied contact
// Parameters:  None
//
HRESULT EnumApplications()
{
    PEER_CONTACT *pContact = NULL;
    PEER_ENDPOINT **ppEndpoints = NULL;
    PEER_APPLICATION **ppApplications = NULL;
    ULONG ulCount;
    WCHAR wzPeerName[MAX_PEERNAME];
    HRESULT hr = S_OK;

    printf("Enter the PeerName of the contact: ");
    hr = StringCbGets(wzPeerName, sizeof(wzPeerName));

    // Grab the contact information
    //
    if (SUCCEEDED(hr))
    {
        hr = PeerCollabGetContact(wzPeerName, &pContact);
    }

    // Grab an endpoint for this contact
    //
    if (SUCCEEDED(hr))
    {
        HPEERENUM hEndpointEnum = NULL;
        hr = PeerCollabEnumEndpoints(pContact, &hEndpointEnum);

        if (SUCCEEDED(hr))
        {
            // Get just the first endpoint
            //
            ulCount = 1;
            hr = PeerGetNextItem(hEndpointEnum, &ulCount, (void***) &ppEndpoints);

            if (ulCount == 0)
            {
                hr = E_FAIL;
            }

            PeerEndEnumeration(hEndpointEnum);
        }
    }
    else
    {
        printf("Could not get contact for %S\n", wzPeerName);
        PrintError(hr);
    }

    // Enumerate the capabilites at that endpoint
    //
    if (SUCCEEDED(hr))
    {
        HPEERENUM hApplicationEnum = NULL;
        hr = PeerCollabEnumApplications(ppEndpoints[0], NULL, &hApplicationEnum);
        if (SUCCEEDED(hr))
        {
            // Get the count of applications
            //
            hr = PeerGetItemCount(hApplicationEnum, &ulCount);

            // Retrieve info about every application and display it
            //
            if (SUCCEEDED(hr) && ulCount > 0)
            {
                hr = PeerGetNextItem(hApplicationEnum, &ulCount, (void***) &ppApplications);

                if (SUCCEEDED(hr))
                {
                    DisplayApplications(ulCount, ppApplications);
                    PeerFreeData(ppApplications);
                }
            }

            PeerEndEnumeration(hApplicationEnum);
        }
        else
        {
            printf("Could not enum applications");
            PrintError(hr);
        }
    }

    PeerFreeData(ppEndpoints);
    PeerFreeData(pContact);

    return hr;
}

//-----------------------------------------------------------------------------
// Function:    EnumContacts
// Purpose:     Enumerates and displays all contacts
// Parameters:  None
//
HRESULT EnumContacts()
{
    HRESULT hr = S_OK;
    HPEERENUM hEnum;
    ULONG count = 0;
    PEER_CONTACT **ppContacts = NULL;

    // Enumerate all the contacts
    //
    hr = PeerCollabEnumContacts(&hEnum);

    if (SUCCEEDED(hr))
    {
        // Retrieve the number of contacts in the enumeration
        //
        hr = PeerGetItemCount(hEnum, &count);

        // Retrieve all the contacts
        //
        if (SUCCEEDED(hr))
        {
            hr = PeerGetNextItem(hEnum, &count, (void***) &ppContacts);
        }

        // Display all the contacts
        //
        if (SUCCEEDED(hr) && count > 0)
        {
            DisplayContacts(count, ppContacts);
            PeerFreeData(ppContacts);
        }

        PeerEndEnumeration(hEnum);
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function:    PrintPresenceInfo
// Purpose:     Prints presence status
// Parameters:  Pointer to a presence info struct to be printed out
//
HRESULT PrintPresenceInformation(__in PPEER_PRESENCE_INFO pPresInfo)
{

    // first print the text equivalent of thier status
    switch (pPresInfo->status)
    {
        case PEER_PRESENCE_OFFLINE:
            printf ("Offline");
            break;
        case PEER_PRESENCE_OUT_TO_LUNCH:
            printf("Out to lunch");
            break;
        case PEER_PRESENCE_AWAY:
            printf("Away");
            break;
        case PEER_PRESENCE_BE_RIGHT_BACK:
            printf("Be Right Back");
            break;
        case PEER_PRESENCE_IDLE:
            printf ("Idle");
            break;
        case PEER_PRESENCE_BUSY:
            printf ("Busy");
            break;
        case PEER_PRESENCE_ON_THE_PHONE:
            printf("On the phone");
            break;
        case PEER_PRESENCE_ONLINE:
            printf("Online");
            break;
        default:
            break;
    }

    // now print any descriptive text 
    // (for example, the user might set status to "Playing Halo")
    printf(" -  %S\n", pPresInfo->pwzDescriptiveText == NULL ? L"" :
                       pPresInfo->pwzDescriptiveText);

    return S_OK;
}



//-----------------------------------------------------------------------------
// Function:   FindIndexOfMatchingContact
// Purpose:    Find the index of a contact in the array of contacts with a matching peer name
// Parameters:
//  ppContacts - pointer to array of contacts returned by enumerating
//  nCount - how many contacts in ppContacts array
//  pwzPeerName - Peername to look for in the contacts array
//  pnIndex (out) - index of contact found with a matching peername
//
HRESULT FindIndexOfMatchingContact(__in PPEER_CONTACT* ppContacts, __in ULONG nCount, __in LPCWSTR pwzPeerName, __out ULONG* pnIndex)
{
    ULONG i;

    for (i = 0; i < nCount; ++i)
    {
        if (0 == _wcsicmp(pwzPeerName, ppContacts[i]->pwzPeerName))
        {
            *pnIndex = i;
            return S_OK;
        }
    }
    
    return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
}


//-----------------------------------------------------------------------------
// Function:    PickEndpointForContact
// Purpose:     If there are more than one endpoint for a contact, let user choose which to use
// Parameters:  
//   pContact - contact to pick an endpoint for
//   pPeerEndpoint (out) - fill in the already allocated endpoint struct with the data for the endpoint the user picked
//
HRESULT PickEndpointForContact(__in PPEER_CONTACT pContact, __out PPEER_ENDPOINT pPeerEndpoint)
{
    HRESULT hr = S_OK;
    HPEERENUM hEnumEndpoints = NULL;
    ULONG nCount = 0;
    ULONG nIndex;
    ULONG i = 0;
    PPEER_ENDPOINT*     ppEndpoints = NULL;
    WCHAR   wzInputBuf[INPUT_BUFSIZE];
    WCHAR   wzAddressBuf[STRING_BUFSIZE];
    DWORD   cchAddressBuf = celems(wzAddressBuf);
    int     rc = 0;
    

    // Enumerate all the endpoints we have for this contact
    hr = PeerCollabEnumEndpoints(pContact, &hEnumEndpoints);
    if (FAILED(hr))
        return hr;

    //see how many endpoints we have
    hr = PeerGetItemCount(hEnumEndpoints, &nCount);    
     if (FAILED(hr))
     {
        PeerEndEnumeration(hEnumEndpoints);
        return hr;
     }

    // get the array of enumerated items to display
    hr = PeerGetNextItem(hEnumEndpoints, &nCount, (void***) &ppEndpoints);
    if (FAILED(hr))
    {
        PeerEndEnumeration(hEnumEndpoints);
        return hr;
    }

    // if there are no endpoints, we can't go any further
    if (0 == nCount)
    {
        printf("We are not currently watching presence for this user.\n");
        PeerFreeData(ppEndpoints);
        PeerEndEnumeration(hEnumEndpoints);
        return E_FAIL;
    }

    if (1 == nCount)
    {
        // if there is only one endpoint, return it, we are done.
        nIndex = 0;
    }
    else 
    {
        // if there are multiple endpoints, display them and let the user choose
        // loop through all the endpoints, displaying them with a number
        printf("This Peername is logged in at multiple IP addresses.  Which one would you like to use?\n");
        printf("   please type a response between 1 and %d\n", nCount+1);

        for (i = 0; i < nCount; ++i)
        {
            rc = WSAAddressToString((SOCKADDR*)(&ppEndpoints[i]->address.sin6),
                                    ppEndpoints[i]->address.dwSize, NULL, 
                                    wzAddressBuf, &cchAddressBuf);
            if (rc == NO_ERROR)            
                printf("     %d. %S\n", (nCount+1), wzAddressBuf);
        }

        // get user response
        hr = StringCbGets(wzInputBuf, sizeof(wzInputBuf));
        if (FAILED(hr))
        {
            printf ("Invalid input.  Expected a number between 1 and %d. (inclusive)\n", nCount+1);
            PeerFreeData(ppEndpoints);
            PeerEndEnumeration(hEnumEndpoints);
            return E_INVALIDARG;
        }

        // translate the user response to a useful index
        //  since wtoi returns 0 on failure, we add 1 to the names for display, and subtract 1 before using them.
        nIndex = _wtoi(wzInputBuf);
        if (0 == nIndex || nIndex > nCount)
        {
            printf ("Invalid input.  Expected a number between 1 and %d.\n", nCount);
            PeerFreeData(ppEndpoints);
            PeerEndEnumeration(hEnumEndpoints);
            return E_INVALIDARG;
        }

        nIndex--;
    }

    // copy information from the selected endpoint into the passed in endpoint struct
    pPeerEndpoint->address = ppEndpoints[nIndex]->address;
    pPeerEndpoint->pwzEndpointName = NULL;
    
    // free the enum information
    PeerFreeData(ppEndpoints);
    PeerEndEnumeration(hEnumEndpoints);
            
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    GetPresenceInformation
// Purpose:     Demonstrate the use of PeerCollabGetPresenceInfo() API
// Parameters:  None
//
HRESULT GetPresenceInformation()
{
    HRESULT         hr = S_OK;
    PEER_ENDPOINT   peerEndpoint = {0};
    PEER_PRESENCE_INFO* pPresInfo = NULL;
    WCHAR   wzPeerName[MAX_PEERNAME];
    HPEERENUM hEnum;
    ULONG nCount = 0;
    ULONG nIndex = 0;
    PEER_CONTACT **ppContacts = NULL;

    // determine which contact to get the presence info from, and fill in the peerEndpoint struct
    //  There are several ways to do this, you could list all contacts and have the user pick one,
    //  have the user enter a friendly name and look for a match, or have the user enter a peer name,
    //  and look for an endpoint for that peer name.  
    printf("Enter the peer name of a user you want to get presence information for:  \n");
    printf("Example: d46e49f3b6923376c9a533ef47abdc025a50c0e5\n");

    // put the peer name into a string
    // enter presence status to set for yourself
    hr = StringCbGets(wzPeerName, sizeof(wzPeerName));
    if (FAILED(hr))
    {
        printf ("Invalid input\n");
        return E_INVALIDARG;
    }

    // now see if we have any contacts with that peer name.  If not, we are done.
    // if we do have contacts, they may be logged into multiple endpoints, and we must pick one
    
    // Enumerate all the contacts
    //
    hr = PeerCollabEnumContacts(&hEnum);
    if (FAILED(hr))
    {
        printf("Unable to enumerate contacts.\n");
        return hr;
    }

    // Retrieve the number of contacts in the enumeration
    //
    hr = PeerGetItemCount(hEnum, &nCount);
    if (FAILED(hr))
    {
        printf("Internal error - unable to get count of enumerated items.\n");
        PeerEndEnumeration(hEnum); 
        return E_FAIL;
    }

    // can't watch presence if we don't have any contacts to watch presence of
    if (0 == nCount)
    {
        printf("No contacts to get presence for.\n");
        PeerEndEnumeration(hEnum); 
        return S_OK;
    }

    // Retrieve all the contacts
    //
    if (SUCCEEDED(hr))
    {
        hr = PeerGetNextItem(hEnum, &nCount, (void***) &ppContacts);
        if (FAILED(hr))
        {
            printf("Internal Error - unable to get items from enumeration");
            PeerEndEnumeration(hEnum); 
            return E_FAIL;
        }
    }

    // Find the one contact that matches this peer name
    hr = FindIndexOfMatchingContact(ppContacts, nCount, wzPeerName, &nIndex);
    if (FAILED(hr))
    {
        printf("Contact with peername '%S' not found in our contact store", wzPeerName);
        PeerFreeData(ppContacts);
        PeerEndEnumeration(hEnum);
        return hr;
    }

    // In case there are multiple endpoints, pick an endpoint for this contact
    hr = PickEndpointForContact(ppContacts[nIndex], &peerEndpoint);  

    // Free all the contacts and the enumeration info structs 
    //
    PeerFreeData(ppContacts);
    PeerEndEnumeration(hEnum); 

    if (FAILED(hr))
    {
        printf("Unable to find an endpoint to use to check presence.\n");
        printf("  This user is offline, or we are not watching this user's presence.\n");
        return hr;
    }

    
    // now we are finally ready to call the API to find the presence information
    hr = PeerCollabGetPresenceInfo(&peerEndpoint, &pPresInfo);
    if (SUCCEEDED(hr))
    {
        // print out the presence information
        hr = PrintPresenceInformation(pPresInfo);
    }
    else
    {
        printf("PeerCollabGetPresenceInfo failed, hr=0x%x\n", hr);
        PrintError(hr);
    }

    // free the presence info struct which was allocated by the PeerCollabGetPresenceInfo API
    PeerFreeData(pPresInfo);
    
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    SetPresenceInformation
// Purpose:     Demonstrate use of the PeerCollabSetPresenceInfo() API
// Parameters:  None
//
HRESULT SetPresenceInformation()
{
    HRESULT             hr = S_OK;
    PEER_PRESENCE_INFO  presInfo = {0};
    WCHAR               wzNumBuf[INPUT_BUFSIZE];
    WCHAR               wzStringBuf[STRING_BUFSIZE];
    int                 nInput = 0;
 
    // get input from user to fill out presence info
    printf("Enter a number for what status you want to set for your current presence status\n"
            "1. Out to lunch\n"
            "2. Away\n"
            "3. Be right back\n"
            "4. Idle\n"
            "5. Busy\n"
            "6. On the phone\n"
            "7. Online\n");
    hr = StringCbGets(wzNumBuf, sizeof(wzNumBuf));
    if (FAILED(hr))
    {
        printf ("Invalid input, expected a number between 1 and 8 (inclusive).\n");
        return E_INVALIDARG;
    }

    // since wtoi can't distinguish between 0 and an error, note that valid values start at 1, not 0    
    nInput = _wtoi(wzNumBuf);
    if (nInput < 1  || nInput > 7)
    {
        printf ("Invalid input, expected a number between 1 and 8 (inclusive).\n");
        return E_INVALIDARG;
    }

    // Convert the input value to a status value
    presInfo.status = (PEER_PRESENCE_STATUS) nInput;
    
    // enter presence status to set for yourself
    printf("Now input your detailed status as freeform text:\n");
    hr = StringCbGets(wzStringBuf, sizeof(wzStringBuf));
    if (FAILED(hr))
    {
        printf ("Invalid input.\n");
        return E_INVALIDARG;
    }
    // now set the string into the presence struct
    presInfo.pwzDescriptiveText = (LPWSTR) wzStringBuf;

    // now that we have the information we need in the struct, call the presence info api (which will make its own copy of the string)
    hr = PeerCollabSetPresenceInfo(&presInfo);

    if (FAILED(hr))
    {
        printf("Call to PeerCollabSetPresenceInfo failed, hr=0x%x\n", hr);
        PrintError(hr);
    }
    
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    PrintMenu
// Purpose:     Prints a menu of options to the user
// Parameters:  None
//
void PrintMenu()
{
    printf("\n"
           "1. Export your contact information\n"
           "2. Import someone's contact information\n"
           "3. Display contacts\n"
           "4. Grant watch permissions to contact, and start watching contact\n"
           "5. Enum contact applications\n"
           "6. Wait for contact presence change\n"
           "7. Get presence information\n"
           "8. Set presence information\n"
           "9. Quit\n"
           "> ");

}


int __cdecl main(int argc, __in_ecount(argc) char *argv[])
{
    HRESULT hr =  PeerCollabStartup(PEER_COLLAB_VERSION);

    if (SUCCEEDED(hr))
    {
        hr = PeerCollabSignin(NULL, PEER_SIGNIN_INTERNET);

        if (FAILED(hr))
        {
            printf("Error signing in. HRESULT=0x%X\n", hr);
            PrintError(hr);
        }

        // Continuously show the menu to user and respond to their request
        //
        while (SUCCEEDED(hr))
        {
            WCHAR wzBuff[INPUT_BUFSIZE];

            PrintMenu();

            hr = StringCbGets(wzBuff, sizeof(wzBuff));
            if (SUCCEEDED(hr))
            {
                switch (wzBuff[0])
                {
                    case L'1':
                        ExportContact();
                        break;
                    case L'2':
                        ImportContact();
                        break;
                    case L'3':
                        EnumContacts();
                        break;
                    case L'4':
                        GiveWatchPermissionsAndStartWatching();
                        break;
                    case L'5':
                        EnumApplications();
                        break;
                    case L'6':
                        WaitForPresenceChange();
                        break;
                    case L'7':
                        GetPresenceInformation();
                        break;
                    case L'8':
                        SetPresenceInformation();
                        break;
                    case L'9':
                        // set hr to failure, to break out of loop.
                        //
                        hr = E_FAIL;
                        break;
                    default:
                        printf("Invalid selection.\n");
                        break;
                }
            }

        }

        PeerCollabShutdown();
    }

    return 0;
}
