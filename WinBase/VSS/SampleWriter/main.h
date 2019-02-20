/*
**++
**
** Copyright (c) 2006  Microsoft Corporation
**
**
** Module Name:
**
**	    main.h
**
**
** Abstract:
**
**	Test program to to register a Writer with various properties
**
*/

#ifndef _MAIN_H_
#define _MAIN_H_

extern "C" int __cdecl wmain(int argc, wchar_t ** argv);
void loadFile(wchar_t* fileName);
BOOL WINAPI handler(DWORD dwCtrlType);

#endif

