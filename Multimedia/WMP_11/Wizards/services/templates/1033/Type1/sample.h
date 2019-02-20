#pragma once

#include "resource.h"

static const WCHAR g_rootURL[] = L"http://www.microsoft.com/windows/windowsmedia/player/11/samples/SampleType1Store/";
static const WCHAR g_audioRootURL[] = L"http://download.microsoft.com/download/a/e/b/aeb86cba-7d91-4eea-9e29-8d53c4bc05aa/";


static const WCHAR g_placeholderTrack[] = L"Placeholder.wma";
static ULONG g_totalDownloadFailures = 0;

static const WCHAR* g_tracks[] =
{
   L"House.wma",
   L"Jeanne.wma",
   L"Laure.wma",
   L"Mellow.wma",
   L"Multilang.wma",
};

typedef struct _DOWNLOAD_BATCH_CONTEXT
{
   IStream* pIStream;
   DWORD cookie;
} DOWNLOAD_BATCH_CONTEXT;

typedef struct _DOWNLOAD_THREAD_CONTEXT
{  
   IStream* pIStream;
   HANDLE hInitialized;
   UINT msgDownloadBatch;
   UINT msgExitMessageLoop;
} DOWNLOAD_THREAD_CONTEXT;


typedef struct _BUY_CONTEXT
{
   IStream* pIStream;
   DWORD cookie;
} BUY_CONTEXT;

typedef struct _BUY_THREAD_CONTEXT
{  
   IStream* pIStream;
   HANDLE hInitialized;
   UINT msgBuy;
   UINT msgExitMessageLoop;
} BUY_THREAD_CONTEXT;

typedef struct _REFRESH_LICENSE_CONTEXT
{
  DWORD dwCookie;
  VARIANT_BOOL fLocal;
  BSTR bstrURL;
  WMPStreamingType type;
  ULONG contentID;
  BSTR  bstrRefreshReason;
  VARIANT* pReasonContext;
} REFRESH_LICENSE_CONTEXT;

typedef struct _REFRESH_LICENSE_THREAD_CONTEXT
{  
   IStream* pIStream;
   HANDLE hInitialized;
   UINT msgRefreshLicense;
   UINT msgExitMessageLoop;
} REFRESH_LICENSE_THREAD_CONTEXT;


