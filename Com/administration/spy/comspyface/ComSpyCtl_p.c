// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Jun 01 15:50:15 2005
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


#include "ComSpyCtl.h"

#define TYPE_FORMAT_STRING_SIZE   1101                              
#define PROC_FORMAT_STRING_SIZE   925                               
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

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


extern const MIDL_SERVER_INFO IComSpy_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IComSpy_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICOMSysLCE_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICOMSysLCE_ProxyInfo;


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

	/* Procedure get_Window */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter phwnd */

/* 24 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure putref_Font */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFont */

/* 60 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Font */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x8 ),	/* 8 */
/* 86 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFont */

/* 96 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Font */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppFont */

/* 132 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	NdrFcShort( 0x18 ),	/* Type Offset=24 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LogFile */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xb ),	/* 11 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 162 */	NdrFcShort( 0x1 ),	/* 1 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 168 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LogFile */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xc ),	/* 12 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x1 ),	/* 1 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 204 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ShowGridLines */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xd ),	/* 13 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x24 ),	/* 36 */
/* 230 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 232 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 240 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ShowGridLines */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xe ),	/* 14 */
/* 260 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 262 */	NdrFcShort( 0x8 ),	/* 8 */
/* 264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 266 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 268 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ColWidth */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0xf ),	/* 15 */
/* 296 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 298 */	NdrFcShort( 0x6 ),	/* 6 */
/* 300 */	NdrFcShort( 0x24 ),	/* 36 */
/* 302 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nColumn */

/* 312 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 318 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 324 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 326 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ColWidth */

/* 330 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 332 */	NdrFcLong( 0x0 ),	/* 0 */
/* 336 */	NdrFcShort( 0x10 ),	/* 16 */
/* 338 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 340 */	NdrFcShort( 0xe ),	/* 14 */
/* 342 */	NdrFcShort( 0x8 ),	/* 8 */
/* 344 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 346 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nColumn */

/* 354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 356 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 358 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter newVal */

/* 360 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 362 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 366 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SelectApplications */

/* 372 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 374 */	NdrFcLong( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0x11 ),	/* 17 */
/* 380 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 384 */	NdrFcShort( 0x8 ),	/* 8 */
/* 386 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 388 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 394 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 396 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 398 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveToFile */

/* 402 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 404 */	NdrFcLong( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x12 ),	/* 18 */
/* 410 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 416 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 418 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 422 */	NdrFcShort( 0x0 ),	/* 0 */
/* 424 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearAllEvents */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x13 ),	/* 19 */
/* 440 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x8 ),	/* 8 */
/* 446 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 448 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 456 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure About */

/* 462 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 464 */	NdrFcLong( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x14 ),	/* 20 */
/* 470 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x8 ),	/* 8 */
/* 476 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 478 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 480 */	NdrFcShort( 0x0 ),	/* 0 */
/* 482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 484 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 488 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LogToFile */

/* 492 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 494 */	NdrFcLong( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x15 ),	/* 21 */
/* 500 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 504 */	NdrFcShort( 0x24 ),	/* 36 */
/* 506 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 508 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 516 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 518 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 522 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 524 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 526 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LogToFile */

/* 528 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 530 */	NdrFcLong( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x16 ),	/* 22 */
/* 536 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 542 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 544 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 552 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 554 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 558 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 560 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 562 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ChooseFont */

/* 564 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0x17 ),	/* 23 */
/* 572 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 578 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 580 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 588 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 590 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ShowOnScreen */

/* 594 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 596 */	NdrFcLong( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x18 ),	/* 24 */
/* 602 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x24 ),	/* 36 */
/* 608 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 610 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 618 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 620 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 622 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 626 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ShowOnScreen */

/* 630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x19 ),	/* 25 */
/* 638 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 640 */	NdrFcShort( 0x8 ),	/* 8 */
/* 642 */	NdrFcShort( 0x8 ),	/* 8 */
/* 644 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 646 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 654 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 656 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 662 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ChooseLogFile */

