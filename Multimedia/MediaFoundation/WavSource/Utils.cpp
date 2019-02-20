//////////////////////////////////////////////////////////////////////////
//
// Utils.cpp : Misc. helper classes.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#include "WavSource.h"


CCritSec::CCritSec()
{
    InitializeCriticalSection(&m_criticalSection);
}

CCritSec::~CCritSec()
{
    DeleteCriticalSection(&m_criticalSection);
}


void CCritSec::Lock()
{
    EnterCriticalSection(&m_criticalSection);
}

void CCritSec::Unlock()
{
    LeaveCriticalSection(&m_criticalSection);
}



CAutoLock::CAutoLock(CCritSec& crit)
{
    m_pCriticalSection = &crit;
    m_pCriticalSection->Lock();
}


CAutoLock::~CAutoLock()
{
    m_pCriticalSection->Unlock();
}

