#include <windows.h> //needed for windows macros
#include <stdio.h> //needed for printf
#include <shlobj.h> //needed for IShellItem, IConditionFactory, and ICondition
#include <propkey.h> //needed for PROPERTYKEY's in Column lists
#include <propvarutil.h> //needed for InitPropVariantFromString
#include "EnumUnknown.h"

HRESULT GetCondition(ICondition** ppCondition);
HRESULT OpenCommonFileDialogTo(IShellItem* psi);

//
//  This program is a sample of how to use the ISearchFolderItemFactory interface
//  To create search folders.  It Creates the SearchFolderItemFactory, and sets
//  a few properties, condtitions, and scope, and then opens up the Common File Dialog
//  to the results of the search.
//
int __cdecl main()
{
    HRESULT hr = CoInitialize(NULL);

    if (SUCCEEDED(hr))
    {
        ISearchFolderItemFactory *pSearchFolderItemFactory = NULL;
        //
        // Create the ISearchFolderItemFactory Object
        //
        hr = CoCreateInstance(CLSID_SearchFolderItemFactory, 
                              NULL, 
                              CLSCTX_INPROC_SERVER, 
                              IID_PPV_ARGS(&pSearchFolderItemFactory));
      
        if (SUCCEEDED(hr))
        {
            PIDLIST_ABSOLUTE pidlScope;
            //
            // Get a pidl for the known folder FOLDERID_Profile
            //
            hr = SHGetKnownFolderIDList(FOLDERID_Profile, 0, NULL, &pidlScope);
            
            if (SUCCEEDED(hr))
            {
                IShellItemArray *pShellItemArray;
                //
                // Converts the pidl to an IShellItemArray
                //
                hr = SHCreateShellItemArrayFromIDLists(1, (LPCITEMIDLIST *) &pidlScope, &pShellItemArray);
                
                if (SUCCEEDED(hr))
                {
                    //
                    // Sets the scope of pSearchFolderItemFactory to the contennts of pShellItemArray
                    //
                    hr = pSearchFolderItemFactory->SetScope(pShellItemArray);
                    
                    if (SUCCEEDED(hr))
                    {
                        //
                        // Sets the display name of pSearchFolderItemFactory to "Specs Authored by foo"
                        //
                        hr = pSearchFolderItemFactory->SetDisplayName(L"Specs Authored by foo");
                     
                        if (SUCCEEDED(hr))
                        {
                            ICondition* pCondition = NULL;
                            //
                            // Gets a condition for us to use
                            //
                            hr = GetCondition(&pCondition);

                            if (SUCCEEDED(hr))
                            {
                                //
                                // Sets the condition for pSearchFolderItemFactory
                                //
                                hr = pSearchFolderItemFactory->SetCondition(pCondition);
                                
                                if (SUCCEEDED(hr))
                                {                
                                    //
                                    // Creates a column list and sets these as the visible columns for pSearchFolderItemFactory
                                    //
                                    PROPERTYKEY rgVisibleColumnKeys[] = {PKEY_ItemNameDisplay, 
                                                                         PKEY_DateModified, 
                                                                         PKEY_Author, 
                                                                         PKEY_ItemType, 
                                                                         PKEY_Keywords};

                                    hr = pSearchFolderItemFactory->SetVisibleColumns(ARRAYSIZE(rgVisibleColumnKeys), 
                                                                                     rgVisibleColumnKeys);
                                    
                                    if (SUCCEEDED(hr))
                                    {
                                        //
                                        // Creates a column list and sets these as the sort columns for pSearchFolderItemFactory.
                                        // In this case first we will sort decending by date modified, and then ascending by display name
                                        //
                                        SORTCOLUMN rgSortColumns[] = {{PKEY_DateModified, SORT_DESCENDING}, 
                                                                      {PKEY_ItemNameDisplay, SORT_ASCENDING}};
                                        
                                        hr = pSearchFolderItemFactory->SetSortColumns(ARRAYSIZE(rgSortColumns), rgSortColumns);
                                        
                                        if (SUCCEEDED(hr))
                                        {
                                            //
                                            // This retrieves an IShellItem from pSearchFolderItemFactory
                                            //
                                            IShellItem* psi = NULL;
                                            
                                            hr = pSearchFolderItemFactory->GetShellItem(IID_PPV_ARGS(&psi));
                                            
                                            if (SUCCEEDED(hr))
                                            {
                                                //
                                                // This method shows pSearchFolderItemFactory in the common file dialog
                                                //
                                                OpenCommonFileDialogTo(psi);
                                                psi->Release();
                                            }
                                        }
                                    }
                                }

                                pCondition->Release();
                            }
                        }
                    }

                    pShellItemArray->Release();
                }

                ILFree(pidlScope);
            }

            pSearchFolderItemFactory->Release();
        }

        CoUninitialize();
    }

    return SUCCEEDED(hr) ? 0 : 1;
}