/* 666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x1a ),	/* 26 */
/* 674 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x24 ),	/* 36 */
/* 680 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 682 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 684 */	NdrFcShort( 0x1 ),	/* 1 */
/* 686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sLogFileName */

/* 690 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 692 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 694 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Parameter bCanceled */

/* 696 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 698 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 704 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Audit */

/* 708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 714 */	NdrFcShort( 0x1b ),	/* 27 */
/* 716 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x24 ),	/* 36 */
/* 722 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 724 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 732 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 734 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 740 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Audit */

/* 744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x1c ),	/* 28 */
/* 752 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 754 */	NdrFcShort( 0x8 ),	/* 8 */
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 760 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 768 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 770 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 774 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 776 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetEventType */

/* 780 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 782 */	NdrFcLong( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x3 ),	/* 3 */
/* 788 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	NdrFcShort( 0x22 ),	/* 34 */
/* 794 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 796 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter e */

/* 804 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 806 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 808 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Return value */

/* 810 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 812 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 814 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetEventClass */

/* 816 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 818 */	NdrFcLong( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0x4 ),	/* 4 */
/* 824 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0x4c ),	/* 76 */
/* 830 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 832 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter guid */

/* 840 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 842 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 844 */	NdrFcShort( 0x60 ),	/* Type Offset=96 */

	/* Return value */

/* 846 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 848 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 850 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Install */

/* 852 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x5 ),	/* 5 */
/* 860 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 864 */	NdrFcShort( 0x8 ),	/* 8 */
/* 866 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 868 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 872 */	NdrFcShort( 0x21 ),	/* 33 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter PropertyName */

/* 876 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 878 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 880 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Parameter PropertyValue */

/* 882 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 884 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 886 */	NdrFcShort( 0x442 ),	/* Type Offset=1090 */

	/* Return value */

/* 888 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 890 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 892 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Uninstall */

