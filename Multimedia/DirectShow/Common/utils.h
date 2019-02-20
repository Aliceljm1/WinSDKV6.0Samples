#pragma once

// Return the name of the COM DLL associated with a given CLSID
HRESULT GetFilenameByCLSID(const GUID *pGUID, WCHAR *szFile, size_t cch);

// Return the name of the COM DLL associated with a given CLSID string.
// (Same as above but takes the CLSID in standard string form.)
HRESULT GetFilenameByCLSIDString(const WCHAR *szGUID, WCHAR *szFile, size_t cch);
