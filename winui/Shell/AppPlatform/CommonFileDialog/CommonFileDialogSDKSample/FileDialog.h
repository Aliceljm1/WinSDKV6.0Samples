//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <windows.h>    // for common windows data types and function headers
#include <shtypes.h>    // for COMDLG_FILTERSPEC

const COMDLG_FILTERSPEC c_rgSaveTypes[] = {{L"Word Document (*.doc)",       L"*.doc"},
                                           {L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
                                           {L"Text Document (*.txt)",       L"*.txt"},
                                           {L"All Documents (*.*)",         L"*.*"}};


class CCommonFileDialog
{
public:
    HRESULT BasicFileOpen(HWND hWnd);
    HRESULT AddItemsToCommonPlaces(HWND hWnd);
    HRESULT AddCustomControls(HWND hWnd);
    HRESULT SetDefaultValuesForProperties(HWND hWnd);
    HRESULT WritePropertiesUsingHandlers(HWND hWnd);
    HRESULT WritePropertiesWithoutUsingHandlers(HWND hWnd);

private:
    HRESULT _WriteDataToCustomFile(LPWSTR pwszFileName);
    HRESULT _WritePropertyToCustomFile(LPWSTR pwszFileName, LPWSTR pwszPropertyName, LPWSTR pwszValue);
    HRESULT _WriteDataToFile(HANDLE hFile, LPWSTR pwszData);
};