/* 894 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 896 */	NdrFcLong( 0x0 ),	/* 0 */
/* 900 */	NdrFcShort( 0x6 ),	/* 6 */
/* 902 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 904 */	NdrFcShort( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 910 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 916 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 918 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 920 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 922 */	0x8,		/* FC_LONG */
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
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  8 */	NdrFcLong( 0xbef6e003 ),	/* -1091117053 */
/* 12 */	NdrFcShort( 0xa874 ),	/* -22412 */
/* 14 */	NdrFcShort( 0x101a ),	/* 4122 */
/* 16 */	0x8b,		/* 139 */
			0xba,		/* 186 */
/* 18 */	0x0,		/* 0 */
			0xaa,		/* 170 */
/* 20 */	0x0,		/* 0 */
			0x30,		/* 48 */
/* 22 */	0xc,		/* 12 */
			0xab,		/* 171 */
/* 24 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 26 */	NdrFcShort( 0xffec ),	/* Offset= -20 (6) */
/* 28 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 30 */	NdrFcShort( 0x1c ),	/* Offset= 28 (58) */
/* 32 */	
			0x13, 0x0,	/* FC_OP */
/* 34 */	NdrFcShort( 0xe ),	/* Offset= 14 (48) */
/* 36 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 38 */	NdrFcShort( 0x2 ),	/* 2 */
/* 40 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 42 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 44 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 46 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 48 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (36) */
/* 54 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 56 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 58 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x4 ),	/* 4 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */
/* 66 */	NdrFcShort( 0xffde ),	/* Offset= -34 (32) */
/* 68 */	
			0x12, 0x0,	/* FC_UP */
/* 70 */	NdrFcShort( 0xffea ),	/* Offset= -22 (48) */
/* 72 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */
/* 76 */	NdrFcShort( 0x4 ),	/* 4 */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (68) */
/* 82 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 84 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/* 86 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 88 */	NdrFcShort( 0x8 ),	/* Offset= 8 (96) */
/* 90 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 92 */	NdrFcShort( 0x8 ),	/* 8 */
/* 94 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 96 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 98 */	NdrFcShort( 0x10 ),	/* 16 */
/* 100 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 102 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 104 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (90) */
			0x5b,		/* FC_END */
/* 108 */	
			0x11, 0x0,	/* FC_RP */
/* 110 */	NdrFcShort( 0xffda ),	/* Offset= -38 (72) */
/* 112 */	
			0x12, 0x0,	/* FC_UP */
/* 114 */	NdrFcShort( 0x3bc ),	/* Offset= 956 (1070) */
/* 116 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 118 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 120 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 122 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 124 */	NdrFcShort( 0x2 ),	/* Offset= 2 (126) */
/* 126 */	NdrFcShort( 0x10 ),	/* 16 */
/* 128 */	NdrFcShort( 0x2f ),	/* 47 */
/* 130 */	NdrFcLong( 0x14 ),	/* 20 */
/* 134 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 136 */	NdrFcLong( 0x3 ),	/* 3 */
/* 140 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 142 */	NdrFcLong( 0x11 ),	/* 17 */
/* 146 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 148 */	NdrFcLong( 0x2 ),	/* 2 */
/* 152 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 154 */	NdrFcLong( 0x4 ),	/* 4 */
/* 158 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 160 */	NdrFcLong( 0x5 ),	/* 5 */
/* 164 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 166 */	NdrFcLong( 0xb ),	/* 11 */
/* 170 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 172 */	NdrFcLong( 0xa ),	/* 10 */
/* 176 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 178 */	NdrFcLong( 0x6 ),	/* 6 */
/* 182 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (414) */
/* 184 */	NdrFcLong( 0x7 ),	/* 7 */
/* 188 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 190 */	NdrFcLong( 0x8 ),	/* 8 */
/* 194 */	NdrFcShort( 0xff82 ),	/* Offset= -126 (68) */
/* 196 */	NdrFcLong( 0xd ),	/* 13 */
/* 200 */	NdrFcShort( 0xdc ),	/* Offset= 220 (420) */
/* 202 */	NdrFcLong( 0x9 ),	/* 9 */
/* 206 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (438) */
/* 208 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 212 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (456) */
/* 214 */	NdrFcLong( 0x24 ),	/* 36 */
/* 218 */	NdrFcShort( 0x30a ),	/* Offset= 778 (996) */
/* 220 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 224 */	NdrFcShort( 0x304 ),	/* Offset= 772 (996) */
/* 226 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 230 */	NdrFcShort( 0x302 ),	/* Offset= 770 (1000) */
/* 232 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 236 */	NdrFcShort( 0x300 ),	/* Offset= 768 (1004) */
/* 238 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 242 */	NdrFcShort( 0x2fe ),	/* Offset= 766 (1008) */
/* 244 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 248 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (1012) */
/* 250 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 254 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (1016) */
/* 256 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 260 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (1020) */
/* 262 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 266 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (1004) */
/* 268 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 272 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (1008) */
/* 274 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 278 */	NdrFcShort( 0x2ea ),	/* Offset= 746 (1024) */
/* 280 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 284 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (1020) */
/* 286 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 290 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (1028) */
/* 292 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 296 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (1032) */
/* 298 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 302 */	NdrFcShort( 0x2de ),	/* Offset= 734 (1036) */
/* 304 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 308 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (1040) */
/* 310 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 314 */	NdrFcShort( 0x2da ),	/* Offset= 730 (1044) */
/* 316 */	NdrFcLong( 0x10 ),	/* 16 */
/* 320 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 322 */	NdrFcLong( 0x12 ),	/* 18 */
/* 326 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 328 */	NdrFcLong( 0x13 ),	/* 19 */
/* 332 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 334 */	NdrFcLong( 0x15 ),	/* 21 */
/* 338 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 340 */	NdrFcLong( 0x16 ),	/* 22 */
/* 344 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 346 */	NdrFcLong( 0x17 ),	/* 23 */
/* 350 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 352 */	NdrFcLong( 0xe ),	/* 14 */
/* 356 */	NdrFcShort( 0x2b8 ),	/* Offset= 696 (1052) */
/* 358 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 362 */	NdrFcShort( 0x2bc ),	/* Offset= 700 (1062) */
/* 364 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 368 */	NdrFcShort( 0x2ba ),	/* Offset= 698 (1066) */
/* 370 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 374 */	NdrFcShort( 0x276 ),	/* Offset= 630 (1004) */
/* 376 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 380 */	NdrFcShort( 0x274 ),	/* Offset= 628 (1008) */
/* 382 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 386 */	NdrFcShort( 0x272 ),	/* Offset= 626 (1012) */
/* 388 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 392 */	NdrFcShort( 0x268 ),	/* Offset= 616 (1008) */
/* 394 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 398 */	NdrFcShort( 0x262 ),	/* Offset= 610 (1008) */
/* 400 */	NdrFcLong( 0x0 ),	/* 0 */
/* 404 */	NdrFcShort( 0x0 ),	/* Offset= 0 (404) */
/* 406 */	NdrFcLong( 0x1 ),	/* 1 */
/* 410 */	NdrFcShort( 0x0 ),	/* Offset= 0 (410) */
/* 412 */	NdrFcShort( 0xffff ),	/* Offset= -1 (411) */
/* 414 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 416 */	NdrFcShort( 0x8 ),	/* 8 */
/* 418 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 420 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 422 */	NdrFcLong( 0x0 ),	/* 0 */
/* 426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 430 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 432 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 434 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 436 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 438 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 440 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 446 */	NdrFcShort( 0x0 ),	/* 0 */
/* 448 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 450 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 452 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 454 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 456 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 458 */	NdrFcShort( 0x2 ),	/* Offset= 2 (460) */
/* 460 */	
			0x12, 0x0,	/* FC_UP */
/* 462 */	NdrFcShort( 0x204 ),	/* Offset= 516 (978) */
/* 464 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 466 */	NdrFcShort( 0x18 ),	/* 24 */
/* 468 */	NdrFcShort( 0xa ),	/* 10 */
/* 470 */	NdrFcLong( 0x8 ),	/* 8 */
/* 474 */	NdrFcShort( 0x5a ),	/* Offset= 90 (564) */
/* 476 */	NdrFcLong( 0xd ),	/* 13 */
/* 480 */	NdrFcShort( 0x7e ),	/* Offset= 126 (606) */
/* 482 */	NdrFcLong( 0x9 ),	/* 9 */
/* 486 */	NdrFcShort( 0x9e ),	/* Offset= 158 (644) */
/* 488 */	NdrFcLong( 0xc ),	/* 12 */
/* 492 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (692) */
/* 494 */	NdrFcLong( 0x24 ),	/* 36 */
/* 498 */	NdrFcShort( 0x124 ),	/* Offset= 292 (790) */
/* 500 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 504 */	NdrFcShort( 0x12e ),	/* Offset= 302 (806) */
/* 506 */	NdrFcLong( 0x10 ),	/* 16 */
/* 510 */	NdrFcShort( 0x148 ),	/* Offset= 328 (838) */
/* 512 */	NdrFcLong( 0x2 ),	/* 2 */
/* 516 */	NdrFcShort( 0x162 ),	/* Offset= 354 (870) */
/* 518 */	NdrFcLong( 0x3 ),	/* 3 */
/* 522 */	NdrFcShort( 0x17c ),	/* Offset= 380 (902) */
/* 524 */	NdrFcLong( 0x14 ),	/* 20 */
/* 528 */	NdrFcShort( 0x196 ),	/* Offset= 406 (934) */
/* 530 */	NdrFcShort( 0xffff ),	/* Offset= -1 (529) */
/* 532 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 534 */	NdrFcShort( 0x4 ),	/* 4 */
/* 536 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 542 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 544 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 546 */	NdrFcShort( 0x4 ),	/* 4 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x1 ),	/* 1 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0x12, 0x0,	/* FC_UP */
/* 558 */	NdrFcShort( 0xfe02 ),	/* Offset= -510 (48) */
/* 560 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 562 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 564 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 566 */	NdrFcShort( 0x8 ),	/* 8 */
/* 568 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 570 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 572 */	NdrFcShort( 0x4 ),	/* 4 */
/* 574 */	NdrFcShort( 0x4 ),	/* 4 */
/* 576 */	0x11, 0x0,	/* FC_RP */
/* 578 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (532) */
/* 580 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 582 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 584 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 594 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 598 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 600 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 602 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (420) */
/* 604 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 606 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 608 */	NdrFcShort( 0x8 ),	/* 8 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x6 ),	/* Offset= 6 (618) */
/* 614 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 616 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 618 */	
			0x11, 0x0,	/* FC_RP */
/* 620 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (584) */
/* 622 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 626 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 630 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 632 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 636 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 638 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 640 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (438) */
/* 642 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 644 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 646 */	NdrFcShort( 0x8 ),	/* 8 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x6 ),	/* Offset= 6 (656) */
/* 652 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 654 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 656 */	
			0x11, 0x0,	/* FC_RP */
/* 658 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (622) */
/* 660 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 662 */	NdrFcShort( 0x4 ),	/* 4 */
/* 664 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 670 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 672 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 674 */	NdrFcShort( 0x4 ),	/* 4 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x1 ),	/* 1 */
/* 680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 684 */	0x12, 0x0,	/* FC_UP */
/* 686 */	NdrFcShort( 0x180 ),	/* Offset= 384 (1070) */
/* 688 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 690 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 692 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0x6 ),	/* Offset= 6 (704) */
/* 700 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 702 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 704 */	
			0x11, 0x0,	/* FC_RP */
/* 706 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (660) */
/* 708 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 710 */	NdrFcLong( 0x2f ),	/* 47 */
/* 714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 718 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 720 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 722 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 724 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 726 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 728 */	NdrFcShort( 0x1 ),	/* 1 */
/* 730 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 732 */	NdrFcShort( 0x4 ),	/* 4 */
/* 734 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 736 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 738 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 740 */	NdrFcShort( 0x10 ),	/* 16 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0xa ),	/* Offset= 10 (754) */
/* 746 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 748 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 750 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (708) */
/* 752 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 754 */	
			0x12, 0x0,	/* FC_UP */
/* 756 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (726) */
/* 758 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 760 */	NdrFcShort( 0x4 ),	/* 4 */
/* 762 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 768 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 770 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 772 */	NdrFcShort( 0x4 ),	/* 4 */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x1 ),	/* 1 */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	0x12, 0x0,	/* FC_UP */
/* 784 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (738) */
/* 786 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 788 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 790 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x6 ),	/* Offset= 6 (802) */
/* 798 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 800 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 802 */	
			0x11, 0x0,	/* FC_RP */
