//---------------------------------------------------------------------
// File      : QueryLog.cpp
//
// Purpose   : This Sample demonstrates how to query EventLog Logs.  It first 
//             submits the log query to EventLog.  Then, the events from queryresults are
//             retrived one at a time and displayed on console. This file is part of the Microsoft SDK Code Samples.
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
#include <tchar.h>

#include "winevt.h"	//EventLog Header

using namespace std;

#define DEFAULT_BUFF_SIZE 1024
#define QUERY_TIMEOUT 5000

void __cdecl wmain()
{
    EVT_HANDLE hQueryResult   = NULL;                                                                // handle to the loq query results
    wchar_t *szChannel              = L"Microsoft.com/SamplePublisher/Operational";    //Channel
    wchar_t *szQuery                 = L"*";                                                                  //XPATH Query (All Events)

    wprintf(L"Querying Microsoft.com/SamplePublisher/Operational Channel Log for all Events... \n");

    //Query the Log
    hQueryResult = EvtQuery(NULL,                       		  //Context
                                            szChannel,                          //Channel
                                            szQuery,                    		 //Query
                                            EvtQueryChannelPath	        //Flags
                                            );
    if( !hQueryResult )
    {
        wprintf(L"Failed to Query the Log!. Error = 0x%x", GetLastError());
        return;
    }

    EVT_HANDLE hEvent = NULL;      	//Events Array
    DWORD dwReturned = 0;      //Number of Events actually returned
    DWORD dwNumofEvents = 0;

    //Retrieve each event in the QueryResult and display on console
    while (EvtNext(   hQueryResult,	 	//QueryResult
                                1, 				//BatchSize
                                &hEvent, 			//EventArray
                                QUERY_TIMEOUT,	//TimeOut
                                0, 				//dwFlags
                                &dwReturned) )	       //Returned
    {
        WCHAR *pBuff                     = NULL;      	//Buffer
        DWORD dwBuffSize             = 0;            //BufferSize
        DWORD dwBuffUsed            = 0;           //Buffersize used/required
        DWORD dwPropertyCount   = 0;

        //Get the BufferSize needed to render the Event
        BOOL bRet = EvtRender(  NULL,                       //Context
                                                hEvent,    	                //HANDLE
                                                EvtRenderEventXml,  //Flags                                                
                                                dwBuffSize,              //BufferSize
                                                pBuff,                      //Buffer
                                                &dwBuffUsed,            //Buffersize used/required
                                                &dwPropertyCount
                                                );
        if (!bRet)
        {
            DWORD dwRes = GetLastError();
            if( dwRes == ERROR_INSUFFICIENT_BUFFER )
            {
                //Allocate the BufferSize needed
                dwBuffSize = dwBuffUsed;
                pBuff = new WCHAR[dwBuffSize/sizeof(WCHAR)];

                //Render the Event
                bRet = EvtRender(   NULL,                       //Context
                                                hEvent,    	                //HANDLE
                                                EvtRenderEventXml,  //Flags                                                
                                                dwBuffSize,              //BufferSize
                                                pBuff,                      //Buffer
                                                &dwBuffUsed,            //Buffersize used/required
                                                &dwPropertyCount
                                                );
                if( !bRet )
                {
                    wprintf(L"Couldn't Render Event!. Error = 0x%x", GetLastError());
                    delete[] pBuff;
                    EvtClose(hQueryResult);
                    return;
                }

                //Display the Event XML on console
                wprintf(L"Event %d : \n %s \n\n", ++dwNumofEvents, pBuff);
                delete[] pBuff;
            }
            else
            {
                wprintf(L"EvtRender failed to get the buffersize needed to Render the Event!. Error = 0x%x", GetLastError());
                EvtClose(hQueryResult);
                return;
            }
        } 
    }

    wprintf(L"Success\n");

    //Close the QueryResults Handle
    EvtClose(hQueryResult);
}
