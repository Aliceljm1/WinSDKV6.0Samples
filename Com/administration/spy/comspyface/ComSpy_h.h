// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Jun 01 15:32:15 2005
 */
/* Compiler settings for ..\ComSpy\ComSpy.Idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ComSpy_h_h__
#define __ComSpy_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISpyCon_FWD_DEFINED__
#define __ISpyCon_FWD_DEFINED__
typedef interface ISpyCon ISpyCon;
#endif 	/* __ISpyCon_FWD_DEFINED__ */


#ifndef __SpyCon_FWD_DEFINED__
#define __SpyCon_FWD_DEFINED__

#ifdef __cplusplus
typedef class SpyCon SpyCon;
#else
typedef struct SpyCon SpyCon;
#endif /* __cplusplus */

#endif 	/* __SpyCon_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ISpyCon_INTERFACE_DEFINED__
#define __ISpyCon_INTERFACE_DEFINED__

/* interface ISpyCon */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISpyCon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("27EF6FD1-AF60-11D0-B188-00AA00BA3258")
    ISpyCon : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpyConVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISpyCon * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISpyCon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISpyCon * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            ISpyCon * This);
        
        END_INTERFACE
    } ISpyConVtbl;

    interface ISpyCon
    {
        CONST_VTBL struct ISpyConVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpyCon_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpyCon_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpyCon_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpyCon_Run(This)	\
    (This)->lpVtbl -> Run(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISpyCon_Run_Proxy( 
    ISpyCon * This);


void __RPC_STUB ISpyCon_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpyCon_INTERFACE_DEFINED__ */



#ifndef __COMSPYLib_LIBRARY_DEFINED__
#define __COMSPYLib_LIBRARY_DEFINED__

/* library COMSPYLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COMSPYLib;

EXTERN_C const CLSID CLSID_SpyCon;

#ifdef __cplusplus

class DECLSPEC_UUID("27EF6FD2-AF60-11D0-B188-00AA00BA3258")
SpyCon;
#endif
#endif /* __COMSPYLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


