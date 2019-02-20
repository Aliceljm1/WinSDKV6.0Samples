#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
//    CanBuySilent method
//    Buy method
//    CompareContainerListPrices method
////////////////////////////////////////////////////////////////////

HRESULT BuyMessageLoop(
   BUY_THREAD_CONTEXT* pThreadCtx,
   CComPtr<IWMPContentPartnerCallback> spCallback);


DWORD WINAPI Buy_ThreadProc(LPVOID lpParameter)
{
   BUY_THREAD_CONTEXT* pThreadCtx = NULL;
   CComPtr<IWMPContentPartnerCallback> spCallback;
   MSG msg = {0};
   HRESULT hr = S_OK;
   BOOL comInitialized = FALSE;

   if(NULL == lpParameter)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   pThreadCtx = static_cast<BUY_THREAD_CONTEXT*>(lpParameter);

   if(NULL == pThreadCtx->pIStream)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

   if(FAILED(hr))
   {
      ATLTRACE2("Buy_ThreadProc: CoInitializeEx failed. %x\n", hr);
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
      ATLTRACE2("Buy_ThreadProc: Failed to get IWMPContentPartnerCallback interface. %x\n", hr);
      goto cleanup;
   }

   if(NULL == spCallback)
   {
      hr = E_UNEXPECTED;
      goto cleanup;
   }
   
   // Make sure we have a message loop.
   PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  
   // Tell the main thread that we are ready to receive messages.
   SetEvent(pThreadCtx->hInitialized);

   hr = BuyMessageLoop(pThreadCtx, spCallback);

cleanup:
   
   // Set this event here, just in case there was a "goto cleanup"
   // before the event was set.
   SetEvent(pThreadCtx->hInitialized);

   // The tread that started this thread allocated a
   // BUY_THREAD_CONTEXT structure.
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
      ATLTRACE2("Buy_ThreadProc: Calling CoUninitialize.\n", 0);
      CoUninitialize();
   }

   return hr;
}




        
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::CanBuySilent( 
   IWMPContentContainerList *pInfo,
   BSTR *pbstrTotalPrice,
   VARIANT_BOOL *pSilentOK)
{
   HRESULT hr = S_OK;

   // Initialize output parameters in case we fail.

   if(pbstrTotalPrice)
   {
      *pbstrTotalPrice = NULL;
   }

   if(pSilentOK)
   {
      *pSilentOK = VARIANT_FALSE;
   }


   if(NULL == pInfo || NULL == pbstrTotalPrice || NULL == pSilentOK)
   {
      return E_INVALIDARG;
   }

   ATLTRACE2("CanBuySilent\n");

   // ToDo: Determine whether this content can be purchased silently.

   *pbstrTotalPrice = SysAllocString(L"12 apples");

   if(NULL == *pbstrTotalPrice)
   {
      hr = E_OUTOFMEMORY;
   }
   
   *pSilentOK = VARIANT_TRUE; // or VARIANT_FALSE

cleanup:

   return hr;
}

       
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::Buy( 
   IWMPContentContainerList *pInfo,
   DWORD cookie)
{
   BUY_CONTEXT* pBuyCtx = NULL; 
   BOOL postResult = FALSE;
   HRESULT hr = S_OK;
   
   if(NULL == pInfo)
   {
      hr = E_INVALIDARG;
      goto cleanup;
   }

   // If the buy thread has not already been started,
   // start it now.
   
   if(!m_buyThreadHandle)
   {  
      hr = this->StartBuyThread();
      if(FAILED(hr))
      {
         ATLTRACE2("Buy: StartBuyThread failed. %x\n", hr);
         goto cleanup;
      } 
      
      ATLTRACE2("Buy: StartBuyThread succeeded.\n");    
   }

   // When we post a buy message, we must provide
   // two things: a cookie that represents the purchase and an
   // IStream interface that the buy thread can use to
   // obtain an IWMPContentContainerList interface. Those two
   // things are the members of BUY_CONTEXT.

   pBuyCtx = new BUY_CONTEXT();

   if(NULL == pBuyCtx)
   {
      ATLTRACE2("Buy: Failed to create new BUY_CONTEXT.\n");
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   ZeroMemory(pBuyCtx, sizeof(BUY_CONTEXT));
   
   hr = CoMarshalInterThreadInterfaceInStream(
      __uuidof(IWMPContentContainerList),
      pInfo,
      &pBuyCtx->pIStream); 

   if(FAILED(hr))
   {
      ATLTRACE2("Buy: Failed to marshal IWMPContentContainerList interface. %x\n", hr);
      goto cleanup;
   } 

   if(NULL == pBuyCtx->pIStream)
   {
      hr = E_UNEXPECTED;
      goto cleanup;
   }

   ATLTRACE2("Buy: Successfully marshaled IWMPContentContainerList interface.\n");
  
   pBuyCtx->cookie = cookie;

   postResult = PostThreadMessage(
      m_buyThreadId,
      m_msgBuy,
      0,
      reinterpret_cast<LPARAM>(pBuyCtx) );

   if(0 == postResult)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("Buy: PostThreadMessage failed. %x\n", hr);        
      goto cleanup;
   }
   
   ATLTRACE2("Buy: PostThreadMessage %x succeeded.\n");

   // We successfully posted the message to the buy thread.
   // We have no more need for the pointer to the buy context.
   pBuyCtx = NULL;

   // The buy thread must free the memory pointed to by pBuyCtx.

cleanup:

   if(pBuyCtx)
   {
      // We failed to post a message to the buy thread.
      // The buy thread will not be able to free the memory
      // pointed to by pBuyCtx.  So we free it here.      
      free(pBuyCtx);
      pBuyCtx = NULL;
   }

   // If pBuyCtx is NULL, buy thread will free the memory 
   // pointed to by pBuyCtx.  

   return hr;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::CompareContainerListPrices( 
   IWMPContentContainerList *pListBase,
   IWMPContentContainerList *pListCompare,
   long *pResult)
{
   // Initialize output parameter in case we fail.
   if(pResult)
   {
      *pResult = 0;
   }


   if(NULL == pListBase || NULL == pListCompare || NULL == pResult)
   {
      return E_INVALIDARG;
   }

   ATLTRACE2("CompareContainerListPrices\n");

   // ToDo: Compare the prices of the two container lists.

   *pResult = 0; // or -1 or 1

   return S_OK;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::StartBuyThread()
{
   DWORD waitResult = 0;
   HRESULT hr = S_OK;
   BUY_THREAD_CONTEXT* pThreadCtx = NULL;

   // What does the new thread need to get started?
   //    1. IStream that it can use to 
   //         unmarshal IWMPContentPartnerCallback
   //    2. Event handle it can use to signal that
   //         the message loop is started
   //    3. ID of the buy message
   //    4. ID of the message to exit the message loop     
   // Those four things are the members of BUY_THREAD_CONTEXT.

   pThreadCtx = new BUY_THREAD_CONTEXT();

   if(!pThreadCtx)
   {
      ATLTRACE2("StartBuyThread: Failed to create BUY_THREAD_CONTEXT.\n");
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }

   ZeroMemory(pThreadCtx, sizeof(BUY_THREAD_CONTEXT));
      
   hr = CoMarshalInterThreadInterfaceInStream(
      __uuidof(IWMPContentPartnerCallback), 
      m_spCallback, 
      &pThreadCtx->pIStream);

   if(FAILED(hr))
   {
      ATLTRACE2("StartBUYThread: Failed to marshal IWMPContentPartnerCallback interface. %x\n", hr);
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
      ATLTRACE2("StartBuyThread: CreateEvent failed.\n", 0);
      hr = E_OUTOFMEMORY;
      goto cleanup;
   }
 
   m_msgBuy = RegisterWindowMessage( _T("Buy") );

   if(0 == m_msgBuy)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartBuyThread: RegisterWindowMessage failed. %x\n", hr);     
      goto cleanup;
   }

   pThreadCtx->msgBuy = m_msgBuy;   
   pThreadCtx->msgExitMessageLoop = m_msgExitMessageLoop;
    
   m_buyThreadHandle = CreateThread(
      NULL,
      0,
      Buy_ThreadProc,
      static_cast<LPVOID>(pThreadCtx),
      0,
      &m_buyThreadId);

   if(NULL == m_buyThreadHandle)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartBuyThread: CreateThread failed.\n %x", hr);   
      goto cleanup;
   }
      
   // Wait for the buy thread to signal that it is initialized.
  
   waitResult = WaitForSingleObject(pThreadCtx->hInitialized, 30000);

   if(WAIT_FAILED == waitResult)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      ATLTRACE2("StartBuyThread: WaitForSingleObject failed. %x\n", hr);    
      goto cleanup;
   }
 
