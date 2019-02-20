/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) Microsoft Corporation.  All rights reserved.

Module Name:

    rmfilterapp.cpp
 
Abstract:

	This module contains the skeleton code for a Windows native 
	API console based Restart Manager conductor process sample.

Notes:
    This application requires Windows Vista

	conductor application should use during a servicing transaction.  
	
	This sample also showcases Restart Manager filter APIs which
	show how to set shutdown/restart filter behavior during a Restart
	Manager session.

	This sample uses notepad as the target process to be shutdown and
	restarted.  It will launch notepad before actually starting the
	Restart Manager transaction.  The parts of the sample which start
	notepad are not required for RM.
       
 History:

     15-Jan-2006    a-paruss

        Creation of this sample application
        
--*/

#include <windows.h>
#include <stdio.h>
#include <restartmanager.h>

/*--

Global declarations

--*/
HRESULT StartProcess(LPCWSTR);


/*++

Routine Description:

This routine is the application primary entry point

--*/
int _cdecl wmain()
{
    DWORD dVal;
    DWORD dwSessionHandle = (DWORD) -1;
    WCHAR sessKey[CCH_RM_SESSION_KEY+1];
    RM_PROCESS_INFO rgAffectedApps[1000];
    UINT nProcInfo = 100;
    UINT nProcInfoNeeded;
    UINT nAffectedApps=ARRAYSIZE(rgAffectedApps);
    DWORD dwRebootNeeded = RmRebootReasonNone;
    DWORD nServices = 0;
    DWORD nProcs = 0;
    DWORD nFiles = 1;
    LPCWSTR rgsFiles[] = { L"c:\\windows\\system32\\notepad.exe" };
    HRESULT hr = NULL;
    WCHAR userPrompt[5];
	
    //
    // allocate filter buffer
    //
    DWORD cbFilterBufNeeded= sizeof(BYTE);
    PBYTE pbFilterBuf = (BYTE *) malloc (cbFilterBufNeeded);

    //
    // for this example, we happen to pre-launch notepad but this is
    // not required for restart manager.  Usually a target process
    // is already running before hand.
    //
    printf("Starting Sample Target Process: %S \n", rgsFiles[0]);
    hr = StartProcess(rgsFiles[0]);

    //
    // Starting Session
    //
    printf("Starting an RM Session: ");
    dVal = RmStartSession( &dwSessionHandle, 0, sessKey );
    if (dVal != ERROR_SUCCESS)
    {
        printf("Failed %d\n", dVal);
        goto RM_END;
    }
    else
    {
        printf("Success\n");
    }

    //
    // Register items
    //
    printf("Registering resources: ");
    dVal = RmRegisterResources(
                        dwSessionHandle,
                        nFiles, rgsFiles,			// Files
                        nProcs, NULL,			// Processes
                        nServices, NULL );		// Services
    if (dVal != ERROR_SUCCESS)
    {
        printf("Failed %d\n", dVal);
        goto RM_END;
    }
    else
    {
        printf("Success\n");
    }
    
    //
    // Getting affected apps
    //
    printf("Getting affected Apps: ");
    dVal = RmGetList(
                dwSessionHandle,
                &nProcInfoNeeded,
                &nAffectedApps, 
                rgAffectedApps, 
                &dwRebootNeeded );
    if (dVal != ERROR_SUCCESS)
    {
        printf("Failed %d\n", dVal);
        goto RM_END;
    }
    else
    {
        printf("Success\n");
    }

    //
    // prompt the user if they want to see affected app detail
    //
    printf("Do you want to see details of the affected apps? (Y/N)  ");
    _getws_s(userPrompt, 4);
	
    if ((0 ==(wcscmp(userPrompt, L"Y"))) || (0 ==(wcscmp(userPrompt, L"y"))))
    {

        //
        // User wants extended details
        //
        printf("  Affected Processes found: %d\n", nAffectedApps);
        if (0 == nAffectedApps)
        {
            return 0;
        }

        printf("  Reboot required: ");
        if (RmRebootReasonNone == dwRebootNeeded)
        {
            printf("TRUE\n");
        }
        else
        {
            printf("FALSE\n");
        }

        for (UINT z=0; z< nAffectedApps; z++)
        {
            printf("  Process ID: %d\n", rgAffectedApps[z].Process);
            printf("    Process: %S\n", rgAffectedApps[z].strAppName);
            printf("    Restartable: %d\n", rgAffectedApps[z].bRestartable);
            printf("    Session: %d\n", rgAffectedApps[z].TSSessionId);
            printf("    Type: %d\n", rgAffectedApps[z].ApplicationType);
        }
    }

    //
    // Setting an RmNoShutdown filter on notepad.exe
    //
    printf("Setting Filter: ");
    dVal = RmAddFilter(
                dwSessionHandle,
                rgsFiles[0],
                NULL,
                NULL,
                RmNoShutdown
                );
    if (dVal != ERROR_SUCCESS)
    {
        printf("Failed %d\n", dVal);
        goto RM_END;
    }
    else
    {
        printf("Success\n");
    }

    //
    // Verifying the filter
    //
    printf("Verifying filter: ");
    dVal = RmGetFilterList(
                    dwSessionHandle,
                    pbFilterBuf,
                    cbFilterBufNeeded,
                    &cbFilterBufNeeded
                    );
    if (dVal != ERROR_SUCCESS)
    {
        //
        // ok allocate filter buffer again to the buffer size
        // returned on the previous RmGetFilterList call
        //
        pbFilterBuf = (BYTE *) malloc (cbFilterBufNeeded);
        dVal = RmGetFilterList(
                    dwSessionHandle,
                    pbFilterBuf,
                    cbFilterBufNeeded,
                    &cbFilterBufNeeded
                    );
        if (dVal != ERROR_SUCCESS)
        {
            printf("Failed %d\n", dVal);
            goto RM_END;
        }
        else
        {
            printf("Success\n");
        }
    }
    else
    {
        printf("Success\n");
    }

    //
    // prompt the user to remove the notepad.exe no shutdown filter 
    //
    printf("Do you want to remove the filter? (Y/N)  ");
    _getws_s(userPrompt, 4);

    if ((0 ==(wcscmp(userPrompt, L"Y"))) || (0 ==(wcscmp(userPrompt, L"y"))))
    {

        //
        // User wants the filter removed
        //
        printf("Removing Filter: ");
        dVal = RmRemoveFilter(
                    dwSessionHandle,
                    rgsFiles[0],
                    NULL,
                    NULL
                    );
        if (dVal != ERROR_SUCCESS)
        {
            printf("Failed %d\n", dVal);
            goto RM_END;
        }
        else
        {
            printf("Success\n");
        }
    }

    //
    // Shut down targetted applications
    //
    printf("Shutting down %S\n",rgsFiles[0]);
    dVal = RmShutdown( dwSessionHandle, 0, NULL );
    if (dVal != ERROR_SUCCESS)
    {
        printf("Failed %d\n", dVal);
        goto RM_END;
    }

    //
    // Restart applications (only RM-aware apps will restart)
    //
    printf("Attempting to restart %S\n",rgsFiles[0]);
    dVal = RmRestart( dwSessionHandle, 0, NULL );
    if (dVal != ERROR_SUCCESS)
    {
        printf("Failed %d\n", dVal);
        goto RM_END;
    }


RM_END:

    //
    // deallocate previously allocated filter buffer
    //
    free(pbFilterBuf);
    
    //
    // tear down rm session
    //
    if (dwSessionHandle != -1)
    {
        dVal = RmEndSession( dwSessionHandle );
        dwSessionHandle = (DWORD) -1;
    }
    return 0;
}

/*++

Routine Description:

This routine is the StartProcess helper function

--*/
HRESULT StartProcess(LPCWSTR FileName)
{
    HRESULT hr = NULL;

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    //
    // Start the child process. 
    //
    if( !CreateProcessW( FileName,
                            NULL,
                            NULL,
                            NULL,
                            FALSE,
                            0,
                            NULL,
                            NULL,
                            &si,
                            &pi)
                            ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return E_FAIL;
    }

    //
    // Need to spin a bit here to allow the process to spinup
    //
    Sleep(500);

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	return hr;
}
