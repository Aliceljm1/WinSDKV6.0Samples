// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <windows.h>
#include <shellapi.h>
#include <wmistr.h>
#include <guiddef.h>
#include <evntrace.h>
#include <tdh.h>

#define MAX_BUFFER (1024)
#define PROPERTY_IS_STRUCTURE(_f) (_f & PropertyStruct)

#define G_ALLOC( s )  HeapAlloc( GetProcessHeap(), (HEAP_ZERO_MEMORY), s )
#define G_FREE( s )   if( s != NULL ) { HeapFree( GetProcessHeap(), 0, s ); }