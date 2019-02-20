// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "StdAfx.h"
#include "PropertyHelper.h"

HRESULT PropertyHelper::EnumerateProperties(PCWSTR pszFilename)
{
	HRESULT hr = pszFilename ? S_OK : E_FAIL ;

	if (SUCCEEDED(hr))
	{
		IPropertyStore* pPropertyStore = NULL;

		// Call the helper to get the property store for the initialized item
		// Note that as long as you have the property store, you are keeping the file open
		// So always release it once you are done.

		hr = GetPropertyStore(pszFilename, &pPropertyStore, GPS_DEFAULT);
		if (SUCCEEDED(hr))
		{
			// Retrieve the number of properties stored in the item.
			DWORD cProperties = 0;
			hr = pPropertyStore->GetCount(&cProperties);
			if(SUCCEEDED(hr))
			{
				for(DWORD i = 0; i < cProperties; i++)
				{
					// Get the property key at a given index.
					PROPERTYKEY propertyKey;
					hr = pPropertyStore->GetAt(i, &propertyKey);
					if(SUCCEEDED(hr))
					{	
						PROPVARIANT propvarValue = {0};
						hr = pPropertyStore->GetValue(propertyKey, &propvarValue);

						if (SUCCEEDED(hr))
						{
							PWSTR pszDisplayValue = NULL;
							hr = PSFormatForDisplayAlloc(propertyKey,  propvarValue, PDFF_DEFAULT, &pszDisplayValue); 
							
							if (SUCCEEDED(hr))
							{
								// Try to get the canonical name of the property
								PWSTR pszCanonicalName = NULL;
								hr = PSGetNameFromPropertyKey(propertyKey, &pszCanonicalName);
								
								if (SUCCEEDED(hr))
								{
									WCHAR szOutputString[MAX_PATH];
			
									hr = StringCchPrintf(szOutputString, ARRAYSIZE(szOutputString), 
														TEXT("%s = %s \n"), pszCanonicalName, pszDisplayValue);

									if (SUCCEEDED(hr))
									{
										wprintf(szOutputString);
									}
									
									CoTaskMemFree(pszCanonicalName);
								}
								CoTaskMemFree(pszDisplayValue);
							}
						}
					}
				}
			}
			pPropertyStore->Release();
		}
		else
		{
			wprintf(TEXT("Error %x: getting the propertystore for the item.\n"), hr);
		}
		
	}
	else
	{
		wprintf(TEXT("Error: Missing filename \n"));
	}
	return hr;
}

HRESULT PropertyHelper::GetPropertyValue(PCWSTR pszFilename, PCWSTR pszCanonicalName)
{
	HRESULT hr = (pszFilename && pszCanonicalName) ? S_OK : E_INVALIDARG;

	if (SUCCEEDED(hr))
	{
		// Convert the Canonical name of the property to PROPERTYKEY
		PROPERTYKEY propertyKey;
		hr = PSGetPropertyKeyFromName(pszCanonicalName, &propertyKey);
			
		if (SUCCEEDED(hr))
		{
			IPropertyStore* pPropertyStore = NULL;

			// Call the helper to get the property store for the initialized item
			hr = GetPropertyStore(pszFilename, &pPropertyStore, GPS_DEFAULT);
			if (SUCCEEDED(hr))
			{
				PROPVARIANT propvarValue = {0};
				hr = pPropertyStore->GetValue(propertyKey, &propvarValue);

				if (SUCCEEDED(hr))
				{
					PWSTR pszDisplayValue = NULL;
					hr = PSFormatForDisplayAlloc(propertyKey,  propvarValue, PDFF_DEFAULT, &pszDisplayValue); 
					
					if (SUCCEEDED(hr))
					{
						WCHAR szOutputString[MAX_PATH];
				
						hr = StringCchPrintf(szOutputString, ARRAYSIZE(szOutputString), 
											TEXT("%s = %s \n"), pszCanonicalName, pszDisplayValue);

						if (SUCCEEDED(hr))
						{
							wprintf(szOutputString);
						}

						CoTaskMemFree(pszDisplayValue);
					}
				}	
				pPropertyStore->Release();
			}
			else
			{
				wprintf(TEXT("Error %x: getting the propertystore for the item.\n"), hr);
			}
		}
	}
	else
	{
		wprintf(TEXT("Error: Missing file or property name. \n"));
	}
	return hr;
}

