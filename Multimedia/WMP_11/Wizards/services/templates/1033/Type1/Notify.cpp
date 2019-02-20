#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
//    SetCallback method
//    Notify method
//    SendMessage method
//    StationEvent method
//    UpdateDevice method
////////////////////////////////////////////////////////////////////


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::SetCallback(
   IWMPContentPartnerCallback* pCallback)
{
   HRESULT hr = S_OK;  

   if(pCallback)
   {
      // Store the callback interface pointer.
      m_spCallback = pCallback;
   }
   else
   {
      hr = this->ShutdownThreads();
      m_spCallback.Release();
   }

   return hr;
}

HRESULT STDMETHODCALLTYPE C[!output Safe_root]::Notify(
   WMPPartnerNotification type, 
   VARIANT *pContext)
{
   HRESULT hr = S_OK;

   ATLTRACE2("Notify: type = %d.\n", type);

   switch(type)
   {
   case wmpsnBackgroundProcessingBegin:

      // ToDo: If this is the first notification to begin
      // background processing, create a background-processing
      // thread.

      // ToDo: Set an event (put it in the signaled state)
      // so that the background-processing thread can run.
      break;

   case wmpsnBackgroundProcessingEnd:

      // ToDo: Reset an event (put it in the non-signaled state)
      // so that the background-processing thread will wait.
      break;

   case wmpsnCatalogDownloadFailure:
   
      // ToDo: Extract the error code from pContext->scode
      // and take appropriate action. 
      break;

   case wmpsnCatalogDownloadComplete:

      // ToDo: Set state indicating that catalog download is complete.
      break;

   default:

      hr = E_UNEXPECTED;
      break;
   }

   return hr;
}    


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::SendMessage( 
   BSTR bstrMsg,
   BSTR bstrParam)
{
   ATLTRACE2("SendMessage: bstrMsg = %S.\n", bstrMsg);

   // ToDo: If this is the first call to the SendMessage method,
   // create a thread that will handle messages.

   // ToDo: Post a message to the message-handling thread
   // and return immediately.

   // For examples of managing threads, see the 
   // RefreshLicense method and the Download method.

   return S_OK;
}
 
  
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::StationEvent( 
   BSTR bstrStationEventType,
   ULONG StationId,
   ULONG PlaylistIndex,
   ULONG TrackID,
   BSTR TrackData,
   DWORD dwSecondsPlayed)
{
   HRESULT hr = S_OK;

   ATLTRACE2("StationEvent: bstrStationEventType = %d.\n", bstrStationEventType);

   if(NULL == bstrStationEventType)
   {
      hr = E_INVALIDARG;
   }
   else if( 0 == wcscmp(g_szStationEvent_Started, bstrStationEventType) )
   {
      // ToDo: Log Started event.
   }
   else if( 0 == wcscmp(g_szStationEvent_Complete, bstrStationEventType) )
   {
      // ToDo: Log Completed event.
   }
   else if( 0 == wcscmp(g_szStationEvent_Skipped, bstrStationEventType) )
   {
      // ToDo: Log Skipped event.
   }
   else
   {
      hr = E_INVALIDARG;
   }

   return hr;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::UpdateDevice( 
   BSTR bstrDeviceName)
{
   ATLTRACE2("UpdateDevice\n");
   // ToDo: If this is the first call to the UpdateDevice method,
   // create a thread that will handle device updates.

   // ToDo: Post a message to the device-update thread and
   // return immediately.

   // For examples of managing threads, see the 
   // RefreshLicense method and the Download method.

   return S_OK;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::ShutdownThreads()
{
   BOOL postResult = 0;
   HRESULT hr = S_OK;
   HANDLE threadHandles[3] = {0, 0, 0};
   ULONG numThreadHandles = 0;

   // Tell the download thread to exit its message loop.
   if(m_downloadThreadHandle)
   {
      postResult = 0;
      postResult = PostThreadMessage(m_downloadThreadId, m_msgExitMessageLoop, 0, 0);

      if(0 == postResult)
      {
         hr = HRESULT_FROM_WIN32(GetLastError());
         ATLTRACE2("ShutdownThreads: Unable to post m_msgExitMessageLoop to the download thread. %x\n", hr);     
      }
      else
      {
         // There is a download thread, and we successfully 
         // posted an exit message to the download thread.
         ATLTRACE2("ShutdownThreads: Successfully posted m_msgExitMessageLoop to the download thread.\n");

         // Add the download thread handle to the array of handles.
         threadHandles[numThreadHandles] = m_downloadThreadHandle;
         ++numThreadHandles;
      }     
   }

   // Tell the buy thread to exit its message loop.
   if(m_buyThreadHandle)
   {
      postResult = 0;
      postResult = PostThreadMessage(m_buyThreadId, m_msgExitMessageLoop, 0, 0);

      if(0 == postResult)
      {
         hr = HRESULT_FROM_WIN32(GetLastError());  
         ATLTRACE2("ShutdownThreads: Unable to post m_msgExitMessageLoop to the buy thread. %x\n", hr);
      }
      else
      {
         // There is a buy thread, and we successfully 
         // posted an exit message to the buy thread.
         ATLTRACE2("ShutdownThreads: Successfully posted m_msgExitMessageLoop to the buy thread.\n");

         // Add the buy thread handle to the array of handles.
         threadHandles[numThreadHandles] = m_buyThreadHandle;
         ++numThreadHandles;
      }     
   }

   // Tell the refresh-license thread to exit its message loop.
   if(m_refreshLicenseThreadHandle)
   {
      postResult = 0;
      PostThreadMessage(m_refreshLicenseThreadId, m_msgExitMessageLoop, 0, 0);

      if(0 == postResult)
      {
         hr = HRESULT_FROM_WIN32(GetLastError());  
         ATLTRACE2("ShutdownThreads: Unable to post m_msgExitMessageLoop to the refresh-license thread. %x\n", hr);    
      }
      else
      {
         // There is a refresh-license thread, and we successfully 
         // posted an exit message to the refresh-license thread.
         ATLTRACE2("ShutdownThreads: Successfully posted m_msgExitMessageLoop to the refresh-license thread.\n");

         // Add the refresh-license thread handle to the array of handles.
         threadHandles[numThreadHandles] = m_refreshLicenseThreadHandle;
         ++numThreadHandles;
      }     
   }

   // If there are any active threads, wait for them to exit.
   if(numThreadHandles)
   {
      DWORD waitResult = WaitForMultipleObjects(
         numThreadHandles,                                   
         threadHandles,                       
         TRUE,
         30000);

      if(WAIT_FAILED == waitResult)
      {
         hr = HRESULT_FROM_WIN32(GetLastError()); 
         ATLTRACE2("ShutdownThreads: WaitForMultipleObjects failed. %x\n", hr);        
         goto cleanup;
      }

      ATLTRACE2("ShutdownThreads: Successfully waited for %d threads to exit.\n", numThreadHandles);     
   }
       

cleanup:

   if(m_downloadThreadHandle)
   {
      CloseHandle(m_downloadThreadHandle);
      m_downloadThreadHandle = 0;
      m_downloadThreadId = 0;
   }

   if(m_buyThreadHandle)
   {
      CloseHandle(m_buyThreadHandle);
      m_buyThreadHandle = 0;
      m_buyThreadId = 0;
   }

   if(m_downloadThreadHandle)
   {
      CloseHandle(m_refreshLicenseThreadHandle);
      m_refreshLicenseThreadHandle = 0;
      m_refreshLicenseThreadId = 0;
   }

   ATLTRACE2("ShutdownThreads: m_downloadThreadHandle has a value of %x.\n", m_downloadThreadHandle);
   ATLTRACE2("ShutdownThreads: m_downloadThreadId has a value of %x.\n", m_downloadThreadId);
   ATLTRACE2("ShutdownThreads: m_buyThreadHandle has a value of %x.\n", m_buyThreadHandle);
   ATLTRACE2("ShutdownThreads: m_buyThreadId has a value of %x.\n", m_buyThreadId);
   ATLTRACE2("ShutdownThreads: m_refreshLicenseThreadHandle has a value of %x.\n", m_refreshLicenseThreadHandle);
   ATLTRACE2("ShutdownThreads: m_refreshLicenseThreadId has a value of %x.\n", m_refreshLicenseThreadId);
   
   return hr;
}