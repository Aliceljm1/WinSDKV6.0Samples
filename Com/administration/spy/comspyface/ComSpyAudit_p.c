// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Jun 01 15:50:17 2005
 */
/* Compiler settings for .\ComSpyAudit.Idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "ComSpyAudit.h"

#define TYPE_FORMAT_STRING_SIZE   1043                              
#define PROC_FORMAT_STRING_SIZE   4621                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IComSpyAudit_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IComSpyAudit_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure OnThreadStart */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 10 */	NdrFcShort( 0x18 ),	/* 24 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 30 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 32 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 34 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 36 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 38 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 40 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszW2KThreadID */

/* 42 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 44 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 46 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwThreadCnt */

/* 48 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 50 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 52 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 54 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 56 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 58 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadTerminate */

/* 60 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 62 */	NdrFcLong( 0x0 ),	/* 0 */
/* 66 */	NdrFcShort( 0x4 ),	/* 4 */
/* 68 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 70 */	NdrFcShort( 0x18 ),	/* 24 */
/* 72 */	NdrFcShort( 0x8 ),	/* 8 */
/* 74 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 76 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 84 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 86 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 88 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 90 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 92 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 94 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 96 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 98 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 100 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszW2KThreadID */

/* 102 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 104 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 106 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwThreadCnt */

/* 108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 110 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 116 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadBindToApartment */

/* 120 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 122 */	NdrFcLong( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x5 ),	/* 5 */
/* 128 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 130 */	NdrFcShort( 0x18 ),	/* 24 */
/* 132 */	NdrFcShort( 0x8 ),	/* 8 */
/* 134 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 136 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 144 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 146 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 148 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 150 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 156 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 158 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 160 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszAptID */

/* 162 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 164 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 166 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwActivityCnt */

/* 168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 170 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadUnBind */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0x6 ),	/* 6 */
/* 188 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 190 */	NdrFcShort( 0x18 ),	/* 24 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 196 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 210 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 212 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 214 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 216 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 220 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszAptID */

/* 222 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 224 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 226 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwActivityCnt */

/* 228 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 230 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 236 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadAssignApartment */

/* 240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0x7 ),	/* 7 */
/* 248 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 250 */	NdrFcShort( 0x10 ),	/* 16 */
/* 252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 254 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 256 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 264 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 266 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 268 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 270 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 272 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 274 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 276 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 278 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 280 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszAptID */

/* 282 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 284 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 286 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 288 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 290 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadUnassignApartment */

/* 294 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 296 */	NdrFcLong( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 302 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 304 */	NdrFcShort( 0x10 ),	/* 16 */
/* 306 */	NdrFcShort( 0x8 ),	/* 8 */
/* 308 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 310 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 318 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 320 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 322 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 324 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 326 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 328 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszAptID */

/* 330 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 332 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 334 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 338 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadWorkEnque */

/* 342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x9 ),	/* 9 */
/* 350 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 352 */	NdrFcShort( 0x18 ),	/* 24 */
/* 354 */	NdrFcShort( 0x8 ),	/* 8 */
/* 356 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 358 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 366 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 368 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 370 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 372 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 376 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 378 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 380 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 382 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter MsgWorkID */

/* 384 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 386 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 388 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwQueueLen */

/* 390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 392 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 396 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 398 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadWorkPrivate */

/* 402 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 404 */	NdrFcLong( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0xa ),	/* 10 */
/* 410 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 412 */	NdrFcShort( 0x10 ),	/* 16 */
/* 414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 416 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 418 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 422 */	NdrFcShort( 0x0 ),	/* 0 */
/* 424 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 426 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 428 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 430 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 432 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 434 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 436 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 438 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 440 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 442 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter MsgWorkID */

/* 444 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 446 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 448 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 450 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 452 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadWorkPublic */

/* 456 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0xb ),	/* 11 */
/* 464 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 466 */	NdrFcShort( 0x18 ),	/* 24 */
/* 468 */	NdrFcShort( 0x8 ),	/* 8 */
/* 470 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 472 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 480 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 482 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 484 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 486 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 488 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 490 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 492 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 494 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 496 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter MsgWorkID */

/* 498 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 500 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 502 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwQueueLen */

/* 504 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 506 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 510 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 512 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadWorkRedirect */

/* 516 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 518 */	NdrFcLong( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0xc ),	/* 12 */
/* 524 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 526 */	NdrFcShort( 0x18 ),	/* 24 */
/* 528 */	NdrFcShort( 0x8 ),	/* 8 */
/* 530 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 532 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 540 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 542 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 544 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 546 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 550 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 552 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 554 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 556 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter MsgWorkID */

/* 558 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 560 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 562 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwQueueLen */

/* 564 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 566 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszThreadNum */

/* 570 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 572 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 574 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 578 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnThreadWorkReject */

/* 582 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0xd ),	/* 13 */
/* 590 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 592 */	NdrFcShort( 0x18 ),	/* 24 */
/* 594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 596 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 598 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 606 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 608 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 610 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 612 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 614 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 616 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszThreadID */

/* 618 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 620 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 622 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter MsgWorkID */

/* 624 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 626 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 628 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwQueueLen */

/* 630 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 632 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 634 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 636 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 638 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAppActivation */

/* 642 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 644 */	NdrFcLong( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0xe ),	/* 14 */
/* 650 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 652 */	NdrFcShort( 0x10 ),	/* 16 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 658 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 666 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 668 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 670 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 672 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 674 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 676 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 678 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 680 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAppShutdown */

/* 684 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 686 */	NdrFcLong( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0xf ),	/* 15 */
/* 692 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 694 */	NdrFcShort( 0x10 ),	/* 16 */
/* 696 */	NdrFcShort( 0x8 ),	/* 8 */
/* 698 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 700 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 710 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 712 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 714 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 716 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 718 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 720 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 722 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 724 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjectCreate */

/* 726 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x10 ),	/* 16 */
/* 734 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 736 */	NdrFcShort( 0x10 ),	/* 16 */
/* 738 */	NdrFcShort( 0x8 ),	/* 8 */
/* 740 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 742 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 750 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 752 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 754 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 756 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 758 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 760 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 762 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 764 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 766 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 768 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 770 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 772 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszTSID */

/* 774 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 776 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 778 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 780 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 782 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 784 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 786 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 788 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 790 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 792 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 794 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjectDestroy */

/* 798 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 800 */	NdrFcLong( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x11 ),	/* 17 */
/* 806 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 808 */	NdrFcShort( 0x10 ),	/* 16 */
/* 810 */	NdrFcShort( 0x8 ),	/* 8 */
/* 812 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 814 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 816 */	NdrFcShort( 0x0 ),	/* 0 */
/* 818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 820 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 822 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 824 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 826 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 828 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 830 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 832 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 834 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 836 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 838 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 840 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 842 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 844 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnTransactionStart */

/* 846 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 848 */	NdrFcLong( 0x0 ),	/* 0 */
/* 852 */	NdrFcShort( 0x12 ),	/* 18 */
/* 854 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 856 */	NdrFcShort( 0x18 ),	/* 24 */
/* 858 */	NdrFcShort( 0x8 ),	/* 8 */
/* 860 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 862 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 870 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 872 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 874 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 876 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 878 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 880 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTxID */

/* 882 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 884 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 886 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTSID */

/* 888 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 890 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 892 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bRoot */

/* 894 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 896 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 900 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 902 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnTransactionPrepared */

/* 906 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 908 */	NdrFcLong( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x13 ),	/* 19 */
/* 914 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 916 */	NdrFcShort( 0x18 ),	/* 24 */
/* 918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 920 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 922 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 928 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 930 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 932 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 934 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 936 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 938 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 940 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTxID */

/* 942 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 944 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 946 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bVoteYes */

/* 948 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 950 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 952 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 954 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 956 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 958 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnTransactionAborted */

/* 960 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 962 */	NdrFcLong( 0x0 ),	/* 0 */
/* 966 */	NdrFcShort( 0x14 ),	/* 20 */
/* 968 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 970 */	NdrFcShort( 0x10 ),	/* 16 */
/* 972 */	NdrFcShort( 0x8 ),	/* 8 */
/* 974 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 976 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 980 */	NdrFcShort( 0x0 ),	/* 0 */
/* 982 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 984 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 986 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 988 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 990 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 992 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 994 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTxID */

/* 996 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 998 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1000 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1002 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1004 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnTransactionCommit */

/* 1008 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0x15 ),	/* 21 */
/* 1016 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1018 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1020 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1022 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1024 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1032 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1034 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1036 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1038 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1040 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1042 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTxID */

/* 1044 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1046 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1048 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1050 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1052 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1054 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnMethodCall */

/* 1056 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1058 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1062 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1064 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1066 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1068 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1070 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1072 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1078 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1080 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1082 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1084 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1086 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1088 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1090 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1092 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1094 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1096 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClassID */

/* 1098 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1100 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1102 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidInterfaceID */

/* 1104 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1106 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1108 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszMethod */

/* 1110 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1112 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1114 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1116 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1118 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1120 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnMethodReturn */

/* 1122 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1124 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1128 */	NdrFcShort( 0x17 ),	/* 23 */
/* 1130 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 1132 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1136 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 1138 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1144 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1146 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1148 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1150 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1152 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1154 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1156 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1158 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1160 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1162 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClassID */

/* 1164 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1166 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1168 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidInterfaceID */

/* 1170 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1172 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1174 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszMethod */

/* 1176 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1178 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1180 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hr */

/* 1182 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1184 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszCallTime */

/* 1188 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1190 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1192 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1194 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1196 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnMethodException */

/* 1200 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1206 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1208 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1210 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1214 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1216 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1222 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1224 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1226 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1228 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1230 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1234 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1236 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1238 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1240 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClassID */

/* 1242 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1244 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1246 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidInterfaceID */

/* 1248 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1250 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1252 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszMethod */

/* 1254 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1256 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1258 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1262 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjectActivate */

/* 1266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1272 */	NdrFcShort( 0x19 ),	/* 25 */
/* 1274 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1276 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1280 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1282 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1288 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1290 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1292 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1294 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1296 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1298 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1300 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 1302 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1304 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1306 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1308 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1310 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1312 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1314 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1316 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1318 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjectDeactivate */

/* 1320 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1322 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0x1a ),	/* 26 */
/* 1328 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1330 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1332 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1334 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 1336 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1342 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1344 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1346 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1348 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1350 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1352 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1354 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 1356 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1358 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1360 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1362 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1364 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1366 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1370 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnDisableCommit */

/* 1374 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x1b ),	/* 27 */
/* 1382 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1384 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1386 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1388 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1390 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1394 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1396 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1398 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1400 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1402 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1404 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1406 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1408 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 1410 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1412 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1414 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1418 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnEnableCommit */

/* 1422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1428 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1430 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1432 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1436 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1438 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1444 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1446 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1448 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1450 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1452 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1454 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1456 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 1458 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1460 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1462 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1464 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1466 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1468 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSetComplete */

/* 1470 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1472 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1476 */	NdrFcShort( 0x1d ),	/* 29 */
/* 1478 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1480 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1482 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1484 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1486 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1492 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1494 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1496 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1498 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1500 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1502 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1504 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 1506 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1508 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1510 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1512 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1514 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1516 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSetAbort */

/* 1518 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1520 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1524 */	NdrFcShort( 0x1e ),	/* 30 */
/* 1526 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1528 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1530 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1532 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 1534 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1540 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1542 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1544 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1546 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1548 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1550 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1552 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszContextID */

/* 1554 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1556 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1558 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1560 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1562 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnResourceCreate */

/* 1566 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1568 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1572 */	NdrFcShort( 0x1f ),	/* 31 */
/* 1574 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1576 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1578 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1580 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1582 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1588 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1590 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1592 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1594 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1596 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1598 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1600 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1602 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1604 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1606 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszType */

/* 1608 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1610 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1612 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszResId */

/* 1614 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1616 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1618 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bEnlisted */

/* 1620 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1622 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1626 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1628 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1630 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnResourceAllocate */

/* 1632 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1634 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1638 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1640 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 1642 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1644 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1646 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 1648 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1654 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1656 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1658 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1660 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1662 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1664 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1666 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1668 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1670 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1672 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszType */

/* 1674 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1676 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1678 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszResId */

/* 1680 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1682 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1684 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bEnlisted */

/* 1686 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1688 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszNumRated */

/* 1692 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1694 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1696 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszRating */

/* 1698 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1700 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1702 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1704 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1706 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnResourceRecycle */

/* 1710 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1716 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1718 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1720 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1722 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1724 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1726 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1732 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1734 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1736 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1738 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1740 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1742 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1744 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1746 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1748 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1750 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszType */

/* 1752 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1754 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1756 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszResId */

/* 1758 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1760 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1762 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1764 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1766 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1768 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnResourceDestroy */

/* 1770 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1772 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1776 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1778 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1780 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1782 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1784 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1786 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1792 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1794 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1796 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1798 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1800 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1802 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1804 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1806 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1808 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1810 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hResult */

/* 1812 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1814 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1816 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszType */

/* 1818 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1820 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1822 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszResId */

/* 1824 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1826 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1828 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 1830 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1832 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnResourceTrack */

/* 1836 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1838 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1842 */	NdrFcShort( 0x23 ),	/* 35 */
/* 1844 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1846 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1850 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 1852 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1858 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1860 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1862 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1864 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1866 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1868 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1870 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1872 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1874 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1876 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszType */

/* 1878 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1880 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1882 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszResId */

/* 1884 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1886 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1888 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bEnlisted */

/* 1890 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1892 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1896 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1898 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1900 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAuthenticate */

/* 1902 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1904 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1908 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1910 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 1912 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1914 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1916 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xa,		/* 10 */
/* 1918 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1924 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 1926 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1928 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1930 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 1932 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1934 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1936 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivity */

/* 1938 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1940 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1942 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 1944 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1946 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1948 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidIID */

/* 1950 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1952 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1954 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszMethod */

/* 1956 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1958 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1960 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszOriginalUser */

/* 1962 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1964 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1966 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszDirectUser */

/* 1968 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1970 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1972 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bCurrentUserInpersonatingInProc */

/* 1974 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1976 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1978 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1980 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1982 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAuthenticateFail */

/* 1986 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1988 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1994 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 1996 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2000 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xa,		/* 10 */
/* 2002 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2010 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2012 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2014 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2016 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2018 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2020 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivity */

/* 2022 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2024 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2026 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 2028 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2030 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2032 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidIID */

/* 2034 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2036 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2038 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszMethod */

/* 2040 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2042 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2044 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszOriginalUser */

/* 2046 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2048 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2050 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszDirectUser */

/* 2052 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2054 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2056 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bCurrentUserInpersonatingInProc */

/* 2058 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2060 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2064 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2066 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 2068 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolPutObject */

/* 2070 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2072 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2076 */	NdrFcShort( 0x26 ),	/* 38 */
/* 2078 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2080 */	NdrFcShort( 0x20 ),	/* 32 */
/* 2082 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2084 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2086 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2088 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2092 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2094 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2096 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2098 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2100 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2102 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2104 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2106 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2108 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2110 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter nReason */

/* 2112 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2114 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwAvailableObjs */

/* 2118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2120 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszObjectID */

/* 2124 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2126 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2128 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2132 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolGetObject */

/* 2136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2142 */	NdrFcShort( 0x27 ),	/* 39 */
/* 2144 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2146 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2150 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2152 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2160 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2162 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2164 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2166 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2168 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2170 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 2172 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2174 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2176 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2178 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2180 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2182 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwAvailableObjs */

/* 2184 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2186 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszObjectID */

/* 2190 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2192 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2194 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2196 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2198 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolRecycleToTx */

/* 2202 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2204 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2208 */	NdrFcShort( 0x28 ),	/* 40 */
/* 2210 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2212 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2214 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2216 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2218 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2224 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2226 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2228 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2230 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2232 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2234 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2236 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 2238 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2240 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2242 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2244 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2246 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2248 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTx */

/* 2250 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2252 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2254 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 2256 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2258 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2260 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2262 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2264 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolGetFromTx */

/* 2268 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2270 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2274 */	NdrFcShort( 0x29 ),	/* 41 */
/* 2276 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2278 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2282 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2284 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2290 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2292 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2294 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2296 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2298 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2300 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2302 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 2304 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2306 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2308 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2310 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2312 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2314 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTx */

/* 2316 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2318 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2320 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 2322 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2324 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2326 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2328 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2330 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2332 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolCreateObject */

/* 2334 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2336 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0x2a ),	/* 42 */
/* 2342 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2344 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2348 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2350 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2356 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2358 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2360 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2362 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2364 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2366 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2368 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2370 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2372 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2374 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwAvailableObjs */

/* 2376 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2378 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszObjectID */

/* 2382 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2384 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2386 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2388 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2390 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2392 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolDestroyObject */

/* 2394 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2396 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2400 */	NdrFcShort( 0x2b ),	/* 43 */
/* 2402 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2404 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2406 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2408 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2410 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2416 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2418 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2420 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2422 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2424 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2426 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2428 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2430 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2432 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2434 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwAvailableObjs */

/* 2436 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2438 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2440 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pszObjectID */

/* 2442 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2444 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2446 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2448 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2450 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2452 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolCreateDecision */

/* 2454 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2456 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2460 */	NdrFcShort( 0x2c ),	/* 44 */
/* 2462 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 2464 */	NdrFcShort( 0x38 ),	/* 56 */
/* 2466 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2468 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 2470 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2476 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2478 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2480 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2482 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2484 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2486 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2488 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwThreadsWaiting */

/* 2490 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2492 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2494 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwAvailableObjs */

/* 2496 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2498 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2500 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwCreatedObjs */

/* 2502 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2504 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2506 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwMin */

/* 2508 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2510 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2512 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwMax */

/* 2514 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2516 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2518 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2520 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2522 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolTimeout */

/* 2526 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2528 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2532 */	NdrFcShort( 0x2d ),	/* 45 */
/* 2534 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2536 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2540 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2542 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2548 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2550 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2552 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2554 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2556 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2558 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2560 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2562 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2564 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2566 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 2568 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2570 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2572 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwTimeout */

/* 2574 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2576 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2578 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2580 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2582 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2584 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjPoolCreatePool */

/* 2586 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2588 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2592 */	NdrFcShort( 0x2e ),	/* 46 */
/* 2594 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2596 */	NdrFcShort( 0x28 ),	/* 40 */
/* 2598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2600 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2602 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2608 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2610 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2612 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2614 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2616 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2618 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2620 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2622 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2624 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2626 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwMin */

/* 2628 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2630 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2632 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwMax */

/* 2634 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2636 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2638 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwTimeout */

/* 2640 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2642 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2644 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2646 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2648 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2650 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnObjectConstruct */

/* 2652 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2654 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2658 */	NdrFcShort( 0x2f ),	/* 47 */
/* 2660 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2662 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2664 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2666 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2668 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2670 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2674 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2676 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2678 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2680 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2682 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2684 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2686 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClassID */

/* 2688 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2690 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2692 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszConstructString */

/* 2694 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2696 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2698 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 2700 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2702 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2704 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2706 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2708 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2710 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityCreate */

/* 2712 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2714 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2718 */	NdrFcShort( 0x30 ),	/* 48 */
/* 2720 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2722 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2724 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2726 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2728 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2734 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2736 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2738 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2740 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2742 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2744 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2746 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 2748 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2750 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2752 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2754 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2756 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2758 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityDestroy */

/* 2760 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2762 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2766 */	NdrFcShort( 0x31 ),	/* 49 */
/* 2768 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2770 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2772 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2774 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2776 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2782 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2784 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2786 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2788 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2790 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2792 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2794 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 2796 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2798 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2800 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2802 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2804 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2806 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityEnter */

/* 2808 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2810 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2814 */	NdrFcShort( 0x32 ),	/* 50 */
/* 2816 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2818 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2822 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2824 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2830 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2832 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2834 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2836 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2838 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2840 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2842 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidCurrentApp */

/* 2844 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2846 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2848 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidEnteredApp */

/* 2850 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2852 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2854 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszW2KThreadID */

/* 2856 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2858 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2860 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 2862 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2864 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2866 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityTimeout */

/* 2868 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2870 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2874 */	NdrFcShort( 0x33 ),	/* 51 */
/* 2876 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 2878 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2880 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2882 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 2884 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2890 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2892 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2894 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2896 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2898 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2900 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2902 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidCurrentApp */

/* 2904 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2906 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2908 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidEnteredApp */

/* 2910 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2912 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2914 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszW2KThreadID */

/* 2916 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2918 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2920 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwTimeout */

/* 2922 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2924 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2926 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2928 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2930 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2932 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityReenter */

/* 2934 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2936 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2940 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2942 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 2944 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2946 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2948 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2950 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2952 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2956 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 2958 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2960 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2962 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 2964 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2966 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2968 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidCurrentApp */

/* 2970 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2972 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2974 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszW2KThreadID */

/* 2976 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 2978 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2980 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwCallDepth */

/* 2982 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2984 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2986 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2988 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2990 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2992 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityLeave */

/* 2994 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2996 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3000 */	NdrFcShort( 0x35 ),	/* 53 */
/* 3002 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3004 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3006 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3008 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 3010 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3016 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3018 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3020 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3022 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3024 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3026 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3028 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidCurrentApp */

/* 3030 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3032 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3034 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidLeftApp */

/* 3036 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3038 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3040 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3042 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3044 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3046 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActivityLeaveSame */

/* 3048 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3050 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3054 */	NdrFcShort( 0x36 ),	/* 54 */
/* 3056 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3058 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3060 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3062 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 3064 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3066 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3070 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3072 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3074 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3076 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3078 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3080 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3082 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidCurrentApp */

/* 3084 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3086 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3088 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwCallDepth */

/* 3090 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3092 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3094 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3096 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3098 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnIISRequestInfo */

/* 3102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3108 */	NdrFcShort( 0x37 ),	/* 55 */
/* 3110 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3112 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3116 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 3118 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3124 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3126 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3128 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3130 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3132 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3134 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3136 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 3138 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3140 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3142 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClientIP */

/* 3144 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3146 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3148 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszServerIP */

/* 3150 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3152 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3154 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszURL */

/* 3156 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3158 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3160 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3164 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCRecord */

/* 3168 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3174 */	NdrFcShort( 0x38 ),	/* 56 */
/* 3176 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 3178 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3180 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3182 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 3184 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3190 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3192 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3194 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3196 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3198 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3200 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3202 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 3204 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3206 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3208 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszQueueName */

/* 3210 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3212 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3214 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidMsgID */

/* 3216 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3218 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3220 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidWorkFlowID */

/* 3222 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3224 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3226 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hr */

/* 3228 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3230 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3236 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCQueueOpen */

/* 3240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3246 */	NdrFcShort( 0x39 ),	/* 57 */
/* 3248 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3250 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3254 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3256 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3262 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3264 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3266 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3268 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3270 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3272 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3274 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszQueueName */

/* 3276 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3278 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3280 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszQueueID */

/* 3282 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3284 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3286 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hr */

/* 3288 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3290 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3294 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3296 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCReceive */

/* 3300 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3302 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3306 */	NdrFcShort( 0x3a ),	/* 58 */
/* 3308 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3310 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3312 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3314 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 3316 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3322 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3324 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3326 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3328 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3330 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3334 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszQueueID */

/* 3336 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3338 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3340 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidMsgID */

/* 3342 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3344 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3346 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidWorkFlowID */

/* 3348 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3350 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3352 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hr */

/* 3354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3356 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3362 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCReceiveFail */

/* 3366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3372 */	NdrFcShort( 0x3b ),	/* 59 */
/* 3374 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3376 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3380 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 3382 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3394 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3396 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3398 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3400 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszQueueID */

/* 3402 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3404 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3406 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hr */

/* 3408 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3410 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3414 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3416 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3418 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCMoveToReTryQueue */

/* 3420 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3422 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3426 */	NdrFcShort( 0x3c ),	/* 60 */
/* 3428 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3430 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3432 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3434 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3436 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3442 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3444 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3446 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3448 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3450 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3452 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3454 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidMsgID */

/* 3456 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3458 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3460 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidWorkFlowID */

/* 3462 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3464 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3466 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter RetryIndex */

/* 3468 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3470 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3476 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCMoveToDeadQueue */

/* 3480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3486 */	NdrFcShort( 0x3d ),	/* 61 */
/* 3488 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3490 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3492 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3494 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 3496 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3502 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3504 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3506 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3508 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3510 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3512 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3514 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidMsgID */

/* 3516 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3518 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3520 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidWorkFlowID */

/* 3522 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3524 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3526 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3528 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3530 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnQCPlayback */

/* 3534 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3536 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3540 */	NdrFcShort( 0x3e ),	/* 62 */
/* 3542 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3544 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3546 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3548 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 3550 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3556 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3558 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3560 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3562 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3564 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3566 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3568 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjectID */

/* 3570 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3572 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3574 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidMsgID */

/* 3576 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3578 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3580 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidWorkFlowID */

/* 3582 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3584 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3586 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter hr */

/* 3588 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3590 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3596 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnExceptionUser */

/* 3600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3606 */	NdrFcShort( 0x3f ),	/* 63 */
/* 3608 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3610 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3614 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3616 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3624 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3628 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3630 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3632 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3634 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszCode */

/* 3636 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3638 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3640 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszAddress */

/* 3642 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3644 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3646 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszStackTrace */

/* 3648 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3650 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3652 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3654 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3656 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMRecoveryStart */

/* 3660 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3662 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3666 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3668 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3670 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3672 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3674 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3676 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3682 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3684 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3686 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3688 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3690 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3692 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3694 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3696 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3698 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMRecoveryDone */

/* 3702 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3708 */	NdrFcShort( 0x41 ),	/* 65 */
/* 3710 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3712 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3714 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3716 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3718 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3724 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3726 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3728 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3730 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3732 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3734 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3736 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3740 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMCheckpoint */

/* 3744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3750 */	NdrFcShort( 0x42 ),	/* 66 */
/* 3752 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3754 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3758 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3760 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3768 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3770 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3772 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3774 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3776 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3778 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3780 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3782 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMBegin */

/* 3786 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3788 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3792 */	NdrFcShort( 0x43 ),	/* 67 */
/* 3794 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 3796 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3798 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3800 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 3802 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3808 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3810 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3812 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3814 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3816 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3818 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3820 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 3822 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3824 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3826 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidActivityID */

/* 3828 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3830 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3832 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidTxUOWID */

/* 3834 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3836 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3838 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszProgIdCompensator */

/* 3840 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3842 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3844 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszDescription */

/* 3846 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3848 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3850 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3852 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3854 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3856 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMPrepare */

/* 3858 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3860 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3864 */	NdrFcShort( 0x44 ),	/* 68 */
/* 3866 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3868 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3870 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3872 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3874 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3880 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3882 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3884 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3886 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3888 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3890 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3892 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 3894 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3896 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3898 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3900 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3902 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMCommit */

/* 3906 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3908 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3912 */	NdrFcShort( 0x45 ),	/* 69 */
/* 3914 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3916 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3920 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3922 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3928 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3930 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3932 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3934 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3936 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3938 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3940 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 3942 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3944 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3946 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3948 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3950 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3952 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMAbort */

/* 3954 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3956 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3960 */	NdrFcShort( 0x46 ),	/* 70 */
/* 3962 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3964 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3968 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 3970 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3976 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 3978 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3980 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3982 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 3984 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3986 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3988 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 3990 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3992 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3994 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 3996 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3998 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4000 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMIndoubt */

/* 4002 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4004 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4008 */	NdrFcShort( 0x47 ),	/* 71 */
/* 4010 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4012 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4014 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4016 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4018 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4024 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4026 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4028 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4030 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4032 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4034 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4036 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4038 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4040 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4042 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4044 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4046 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4048 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMDone */

/* 4050 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4052 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4056 */	NdrFcShort( 0x48 ),	/* 72 */
/* 4058 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4060 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4062 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4064 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4066 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4072 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4074 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4076 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4078 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4080 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4082 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4084 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4086 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4088 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4090 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4092 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4094 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4096 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMRelease */

/* 4098 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4100 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4104 */	NdrFcShort( 0x49 ),	/* 73 */
/* 4106 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4108 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4110 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4112 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4114 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4120 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4122 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4124 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4126 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4128 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4130 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4132 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4134 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4136 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4138 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4140 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4142 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMAnalyze */

/* 4146 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4148 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4152 */	NdrFcShort( 0x4a ),	/* 74 */
/* 4154 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 4156 */	NdrFcShort( 0x18 ),	/* 24 */
/* 4158 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4160 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 4162 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4168 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4170 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4172 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4174 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4176 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4178 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4180 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4182 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4184 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4186 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszCrmRecordType */

/* 4188 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4190 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4192 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter dwRecordSize */

/* 4194 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4196 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4200 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4202 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 4204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMWrite */

/* 4206 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4208 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4212 */	NdrFcShort( 0x4b ),	/* 75 */
/* 4214 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 4216 */	NdrFcShort( 0x20 ),	/* 32 */
/* 4218 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4220 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 4222 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4228 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4230 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4232 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4234 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4236 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4238 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4240 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4242 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4244 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4246 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bVariants */

/* 4248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4250 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4252 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwRecordSize */

/* 4254 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4256 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4262 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 4264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMForget */

/* 4266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4272 */	NdrFcShort( 0x4c ),	/* 76 */
/* 4274 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4276 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4280 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4282 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4288 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4290 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4292 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4294 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4296 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4298 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4300 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4302 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4304 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4306 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4310 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMForce */

/* 4314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4320 */	NdrFcShort( 0x4d ),	/* 77 */
/* 4322 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4324 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4328 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4330 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4338 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4342 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4344 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4346 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4348 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4350 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4352 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4354 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4356 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4358 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4360 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCRMDeliver */

/* 4362 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4364 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4368 */	NdrFcShort( 0x4e ),	/* 78 */
/* 4370 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 4372 */	NdrFcShort( 0x20 ),	/* 32 */
/* 4374 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4376 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 4378 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4384 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4386 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4388 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4390 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszGuidApp */

/* 4392 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4394 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4396 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszGuidClerkCLSID */

/* 4398 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4400 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4402 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter bVariants */

/* 4404 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4406 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwRecordSize */

/* 4410 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4412 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4414 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4418 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 4420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TargetUp */

/* 4422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4428 */	NdrFcShort( 0x4f ),	/* 79 */
/* 4430 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4432 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4436 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4438 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4444 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4446 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4448 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4450 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszServerName */

/* 4452 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4454 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4456 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClsidEng */

/* 4458 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4460 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4462 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4464 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4466 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4468 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TargetDown */

/* 4470 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4472 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4476 */	NdrFcShort( 0x50 ),	/* 80 */
/* 4478 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4480 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4482 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4484 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4486 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4492 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4494 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4496 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4498 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszServerName */

/* 4500 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4502 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4504 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszClsidEng */

/* 4506 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4508 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4510 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4512 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4514 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4516 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EngineDefined */

/* 4518 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4520 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4524 */	NdrFcShort( 0x51 ),	/* 81 */
/* 4526 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 4528 */	NdrFcShort( 0x10 ),	/* 16 */
/* 4530 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4532 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 4534 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4538 */	NdrFcShort( 0x20 ),	/* 32 */
/* 4540 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter perfCount */

/* 4542 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4544 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4546 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter pszPropName */

/* 4548 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4550 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4552 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter varPropValue */

/* 4554 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4556 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4558 */	NdrFcShort( 0x408 ),	/* Type Offset=1032 */

	/* Parameter pszClsidEng */

/* 4560 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4562 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4564 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4566 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4568 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4570 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Init */

/* 4572 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4574 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4578 */	NdrFcShort( 0x52 ),	/* 82 */
/* 4580 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4584 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4586 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4588 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4594 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sDSN */

/* 4596 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4598 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4600 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter sUser */

/* 4602 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4604 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4606 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter sPw */

/* 4608 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 4610 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4612 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 4614 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4616 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x0,	/* FC_RP */
/*  8 */	NdrFcShort( 0x400 ),	/* Offset= 1024 (1032) */
/* 10 */	
			0x12, 0x0,	/* FC_UP */
/* 12 */	NdrFcShort( 0x3e8 ),	/* Offset= 1000 (1012) */
/* 14 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 16 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 18 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 20 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 22 */	NdrFcShort( 0x2 ),	/* Offset= 2 (24) */
/* 24 */	NdrFcShort( 0x10 ),	/* 16 */
/* 26 */	NdrFcShort( 0x2f ),	/* 47 */
/* 28 */	NdrFcLong( 0x14 ),	/* 20 */
/* 32 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 34 */	NdrFcLong( 0x3 ),	/* 3 */
/* 38 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 40 */	NdrFcLong( 0x11 ),	/* 17 */
/* 44 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 46 */	NdrFcLong( 0x2 ),	/* 2 */
/* 50 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 52 */	NdrFcLong( 0x4 ),	/* 4 */
/* 56 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 58 */	NdrFcLong( 0x5 ),	/* 5 */
/* 62 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 64 */	NdrFcLong( 0xb ),	/* 11 */
/* 68 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 70 */	NdrFcLong( 0xa ),	/* 10 */
/* 74 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 76 */	NdrFcLong( 0x6 ),	/* 6 */
/* 80 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (312) */
/* 82 */	NdrFcLong( 0x7 ),	/* 7 */
/* 86 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 88 */	NdrFcLong( 0x8 ),	/* 8 */
/* 92 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (318) */
/* 94 */	NdrFcLong( 0xd ),	/* 13 */
/* 98 */	NdrFcShort( 0xf6 ),	/* Offset= 246 (344) */
/* 100 */	NdrFcLong( 0x9 ),	/* 9 */
/* 104 */	NdrFcShort( 0x102 ),	/* Offset= 258 (362) */
/* 106 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 110 */	NdrFcShort( 0x10e ),	/* Offset= 270 (380) */
/* 112 */	NdrFcLong( 0x24 ),	/* 36 */
/* 116 */	NdrFcShort( 0x336 ),	/* Offset= 822 (938) */
/* 118 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 122 */	NdrFcShort( 0x330 ),	/* Offset= 816 (938) */
/* 124 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 128 */	NdrFcShort( 0x32e ),	/* Offset= 814 (942) */
/* 130 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 134 */	NdrFcShort( 0x32c ),	/* Offset= 812 (946) */
/* 136 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 140 */	NdrFcShort( 0x32a ),	/* Offset= 810 (950) */
/* 142 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 146 */	NdrFcShort( 0x328 ),	/* Offset= 808 (954) */
/* 148 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 152 */	NdrFcShort( 0x326 ),	/* Offset= 806 (958) */
/* 154 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 158 */	NdrFcShort( 0x324 ),	/* Offset= 804 (962) */
/* 160 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 164 */	NdrFcShort( 0x30e ),	/* Offset= 782 (946) */
/* 166 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 170 */	NdrFcShort( 0x30c ),	/* Offset= 780 (950) */
/* 172 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 176 */	NdrFcShort( 0x316 ),	/* Offset= 790 (966) */
/* 178 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 182 */	NdrFcShort( 0x30c ),	/* Offset= 780 (962) */
/* 184 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 188 */	NdrFcShort( 0x30e ),	/* Offset= 782 (970) */
/* 190 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 194 */	NdrFcShort( 0x30c ),	/* Offset= 780 (974) */
/* 196 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 200 */	NdrFcShort( 0x30a ),	/* Offset= 778 (978) */
/* 202 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 206 */	NdrFcShort( 0x308 ),	/* Offset= 776 (982) */
/* 208 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 212 */	NdrFcShort( 0x306 ),	/* Offset= 774 (986) */
/* 214 */	NdrFcLong( 0x10 ),	/* 16 */
/* 218 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 220 */	NdrFcLong( 0x12 ),	/* 18 */
/* 224 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 226 */	NdrFcLong( 0x13 ),	/* 19 */
/* 230 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 232 */	NdrFcLong( 0x15 ),	/* 21 */
/* 236 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 238 */	NdrFcLong( 0x16 ),	/* 22 */
/* 242 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 244 */	NdrFcLong( 0x17 ),	/* 23 */
/* 248 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 250 */	NdrFcLong( 0xe ),	/* 14 */
/* 254 */	NdrFcShort( 0x2e4 ),	/* Offset= 740 (994) */
/* 256 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 260 */	NdrFcShort( 0x2e8 ),	/* Offset= 744 (1004) */
/* 262 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 266 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (1008) */
/* 268 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 272 */	NdrFcShort( 0x2a2 ),	/* Offset= 674 (946) */
/* 274 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 278 */	NdrFcShort( 0x2a0 ),	/* Offset= 672 (950) */
/* 280 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 284 */	NdrFcShort( 0x29e ),	/* Offset= 670 (954) */
/* 286 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 290 */	NdrFcShort( 0x294 ),	/* Offset= 660 (950) */
/* 292 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 296 */	NdrFcShort( 0x28e ),	/* Offset= 654 (950) */
/* 298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 302 */	NdrFcShort( 0x0 ),	/* Offset= 0 (302) */
/* 304 */	NdrFcLong( 0x1 ),	/* 1 */
/* 308 */	NdrFcShort( 0x0 ),	/* Offset= 0 (308) */
/* 310 */	NdrFcShort( 0xffff ),	/* Offset= -1 (309) */
/* 312 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 314 */	NdrFcShort( 0x8 ),	/* 8 */
/* 316 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 318 */	
			0x12, 0x0,	/* FC_UP */
/* 320 */	NdrFcShort( 0xe ),	/* Offset= 14 (334) */
/* 322 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 324 */	NdrFcShort( 0x2 ),	/* 2 */
/* 326 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 328 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 330 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 332 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 334 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (322) */
/* 340 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 342 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 344 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 346 */	NdrFcLong( 0x0 ),	/* 0 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 354 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 356 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 358 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 360 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 362 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 364 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 374 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 376 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 378 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 380 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 382 */	NdrFcShort( 0x2 ),	/* Offset= 2 (384) */
/* 384 */	
			0x12, 0x0,	/* FC_UP */
/* 386 */	NdrFcShort( 0x216 ),	/* Offset= 534 (920) */
/* 388 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 390 */	NdrFcShort( 0x18 ),	/* 24 */
/* 392 */	NdrFcShort( 0xa ),	/* 10 */
/* 394 */	NdrFcLong( 0x8 ),	/* 8 */
/* 398 */	NdrFcShort( 0x5a ),	/* Offset= 90 (488) */
/* 400 */	NdrFcLong( 0xd ),	/* 13 */
/* 404 */	NdrFcShort( 0x7e ),	/* Offset= 126 (530) */
/* 406 */	NdrFcLong( 0x9 ),	/* 9 */
/* 410 */	NdrFcShort( 0x9e ),	/* Offset= 158 (568) */
/* 412 */	NdrFcLong( 0xc ),	/* 12 */
/* 416 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (616) */
/* 418 */	NdrFcLong( 0x24 ),	/* 36 */
/* 422 */	NdrFcShort( 0x124 ),	/* Offset= 292 (714) */
/* 424 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 428 */	NdrFcShort( 0x140 ),	/* Offset= 320 (748) */
/* 430 */	NdrFcLong( 0x10 ),	/* 16 */
/* 434 */	NdrFcShort( 0x15a ),	/* Offset= 346 (780) */
/* 436 */	NdrFcLong( 0x2 ),	/* 2 */
/* 440 */	NdrFcShort( 0x174 ),	/* Offset= 372 (812) */
/* 442 */	NdrFcLong( 0x3 ),	/* 3 */
/* 446 */	NdrFcShort( 0x18e ),	/* Offset= 398 (844) */
/* 448 */	NdrFcLong( 0x14 ),	/* 20 */
/* 452 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (876) */
/* 454 */	NdrFcShort( 0xffff ),	/* Offset= -1 (453) */
/* 456 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 458 */	NdrFcShort( 0x4 ),	/* 4 */
/* 460 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 466 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 468 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 470 */	NdrFcShort( 0x4 ),	/* 4 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x1 ),	/* 1 */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	0x12, 0x0,	/* FC_UP */
/* 482 */	NdrFcShort( 0xff6c ),	/* Offset= -148 (334) */
/* 484 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 486 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 488 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 490 */	NdrFcShort( 0x8 ),	/* 8 */
/* 492 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 494 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 496 */	NdrFcShort( 0x4 ),	/* 4 */
/* 498 */	NdrFcShort( 0x4 ),	/* 4 */
/* 500 */	0x11, 0x0,	/* FC_RP */
/* 502 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (456) */
/* 504 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 506 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 508 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 512 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 518 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 522 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 524 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 526 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (344) */
/* 528 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 530 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 532 */	NdrFcShort( 0x8 ),	/* 8 */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x6 ),	/* Offset= 6 (542) */
/* 538 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 540 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 542 */	
			0x11, 0x0,	/* FC_RP */
