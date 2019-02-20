/**************************************************************************
    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
 
   (c) Microsoft Corporation. All Rights Reserved.
**************************************************************************/

#include <windows.h>
#include <shlobj.h>
#include "FolderViewImpl.h"
#include "resource.h"
#include "Utils.h"
#include "shlwapi.h"

HRESULT LoadFolderViewImplDisplayStrings(WCHAR* wszArrStrings[], UINT cArray)
{
    HRESULT hr = E_FAIL;
    UINT uString = 0;
    for (UINT i = 0; i < cArray; i++)
    {
        switch (i)
        {
        case 0:
            uString = IDS_ZERO;
            break;
        case 1:
            uString = IDS_ONE;
            break;
        case 2:
            uString = IDS_TWO;
            break;
        case 3:
            uString = IDS_THREE;
            break;
        case 4:
            uString = IDS_FOUR;
            break;
        case 5:
            uString = IDS_FIVE;
            break;
        case 6:
            uString = IDS_SIX;
            break;
        case 7:
            uString = IDS_SEVEN;
            break;
        case 8:
            uString = IDS_EIGHT;
            break;
        case 9:
            uString = IDS_NINE;
            break;
        default:
            uString = 0;
        }

        if (uString)
        {
            WCHAR wszTemp[MAX_PATH];
            LoadStringW(g_hInst, uString, wszTemp, ARRAYSIZE(wszTemp));
            hr = SHStrDupW(wszTemp, &wszArrStrings[i]);
            if (FAILED(hr))
            {
                // Free those already allocated.
                for (UINT k = 0; k < i; k++)
                {
                    CoTaskMemFree(wszArrStrings[k]);
                    wszArrStrings[k] = NULL;
                }
                
                break;
            }
        }
        else
        {
            // Somebody tried increasing the size of the array without 
            // adding additional strings.
            wszArrStrings[i] = NULL;
        }
    }
    
    return hr;
}

STDAPI StringToStrRetW(LPCWSTR pszName, STRRET *pStrRet)
{
    pStrRet->uType = STRRET_WSTR;
    return SHStrDupW(pszName, &pStrRet->pOleStr);
}
