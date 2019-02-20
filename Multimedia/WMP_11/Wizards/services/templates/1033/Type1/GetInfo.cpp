#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
//    GetItemInfo method
//    GetContentPartnerInfo method
//    GetStreamingURL method
//    GetCatalogURL method
//    GetTemplate method
//    GetListContents method
////////////////////////////////////////////////////////////////////


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetItemInfo(
   BSTR bstrInfoName,   // in
   VARIANT *pContext,   // in
   VARIANT *pData)      // out
{
   WCHAR url[INTERNET_MAX_URL_LENGTH] = L"";
   HRESULT hr = S_OK;

   // Set output parameter type to VT_EMPTY in case we fail.
   if(NULL != pData)
   {
      VariantInit(pData);    
   }

   if(NULL == bstrInfoName || NULL == pContext || NULL == pData)
   {
      return E_INVALIDARG;
   }

   ATLTRACE2("GetItemInfo: pstrInfoName = %S.\n", bstrInfoName);

   if( 0 == wcscmp(bstrInfoName, g_szItemInfo_PopupURL) )
   {

      // ToDo: Get the index of the popup by inspecting pContext->lVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/Popup.htm");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // Note: The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_AuthenticationSuccessURL) )
   {   
      // ToDo: Get the index by inspecting pContext->lVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/AuthenticationSuccess.htm");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_LoginFailureURL) )
   {    
      // ToDo: Get the index by inspecting pContext->ulVal.  

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/LoginFailure.htm");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_HTMLViewURL) )
   {     
      // We are passed a string value from an ASX file.
      // <param name="HTMLFLINK" value="xxx">

      // ToDo: Get the string by inspecting pContext->bstrVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/HtmlView.htm");
     
      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal;
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_PopupCaption) )
   {   
      // ToDo: Get the index by inspecting pContext->lVal.
      
      pData->bstrVal = SysAllocString(L"CP Test Popup Caption");

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_ForgetPasswordURL) )
   {
      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/ForgotPassword.htm");
      
      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_CreateAccountURL) )
   {    
      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/CreateAccount.htm");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_ArtistArtURL) )
   {
      // ToDo: Get the artist ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/Artist.png");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_AlbumArtURL) )
   {
      // ToDo: Get the album ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/Album.png");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_ListArtURL) )
   {
      // ToDo: Get the list ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/List.png");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_GenreArtURL) )
   {
      // ToDo: Get the genre ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/Genre.png");
;
      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_SubGenreArtURL) )
   {
      // ToDo: Get the sub-genre ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/Subgenre.png");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_RadioArtURL) )
   {
      // ToDo: Get the radio playlist ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/Radio.png");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_TreeListIconURL) )
   {
      // ToDo: Get the list ID by inspecting pContext->ulVal.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Images/Tree.png");
      
      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_ErrorDescription) )
   {
      // ToDo: Get the error code by inspecting pContext->scode.

      pData->bstrVal = SysAllocString(L"Test error description. The operation was aborted.");

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_ErrorURL) )
   {
      // ToDo: Get the error code by inspecting pContext->scode.

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/Error.htm");

      pData->bstrVal = SysAllocString(url);

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if( 0 == wcscmp(bstrInfoName, g_szItemInfo_ErrorURLLinkText) )
   {
      // ToDo: Get the error code by inspecting pContext->scode.

      pData->bstrVal = SysAllocString(L"Test error link text: Error Details");

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else 
   {
      hr = E_UNEXPECTED;
   }

   return hr;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetContentPartnerInfo(
   BSTR bstrInfoName,
   VARIANT *pData)
{
   HRESULT hr = S_OK;

   // Set the output parameter type to VT_EMPTY in case we fail.
   if(NULL != pData)
   {
      VariantInit(pData);    
   }

   if(NULL == bstrInfoName || NULL == pData)
   {
      return E_INVALIDARG;
   }

   ATLTRACE2("GetContentPartnerInfo: bstrInfoName = %S.\n", bstrInfoName);

   if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_LoginState))
   {
      // ToDo: Determine whether the user is logged in.

      pData->vt = VT_BOOL;
      pData->boolVal = VARIANT_TRUE; // or VARIANT_FALSE
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_MediaPlayerAccountType))
   {
      // ToDo: Determine the account type.

      pData->vt = VT_UI4;
      pData->ulVal = wmpatSubscription; // or another enumeration member  
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_AccountType))
   {   
      pData->bstrVal = SysAllocString(L"Super Subscription Account");

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_HasCachedCredentials))
   {
      pData->vt = VT_BOOL;
      pData->boolVal = VARIANT_TRUE; // or VARIANT_FALSE
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_LicenseRefreshAdvanceWarning))
   {
      pData->vt = VT_UI4;
      pData->ulVal = 5; // or some other number of days
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_PurchasedTrackRequiresReDownload))
   {
      pData->vt= VT_BOOL;
      pData->boolVal = VARIANT_TRUE; // or VARIANT_FALSE
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_MaximumTrackPurchasePerPurchase))
   {
      pData->vt = VT_UI4;
      pData->ulVal = 30; // or some other number of tracks
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_AccountBalance))
   {
      pData->bstrVal = SysAllocString(L"$dd.cc"); // Replace $dd.cc with actual balance.

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }
      
      // The caller must free pData->bstrVal.
   }

   else if(0 == wcscmp(bstrInfoName, g_szContentPartnerInfo_UserName))
   {
      pData->bstrVal = SysAllocString(L"xxx"); // Replace xxx with user's name.

      if(NULL != pData->bstrVal)
      {
          pData->vt = VT_BSTR;
      }
      else
      {
          hr = E_OUTOFMEMORY;
      }

      // The caller must free pData->bstrVal.
   }

   else
   {
      hr = E_UNEXPECTED;
   }

   return hr;
}



HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetStreamingURL( 
   WMPStreamingType st,
   VARIANT *pStreamContext,
   BSTR *pbstrURL)
{
   HRESULT hr = S_OK;
   WCHAR url[INTERNET_MAX_URL_LENGTH] = L"";
   ULONG trackId = 0xffffffff;
   ULONG availableUrlStrings = 0;

   // Set the output parameter to NULL in case we fail.
   if(NULL != pbstrURL)
   {
      *pbstrURL = NULL;
   }

   if(NULL == pStreamContext || NULL == pbstrURL)
   {
      return E_INVALIDARG;
   }

   if(VT_UI4 != pStreamContext->vt)
   {
      return E_INVALIDARG;
   }

   ATLTRACE2("GetStreamingURL: st = %d.\n", st);
 

   switch(st)
   {
   case wmpstMusic:

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_audioRootURL);

      // Get the track ID from pContext.
      trackId = pStreamContext->ulVal;

      availableUrlStrings = sizeof(g_tracks)/sizeof(g_tracks[0]);

      if(trackId < availableUrlStrings)
      {    
         wcscat_s(url, INTERNET_MAX_URL_LENGTH, g_tracks[trackId]);
      }
      else
      {
         wcscat_s(url, INTERNET_MAX_URL_LENGTH, g_placeholderTrack);       
      }

      *pbstrURL = SysAllocString(url);

      if(NULL == *pbstrURL)
      {
         hr = E_OUTOFMEMORY;
      }

      // The caller must free *pbstrURL.
      break;

   case wmpstVideo:
       // ToDo: Get the media item ID by inspecting pStreamContext->ulVal.
       // Return the URL.
   case wmpstRadio:
      // ToDo: Get the media item ID by inspecting pStreamContext->ulVal. 
      // Return the URL.    
   default:
      *pbstrURL = NULL;
      hr = E_UNEXPECTED;
      break;

   }  // switch

   return hr;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetCatalogURL( 
   DWORD dwCatalogVersion,
   DWORD dwCatalogSchemaVersion,
   LCID catalogLCID,
   DWORD *pdwNewCatalogVersion,  // out
   BSTR *pbstrCatalogURL,        // out
   VARIANT *pExpirationDate)     // out
{
   WCHAR url[INTERNET_MAX_URL_LENGTH] = L"";
   HRESULT hr = S_OK;

   // Initialize output parameters in case we fail.

   if(NULL != pdwNewCatalogVersion)
   {
      *pdwNewCatalogVersion = 0;
   }

   if(NULL != pbstrCatalogURL)
   {
      *pbstrCatalogURL = NULL;
   }

   if(NULL != pExpirationDate)
   {
      VariantInit(pExpirationDate);
   }


   if(NULL == pdwNewCatalogVersion || 
      NULL == pbstrCatalogURL || 
      NULL == pExpirationDate)
   {
      return E_INVALIDARG;
   }

   ATLTRACE("GetCatalogURL:\n");
   ATLTRACE("    dwCatalogVersion = %d.\n", dwCatalogVersion);
   ATLTRACE("    dwCatalogSchemaVersion = %d.\n", dwCatalogSchemaVersion);

   *pdwNewCatalogVersion = 2; // or some other catalog version

   wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
   wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Catalog/catalog.wmdb.lz");  
 
   *pbstrCatalogURL = SysAllocString(url);

   if(NULL == *pbstrCatalogURL)
   {
      hr = E_OUTOFMEMORY;
   }

   // The caller must free *pbstrCatalogURL.

   pExpirationDate->vt = VT_DATE;
   pExpirationDate->date = 43831.00; // January 1, 2020

   return hr;
}
        
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetTemplate( 
   WMPTaskType task,
   BSTR location,
   VARIANT *pContext,
   BSTR clickLocation,
   VARIANT *pClickContext,
   BSTR bstrFilter,
   BSTR bstrViewParams,
   BSTR *pbstrTemplateURL,           // out
   WMPTemplateSize *pTemplateSize)   // out
{
   HRESULT hr = S_OK;
   WCHAR url[INTERNET_MAX_URL_LENGTH] = L"";

   // Initialize output parameters in case we fail.

   if(NULL != pbstrTemplateURL)
   {
      *pbstrTemplateURL = NULL;
   }

   if(NULL != pTemplateSize)
   {
      *pTemplateSize = wmptsSmall; // wmpstSmall = 0
   }


   if(NULL == pbstrTemplateURL || NULL == pTemplateSize)
   {
      return E_INVALIDARG;
   }

   ATLTRACE2("GetTemplate: task = %d, location = %S.\n", task, location);

   switch(task)
   {
   case wmpttBrowse:

      *pTemplateSize = wmptsLarge;

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/library.htm");    
      *pbstrTemplateURL = SysAllocString(url);

      if(NULL == *pbstrTemplateURL)
      {
         hr = E_OUTOFMEMORY;
      }

      // The caller must free *pbstrTemplateURL.
      break;

   case wmpttSync:

      *pTemplateSize = wmptsMedium;

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/sync.htm");    
      *pbstrTemplateURL = SysAllocString(url);

      if(NULL == *pbstrTemplateURL)
      {
         hr = E_OUTOFMEMORY;
      }

      // The caller must free *pbstrTemplateURL.      
      break;

   case wmpttBurn:
 
      if( 0 == wcscmp(g_szAllCPAlbumIDs, location) || 
          0 == wcscmp(g_szCPAlbumID, location) )
	  {
         *pTemplateSize = wmptsMedium;
         wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
         wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/BurnAlbums.htm");    
	  }
	  else if( 0 == wcscmp(g_szAllCPTrackIDs, location) || 
               0 == wcscmp(g_szCPTrackID, location) )
	  {
         *pTemplateSize = wmptsMedium;
         wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
         wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/BurnSongs.htm");    
	  }
	  else
	  {
         *pTemplateSize = wmptsSmall;
         wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
         wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/Burn.htm");    
	  }
    
      *pbstrTemplateURL = SysAllocString(url);

      if(NULL == *pbstrTemplateURL)
      {
         hr = E_OUTOFMEMORY;
      }

      // The caller must free *pbstrTemplateURL.
      break;

   case wmpttCurrent:

      *pTemplateSize = wmptsSmall;

      wcscpy_s(url, INTERNET_MAX_URL_LENGTH, g_rootURL);
      wcscat_s(url, INTERNET_MAX_URL_LENGTH, L"Pages/library.htm");    
      *pbstrTemplateURL = SysAllocString(url);

      if(NULL == *pbstrTemplateURL)
      {
         hr = E_OUTOFMEMORY;
      }

      // The caller must free *pbstrTemplateURL.
      break;

   default:
      *pbstrTemplateURL = NULL;
      hr = E_INVALIDARG;
      break;
   }

   return hr;
}


HRESULT STDMETHODCALLTYPE C[!output Safe_root]::GetListContents( 
   BSTR location,
   VARIANT *pContext,
   BSTR bstrListType,
   BSTR bstrParams,
   DWORD dwListCookie)
{
   ATLTRACE2("GetListContents: location = %S.\n", location);

   // ToDo: If this is the first call to the GetListContents method,
   // create a thread that will handle list-retrieval requests.

   // ToDo: Post a message to the list-retrieval thread and
   // return immediately.

   // For examples of managing threads, see the 
   // RefreshLicense method and the Download method.

   return S_OK;
}