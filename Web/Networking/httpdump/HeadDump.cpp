
//=--------------------------------------------------------------------------=
//  (C) Copyright 1996-2000 Microsoft Corporation. All Rights Reserved.
//=--------------------------------------------------------------------------=
/****************************************************************************
*
* HeadDump: requests the default HTML document from the server and prints
*			along with HTTP headers.
*			Microsoft Corporation (C). L.B.
*	
* PURPOSE:
* This sample demonstrates how to create and submit HTTP
* request. Sample requests the default HTML document from the
* server and then display it along with the HTTP transaction headers.
* This sample also allows to access password protected pages. It
* checks for HTTP server response code and it is "401 Access Denied"
* it asks password and user name and then resubmit request.

*
* This sample features:
*  - SSL/PCT support
*  - NTLM Authentication (when IE 3.0 or above is installed)
*  - Basic Authentication
*  - Generating standard user name/password UI to get user credentials.
*  - Getting user name and password without UI
*  - getting any document of any MIME type
*  - translation of Internet error codes to the extended error messages
*  - ability to save remote object to a local file (when no local file
*	 specify output goes to STDOUT).
*
* Usage:
*   headdump.exe -h host [-o object] [-p proxy] [-u] [-s] [-?] [-f file]
*		-h host is a HTTP server such as www.server.com
*		-p proxy is optional proxy, in form: http://proxy:80
*		-o object is optional document to GET, in form: /foobar.htm
*		-u use this flag to bring user name and password UI (optional)
*		-f save remote object as local file  (optional)
*		-? get help
*		-s use SSL (optional)
*
* Samples:
* To get help on command line parameters:
*	C:> headdump.exe -?	
*	
* Note that HeadDump can also invoke ISAPI dll or CGI like this:
* 	C:> headdump.exe -s www.server.name -o /scripts/myisapi.dll
*
* Since sample generates large output, you may want to pipe to more:
* 	C:> headdump.exe -s www.server.name -o /scripts/myisapi.dll  | more
*
* To save remote HTML object as file:
*	C:> headdump.exe -s www.server.name -o /exe_files/foo.exe -f foo.exe
*
*
*
****************************************************************************/

#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <tchar.h>
// Use safe string APIs.
#include <strsafe.h>

#if defined(_UNICODE)
# define _tcout wcout
# define _tcin wcin
# define _tcerr wcerr
#else
# define _tcout cout
# define _tcin cin
# define _tcerr cerr
#endif

using namespace std;

BOOL ErrorOut  ( DWORD dError, TCHAR * szCallingFunc);
BOOL ParseParams (int argc , TCHAR ** argv, TCHAR* szServer, TCHAR* szObject,
                  TCHAR* szProxy, TCHAR * szFile, BOOL& bUi, BOOL& bSSL);
void Usage (TCHAR *szName);

int _tmain (int argc, TCHAR *argv[])
{
 HINTERNET hOpen, hConnect, hReq;
 DWORD  dwSize, dwCode;
 DWORD  dwFlags = INTERNET_FLAG_RELOAD |
                  INTERNET_FLAG_NO_CACHE_WRITE |
                  INTERNET_FLAG_KEEP_CONNECTION;
 CHAR *lpBuffer, szData[51];
 TCHAR szServer[256], szObject [256], szProxy [256], szFile [256];
 BOOL bSSL, bUi;
 HANDLE hFile = NULL;
 INTERNET_PORT dwPort;
 TCHAR szAccept[] = TEXT("*/*");
 LPTSTR AcceptTypes[2]={0};


 AcceptTypes[0]=szAccept;



if (!ParseParams ( argc , argv, szServer, szObject,
                  szProxy,  szFile, bUi, bSSL))
    return (FALSE);

if (szFile [0])
{
	// Open local file
	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, FALSE};
	if  ( !(hFile = CreateFile (szFile,
		                        GENERIC_WRITE,
								FILE_SHARE_READ,
								&sa,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, NULL) ))
	{
		_tcerr << TEXT("Can't open file: ") << szFile << TEXT(" Error: ") << GetLastError() << endl;
		return 0;
	}
}



