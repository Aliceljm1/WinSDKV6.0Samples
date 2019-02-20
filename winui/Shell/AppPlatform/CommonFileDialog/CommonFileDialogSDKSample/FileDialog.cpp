// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//


#include "FileDialog.h"             // for CCommonFileDialog class
#include "Resource.h"               // for some #defines used here such as INDEX_WORDDOC
#include <shobjidl.h>               // for IFileDialog/IFileOpenDialog/IFileSaveDialog
#include <knownfolders.h>           // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>            // for PROPVAR-related functions
#include <propkey.h>                // for the Property key APIs/datatypes
#include <propidl.h>                // for the Property System APIs
#include <strsafe.h>                // for StringCchPrintfW
#include <shlwapi.h>                // for PathAppendW
#include <shlobj.h>                 // for KNOWNFOLDERID_Pictures

extern HINSTANCE g_hInstance;  // Handle to application instance
extern HWND g_hWndApp;         // HWND of the app

HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void **ppv); // CDialogEventHandler instance creator


//
// This code snippet demonstrates how to work with the common file dialog interface
//
HRESULT CCommonFileDialog::BasicFileOpen(HWND hWnd)
{
    IFileDialog *pfd = NULL;

    //
    // CoCreate the File Open Dialog object
    //
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        //
        // Create an event handling object, and hook it up to the dialog
        //
        IFileDialogEvents   *pfde       = NULL;
        DWORD               dwCookie    = 0;
        
        hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (SUCCEEDED(hr))
        {
            //
            // Hook up the event handler
            //
            hr = pfd->Advise(pfde, &dwCookie);
            if (SUCCEEDED(hr))
            {
                //
                // Set the options on the dialog
                //
                DWORD dwFlags = 0;

                //
                // Before setting, always get the options first in order not 
                // to override existing options
                //
                hr = pfd->GetOptions(&dwFlags);
                if (SUCCEEDED(hr))
                {
                    //
                    // In this case, get shell items only for file system items. 
                    //
                    hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                    if (SUCCEEDED(hr))
                    {
                        //
                        // Set the file types to display only. Notice that, this is a 
                        // 1-based array
                        //
                        hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                        if (SUCCEEDED(hr))
                        {
                            //
                            // Set the selected file type index to Word Docs for this example.
                            //
                            hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
                            if (SUCCEEDED(hr))
                            {
                                //
                                // Set the default extension to be ".doc" file.

                                hr = pfd->SetDefaultExtension(L"doc");
                                if (SUCCEEDED(hr))
                                {
                                    //
                                    // Show the dialog
                                    //
                                    hr = pfd->Show(hWnd);
                                    if (SUCCEEDED(hr))
                                    {
                                        //
                                        // Obtain the result, once the user clicks the 'Open' button.
                                        // The result is an IShellItem object.
                                        //
                                        IShellItem *psiResult;

                                        hr = pfd->GetResult(&psiResult);
                                        if (SUCCEEDED(hr))
                                        {
                                            //
                                            // We are just going to print out the name of the file
                                            // for sample sake
                                            //
                                            LPWSTR pwszFilePath = NULL;

                                            hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pwszFilePath);
                                            if (SUCCEEDED(hr))
                                            {
                                                TaskDialog(hWnd,
                                                           NULL,
                                                           L"CommonFileDialogApp",
                                                           pwszFilePath, 
                                                           NULL,
                                                           TDCBF_OK_BUTTON,
                                                           TD_INFORMATION_ICON,
                                                           NULL);
                                                CoTaskMemFree(pwszFilePath);
                                            }

                                            psiResult->Release();                
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //
                // Unhook the event handler
                //
                pfd->Unadvise(dwCookie);
            }
        
            pfde->Release();
        }

        pfd->Release();
    }

    return hr;
}
        

//
// The Common Places area in the File Dialog is extensible.
// This code snippet demonstrates how to extend the Common Places area.
// Look at CDialogEventHandler::OnItemSelected to see how messages pertaining to the added
// controls can be processed
//
HRESULT CCommonFileDialog::AddItemsToCommonPlaces(HWND hWnd)
{
    IFileDialog *pfd = NULL;

    //
    // CoCreate the File Open Dialog object
    //
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        //
        // Always use known folders instead of hard-coding physical file
        // paths. In this case we are using Public Music KnownFolder.
        //
        IKnownFolderManager *pkfm = NULL;

        //
        // CoCreate the KnownFolderManager object
        //
        hr = CoCreateInstance(CLSID_KnownFolderManager, NULL, CLSCTX_INPROC_SERVER, 
                              IID_PPV_ARGS(&pkfm));
        if (SUCCEEDED(hr))
        {
            IKnownFolder *pKnownFolder = NULL;

            //
            // Get the known folder
            //
            hr = pkfm->GetFolder(FOLDERID_PublicMusic, &pKnownFolder);
            if (SUCCEEDED(hr))
            {   
                //
                // File Dialog APIs need an IShellItem that represents the location
                //
                IShellItem *psi = NULL;
                
                hr = pKnownFolder->GetShellItem(0, IID_PPV_ARGS(&psi));
                if (SUCCEEDED(hr))
                {
                    //
                    // Add the place to the bottom of default list in Common File Dialog.
                    //
                    hr = pfd->AddPlace(psi, FDAP_BOTTOM);
                    if (SUCCEEDED(hr))
                    {
                        //
                        // Show the File Dialog
                        //
                        hr = pfd->Show(hWnd);
                        if (SUCCEEDED(hr))
                        {
                            //
                            // You can add your own code here to handle the results
                            //
                        }
                    }

                    psi->Release();
                }

                pKnownFolder->Release();
            }

            pkfm->Release();
        }

        pfd->Release();
    }
    
    return hr;
}


//
// This code snippet demonstrates how to add custom controls in the Common File Dialog
//
HRESULT CCommonFileDialog::AddCustomControls(HWND hWnd)
{
    IFileDialog *pfd = NULL;

    //
    // CoCreate the File Open Dialog object
    //
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        //
        // Create an event handling object, and hook it up to the dialog
        //
        IFileDialogEvents   *pfde       = NULL;
        DWORD               dwCookie    = 0;
        
        hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (SUCCEEDED(hr))
        {
            //
            // Hook up the event handler
            //
            hr = pfd->Advise(pfde, &dwCookie);
            if (SUCCEEDED(hr))
            {
                IFileDialogCustomize *pfdc = NULL;

                //
                // Create a Customization
                //
                hr = pfd->QueryInterface(IID_PPV_ARGS(&pfdc));
                if (SUCCEEDED(hr))
                {
                    //
                    // Create a Visual Group
                    //
                    hr = pfdc->StartVisualGroup(CONTROL_GROUP, L"Sample Group");
                    if (SUCCEEDED(hr))
                    {
                        //
                        // Add a radio-button list
                        //
                        hr = pfdc->AddRadioButtonList(CONTROL_RADIOBUTTONLIST);
                        if (SUCCEEDED(hr))
                        {
                            //
                            // Set the state of the added radio-button list
                            //
                            hr = pfdc->SetControlState(CONTROL_RADIOBUTTONLIST, 
                                                       CDCS_VISIBLE | CDCS_ENABLED);
                            if (SUCCEEDED(hr))
                            {
                                //
                                // Add individual buttons to the radio-button list
                                //
                                hr = pfdc->AddControlItem(CONTROL_RADIOBUTTONLIST, 
                                                          CONTROL_RADIOBUTTON1, 
                                                          L"Change Title to Longhorn");
                                if (SUCCEEDED(hr))
                                {
                                    hr = pfdc->AddControlItem(CONTROL_RADIOBUTTONLIST, 
                                                              CONTROL_RADIOBUTTON2, 
                                                              L"Change Title to Vista");
                                    if (SUCCEEDED(hr))
                                    {
                                        //
                                        // Set the default selection to option 1
                                        //
                                        hr = pfdc->SetSelectedControlItem(CONTROL_RADIOBUTTONLIST, 
                                                                          CONTROL_RADIOBUTTON1);
                                    }
                                }
                            }
                        }

                        //
                        // End the visual group
                        //
                        pfdc->EndVisualGroup();
                    }
                
                    pfdc->Release();
                }
            }

            pfde->Release();
        }

        if (SUCCEEDED(hr))
        {
            //
            // Now show the dialog
            //
            hr = pfd->Show(hWnd);
            if (SUCCEEDED(hr))
            {
                //
                // You can add your own code here to handle the results
                //
            }

            //
            // Unhook the event handler
            //
            pfd->Unadvise(dwCookie);
        }

        pfd->Release();
    }

    return hr;
}


//
// This code snippet demonstrates how to add default metadata in the Common File Dialog
// Look at CDialogEventHandler::OnTypeChange to see to change the order/list of properties
// displayed in the Common File Dialog
//
HRESULT CCommonFileDialog::SetDefaultValuesForProperties(HWND hWnd)
{
    IFileSaveDialog *pfsd = NULL;

    //
    // CoCreate the File Open Dialog object
    //
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        //
        // Create an event handling object, and hook it up to the dialog
        //
        IFileDialogEvents   *pfde       = NULL;
        DWORD               dwCookie    = 0;
        
        hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (SUCCEEDED(hr))
        {
            //
            // Hook up the event handler
            //
            hr = pfsd->Advise(pfde, &dwCookie);
            if (SUCCEEDED(hr))
            {
                //
                // Set the file types to display
                //
                hr = pfsd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                if (SUCCEEDED(hr))
                {
                    hr = pfsd->SetFileTypeIndex(INDEX_WORDDOC);
                    if (SUCCEEDED(hr))
                    {
                        hr = pfsd->SetDefaultExtension(L"doc");
                        if (SUCCEEDED(hr))
                        {
                            IPropertyStore *pPropertyStore = NULL;

                            //
                            // The InMemory Property Store is a Property Store that is 
                            // kept in the memory instead of persisted in a file stream.
                            //
                            hr = CoCreateInstance(CLSID_InMemoryPropertyStore, NULL, CLSCTX_INPROC_SERVER, 
                                                  IID_PPV_ARGS(&pPropertyStore));
                            if (SUCCEEDED(hr))
                            {
                                PROPVARIANT propvarValue = {0};
                                
                                hr = InitPropVariantFromString(L"Foobar", &propvarValue);
                                if (SUCCEEDED(hr))
                                {
                                    //
                                    // Always let the property system APIs do the coercion for you
                                    //
                                    hr = PSCoerceToCanonicalValue(PKEY_Keywords, &propvarValue);
                                    if (SUCCEEDED(hr))
                                    {
                                        //
                                        // Set the value to the property store of the item
                                        //
                                        hr = pPropertyStore->SetValue(PKEY_Keywords, propvarValue);
                                        if (SUCCEEDED(hr))
                                        {
                                            //
                                            // Commit does the actual writing back to the in memory store.
                                            //
                                            hr = pPropertyStore->Commit();
                                            if (SUCCEEDED(hr))
                                            {
                                                //
                                                // Hand these properties to the File Dialog
                                                //
                                                hr = pfsd->SetProperties(pPropertyStore);
                                            }
                                        }
                                    }
                                }
                                
                                PropVariantClear(&propvarValue);
                                pPropertyStore->Release();
                            }
                        }
                    }
                }
            }

            pfde->Release();
        }

        if (SUCCEEDED(hr))
        {
            //
            // Now show the dialog
            //
            hr = pfsd->Show(hWnd);
            if (SUCCEEDED(hr))
            {
                //
                // You can add your own code here to handle the results
                //
            }

            //
            // Unhook the event handler
            //
            pfsd->Unadvise(dwCookie);
        }

        pfsd->Release();
    }

    return hr;
}


//
// 
// The following code snippet demonstrates two things:
// 1.  How to write properties using property handlers
// 2.  Replicating properties in the "Save As" scenario where the user choses to save an existing file 
//     with a different name.  We need to make sure we replicate not just the data, 
//     but also the properties of the original file
//
//
HRESULT CCommonFileDialog::WritePropertiesUsingHandlers(HWND hWnd)
{
    IFileSaveDialog *pfsd = NULL;

    //
    // CoCreate the File Open Dialog object
    //
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        WCHAR wszFullPathToTestFile[MAX_PATH] = {0};

        //
        // For this exercise, let's just support only one file type to make things simpler
        // Also, let's use the jpg format for sample purpose because the Windows ships with
        // property handlers for jpg files
        //
        const COMDLG_FILTERSPEC rgSaveTypes[] = {{L"Photo Document (*.jpg)", L"*.jpg"}};

        //
        // Set the file types to display
        //
        hr = pfsd->SetFileTypes(ARRAYSIZE(rgSaveTypes), rgSaveTypes);
        if (SUCCEEDED(hr))
        {
            hr = pfsd->SetFileTypeIndex(0);
        
            if (SUCCEEDED(hr))
            {
                //
                // Set default file extension
                //
                hr = pfsd->SetDefaultExtension(L"jpg");

                if (SUCCEEDED(hr))
                {
                    //
                    // Ensure the dialog only returns items that can be represented by file system paths
                    //
                    DWORD dwFlags;

                    hr = pfsd->GetOptions(&dwFlags);
                    
                    if (SUCCEEDED(hr))
                    {
                        hr = pfsd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

                        //
                        // Let's first get the current property set of the file we are replicating
                        // and give it to the file dialog object
                        //
                        // For simplicity sake, let's just get the property set from a pre-existing jpg file (in the Pictures folder)
                        // In the real-world, you would actually add code to get the property set of the file 
                        // that is currently open and is being replicated
                        // 
                        if (SUCCEEDED(hr))
                        {
                            LPWSTR pwszPicturesFolderPath = NULL;

                            hr = SHGetKnownFolderPath(FOLDERID_Pictures,
                                                      KF_FLAG_DEFAULT_PATH,
                                                      NULL,
                                                      &pwszPicturesFolderPath);
                            if (SUCCEEDED(hr))
                            {
                                PathCombineW(wszFullPathToTestFile, pwszPicturesFolderPath, L"TestPic.jpg");

                                if (SUCCEEDED(hr))
                                {
                                    IPropertyStore *pPropertyStore;

                                    hr = SHGetPropertyStoreFromParsingName(wszFullPathToTestFile,
                                                                           NULL,
                                                                           GPS_DEFAULT, 
                                                                           IID_PPV_ARGS(&pPropertyStore));

                                    if (FAILED(hr))
                                    {
                                        //
                                        // TestPic.jpg is probably not in the Pictures folder
                                        //
                                        TaskDialog(hWnd,
                                                   NULL,
                                                   L"CommonFileDialogApp",
                                                   L"Create TestPic.jpg in the Pictures folder and try again.", 
                                                   NULL,
                                                   TDCBF_OK_BUTTON,
                                                   TD_ERROR_ICON ,
                                                   NULL);
                                    }
                                    else
                                    {
                                        //
                                        // Call SetProperties on the file dialog object for getting back later
                                        //
                                        pfsd->SetProperties(pPropertyStore);
                                        pPropertyStore->Release();
                                    }
                                }

                                CoTaskMemFree(pwszPicturesFolderPath);
                            }
                        }
                    }
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = pfsd->Show(hWnd);
            if (SUCCEEDED(hr))
            {
                IShellItem *psiResult;

                //
                // Get the result
                //
                hr = pfsd->GetResult(&psiResult);
                if (SUCCEEDED(hr))
                {
                    LPWSTR pwszNewFileName;

                    //
                    // Get the path to the file
                    //
                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pwszNewFileName);
                    if (SUCCEEDED(hr))
                    {
                        //
                        // This is where you add code to write data to your file
                        // For simplicity, let's just copy a pre-existing dummy jpg file
                        //
                        // In the real-world, you would actually add code to replicate the data of 
                        // file that is currently open
                        //
                        hr = CopyFileW(wszFullPathToTestFile, pwszNewFileName, FALSE) ? 
                                S_OK : HRESULT_FROM_WIN32(GetLastError());
                        if (SUCCEEDED(hr))
                        {
                            //
                            // Now apply the properties
                            //
                            // Get the property store first by calling GetPropertyStore and pass it on to ApplyProperties
                            // This will make the registered propety handler for the specified file type (jpg)
                            // do all the work of writing the properties for you
                            //
                            // Property handlers for the specified file type should be registered for this
                            // to work
                            //
                            //
                            IPropertyStore *pPropertyStore;

                            //
                            // When we call GetProperties, we get back all the properties that we originally set 
                            // (in our call to SetProperties above) plus the ones user modified in the file dialog
                            //
                            hr = pfsd->GetProperties(&pPropertyStore);
                            if (SUCCEEDED(hr))
                            {
                                //
                                // Now apply the properties making use of the registered property handler for the file type
                                //
                                // hWnd is used as parent for any error dialogs that might popup when writing properties
                                // Pass NULL for IFileOperationProgressSink as we don't want to register any callback for progress notifications
                                //
                                hr = pfsd->ApplyProperties(psiResult, pPropertyStore, hWnd, NULL);
                                pPropertyStore->Release();
                            }
                        }

                        CoTaskMemFree(pwszNewFileName);
                    }

                    psiResult->Release();
                }
            }
        }

        pfsd->Release();
    }

    return hr;
}


//
// This code snippet demonstrates how to write properties without using property handlers
//
HRESULT CCommonFileDialog::WritePropertiesWithoutUsingHandlers(HWND hWnd)
{
    IFileSaveDialog *pfsd = NULL;

    //
    // CoCreate the File Open Dialog object
    //
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        //
        // For this exercise, let's use a custom file type 
        //
        const COMDLG_FILTERSPEC rgSaveTypes[] = {{L"MyApp Document (*.myApp)", L"*.myApp"}};

        //
        // Set the file types to display
        //
        hr = pfsd->SetFileTypes(ARRAYSIZE(rgSaveTypes), rgSaveTypes);
        if (SUCCEEDED(hr))
        {
            hr = pfsd->SetFileTypeIndex(0);
            if (SUCCEEDED(hr))
            {
                //
                // Set default file extension
                //
                hr = pfsd->SetDefaultExtension(L"myApp");
                if (SUCCEEDED(hr))
                {
                    //
                    // Ensure the dialog only returns items that can be represented by file system paths
                    //
                    DWORD dwFlags;

                    hr = pfsd->GetOptions(&dwFlags);
                    if (SUCCEEDED(hr))
                    {
                        hr = pfsd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                        if (SUCCEEDED(hr))
                        {
                            //
                            // Set the properties you want the FileSave dialog to collect from the user
                            //
                            IPropertyDescriptionList *pdl;

                            hr = PSGetPropertyDescriptionListFromString(L"prop:System.Keywords", 
                                                                        IID_PPV_ARGS(&pdl));
                            if (SUCCEEDED(hr))
                            {
                                //
                                // TRUE as second param == show default properties as well, but show Keyword first
                                //
                                hr = pfsd->SetCollectedProperties(pdl, TRUE);
                                pdl->Release();
                            }
                        }
                    }
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            //
            // Now show the dialog
            //
            hr = pfsd->Show(hWnd);
            if (SUCCEEDED(hr))
            {
                IShellItem *psiResult;

                //
                // Get the result
                //
                hr = pfsd->GetResult(&psiResult);
                if (SUCCEEDED(hr))
                {
                    LPWSTR pwszNewFileName;

                    //
                    // Get the path to the file
                    //
                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pwszNewFileName);
                    if (SUCCEEDED(hr))
                    {
                        //
                        // Write data to the file
                        //
                        hr = _WriteDataToCustomFile(pwszNewFileName);
                        if (SUCCEEDED(hr))
                        {
                            //
                            // Now get the property store and write each individual property to the file
                            //
                            IPropertyStore *pPropertyStore;
                            
                            hr = pfsd->GetProperties(&pPropertyStore);
                            if (SUCCEEDED(hr))
                            {
                                DWORD cProps = 0;

                                hr = pPropertyStore->GetCount(&cProps);

                                //
                                // Loop over property set and write each property/value pair to the file
                                //
                                for (DWORD i = 0; i < cProps && SUCCEEDED(hr); i++)
                                {
                                    PROPERTYKEY key;

                                    hr = pPropertyStore->GetAt(i, &key);
                                    if (SUCCEEDED(hr))
                                    {
                                        LPWSTR pwszPropertyName;

                                        hr = PSGetNameFromPropertyKey(key, &pwszPropertyName);
                                        if (SUCCEEDED(hr))
                                        {
                                            PROPVARIANT propvarValue;

                                            PropVariantInit(&propvarValue);

                                            //
                                            // Get the value of the property
                                            //
                                            hr = pPropertyStore->GetValue(key, &propvarValue);
                                            if (SUCCEEDED(hr))
                                            {
                                                WCHAR wszValue[MAX_PATH];

                                                //
                                                // Always use property system APIs to do the conversion for you
                                                //
                                                hr = PropVariantToString(propvarValue, 
                                                                         wszValue, 
                                                                         ARRAYSIZE(wszValue));
                                                if (SUCCEEDED(hr))
                                                {
                                                    //
                                                    // Write the property to the file
                                                    //
                                                    hr = _WritePropertyToCustomFile(pwszNewFileName, 
                                                                                    pwszPropertyName, 
                                                                                    wszValue);
                                                }
                                            }

                                            PropVariantClear(&propvarValue);
                                            CoTaskMemFree(pwszPropertyName);
                                        }
                                    }
                                }

                                pPropertyStore->Release();
                            }
                        }

                        CoTaskMemFree(pwszNewFileName);
                    }

                    psiResult->Release();
                }
            }
        }
    }

    return hr;
}


//
// Helper function to write dummy content to a custom file format
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
//
HRESULT CCommonFileDialog::_WriteDataToCustomFile(LPWSTR pwszFileName)
{
    HANDLE hFile = CreateFileW(pwszFileName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ, 
                               NULL, 
                               CREATE_ALWAYS,  // Let's always create this file
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);

    HRESULT hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;
    if (SUCCEEDED(hr))
    {
        WCHAR wszDummyContent[] = L"[MYAPPDATA]\r\nThis is an example of how to use the IFileSaveDialog interface.\r\n[ENDMYAPPDATA]\r\n";

        hr = _WriteDataToFile(hFile, wszDummyContent);
        CloseHandle(hFile);
    }

    return hr;
}


//
// Helper function to write property/value into a custom file format
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
//
HRESULT CCommonFileDialog::_WritePropertyToCustomFile(LPWSTR pwszFileName, LPWSTR pwszPropertyName, LPWSTR pwszValue)
{
    HANDLE hFile = CreateFileW(pwszFileName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ, 
                               NULL, 
                               OPEN_ALWAYS, // We will write only if the file exists
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);

    HRESULT hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;

    if (SUCCEEDED(hr))
    {
        static WCHAR    wszPropertyStartTag[]   = L"[PROPERTY]";
        static WCHAR    wszPropertyEndTag[]     = L"[ENDPROPERTY]\r\n";
        static DWORD    cchPropertyStartTag     = (DWORD) wcslen(wszPropertyStartTag);
        static DWORD    cchPropertyEndTag       = (DWORD) wcslen(wszPropertyEndTag);
        DWORD cchPropertyLine = cchPropertyStartTag + 
                                cchPropertyEndTag + 
                                (DWORD) wcslen(pwszPropertyName) + 
                                (DWORD) wcslen(pwszValue) +
                                2; // 1 for '=' + 1 for NULL terminator
        LPWSTR pwszPropertyLine = new WCHAR[cchPropertyLine];

        if (pwszPropertyLine == NULL)
        {
            hr = E_OUTOFMEMORY;
        }
        else
        {
            hr = StringCchPrintfW(pwszPropertyLine, 
                                  cchPropertyLine, 
                                  L"%s%s=%s%s",
                                  wszPropertyStartTag,
                                  pwszPropertyName,
                                  pwszValue,
                                  wszPropertyEndTag);
            if (SUCCEEDED(hr))
            {
                hr = SetFilePointer(hFile, 0, NULL, FILE_END) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
                
                if (SUCCEEDED(hr))
                {
                    hr = _WriteDataToFile(hFile, pwszPropertyLine);
                }
            }

            delete [] pwszPropertyLine;
        }

        CloseHandle(hFile);
    }

    return hr;
}


//
// A helper function that converts UNICODE data to ANSI and writes it to the given file
// We write in ANSI format to make it easier to open the output file in Notepad.
//
HRESULT CCommonFileDialog::_WriteDataToFile(HANDLE hFile, LPWSTR pwszData)
{
    HRESULT hr = E_FAIL;

    //
    // First figure out how big a buffer we need...
    //
    DWORD cbData = WideCharToMultiByte(CP_ACP, 0, pwszData, -1, NULL, 0, NULL, NULL);
    if (cbData == 0)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    else
    {
        //
        // Now allocate a buffer of the required size, and call WideCharToMultiByte again to do the actual conversion
        //
        CHAR *pszData = new CHAR[cbData];
        if (pszData == NULL)
        {
            hr = E_OUTOFMEMORY;
        }
        else
        {
            hr = WideCharToMultiByte(CP_ACP, 0, pwszData, -1, pszData, cbData, NULL, NULL) ? 
                    S_OK : HRESULT_FROM_WIN32(GetLastError());
            if (SUCCEEDED(hr))
            {

                DWORD dwBytesWritten = 0;

                hr = WriteFile(hFile, pszData, cbData - 1, &dwBytesWritten, NULL) ? 
                        S_OK : HRESULT_FROM_WIN32(GetLastError());
            }

            delete [] pszData;
        }
    }

    return hr;
}
