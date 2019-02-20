
//
// BTHCxnDemo.cpp - Simple Bluetooth application using Winsock 2.2 and RFCOMM protocol
//
//      This is a part of the Microsoft Source Code Samples.
//      Copyright 1996 - 2002 Microsoft Corporation.
//      All rights reserved.
//      This source code is only intended as a supplement to
//      Microsoft Development Tools and/or WinHelp documentation.
//      See these sources for detailed information regarding the
//      Microsoft samples programs.
//
//      This example demonstrates how to use Winsock-2.2 APIs to connect
//      between two remote bluetooth devices and transfer data between them.
//      This example only supports address family of type AF_BTH,
//      socket of type SOCK_STREAM, protocol of type BTHPROTO_RFCOMM.
//
//      Once this source code is built, the resulting application can be
//      run either in server mode or in client mode.  See command line help
//      for command-line-examples and detailed explanation about all options.
//


#include <stdio.h>
#include <initguid.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <strsafe.h>


// {B62C4E8D-62CC-404b-BBBF-BF3E3BBB1374}
DEFINE_GUID(g_guidServiceClass, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);

//
// Test data handling is not UNICODE-ready
//
#define CXN_TEST_DATA_STRING ("~!@#$%^&*()-_=+?<>1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define CXN_TRANSFER_DATA_LENGTH          sizeof(CXN_TEST_DATA_STRING)

#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CXN_SUCCESS                       0
#define CXN_ERROR                         1

char g_szRemoteName[BTH_MAX_NAME_SIZE + 1] = {0};  // 1 extra for trailing NULL character
char g_szRemoteAddr[CXN_BDADDR_STR_LEN + 1] = {0}; // 1 extra for trailing NULL character
int  g_ulMaxCxnCycles = 1, g_iOutputLevel = 0;

ULONG NameToBthAddr(IN const char * pszRemoteName, OUT BTH_ADDR * pRemoteBthAddr);
ULONG AddrStringToBtAddr(IN const char * pszRemoteAddr, OUT BTH_ADDR * pRemoteBtAddr);
ULONG RunClientMode(IN ULONGLONG ululRemoteBthAddr, IN int iMaxCxnCycles = 1);
ULONG RunServerMode(IN int iMaxCxnCycles = 1);
void  ShowCmdLineHelp(void);
ULONG ParseCmdLine(IN int argc, IN char * argv[]);


int _cdecl main(int argc, char *argv[])
{
    ULONG 		ulRetCode = 0;
    WSADATA		WSAData = {0};
    ULONGLONG 	ululRemoteBthAddr = 0;

    //
    // Parse the command line
    //
    ulRetCode = ParseCmdLine(argc, argv);
    if ( 0 == ulRetCode ) {
        //
        // Ask for Winsock version 2.2.
        //
        ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData);
        if ( 0 != ulRetCode ) { // "zero" per SDK
            printf("-FATAL- | Unable to initialize Winsock version 2.2\n");
            goto CleanupAndExit;
        }

        //
        // Note, this app "prefers" the name if provided, but it is app-specific
        // Other applications may provide more generic treatment.
        //
        if ( '\0' != g_szRemoteName[0] ) {
            //
            // Get address from name of the remote device and run the application
            // in client mode
            //
            ulRetCode = NameToBthAddr(g_szRemoteName, (BTH_ADDR *) &ululRemoteBthAddr);
            if ( CXN_SUCCESS != ulRetCode ) {
                printf("-FATAL- | Unable to get address of the remote radio having name %s\n", g_szRemoteName);
                goto CleanupAndExit;
            }

            ulRetCode = RunClientMode(ululRemoteBthAddr, g_ulMaxCxnCycles);
        } else if ( '\0' != g_szRemoteAddr[0] ) {
            //
            // Get address from formated address-string of the remote device and
            // run the application in client mode
            //
            // Optional use of this private locally defined function, prefer
            // WSAStringToAddress() as below
            // ulRetCode = AddrStringToBtAddr(g_szRemoteAddr, (BTH_ADDR *) &ululRemoteBthAddr);
            //
            int iAddrLen = sizeof(ululRemoteBthAddr);
            ulRetCode = WSAStringToAddress(g_szRemoteAddr,
                                           AF_BTH,
                                           NULL,
                                           (LPSOCKADDR)&ululRemoteBthAddr,
                                           &iAddrLen);
            if ( 0 != ulRetCode ) {
                printf("-FATAL- | Unable to get address of the remote radio having formated address-string %s\n", g_szRemoteAddr);
                goto CleanupAndExit;
            }

            ulRetCode = RunClientMode(ululRemoteBthAddr, g_ulMaxCxnCycles);
        } else {
            //
            // No remote name/address specified.  Run the application in server mode
            //
            ulRetCode = RunServerMode(g_ulMaxCxnCycles);
        }
    } else if ( 1 == ulRetCode ) {
        //
        // Command line syntax error.  Display cmd line help
        //
        ShowCmdLineHelp();
    } else {
        printf("-FATAL- | Error in parsing command line\n");
    }

    CleanupAndExit:
    return(int)ulRetCode;
}


