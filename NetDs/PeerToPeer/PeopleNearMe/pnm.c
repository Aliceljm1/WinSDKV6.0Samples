/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) 1985-2005 Microsoft Corporation. All Rights Reserved.

Abstract:
    This C file includes sample code for enumerating people near me
    with the Microsoft Peer-to-Peer Collaboration APIs.

Feedback:
    If you have any questions or feedback, please contact us using
    any of the mechanisms below:

    Email: peerfb@microsoft.com
    Newsgroup: Microsoft.public.win32.programmer.networks
    Website: http://www.microsoft.com/p2p

--********************************************************************/

#include <p2p.h>
#include <stdio.h>
#include <strsafe.h>
#include <string.h>
#include <rpc.h>

#define ERRBUFSIZE 1024
#define HRESULTTOWIN32(hres)                                \
            ((HRESULT_FACILITY(hres) == FACILITY_WIN32)     \
                ? HRESULT_CODE(hres)                        \
                : (hres))

PWSTR g_rgPublicationScope[] =
{
    L"None",
    L"Near Me",
    L"Internet",
    L"All"
};

PWSTR g_rgEventType[] = 
{
    L"Added",
    L"Deleted",
    L"Updated"
};

CONST GUID MESSAGE_GUID = { /* 191312ce-4466-4dc4-a8d6-cb2e9710157f */
    0x191312ce,
    0x4466,
    0x4dc4,
    {0xa8, 0xd6, 0xcb, 0x2e, 0x97, 0x10, 0x15, 0x7f}
  };

HPEEREVENT          g_hPeerEvent = NULL;
HANDLE              g_hWaitObject = NULL;
HANDLE              g_hEvent = NULL;

//-----------------------------------------------------------------------------
// Function:    PrintError
// Purpose:     Prints the error string associated with a given hresult
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
// Function:   PrintApplication
// Purpose:    Prints out an application object
// Parameters:
//   pApplication : [in] pointer to PEER_APPLICATION object
//
VOID DisplayApplication(__in PCPEER_APPLICATION pApplication)
{
    PWSTR   pwzApplicationId = NULL;

    wprintf(L"Application:\n");

    if (UuidToString((UUID* )&pApplication->id, &pwzApplicationId) == RPC_S_OK)
    {
        wprintf(L"\tId: %s\n", pwzApplicationId);
        RpcStringFree(&pwzApplicationId);
    }

    if (pApplication->pwzDescription != NULL)
    {
        wprintf(L"\tDescription: %s\n", pApplication->pwzDescription);
    }

    wprintf(L"\tData Size: %d\n\n", pApplication->data.cbData);

}

//-----------------------------------------------------------------------------
// Function:   DisplayObject
// Purpose:    Prints out a PEER_OBJECT
// Parameters:
//   pObject : [in] pointer to PEER_OBJECT structure
//
VOID DisplayObject(__in PCPEER_OBJECT pObject)
{
    PWSTR   pwzObjectId = NULL;
    
    wprintf(L"Object:\n");

    if (UuidToString((UUID* )&pObject->id, &pwzObjectId) == RPC_S_OK)
    {
        wprintf(L"\tId: %s\n", pwzObjectId);
        RpcStringFree(&pwzObjectId);
    }

    wprintf(L"\tSize: %d\n", pObject->data.cbData);

    // Displays contents of message object when object's GUID equals MESSAGE_GUID
    if (pObject->data.cbData != 0 &&
        memcmp(&(pObject->id), &(MESSAGE_GUID), sizeof(pObject->id)) == 0)
    {
        wprintf(L"\tData: %s\n", (PWSTR) pObject->data.pbData);
    }
}