if (szProxy [0] == NULL)
{
   // No proxy was specified
   if ( !(hOpen = InternetOpen ( TEXT("HeadDump"),  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) )
   {
       ErrorOut ( GetLastError(), TEXT("InternetOpen"));
       return 0;
   }
 }
 else
 {
   // Use proxy server
   if ( !(hOpen = InternetOpen ( TEXT("HeadDump"),  CERN_PROXY_INTERNET_ACCESS, szProxy, NULL, 0) ) )
   {
        ErrorOut ( GetLastError(), TEXT("InternetOpen"));
       return 0;
   }
 }


if  (bSSL)
{
    dwPort = INTERNET_DEFAULT_HTTPS_PORT;
    dwFlags |= INTERNET_FLAG_SECURE |
               INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
               INTERNET_FLAG_IGNORE_CERT_DATE_INVALID  ;
}
else
    dwPort = INTERNET_INVALID_PORT_NUMBER;

if ( !(hConnect = InternetConnect ( hOpen, szServer , dwPort, TEXT(""),  TEXT(""), INTERNET_SERVICE_HTTP, 0, 0) ) )
{
    ErrorOut (GetLastError(), TEXT("InternetConnect"));
    return 0;
}

if ( !(hReq = HttpOpenRequest (hConnect, TEXT("GET"), szObject, HTTP_VERSION, TEXT(""),
                                (LPCTSTR*) AcceptTypes, dwFlags ,0 )))
{
    ErrorOut (GetLastError(), TEXT("HttpOpenRequest"));
    return 0;
}


again:
if ( !HttpSendRequest (hReq, // handle from HttpOpenRequest
                                             NULL, 0, NULL, 0) )
{
       ErrorOut (GetLastError(), TEXT("HttpSendRequest"));

       dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA                 |
                           SECURITY_FLAG_IGNORE_REVOCATION        |
                           SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP  |
                           SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
                           SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                           SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

       if ( !InternetSetOption (hReq,
                                INTERNET_OPTION_SECURITY_FLAGS,
                                &dwFlags,
                                sizeof (dwFlags) ) )
       {
          _tcerr << TEXT("InternetSetOptionFailed: ") << GetLastError() << endl;
          return FALSE;
       }
}

if ( !HttpSendRequest (hReq, NULL, 0, NULL, 0) )
{
       ErrorOut (GetLastError(), TEXT("HttpSendRequest"));
       return FALSE;
}

dwSize = sizeof (DWORD) ;
if ( !HttpQueryInfo (hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL))
{
	ErrorOut (GetLastError(), TEXT("HttpQueryInfo"));
	return FALSE;
}

if ( dwCode == HTTP_STATUS_DENIED || dwCode == HTTP_STATUS_PROXY_AUTH_REQ)
{
    // We have to read all outstanding data on the Internet handle
    // before we can resubmit request. Just discard the data.
    do
	{
		InternetReadFile (hReq, (LPVOID)szData, 50, &dwSize);
	}
	while (dwSize != 0);

    if (!bUi)
    {
        // This is a secure page.
        _tcerr << TEXT("This page is password protected. ") << endl;
      TCHAR szUser[50]=TEXT("");
      TCHAR szPass[50]=TEXT("");

    	wcerr << TEXT("User: ");
    	_tcin  >> szUser;
    	_tcerr << TEXT("Pass: ");
    	_tcin  >> szPass;
        if ( !InternetSetOption (hConnect, INTERNET_OPTION_USERNAME, (LPVOID) szUser, lstrlen (szUser) ))
        {
            _tcerr << TEXT("InternetSetOptionFailed: ") << GetLastError() << endl;
            return FALSE;
        }

        if ( !InternetSetOption (hConnect, INTERNET_OPTION_PASSWORD, (LPVOID) szPass, lstrlen (szPass) ))
        {
            _tcerr << TEXT("InternetSetOptionFailed: ") << GetLastError() << endl;
            return FALSE;
        }
        goto again;
    }
    else
    {
        if ( InternetErrorDlg (GetDesktopWindow(),
                               hReq,
                               ERROR_INTERNET_INCORRECT_PASSWORD,
                               FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
                               FLAGS_ERROR_UI_FLAGS_GENERATE_DATA |
                               FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
                               NULL) == ERROR_INTERNET_FORCE_RETRY)
            goto again;
    }
	
}


// First time we will find out the size of the headers.
HttpQueryInfo (hReq,HTTP_QUERY_RAW_HEADERS_CRLF,  NULL, &dwSize, NULL);
lpBuffer =  new char [dwSize + 1 ];

// Now we call HttpQueryInfo again to get the headers.
if (!HttpQueryInfo (hReq,HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID) lpBuffer,
    &dwSize, NULL))
{
	ErrorOut (GetLastError(), TEXT("HttpQueryInfo"));
	return FALSE;
}
*(lpBuffer + dwSize) = '\0';
cout << lpBuffer << endl;


