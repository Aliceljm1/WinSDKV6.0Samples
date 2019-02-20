//---------------------------------------------------------------------
//  
//File      : Subscriber_pull.cpp
//
//Purpose   : This Sample demonstrates how to subscribe to real-time events from 
//            Event Log channels using a pull subscription.  
//            It Registers a subscriber by provding an XPATH 
//            query.  Then, if any event is received, 
//            the event XML is displayed on console using EvtRender.
// 
//  Copyright (C) Microsoft Corporation.  All rights reserved.
// 
// 
//THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
//KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//PARTICULAR PURPOSE.
//---------------------------------------------------------------------


#include <windows.h>
#include <wchar.h>
#include <winevt.h> // EventLog Header
# pragma comment(lib, "wevtapi.lib")

void __cdecl wmain()
{
    PWSTR szChannel = L"Application";   // Channel.
    PWSTR szQuery = L"*";   // XPATH Query to specify which events to subscribe to.

    wprintf(L"Subscribing to all events from the Application channel... \n");

    const int BatchSize = 10;
    DWORD numRead = 0;
    EVT_HANDLE batch[BatchSize];  

    HANDLE signalEvent = CreateEventW(NULL, false, false, NULL);

    // Register the subscription.
    EVT_HANDLE subscription = EvtSubscribe(  
        NULL,                    // Session
        signalEvent,             // Used for pull subscriptions.
        szChannel,               // Channel.
        szQuery,                 // XPath query.
        NULL,                    // Bookmark.
        NULL,                    // CallbackContext.
        NULL,                    // Callback.
        EvtSubscribeToFutureEvents   // Flags.
        );

    if( subscription == NULL )
    {
        wprintf(L"Couldn't Subscribe to Events!. Error = 0x%x", GetLastError());
        return;
    }
    else
    {
        DWORD result = ERROR_SUCCESS;
        while( result == ERROR_SUCCESS )
        {
            if( EvtNext( subscription, BatchSize, batch, -1, 0, &numRead) )
            {
                // Do something with numRead event handles in the batch array.
                // For example, render the events.

                for( DWORD i=0; i < numRead; i++)
                {
                    // Render the events in the array
                    WCHAR *pBuff = NULL;
                    DWORD dwBuffSize = 0;
                    DWORD dwBuffUsed = 0;
                    DWORD dwRes = 0;
                    DWORD dwPropertyCount = 0;

                    // Get the XML EventSize to allocate the buffer size.
                    BOOL bRet = EvtRender(  
                        NULL,                // Session.
                        batch[i],            // EVT_HANDLE.
                        EvtRenderEventXml,   // Flags.                                              
                        dwBuffSize,          // BufferSize.
                        pBuff,               // Buffer.
                        &dwBuffUsed,         // Buffersize that is used or required.
                        &dwPropertyCount);

                    if (!bRet)
                    {
                        dwRes = GetLastError();
                        if( dwRes == ERROR_INSUFFICIENT_BUFFER )
                        {
                            // Allocate the buffer size needed to for the XML event.
                            dwBuffSize = dwBuffUsed;
                            pBuff = new WCHAR[dwBuffSize/sizeof(WCHAR)];
                            
                            //Get the Event XML
                            bRet = EvtRender(   
                                NULL,                 // Session.
                                batch[i],             // EVT_HANDLE.
                                EvtRenderEventXml,    // Flags.                                              
                                dwBuffSize,           // BufferSize.
                                pBuff,                // Buffer.
                                &dwBuffUsed,          // Buffer size that is used or required.
                                &dwPropertyCount);

                            if( !bRet )
                            {
                                wprintf(L"Couldn't Render Events!. Error = 0x%x", GetLastError());
                                delete[] pBuff;
                                // Close the subscriber handle.
                                for(DWORD j=0; j< numRead; j++)
                                {
                                    EvtClose(batch[j]);
                                }
                                return;
                            }
                        }
                    }    

                    //Display the Event XML on console
                    wprintf(L"The following Event is received : \n %s \n\n", pBuff);

                    //Cleanup
                    delete[] pBuff;
                    EvtClose(batch[i]);
                }
            }
            else
            {

                DWORD waitResult = 0;
                result = GetLastError();
                if( result == ERROR_NO_MORE_ITEMS )
                {
                    // wait for the subscription results
                    waitResult = WaitForSingleObject( signalEvent, INFINITE );
                    if( waitResult == WAIT_OBJECT_0 )
                    {
                        result = ERROR_SUCCESS;
                    }
                    else
                    {
                        result = GetLastError();
                        break;
                    }
                }
            }
        }
    }    

    // Close the subscriber handle.
    EvtClose(subscription);

    CloseHandle(signalEvent);

    wprintf(L"Event Subscription Closed !\n");
}
