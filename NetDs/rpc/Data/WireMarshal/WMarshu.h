// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/****************************************************************************
						Microsoft RPC
           
                        WMarsh Example

    FILE:       WMarshu.h

    PURPOSE:    Function prototypes for functions in WMarshu.c

****************************************************************************/

#ifndef _WMarshU_H_
#define _WMarshU_H_


DOUBLE_LINK_TYPE 
InsertNewNode(
    short sValue,    DOUBLE_LINK_TYPE pPrevious
    );

void
ArrayWalkProc(
    DOUBLE_WMarsh_TYPE * pArray
    );

void
ListWalkProc(
    DOUBLE_LINK_TYPE  pList
    );

#endif

/* end file WMarshu.h */
