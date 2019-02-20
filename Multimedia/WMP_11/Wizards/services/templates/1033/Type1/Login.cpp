#include "stdafx.h"
#include "[!output root].h"

////////////////////////////////////////////////////////////////////
// C[!output Safe_root]
//    Login method
//    Authenticate method
//    Logout method
////////////////////////////////////////////////////////////////////

        
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::Login( 
   BLOB userInfo,
   BLOB pwdInfo,
   VARIANT_BOOL fUsedCachedCreds,
   VARIANT_BOOL fOkToCache)
{
   ATLTRACE2("Login\n");
   return S_OK;
}
        
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::Authenticate( 
   BLOB userInfo,
   BLOB pwdInfo)
{
   ATLTRACE2("Authenticate\n");
   return S_OK;
}
        
HRESULT STDMETHODCALLTYPE C[!output Safe_root]::Logout(void)
{ 
   ATLTRACE2("Logout\n");
   return S_OK;
}
 