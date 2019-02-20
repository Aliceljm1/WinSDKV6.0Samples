// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/****************************************************************************
						Microsoft RPC
           
                       usrdef Example

    FILE:       usrdefs.c

    USAGE:      usrdefs  -p protocol_sequence
                         -e endpoint
                         -m max calls
                         -n min calls
                         -f flag for RpcServerListen

    PURPOSE:    Server side of RPC distributed application usrdef

    FUNCTIONS:  main() - registers server as RPC server

    COMMENTS:   This distributed application uses a user-defined handle.

****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "usrdef.h"    // header file generated by MIDL compiler

#define PURPOSE \
"This Microsoft RPC Version 2.0 sample program demonstrates\n\
the use of user-defined handles. For more information\n\
about the [handle] attribute and RPC API functions, see the\n\
RPC programming guide and reference.\n\n"

void Usage(char * pszProgramName)
{
    fprintf_s(stderr, "%s", PURPOSE);
    fprintf_s(stderr, "Usage:  %s\n", pszProgramName);
    fprintf_s(stderr, " -p protocol_sequence\n");
    fprintf_s(stderr, " -e endpoint\n");
    fprintf_s(stderr, " -m maxcalls\n");
    fprintf_s(stderr, " -n mincalls\n");
    fprintf_s(stderr, " -f flag_wait_op\n");
    exit(1);
}

void __cdecl main(int argc, char * argv[])
{
    RPC_STATUS status;
    unsigned char * pszProtocolSequence = "ncacn_ip_tcp";
    unsigned char * pszSecurity         = NULL;
    unsigned char * pszEndpoint         = "8765";
    unsigned int    cMinCalls           = 1;
    unsigned int    cMaxCalls           = 20;
    unsigned int    fDontWait           = FALSE;
    int i;

    /* allow the user to override settings with command line switches */
    for (i = 1; i < argc; i++) {
        if ((*argv[i] == '-') || (*argv[i] == '/')) {
            switch (tolower(*(argv[i]+1))) {
            case 'p':  // protocol sequence
                pszProtocolSequence = argv[++i];
                break;
            case 'e':
                pszEndpoint = argv[++i];
                break;
            case 'm':
                cMaxCalls = (unsigned int) atoi(argv[++i]);
                break;
            case 'n':
                cMinCalls = (unsigned int) atoi(argv[++i]);
                break;
            case 'f':
                fDontWait = (unsigned int) atoi(argv[++i]);
                break;
            case 'h':
            case '?':
            default:
                Usage(argv[0]);
            }
        }
        else
            Usage(argv[0]);
    }

    status = RpcServerUseProtseqEp(pszProtocolSequence,
                                   cMaxCalls,
                                   pszEndpoint,
                                   pszSecurity);  // Security descriptor
    printf_s("RpcServerUseProtseqEp returned 0x%x\n", status);
    if (status) {
        exit(status);
    }

 status = RpcServerRegisterIfEx(usrdef_ServerIfHandle,  NULL, NULL, RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, RPC_C_LISTEN_MAX_CALLS_DEFAULT, NULL ); 


    printf_s("RpcServerRegisterIf returned 0x%x\n", status);
    if (status) {
        exit(status);
    }

    printf_s("Calling RpcServerListen\n");
    status = RpcServerListen(cMinCalls,
                             cMaxCalls,
                             fDontWait);
    printf_s("RpcServerListen returned: 0x%x\n", status);
    if (status) {
        exit(status);
    }

    if (fDontWait) {
        printf_s("Calling RpcMgmtWaitServerListen\n");
        status = RpcMgmtWaitServerListen();  //  wait operation
        printf_s("RpcMgmtWaitServerListen returned: 0x%x\n", status);
        if (status) {
            exit(status);
        }
    }

}  // end main()

/*********************************************************************/
/*                 MIDL allocate and free                            */
/*********************************************************************/

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
    free(ptr);
}

/* end file usrdefs.c */
