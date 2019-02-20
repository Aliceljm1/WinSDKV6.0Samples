#include "dialog.h"
#include "strsafe.h"

HRESULT GetFilenameByCLSID(const GUID *pGUID, WCHAR *szFile, size_t cch)
{
	const DWORD GUID_STR_LEN = 40;

	WCHAR szGUID[GUID_STR_LEN];


	if (0 == StringFromGUID2(*pGUID, szGUID, GUID_STR_LEN))
	{
		return E_FAIL;
	}
	else
	{
		return GetFilenameByCLSIDString(szGUID, szFile, cch);
	}
}


HRESULT GetFilenameByCLSIDString(const WCHAR *szGUID, WCHAR *szFile, size_t cch)
{
    HRESULT hr;

	const DWORD STR_LEN = 512;

	WCHAR szKey[STR_LEN];


	int rc=0;

	// Create key name for reading filename from registry
	hr = StringCchPrintf(
		szKey, 
		STR_LEN, 
		L"Software\\Classes\\CLSID\\%s\\InprocServer32\0",
		szGUID);

	if (SUCCEEDED(hr))
	{

        HKEY hkeyFilter=0;
        DWORD dwSize=MAX_PATH;
        BYTE szFilename[MAX_PATH];

        // Open the CLSID key that contains information about the filter
        rc = RegOpenKey(HKEY_LOCAL_MACHINE, szKey, &hkeyFilter);
        if (rc == ERROR_SUCCESS)
        {
            rc = RegQueryValueEx(hkeyFilter, NULL,  // Read (Default) value
                                 NULL, NULL, szFilename, &dwSize);
           
            if (rc == ERROR_SUCCESS)
			{
                hr = StringCchPrintf(szFile, cch, L"%s\0", szFilename);
			}
			else
			{
				hr = E_FAIL;
			}
            RegCloseKey(hkeyFilter);
        }
		else
		{
			hr = E_FAIL;
		}

    }
	return hr;
}
