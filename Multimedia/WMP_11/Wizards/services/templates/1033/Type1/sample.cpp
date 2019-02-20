#include "stdafx.h"
#include "[!output root].h"

OBJECT_ENTRY_AUTO(__uuidof(C[!output Safe_root]), C[!output Safe_root])

C[!output Safe_root]::C[!output Safe_root]()
{
   m_downloadThreadHandle = 0;
   m_downloadThreadId = 0;
   m_msgDownloadBatch = 0;
   m_buyThreadHandle = 0;
   m_buyThreadId = 0;
   m_msgBuy = 0;
   m_refreshLicenseThreadHandle = 0;
   m_refreshLicenseThreadId = 0;
   m_msgRefreshLicense = 0;
   m_msgExitMessageLoop = 0;  
}


HRESULT C[!output Safe_root]::FinalConstruct()
{
   DWORD lastError = ERROR_SUCCESS;
   HRESULT hr = S_OK;

   m_msgExitMessageLoop = RegisterWindowMessage(_T("ExitMessageLoop"));

   if(0 == m_msgExitMessageLoop)
   {
      lastError = GetLastError();
      ATLTRACE2("FinalConstruct: RegisterWindowMessage failed. %x\n", lastError);
      hr = HRESULT_FROM_WIN32(lastError);
   }

   return hr;
}

C[!output Safe_root]::~C[!output Safe_root]()
{
   ATLTRACE2("[!output Safe_root] destructor: Total download failures: %d\n", 
      g_totalDownloadFailures); 
}


class C[!output Safe_root]Module : public CAtlDllModuleT< C[!output Safe_root]Module >
{};

C[!output Safe_root]Module _AtlModule;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   return _AtlModule.DllMain(dwReason, lpReserved); 
}

STDAPI DllCanUnloadNow(void)
{
   return _AtlModule.DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
   return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
   // There is no type library, so pass
   // FALSE to _AtlModule.DllRegistreServer
   HRESULT hr = _AtlModule.DllRegisterServer(FALSE);
	return hr;
}

STDAPI DllUnregisterServer(void)
{
   // There is no type library, so pass
   // FALSE to _AtlModule.DllUnRegistreServer
	HRESULT hr = _AtlModule.DllUnregisterServer(FALSE);
	return hr;
}