do
{
	if (!InternetReadFile (hReq, (LPVOID)szData, 50, &dwSize) )
	{
	 	ErrorOut (GetLastError (), TEXT("InternetReadFile"));
		return FALSE;
	}
	if (dwSize != 0)
	{
		if (!hFile)
		{
			// dump text to the screen
			szData[dwSize] = '\0';
			cout << szData;
		}
		else
		{
			DWORD dwWritten;
			if (!WriteFile(hFile,  szData, dwSize,  &dwWritten,  NULL))
      {
        CloseHandle (hFile);
        _tcerr << TEXT("WriteFile to ") << szFile << TEXT(" failed: ") <<
        GetLastError () << endl;
        return FALSE;
      }
		}
	}
}
while (dwSize);

_tcout << endl;

if (!InternetCloseHandle (hReq) )
{
    ErrorOut (GetLastError (), TEXT("CloseHandle on hReq"));
    return FALSE;
}
if (!InternetCloseHandle (hConnect) )
{
    ErrorOut (GetLastError (), TEXT("CloseHandle on hConnect"));
    return FALSE;
}
if (!InternetCloseHandle (hOpen) )
{
    ErrorOut (GetLastError (), TEXT("CloseHandle on hOpen"));
    return FALSE;
}


delete [] lpBuffer;

if (hFile)
{
	FlushFileBuffers (hFile);
	CloseHandle (hFile);
}
return TRUE;
}

/****************************************************************************
*
*    FUNCTION: ErrorOut
*
*    PURPOSE: This function is used to get extended Internet error.
*
*    COMMENTS:  Function returns TRUE on success and FALSE on failure.
*
****************************************************************************/

BOOL ErrorOut ( DWORD dError, TCHAR * szCallFunc)
{
    #define SZTEMP_SIZE 100
    TCHAR szTemp[SZTEMP_SIZE] = TEXT(""), *szBuffer=NULL, *szBufferFinal = NULL;
    char strName[256]="";
    DWORD  dwIntError , dwLength = 0, dwFinalLength;

    FormatMessage(FORMAT_MESSAGE_FROM_HMODULE,
 	              GetModuleHandle(TEXT("wininet.dll")),dError,0,
                  (LPTSTR)strName,256,NULL);

    StringCchPrintf( szTemp, SZTEMP_SIZE, TEXT("%s error code: %d\nMessage: %s\n"),
		             szCallFunc, dError, strName);
	
	if (dError == ERROR_INTERNET_EXTENDED_ERROR)
	{
		InternetGetLastResponseInfo (&dwIntError, NULL, &dwLength);
		if (dwLength)
		{
			if ( !(szBuffer = (TCHAR *) LocalAlloc ( LPTR,  dwLength) ) )
			{
				StringCchCat(szTemp, SZTEMP_SIZE, TEXT ( "Unable to allocate memory to display Internet error code. Error code: "));
				StringCchCat(szTemp, SZTEMP_SIZE, _itot(GetLastError(), szBuffer, 10));
				StringCchCat(szTemp, SZTEMP_SIZE, TEXT ("\n") );
				_tcerr << szTemp << endl;
				return FALSE;
			}
			if (!InternetGetLastResponseInfo (&dwIntError, (LPTSTR) szBuffer, &dwLength))
			{
				StringCchCat(szTemp, SZTEMP_SIZE, TEXT ( "Unable to get Internet error. Error code: ") );
				StringCchCat(szTemp, SZTEMP_SIZE, _itot (GetLastError(), szBuffer, 10));
				StringCchCat(szTemp, SZTEMP_SIZE, TEXT ("\n") );
				_tcerr << szTemp << endl;
				return FALSE;
			}

			size_t sizeBuffer;
			size_t sizeTemp;
			if( FAILED(StringCchLength( szBuffer, dwLength, &sizeBuffer ) )||
				FAILED(StringCchLength( szTemp, SZTEMP_SIZE, &sizeTemp ) ) )
			{
				_tcerr << TEXT("Unable to determine buffer sizes.") << endl;
				return FALSE;
			}

			dwFinalLength = sizeBuffer + sizeTemp + 1;
			if ( !(szBufferFinal = (TCHAR *) LocalAlloc ( LPTR,  dwFinalLength)))
			{
				StringCchCat(szTemp, SZTEMP_SIZE, TEXT ( "Unable to allocate memory. Error code: ") );
				StringCchCat(szTemp, SZTEMP_SIZE, _itot (GetLastError(), szBuffer, 10));
				StringCchCat(szTemp, SZTEMP_SIZE, TEXT ("\n") );
				_tcerr << szTemp << endl;
				return FALSE;
			}
			StringCchCopy(szBufferFinal, dwFinalLength, szTemp);
			StringCchCat(szBufferFinal, dwFinalLength, szBuffer);
			LocalFree (szBuffer);
			_tcerr <<  szBufferFinal  << endl;
			LocalFree (szBufferFinal);
		}
	}
	else
    _tcerr << szTemp << endl;

    return TRUE;
}


