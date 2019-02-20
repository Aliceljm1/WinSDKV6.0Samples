// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/****************************************************************************
						Microsoft RPC
           
                        WMarsh Example

    FILE:       WMarshu.c

    PURPOSE:    Utility functions used by both client and server
                sides of the RPC distributed application.
                This sample demonstrates the transmit_as example.
                A doubly-linked list is transmitted over the network
                as a sized array.

    RELATED:    WMarshs.c - server main
                WMarshp.c - remote procedures
                WMarshc.c - client main

    FUNCTIONS:  DOUBLE_LINK_TYPE_to_WMarsh - convert list to array
                DOUBLE_LINK_TYPE_from_WMarsh - convert array to list
                DOUBLE_LINK_TYPE_free_inst - free linked list memory
                DOUBLE_LINK_TYPE_free_WMarsh - free array memory
                midl_user_allocate - user-supplied memory allocator
                midl_user_free - user-supplied routine to free memory

                ArrayWalkProc - utility to display the array
                ListWalkProc - utility to display the linked list
                InsertNewNode - utility to add a node to the list

    COMMENTS:   This sample program generates a linked list to
                demonstrate how a list with aliasing can be transmitted
                using the transmit_as attribute as a sized array.
                The pointers are rebuilt on the server side.

****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "WMarsh.h"    // header file generated by MIDL compiler
#include "WMarshu.h"


/***************************************************************************/

void ArrayWalkProc(DOUBLE_WMarsh_TYPE * pArray)
{
    int i;

    printf("Display contents of transmitted array:\n");
    for (i = 0; i < pArray->sSize; i++)
        printf("pArray->asNumber[%d] = %d\n", i, pArray->asNumber[i]);
}

void ListWalkProc(DOUBLE_LINK_TYPE pList)
{
    printf("Display contents of doubly linked list:\n");
    while (pList != NULL) {
        printf("pList %p = %d, Next = %p\n",
               pList, pList->sNumber, pList->pNext);
        pList = pList->pNext;
    }
}

DOUBLE_LINK_TYPE  InsertNewNode(short sValue, DOUBLE_LINK_TYPE  pPrevious)
{
    DOUBLE_LINK_TYPE pNew;

    do {
        pNew = (DOUBLE_LINK_TYPE)midl_user_allocate(sizeof(DOUBLE_LINK_LIST));
    } while (pNew == pPrevious);

    pNew->pNext = NULL;      // initialize
    pNew->pPrevious = NULL;  // initialize
    pNew->sNumber = sValue;  // insert b between a and c

    pNew->pPrevious = pPrevious;             // prev(b) = a
    if (pPrevious == NULL)
        pNew->pNext = NULL;
    else {
        pNew->pNext = pPrevious->pNext;      // next(b) = c
        pPrevious->pNext = pNew;             // next(a) = b
        if (pNew->pNext != NULL)
            (pNew->pNext)->pPrevious = pNew; // prev(c) = b
    }

    return(pNew);
}

// Calculate size that converted data will 
// require in the buffer
unsigned long __RPC_USER DOUBLE_LINK_TYPE_UserSize( 
    ULONG __RPC_FAR * pulFlags, 
    unsigned long    startingSize,
	DOUBLE_LINK_TYPE __RPC_FAR * pList){
		int cCount=0;
		DOUBLE_LINK_TYPE pCur = *pList;
		for(;pCur!=NULL;pCur=pCur->pNext,cCount++);
		return startingSize + (cCount+1) * sizeof(short);
}


unsigned char * __RPC_USER DOUBLE_LINK_TYPE_UserMarshal( 
    ULONG __RPC_FAR *pulFlags, 
    unsigned char __RPC_FAR * pBufferStart, 
	DOUBLE_LINK_TYPE __RPC_FAR * pList){		
		unsigned short cCount,i;

		DOUBLE_LINK_TYPE __RPC_FAR  pCur = *pList;	

	 //   DOUBLE_WMarsh_TYPE * ptr = (DOUBLE_WMarsh_TYPE *) pBufferStart;
		
		unsigned char __RPC_FAR * pCurBuffer = pBufferStart;

	   for (cCount = 0; pCur != NULL; pCur = pCur->pNext){		
			cCount++;
		}
	
	   *((unsigned short *) pCurBuffer) = cCount;

	   ((unsigned short *) pCurBuffer)++;
	   
			/* copy the linked list contents into the array */
		for (i = 0,pCur = *pList; pCur!= NULL; i++, pCur = pCur->pNext){					
			*((unsigned short *) pCurBuffer) = pCur->sNumber;
			((unsigned short *) pCurBuffer)++;
		}

	return pCurBuffer;
}


unsigned char * __RPC_USER DOUBLE_LINK_TYPE_UserUnmarshal( 
    ULONG __RPC_FAR * pulFlags, 
    unsigned char __RPC_FAR * pBufferStart, 
	DOUBLE_LINK_TYPE __RPC_FAR * pList){	

    int i;

	unsigned short cCount = 0;
	DOUBLE_LINK_TYPE  pCur;
	DOUBLE_LINK_TYPE  pHead;
	unsigned char __RPC_FAR * pCurBuffer = pBufferStart;
	
	cCount = *(unsigned short *) pCurBuffer;
	((unsigned short *) pCurBuffer)++;
    if (cCount <= 0) {  // error checking
        pList = NULL;
        return NULL;
    }

	*pList = InsertNewNode(*((unsigned short *) pCurBuffer), NULL);
	((unsigned short *) pCurBuffer)++;

	pCur = *pList;	
	pHead = pCur;
	//pList = &pHead;	

	//the first element of the buffer is the size
	for (i = 1; i < cCount; i++) { // write new values
        pCur = InsertNewNode(*((unsigned short *) pCurBuffer), pCur);	
		((unsigned short *) pCurBuffer)++;
	}
	
	return pCurBuffer;
}


void __RPC_USER DOUBLE_LINK_TYPE_UserFree( 
    ULONG __RPC_FAR * pulFlags, 
    DOUBLE_LINK_TYPE __RPC_FAR * head 
	){
		DOUBLE_LINK_TYPE   cur = *head;
		DOUBLE_LINK_TYPE   next = cur->pNext;
		while(cur!=NULL){			
			free(cur);
			cur = next;
			if(next!=NULL)
				next = next->pNext;		
		}
}

/***************************************************************************/

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
    free(ptr);
}


/***************************************************************************/


/* end file WMarshu.c */