//
// This creates a ICondition object based on some hard-coded criteria
//
HRESULT GetCondition(ICondition **ppCondition)
{
    IConditionFactory *pcf;        
    //
    // Create our condition factory.  This interface helps create conditions
    //
    HRESULT hr = CoCreateInstance(CLSID_ConditionFactory, 
                                  NULL, 
                                  CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pcf));
    
    if (SUCCEEDED(hr))
    {
        //
        // Below is a structure that allows my condition creation to be easily extensible
        //
        struct PROPERTY_MAPPING
        {
            LPWSTR pszValue;
            LPWSTR pszPropName;
        } rgpm[] = {{L"spec", L"System.Keywords"},
                    {L"Document", L"System.Kind"},
                    {L"foo", L"System.Author"}};

        IUnknown *rgUnknowns[ARRAYSIZE(rgpm)] = {0};

        for (int i = 0; i < ARRAYSIZE(rgUnknowns) && SUCCEEDED(hr); i++)
        {
            PROPVARIANT propvar;
            //
            // This creates a leaf condition for every entry in rgpm
            //
            hr = InitPropVariantFromString(rgpm[i].pszValue, &propvar);
            
            if (SUCCEEDED(hr))
            {
                ICondition* pCondition;
                hr = pcf->MakeLeaf(rgpm[i].pszPropName, //The Property Name
                                   COP_EQUAL,           //Comparison Operator
                                   NULL,                //The name of a semantic type, or NULL for plain string
                                   &propvar,            //The constant to which the property will be compared
                                   NULL,                //Pointer to an IRichChunk, only used for rebuilding previously created condition trees
                                   NULL,                //Pointer to an IRichChunk, only used for rebuilding previously created condition trees
                                   NULL,                //Pointer to an IRichChunk, only used for rebuilding previously created condition trees
                                   VARIANT_FALSE,       //only necisarry when first param is a virtual property
                                   &pCondition);   //out param to the condition
                
                if(SUCCEEDED(hr))
                {
                    hr = pCondition->QueryInterface(IID_PPV_ARGS(&rgUnknowns[i]));
                    pCondition->Release();
                }
            }
            
            PropVariantClear(&propvar);
        }

        if (SUCCEEDED(hr))
        {
            IEnumUnknown *penum;
            //
            // This creates an IEnumUnknown for our condition array.  This is needed to make a complicated condition
            //
            hr = CEnumUnknownWrapper::CreateInstance(rgUnknowns, ARRAYSIZE(rgUnknowns), IID_PPV_ARGS(&penum));
            
            if (SUCCEEDED(hr))
            {
                //
                // This combines the leaf conditions into one And Condition
                //
                hr = pcf->MakeAndOr(CT_AND_CONDITION, penum, VARIANT_FALSE, ppCondition);
                penum->Release();
            }
        }

        for (int i = 0; i < ARRAYSIZE(rgUnknowns); i++)
        {
            if (rgUnknowns[i] != NULL)
            {
                rgUnknowns[i]->Release();
            }
        }

        pcf->Release();
    }

    return hr;
}


//
// This opens up the common file dialog to an IShellItem and waits
// for the user to select a file from the results.  Then when they
// choose something, it displays their choice back to them.
//
HRESULT OpenCommonFileDialogTo(IShellItem* psi)
{
    IFileDialog* pfd = NULL;
    //
    // Create an instance of IFileOpenDialog
    //
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, 
                                  NULL, 
                                  CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfd));
    
    if (SUCCEEDED(hr))
    {
        //
        // Set it to the folder we want to show
        //
        hr = pfd->SetFolder(psi);
    
        if (SUCCEEDED(hr))
        {
            //
            // Show the File Dialog
            //
            hr = pfd->Show(NULL);

            if (SUCCEEDED(hr))
            {
                //
                // Now get the file that the user selected
                //
                IShellItem *psiResult = NULL;
                
                hr = pfd->GetResult(&psiResult);
                
                if (SUCCEEDED(hr))
                {
                    //
                    // Get the name from that file
                    //
                    LPWSTR pszPath;
                
                    hr = psiResult->GetDisplayName(SIGDN_NORMALDISPLAY, &pszPath);

                    if (SUCCEEDED(hr))
                    {
                        //
                        // Display it back to the user
                        //
                        wprintf(L"You Chose %s  Nice Choice\n", pszPath);
                        CoTaskMemFree(pszPath);
                    }

                    psiResult->Release();
                }
            }
        }

        pfd->Release();
    }

    return hr;
}