BOOL ParseParams (int argc , TCHAR ** argv, TCHAR* szServer, TCHAR* szObject,
                  TCHAR* szProxy, TCHAR * szFile, BOOL& bUi, BOOL& bSSL)
{
    int i = 1;
    BOOL bServer = FALSE;

    szProxy[0]  = NULL, bUi = FALSE, bSSL = FALSE, szFile[0]  = NULL;
    StringCchCopy(szObject, 256, TEXT(""));

    while (i < argc)
    {
       if ( *argv[i] != '-')
       {
           Usage (argv[0]);
           return FALSE;
       }
       switch (argv[i][1])
       {
           case 'H':
           case 'h':
              StringCchCopy(szServer, 256, argv[i+1]);
              i += 2;
              bServer = TRUE;
           break;
           case 'o':
           case 'O':
              StringCchCopy(szObject, 256, argv[i+1]);
              i += 2;
           break;

           case 'p':
           case 'P':
              StringCchCopy(szProxy, 256, argv[i+1]);
              i += 2;
           break;

           case 'u':
           case 'U':
              bUi = TRUE;
              i++;
           break;

           case 's':
           case 'S':
              bSSL = TRUE;
              i++;
           break;

           case 'f':
           case 'F':
              StringCchCopy(szFile, 256, argv[i+1]);
              i += 2;
           break;
		
		   case '?':
			   Usage (argv[0]);
			   return FALSE;
		   break;
       default:
           _tcerr << TEXT("Invalid command line argument: ")<< argv[i] << endl;
           Usage (argv[0]);
           return FALSE;
        }
    }
    if ( bServer )
        return TRUE;
    else
    {
        Usage(argv[0]);
        return FALSE;
    }
}


void Usage (TCHAR *szName)
{
    _tcerr << TEXT("Usage: " << szName <<" -h host [-o object] [-p proxy] [-u] [-s] [-?] [-f file]") << endl;
    _tcerr << TEXT("\t-h host is a HTTP server such as www.server.com") << endl;
    _tcerr << TEXT("\t-p proxy is optional proxy in form: http://proxy:80") << endl;
    _tcerr << TEXT("\t-o object is optional document to GET in form: /foobar.htm") << endl;
    _tcerr << TEXT("\t-u use this flag to bring user name and password UI ") << endl;
    _tcerr << TEXT("\t-f save remote object as local file ") << endl;
    _tcerr << TEXT("\t-? get help ") << endl;
    _tcerr << TEXT("\t-s use SSL ") << endl;
}
