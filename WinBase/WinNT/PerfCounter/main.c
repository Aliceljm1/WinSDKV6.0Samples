/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All rights reserved.

Abstract:

	//
	// Implements a Perflib V2 provider (PerflibV2SampleProvider.exe)
	// on top of the boiler source code produced by ctrpp.exe.
	//

Environment:

    User mode

--*/
#include "PerflibV2SampleProvider.h"
#include <stdio.h>
#include <strsafe.h>
#include <math.h>

/*++

Routine Description:

    Starting point of the V2 provider executable.
    1. Initializes the provider 
    2. Creates instacnes for the counter sets
    3. Sets the counter values for all the objects' counters in both counter 
    	sets iteratively sleeps for 1 second between iterations. 

Arguments:

    Default arguments to wmain

Return Value:

    Standard ULONG Status of perflib V2 provider API.

--*/

int
__cdecl wmain(int argc, __in WCHAR ** argv)
{
    ULONG                     Status     = 0x0;
    PPERF_COUNTERSET_INSTANCE pObj1Inst1 = NULL;
    PPERF_COUNTERSET_INSTANCE pObj1Inst2 = NULL;
    PPERF_COUNTERSET_INSTANCE pObj1Inst3 = NULL;
    PPERF_COUNTERSET_INSTANCE pObj2Inst  = NULL;
    DWORD                     dwSine;
    DWORD                     dwCoSine;
    DWORD                     dwConst    = 50;
    DWORD                     dwBase1    = 17;
    DWORD                     dwBase2    = 50;
    DWORD                     dwBase3    = 83;
    DWORD                     dwDegree   = 0;
    DWORD                     dwSign;
    DWORD                     dwAngle;
    DWORD                     dwIncrease;
    DWORD                     dwLoopCount = 1000;
    DWORD                     dwNaturalNumbers = 1;
    double                    dAngle;
	
    argc;
    argv;

    //
    // Call PerfAutoInitialize();
    // PerfAutoInitialize() is created by ctrpp.exe and is present in PerflibV2SampleProvider.c
    // PerfAutoInitialize starts the provider and initializes the counter sets.
    // In this sample we have a multiple instance counter set and a single instance counter set.
    //
    Status     = PerfAutoInitialize();
    if (Status != ERROR_SUCCESS) {
        return Status;
    }

    //
    // Create the instances for multiple instance counter set.
    //
    pObj1Inst1 = PerfCreateInstance(hDataSource_PerflibV2SampleProvider_1, & CtrSetGuid_PerflibV2SampleProvider_1_1, L"Instance_1", 0);
    printf("\tPerfCreateInstance(0x%p,%d)\n", pObj1Inst1, GetLastError());
    if (pObj1Inst1 == NULL) {
        Status = GetLastError();
        goto Cleanup;
    }

    pObj1Inst2 = PerfCreateInstance(hDataSource_PerflibV2SampleProvider_1, & CtrSetGuid_PerflibV2SampleProvider_1_1, L"Instance_2", 0);
    printf("\tPerfCreateInstance(0x%p,%d)\n", pObj1Inst2, GetLastError());
    if (pObj1Inst2 == NULL) {
        Status = GetLastError();
        goto Cleanup;
    }

    pObj1Inst3 = PerfCreateInstance(hDataSource_PerflibV2SampleProvider_1, & CtrSetGuid_PerflibV2SampleProvider_1_1, L"Instance_3", 0);
    printf("\tPerfCreateInstance(0x%p,%d)\n", pObj1Inst3, GetLastError());
    if (pObj1Inst3 == NULL) {
        Status = GetLastError();
        goto Cleanup;
    }

    //
    // Create the instances for a single instance counter set.
    //
    pObj2Inst = PerfCreateInstance(hDataSource_PerflibV2SampleProvider_1, & CtrSetGuid_PerflibV2SampleProvider_1_2, L"_Default", 0);
    printf("\tPerfCreateInstance(0x%p,%d)\n", pObj2Inst, GetLastError());
    if (pObj2Inst == NULL) {
        Status = GetLastError();
        goto Cleanup;
    }
    else {
        Status = PerfSetCounterRefValue(hDataSource_PerflibV2SampleProvider_1, pObj2Inst, 3, & dwSine);
        if (Status != ERROR_SUCCESS) {
            goto Cleanup;
        }
        Status = PerfSetCounterRefValue(hDataSource_PerflibV2SampleProvider_1, pObj2Inst, 4, & dwCoSine);
        if (Status != ERROR_SUCCESS) {
            goto Cleanup;
        }
        Status = PerfSetCounterRefValue(hDataSource_PerflibV2SampleProvider_1, pObj2Inst, 5, & dwConst);
        if (Status != ERROR_SUCCESS) {
            goto Cleanup;
        }

	Status = PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj2Inst, 6, dwConst);
        if (Status != ERROR_SUCCESS) {
            goto Cleanup;
        }
    }

    // Generate some data. In this particular example, we use cosine and sine values for samples.

    while ((Status == ERROR_SUCCESS) && (dwLoopCount)) {
        dwSign  = ((dwDegree % 180) > 90) ? 15 : -15;
        if (dwDegree > 270) {
            dwAngle = 360 - dwDegree;
        }
        else if (dwDegree > 180) {
            dwAngle = dwDegree - 180;
        }
        else if (dwDegree > 90) {
            dwAngle = 180 - dwDegree;
        }
        else {
            dwAngle = dwDegree;
        }
        dwIncrease = (DWORD) ((15.00 * ((double) dwAngle)) / 90.00);

        // Set raw counter data for MultiInstanceCounterSet Instance_1, the base is 17
        //
        PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst1, 2, dwBase1 + dwSign);
        if (dwDegree > 180) {
            PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst1, 1, dwBase1 - dwIncrease);
        }
        else {
            PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst1, 1, dwBase1 + dwIncrease);
        }
        printf("\tObject1_MultiInstance(Instance_1,%d,%d,%d,%d)\n", dwBase1, dwSign, dwAngle, dwIncrease);

        // Set raw counter data for MultiInstanceCounterSet Instance_2, the base is 50
        //
        PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst2, 2, dwBase2 + dwSign);
        if (dwDegree > 180) {
            PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst2, 1, dwBase2 - dwIncrease);
        }
        else {
            PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst2, 1, dwBase2 + dwIncrease);
        }
        printf("\tObject1_MultiInstance(Instance_2,%d,%d,%d,%d)\n", dwBase2, dwSign, dwAngle, dwIncrease);

        // Set raw counter data for MultiInstanceCounterSet Instance_3, the base is 83
        //
        PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst3, 2, dwBase3 + dwSign);
        if (dwDegree > 180) {
            PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst3, 1, dwBase3 - dwIncrease);
        }
        else {
            PerfSetULongCounterValue(hDataSource_PerflibV2SampleProvider_1, pObj1Inst3, 1, dwBase3 + dwIncrease);
        }
        printf("\tObject1_MultiInstance(Instance_3,%d,%d,%d,%d)\n", dwBase3, dwSign, dwAngle, dwIncrease);

        // Set raw counter data for SingleInstanceCounterSet
        //
        dAngle   = (((double) dwDegree) * 3.14159) / (180.00);
        dwSine   = dwConst + (DWORD) (30.00 * sin(dAngle));
        dwCoSine = dwConst + (DWORD) (30.00 * cos(dAngle));

        printf("\tObject2_SingleInstance(%d,%d,%d,%d)\n", dwSine, dwCoSine, dwConst, dwDegree);

        // Increment the dwDegree value to between 0 - 360
        //
        dwDegree += 10;
        if (dwDegree >= 360) {
            dwDegree = 0;
        }
		
		// Increment the Natural Number counter. Set it to 1 if we reach 100
		//
		dwNaturalNumbers++;
		if( dwNaturalNumbers == 100) {
			dwNaturalNumbers = 1;
		}
		// Set raw counter data for SingleInstanceCounterSet
        	Status = PerfSetCounterRefValue(hDataSource_PerflibV2SampleProvider_1, pObj2Inst, 7, &dwNaturalNumbers);
        	if (Status != ERROR_SUCCESS) {
			printf("\tPerfSetCounterRefValue Fails with error: (%d)\n", Status);
            		goto Cleanup;
        	}

		//
		//Sleep for 1 second before iterating once again to change the counter values.
		//
        
		Sleep(1000);
	 dwLoopCount--;
    }
    printf(" Provider finished - Returning from main() \n");

Cleanup:
    PerfAutoCleanup();
	return Status;
}
