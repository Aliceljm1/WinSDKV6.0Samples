#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
//    Download method
//    DownloadTrackComplete method
////////////////////////////////////////////////////////////////////

HRESULT DownloadMessageLoop(
   DOWNLOAD_THREAD_CONTEXT* pThreadCtx,
   CComPtr<IWMPContentPartnerCallback> spCallback);


DWORD WINAPI Download_ThreadProc(LPVOID lpParameter)
{
   DOWNLOAD_THREAD_CONTEXT* pThreadCtx = NULL;
   CComPtr<IWMPContentPartnerCallback> spCallback;
   MSG msg = {0};  
   HRESULT hr = S_OK;
   BOOL comInitialized = FALSE;
   
   if(NULL == lpParameter)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   pThreadCtx = static_cast<DOWNLOAD_THREAD_CONTEXT*>(lpParameter);

   if(NULL == pThreadCtx->pIStream)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

   if(FAILED(hr))
   {
      ATLTRACE2("Download_ThreadProc: CoInitializeEx failed. %x\n", hr);
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
      ATLTRACE2("Download_ThreadProc: Failed to get IWMPContentPartnerCallback interface. %x\n", hr);
      goto cleanup;
   }

   if(NULL == spCallback)
   {
      hr = E_UNEXPECTED;
      goto cleanup;
   }
   
   ATLTRACE2("Download_ThreadProc: Succeeded in getting IWMPContentPartnerCallback interface.\n");

   // Make sure we have a message loop.
   PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  
   // Tell the main thread that we are ready to receive messages.
   SetEvent(pThreadCtx->hInitialized);
   
   hr = DownloadMessageLoop(pThreadCtx, spCallback);

cleanup:

   // Set this event here, just in case there was a "goto cleanup"
   // before the event was set.
   SetEvent(pThreadCtx->hInitialized);

   // The thread that started this thread allocated a
   // DOWNLOAD_THREAD_CONTEXT structure.
   // We must free that memory here.
   if(pThreadCtx)
   {
      if(NULL != pThreadCtx->pIStream)
      {
         // For some reason, CoGetInterfaceAndReleaseStream never got called.
         // So we release the stream here.
         pThreadCtx->pIStream->Release();
         pThreadCtx->pIStream = NULL;
      }

      free(pThreadCtx);
      pThreadCtx = NULL;
   }
                
   if(comInitialized)
   {
      ATLTRACE2("Download_ThreadProc: Calling CoUninitialize.\n", 0);
      CoUninitialize();
   }

   return hr;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::Download( 
   IWMPContentContainerList *pInfo,
   DWORD cookie)
{  
   DOWNLOAD_BATCH_CONTEXT* pBatchCtx = NULL; 
   BOOL postResult = FALSE;
   HRESULT hr = S_OK;
   
   if(NULL == pInfo)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   // If the download thread has not already been started,
   // start it now.
   
   if(0 == m_downloadThreadHandle)
   {  
      hr = this->StartDownloadThread();

      if(FAILED(hr))
      {
         ATLTRACE2("Download: StartDownloadThread failed. %x\n", hr);
         goto cleanup;
      } 
     
      ATLTRACE2("Download: StartDownloadThread succeeded.\n");
   }

   // At this point, we know the download thread started, but
   // we don't know whether it is still active.

   // When we post a download-batch message, we must provide
   // two things: a cookie that represents the batch and an
   // IStream interface that the download thread can use to
   // obtain an IWMPContentContainerList interface. Those two
   // things are the members of DOWNLOAD_BATCH_CONTEXT.

   pBatchCtx = new DOWNLOAD_BATCH_CONTEXT();

   if(NULL == pBatchCtx)
   {
      ATLTRACE2("Download: Failed to create new DOWNLOAD_BATCH_CONTEXT.\n");
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   ZeroMemory(pBatchCtx, sizeof(DOWNLOAD_BATCH_CONTEXT));
   
   hr = CoMarshalInterThreadInterfaceInStream(
      __uuidof(IWMPContentContainerList),
      pInfo,
      &pBatchCtx->pIStream); 

   if(FAILED(hr) || NULL == pBatchCtx->pIStream)
   {
      ATLTRACE2("Download: Failed to marshal IWMPContentContainerList interface. %x\n", hr);
      goto cleanup;
   } 

   ATLTRACE2("Download: Successfully marshaled IWMPContentContainerList interface.\n");
  
   pBatchCtx->cookie = cookie;

   // If the download thread is not active, the following
   // call to PostThreadMessage will fail. 

   postResult = PostThreadMessage(
      m_downloadThreadId,
      m_msgDownloadBatch,
      0,
      reinterpret_cast<LPARAM>(pBatchCtx) );

   if(0 == postResult)
   {    
      hr = HRESULT_FROM_WIN32(GetLastError());  
      ATLTRACE2("Download: PostThreadMessage failed. %x\n", hr);
      goto cleanup;
   }
   
   ATLTRACE2("Download: PostThreadMessage %x succeeded.\n");

   // We successfully posted the message to the download thread.
   // We have no more need for the pointer to the batch context.
   pBatchCtx = NULL;

   // The download thread must free the memory pointed to by pBatchCtx.

cleanup:

   if(pBatchCtx)
   {
      // We failed to post a message to the download thread.
      // The download thread will not be able to free the memory
      // pointed to by pBatchCtx.  So we free it here.      
      free(pBatchCtx);
      pBatchCtx = NULL;
   }

   // If pBatchCtx is NULL, download thread will free the memory 
   // pointed to by pBatchCtx.  

   return hr;
}

        
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::DownloadTrackComplete( 
   HRESULT hrResult,
   ULONG contentID,
   BSTR downloadTrackParam)
{
   if(FAILED(hrResult))
   {
      ATLTRACE2("DownloadTrackComplete: Windows Media Player failed to download track %d.\n", contentID);
      ++g_totalDownloadFailures;
   }
   else
   {
      ATLTRACE2("DownloadTrackComplete: Windows Media Player has completed the download for track %d.\n", contentID);
   }

   return S_OK;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::StartDownloadThread()
{
   DWORD waitResult = 0;
   HRESULT hr = S_OK;
   DOWNLOAD_THREAD_CONTEXT* pThreadCtx = NULL;

   // What does the new thread need to get started?
   //    1. IStream that it can use to unmarshal IWMPContentPartnerCallback   
   //    2. Event handle it can use to signal that the thread is started
   //    3. ID of the message for downloading a batch of media items
   //    4. ID of the message to exit the message loop  
   // Those four things are the members of DOWNLAD_THREAD_CONTEXT.

   pThreadCtx = new DOWNLOAD_THREAD_CONTEXT();

   if(NULL == pThreadCtx)
   {
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   ZeroMemory(pThreadCtx, sizeof(DOWNLOAD_THREAD_CONTEXT));
      
   hr = CoMarshalInterThreadInterfaceInStream(
      __uuidof(IWMPContentPartnerCallback), 
      m_spCallback, 
      &pThreadCtx->pIStream);

   if(FAILED(hr))
   {
      ATLTRACE2("StartDownloadThread: Failed to marshal IWMPContentPartnerCallback interface. %x\n", hr);
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
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   m_msgDownloadBatch = RegisterWindowMessage( _T("DownloadBatch") );

   if(0 == m_msgDownloadBatch)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      goto cleanup;
   }

   pThreadCtx->msgDownloadBatch = m_msgDownloadBatch;   
   pThreadCtx->msgExitMessageLoop = m_msgExitMessageLoop;

   // At this point, all members of pThreadCtx are initialized.
    
   m_downloadThreadHandle = CreateThread(
      NULL,
      0,
      Download_ThreadProc,
      static_cast<LPVOID>(pThreadCtx),
      0,
      &m_downloadThreadId);

   if(NULL == m_downloadThreadHandle)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartDownloadThread: CreateThread failed.\n %x", hr);
      goto cleanup;
   }
  
   // Wait for the download thread to signal that it is initialized.
  
   waitResult = WaitForSingleObject(pThreadCtx->hInitialized, 30000);

   if(WAIT_FAILED == waitResult)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      goto cleanup;
   }

   ATLTRACE2("StartDownloadThread: Download thread signaled that it is initialized.\n", 0);

   // At this point, we know the download thread called SetEvent to
   // let us know that we can stop waiting.
   // But that could mean one of two things:
   //   1. The download thread called SetEvent and then entered its message loop.
   //   2. The download thread failed to enter its message loop and 
   //      called SetEvent as part of its cleanup code.
   //
   // The point is this: we know that the download thread started, but we
   // do not know whether the download thread is still active.
  
 cleanup:

   if(pThreadCtx->hInitialized)
   {
      CloseHandle(pThreadCtx->hInitialized);
      pThreadCtx->hInitialized = 0;
   }

   if(0 == m_downloadThreadHandle)
   {
      // The download thread did not start; that is, CreateThread failed.
      // The download thread will not be able to free the memory pointed
      // to by pThreadCtx, so we free it here.
 
      if(pThreadCtx)
      {
         free(pThreadCtx);
         pThreadCtx = NULL;
      }
   }

   // If 0 != m_downloadThreadHandle, the download thread 
   // will free the memory pointed to by pThreadCtx.

   // If 0 != m_downloadThreadHandle, code in the 
   // IWMPContentPartner::SetCallback method will close
   // m_downloadThreadHandle and set it to 0.

   return hr;

} // StartDownloadThread


HRESULT DownloadMessageLoop(
   DOWNLOAD_THREAD_CONTEXT* pThreadCtx,
   CComPtr<IWMPContentPartnerCallback> spCallback)
{
   DOWNLOAD_BATCH_CONTEXT* pBatchCtx = NULL;
   MSG msg = {0};
   HRESULT hr = S_OK;

   // Windows message loop
   while(TRUE)
   {
      BOOL ret = 0;

      // Suppose the user clicks several Download buttons in rapid
      // succession and then closes Windows Media Player. We would like
      // to let Windows Media Player close without waiting for all
      // those download messages to be processed.
      //
      // So we use the following strategy.
      //
      // Peek into the message queue to see if there is an exit message.
      // If there is an exit message anywhere in the queue, break out of
      // the message loop even though there might be several download
      // messages remaining in the queue.
      
      if( PeekMessage(
             &msg, 
             NULL, 
             pThreadCtx->msgExitMessageLoop, 
             pThreadCtx->msgExitMessageLoop,
             PM_REMOVE) )
      {
         ATLTRACE2("DownloadMessageLoop: PeekMessage retrieved an exit message.\n");
         goto cleanup;
      }

      ret = GetMessage(&msg, 0, 0, 0);
      if(-1 == ret)
      {
          ATLTRACE2("DownloadMessageLoop: GetMessage failed (returned -1).\n");
          hr = HRESULT_FROM_WIN32(GetLastError());
          goto cleanup;
      }
      

      if(pThreadCtx->msgDownloadBatch == msg.message)
      {
         CComPtr<IWMPContentContainerList> spContainerList;
         ULONG numContainers = 0;
         ULONG availableUrlStrings = sizeof(g_tracks)/sizeof(g_tracks[0]);

         ATLTRACE2("DownloadMessageLoop: GetMessage retrieved message to download a batch.\n");

         pBatchCtx = reinterpret_cast<DOWNLOAD_BATCH_CONTEXT*>(msg.lParam);
         
         if(NULL == pBatchCtx)
         {
            hr = E_UNEXPECTED;
            // Break out of the message loop and let
            // the download thread terminate.
            goto cleanup;
         }       

         // Get a pointer to an IWMPContentContainerList interface.

         if(NULL == pBatchCtx->pIStream)
         {
            hr = E_UNEXPECTED;
            // Break out of the message loop and let
            // the download thread terminate.
            goto cleanup;
         }

         hr = CoGetInterfaceAndReleaseStream(
            pBatchCtx->pIStream,
            __uuidof(IWMPContentContainerList),
            reinterpret_cast<LPVOID*>(&spContainerList) );

         // The stream was released (even if CoGetInterfaceAndReleaseStream failed). 
         // Set the stream pointer to NULL.
         pBatchCtx->pIStream = NULL;

         if(FAILED(hr))
         {
            ATLTRACE2("DownloadMessageLoop: Failed to get IWMPContentContainerList interface. %x\n", hr);          
            // Break out of the message loop and let
            // the download thread terminate.
            goto cleanup;
         }

         if(NULL == spContainerList)
         {
            hr = E_UNEXPECTED;
            // Break out of the message loop and let
            // the download thread terminate.
            goto cleanup;
         }

         ATLTRACE2("DownloadMessageLoop: Successfully obtained IWMPContentContainerList interface.\n");
     
         hr = spContainerList->GetContainerCount(&numContainers);

         if(FAILED(hr))
         {
            ATLTRACE2("DownloadMessageLoop: GetContainerCount failed. %x\n", hr);
            // Break out of the message loop and let
            // the download thread terminate.
            goto cleanup;
         }

         ATLTRACE2("DownloadMessageLoop: numContainers = %d.\n", numContainers);       
  
         for(ULONG j = 0; j < numContainers; ++j)
         {
            ULONG numItems = 0;
            ULONG containerID = 0;
            CComPtr<IWMPContentContainer> spContainer;

            hr = spContainerList->GetContainer(j, &spContainer);

            if(FAILED(hr))
            {
               break;  // Break out of the for-j loop.
            }
            
            hr = spContainer->GetContentCount(&numItems); 

            if(FAILED(hr))
            {
               // Make sure we don't enter the for-k loop.
               numItems = 0;
            }      
                
            ATLTRACE2("DownloadMessageLoop: Container has %d items.\n", numItems);

            for(ULONG k = 0; k < numItems; ++k)
            {
               ULONG itemID = 0;
               HRESULT hrDownload = S_OK;
               BSTR bstrUrl = NULL;
               WCHAR url[INTERNET_MAX_URL_LENGTH] = L"";
   
               hr = spContainer->GetContentID(k, &itemID);

               if(FAILED(hr))
               {
                  break;  // Break out of the for-k loop.
                  // This means we won't call DownloadTrack.
               }

               wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_audioRootURL);
                  
               if(itemID < availableUrlStrings)
               {    
                  wcscat_s(url, INTERNET_MAX_URL_LENGTH, g_tracks[itemID]);
               }
               else
               {
                  wcscat_s(url, INTERNET_MAX_URL_LENGTH, g_placeholderTrack);       
               }

               bstrUrl = SysAllocString(url);

               if(NULL == bstrUrl)
               {
                  hrDownload = E_OUTOFMEMORY;
               }                

               hr = spCallback->DownloadTrack(
                  pBatchCtx->cookie, 
                  bstrUrl,
                  itemID, 
                  NULL, 
                  hrDownload);

               // ToDo: Check hr for success or failure.

               SysFreeString(bstrUrl);

            } // for k
         } // for j   

         // The thread that posted msgDownloadBatch allocated a
         // DOWNLOAD_BATCH_CONTEXT structure.
         // We must free that memory here.

         if(NULL != pBatchCtx->pIStream)
         {
            // For some reason, CoGetInterfaceAndReleaseStream never got called.
            // So release the stream here.
            pBatchCtx->pIStream->Release();
            pBatchCtx->pIStream = NULL;
         }

         free(pBatchCtx);
         pBatchCtx = NULL;

      } // pThreadCtx->msgDownloadBatch == msg.message

      else if(pThreadCtx->msgExitMessageLoop == msg.message)
      {
         ATLTRACE2("DownloadMessageLoop: GetMessage retrieved an exit message.\n");
         goto cleanup;
      }

      else
      {
         ATLTRACE2("DownloadMessageLoop: GetMessage retrieved an unrecognized message %x.\n", msg.message);
         DispatchMessage(&msg);
      } 
  
   } // while(TRUE)


cleanup:

   // If we broke out of the message loop without freeing pBatchCtx, free it here.
   if(pBatchCtx)
   {
      if(NULL != pBatchCtx->pIStream)
      {
         // For some reason, CoGetInterfaceAndReleaseStream never got called.
         // So release the stream here.
         pBatchCtx->pIStream->Release();
         pBatchCtx->pIStream = NULL;
      }

      free(pBatchCtx);
      pBatchCtx = NULL;
   }


   // Clean out any download messages that remain in the queue.

   while( PeekMessage( 
             &msg, 
             NULL, 
             pThreadCtx->msgDownloadBatch, 
             pThreadCtx->msgDownloadBatch, 
             PM_REMOVE ) )
   {  
      ATLTRACE2("DownloadMessageLoop: PeekMessage in cleanup retrieved message to download a batch.\n");
      pBatchCtx = reinterpret_cast<DOWNLOAD_BATCH_CONTEXT*>(msg.lParam);

      if(pBatchCtx)
      {
         if(pBatchCtx->pIStream)
         {
            pBatchCtx->pIStream->Release();
            pBatchCtx->pIStream = NULL;         
         }

         free(pBatchCtx);
         pBatchCtx = NULL;
      }

   } // while PeekMessage

   return hr;
}