cleanup:

   if(pThreadCtx->hInitialized)
   {
      CloseHandle(pThreadCtx->hInitialized);
      pThreadCtx->hInitialized = 0;
   }

   if(0 == m_buyThreadHandle)
   {
      // The buy thread did not start; that is, CreateThread failed.
      // The buy thread will not be able to free the memory pointed
      // to by pThreadCtx, so we free it here.
 
      if(pThreadCtx)
      {
         free(pThreadCtx);
         pThreadCtx = NULL;
      }
   }

   // If 0 != m_buyThreadHandle, the buy thread 
   // will free the memory pointed to by pBuyCtx.

   // If 0 != m_buyThreadHandle, code in the 
   // IWMPContentPartner::SetCallback method will close
   // m_buyThreadHandle and set it to 0.

   return hr;

} // StartRefreshLicenseThread



HRESULT BuyMessageLoop(
   BUY_THREAD_CONTEXT* pThreadCtx,
   CComPtr<IWMPContentPartnerCallback> spCallback)
{
   BUY_CONTEXT* pBuyCtx = NULL;
   MSG msg = {0};
   HRESULT hr = S_OK;

// Windows message loop
   while(TRUE)
   {      
      BOOL ret = FALSE;

      // Suppose the user clicks several Buy buttons in rapid
      // succession and then closes Windows Media Player. We would like
      // to let Windows Media Player close without waiting for all
      // those buy messages to be processed.
      //
      // So we use the following strategy.
      //
      // Peek into the message queue to see if there is an exit message.
      // If there is an exit message anywhere in the queue, break out of
      // the message loop even though there might be several buy
      // messages remaining in the queue.
      
      if( PeekMessage(
             &msg, 
             NULL, 
             pThreadCtx->msgExitMessageLoop, 
             pThreadCtx->msgExitMessageLoop,
             PM_REMOVE) )
      {
         ATLTRACE2("BuyMessageLoop: PeekMessage retrieved an exit message.\n");
         goto cleanup;
      }

      ret = GetMessage(&msg, 0, 0, 0);
      if(-1 == ret)
      {
          ATLTRACE2("BuyThreadMessageLoop: GetMessage returned -1.\n");
          hr = HRESULT_FROM_WIN32(GetLastError());

          // Break out of the message loop and let
          // the buy thread terminate.
          goto cleanup;
      }


      if(pThreadCtx->msgBuy == msg.message)
      {
         CComPtr<IWMPContentContainerList> spContainerList = NULL;
         ULONG numContainers = 0;

         ATLTRACE2("BuyThreadMessageLoop: Received message to buy.\n");

         pBuyCtx = reinterpret_cast<BUY_CONTEXT*>(msg.lParam);

         if(NULL == pBuyCtx)
         {
            hr = E_UNEXPECTED;
            // Break out of the message loop and let
            // the buy thread terminate.
            goto cleanup;
         }       

         if(NULL == pBuyCtx->pIStream)
         {
            hr = E_UNEXPECTED;

            // Break out of the message loop and let
            // the buy thread terminate.
            goto cleanup;
         }

         // Get a pointer to an IWMPContentContainerList interface.

         hr = CoGetInterfaceAndReleaseStream(
            pBuyCtx->pIStream,
            __uuidof(IWMPContentContainerList),
            reinterpret_cast<LPVOID*>(&spContainerList) );

         // The stream was released (even if CoGetInterfaceAndReleaseStream failed). 
         // Set the stream pointer to NULL.
         pBuyCtx->pIStream = NULL;

         if(FAILED(hr))
         {
            ATLTRACE2("BuyThreadMessageLoop: Failed to get IWMPContentContainerList interface. %x\n", hr);
            // Break out of the message loop and let
            // the buy thread terminate.
            goto cleanup;
         }

         if(NULL == spContainerList)
         {
            hr = E_UNEXPECTED;
            goto cleanup;
         }

         ATLTRACE2("BuyThreadMessageLoop: Successfully obtained IWMPContentContainerList interface.\n");
         
         hr = spContainerList->GetContainerCount(&numContainers);

         if(FAILED(hr))
         {
            ATLTRACE2("BuyThreadMessageLoop: GetContainerCount failed. %x\n", hr);
            // Break out of the message loop and let
            // the buy thread terminate.
            goto cleanup;
         }

         ATLTRACE2("BuyThreadMessageLoop: numContainers = %d.\n", numContainers);

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

            for(ULONG k = 0; k < numItems; ++k)
            {
               ULONG itemID = 0;

               hr = spContainer->GetContentID(k, &itemID);

               if(FAILED(hr))
               {
                  break;  // Break out of the for-k loop.
               }

               // ToDo: Update records to reflect the purchase of this item.

               ATLTRACE2("BuyThreadMessageLoop: Buying item %d.\n", itemID);

            } // for k
         } // for j   

         // Tell Windows Media Player that we have finished 
         // processing the purchase request.

         hr = spCallback->BuyComplete(S_OK, pBuyCtx->cookie);

         if(FAILED(hr))
         {
            ATLTRACE2("BuyThreadMessageLoop: BuyComplete failed. %x\n", hr);
         }

         // The thread that posted msgBuy allocated a
         // BUY_CONTEXT structure.
         // We must free that memory here.

         if(NULL != pBuyCtx->pIStream)
         {
            // For some reason, CoGetInterfaceAndReleaseStream never got called.
            // So release the stream here.
            pBuyCtx->pIStream->Release();
            pBuyCtx->pIStream = NULL;
         }

         free(pBuyCtx);
         pBuyCtx = NULL;

      } // pThreadCtx->msgBuy == msg.message

      else if(pThreadCtx->msgExitMessageLoop == msg.message)
      {
         ATLTRACE2("BuyThreadMessageLoop: Received message to exit the message loop.\n");
         
         // Break out of the message loop and let
         // the buy thread terminate.
         goto cleanup;
      } 
      else
      {
         ATLTRACE2("BuyThreadMessageLoop: Received unexpected message. %x\n", msg.message);
         DispatchMessage(&msg);
      } 
  
   } // while(TRUE) -- message loop

cleanup:

   // If we broke out of the message loop without freeing pBuyCtx, free it here.
   if(pBuyCtx)
   {
      if(NULL != pBuyCtx->pIStream)
      {
         // For some reason, CoGetInterfaceAndReleaseStream never got called.
         // So release the stream here.
         pBuyCtx->pIStream->Release();
         pBuyCtx->pIStream = NULL;
      }

      free(pBuyCtx);
      pBuyCtx = NULL;
   }


    // Clean out any buy messages that remain in the queue.

   while( PeekMessage( 
             &msg, 
             NULL, 
             pThreadCtx->msgBuy, 
             pThreadCtx->msgBuy, 
             PM_REMOVE ) )
   {  
      ATLTRACE2("BuyMessageLoop: PeekMessage in cleanup retrieved a buy message.\n");
      pBuyCtx = reinterpret_cast<BUY_CONTEXT*>(msg.lParam);

      if(pBuyCtx)
      {
         if(pBuyCtx->pIStream)
         {
            pBuyCtx->pIStream->Release();
            pBuyCtx->pIStream = NULL;         
         }

         free(pBuyCtx);
         pBuyCtx = NULL;
      }

   } // while PeekMessage

   return hr;
}