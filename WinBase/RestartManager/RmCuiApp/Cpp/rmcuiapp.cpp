/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) Microsoft Corporation.  All rights reserved.

Module Name:

    rmcuiapp.cpp
 
Abstract:

	This module contains the skeleton code for a Windows native 
	API based CUI application that is Restart Manager and Windows 
	Error Reporting aware.

Notes:
    This application requires Windows Vista
    
    This sample supports being shutdown and restarted by a Restart 
    Manager API enabled application or by the Windows Error 
    Reporting service.

    This sample supports being shutdown by the user issued ctrl+c 
    or ctrl+break signal events during program execution 
    
    This sample supports being terminated by system logoff, system
    close app and system shutdown events.   

 History:

     23-Nov-2005    a-paruss

        Creation of this sample application
        
--*/
#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <windows.h>
#include <stdio.h>

/*--

Global declarations

--*/
WCHAR CommandLineParameter[]=L"";
BOOL TerminateRequest=FALSE;
const INT MAXBUFFER = 1000;
HANDLE StandardInConsole={0};

BOOL ControlHandlerFunction(
	DWORD
	);

void ProcessUserIo(
	);

void ErrorHandler(
	WCHAR *,
	DWORD
	);

DWORD RecoveryCallback(
	PVOID //pvParameter
	);

BOOL RegisterForRecovery(
	);


/*++

Routine Description:

This routine is the application primary entry point

--*/
int _cdecl wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	BOOL Success;

    //
	// Obtain the current console input handle
    //
	StandardInConsole = GetStdHandle(STD_INPUT_HANDLE);
	if ((StandardInConsole == NULL) || (StandardInConsole == INVALID_HANDLE_VALUE))
	{
        ErrorHandler(L"Error: In GetStdHandle", GetLastError());
		return 0;
	}

    //
	// register a ctrl handler callback routine
    //
	Success = SetConsoleCtrlHandler((PHANDLER_ROUTINE) ControlHandlerFunction, TRUE);
	if (!Success)
	{
        ErrorHandler(L"Error: In SetConsoleCtrlHandler", GetLastError());
	}

	//
    // register the application for restart
    //
	if (RegisterApplicationRestart(CommandLineParameter, 0) != S_OK)
	{
        //
		// log an event or do some error handling
        //
	}

	// register the applications hang recovery callback routine
	if (RegisterApplicationRecoveryCallback((APPLICATION_RECOVERY_CALLBACK)RecoveryCallback, (
										PVOID)NULL,
                                        RECOVERY_DEFAULT_PING_INTERVAL,
                                        0) != S_OK)
	{
        //
		// log an event or do some error handling
        //
	}

	//
    // set title of the console window
    //
	DWORD ProcessId = GetProcessId(GetCurrentProcess());
	WCHAR TitleBuffer[MAXBUFFER];
	_itow_s(ProcessId, TitleBuffer, MAXBUFFER, 10);
	wcscat_s(TitleBuffer, MAXBUFFER, L" - RMCUIAPP - RM Aware Console App v1.0 ");
	SetConsoleTitle(TitleBuffer);

	//
    // notify the user of the options they have to terminate this application
    //
	wprintf_s(L"Press CTRL+BREAK or CTRL+C to end this application\n\n");
	
	//
    // launch the primary user I/O routine.
    //
	ProcessUserIo();

 return 0;
}

/*++

Routine Description:

This routine handles user console text I/O

--*/
void ProcessUserIo()
{
	WCHAR CaptureBuffer[MAXBUFFER] = {0};
	DWORD BytesRead = 0;

	//
    // read input from the user which will happen to shown on the screen
    //
	do
	{
		ReadFile(StandardInConsole, CaptureBuffer, MAXBUFFER, &BytesRead, 0);
        //
        // read user input until the terminate flag is set to true
        //
	} while(TerminateRequest != TRUE); 
}

/*++

Routine Description:

This routine handles error handling output

--*/
void ErrorHandler(WCHAR *ErrorString, DWORD ErrorValue)
{
	//
    // output the error
    //
	wprintf_s(L"%s\nError Number %d", ErrorString, ErrorValue);

	//
    // terminate the application
    //
	ExitProcess(ErrorValue);
}


/*++

Routine Description:

This routine provides the applications CTRL_xxxx_EVENT HandlerRoutine

--*/
BOOL ControlHandlerFunction(DWORD ControlEvent)
{

	//
    // add special cleanup/shutdown code here when specific CTRL events 
	// are received.
    //
	switch(ControlEvent)
	{
		case CTRL_C_EVENT:
			//
			// add special cleanup/shutdown code here when the control 
			// c event is received
			//

			//
			// change the termination request flag to true so we can 
			// terminate the application
			//
			TerminateRequest = TRUE;
			return FALSE;
		case CTRL_CLOSE_EVENT:
			//
			// add special cleanup/shutdown code here when the control 
			// close event is received.
			//

			//
			// change the termination request flag to true so we can 
			// terminate the application
			//
			TerminateRequest = TRUE;
			return FALSE;
		case CTRL_BREAK_EVENT:
			//
			// add special cleanup/shutdown code here when the control 
			// break event is received.
			//

			//
			// change the termination request flag to true so we can 
			// terminate the application
			//
			TerminateRequest = TRUE;
			return FALSE;
		case CTRL_LOGOFF_EVENT:
			//
			// add special cleanup/shutdown code here when the 
			// control logoff event is received.
			//

			//
			// change the termination request flag to true so we can 
			// terminate the application
			//            
			TerminateRequest = TRUE;
			return FALSE;

		case CTRL_SHUTDOWN_EVENT:
			//
			// add special cleanup/shutdown code here when the 
			// control shutdown event is received
			//

			//
			// change the termination request flag to true so we can 
			// terminate the application
			//
			TerminateRequest = TRUE;
			return FALSE;

		default:
			//
			// add special handling code here to process unknown CTRL events
			//
			return FALSE;
	}
}

/*++

Routine Description:

This routine provides the hung app recovery callback routine

--*/
DWORD RecoveryCallback(PVOID pvParameter)
{

	HRESULT Result = S_OK;
	BOOL CancelRecovery = FALSE;

	//
    // register for restart
    //
	if (RegisterApplicationRestart(CommandLineParameter, 0) != S_OK)
	{
        //
		// log an event or do some error handling
        //
		goto cleanup;
	}

	//
    // signal WER on recovery progress
    //
	Result = ApplicationRecoveryInProgress(&CancelRecovery);
	if ((Result != S_OK) || CancelRecovery == TRUE)
	{
		goto cleanup;
	}

	//
    // signal WER recovery registration is finished
    //
	ApplicationRecoveryFinished(TRUE);

cleanup:
	if (CancelRecovery)
	{
        //
		// the user has cancelled
        //
		ApplicationRecoveryFinished(FALSE);
	}

	//
    // return value is ignored.
    //
	return 0;
}
