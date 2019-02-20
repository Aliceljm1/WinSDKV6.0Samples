

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0493 */
/* at Sat May 20 11:34:18 2006
 */
/* Compiler settings for .\PreviewHandlerSDKSample.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __PreviewHandlerSDKSample_h__
#define __PreviewHandlerSDKSample_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __RecipePreviewHandler_FWD_DEFINED__
#define __RecipePreviewHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class RecipePreviewHandler RecipePreviewHandler;
#else
typedef struct RecipePreviewHandler RecipePreviewHandler;
#endif /* __cplusplus */

#endif 	/* __RecipePreviewHandler_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __PreviewHandlerSDKSampleLib_LIBRARY_DEFINED__
#define __PreviewHandlerSDKSampleLib_LIBRARY_DEFINED__

/* library PreviewHandlerSDKSampleLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PreviewHandlerSDKSampleLib;

EXTERN_C const CLSID CLSID_RecipePreviewHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("F654F1BF-54D9-4A2E-B703-889091D3CB2D")
RecipePreviewHandler;
#endif
#endif /* __PreviewHandlerSDKSampleLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


