// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/*************************************************************************
                  
                        Microsoft RPC
						strout sample

  FILE      :   client.c

  USAGE     :   client  -n network_address
                        -p protocol_sequence
                        -e endpoint
                        -o options

  PURPOSE   :   Client side of the RPC distributed application strout.

  COMMENTS  :   This program shows how to call remote procedures that
                allocate memory for a two dimensional array (an array of
                character pointers). It calls the server that allocates 
                memory to store all the environment strings in, and copies
                the environment variables to this memory. The client then 
                displays all the data on the client machine.

                Since this program uses the implicit binding method, some 
                of the binding handling must be done at the client side
*************************************************************************/

#include "strout.h"     /* Generated by the midl compiler               */
#include "common.h"     /* Definitions that are common to all files	    */


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*  Procedure   :   void Usage(_TUCHAR *)                               */
/*  Desc        :   This procedure prints out an error message if the   */
/*                  command line arguments are wrong                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Usage(_TUCHAR * pszProgramName)
{
    _tprintf_s(TEXT("USAGE : %s [-option]\n"), pszProgramName);
    _tprintf_s(TEXT("Options : -n Network Address\n"));
    _tprintf_s(TEXT("          -p Protocol Sequence\n"));  
    _tprintf_s(TEXT("          -e Endpoint\n"));  
    _tprintf_s(TEXT("          -o Options\n"));  
    exit(EXECUTION_FAILED);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* The client main program                                              */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int main(int argc, char *argv[])
{
    RPC_STATUS nStatus;     /* Return value from the RPC calls          */
    unsigned int
        nIdx,               /* Counter in loops                         */
        nNumArgs;           /* Number of commandline arguments          */
    unsigned long 
        nNumLines = 0;      /* Number of lines in the environment block */
    str	*pEnvBlock;         /* Pointer to an array of string pointers   */
    
    // These variables are used for the implicit binding
    _TUCHAR	*pszUuid            = NULL;
    _TUCHAR	*pszProtocolSequence= PROTOCOL_SEQUENCE;
    _TUCHAR	*pszNetworkAddress  = NULL;	
    _TUCHAR	*pszEndpoint        = END_POINT;
    _TUCHAR	*pszOptions         = NULL;
    _TUCHAR	*pszStringBinding   = NULL;
	
    /* Get a common handle on the command line arguments for both       */
    /* UNICODE and ASCII                                                */
#ifdef _UNICODE
    LPWSTR	*szArglist = CommandLineToArgvW(GetCommandLine(), &nNumArgs);
    if (NULL == szArglist)
    {
        _tprintf_s(TEXT("SERVER.C : CommandLineToArgW failed"));
        exit(EXECUTION_FAILED);
    }
#else
    char **szArglist= argv;
    nNumArgs = argc;
#endif

    /* Allow the user to override settings with commandline switches    */
    for (nIdx = 1; nIdx < nNumArgs; nIdx++) 
    {
        if ((_tcscmp(szArglist[nIdx], TEXT("-n")) == 0) || 
            (_tcscmp(szArglist[nIdx], TEXT("-N")) == 0))
        {
            pszNetworkAddress = szArglist[++nIdx];
        }
        else if((_tcscmp(szArglist[nIdx], TEXT("-p")) == 0) || 
            (_tcscmp(szArglist[nIdx], TEXT("-P")) == 0))
        {
            pszProtocolSequence = szArglist[++nIdx];
        }
        else if((_tcscmp(szArglist[nIdx], TEXT("-e")) == 0) || 
                (_tcscmp(szArglist[nIdx], TEXT("-e")) == 0))
        {
            pszEndpoint = szArglist[++nIdx];
        }
        else if((_tcscmp(szArglist[nIdx], TEXT("-o")) == 0) || 
                (_tcscmp(szArglist[nIdx], TEXT("-O")) == 0))
        {
            pszOptions = szArglist[++nIdx];
        }
        else 
        {
            Usage(szArglist[0]);
        }
    }
            

    /* Since we are using implicit binding, we need to do some binding  */
    /* from the client side as well.                                    */
    /* Use a function to concatenate the elements of the string         */
    /* binding into the proper sequence                                 */
    nStatus = RpcStringBindingCompose(	
        pszUuid,							
        pszProtocolSequence,
        pszNetworkAddress,
        pszEndpoint,
        pszOptions,
        &pszStringBinding);
    EXIT_IF_FAIL(nStatus, "RpcStringBindingCompose");

    /* Set the binding handle that will be used to bind to the server   */
    nStatus = RpcBindingFromStringBinding(	
        pszStringBinding,					
        &strout_sample_v1_0_c_ifspec);      /* The global handle used   */
    EXIT_IF_FAIL(nStatus, "RpcBindingFromStringBinding");

    /* Initialize the pointer to NULL */
    pEnvBlock = NULL;
	
    RpcTryExcept                /* Catch any exception that occurs      */
    {
        /* Call the remote procedure */
        _tprintf_s(TEXT("Calling the remote procedure \'GetRemoteEnv\' \n"));
        GetRemoteEnv(strout_sample_v1_0_c_ifspec,&nNumLines, &pEnvBlock);
    }
    RpcExcept(( ( (RpcExceptionCode() != STATUS_ACCESS_VIOLATION) &&
                   (RpcExceptionCode() != STATUS_DATATYPE_MISALIGNMENT) &&
                   (RpcExceptionCode() != STATUS_PRIVILEGED_INSTRUCTION) &&
                   (RpcExceptionCode() != STATUS_BREAKPOINT) &&
                   (RpcExceptionCode() != STATUS_STACK_OVERFLOW) &&
                   (RpcExceptionCode() != STATUS_IN_PAGE_ERROR) &&
                   (RpcExceptionCode() != STATUS_GUARD_PAGE_VIOLATION)
                    )
                    ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH ))
    {
        _tprintf_s(TEXT("Run-time exception %08X = %d in %s\n"), 
            RpcExceptionCode(), RpcExceptionCode(), TEXT(__FILE__));

	_tprintf_s("\n!!!NOTE!!!\nIf you are running XP or Vista, this problem might be due to Access Denial. Kindly go through the MSDN material: http://msdn.microsoft.com/security/productinfo/XPSP2/networkprotection/restrict_remote_clients_devimp.aspx.  \n\n");

        exit(EXECUTION_FAILED);
    }
    RpcEndExcept


    /* Print out the result */
    _tprintf_s(TEXT("There are %d environment variables\n"), nNumLines);
    _tprintf_s(TEXT("They are:\n"));
    for (nIdx = 0; nIdx < nNumLines; nIdx++)
    {
        _tprintf_s(TEXT("\t%s\n"), pEnvBlock[nIdx]);
    }
	
	
    // Deallocate all the memory used for the EnvBlock
    for (nIdx = 0; nIdx < nNumLines; nIdx++)
    {
        midl_user_free(pEnvBlock[nIdx]);
    }
    midl_user_free(pEnvBlock);

    // Deallocate the memory used for the ARGLIST if using UNICODE
#ifdef _UNICODE
    if (NULL != szArglist)
        free(szArglist);
#endif

    /* When the call is done, shut down the server application */
    ShutDown(strout_sample_v1_0_c_ifspec);

    return (EXECUTION_OK);
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Procedure    :   midl_user_allocate() and midl_user_free()           */
/* Desc.        :   These procedure are declared in the header file     */
/*                  generated by the midl compiler. These procedures    */
/*                  should be used for all memory allocation and        */
/*                  deallocation.                                       */
/*                  These procedures are also called by the stub code to*/
/*                  allocate and free memory.                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void __RPC_FAR * __RPC_API midl_user_allocate(size_t nLen)
{
    return (malloc(nLen));
}

void __RPC_API midl_user_free(void __RPC_FAR * lpvPointer)
{
    if (lpvPointer != NULL)
        free (lpvPointer);
}
