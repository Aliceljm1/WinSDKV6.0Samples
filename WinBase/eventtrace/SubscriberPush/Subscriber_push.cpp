//---------------------------------------------------------------------
//  
//File      : Subscriber.cpp
//
//Purpose   : This Sample demonstrates how to subscribe to real-time events from 
//            EventLog Channels.  It Registers a subscriber by provding an XPATH 
//            query and Callback.  Then, if any event is received via Callback, 
//            the Event XML is displayed on console using EvtRender.
//  This file is part of the Microsoft SDK Code Samples.
// 
//  Copyright (C) Microsoft Corporation.  All rights reserved.
// 
//This source code is intended only as a supplement to Microsoft
//Development Tools and/or on-line documentation.  See these other
//materials for detailed information regarding Microsoft code samples.
// 
//THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
//KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//PARTICULAR PURPOSE.
//---------------------------------------------------------------------


#include <windows.h>
#include <iostream>

#include <winevt.h> // EventLog Header
# pragma comment(lib, "wevtapi.lib")

using namespace std;

// Callback to receive RealTime Events.
DWORD WINAPI SubscriptionCallBack(
    EVT_SUBSCRIBE_NOTIFY_ACTION Action,
    PVOID Context, 
    EVT_HANDLE Event );


void __cdecl wmain()
{
    EVT_HANDLE hSub = NULL;   // Handle to the event subscriber.
    wchar_t *szChannel = L"Application";   // Channel.
    wchar_t *szQuery = L"*[System/Level=2]";   // XPATH Query to specify which events to subscribe to.

    wprintf(L"Subscribing to all level 2 events from the Application channel... \n");
    wprintf(L"NOTE: Hit 'Q' or 'q' to stop the event subscription\n");

    // Register the subscription.
    hSub = EvtSubscribe(  NULL,                                                                            //Session
        NULL,                    // Used for pull subscriptions.
        szChannel,               // Channel.
        szQuery,                 // XPath query.
        NULL,                    // Bookmark.
        NULL,                    // CallbackContext.
        (EVT_SUBSCRIBE_CALLBACK) SubscriptionCallBack,  // Callback.
        EvtSubscribeToFutureEvents   // Flags.
        );

    if( !hSub )
    {
        wprintf(L"Couldn't Subscribe to Events!. Error = 0x%x", GetLastError());
        return;
    }
    else
    {
        // Keep listening for events until 'q' or 'Q' is hit.
        WCHAR ch = L'0';
        do
        {
            ch = _getwch();
            ch = towupper( ch );
            Sleep(100);
        } while( ch != 'Q' );
    }    

    // Close the subscriber handle.
    EvtClose(hSub);

    wprintf(L"Event Subscription Closed !\n");
}

/*******************************************************************************************************************
Function:  CallBack
Description:  This function is called by EventLog to deliver RealTime Events. Once the event is received it is rendered
              to the console.

Return :
    DWORD is returned. 0 if succeeded, otherwise a Win32 errorcode.

********************************************************************************************************************/
DWORD WINAPI SubscriptionCallBack( 
    EVT_SUBSCRIBE_NOTIFY_ACTION Action, 
    PVOID Context, 
    EVT_HANDLE Event )
{
    WCHAR *pBuff = NULL;
    DWORD dwBuffSize = 0;
    DWORD dwBuffUsed = 0;
    DWORD dwRes = 0;
    DWORD dwPropertyCount = 0;

    // Get the XML EventSize to allocate the buffer size.
    BOOL bRet = EvtRender(  
        NULL,                // Session.
        Event,               // HANDLE.
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
                Event,                // HANDLE.
                EvtRenderEventXml,    // Flags.                                              
                dwBuffSize,           // BufferSize.
                pBuff,                // Buffer.
                &dwBuffUsed,          // Buffer size that is used or required.
                &dwPropertyCount);

            if( !bRet )
            {
                wprintf(L"Couldn't Render Events!. Error = 0x%x", GetLastError());
                delete[] pBuff;
                return dwRes;
            }
        }
    }    

    //Display the Event XML on console
    wprintf(L"The following Event is received : \n %s \n\n", pBuff);

    //Cleanup
    delete[] pBuff;

    return dwRes;
}