/* 804 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (758) */
/* 806 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 808 */	NdrFcShort( 0x18 ),	/* 24 */
/* 810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 812 */	NdrFcShort( 0xa ),	/* Offset= 10 (822) */
/* 814 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 816 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 818 */	NdrFcShort( 0xfd2e ),	/* Offset= -722 (96) */
/* 820 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 822 */	
			0x11, 0x0,	/* FC_RP */
/* 824 */	NdrFcShort( 0xff10 ),	/* Offset= -240 (584) */
/* 826 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 828 */	NdrFcShort( 0x1 ),	/* 1 */
/* 830 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 836 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 838 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 842 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 844 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 846 */	NdrFcShort( 0x4 ),	/* 4 */
/* 848 */	NdrFcShort( 0x4 ),	/* 4 */
/* 850 */	0x12, 0x0,	/* FC_UP */
/* 852 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (826) */
/* 854 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 856 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 858 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 860 */	NdrFcShort( 0x2 ),	/* 2 */
/* 862 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 866 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 868 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 870 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 874 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 876 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 878 */	NdrFcShort( 0x4 ),	/* 4 */
/* 880 */	NdrFcShort( 0x4 ),	/* 4 */
/* 882 */	0x12, 0x0,	/* FC_UP */
/* 884 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (858) */
/* 886 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 888 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 890 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 892 */	NdrFcShort( 0x4 ),	/* 4 */
/* 894 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 898 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 900 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 902 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 904 */	NdrFcShort( 0x8 ),	/* 8 */
/* 906 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 908 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 910 */	NdrFcShort( 0x4 ),	/* 4 */
/* 912 */	NdrFcShort( 0x4 ),	/* 4 */
/* 914 */	0x12, 0x0,	/* FC_UP */
/* 916 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (890) */
/* 918 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 920 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 922 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 924 */	NdrFcShort( 0x8 ),	/* 8 */
/* 926 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 930 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 932 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 934 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 936 */	NdrFcShort( 0x8 ),	/* 8 */
/* 938 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 940 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 942 */	NdrFcShort( 0x4 ),	/* 4 */
/* 944 */	NdrFcShort( 0x4 ),	/* 4 */
/* 946 */	0x12, 0x0,	/* FC_UP */
/* 948 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (922) */
/* 950 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 952 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 954 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 956 */	NdrFcShort( 0x8 ),	/* 8 */
/* 958 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 960 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 962 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 964 */	NdrFcShort( 0x8 ),	/* 8 */
/* 966 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 968 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 970 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 972 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 974 */	NdrFcShort( 0xffec ),	/* Offset= -20 (954) */
/* 976 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 978 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 980 */	NdrFcShort( 0x28 ),	/* 40 */
/* 982 */	NdrFcShort( 0xffec ),	/* Offset= -20 (962) */
/* 984 */	NdrFcShort( 0x0 ),	/* Offset= 0 (984) */
/* 986 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 988 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 990 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 992 */	NdrFcShort( 0xfdf0 ),	/* Offset= -528 (464) */
/* 994 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 996 */	
			0x12, 0x0,	/* FC_UP */