//-----------------------------------------------------------------------------
// Function:   DisplayEndpointInfo
// Purpose:    Prints out endpoint information from a PEER_PEOPLE_NEAR_ME array
// Parameters:
//   cEndpoints  : [in] number of endpoints retrieved
//   ppEndpoints : [in] pointer to an array of PEER_PEOPLE_NEAR_ME
//
VOID DisplayPeopleNearMeInfo(ULONG cPeopleNearMe, __in PEER_PEOPLE_NEAR_ME **ppPeopleNearMe)
{
    ULONG i = 0;
    WCHAR wzAddr[256] = {0};

    wprintf(L"\nPeople Near Me: \n");
    for (i = 0; i < cPeopleNearMe; i++)
    {
        DWORD dwLen = (sizeof(wzAddr) / sizeof(wzAddr[0]));
        
        // Print out nickname
        wprintf(L"%d) %s, ", i+1, ppPeopleNearMe[i]->pwzNickName);
              
        // Print out address
        if (WSAAddressToString(
                (LPSOCKADDR) &(ppPeopleNearMe[i]->endpoint.address.sin6),
                sizeof(SOCKADDR_IN6),
                NULL,
                wzAddr,
                &dwLen) != SOCKET_ERROR)
        {
            wprintf(L"%s\n", wzAddr);
        }
        else
        {
            wprintf(L"Error parsing address\n");
        }

        // Print out endpoint name, if applicable
        if (ppPeopleNearMe[i]->endpoint.pwzEndpointName)
        {
            wprintf(L"\tEndpoint Name: %s\n", ppPeopleNearMe[i]->endpoint.pwzEndpointName);
        }
    }
}