/* 544 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (508) */
/* 546 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 556 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 560 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 562 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 564 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (362) */
/* 566 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 568 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 570 */	NdrFcShort( 0x8 ),	/* 8 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x6 ),	/* Offset= 6 (580) */
/* 576 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 578 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 580 */	
			0x11, 0x0,	/* FC_RP */
/* 582 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (546) */
/* 584 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 586 */	NdrFcShort( 0x4 ),	/* 4 */
/* 588 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 594 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 596 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 598 */	NdrFcShort( 0x4 ),	/* 4 */
/* 600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x1 ),	/* 1 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	0x12, 0x0,	/* FC_UP */
/* 610 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1012) */
/* 612 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 614 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 616 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x6 ),	/* Offset= 6 (628) */
/* 624 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 626 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 628 */	
			0x11, 0x0,	/* FC_RP */
/* 630 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (584) */
/* 632 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 634 */	NdrFcLong( 0x2f ),	/* 47 */
/* 638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 644 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 646 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 648 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 650 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 652 */	NdrFcShort( 0x1 ),	/* 1 */
/* 654 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 656 */	NdrFcShort( 0x4 ),	/* 4 */
/* 658 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 660 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 662 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 664 */	NdrFcShort( 0x10 ),	/* 16 */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0xa ),	/* Offset= 10 (678) */
/* 670 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 672 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 674 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (632) */
/* 676 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 678 */	
			0x12, 0x0,	/* FC_UP */