class __declspec( uuid("{[!output CLASSID]}") ) 
C[!output Safe_root]:
   public CComObjectRootEx<CComSingleThreadModel>,
   public CComCoClass<C[!output Safe_root], &__uuidof(C[!output Safe_root])>,
   public IWMPContentPartner
{

public:

   DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY1)

   BEGIN_COM_MAP(C[!output Safe_root])
      COM_INTERFACE_ENTRY(IWMPContentPartner)
   END_COM_MAP()

   virtual HRESULT STDMETHODCALLTYPE SetCallback(
      IWMPContentPartnerCallback* pCallback);

   virtual HRESULT STDMETHODCALLTYPE Notify(
      WMPPartnerNotification type, 
      VARIANT *pContext);     

   virtual HRESULT STDMETHODCALLTYPE GetItemInfo(
      BSTR bstrInfoName,
      VARIANT *pContext,
      VARIANT *pData);

   virtual HRESULT STDMETHODCALLTYPE GetContentPartnerInfo(
      BSTR bstrInfoName,
      VARIANT *pData);
        
   virtual HRESULT STDMETHODCALLTYPE GetCommands( 
      BSTR location,
      VARIANT *pLocationContext,
      BSTR itemLocation,
      ULONG cItemIDs,
      ULONG *prgItemIDs,
      ULONG *pcItemIDs,
      WMPContextMenuInfo **pprgItems);
        
   virtual HRESULT STDMETHODCALLTYPE InvokeCommand( 
      DWORD dwCommandID,
      BSTR location,
      VARIANT *pLocationContext,
      BSTR itemLocation,
      ULONG cItemIDs,
      ULONG *rgItemIDs);
        
   virtual HRESULT STDMETHODCALLTYPE CanBuySilent( 
      IWMPContentContainerList *pInfo,
      BSTR *pbstrTotalPrice,
      VARIANT_BOOL *pSilentOK);
        
   virtual HRESULT STDMETHODCALLTYPE Buy( 
      IWMPContentContainerList *pInfo,
      DWORD cookie);
        
   virtual HRESULT STDMETHODCALLTYPE GetStreamingURL( 
      WMPStreamingType st,
      VARIANT *pStreamContext,
      BSTR *pbstrURL);
        
   virtual HRESULT STDMETHODCALLTYPE Download( 
      IWMPContentContainerList *pInfo,
      DWORD cookie);
        
   virtual HRESULT STDMETHODCALLTYPE DownloadTrackComplete( 
      HRESULT hrResult,
      ULONG contentID,
      BSTR downloadTrackParam);
        
   virtual HRESULT STDMETHODCALLTYPE RefreshLicense( 
      DWORD dwCookie,
      VARIANT_BOOL fLocal,
      BSTR bstrURL,
      WMPStreamingType type,
      ULONG contentID,
      BSTR bstrRefreshReason,
      VARIANT *pReasonContext);
        
   virtual HRESULT STDMETHODCALLTYPE GetCatalogURL( 
      DWORD dwCatalogVersion,
      DWORD dwCatalogSchemaVersion,
      LCID catalogLCID,
      DWORD *pdwNewCatalogVersion,
      BSTR *pbstrCatalogURL,
      VARIANT *pExpirationDate);
        
   virtual HRESULT STDMETHODCALLTYPE GetTemplate( 
      WMPTaskType task,
      BSTR location,
      VARIANT *pContext,
      BSTR clickLocation,
      VARIANT *pClickContext,
      BSTR bstrFilter,
      BSTR bstrViewParams,
      BSTR *pbstrTemplateURL,
      WMPTemplateSize *pTemplateSize);
        
   virtual HRESULT STDMETHODCALLTYPE UpdateDevice( 
      BSTR bstrDeviceName);
        
   virtual HRESULT STDMETHODCALLTYPE GetListContents( 
      BSTR location,
      VARIANT *pContext,
      BSTR bstrListType,
      BSTR bstrParams,
      DWORD dwListCookie);
        
   virtual HRESULT STDMETHODCALLTYPE Login( 
      BLOB userInfo,
      BLOB pwdInfo,
      VARIANT_BOOL fUsedCachedCreds,
      VARIANT_BOOL fOkToCache);
        
   virtual HRESULT STDMETHODCALLTYPE Authenticate( 
      BLOB userInfo,
      BLOB pwdInfo);
        
   virtual HRESULT STDMETHODCALLTYPE Logout(void);
        
   virtual HRESULT STDMETHODCALLTYPE SendMessage( 
      BSTR bstrMsg,
      BSTR bstrParam);
        
   virtual HRESULT STDMETHODCALLTYPE StationEvent( 
      BSTR bstrStationEventType,
      ULONG StationId,
      ULONG PlaylistIndex,
      ULONG TrackID,
      BSTR TrackData,
      DWORD dwSecondsPlayed);
        
   virtual HRESULT STDMETHODCALLTYPE CompareContainerListPrices( 
      IWMPContentContainerList *pListBase,
      IWMPContentContainerList *pListCompare,
      long *pResult);
        
   virtual HRESULT STDMETHODCALLTYPE VerifyPermission( 
      BSTR bstrPermission,
      VARIANT *pContext);

   C[!output Safe_root]();
   HRESULT STDMETHODCALLTYPE FinalConstruct();
   ~C[!output Safe_root]();

private:
   HRESULT STDMETHODCALLTYPE StartDownloadThread();
   HRESULT STDMETHODCALLTYPE StartBuyThread();
   HRESULT STDMETHODCALLTYPE StartRefreshLicenseThread();
   HRESULT STDMETHODCALLTYPE ShutdownThreads();

   CComPtr<IWMPContentPartnerCallback> m_spCallback;

   // Members related to the download thread
   HANDLE m_downloadThreadHandle;
   DWORD m_downloadThreadId;
   UINT m_msgDownloadBatch;

   // Members related to the buy thread
   HANDLE m_buyThreadHandle;
   DWORD m_buyThreadId;
   UINT m_msgBuy;

   // Members related to the refresh-license thread
   HANDLE m_refreshLicenseThreadHandle;
   DWORD m_refreshLicenseThreadId;
   UINT m_msgRefreshLicense;

   UINT m_msgExitMessageLoop;  
};