//-----------------------------------------------------------------------------
// Function:    DisplayEndpointObjects
// Purpose:     Retrieves and displays object information from an endpoint. 
//              Should call PeerCollabRefreshEndpointData before using this function.
// Parameters:  
//   PCPEER_ENDPOINT : [in] endpoint about which information is displayed
//
HRESULT DisplayEndpointObjects(__in PCPEER_ENDPOINT pcEndpoint)
{
    HPEERENUM           hObjectEnum = NULL;
    ULONG               cObjects = 0;
    ULONG               i = 0;
    PPEER_OBJECT*       ppObjects = NULL;
    HRESULT             hr = S_OK;

    // Get a list of objects published by this endpoint - The NULL parameter
    // indicates that we wish to retrieve all objects
    hr = PeerCollabEnumObjects(pcEndpoint, NULL, &hObjectEnum);
    
    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hObjectEnum, &cObjects);
        if (SUCCEEDED(hr))
        {
            if (cObjects == 0)
            {
                wprintf(L"No objects found for this endpoint\n");
                PeerEndEnumeration(hObjectEnum);        
                return hr;
            }

            hr = PeerGetNextItem(hObjectEnum, &cObjects, (PVOID **) &ppObjects);
            if (SUCCEEDED(hr))
            {
                for (i = 0; i < cObjects; i++)
                {
                    DisplayObject(ppObjects[i]);
                }
                PeerFreeData(ppObjects);
            }
            else
            {
                wprintf(L"Error retrieving objects\nHRESULT=0x%x\n", hr);
                PrintError(hr);
            }
        }
        PeerEndEnumeration(hObjectEnum);
    }
    else
    {
        wprintf(L"Error retrieving object info.\n");

    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    DisplayEndpointApplications
// Purpose:     Retrieves and displays application information from an endpoint
// Parameters:  
//   PCPEER_ENDPOINT : [in] endpoint about which information is displayed
//
HRESULT DisplayEndpointApplications(__in PCPEER_ENDPOINT pcEndpoint)
{
    PPEER_APPLICATION*  ppApplications = NULL;
    HPEERENUM           hApplicationEnum;
    ULONG               cApplications = 0;
    ULONG               i = 0;
    HRESULT             hr = S_OK;

    // Get a list of applications from the endpoint - The NULL parameter 
    // indicates that we want to retrieve all applications
    hr = PeerCollabEnumApplications(pcEndpoint, NULL, &hApplicationEnum);

    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hApplicationEnum, &cApplications);
        if (SUCCEEDED(hr))
        {
            if (cApplications == 0)
            {
                wprintf(L"No applications found for this endpoint.\n");
                PeerEndEnumeration(hApplicationEnum);        
                return hr;
            }

            hr = PeerGetNextItem(hApplicationEnum, &cApplications, (PVOID **) &ppApplications);
            if (SUCCEEDED(hr))
            {
                for (i = 0; i < cApplications; i++)
                {
                    DisplayApplication(ppApplications[i]);
                }
                PeerFreeData(ppApplications);
            }
            else
            {
                wprintf(L"Error retrieving applications\nHRESULT=0x%x\n", hr);
                PrintError(hr);
            }
        }
        PeerEndEnumeration(hApplicationEnum);
    } 
    else
    {
        wprintf(L"Error retrieving application info.\n");
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function:    RefreshEndpoint
// Purpose:     Requests a refresh of a specific endpoint's data.
//              Note that if a subscription to the endpoint exists via
//              PeerCollabSubscribeEndpoint, this is not necessary.
// Parameters:  
//   pcEndpoint : [in] endpoint to refresh
//
HRESULT RefreshEndpoint(__in PCPEER_ENDPOINT pcEndpoint)
{
    HPEEREVENT            hPeerEvent = NULL;
    HRESULT               hr = S_OK;
    PEER_COLLAB_EVENT_REGISTRATION  eventReg = {0};
    HANDLE                hEvent;

    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEvent == NULL)
    {
        return E_OUTOFMEMORY;
    }

    eventReg.eventType = PEER_EVENT_REQUEST_STATUS_CHANGED;
    eventReg.pInstance = NULL;

    // Register to be notified when the request finishes  
    hr = PeerCollabRegisterEvent(hEvent, 1, &eventReg, &hPeerEvent);

    if (SUCCEEDED(hr))
    {
        hr = PeerCollabRefreshEndpointData(pcEndpoint);
        if (FAILED(hr))
        {
            wprintf(L"Failed to refresh endpoint data\nHRESULT=0x%x", hr);
            PrintError(hr);
            PeerCollabUnregisterEvent(hPeerEvent);
            CloseHandle(hEvent);
            return hr;
        }   

        wprintf(L"Refreshing endpoint data...\n");

        // Block until a event is set indicating that endpoint data has
        // successfully been refreshed
        if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
        {
            PEER_COLLAB_EVENT_DATA *pEventData = NULL;

            // Find out if refresh request succeeded
            hr = PeerCollabGetEventData(hPeerEvent, &pEventData);

            if (SUCCEEDED(hr) && 
                SUCCEEDED(pEventData->requestStatusChangedData.hrChange))
            {
                wprintf(L"Endpoint successfully refreshed.\n");
                PeerFreeData(pEventData);
            }
        }
        PeerCollabUnregisterEvent(hPeerEvent);
    }

    CloseHandle(hEvent);
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    AddEndpointAsContact
// Purpose:     Takes endpoint information as input, attempts to retrieve that
//              endpoint's contact data and add it to the contact store
// Parameters:  
//   pcEndpoint [in] : pointer to Endpoint that we wish to add as a contact
//
HRESULT AddEndpointAsContact(__in PCPEER_ENDPOINT pcEndpoint)
{
    HRESULT         hr = S_OK;
    PWSTR           pwzContactData = NULL;
    PPEER_CONTACT   pContact = NULL;

    hr = RefreshEndpoint(pcEndpoint);
    if (SUCCEEDED(hr))
    {
        hr = PeerCollabQueryContactData(pcEndpoint, &pwzContactData);
        if (SUCCEEDED(hr))
        {
            hr = PeerCollabAddContact(pwzContactData, &pContact);
            if (SUCCEEDED(hr))
            {
                wprintf(L"Successfully added endpoint as contact.\n");

                // In order to successfully propagate the contact changes,
                // the watch permissions must be set and the contact updated
                pContact->fWatch = TRUE;
                pContact->WatcherPermissions = PEER_WATCH_ALLOWED;
                hr = PeerCollabUpdateContact(pContact);
                if (FAILED(hr))
                {
                    wprintf(L"Failed to update watch permissions on contact\n");
                }

                PeerFreeData(pContact);
            }
            else
            {
                wprintf(L"PeerCollabAddContact failed.\nHRESULT=0x%x\n", hr);
            }
            PeerFreeData(pwzContactData);        
        } 
        else
        {
            wprintf(L"PeerCollabQueryContactData failed.\nHRESULT=0x%x\n", hr);
        }
    }
    else
    {
        wprintf(L"RefreshEndpoint failed.\nHRESULT=0x%x\n", hr);
    }

    if (FAILED(hr))
    {
        PrintError(hr);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function:    SubscribeEndpointData
// Purpose:     Takes an endpoint and a flag to subscribe to
//              changes in a specific endpoint's data
// Parameters:  
//   pcEndpoint [in] : pointer to endpoint whose data we wish to subscribe to
//
HRESULT SubscribeEndpointData(__in PCPEER_ENDPOINT pcEndpoint)
{
    HRESULT         hr = S_OK;
    
    hr = PeerCollabSubscribeEndpointData(pcEndpoint);
    if (SUCCEEDED(hr))
    {
        wprintf(L"Successfully subscribed to endpoint data.\n");
    }
    else
    {
        wprintf(L"PeerCollabSubscribeEndpointData failed.\nHRESULT=0x%x\n", hr);
        PrintError(hr);
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    UnsubscribeEndpointData
// Purpose:     Takes an endpoint and a flag to unsubscribe to
//              changes in a specific endpoint's data
// Parameters:  
//   pcEndpoint [in] : pointer to Endpoint whose data we wish to unsubscribe to
//
HRESULT UnsubscribeEndpointData(__in PCPEER_ENDPOINT pcEndpoint)
{
    HRESULT         hr = S_OK;

    hr = PeerCollabUnsubscribeEndpointData(pcEndpoint);
    if (SUCCEEDED(hr))
    {
        wprintf(L"Successfully unsubscribed to endpoint data.\n");
    }
    else
    {
        wprintf(L"PeerCollabUnsubscribeEndpointData failed.\nHRESULT=0x%x\n", hr);
        PrintError(hr);
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    DisplayPeopleNearMe
// Purpose:     Simple wrapper function that calls PeerCollabEnumPeopleNearMe
//              and displays the results.  Setting fPrompt flag prompts user
//              to select an endpoint for further information.
// Parameters:  (none)
//
HRESULT DisplayPeopleNearMe()
{
    PEER_PEOPLE_NEAR_ME**    ppPeopleNearMe = NULL;
    HRESULT                  hr = S_OK;
    HPEERENUM                hEnum = NULL;
    ULONG                    count = 0;
    ULONG                    ulEndpoint = 0;   
    WCHAR                    wzInputBuffer[5] = {0};
 
    hr = PeerCollabEnumPeopleNearMe(&hEnum);

    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hEnum, &count);

        if (SUCCEEDED(hr))
        {
            if (count == 0)
            {
                wprintf(L"No people near me found\n");
                PeerEndEnumeration(hEnum);
                return hr;
            }
            hr = PeerGetNextItem(hEnum, &count, (PVOID **) &ppPeopleNearMe);
        }

        if (SUCCEEDED(hr))
        {
            DisplayPeopleNearMeInfo(count, ppPeopleNearMe);            
            PeerFreeData(ppPeopleNearMe);
        } 
        else
        {
            wprintf(L"Error retrieving endpoints, HRESULT=0x%x", hr);
            PrintError(hr);
        }
        PeerEndEnumeration(hEnum);        
    }
    else
    {
        wprintf(L"PeerCollabEnumPeopleNearMe failed.\nHRESULT=0x%x\n", hr);
        PrintError(hr);
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    SelectEndpoint
// Purpose:     Returns an endpoint selected by the user from a menu of all
//              available endpoints
// Parameters:  (none)
//
HRESULT SelectEndpoint(__out PPEER_ENDPOINT pEndpoint)
{
    PEER_PEOPLE_NEAR_ME**    ppPeopleNearMe = NULL;
    HRESULT                  hr = S_OK;
    HPEERENUM                hEnum = NULL;
    ULONG                    count = 0;
    ULONG                    ulEndpoint = 0;   
    WCHAR                    wzInputBuffer[5] = {0};
 
    hr = PeerCollabEnumPeopleNearMe(&hEnum);

    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hEnum, &count);

        if (SUCCEEDED(hr))
        {
            if (count == 0)
            {
                wprintf(L"No people near me found\n");
                PeerEndEnumeration(hEnum);
                return hr;
            }
            hr = PeerGetNextItem(hEnum, &count, (PVOID **) &ppPeopleNearMe);
        }

        if (SUCCEEDED(hr))
        {
            DisplayPeopleNearMeInfo(count, ppPeopleNearMe);
        
       
            // Loop until valid selection is made
            while (ulEndpoint == 0)
            {
                wprintf(L"\nSelect Endpoint (1-%d): ", count);
                hr = StringCbGets(wzInputBuffer, sizeof(wzInputBuffer));
                if (SUCCEEDED(hr))
                {
                    ulEndpoint = _wtoi(wzInputBuffer);
                    if (ulEndpoint < 1 || ulEndpoint > count)
                    {
                        wprintf(L"\nInvalid Selection\n");
                        ulEndpoint = 0;
                    }
                    else
                    {
                        ZeroMemory(pEndpoint, sizeof(*pEndpoint));
                        pEndpoint->address = ppPeopleNearMe[ulEndpoint-1]->endpoint.address;
                    }
                }
            }
            PeerFreeData(ppPeopleNearMe);
        }
        
        if (FAILED(hr))
        {
            wprintf(L"Error displaying endpoints, HRESULT=0x%x", hr);
            PrintError(hr);
        }
        PeerEndEnumeration(hEnum);        
    }
    else
    {
        wprintf(L"PeerCollabEnumPeopleNearMe failed.\nHRESULT=0x%x", hr);
        PrintError(hr);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function:    PublishObject
// Purpose:     Publishes a sample object (a string message) to People Near Me
// Parameters:  None
//
HRESULT PublishObject()
{
    PEER_OBJECT   object = {0};
    WCHAR         wzBuff[256] = {0};
    HRESULT       hr = S_OK;

    wprintf(L"\nObject Data (String): ");
    hr = StringCbGets(wzBuff, sizeof(wzBuff));
    if (SUCCEEDED(hr))
    {
        object.id = MESSAGE_GUID;
        object.data.cbData = (ULONG) (wcslen(wzBuff) + 1) * sizeof(WCHAR);
        object.data.pbData = (PBYTE) wzBuff;
        object.dwPublicationScope = PEER_PUBLICATION_SCOPE_NEAR_ME;
        hr = PeerCollabSetObject(&object);

        if (SUCCEEDED(hr))
        {
            wprintf(L"Object published.\n");
        }
        else
        {
            wprintf(L"PeerCollabSetObject failed\nHRESULT=0x%x\n", hr);
            PrintError(hr);
        }
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function:    ProcessUpdateCallBack
// Purpose:     Called when PEER_EVENT_PEOPLE_NEAR_ME_CHANGED event is received
//              Reads in event data and displays the type of change
// Parameters:  
//   lpContext [in] : Not used
//   fTimer    [in] : Not Used
//
VOID CALLBACK ProcessUpdateCallBack(LPVOID lpContext, BOOLEAN fTimer)
{
    PEER_COLLAB_EVENT_DATA *pEventData = NULL;
    HRESULT hr = S_OK;

    hr = PeerCollabGetEventData(g_hPeerEvent, &pEventData);
    while (SUCCEEDED(hr) && pEventData)
    {        
        wprintf(L"Endpoint %s", g_rgEventType[pEventData->peopleNearMeChangedData.changeType]);
        if (pEventData->peopleNearMeChangedData.pPeopleNearMe == NULL)
        {
            wprintf(L": <Me>\n");
        }
        else
        {
            wprintf(L": %s\n", pEventData->peopleNearMeChangedData.pPeopleNearMe->pwzNickName);
        }
        PeerFreeData(pEventData);

        hr = PeerCollabGetEventData(g_hPeerEvent, &pEventData);
    }

    if (FAILED(hr))
    {
        wprintf(L"PeerCollabGetEventData failed\nHRESULT=0x%x\n", hr);
        PrintError(hr);
    }
}

//-----------------------------------------------------------------------------
// Function:    EventHandler
// Purpose:     Watches for changes in endpoint data and displays endpoint data
//              when a PEER_EVENT_REQUEST_STATUS_CHANGED event is received.  This 
//              event is fired when an endpoint is refreshed or when an subscribed
//              endpoint's data changes.
// Parameters:  None
//
DWORD WINAPI EventHandler(LPVOID lpv)
{
    HPEEREVENT            hPeerEvent = NULL;
    HRESULT               hr = S_OK;
    PEER_COLLAB_EVENT_REGISTRATION  eventReg[3] = {0};
    HANDLE                hEvent;

    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEvent == NULL)
    {
        return E_OUTOFMEMORY;
    }

    eventReg[0].eventType = PEER_EVENT_REQUEST_STATUS_CHANGED;
    eventReg[0].pInstance = NULL;

    eventReg[1].eventType = PEER_EVENT_ENDPOINT_APPLICATION_CHANGED;
    eventReg[1].pInstance = NULL;

    eventReg[2].eventType = PEER_EVENT_ENDPOINT_OBJECT_CHANGED;
    eventReg[2].pInstance = NULL;

    // Register to be notified when the request finishes  
    hr = PeerCollabRegisterEvent(hEvent, 3, eventReg, &hPeerEvent);  

    if (SUCCEEDED(hr))
    {
        while (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
        {
            PEER_COLLAB_EVENT_DATA *pEventData = NULL;

            // retrieve all event data 
            while (PeerCollabGetEventData(hPeerEvent, &pEventData) == S_OK)
            {
                switch(pEventData->eventType)
                {
                case PEER_EVENT_ENDPOINT_OBJECT_CHANGED:
                    (void) DisplayObject(pEventData->objectChangedData.pObject);
                    break;
                case PEER_EVENT_ENDPOINT_APPLICATION_CHANGED:
                    (void) DisplayApplication(pEventData->applicationChangedData.pApplication);
                    break;
                case PEER_EVENT_REQUEST_STATUS_CHANGED:
                    if (FAILED(pEventData->requestStatusChangedData.hrChange))
                    {
                        wprintf(L"Retrieve endpoint data failed\nHRESULT=0x%x\n",
                                pEventData->requestStatusChangedData.hrChange);
                        PrintError(pEventData->requestStatusChangedData.hrChange);
                    }
                    break;
                }
                PeerFreeData(pEventData);
            }
        }
    }
    else
    {
        wprintf(L"PeerCollabRegisterEvent failed.\nHRESULT=0x%x\n", hr);
        PrintError(hr);
    }
    return 0; 
}

//-----------------------------------------------------------------------------
// Function:    MonitorPeopleNearMe
// Purpose:     Watches for change events in PeopleNearMe data
// Parameters:  None
//
HRESULT MonitorPeopleNearMe()
{
    HRESULT                  hr = S_OK;
    PEER_COLLAB_EVENT_REGISTRATION  eventReg = {0};
    
    
    g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (g_hEvent == NULL)
    {
        return E_OUTOFMEMORY;
    }

    eventReg.eventType = PEER_EVENT_PEOPLE_NEAR_ME_CHANGED;
    eventReg.pInstance = NULL;

    // Register to be notified when the People Near Me change.  
    hr = PeerCollabRegisterEvent(g_hEvent, 1, &eventReg, &g_hPeerEvent);

    if (SUCCEEDED(hr))
    {
        // Registers a wait object that will call ProcessUpdateCallBack
        // whenever a PEER_EVENT_PEOPLE_NEAR_ME_CHANGED is received.
        if (!RegisterWaitForSingleObject(&g_hWaitObject,
                                    g_hEvent, 
                                    ProcessUpdateCallBack,
                                    NULL,
                                    INFINITE,
                                    0))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            wprintf(L"RegisterWaitForSingleObject failed\nHRESULT=0x%x\n", hr);
        }
        else
        {
            wprintf(L"Monitoring endpoint changes...\n");
        }
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
    wprintf(L"\n1. Enumerate People Near Me\n"
           L"2. Display information about an endpoint\n"
           L"3. Add endpoint to contacts\n"
           L"4. Subscribe to an endpoint's data\n"
           L"5. Unsubscribe from an endpoint's data\n"
           L"6. Publish an object to People Near Me\n"
           L"7. Quit\n"
           L"> ");
}

//-----------------------------------------------------------------------------
// Function:    main
// Purpose:     Program entry point
// Parameters:
//      argc  :  count of program arguments
//      argv  :  program arguments
//
int __cdecl main(int argc, __in_ecount(argc) char *argv[])
{
    HRESULT hr = S_OK;
    WCHAR wzBuff[5];
    PEER_ENDPOINT endpoint = {0};

    wprintf(L"Starting up People Near Me...\n");
    hr =  PeerCollabStartup(PEER_COLLAB_VERSION);

    if (SUCCEEDED(hr))
    {
        hr = PeerCollabSignin(NULL, PEER_SIGNIN_NEAR_ME);

        if (SUCCEEDED(hr))
        {
            if (!CreateThread(NULL, 0, EventHandler, NULL, 0, NULL))
            {                                
                hr = E_FAIL;
                wprintf(L"CreateThread failed\n");
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = MonitorPeopleNearMe();
            if (FAILED(hr))
            {
                wprintf(L"Error monitoring PNM\nHRESULT=0x%x\n", hr);
            }
        }
        else
        {
            wprintf(L"Error signing in\nHRESULT=0x%x\n", hr);
            PrintError(hr);
        }
        
    
        while (SUCCEEDED(hr))
        {
            PrintMenu();

            hr = StringCbGets(wzBuff, sizeof(wzBuff));

            if (SUCCEEDED(hr))
            {
                switch (wzBuff[0])
                {
                    case L'1':
                        (void) DisplayPeopleNearMe();
                        break;
                    case L'2':
                    case L'3':
                    case L'4':
                    case L'5':
                        hr = SelectEndpoint(&endpoint);
                        switch (wzBuff[0])
                        {
                        case L'2':
                            RefreshEndpoint(&endpoint);
                            DisplayEndpointObjects(&endpoint);
                            DisplayEndpointApplications(&endpoint);
                            break;
                        case L'3':
                            AddEndpointAsContact(&endpoint);
                            break;
                        case L'4':
                            SubscribeEndpointData(&endpoint);
                            break;
                        case L'5':
                            UnsubscribeEndpointData(&endpoint);
                            break;
                        }
                        break;
                    case L'6':
                        (void) PublishObject();
                        break;
                    case L'7':
                        // set hr to failure, to break out of loop.
                        hr = E_FAIL;
                        break;
                    default:
                        wprintf(L"Invalid selection.\n");
                        break;
                }
            }
        }

        // Like all well-behaved applications built on the PeerCollab API,
        // we do NOT call PeerCollabSignout here.

        if (g_hWaitObject != NULL)
        {
            UnregisterWait(g_hWaitObject);
        }
        if (g_hPeerEvent != NULL)
        {
            PeerCollabUnregisterEvent(g_hPeerEvent);
        }
        if (g_hEvent != NULL)
        {
            CloseHandle(g_hEvent);
        }

        PeerCollabShutdown();
    }
}
