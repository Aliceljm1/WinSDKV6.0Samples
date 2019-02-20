// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Jun 01 15:37:52 2005
 */
/* Compiler settings for .\ComSpyCtl.Idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IComSpy,0xA9B39080,0xAAD0,0x11D0,0xB1,0x83,0x00,0xAA,0x00,0xBA,0x32,0x58);


MIDL_DEFINE_GUID(IID, IID_ICOMSysLCE,0x0243D433,0x3849,0x11d2,0x89,0xBB,0x00,0xC0,0x4F,0x99,0x14,0x0F);


MIDL_DEFINE_GUID(IID, LIBID_COMSPYCTLLib,0xA9B39071,0xAAD0,0x11D0,0xB1,0x83,0x00,0xAA,0x00,0xBA,0x32,0x58);


MIDL_DEFINE_GUID(CLSID, CLSID_ComSpy,0xA9B39081,0xAAD0,0x11D0,0xB1,0x83,0x00,0xAA,0x00,0xBA,0x32,0x58);


MIDL_DEFINE_GUID(CLSID, CLSID_ComSpyPropPage,0xAAF6270D,0xAC5E,0x11D0,0xB1,0x85,0x00,0xAA,0x00,0xBA,0x32,0x58);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



