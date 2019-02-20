//////////////////////////////////////////////////////////////////////////////
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <shobjidl.h> //For IShellItemImageFactory
#include <stdio.h>
#include "resource.h"


INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void DisplayUsage();
int LookUp(LPWSTR pwszArg);

int __cdecl wmain(int argc, wchar_t *argv[])
{
    int nSize ;

    if ((argc != 3))
    {
        DisplayUsage();
    }
    else if ( !(nSize = LookUp(argv[1])))
    {
        DisplayUsage();
    }
    else
    {
        LPWSTR pwszError = NULL;

        HRESULT hRes = CoInitializeEx(NULL, ::COINIT_APARTMENTTHREADED);
        if (SUCCEEDED(hRes))
        {            
            IShellItemImageFactory *pImageFactory;

            //Getting the IShellItemImageFactory interface pointer by using passing the absolute path to a file.
            hRes = SHCreateItemFromParsingName(argv[2], NULL, IID_PPV_ARGS(&pImageFactory));

            if (SUCCEEDED(hRes))
            {
                SIZE sz = { nSize, nSize };

                HBITMAP hImage;             
    
                //sz - Size of the image, SIIGBF_BIGGERSIZEOK - GetImage will stretch down the bitmap (preserving aspect ratio)
                hRes = pImageFactory->GetImage(sz, SIIGBF_BIGGERSIZEOK , &hImage);

                if (SUCCEEDED(hRes))
                {
                    DialogBoxParamW(NULL, MAKEINTRESOURCEW(IDD_DIALOG1), GetDesktopWindow(), DialogProc, (LPARAM)hImage);
                }
                else
                {
                    pwszError = L"IShellItemImageFactory::GetImage failed with error code %x";
                }

                pImageFactory->Release();
            }
            else
            {
                pwszError = L"SHCreateItemFromParsingName failed with error %x";                        
            }

            CoUninitialize();
        }
        else
        {
            pwszError = L"CoInitializeEx failed with error code %x";
        }

        if (FAILED(hRes))
        {
            wprintf(pwszError, hRes);
        }
    }

    return 0;
}

struct IMAGESIZE 
{
    LPWSTR pwszSize;
    int nVal;
};


int LookUp(LPWSTR pwszArg)
{
    int nReturn = 0;

    //The possbile sizes for the image that is requested
    IMAGESIZE sizeTable[] = { {L"small", 16}, {L"medium", 48}, {L"large", 96}, {L"extralarge", 256} };

    for (int i = 0; i < ARRAYSIZE(sizeTable); i++)
    {
        if ( lstrcmpiW(pwszArg, sizeTable[i].pwszSize) == 0)
        {
            nReturn = sizeTable[i].nVal;
            break;
        }
    }

    return nReturn;
}

void DisplayUsage()
{
    wprintf(L"Usage: IShellItemImageFacotry sample app help                         \n");
    wprintf(L"ImageFacotrySample.exe <size> <Absolute Path to file> \n");
    wprintf(L"size - small, medium, large, extralarge     \n");
    wprintf(L"e.g. ImageFactorySample.exe medium c:\\HelloWorld.jpg \n");

}

INT_PTR CALLBACK  DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    INT_PTR nReturn = FALSE;

    switch (message)
    {
    case WM_INITDIALOG:                  
        SendDlgItemMessage(hDlg,IDC_STATIC1, STM_SETIMAGE , IMAGE_BITMAP, (LPARAM)lParam);
        nReturn = TRUE;     

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            nReturn = TRUE;
        }
        break;
    }
    return nReturn;
}


