#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]::RefreshLicense method
// RefreshLicense_ThreadProc function
////////////////////////////////////////////////////////////////////

HRESULT RefreshLicenseMessageLoop(
   REFRESH_LICENSE_THREAD_CONTEXT* pThreadCtx,
   CComPtr<IWMPContentPartnerCallback> spCallback);


DWORD WINAPI RefreshLicense_ThreadProc(LPVOID lpParameter)
{
   REFRESH_LICENSE_THREAD_CONTEXT* pThreadCtx = NULL;
   IWMPContentPartnerCallback* spCallback;
   MSG msg = {0};
   HRESULT hr = S_OK;
   BOOL comInitialized = FALSE;

   if(NULL == lpParameter)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   pThreadCtx = static_cast<REFRESH_LICENSE_THREAD_CONTEXT*>(lpParameter);

   if(NULL == pThreadCtx->pIStream)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

   if(FAILED(hr))
   {
      ATLTRACE2("RefreshLicense_ThreadProc: CoInitializeEx failed. %x\n", hr);
      goto cleanup;
   }  

   comInitialized = TRUE;

   // Get a pointer to an IWMPContentPartnerCallback interface.

   hr = CoGetInterfaceAndReleaseStream(
      pThreadCtx->pIStream,
      __uuidof(IWMPContentPartnerCallback),
      reinterpret_cast<LPVOID*>(&spCallback) );

   // The stream was released (even if CoGetInterfaceAndReleaseStream failed). 
   // Set the stream pointer to NULL.
   pThreadCtx->pIStream = NULL;

   if(FAILED(hr))
   {
      ATLTRACE2("RefreshLicense_ThreadProc: Failed to get IWMPContentPartnerCallback interface. %x\n", hr);
      goto cleanup;
   }

   if(NULL == spCallback)
   {
      hr = E_UNEXPECTED;
      goto cleanup;
   }

   ATLTRACE2("RefreshLicense_ThreadProc: Succeeded in getting IWMPContentPartnerCallback interface.\n");

   // Make sure we have a message loop.
   PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

   // Tell the main thread that we are ready to receive messages.
   SetEvent(pThreadCtx->hInitialized);

   hr = RefreshLicenseMessageLoop(pThreadCtx, spCallback);

cleanup:

   // Set this event here, just in case there was a "goto cleanup"
   // before the event was set.
   SetEvent(pThreadCtx->hInitialized);

   // The thread that started this thread allocated a
   // REFRESH_LICENSE_THREAD_CONTEXT structure.
   // We must free that memory here.
   if(pThreadCtx)
   {

     if(NULL != pThreadCtx->pIStream)
      {
         // For some reason, CoGetInterfaceAndReleaseStream never got called.
         // So release the stream here.
         pThreadCtx->pIStream->Release();
         pThreadCtx->pIStream = NULL;
      }

      free(pThreadCtx);
      pThreadCtx = NULL;
   }
                   
   if(comInitialized)
   {
      ATLTRACE2("RefreshLicense_ThreadProc: Calling CoUninitialize.\n");
      CoUninitialize();
   } 

   return hr;
}
  
      
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::RefreshLicense( 
   DWORD dwCookie,
   VARIANT_BOOL fLocal,
   BSTR bstrURL,
   WMPStreamingType type,
   ULONG contentID,
   BSTR bstrRefreshReason,
   VARIANT *pReasonContext)
{

   REFRESH_LICENSE_CONTEXT* pRefrLicCtx = NULL; 
   BOOL postResult = 0;
   HRESULT hr = S_OK;
   
   if(NULL == bstrURL || NULL == bstrRefreshReason || NULL == pReasonContext)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   // If the refresh-license thread has not already been started,
   // start it now.
   
   if(0 == m_refreshLicenseThreadHandle)
   {  
      hr = this->StartRefreshLicenseThread();

      if(FAILED(hr))
      {
         ATLTRACE2("RefreshLicense: StartRefreshLicenseThread failed. %x\n", hr);
         goto cleanup;
      } 
      
      ATLTRACE2("RefreshLicense: StartRefreshLicenseThread succeeded.\n");     
   }

   // When we post a refresh-license message, we must provide
   // all the information we were passed in the seven parameters
   // of this method. So we copy the seven parameters into a
   // REFRESH_LICENSE_CONTEXT structure.

   pRefrLicCtx = new REFRESH_LICENSE_CONTEXT();

   if(!pRefrLicCtx)
   {
      ATLTRACE2("RefreshLicense: Failed to create new REFRESH_LICENSE_CONTEXT.\n");
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   ZeroMemory(pRefrLicCtx, sizeof(REFRESH_LICENSE_CONTEXT));

   pRefrLicCtx->dwCookie = dwCookie;
   pRefrLicCtx->fLocal = fLocal;
   pRefrLicCtx->bstrURL = bstrURL;
   pRefrLicCtx->type = type;
   pRefrLicCtx->contentID = contentID;
   pRefrLicCtx->bstrRefreshReason = bstrRefreshReason;
   pRefrLicCtx->pReasonContext = pReasonContext;

   postResult = PostThreadMessage(
      m_refreshLicenseThreadId,
      m_msgRefreshLicense,
      0,
      reinterpret_cast<LPARAM>(pRefrLicCtx) );

   if(0 == postResult)
   {     
      hr = HRESULT_FROM_WIN32(GetLastError()); 
      ATLTRACE2("RefreshLicense: PostThreadMessage failed. %x\n", hr);
      goto cleanup;
   }
  
   ATLTRACE2("RefreshLicense: PostThreadMessage succeeded.\n");

   // We successfully posted the message to the refresh-license thread.
   // We have no more need for the pointer to the refresh-license context.
   pRefrLicCtx = NULL;

   // The refresh-license thread must free the memory pointed to by pRefrLicCtx.

   
cleanup:

   if(pRefrLicCtx)
   {
      // We failed to post a message to the refresh-license thread.
      // The refresh-license thread will not be able to free the memory
      // pointed to by pRefrLicCtx.  So we free it here.      
      free(pRefrLicCtx);
      pRefrLicCtx = NULL;
   }

   // If pRefrLicCtx is NULL, refresh-license thread will free the memory 
   // pointed to by pRefrLicCtx.  

   return hr;
}



HRESULT STDMETHODCALLTYPE C[!output Safe_root]::StartRefreshLicenseThread()
{
   DWORD waitResult = 0;
   HRESULT hr = S_OK;
   REFRESH_LICENSE_THREAD_CONTEXT* pThreadCtx = NULL;


   // What does the new thread need to get started?
   //    1. IStream that it can use to 
   //         unmarshal IWMPContentPartnerCallback
   //    2. Event handle it can use to signal that
   //         the message loop is started
   //    3. ID of the refresh-license message
   //    4. ID of the message to exit the message loop     
   // Those four things are the members of REFRESH_LICENSE_THREAD_CONTEXT.

   pThreadCtx = new REFRESH_LICENSE_THREAD_CONTEXT();

   if(!pThreadCtx)
   {
      ATLTRACE2("StartRefreshLicenseThread: Failed to create REFRESH_LICENSE_THREAD_CONTEXT.\n");
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   ZeroMemory(pThreadCtx, sizeof(REFRESH_LICENSE_THREAD_CONTEXT));
      
   hr = CoMarshalInterThreadInterfaceInStream(
      __uuidof(IWMPContentPartnerCallback), 
      m_spCallback, 
      &pThreadCtx->pIStream);

   if(FAILED(hr))
   {
      ATLTRACE2("StartRefreshLicenseThread: Failed to marshal IWMPContentPartnerCallback interface. %x\n", hr);
      goto cleanup; 
   }

   if(NULL == pThreadCtx->pIStream)
   {
      hr = E_UNEXPECTED;
      goto cleanup;
   }

   pThreadCtx->hInitialized = CreateEvent(NULL, TRUE, FALSE, NULL);

   if(NULL == pThreadCtx->hInitialized)
   {
      ATLTRACE2("StartRefreshLicenseThread: CreateEvent failed.\n", 0);
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }
 
   m_msgRefreshLicense = RegisterWindowMessage( _T("RefreshLicense") );

   if(0 == m_msgRefreshLicense)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartRefreshLicenceThread: RegisterWindowMessage failed. %x\n", hr);    
      goto cleanup;
   }

   pThreadCtx->msgRefreshLicense = m_msgRefreshLicense;   
   pThreadCtx->msgExitMessageLoop = m_msgExitMessageLoop;
    
   m_refreshLicenseThreadHandle = CreateThread(
      NULL,
      0,
      RefreshLicense_ThreadProc,
      static_cast<LPVOID>(pThreadCtx),
      0,
      &m_refreshLicenseThreadId);

   if(NULL == m_refreshLicenseThreadHandle)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartRefreshLicenseThread: CreateThread failed.\n %x", hr);   
      goto cleanup;
   }
   
   // Wait for the refresh-license thread to signal that it is initialized.
  
   waitResult = WaitForSingleObject(pThreadCtx->hInitialized, 30000);

   if(WAIT_FAILED == waitResult)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartRefreshLicenseThread: WaitForSingleObject failed. %x\n", hr);    
      goto cleanup;
   }

   ATLTRACE2("StartRefreshLicenseThread: Refresh-license thread signaled that it is initialized.\n", 0);

   // At this point, we know the refresh-license thread called SetEvent to
   // let us know that we can stop waiting.
   // But that could mean one of two things:
   //   1. The refresh-license thread called SetEvent and then entered its message loop.
   //   2. The refresh-license thread failed to enter its message loop and 
   //      called SetEvent as part of its cleanup code.
   //
   // The point is this: we know that the refresh-license thread started, but we
   // do not know whether the refresh-license thread is still active.
   
 cleanup:

   if(pThreadCtx->hInitialized)
   {
      CloseHandle(pThreadCtx->hInitialized);
      pThreadCtx->hInitialized = 0;
   }

   if(0 == m_refreshLicenseThreadHandle)
   {
      // The refresh-license thread did not start; that is, CreateThread failed.
      // The refresh-license thread will not be able to free the memory pointed
      // to by pThreadCtx, so we free it here.
 
      if(pThreadCtx)
      {
         free(pThreadCtx);
         pThreadCtx = NULL;
      }
   }

   // If 0 != m_refreshLicenseThreadHandle, the refresh-license thread 
   // will free the memory pointed to by pThreadCtx.

   // If 0 != m_refreshLicenseThreadHandle, code in the 
   // IWMPContentPartner::SetCallback method will close
   // m_refreshLicenseThreadHandle and set it to 0.

   return hr;

} // StartRefreshLicenseThread


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::VerifyPermission( 
   BSTR bstrPermission,
   VARIANT *pContext)
{  
   // ToDo: If this is the first call to the VerifyPermission method,
   // create a thread that will handle permission verification.

   // ToDo: Post a message to the permission-verification 
   // thread and return immediately.

   // For examples of managing threads, see the 
   // RefreshLicense method and the Download method.

   return  S_OK;
}