/* 680 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (650) */
/* 682 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 684 */	NdrFcShort( 0x4 ),	/* 4 */
/* 686 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 692 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 694 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 696 */	NdrFcShort( 0x4 ),	/* 4 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	NdrFcShort( 0x1 ),	/* 1 */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	0x12, 0x0,	/* FC_UP */
/* 708 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (662) */
/* 710 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 712 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 714 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 716 */	NdrFcShort( 0x8 ),	/* 8 */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x6 ),	/* Offset= 6 (726) */
/* 722 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 724 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 726 */	
			0x11, 0x0,	/* FC_RP */
/* 728 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (682) */
/* 730 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 732 */	NdrFcShort( 0x8 ),	/* 8 */
/* 734 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 736 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 738 */	NdrFcShort( 0x10 ),	/* 16 */
/* 740 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 742 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 744 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (730) */
			0x5b,		/* FC_END */
/* 748 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 750 */	NdrFcShort( 0x18 ),	/* 24 */
/* 752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 754 */	NdrFcShort( 0xa ),	/* Offset= 10 (764) */
/* 756 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 758 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 760 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (736) */
/* 762 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 764 */	
			0x11, 0x0,	/* FC_RP */
/* 766 */	NdrFcShort( 0xfefe ),	/* Offset= -258 (508) */
/* 768 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 770 */	NdrFcShort( 0x1 ),	/* 1 */
/* 772 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 778 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 780 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 782 */	NdrFcShort( 0x8 ),	/* 8 */
/* 784 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 786 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 788 */	NdrFcShort( 0x4 ),	/* 4 */
/* 790 */	NdrFcShort( 0x4 ),	/* 4 */
/* 792 */	0x12, 0x0,	/* FC_UP */
/* 794 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (768) */
/* 796 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 798 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 800 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 802 */	NdrFcShort( 0x2 ),	/* 2 */
/* 804 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 810 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 812 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 816 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 818 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	NdrFcShort( 0x4 ),	/* 4 */
/* 824 */	0x12, 0x0,	/* FC_UP */
/* 826 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (800) */
/* 828 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 830 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 832 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 834 */	NdrFcShort( 0x4 ),	/* 4 */
/* 836 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 842 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 844 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 848 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 850 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 852 */	NdrFcShort( 0x4 ),	/* 4 */
/* 854 */	NdrFcShort( 0x4 ),	/* 4 */
/* 856 */	0x12, 0x0,	/* FC_UP */
/* 858 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (832) */
/* 860 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 862 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 864 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 868 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 872 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 874 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 876 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 880 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 882 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 884 */	NdrFcShort( 0x4 ),	/* 4 */
/* 886 */	NdrFcShort( 0x4 ),	/* 4 */
/* 888 */	0x12, 0x0,	/* FC_UP */
/* 890 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (864) */
/* 892 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 894 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 896 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 898 */	NdrFcShort( 0x8 ),	/* 8 */
/* 900 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 902 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 904 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 910 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 912 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 914 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 916 */	NdrFcShort( 0xffec ),	/* Offset= -20 (896) */
/* 918 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 920 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 922 */	NdrFcShort( 0x28 ),	/* 40 */
/* 924 */	NdrFcShort( 0xffec ),	/* Offset= -20 (904) */
/* 926 */	NdrFcShort( 0x0 ),	/* Offset= 0 (926) */
/* 928 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 930 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 932 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 934 */	NdrFcShort( 0xfdde ),	/* Offset= -546 (388) */
/* 936 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 938 */	
			0x12, 0x0,	/* FC_UP */
