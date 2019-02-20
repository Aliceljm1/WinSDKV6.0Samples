/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) Microsoft Corporation.  All rights reserved.

Module Name:

    rmguiapp.cpp
 
Abstract:

    This module contains the skeleton code for a Windows native 
    API based GUI application that is Restart Manager and Windows 
    Error Reporting aware and supports based document recovery

Notes:
    This application requires Windows Vista
    
    This sample shows the basics required to be part of your application
    to support properly being shutdown and restarted by a Restart 
    Manager API enabled application or by the Windows Error 
    Reporting service.
    
    This sample also shows support for being shutdown by a system logoff, 
    close app and system shutdown events.

    This sample utilizes a simple data serialization and restoration 
    mechanism to simulate a document recovery experience. This will 
    help you observe a model by which you can enable document recovery 
    in your application.

 History:

     23-Nov-2005    a-paruss

	 17-Jul-2006	rohind

        Creation of this sample application
--*/

#include <windows.h>
#include <stdio.h>
#include <strsafe.h>

/*--

Global declarations

--*/
#define ID_EDITCHILD 5000
const   INT MAXBUFFERSIZE = 65536;  // setting to 64k to compensate for max edit control size
CHAR    ApplicationName[] = "RMGUIAPP";
CHAR    ApplicationTitle[] = "Restart Manager Aware Windows GUI App 1.0";
HWND    EditControlHwnd;
CHAR    RecoveryFile[MAX_PATH];
CHAR    RehydrateFile[MAX_PATH];
CHAR    RecoveryCallBackString[MAX_PATH];
HANDLE  hSaveThread;
PTSTR TextBuffer;
INT TextBufferLength;