HRESULT RefreshLicenseMessageLoop(
   REFRESH_LICENSE_THREAD_CONTEXT* pThreadCtx,
   CComPtr<IWMPContentPartnerCallback> spCallback)
{
   REFRESH_LICENSE_CONTEXT* pRefrLicCtx = NULL;
   MSG msg = {0};
   HRESULT hr = S_OK;

// Windows message loop
   while(TRUE)
   {
      BOOL ret = FALSE;

      // Suppose this plug-in gets several refresh-license messages in rapid
      // succession and then the user closes Windows Media Player. We would
      // like to let Windows Media Player close without waiting for all
      // those refresh-license messages to be processed.
      //
      // So we use the following strategy.
      //
      // Peek into the message queue to see if there is an exit message.
      // If there is an exit message anywhere in the queue, break out of
      // the message loop even though there might be several refresh-license
      // messages remaining in the queue.
      
      if( PeekMessage(
             &msg, 
             NULL, 
             pThreadCtx->msgExitMessageLoop, 
             pThreadCtx->msgExitMessageLoop,
             PM_REMOVE) )
      {
         ATLTRACE2("RefreshLicenseMessageLoop: PeekMessage retrieved an exit message.\n");
         goto cleanup;
      }

      ret = GetMessage(&msg, 0, 0, 0);
      if(-1 == ret)
      {
          ATLTRACE2("RefreshLicenseMessageLoop: GetMessage returned -1.\n");
          hr = HRESULT_FROM_WIN32(GetLastError());

          // Break out of the message loop and let
          // the refresh-license thread terminate.
          goto cleanup;
      }

      if(pThreadCtx->msgRefreshLicense == msg.message)
      {
         ATLTRACE2("RefreshLicenseMessageLoop: Recieved message to refresh license.\n");

         pRefrLicCtx = reinterpret_cast
            <REFRESH_LICENSE_CONTEXT*>(msg.lParam);

         if(NULL == pRefrLicCtx)
         {
            hr = E_UNEXPECTED;

            // Break out of the message loop and let
            // the refresh-license thread terminate.
            goto cleanup;
         }        

         // Simulate lengthy operation by sleeping four seconds.
         SleepEx(4000, FALSE);

         // Tell Windows Media Player that we have finished refreshing the license.
         hr = spCallback->RefreshLicenseComplete(
            pRefrLicCtx->dwCookie, 
            pRefrLicCtx->contentID, 
            S_OK);

         if(FAILED(hr))
         {
            ATLTRACE2("RefreshLicenseMessageLoop: RefreshLicenseComplete failed. %x\n", hr);
         }

         // The thread that posted this message allocated a
         // REFRESH_LICENCE_CONTEXT structure.
         // We must free that memory here.

         free(pRefrLicCtx);
         pRefrLicCtx = NULL; 
      }

      else if(pThreadCtx->msgExitMessageLoop == msg.message)
      {
         ATLTRACE2("RefreshLicenseMessageLoop: Received message to exit message loop.\n");
         
         // Break out of the message loop and let
         // the refresh-license thread terminate.
         goto cleanup;
      } 
      else
      {
         ATLTRACE2("RefreshLicenseMessageLoop: Received unexpected message. %x\n", msg.message);
         DispatchMessage(&msg);
      }      
  
   } // while(TRUE) -- message loop

cleanup:

   // If we broke out of the message loop without freeing pRefrLicCtx, free it here.
   if(pRefrLicCtx)
   {
      free(pRefrLicCtx);
      pRefrLicCtx = NULL;
   }


   // Clean out any refresh-license messages that remain in the queue.

   while( PeekMessage( 
             &msg, 
             NULL, 
             pThreadCtx->msgRefreshLicense, 
             pThreadCtx->msgRefreshLicense, 
             PM_REMOVE ) )
   {  
      ATLTRACE2("RefreshLicenseMessageLoop: PeekMessage in cleanup retrieved a refresh-license message.\n");
      pRefrLicCtx = reinterpret_cast<REFRESH_LICENSE_CONTEXT*>(msg.lParam);

      if(pRefrLicCtx)
      {
         free(pRefrLicCtx);
         pRefrLicCtx = NULL;
      }

   } // while PeekMessage

   return hr;
}