HRESULT PropertyHelper::SetPropertyValue(PCWSTR pszFilename, PCWSTR pszCanonicalName, PCWSTR pszValue)
{
	HRESULT hr = (pszFilename && pszCanonicalName) ? S_OK : E_INVALIDARG;

	if (SUCCEEDED(hr))
	{
		// Convert the Canonical name of the property to PROPERTYKEY
		PROPERTYKEY propertyKey;
		hr = PSGetPropertyKeyFromName(pszCanonicalName, &propertyKey);
        if(SUCCEEDED(hr))
		{

			IPropertyStore* pPropertyStore = NULL;

			// Call the helper to get the property store for the 
			// initialized item
			hr = GetPropertyStore(pszFilename, &pPropertyStore, GPS_READWRITE);
			if (SUCCEEDED(hr))
			{
				PROPVARIANT propvarValue = {0};
				
				hr = InitPropVariantFromString(pszValue, &propvarValue);

				if (SUCCEEDED(hr))
				{
					IPropertyDescription* pPropertyDescription = NULL;
					hr = PSGetPropertyDescriptionByName(pszCanonicalName, IID_PPV_ARGS(&pPropertyDescription));

					if (SUCCEEDED(hr))
					{
						hr = pPropertyDescription->CoerceToCanonicalValue(&propvarValue);
						
						if (SUCCEEDED(hr))
						{
							// Set the value to the property store of the item.
							hr = pPropertyStore->SetValue(propertyKey, propvarValue);
							if (SUCCEEDED(hr))
							{
								// Commit does the actual writing back to the file stream.
								hr = pPropertyStore->Commit();
								if (SUCCEEDED(hr))
								{
									WCHAR szOutputString[MAX_PATH];
							
									hr = StringCchPrintf(szOutputString, ARRAYSIZE(szOutputString), 
									TEXT("Property %s value %s written succesfully \n"), 
									pszCanonicalName, pszValue);
						
									if (SUCCEEDED(hr))
									{
										wprintf(szOutputString);
									}
								}
								else
								{
									wprintf(TEXT("Error %x: Commit to the propertystore failed.\n"), hr);
								}
							}
							else
							{
								wprintf(TEXT("Error %x: Set value to the propertystore failed.\n"), hr);
							}
						}
						PropVariantClear(&propvarValue);
						pPropertyDescription->Release();
					}
				}
				pPropertyStore->Release();
			}
			else
			{
				wprintf(TEXT("Error %x: getting the propertystore for the item.\n"), hr);
			}
		}
	}
	else
	{
		wprintf(TEXT("Error: Missing file or property name. \n"));
	}
	return hr;
}

HRESULT PropertyHelper::GetPropertyDescription(PCWSTR pszCanonicalName)
{
	HRESULT hr = (pszCanonicalName) ? S_OK : E_INVALIDARG;

	if (SUCCEEDED(hr))
	{
		// Convert the Canonical name of the property to PROPERTYKEY
		PROPERTYKEY propertyKey;
		hr = PSGetPropertyKeyFromName(pszCanonicalName, &propertyKey);
        if(SUCCEEDED(hr))
		{
			// Get the property description for the given property.
			// Property description contains meta information on the property itself.
			IPropertyDescription* pPropertyDescription = NULL;
			hr = PSGetPropertyDescription(propertyKey, IID_PPV_ARGS(&pPropertyDescription));
			if(SUCCEEDED(hr))
			{		 
				PWSTR pszPropertyLabel = NULL;

				hr = pPropertyDescription->GetDisplayName(&pszPropertyLabel);

				if (SUCCEEDED(hr))
				{
					WCHAR szOutputString[MAX_PATH];
			
					hr = StringCchPrintf(szOutputString, ARRAYSIZE(szOutputString), 
								TEXT("Property %s has a label : %s \n"), 
								pszCanonicalName, pszPropertyLabel);

					if (SUCCEEDED(hr))
					{
						wprintf(szOutputString);
					}
					CoTaskMemFree(pszPropertyLabel);
				}
				pPropertyDescription->Release();
			}
		}
	}
	return hr;
}

HRESULT PropertyHelper::GetPropertyStore(PCWSTR pszFilename, IPropertyStore** ppPropertyStore, GETPROPERTYSTOREFLAGS gpsfFlags)
{
	HRESULT hr = S_OK;
    
	// Create a Shell Item using the file name (parsing name) of the item.
	hr = SHGetPropertyStoreFromParsingName(pszFilename, NULL, gpsfFlags, IID_PPV_ARGS(ppPropertyStore));
	if (FAILED(hr))
	{
		// Assume in the working directory
		WCHAR pszCurrentDirectory[MAX_PATH];
		hr = GetCurrentDirectory(MAX_PATH, pszCurrentDirectory) ? S_OK : GetLastError();
		
		if (SUCCEEDED(hr))
		{
			WCHAR pszNewFilename[MAX_PATH];

			hr = StringCchPrintf(pszNewFilename, ARRAYSIZE(pszNewFilename), TEXT("%s\\%s"), pszCurrentDirectory, 
						pszFilename);
			if (SUCCEEDED(hr))
			{
				hr = SHGetPropertyStoreFromParsingName(pszNewFilename,  NULL, gpsfFlags, IID_PPV_ARGS(ppPropertyStore));
				if (FAILED(hr))
				{
					// Cannot find the file, return with failure
					wprintf(TEXT("Error %x: Cannot find the file.\n"), hr);
					return hr;
				}
			}
		}
	}
	return hr;	
}
