//+-------------------------------------------------------------------------
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//  File:       RebootReduction.cpp
//
//--------------------------------------------------------------------------

#include <windows.h>
#include <msiltcfg.h>
#include <stdio.h>

LPCTSTR szmsiltcfg = TEXT("msiltcfg.dll");
LPSTR szShutdownMsi = "ShutdownMsi";
LPSTR szRestartMsi = "RestartMsi";

extern "C" int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst , LPSTR lpszCmdLine, int nCmdShow)
{
	HMODULE hLib = LoadLibrary(szmsiltcfg);
	typedef UINT (WINAPI *PFnShutdownMsi)(DWORD);
	typedef UINT (WINAPI *PFnRestartMsi)(DWORD);

	if(!hLib)
	{
		printf(TEXT("Failed to load %s. Error: %d"), szmsiltcfg, GetLastError());
		goto ReturnFromMain;
	}

	PFnShutdownMsi ShutdownMsi = (PFnShutdownMsi)GetProcAddress(hLib, szShutdownMsi);
	PFnRestartMsi RestartMsi = (PFnRestartMsi)GetProcAddress(hLib, szRestartMsi);
	if(!ShutdownMsi || !RestartMsi)
	{
		printf(TEXT("Missing entry points in %s"), szmsiltcfg);
		goto ReturnFromMain;
		return 0;
	}
	

	UINT uiRet = ERROR_SUCCESS;
	if(ERROR_SUCCESS != (uiRet = ShutdownMsi(0)))
	{
		printf("ShutdownMsi failed with %u", uiRet);
		goto ReturnFromMain;
	}

	// Update Windows Installer here
	
	if(ERROR_SUCCESS != (uiRet = RestartMsi(0)))
	{
		printf("RestartMsi failed with %u", uiRet);
		goto ReturnFromMain;
	}

ReturnFromMain:	
	if(hLib)
		FreeLibrary(hLib);
	return 0;
}