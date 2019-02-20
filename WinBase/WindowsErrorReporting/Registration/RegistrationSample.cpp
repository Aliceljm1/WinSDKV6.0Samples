// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include <windows.h>
#include <werapi.h>
#include <stdio.h>
#include <tchar.h>

// Sample debug struct used for registration
typedef struct _DebugInfo
{
    DWORD dwCurrentStatus;
    
} DebugInfo;

// Application Recovery Callback function
DWORD WINAPI AppRecovery (LPVOID pData)
{
    DebugInfo* pDebugInfo = NULL;
	pDebugInfo = (DebugInfo*) pData;
    BOOL bRecoveryCancelled = FALSE;

    // Note: Application Recovery supports a heartbeat model
    // that requires that the recovery function call back to
    // Windows Error Reporting once every 5 seconds to indicate
    // that recovery is still occurring.
    //
    // If the user cancels recovery then during the heartbeat callback
    // the BOOL value will be returned as FALSE.
    ApplicationRecoveryInProgress (&bRecoveryCancelled);
    if (bRecoveryCancelled)
    {
        printf("\nRecovery Cancelled\n");
        return 0;
    }

	printf("\nIn application recovery function\n");

	for (unsigned int iCount = 0; iCount < 3; iCount++)
	{
		// ... 
		// Do the actual recovery
		// ...
		printf("    %d: Performing application recovery\n", iCount);
		Sleep(1000);
	}

	// Note: Remember to callback every 5 seconds, or else recovery will
	// be terminated
	ApplicationRecoveryInProgress (&bRecoveryCancelled);
	if (bRecoveryCancelled)
	{
		printf("\nRecovery Cancelled\n");
		return 0;
	}

    // When recovery function is finished, the recovery function needs to
    // signal success or failure to Windows Error Reporting by the BOOL value
    // passed to the RecoveryFinished function.
    ApplicationRecoveryFinished(TRUE);
    
    return 0; // return value is not currently used
}

int main(int argc, const _TCHAR* argv[])
{
    HRESULT hr = S_OK;
    DebugInfo* pDebugInfo = new DebugInfo;
	int iReturn = 0;

	UNREFERENCED_PARAMETER(argv);

	printf("\nRegistration API Sample\n");
	printf("-----------------------\n");

	// If restarted by Windows, then incidate restarted and exit.
	if (argc == 2)
	{
		printf("\nRestarted with command line: '%s'\n", argv[1]);
		printf("Sleeping for 5s and then exiting.\n");
		Sleep(5000);
		goto End;
	}

    // File Registration
    // 
    // Use this API to add files to Watson CABs
    // when additional data is requested from Watson.
    // 
    // Note: Use of environment variables is allowed.
	// 
	// Note: See werapi.h for flags and type of file attachment
    // 
    // Note: Use WIN95 naming convention, with a maximum 
    // path length of MAX_PATH.
    // 
    hr = WerRegisterFile (TEXT("%TEMP%\\Application_Debug_Log.txt"), WerRegFileTypeOther, 0);
    if (S_OK != hr)
    {
        printf("\nUnable to register file, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }

	// File Unregistration
	// 
	// Use this API to unregister files previously registered.
	// 
    /*
	NOTE: Uncomment this code to see unregistration working.
	hr = WerUnRegisterFile (TEXT("%TEMP%\\Application_Debug_Log.txt"));
    if (S_OK != hr)
    {
        printf("\nUnable to register file, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }
	*/

    // Memory Block Registration
    // 
    // Use this API to add memory ranges to memory dumps
    // that are requested as additional information from Watson.
    //
    // Note: Memory must be available at time of error reporting.
    //
    // Note: Each memory range has a maximum size of 64KB.
    // 
    hr = WerRegisterMemoryBlock (pDebugInfo, sizeof(DebugInfo));
    if (S_OK != hr)
    {
        printf("\nUnable to register memory block, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }

	// Memory Block Unregistration
	// 
	// Use this API to unregister memory blocks previously registered.
	//
	/*
	NOTE: Uncomment this code to see unregistration
    hr = WerUnRegisterMemoryBlock (pDebugInfo);
    if (S_OK != hr)
    {
        printf("\nUnable to unregister memory block, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }
	*/

    // Application Recovery Registration
    //
    // Use this API to provide a callback Windows Error Reporting will
    // use when performing application recovery.
    //
    // Note: Pointer to data needs to be available at time of application
    // recovery.
    //
    // See the sample recovery function above for information on how
    // the recovery function needs to behave.
    //
    hr = RegisterApplicationRecoveryCallback (AppRecovery, pDebugInfo, 0, 0);
    if (S_OK != hr)
    {
        printf("\nUnable to register for recovery, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }

    // Application Restart Registration
    //
    // After Windows Error Reporting is complete, this API specifies
    // what command-line arguments the application should be called with
    // when it is being restarted.
    //
    // Note: If NULL is passed in, then any previous registration is removed.
    //
    // Note: The string provided will be passed after the name of the process,
    // so only command-line arguments should be passed.
    //
    // Note: Windows will only restart the application if it has been 
	// alive for a minimum of 10 seconds, to prevent cyclical restarts.
    // 
    hr = RegisterApplicationRestart (TEXT("/restarted"), 0);
    if (S_OK != hr)
    {
        printf("\nUnable to register for restart, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }
    
	// Unregistering for application restart
	//
	// Passing NULL for the string will unregister any previously registered 
	// restart command line.
	//
	/* 
	NOTE: Uncomment this code to see unregistration for application restart
    hr = RegisterApplicationRestart (NULL, 0);
    if (S_OK != hr)
    {
        printf("\nUnable to unregister for restart, Error: 0x%X\n", hr);
		iReturn = -1;
        goto End;
    }
	*/

	// Sleep for 12s to satisfy the lifetime requirement, so app will be
	// restarted
	printf("\nSleeping for 12s to ensure Windows restarts application\n");
	Sleep(12*1000);

	// Now cause an access, this will demonstrate recovery/restart functionality.
	printf("\nCausing an access violation to demonstrate recovery/restart functionality\n");
	int* p = NULL;
	*p = 5;

End:
    // clean up memory before exiting
    delete pDebugInfo;
    return iReturn;
}