// Global functions:
ATOM RegisterClass(HINSTANCE);
HRESULT InitInstance(HINSTANCE, INT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD RecoveryCallback(PVOID);
HRESULT RegisterForRecovery(CHAR*);
HRESULT SaveData(CHAR*);
HRESULT LoadData();
DWORD WINAPI SaveDataProc(LPVOID lpParameter);

/*++

Routine Description:

This routine provides the applications primary entry point

--*/

int WINAPI WinMain(HINSTANCE hInstance, 
                   HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, 
                   int nShowCmd)
{

    MSG    msg;
    HRESULT hr;
    BOOL Return;
    DWORD BufferSize=MAX_PATH;
    CHAR PathBuffer[MAX_PATH];
    DWORD dwReturnValue;
    UINT uReturnValue;
    hSaveThread = NULL;

    //
    // This check should really be a more comprehensive command line parsing function 
    // but for the purpose of this sample we will simply assign the value to the recovery 
    // string global variable
    //
    if (strlen(lpCmdLine) != 0)
    {
        strcpy_s(RecoveryCallBackString, sizeof(RecoveryCallBackString),TEXT(lpCmdLine));
    }
    
    //Setup the temp data file for saving user data for recovery/rehydrate scenarios. 
    //
    // Get the temp path
    //
    dwReturnValue = GetTempPath(BufferSize,PathBuffer);
    if (dwReturnValue > BufferSize)
    {
        hr = E_FAIL;
        //log some event or do some error handling.
        
    }

    //
    // Create a temporary file for recovery scenarios.
    //
    uReturnValue = GetTempFileName(PathBuffer, "wer", 0, RecoveryFile);
    if (uReturnValue == 0)
    {
        hr = E_FAIL;
        //log some event or do some error handling.
    } 

    //
    // Create a temporary file for rehydrate scenarios used with Restart Manager.
    //
    uReturnValue = GetTempFileName(PathBuffer, "~rm", 0, RehydrateFile);
    if (uReturnValue == 0)
    {
        hr = E_FAIL;
        //log some event or do some error handling.
    }

    //
    // register the applications primary window class
    //
    RegisterClass(hInstance);

    //
    // initialize the applications primary window instance
    //
    hr = InitInstance(hInstance, nShowCmd);
    if (hr == E_FAIL)
    {
        MessageBox(NULL, "Unable to initialize application", "RMGUIAPP", MB_OK);
        return 0;
    }


    //
    // register for restart with the RecoveryFile to allow WER to recover the app.
    //
    hr = RegisterForRecovery(RecoveryFile);
    if (hr != S_OK)
    {
        //
        // log an error or perform additional error handling. In this sample 
        // we are returning E_FAIL on all non S_OK responses.
        //
        hr = E_FAIL;
    }
    
    //
    // register the applications hang recovery callback routine
    //
    if (S_OK != RegisterApplicationRecoveryCallback(
                    (APPLICATION_RECOVERY_CALLBACK)RecoveryCallback,
                    (PVOID)NULL,
                    RECOVERY_DEFAULT_PING_INTERVAL,
                    0) ){
              //
              // log an event or do some error handling
              //
    }


    //
    // Main message dispatcher
    //
    while( (Return = GetMessage( &msg, NULL, 0, 0 )) != 0)
    { 
        if (Return == -1)
        {    
            //
            // handle the error and possibly exit
            //
        }
        else
        {
            //
            // dispatch message
            //
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
    }
  
  //
  //If the worker thread is currently saving data...then let it finish the operation.
  //
    if(NULL != hSaveThread){
        //Wait for worker thread to exit.
        dwReturnValue = WaitForSingleObject(hSaveThread, INFINITE);
          if(WAIT_FAILED == dwReturnValue){
                //
                //log an event or do some error handling.
                //
          }
        
        //Close the open thread handle
          if(hSaveThread){
              CloseHandle(hSaveThread);
          }
    }
    
    return (int) msg.wParam;
}

/*++

Routine Description:

This routine provides the primary window class definition and registration

--*/

ATOM RegisterClass(HINSTANCE hInstance)
{

    WNDCLASSEX         wc;

    //
    // define the window class
    //
    wc.cbSize                = sizeof(WNDCLASSEX);
    wc.style                = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc            = WndProc;
    wc.cbClsExtra            = 0;
    wc.cbWndExtra            = 0;
    wc.hInstance            = hInstance;
    wc.hIcon                = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor                = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground        = (HBRUSH) (COLOR_WINDOW+1);
    wc.lpszMenuName            = NULL;
    wc.lpszClassName        = ApplicationName;
    wc.hIconSm                = LoadIcon(NULL, IDI_APPLICATION);

    //
    // register the window class
    //
    return RegisterClassEx(&wc);
}


/*++

Routine Description:

This routine initializes the instance of this application

--*/
HRESULT InitInstance(HINSTANCE hInstance, 
                  INT nCmdShow)
{
    HWND hWnd;
    
    //
    // create the applications primary window
    //
    hWnd = CreateWindow(
                        ApplicationName, 
                        ApplicationTitle, 
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 
                        0, 
                        CW_USEDEFAULT, 
                        0, 
                        NULL, 
                        NULL, 
                        hInstance, 
                        NULL);
    if (!hWnd)
    {
      return E_FAIL;
    }

    //
    // show the default window
    //
    ShowWindow(hWnd, nCmdShow);
    return S_OK;
}

/*++

Routine Description:

This routine provides the applications message call back handler

--*/
LRESULT CALLBACK WndProc(HWND hWnd, 
                         UINT message, 
                         WPARAM wParam, 
                         LPARAM lParam)
{

    HRESULT hr = S_OK;
    BOOL bRet = FALSE;
    DWORD   dwSaveThreadID;
       
    //
    // process received messages
    //
    switch(message)
    {
    case WM_CREATE:

        //
        // create edit control
        //
        EditControlHwnd = CreateWindow("EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
                                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
                                0, 0, 0, 0,
                                hWnd,
                                (HMENU) ID_EDITCHILD,
                                (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE), 
                                NULL);
        
        //
        // set focus on the edit control
        //
        SetFocus(EditControlHwnd);

        //
        // we are getting passed a recovery string so lets try to open the file.. 
        //
        if (strlen(RecoveryCallBackString) != 0)
        {
            hr = LoadData();
            if (hr == E_FAIL)
            {
                //
                // log an event or do some error handling
                //
            }
        }
       
    return 0;


    case WM_SIZE:
        //
        // resize edit control to span the primary window dialog
        //
        MoveWindow(EditControlHwnd, 0, 0, LOWORD(lParam),HIWORD(lParam),TRUE);

        return 0;

    case WM_QUERYENDSESSION:

        //
        // This application is being told to shutdown by Restart Manager (e.g. an installer 
        // needs to replace files this app has in use)
        //
        if(lParam & ENDSESSION_CLOSEAPP)
        {
            //Disable mouse & keyboard input until WM_ENDSESSION message is received.
            bRet = EnableWindow(EditControlHwnd, FALSE);
              if(bRet == TRUE){//window was previously disabled.
                  //
                  //log an event or do some error handling.
                  //
              }
                        
            //
            // register for restart with the temp file used for rehydrate scenarios.
            //
            hr = RegisterForRecovery(RehydrateFile);
              if (hr == E_FAIL)
              {
                    //
                    // log an event or do some error handling
                    //
              }
                     
            // exit message handler
            return 1;
        }

        //
        //If the app is performing an operation which cant be interrupted such as burning a CD or DVD;
        //then you could veto the WM_QUERYENDSESSION message. The return value in this case would be FALSE.
        //Also the app should call ShutdownBlockReasonCreate to set a reason for blocking the shutdown operation.
        //Once the critical operation (eg: burning a CD) has completed the app should call ShutdownBlockReasonDestroy 
        //to indicate that the app is now ready to terminate.
        //

        return 1;

    case WM_ENDSESSION:
    if(lParam & ENDSESSION_CLOSEAPP){
        //Session is being ended, save the app state.
        if(wParam == TRUE)      
        {
            //
            //Copy the window data to a buffer as the window will be destroyed later and the worker thread willnot
            //be able to access the window at a later time.
            //
            
            // get size of the edit control buffer
            //
            TextBufferLength = GetWindowTextLength(EditControlHwnd);

            //
            // allocate a temp string buffer large enough to store text from edit control
            //
            TextBuffer = (PTSTR) malloc ((TextBufferLength+1) * sizeof(TCHAR));
              if (!TextBuffer)
                {
                    //log an event or do some error handling.
                }

            //
            // copy the text from the edit control to the temp string buffer
            //
            GetWindowText(EditControlHwnd, TextBuffer, TextBufferLength+1);

            //
            //Create a worker thread which will save user data on WM_ENDSESSION msg. This will allow
            //multiple apps to concurrently save data rather than blocking on WM_ENDSESSION response.
            //
            hSaveThread = CreateThread(NULL, 
                                       0, 
                                       SaveDataProc,
                                       NULL,
                                       0, 
                                       &dwSaveThreadID);
             if(NULL == hSaveThread)
             {
                  //log some event or do some error handling
                  //
                  //Free the TextBuffer if we failed to create the worker thread.
                 //
                 if(TextBuffer){
                      free(TextBuffer);
                     TextBuffer = NULL;
                }
             }
          
        }else{
            //
            //Enable mouse and keyboard input to the specified window or control.
            //
            bRet = EnableWindow(EditControlHwnd, TRUE);
             if(bRet == FALSE){//window was in an enabled state.
                    //
                   //log an event or do some error handling.
                   //
             }  
             
            //
            // Revert back to the commandline previously registered for the recovery scenarios. 
            // This allows the data to be saved for the app if WER needs to recover the app later.
            //
            hr = RegisterForRecovery(RecoveryFile);
             if(hr == E_FAIL)
             {
                    //
                    // log an event or do some error handling
                    //
             } 
          }
        }
      
      return 0;

    case WM_DESTROY:
        //
        // Tell the system we are terminating ourselves
        //
        PostQuitMessage(0);

        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/*++

Routine Description:

This routine provides the hung app recovery callback routine

--*/
DWORD RecoveryCallback(PVOID pvParameter)
{

    HRESULT hr = S_OK;
    BOOL RecoveryCanceled = FALSE;

    //
    // signal WER on recovery progress
    //
    hr = ApplicationRecoveryInProgress(&RecoveryCanceled);
    if ((hr != S_OK) || RecoveryCanceled == TRUE)
    {
        goto cleanup;
    }

    //
    // serialize data that the user typed in the edit control
    //
    hr = SaveData(RecoveryFile);
    if (hr != S_OK)
    {
        //
        // log an error or perform additional error handling. In this sample 
        // we are returning E_FAIL on all non S_OK responses.
        //
        hr = E_FAIL;
        goto cleanup;
    }

    //
    // signal WER on recovery progress
    //
    hr = ApplicationRecoveryInProgress(&RecoveryCanceled);
    if ((hr != S_OK) || RecoveryCanceled == TRUE)
    {
        //
        // log an error or perform additional error handling. In this sample 
        // we are returning E_FAIL on all non S_OK responses.
        //
        hr = E_FAIL;
        goto cleanup;
    }

    //
    // register for restart
    //
    hr = RegisterForRecovery(RecoveryFile);
    if (hr != S_OK)
    {
        //
        // log an error or perform additional error handling. In this sample 
        // we are returning E_FAIL on all non S_OK responses.
        //
        hr = E_FAIL;
        goto cleanup;
    }

    //
    // signal WER on recovery progress
    //
    hr = ApplicationRecoveryInProgress(&RecoveryCanceled);
    if ((hr != S_OK) || RecoveryCanceled == TRUE)
    {
        //
        // log an error or perform additional error handling. In this sample 
        // we are returning E_FAIL on all non S_OK responses.
        //
        hr = E_FAIL;
        goto cleanup;
    }

cleanup:
    if (RecoveryCanceled)
    {
        //
        // the user has cancelled
        //
        ApplicationRecoveryFinished(FALSE);
    }
    else
    {
        //
        // signal WER that recovery is finished
        //
        ApplicationRecoveryFinished(TRUE);
    }
    return 0;
}

/*++

Routine Description:

This routine provides the common registeration recovery calls

--*/

HRESULT RegisterForRecovery(CHAR *TempFile)
{
    HRESULT hr = S_OK;
    WCHAR CommandLineParameter[MAX_PATH];
    //
    // since the lpCmdLine is in ANSI, we need to convert the
    // CommandLineParameter character string to wide-char before
    // we call the RegisterApplicationRestart API
    //
    MultiByteToWideChar(CP_ACP, 
                        0, 
                        TempFile, 
                        strlen(TempFile)+1, 
                        CommandLineParameter, 
                        sizeof(CommandLineParameter)/sizeof(CommandLineParameter[0]));

    hr = RegisterApplicationRestart(CommandLineParameter, NULL); 
    if (hr != S_OK)
    {
        //
        // log an error or perform additional error handling. In this sample 
        // we are returning E_FAIL on all non S_OK responses.
        //
        hr = E_FAIL;
    }

    return hr;
}

/*++

Routine Description:

This routine provides data serialization support

--*/

HRESULT SaveData(CHAR *TempFile)
{

    HRESULT hr = S_OK;
    HANDLE FileHandle = INVALID_HANDLE_VALUE;
    BOOL Result;
    DWORD NumberOfBytesWritten;

    //
    //Check whether any data is available
    //
    if(!TextBuffer){
      //log an event or do some error handling.
      goto cleanup;
    }
    
    //
    // create recovery file
    //
    FileHandle = CreateFile((LPTSTR)TempFile,
                            GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    if (FileHandle == INVALID_HANDLE_VALUE) 
    { 
        hr = E_FAIL;
        goto cleanup;
    }

    //
    // serialize temp buffer into file
    //
    Result = WriteFile(FileHandle, TextBuffer, TextBufferLength+1, &NumberOfBytesWritten, NULL);
    if (!Result) 
    {
        hr = E_FAIL;
        goto cleanup;
    }

cleanup:
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        //
        // close recovery file
        //
        CloseHandle(FileHandle);
    }
    if(TextBuffer){
      //Free the Buffer.
        free(TextBuffer); 
        TextBuffer = NULL;
    }

    return hr;
}

/*++

Routine Description:

This routine provides data restoration support

--*/
HRESULT LoadData()
{

    HRESULT hr = S_OK;
    HANDLE FileHandle = INVALID_HANDLE_VALUE;
    BOOL Result;
    DWORD NumberOfBytesRead;
    CHAR TextBuffer[MAXBUFFERSIZE];

    //
    // open the recovery file
    //
    FileHandle = CreateFile(TEXT(RecoveryCallBackString),
                            GENERIC_READ,
                            0, 
                            NULL,
                            OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    if (FileHandle == INVALID_HANDLE_VALUE) 
    { 
        hr = E_FAIL;
        goto cleanup;
    }

    //
    // read the data from the file
    //
    Result = ReadFile(FileHandle, TextBuffer, MAXBUFFERSIZE, &NumberOfBytesRead, 0);
    if (!Result) 
    {
        hr = E_FAIL;
        goto cleanup;
    }

    //
    // update the edit control with the recovered text
    //
    SendMessage(EditControlHwnd, WM_SETTEXT, 0, (LPARAM)TextBuffer);

cleanup:

    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        //
        // close recovery file
        //
        CloseHandle(FileHandle);
    }

    //
    // delete the recovery file
    //
    if (DeleteFile(TEXT(RecoveryCallBackString)) == FALSE)
    {
        hr = E_FAIL;
    }

    return hr;
}

/*++

Routine Description:

This routine defines the save data functionality to be executed by the thread.

--*/
DWORD WINAPI SaveDataProc(LPVOID lpParameter){
  
  HRESULT hr = S_OK;
  
     //
    //Serialize and save data that the user typed in the edit control
    //
    hr = SaveData(RehydrateFile);
    if(E_FAIL == hr){
      //
      //log an event or do some error handling.
      //
    }
      
  return 0;
}
