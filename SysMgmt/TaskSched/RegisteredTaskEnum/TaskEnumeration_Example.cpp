/********************************************************************
 This sample enumerates through the tasks on the local computer and 
 displays their name and status. 
********************************************************************/

#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
//  Include the task header file.
#include "rpcsal.h"
#include "taskschd.h"
# pragma comment(lib, "taskschd.lib")
# pragma comment(lib, "comsupp.lib")


using namespace std;

void main(void)
{
    //  ------------------------------------------------------
    //  Initialize COM.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if( FAILED(hr) )
    {
        printf("\nCoInitializeEx failed: %x", hr );
        return;
    }

    //  Set general COM security levels.
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if( FAILED(hr) )
    {
        printf("\nCoInitializeSecurity failed: %x", hr );
        return;
    }

    //  ------------------------------------------------------
    //  Create an instance of the Task Service. 
    ITaskService *pService = NULL;
    hr = CoCreateInstance( CLSID_TaskScheduler,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_ITaskService,
                           (void**)&pService );  
    if (FAILED(hr))
    {
          printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
          CoUninitialize();
          return;
    }
        
    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(),
        _variant_t(), _variant_t());
    if( FAILED(hr) )
    {
        printf("ITaskService::Connect failed: %x", hr );
		pService->Release();
        CoUninitialize();
        return;
    }

    //  ------------------------------------------------------
    //  Get the pointer to the root task folder.  This folder will hold the
    //  new task that is registered.
    ITaskFolder *pRootFolder = NULL;
    hr = pService->GetFolder( _bstr_t( L"\\") , &pRootFolder );
    
	pService->Release();
	if( FAILED(hr) )
    {
        printf("Cannot get Root Folder pointer: %x", hr );
		CoUninitialize();
        return;
    }
    
	//  -------------------------------------------------------
	//  Get the registered tasks in the folder.
	IRegisteredTaskCollection* pTaskCollection = NULL;
	hr = pRootFolder->GetTasks( NULL, &pTaskCollection );

	pRootFolder->Release();
	if( FAILED(hr) )
	{
		printf("Cannot get the registered tasks.: %x", hr);
		CoUninitialize();
        return;
	}

	LONG numTasks = 0;
	hr = pTaskCollection->get_Count(&numTasks);

	if( numTasks == 0 )
     {
		printf("\nNo Tasks are currently running" );
		pTaskCollection->Release();
		CoUninitialize();
        return;
     }

    printf("\nNumber of Tasks : %d", numTasks );

	IRegisteredTask* pRegisteredTask = NULL;
    BSTR taskName = L"";
	TASK_STATE taskState;
	
	for(LONG i=0; i < numTasks; i++)
	{
		hr = pTaskCollection->get_Item( _variant_t(i+1), &pRegisteredTask );
		
        hr = pRegisteredTask->get_Name(&taskName);
		hr = pRegisteredTask->get_State(&taskState);
		
		if( FAILED(hr) )
	    {
		    printf("Cannot get the registered task information: %x", hr);
		    
	    }
		else if( SUCCEEDED(hr) )
		{
		    printf("\nTask Name: %S", taskName);
		    printf("\n\tState: %d", taskState);
		}
	}

	pTaskCollection->Release();
	pRegisteredTask->Release();
	CoUninitialize();
    return;
}