/* 998 */	NdrFcShort( 0xfefc ),	/* Offset= -260 (738) */
/* 1000 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1002 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1004 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1006 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1008 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1010 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1012 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1014 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1016 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1018 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1020 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1022 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1024 */	
			0x12, 0x0,	/* FC_UP */
/* 1026 */	NdrFcShort( 0xfd9c ),	/* Offset= -612 (414) */
/* 1028 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1030 */	NdrFcShort( 0xfc3e ),	/* Offset= -962 (68) */
/* 1032 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1034 */	NdrFcShort( 0xfd9a ),	/* Offset= -614 (420) */
/* 1036 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1038 */	NdrFcShort( 0xfda8 ),	/* Offset= -600 (438) */
/* 1040 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1042 */	NdrFcShort( 0xfdb6 ),	/* Offset= -586 (456) */
/* 1044 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1046 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1048) */
/* 1048 */	
			0x12, 0x0,	/* FC_UP */
/* 1050 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1070) */
/* 1052 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1054 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1056 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1058 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1060 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1062 */	
			0x12, 0x0,	/* FC_UP */
/* 1064 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1052) */
/* 1066 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1068 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1070 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1072 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1076) */
/* 1078 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1080 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1082 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1084 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1086 */	NdrFcShort( 0xfc36 ),	/* Offset= -970 (116) */
/* 1088 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1090 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1092 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1094 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1098 */	NdrFcShort( 0xfc26 ),	/* Offset= -986 (112) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_ComSpyCtl_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IComSpy, ver. 0.0,
   GUID={0xA9B39080,0xAAD0,0x11D0,{0xB1,0x83,0x00,0xAA,0x00,0xBA,0x32,0x58}} */