/* 940 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (662) */
/* 942 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 944 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 946 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 948 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 950 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 952 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 954 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 956 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 958 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 960 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 962 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 964 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 966 */	
			0x12, 0x0,	/* FC_UP */
/* 968 */	NdrFcShort( 0xfd70 ),	/* Offset= -656 (312) */
/* 970 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 972 */	NdrFcShort( 0xfd72 ),	/* Offset= -654 (318) */
/* 974 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 976 */	NdrFcShort( 0xfd88 ),	/* Offset= -632 (344) */
/* 978 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 980 */	NdrFcShort( 0xfd96 ),	/* Offset= -618 (362) */
/* 982 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 984 */	NdrFcShort( 0xfda4 ),	/* Offset= -604 (380) */
/* 986 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 988 */	NdrFcShort( 0x2 ),	/* Offset= 2 (990) */
/* 990 */	
			0x12, 0x0,	/* FC_UP */
/* 992 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1012) */
/* 994 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 996 */	NdrFcShort( 0x10 ),	/* 16 */
/* 998 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1000 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1002 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1004 */	
			0x12, 0x0,	/* FC_UP */
/* 1006 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (994) */
/* 1008 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1010 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1012 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1014 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1016 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1018 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1018) */
/* 1020 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1022 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1024 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1026 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1028 */	NdrFcShort( 0xfc0a ),	/* Offset= -1014 (14) */
/* 1030 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1032 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1034 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1036 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1040 */	NdrFcShort( 0xfbfa ),	/* Offset= -1030 (10) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IComSpyAudit, ver. 0.0,
   GUID={0xAFCF0622,0x1429,0x11D1,{0xB1,0xB8,0x00,0xAA,0x00,0xBA,0x32,0x58}} */

