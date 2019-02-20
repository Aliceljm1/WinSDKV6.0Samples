//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include "FileDialogEventHandler.h" // for CDialogEventHandler class
#include "Resource.h"               // for some #defines used here such as INDEX_WORDDOC

//
// IFileDialogEvents methods
// This method gets called when the file-type is changed (combo-box selection changes)
// For sample sake, let's react to this event by changing the properties show
//
HRESULT CDialogEventHandler::OnTypeChange(IFileDialog *pfd)
{
    IFileSaveDialog *pfsd;

    HRESULT hr = pfd->QueryInterface(IID_PPV_ARGS(&pfsd));
    
    if (SUCCEEDED(hr))
    {
        UINT uIndex;
    
        //
        // Get the current file-type selection
        //
        hr = pfsd->GetFileTypeIndex(&uIndex);

        if (SUCCEEDED(hr))
        {
            IPropertyDescriptionList *pdl = NULL;
            
            switch (uIndex)
            {
            case INDEX_WORDDOC:
                //
                // When .doc is selected, let's ask for some arbitrary property, say Title 
                //
                hr = PSGetPropertyDescriptionListFromString(L"prop:System.Title", IID_PPV_ARGS(&pdl));
                
                if (SUCCEEDED(hr))
                {
                    //
                    // FALSE as second param == do not show default properties
                    //
                    hr = pfsd->SetCollectedProperties(pdl, FALSE);
                    pdl->Release();
                }

                break;

            case INDEX_WEBPAGE:
                //
                // When .html is selected, let's ask for some other arbitrary property, say Keywords
                //
                hr = PSGetPropertyDescriptionListFromString(L"prop:System.Keywords", IID_PPV_ARGS(&pdl));
                
                if (SUCCEEDED(hr))
                {
                    //
                    // FALSE as second param == do not show default properties
                    //
                    hr = pfsd->SetCollectedProperties(pdl, FALSE);
                    pdl->Release();
                }

                break;

            case INDEX_TEXTDOC:
                //
                // When .txt is selected, let's ask for some other arbitrary property, say Author 
                //
                hr = PSGetPropertyDescriptionListFromString(L"prop:System.Author", IID_PPV_ARGS(&pdl));
                
                if (SUCCEEDED(hr))
                {
                    //
                    // TRUE as second param == show default properties as well, but show Author property first in list
                    //
                    hr = pfsd->SetCollectedProperties(pdl, TRUE);
                    pdl->Release();
                }

                break;

            default:
                break;
            }
        }

        pfsd->Release();
    }

    return hr;
}


//
// IFileDialogControlEvents
// This method gets called when an dialog control item selection happens (radio-button selection. etc)
// For sample sake, let's react to this event by changing the dialog title
//
HRESULT CDialogEventHandler::OnItemSelected(IFileDialogCustomize *pfdc, DWORD dwIDCtl, DWORD dwIDItem)
{
    IFileDialog *pfd = NULL;

    HRESULT hr = pfdc->QueryInterface(IID_PPV_ARGS(&pfd));

    if (SUCCEEDED(hr))
    {
        if (dwIDCtl == CONTROL_RADIOBUTTONLIST)
        {
            switch (dwIDItem)
            {
            case CONTROL_RADIOBUTTON1:
                hr = pfd->SetTitle(L"Longhorn Dialog");
                break;

            case CONTROL_RADIOBUTTON2:
                hr = pfd->SetTitle(L"Vista Dialog");
                break;

            default:
                break;
            }
        }

        pfd->Release();
    }

    return hr;
}


//
// IUnknown
//
HRESULT CDialogEventHandler::QueryInterface(REFIID riid, void **ppv)
{
    HRESULT hr = E_NOINTERFACE;

    if (__uuidof(IUnknown) == riid)
    {
        *ppv = static_cast<IFileDialogEvents *>(this);   
    }
    else if (__uuidof(IFileDialogEvents) == riid)
    {
        *ppv = static_cast<IFileDialogEvents *>(this);   
    }
    else if (__uuidof(IFileDialogControlEvents) == riid)
    {
        *ppv = static_cast<IFileDialogControlEvents *>(this);   
    }
    else
    {
        *ppv = NULL;
    }

    if (*ppv != NULL)
    {
        AddRef();
        hr = S_OK;
    }

    return hr;
}


ULONG CDialogEventHandler::AddRef()
{
    return InterlockedIncrement(&_cRef);
}


ULONG CDialogEventHandler::Release()
{
    ULONG cRef = InterlockedDecrement(&_cRef);

    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}


//
// Instance creation helper
//
HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void **ppv)
{
    HRESULT hr = E_OUTOFMEMORY;
    CDialogEventHandler *pDialogEventHandler = new CDialogEventHandler();
    
    if (pDialogEventHandler != NULL)
    {
        hr = pDialogEventHandler->QueryInterface(riid, ppv);
        pDialogEventHandler->Release();
    }

    return hr;
}