#pragma code_seg(".orpc")
static const unsigned short IComSpy_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    144,
    180,
    216,
    252,
    288,
    330,
    372,
    402,
    432,
    462,
    492,
    528,
    564,
    594,
    630,
    666,
    708,
    744
    };

static const MIDL_STUBLESS_PROXY_INFO IComSpy_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IComSpy_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IComSpy_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IComSpy_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(29) _IComSpyProxyVtbl = 
{
    &IComSpy_ProxyInfo,
    &IID_IComSpy,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_Window */ ,
    (void *) (INT_PTR) -1 /* IComSpy::putref_Font */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_Font */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_Font */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_LogFile */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_LogFile */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_ShowGridLines */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_ShowGridLines */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_ColWidth */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_ColWidth */ ,
    (void *) (INT_PTR) -1 /* IComSpy::SelectApplications */ ,
    (void *) (INT_PTR) -1 /* IComSpy::SaveToFile */ ,
    (void *) (INT_PTR) -1 /* IComSpy::ClearAllEvents */ ,
    (void *) (INT_PTR) -1 /* IComSpy::About */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_LogToFile */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_LogToFile */ ,
    (void *) (INT_PTR) -1 /* IComSpy::ChooseFont */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_ShowOnScreen */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_ShowOnScreen */ ,
    (void *) (INT_PTR) -1 /* IComSpy::ChooseLogFile */ ,
    (void *) (INT_PTR) -1 /* IComSpy::get_Audit */ ,
    (void *) (INT_PTR) -1 /* IComSpy::put_Audit */
};


