// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/****************************************************************************
						Microsoft RPC
          
                       inout Example

    FILE:       inoutp.c

    PURPOSE:    Remote procedures that are linked with the server
                side of RPC distributed application

    FUNCTIONS:  InOutProc() - demonstrates in, out parameters

    COMMENTS:

****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "inout.h"    // header file generated by MIDL compiler

#define CONSTANT 257

void InOutProc(IN RPC_BINDING_HANDLE hBinding,
			   short    s1,
               short * ps2,
               float * pf3)
{
    printf_s("on entry, *pf3 = %f\n", *pf3);

    *pf3 = (float) s1 / (float) *ps2;
    printf_s("%d / %d = %0.3f\n", s1, *ps2, *pf3);

    *ps2 = (short)CONSTANT - s1;
    printf_s("%d - %d = %d\n", CONSTANT, s1, *ps2);

    s1++;

    return;
}

void Shutdown(IN RPC_BINDING_HANDLE hBinding)
{
    RPC_STATUS status;

    printf_s("Calling RpcMgmtStopServerListening\n");
    status = RpcMgmtStopServerListening(NULL);
    printf_s("RpcMgmtStopServerListening returned: 0x%x\n", status);
    if (status) {
        exit(status);
    }

    printf_s("Calling RpcServerUnregisterIf\n");
    status = RpcServerUnregisterIf(NULL, NULL, FALSE);
    printf_s("RpcServerUnregisterIf returned 0x%x\n", status);
    if (status) {
        exit(status);
    }
}

/* end file inoutp.c */
