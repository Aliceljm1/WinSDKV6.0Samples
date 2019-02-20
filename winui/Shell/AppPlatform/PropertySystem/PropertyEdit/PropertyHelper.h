// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

#include <shobjidl.h>
//#include <shlobj.h>
#include <propsys.h>
#include <propvarutil.h>
//#include <shlwapi.h>
#include <propkey.h>
//#include <propidl.h>
#include <strsafe.h>

class PropertyHelper
{
public:
	static HRESULT EnumerateProperties(PCWSTR pszFilename);
	static HRESULT GetPropertyValue(PCWSTR pszFilename, PCWSTR pszCanonicalName);
	static HRESULT SetPropertyValue(PCWSTR pszFilename, PCWSTR pszCanonicalName, PCWSTR pszValue);
	static HRESULT GetPropertyDescription(PCWSTR pszCanonicalName);

private:
	static HRESULT GetPropertyStore(PCWSTR pszFilename, IPropertyStore** ppPropertyStore, GETPROPERTYSTOREFLAGS gpsfFlags);

};

enum OperationType
{
	Get = 0,
	Set = 1,
	Enumerate = 2,
	Info = 3,
};