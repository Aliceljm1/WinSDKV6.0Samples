// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include "propertyhelper.h"

#define ISFLAG(pszArg) ((pszArg)[0] == TEXT('-') || \
                        (pszArg)[0] == TEXT('/'))

bool fVerbose = FALSE;	
bool fEnum = FALSE;
PWSTR pszPropertyName = NULL, pszFilename = NULL, pszPropertyValue = NULL;
OperationType operationType;
 

void Usage(LPTSTR pszAppName)
{
    wprintf(TEXT("Usage: %s [OPTIONS] [Filename] \n"), pszAppName);
    wprintf(TEXT("\n"));
    wprintf(TEXT("Options:\n"));
    wprintf(TEXT(" -get <PropertyName>   Get the value for the property defined\n"));
    wprintf(TEXT("                       by its Canonical Name in <propertyName>\n"));
    wprintf(TEXT(" -set <PropertyName>   Set the value for the property defined\n"));
    wprintf(TEXT("      <PropertyValue>	 by <PropertyName> with value <PropertyValue>\n"));
    wprintf(TEXT(" -enum                 Enumerate all the properties.\n"));
	wprintf(TEXT(" -info <PropertyName>  Get schema information on property.\n"));
    wprintf(TEXT("\n"));
    wprintf(TEXT("Examples:\n"));
    wprintf(TEXT("PropertyEdit -get \"System.Author\" foo.jpg\n"));
	wprintf(TEXT("PropertyEdit -set \"System.Author\" \"John Doe\" foo.jpg\n"));
	wprintf(TEXT("PropertyEdit -enum foo.jpg\n"));
	wprintf(TEXT("PropertyEdit -info \"System.Author\" \n"));
}

HRESULT ParseInputOptions(int argc, _TCHAR* argv[])
{
	HRESULT hr = (argc > 1 && argv) ? S_OK : E_INVALIDARG;
	
	for(int i = 1; SUCCEEDED(hr) && (i < argc); i++)
	{
		PWSTR pszArg = argv[i];
		if(ISFLAG(pszArg))
		{
			/* skip - or / */
			pszArg++;

			if(!_wcsicmp(pszArg, TEXT("?")))
            {
                Usage(argv[0]);
            }
			
			if(!_wcsicmp(pszArg, TEXT("get")))
			{
				operationType = Get;	

				pszPropertyName = argv[++i];
				pszFilename = argv[++i];
			}
			
			if(!_wcsicmp(pszArg, TEXT("enum")))
			{
				operationType = Enumerate;

				pszFilename = argv[++i];
			}
			
			if(!_wcsicmp(pszArg, TEXT("set")))
			{
				operationType = Set;	

				pszPropertyName = argv[++i];
				pszPropertyValue = argv[++i];	

				pszFilename = argv[++i];
			}

			if (!_wcsicmp(pszArg, TEXT("info")))
			{
				operationType = Info;

				pszPropertyName = argv[++i];
			}
		}
	}

	switch (operationType)
	{
		case Get:
			hr = (pszPropertyName) ? S_OK : E_INVALIDARG;
			if (FAILED(hr))
			{
				wprintf(TEXT("No property canonical name specified. \n"));
			}
			break;
		case Set:
			hr = (pszPropertyName && pszPropertyValue) ? S_OK : E_INVALIDARG;
			if (FAILED(hr))
			{
				wprintf(TEXT("No property canonical name or property value specified. \n"));
			}
			break;
		case Info:
			hr = (pszPropertyName) ? S_OK : E_INVALIDARG;
			if (FAILED(hr))
			{
				wprintf(TEXT("No property canonical name specified. \n"));
			}
			break;
		case Enumerate:
		default:
			break;
	}

	return hr;
}



int _tmain(int argc, _TCHAR* argv[])
{
	// Initialize COM
	HRESULT hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		hr = ParseInputOptions(argc, argv);

		if (SUCCEEDED(hr))
		{		
			if (SUCCEEDED(hr))
			{
				switch (operationType)
				{
				case Get:
					hr = PropertyHelper::GetPropertyValue(pszFilename, pszPropertyName);
					break;
				case Enumerate:
					hr = PropertyHelper::EnumerateProperties(pszFilename);
					break;
				case Set:
					hr = PropertyHelper::SetPropertyValue(pszFilename, pszPropertyName, pszPropertyValue);
					break;
				case Info:
					hr = PropertyHelper::GetPropertyDescription(pszPropertyName);
					break;
				default :
					wprintf(TEXT("No operation specified"));
					Usage(argv[0]);
					break;
				}
			}
		}
		else
		{
			Usage(argv[0]);
		}

		CoUninitialize();
	}
	return 0;
}