#pragma code_seg(".orpc")
static const unsigned short IComSpyAudit_FormatStringOffsetTable[] =
    {
    0,
    60,
    120,
    180,
    240,
    294,
    342,
    402,
    456,
    516,
    582,
    642,
    684,
    726,
    798,
    846,
    906,
    960,
    1008,
    1056,
    1122,
    1200,
    1266,
    1320,
    1374,
    1422,
    1470,
    1518,
    1566,
    1632,
    1710,
    1770,
    1836,
    1902,
    1986,
    2070,
    2136,
    2202,
    2268,
    2334,
    2394,
    2454,
    2526,
    2586,
    2652,
    2712,
    2760,
    2808,
    2868,
    2934,
    2994,
    3048,
    3102,
    3168,
    3240,
    3300,
    3366,
    3420,
    3480,
    3534,
    3600,
    3660,
    3702,
    3744,
    3786,
    3858,
    3906,
    3954,
    4002,
    4050,
    4098,
    4146,
    4206,
    4266,
    4314,
    4362,
    4422,
    4470,
    4518,
    4572
    };

static const MIDL_STUBLESS_PROXY_INFO IComSpyAudit_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IComSpyAudit_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IComSpyAudit_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IComSpyAudit_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(83) _IComSpyAuditProxyVtbl = 
{
    &IComSpyAudit_ProxyInfo,
    &IID_IComSpyAudit,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadStart */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadTerminate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadBindToApartment */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadUnBind */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadAssignApartment */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadUnassignApartment */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadWorkEnque */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadWorkPrivate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadWorkPublic */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadWorkRedirect */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnThreadWorkReject */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnAppActivation */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnAppShutdown */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjectCreate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjectDestroy */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnTransactionStart */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnTransactionPrepared */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnTransactionAborted */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnTransactionCommit */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnMethodCall */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnMethodReturn */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnMethodException */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjectActivate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjectDeactivate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnDisableCommit */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnEnableCommit */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnSetComplete */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnSetAbort */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnResourceCreate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnResourceAllocate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnResourceRecycle */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnResourceDestroy */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnResourceTrack */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnAuthenticate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnAuthenticateFail */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolPutObject */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolGetObject */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolRecycleToTx */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolGetFromTx */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolCreateObject */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolDestroyObject */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolCreateDecision */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolTimeout */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjPoolCreatePool */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnObjectConstruct */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityCreate */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityDestroy */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityEnter */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityTimeout */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityReenter */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityLeave */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnActivityLeaveSame */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnIISRequestInfo */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCRecord */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCQueueOpen */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCReceive */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCReceiveFail */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCMoveToReTryQueue */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCMoveToDeadQueue */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnQCPlayback */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnExceptionUser */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMRecoveryStart */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMRecoveryDone */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMCheckpoint */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMBegin */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMPrepare */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMCommit */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMAbort */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMIndoubt */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMDone */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMRelease */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMAnalyze */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMWrite */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMForget */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMForce */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::OnCRMDeliver */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::TargetUp */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::TargetDown */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::EngineDefined */ ,
    (void *) (INT_PTR) -1 /* IComSpyAudit::Init */
};

const CInterfaceStubVtbl _IComSpyAuditStubVtbl =
{
    &IID_IComSpyAudit,
    &IComSpyAudit_ServerInfo,
    83,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x600016e, /* MIDL Version 6.0.366 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _ComSpyAudit_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IComSpyAuditProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ComSpyAudit_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IComSpyAuditStubVtbl,
    0
};

PCInterfaceName const _ComSpyAudit_InterfaceNamesList[] = 
{
    "IComSpyAudit",
    0
};


#define _ComSpyAudit_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ComSpyAudit, pIID, n)

int __stdcall _ComSpyAudit_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_ComSpyAudit_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo ComSpyAudit_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ComSpyAudit_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ComSpyAudit_StubVtblList,
    (const PCInterfaceName * ) & _ComSpyAudit_InterfaceNamesList,
    0, // no delegation
    & _ComSpyAudit_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