static const PRPC_STUB_FUNCTION IComSpy_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IComSpyStubVtbl =
{
    &IID_IComSpy,
    &IComSpy_ServerInfo,
    29,
    &IComSpy_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ICOMSysLCE, ver. 0.0,
   GUID={0x0243D433,0x3849,0x11d2,{0x89,0xBB,0x00,0xC0,0x4F,0x99,0x14,0x0F}} */

#pragma code_seg(".orpc")
static const unsigned short ICOMSysLCE_FormatStringOffsetTable[] =
    {
    780,
    816,
    852,
    894
    };

static const MIDL_STUBLESS_PROXY_INFO ICOMSysLCE_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ICOMSysLCE_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICOMSysLCE_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ICOMSysLCE_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _ICOMSysLCEProxyVtbl = 
{
    &ICOMSysLCE_ProxyInfo,
    &IID_ICOMSysLCE,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* ICOMSysLCE::GetEventType */ ,
    (void *) (INT_PTR) -1 /* ICOMSysLCE::GetEventClass */ ,
    (void *) (INT_PTR) -1 /* ICOMSysLCE::Install */ ,
    (void *) (INT_PTR) -1 /* ICOMSysLCE::Uninstall */
};

const CInterfaceStubVtbl _ICOMSysLCEStubVtbl =
{
    &IID_ICOMSysLCE,
    &ICOMSysLCE_ServerInfo,
    7,
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

const CInterfaceProxyVtbl * _ComSpyCtl_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ICOMSysLCEProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IComSpyProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ComSpyCtl_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ICOMSysLCEStubVtbl,
    ( CInterfaceStubVtbl *) &_IComSpyStubVtbl,
    0
};

PCInterfaceName const _ComSpyCtl_InterfaceNamesList[] = 
{
    "ICOMSysLCE",
    "IComSpy",
    0
};

const IID *  _ComSpyCtl_BaseIIDList[] = 
{
    0,
    &IID_IDispatch,
    0
};


#define _ComSpyCtl_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ComSpyCtl, pIID, n)

int __stdcall _ComSpyCtl_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _ComSpyCtl, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _ComSpyCtl, 2, *pIndex )
    
}

const ExtendedProxyFileInfo ComSpyCtl_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ComSpyCtl_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ComSpyCtl_StubVtblList,
    (const PCInterfaceName * ) & _ComSpyCtl_InterfaceNamesList,
    (const IID ** ) & _ComSpyCtl_BaseIIDList,
    & _ComSpyCtl_IID_Lookup, 
    2,
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