//
// TODO: use inquiry timeout SDP_DEFAULT_INQUIRY_SECONDS
//

//
// NameToBthAddr converts a bluetooth device name to a bluetooth address,
// if required by performing inquiry with remote name requests.
// This function demonstrates device inquiry, with optional LUP flags.
//
ULONG NameToBthAddr(IN const char * pszRemoteName, OUT BTH_ADDR * pRemoteBtAddr)
{
    INT				iResult = 0, iRetryCount = 0;
    BOOL			bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
    ULONG			ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
    HANDLE			hLookup = 0;
    PWSAQUERYSET	pWSAQuerySet = NULL;

    if ( ( NULL == pszRemoteName ) || ( NULL == pRemoteBtAddr ) ) {
        goto CleanupAndExit;
    }
    pWSAQuerySet = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(),
                                            HEAP_ZERO_MEMORY,
                                            ulPQSSize);
    if ( NULL == pWSAQuerySet ) {
        printf("!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
        goto CleanupAndExit;
    }

    //
    // Search for the device with the correct name
    //
    for ( iRetryCount = 0;
        !bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY);
        iRetryCount++ ) {
        //
        // WSALookupService is used for both service search and device inquiry
        // LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
        //
        ulFlags = LUP_CONTAINERS;

        //
        // Friendly device name (if available) will be returned in lpszServiceInstanceName
        //
        ulFlags |= LUP_RETURN_NAME;

        //
        // BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
        //
        ulFlags |= LUP_RETURN_ADDR;

        if ( 0 == iRetryCount ) {
            printf("*INFO* | Inquiring device from cache...\n");
        } else {
            //
            // Flush the device cache for all inquiries, except for the first inquiry
            //
            // By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
            // a fresh lookup instead of pulling the information from device cache.
            //
            ulFlags |= LUP_FLUSHCACHE;

            //
            // Pause for some time before all the inquiries after the first inquiry
            //
            // Remote Name requests will arrive after device inquiry has
            // completed.  Without a window to receive IN_RANGE notifications,
            // we don't have a direct mechanism to determine when remote
            // name requests have completed.
            //
            printf("*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
            Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

            printf("*INFO* | Inquiring device ...\n");
        }

        //
        // Start the lookup service
        //
        iResult = 0;
        hLookup = 0;
        bContinueLookup = FALSE;
        ZeroMemory(pWSAQuerySet, ulPQSSize);
        pWSAQuerySet->dwNameSpace = NS_BTH;
        pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
        iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

        if ( (NO_ERROR == iResult) && (NULL != hLookup) ) {
            bContinueLookup = TRUE;
        } else if ( 0 < iRetryCount ) {
            printf("=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSALastError = %d\n", iResult, WSAGetLastError());
            goto CleanupAndExit;
        }

        while ( bContinueLookup ) {
            //
            // Get information about next bluetooth device
            //
            // Note you may pass the same WSAQUERYSET from LookupBegin
            // as long as you don't need to modify any of the pointer
            // members of the structure, etc.
            //
            // ZeroMemory(pWSAQuerySet, ulPQSSize);
            // pWSAQuerySet->dwNameSpace = NS_BTH;
            // pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
            if ( NO_ERROR == WSALookupServiceNext(hLookup,
                                                  ulFlags,
                                                  &ulPQSSize,
                                                  pWSAQuerySet) ) {
                //
                // Since we're a non-unicode application, the remote
                // name in lpszServiceInstanceName will have been converted
                // from CP_UTF8 to CP_ACP, this may cause the name match
                // to fail unexpectedly.  If the app is to handle this,
                // the app needs to be unicode.
                //
                if ( ( pWSAQuerySet->lpszServiceInstanceName != NULL ) &&
                     ( 0==_stricmp(pWSAQuerySet->lpszServiceInstanceName, pszRemoteName) ) ) {
                    //
                    // Found a remote bluetooth device with matching name.
                    // Get the address of the device and exit the lookup.
                    //
                    CopyMemory(pRemoteBtAddr,
                               &((PSOCKADDR_BTH) pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr,
                               sizeof(*pRemoteBtAddr));
                    bRemoteDeviceFound = TRUE;
                    bContinueLookup = FALSE;
                }
            } else {
                if ( WSA_E_NO_MORE == ( iResult = WSAGetLastError() ) ) { //No more data
                    //
                    // No more devices found.  Exit the lookup.
                    //
                    bContinueLookup = FALSE;
                } else if ( WSAEFAULT == iResult ) {
                    //
                    // The buffer for QUERYSET was insufficient.
                    // In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
                    // the required size.  So we can use this parameter to reallocate memory for QUERYSET.
                    //
                    HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
                    pWSAQuerySet = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(),
                                                            HEAP_ZERO_MEMORY,
                                                            ulPQSSize);
                    if ( NULL == pWSAQuerySet ) {
                        printf("!ERROR! | Unable to allocate memory for WSAQERYSET\n");
                        bContinueLookup = FALSE;
                    }
                } else {
                    printf("=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);
                    bContinueLookup = FALSE;
                }
            }
        }

        //
        // End the lookup service
        //
        WSALookupServiceEnd(hLookup);
    }

    CleanupAndExit:
    if ( NULL != pWSAQuerySet ) {
        HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
        pWSAQuerySet = NULL;
    }

    if ( bRemoteDeviceFound ) {
        return(CXN_SUCCESS);
    } else {
        return(CXN_ERROR);
    }
}

//
// Convert a formatted BTH address string to populate a BTH_ADDR (actually a ULONGLONG)
//
// Note: this is an illustration, prefer to use the Winsock library function
//      WSAStringToAddress
//
ULONG AddrStringToBtAddr(IN const char * pszRemoteAddr, OUT BTH_ADDR * pRemoteBtAddr)
{
    ULONG		ulAddrData[6], ulRetCode = CXN_SUCCESS;
    BTH_ADDR	BtAddrTemp = 0;

    if ( ( NULL == pszRemoteAddr ) || ( NULL == pRemoteBtAddr ) ) {
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    *pRemoteBtAddr = 0;

    //
    // Populate a 6 membered array of unsigned long integers
    // by parsing the given address in string format
    //
    sscanf_s(pszRemoteAddr,
           "%02x:%02x:%02x:%02x:%02x:%02x",
           &ulAddrData[0],&ulAddrData[1],&ulAddrData[2],&ulAddrData[3],&ulAddrData[4],&ulAddrData[5]);

    //
    // Construct a BTH_ADDR from the 6 integers stored in the array
    //
    for ( int i=0; i<6; i++ ) {
        //
        // Extract data from the first 8 lower bits.
        //
        BtAddrTemp = (BTH_ADDR)( ulAddrData[i] & 0xFF );

        //
        // Push 8 bits to the left
        //
        *pRemoteBtAddr = ( (*pRemoteBtAddr) << 8 ) + BtAddrTemp;
    }

    CleanupAndExit:
    return(ulRetCode);
}

//
// RunClientMode runs the application in client mode.  It opens a socket, connects it to a
// remote socket, transfer some data over the connection and closes the connection.
//
ULONG RunClientMode(ULONGLONG ululRemoteAddr, int iMaxCxnCycles)
{
    ULONG			ulRetCode = CXN_SUCCESS;
    int				iCxnCount = 0;
    char			*pszData = NULL;
    SOCKET			LocalSocket = INVALID_SOCKET;
    SOCKADDR_BTH	SockAddrBthServer = {0};
    HRESULT			res;

    pszData = (char *) HeapAlloc(GetProcessHeap(),
                                 HEAP_ZERO_MEMORY,
                                 CXN_TRANSFER_DATA_LENGTH);
    if ( NULL == pszData ) {
        printf("=CRITICAL= | HeapAlloc failed | out of memory, gle = [%d] \n", GetLastError());
        return(CXN_ERROR);
    }
    //
    // Setting address family to AF_BTH indicates winsock2 to use Bluetooth sockets
    // Port should be set to 0 if ServiceClassId is spesified.
    //
    SockAddrBthServer.addressFamily = AF_BTH;
    SockAddrBthServer.btAddr = (BTH_ADDR) ululRemoteAddr;
    SockAddrBthServer.serviceClassId = g_guidServiceClass;
    SockAddrBthServer.port = 0;

    //
    // Create a static data-string, which will be transferred to the remote
    // Bluetooth device
    //
    res = StringCchCopyN(pszData, CXN_TRANSFER_DATA_LENGTH, CXN_TEST_DATA_STRING, CXN_TRANSFER_DATA_LENGTH-1);
    if ( FAILED(res) ) {
        printf("=CRITICAL= | Creating a static data string failed\n");
        return(CXN_ERROR);
    }
    pszData[CXN_TRANSFER_DATA_LENGTH] = 0;

    //
    // Run the connection/data-transfer for user specified number of cycles
    //
    for ( iCxnCount = 0;
        (0 == ulRetCode) && (iCxnCount < iMaxCxnCycles || iMaxCxnCycles == 0);
        iCxnCount++ ) {

        printf("\n");

        //
        // Open a bluetooth socket using RFCOMM protocol
        //
        LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
        if ( INVALID_SOCKET == LocalSocket ) {
            printf("=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
            ulRetCode = CXN_ERROR;
            goto CleanupAndExit;
        }

        if ( 2 <= g_iOutputLevel ) {
            printf("*INFO* | socket() call succeeded. Socket = [0x%X]\n", LocalSocket);
        }

        if ( 1 <= g_iOutputLevel ) {
            printf("*INFO* | connect() attempt with Remote BTHAddr = [0x%X]\n", ululRemoteAddr);
        }

        //
        // Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
        //
        if ( SOCKET_ERROR == connect(LocalSocket,
                                     (struct sockaddr *) &SockAddrBthServer,
                                     sizeof(SOCKADDR_BTH)) ) {
            printf("=CRITICAL= | connect() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
            ulRetCode = CXN_ERROR;
            goto CleanupAndExit;
        }

        if ( 2 <= g_iOutputLevel ) {
            printf("*INFO* | connect() call succeeded\n");
        }

        //
        // send() call indicates winsock2 to send the given data
        // of a specified length over a given connection.
        //
        printf("*INFO* | Sending following data string:\n%s\n", pszData);
        if ( SOCKET_ERROR == send(LocalSocket,
                                  pszData,
                                  CXN_TRANSFER_DATA_LENGTH,
                                  0) ) {
            printf("=CRITICAL= | send() call failed w/socket = [0x%X], szData = [%p], dataLen = [%d]. WSAGetLastError=[%d]\n", LocalSocket, pszData, CXN_TRANSFER_DATA_LENGTH, WSAGetLastError());
            ulRetCode = CXN_ERROR;
            goto CleanupAndExit;
        }

        if ( 2 <= g_iOutputLevel ) {
            printf("*INFO* | send() call succeeded\n");
        }

        //
        // Close the socket
        //
        if ( SOCKET_ERROR == closesocket(LocalSocket) ) {
            printf("=CRITICAL= | closesocket() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
            ulRetCode = CXN_ERROR;
            goto CleanupAndExit;
        }

        LocalSocket = INVALID_SOCKET;

        if ( 2 <= g_iOutputLevel ) {
            printf("*INFO* | closesocket() call succeeded");
        }
    }

    CleanupAndExit:
    if ( INVALID_SOCKET != LocalSocket ) {
        closesocket(LocalSocket);
        LocalSocket = INVALID_SOCKET;
    }

    if ( NULL != pszData ) {
        HeapFree(GetProcessHeap(), 0, pszData);
    }

    return(ulRetCode);
}

//
// RunServerMode runs the application in server mode.  It opens a socket, connects it to a
// remote socket, transfer some data over the connection and closes the connection.
//

#define CXN_INSTANCE_STRING "Sample Bluetooth Server"

ULONG RunServerMode(int iMaxCxnCycles)
{
    ULONG			ulRetCode = CXN_SUCCESS;
    int				iAddrLen = sizeof(SOCKADDR_BTH);
    int				iCxnCount = 0;
    int				iLengthReceived = 0;
    UINT			uiTotalLengthReceived;
    size_t			cbInstanceNameSize;
    char *			pszDataBuffer = NULL;
    char *			pszDataBufferIndex = NULL;
    char *			pszInstanceName = NULL;
    char			szThisComputerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD			dwLenComputerName = MAX_COMPUTERNAME_LENGTH + 1;
    SOCKET			LocalSocket = INVALID_SOCKET;
    SOCKET			ClientSocket = INVALID_SOCKET;
    WSAQUERYSET		wsaQuerySet = {0};
    SOCKADDR_BTH	SockAddrBthLocal = {0};
    LPCSADDR_INFO	lpCSAddrInfo = NULL;
    HRESULT			res;

    //
    // This fixed-size allocation can be on the stack assuming the
    // total doesn't cause a stack overflow (depends on your compiler settings)
    // However, they are shown here as dynamic to allow for easier expansion
    //
    lpCSAddrInfo = (LPCSADDR_INFO) HeapAlloc( GetProcessHeap(),
                                              HEAP_ZERO_MEMORY,
                                              sizeof(CSADDR_INFO) );
    if ( NULL == lpCSAddrInfo ) {
        printf("!ERROR! | Unable to allocate memory for CSADDR_INFO\n");
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    if ( !GetComputerName(szThisComputerName, &dwLenComputerName) ) {
        printf("=CRITICAL= | GetComputerName() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    //
    // Open a bluetooth socket using RFCOMM protocol
    //
    LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if ( INVALID_SOCKET == LocalSocket ) {
        printf("=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    if ( 2 <= g_iOutputLevel ) {
        printf("*INFO* | socket() call succeeded. Socket = [0x%X]\n", LocalSocket);
    }

    //
    // Setting address family to AF_BTH indicates winsock2 to use Bluetooth port
    //
    SockAddrBthLocal.addressFamily = AF_BTH;
    SockAddrBthLocal.port = BT_PORT_ANY;

    //
    // bind() associates a local address and port combination
    // with the socket just created. This is most useful when
    // the application is a server that has a well-known port
    // that clients know about in advance.
    //
    if ( SOCKET_ERROR == bind(LocalSocket,
                              (struct sockaddr *) &SockAddrBthLocal,
                              sizeof(SOCKADDR_BTH) ) ) {
        printf("=CRITICAL= | bind() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    if ( 2 <= g_iOutputLevel ) {
        printf("*INFO* | bind() call succeeded\n");
    }

    ulRetCode = getsockname(LocalSocket,
                            (struct sockaddr *)&SockAddrBthLocal,
                            &iAddrLen);
    if ( SOCKET_ERROR == ulRetCode ) {
        printf("=CRITICAL= | getsockname() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    //
    // CSADDR_INFO
    //
    lpCSAddrInfo[0].LocalAddr.iSockaddrLength = sizeof( SOCKADDR_BTH );
    lpCSAddrInfo[0].LocalAddr.lpSockaddr = (LPSOCKADDR)&SockAddrBthLocal;
    lpCSAddrInfo[0].RemoteAddr.iSockaddrLength = sizeof( SOCKADDR_BTH );
    lpCSAddrInfo[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&SockAddrBthLocal;
    lpCSAddrInfo[0].iSocketType = SOCK_STREAM;
    lpCSAddrInfo[0].iProtocol = BTHPROTO_RFCOMM;

    //
    // If we got an address, go ahead and advertise it.
    //
    ZeroMemory(&wsaQuerySet, sizeof(WSAQUERYSET));
    wsaQuerySet.dwSize = sizeof(WSAQUERYSET);
    wsaQuerySet.lpServiceClassId = (LPGUID) &g_guidServiceClass;

    //
    // Adding a byte to the size to account for the space in the
    // format string in the sprintf call. This will have to change if converted
    // to UNICODE
    //
    res = StringCchLength(szThisComputerName, sizeof(szThisComputerName), &cbInstanceNameSize);
    if( FAILED(res) ) {
        printf("-FATAL- | ComputerName specified is too large\n");
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }
    cbInstanceNameSize += sizeof(CXN_INSTANCE_STRING) + 1;
    pszInstanceName = (LPSTR)HeapAlloc(GetProcessHeap(),
                                       HEAP_ZERO_MEMORY,
                                       cbInstanceNameSize);
    if ( NULL == pszInstanceName ) {
        printf("-FATAL- | HeapAlloc failed | out of memory | gle = [%d] \n", GetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    StringCchPrintf(pszInstanceName, cbInstanceNameSize, "%s %s", szThisComputerName, CXN_INSTANCE_STRING);
    wsaQuerySet.lpszServiceInstanceName = pszInstanceName;
    wsaQuerySet.lpszComment = "Example Service instance registered in the directory service through RnR";
    wsaQuerySet.dwNameSpace = NS_BTH;
    wsaQuerySet.dwNumberOfCsAddrs = 1;      // Must be 1.
    wsaQuerySet.lpcsaBuffer = lpCSAddrInfo; // Req'd.

    //
    // As long as we use a blocking accept(), we will have a race
    // between advertising the service and actually being ready to
    // accept connections.  If we use non-blocking accept, advertise
    // the service after accept has been called.
    //
    if ( SOCKET_ERROR == WSASetService(&wsaQuerySet, RNRSERVICE_REGISTER, 0) ) {
        printf("=CRITICAL= | WSASetService() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    //
    // listen() call indicates winsock2 to listen on a given socket for any incoming connection.
    //
    if ( SOCKET_ERROR == listen(LocalSocket, SOMAXCONN) ) {
        printf("=CRITICAL= | listen() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
        ulRetCode = CXN_ERROR;
        goto CleanupAndExit;
    }

    if ( 2 <= g_iOutputLevel ) {
        printf("*INFO* | listen() call succeeded\n");
    }

    for ( iCxnCount = 0;
        (0 == ulRetCode) && ( (iCxnCount < iMaxCxnCycles) || (iMaxCxnCycles == 0) );
        iCxnCount++ ) {

        printf("\n");

        //
        // accept() call indicates winsock2 to wait for any
        // incoming connection request from a remote socket.
        // If there are already some connection requests on the queue,
        // then accept() extracts the first request and creates a new socket and
        // returns the handle to this newly created socket. This newly created
        // socket represents the actual connection that connects the two sockets.
        //
        if ( INVALID_SOCKET == ( ClientSocket = accept(LocalSocket, NULL, NULL) ) ) {
            printf("=CRITICAL= | accept() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
            ulRetCode = CXN_ERROR;
            break; // Break out of the for loop
        }

        if ( 2 <= g_iOutputLevel ) {
            printf("*INFO* | accept() call succeeded. CientSocket = [0x%X]\n", ClientSocket);
        }

        //
        // Read data from the incoming stream
        //
        BOOL bContinue = TRUE;
        pszDataBuffer = (char *)HeapAlloc(GetProcessHeap(),
                                          HEAP_ZERO_MEMORY,
                                          CXN_TRANSFER_DATA_LENGTH);
        if ( NULL == pszDataBuffer ) {
            printf("-FATAL- | HeapAlloc failed | out of memory | gle = [%d] \n", GetLastError());
            ulRetCode = CXN_ERROR;
            goto CleanupAndExit;
        }
        pszDataBufferIndex = pszDataBuffer;
        uiTotalLengthReceived = 0;
        while ( bContinue && (uiTotalLengthReceived < CXN_TRANSFER_DATA_LENGTH) ) {
            //
            // recv() call indicates winsock2 to receive data
            // of an expected length over a given connection.
            // recv() may not be able to get the entire length
            // of data at once.  In such case the return value,
            // which specifies the number of bytes received,
            // can be used to calculate how much more data is
            // pending and accordingly recv() can be called again.
            //
            iLengthReceived = recv(ClientSocket,
                                   pszDataBufferIndex,
                                   (CXN_TRANSFER_DATA_LENGTH - uiTotalLengthReceived),
                                   0);

            switch ( iLengthReceived ) {
            case 0: // socket connection has been closed gracefully
                bContinue = FALSE;
                break;

            case SOCKET_ERROR:
                printf("=CRITICAL= | recv() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
                bContinue = FALSE;
                ulRetCode = CXN_ERROR;
                break;

            default: // most cases when data is being read
                pszDataBufferIndex += iLengthReceived;
                uiTotalLengthReceived += iLengthReceived;
                if ( 2 <= g_iOutputLevel ) {
                    printf("*INFO* | Receiving data of length = [%d]. Current Total = [%d]\n", iLengthReceived, uiTotalLengthReceived);
                }
                break;
            }
        }

        if ( 0 == ulRetCode ) {
            if ( CXN_TRANSFER_DATA_LENGTH != uiTotalLengthReceived ) {
                printf("+WARNING+ | Data transfer aborted mid-stream. Expected Length = [%d], Actual Length = [%d]\n", CXN_TRANSFER_DATA_LENGTH, uiTotalLengthReceived);
            }

            printf("*INFO* | Received following data string from remote device:\n%s\n", pszDataBuffer);

            //
            // Close the connection
            //
            if ( SOCKET_ERROR == closesocket(ClientSocket) ) {
                printf("=CRITICAL= | closesocket() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
                ulRetCode = CXN_ERROR;
            } else {
                //
                // Make the connection invalid regardless
                //
                ClientSocket = INVALID_SOCKET;

                if ( 2 <= g_iOutputLevel ) {
                    printf("*INFO* | closesocket() call succeeded w/socket=[0x%X]\n", ClientSocket);
                }
            }
        }
    }

    CleanupAndExit:
    if ( INVALID_SOCKET != ClientSocket ) {
        closesocket(ClientSocket);
        ClientSocket = INVALID_SOCKET;
    }

    if ( INVALID_SOCKET != LocalSocket ) {
        closesocket(LocalSocket);
        LocalSocket = INVALID_SOCKET;
    }

    if ( NULL != lpCSAddrInfo ) {
        HeapFree(GetProcessHeap(), 0, lpCSAddrInfo);
        lpCSAddrInfo = NULL;
    }
    if ( NULL != pszInstanceName ) {
        HeapFree(GetProcessHeap(), 0, pszInstanceName);
        pszInstanceName = NULL;
    }

    if ( NULL != pszDataBuffer ) {
        HeapFree(GetProcessHeap(), 0, pszDataBuffer);
        pszDataBuffer = NULL;
    }

    return(ulRetCode);
}

//
// ShowCmdLineSyntaxHelp displays the command line usage
//
void ShowCmdLineHelp(void)
{
    printf(
          "\n  Bluetooth example application for demonstrating connection and data transfer."
          "\n"
          "\n"
          "\n  BTHCxn.exe  [-n<RemoteName> | -a<RemoteAddress>] "
          "\n                  [-c<ConnectionCycles>] [-o<Output Level>]"
          "\n"
          "\n"
          "\n  Switches applicable for Client mode:"
          "\n    -n<RemoteName>        Specifies name of remote BlueTooth-Device."
          "\n"
          "\n    -a<RemoteAddress>     Specifies address of remote BlueTooth-Device."
          "\n                          The address is in form XX:XX:XX:XX:XX:XX"
          "\n                          where XX is a hexidecimal byte"
          "\n"
          "\n                          One of the above two switches is required for client."
          "\n"
          "\n"
          "\n  Switches applicable for both Client and Server mode:"
          "\n    -c<ConnectionCycles>  Specifies number of connection cycles."
          "\n                          Default value for this parameter is 1.  Specify 0 to "
          "\n                          run infinite number of connection cycles."
          "\n"
          "\n    -o<OutputLevel>       Specifies level of information reporting in cmd window."
          "\n                          Default value for this parameter is 0 (minimal info)."
          "\n                          Possible values: 1 (more info), 2 (all info)."
          "\n"
          "\n"
          "\n  Command Line Examples:"
          "\n    \"BTHCxn.exe -c0\""
          "\n    Runs the BTHCxn server for infinite connection cycles."
          "\n    The application reports minimal information onto the cmd window."
          "\n"
          "\n    \"BTHCxn.exe -nServerDevice -c50 -o2\""
          "\n    Runs the BTHCxn client connecting to remote device (having name "
          "\n    \"ServerDevice\" for 50 connection cycles."
          "\n    The application reports minimal information onto the cmd window."
          "\n"
          );
}

//
// ParseCmdLine parses the command line and sets the global variables accordingly.
// It returns 0 if successful, returns 1 if command line usage is to be displayed,
// returns 2 in case of any other error while parsing
//
ULONG ParseCmdLine (IN int argc, IN char * argv[])
{
    int		cbStrLen = 0;
    ULONG	ulRetCode = CXN_SUCCESS;
    HRESULT	res;

    for ( int i = 1; i < argc; i++ ) {
        char * pszToken = argv[i];
        if ( *pszToken == '-' || *pszToken == '/' ) {
            char token;

            //
            // skip over the "-" or "/"
            //
            pszToken++;

            //
            // Get the command line option
            //
            token = *pszToken;

            //
            // Go one past the option the option-data
            //
            pszToken++;

            //
            // Get the option-data
            //
            switch ( token ) {
            case 'n':
                cbStrLen = lstrlen(pszToken);
                if ( ( 0 < cbStrLen ) && ( BTH_MAX_NAME_SIZE >= cbStrLen ) ) {
                    res = StringCchCopy(g_szRemoteName, sizeof(g_szRemoteName), pszToken);
                    if ( FAILED(res) ) {
                        ulRetCode = CXN_ERROR;
                        printf("!ERROR! | cmd line | Unable to parse -n<RemoteName>, length error (min 1 char, max %d chars)\n", BTH_MAX_NAME_SIZE);
                    }
                } else {
                    ulRetCode = CXN_ERROR;
                    printf("!ERROR! | cmd line | Unable to parse -n<RemoteName>, length error (min 1 char, max %d chars)\n", BTH_MAX_NAME_SIZE);
                }
                break;

            case 'a':
                cbStrLen = lstrlen(pszToken);
                if ( CXN_BDADDR_STR_LEN == cbStrLen ) {
                    res = StringCchCopy(g_szRemoteAddr, sizeof(g_szRemoteAddr), pszToken);
                    if ( FAILED(res) ) {
                        ulRetCode = CXN_ERROR;
                        printf("!ERROR! | cmd line | Unable to parse -a<RemoteAddress>, Remote bluetooth radio address string length expected %d | Found: %d)\n", CXN_BDADDR_STR_LEN, cbStrLen);
                    }
                } else {
                    ulRetCode = CXN_ERROR;
                    printf("!ERROR! | cmd line | Unable to parse -a<RemoteAddress>, Remote bluetooth radio address string length expected %d | Found: %d)\n", CXN_BDADDR_STR_LEN, cbStrLen);
                }
                break;

            case 'c':
                if ( 0 < lstrlen(pszToken) ) {
                    sscanf_s(pszToken, "%d", &g_ulMaxCxnCycles);
                    if ( 0 > g_ulMaxCxnCycles ) {
                        ulRetCode = CXN_ERROR;
                        printf("!ERROR! | cmd line | Must provide +ve or 0 value with -c option\n");
                    }
                } else {
                    ulRetCode = CXN_ERROR;
                    printf("!ERROR! | cmd line | Must provide a value with -c option\n");
                }
                break;

            case 'o':
                if ( 0 < lstrlen(pszToken) ) {
                    sscanf_s(pszToken, "%d", &g_iOutputLevel);
                    if ( 0 > g_iOutputLevel ) {
                        ulRetCode = CXN_ERROR;
                        printf("!ERROR! | cmd line | Must provide a +ve or 0 value with -o option");
                    }
                } else {
                    ulRetCode = CXN_ERROR;
                    printf("!ERROR! | cmd line | Must provide a value with -o option");
                }
                break;

            case '?':
            case 'h':
            case 'H':
            default:
                ulRetCode = CXN_ERROR;
            }
        } else {
            ulRetCode = CXN_ERROR;
            printf("!ERROR! | cmd line | Bad option prefix, use '/' or '-' \n");
        }
    }

    return(ulRetCode);
}