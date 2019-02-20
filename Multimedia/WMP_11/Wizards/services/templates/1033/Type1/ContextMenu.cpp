#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
//    GetCommands method
//    InvokeCommands method
////////////////////////////////////////////////////////////////////

// For now, don't warn about unreferenced parameters.
#pragma warning(disable:4100)

      
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetCommands( 
   BSTR location,
   VARIANT *pLocationContext,
   BSTR itemLocation,
   ULONG cItemIDs,
   ULONG *prgItemIDs,
   ULONG *pcItemIDs,
   WMPContextMenuInfo **pprgItems)
{
   LPVOID buffer = NULL;
   WMPContextMenuInfo* commands = NULL;
   HRESULT hr = S_OK;

   ATLTRACE2("GetCommands: location = %S.\n", location);

   // ToDo: Determine what the menu commands should be by
   // inspecting the first five parameters.

   // Set output parameters to 0 in case we fail.

   if(NULL != pcItemIDs)
   {
      *pcItemIDs = 0;
   }

   if(NULL != pprgItems)
   {
      *pprgItems = NULL;
   }

   // Allocate an array of two WMPContextMenuInfo structures.

   buffer = CoTaskMemAlloc(2*sizeof(WMPContextMenuInfo));

   if(!buffer)
   {
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   commands = static_cast<WMPContextMenuInfo*>(buffer);

   // Initialize the first WMPContextMenuInfo structure in the array.

   commands[0].dwID = 100;
   commands[0].bstrMenuText = SysAllocString(L"Send to a friend");
   commands[0].bstrHelpText = SysAllocString(L"Send this item to a friend.");

   if(NULL == commands[0].bstrMenuText || NULL == commands[0].bstrHelpText)
   {
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   // Initialize the second WMPContextMenuInfo structure in the array.

   commands[1].dwID = 200;
   commands[1].bstrMenuText = SysAllocString(L"Special offer");
   commands[1].bstrHelpText = SysAllocString(L"Learn about a special offer.");

   if(NULL == commands[1].bstrMenuText || NULL == commands[1].bstrHelpText)
   {
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }
  
   // Return the array of menu commands to the caller.
   *pcItemIDs =   2;
   *pprgItems = commands;
 

cleanup:

   // If any of the calls to SysAllocString failed, do a big cleanup here.
   // Note: It is OK to pass NULL to SysFreeString.

   if(FAILED(hr) && NULL != commands)
   {
      SysFreeString(commands[0].bstrMenuText);
      SysFreeString(commands[0].bstrHelpText);
      SysFreeString(commands[1].bstrMenuText);
      SysFreeString(commands[1].bstrHelpText);
      CoTaskMemFree( static_cast<LPVOID>(commands) );
   }

   // If this method succeeds, the caller is responsible for
   // freeing all the menu text strings, all the help text strings,
   // and the array of WMPContextMenuInfo structures. 

   return hr;
}
  
      
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::InvokeCommand( 
   DWORD dwCommandID,
   BSTR location,
   VARIANT *pLocationContext,
   BSTR itemLocation,
   ULONG cItemIDs,
   ULONG *rgItemIDs)
{
   ATLTRACE2("InvokeCommand: dwCommandID = %d.\n", dwCommandID);

   // ToDo: Execute the context menu command specified by dwCommandID.

   return S_OK;
}