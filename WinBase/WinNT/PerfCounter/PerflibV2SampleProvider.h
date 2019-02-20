#ifndef _PerflibV2SampleProvider_H
#define _PerflibV2SampleProvider_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <perflib.h>
#include <winperf.h>

#ifdef __INIT_PerflibV2SampleProvider_IMP
#define PROVIDER_GUID_PerflibV2SampleProvider_1 { 0xFFEEDDBB, 0x965A, 0x4CF9, 0x9C, 0x07, 0xFE, 0x25, 0x37, 0x8C, 0x2A, 0x23 }
GUID ProviderGuid_PerflibV2SampleProvider_1 = PROVIDER_GUID_PerflibV2SampleProvider_1;
#else
extern GUID ProviderGuid_PerflibV2SampleProvider_1;
#endif

typedef struct _CTRSET_PerflibV2SampleProvider_1_1 {
    PERF_COUNTERSET_INFO CtSet_PerflibV2SampleProvider_1_1;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_1_1;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_1_2;
} CTRSET_PerflibV2SampleProvider_1_1, * PCTRSET_PerflibV2SampleProvider_1_1;

#ifdef __INIT_PerflibV2SampleProvider_IMP
#define CTRSET_GUID_PerflibV2SampleProvider_1_1 { 0xFFEEDDBB, 0xC923, 0x4794, 0xB6, 0x96, 0x70, 0x57, 0x76, 0x30, 0xB5, 0xCF }
GUID CtrSetGuid_PerflibV2SampleProvider_1_1 = CTRSET_GUID_PerflibV2SampleProvider_1_1;
CTRSET_PerflibV2SampleProvider_1_1 CtrSet_PerflibV2SampleProvider_1_1 = {
    { CTRSET_GUID_PerflibV2SampleProvider_1_1, PROVIDER_GUID_PerflibV2SampleProvider_1, 2, PERF_COUNTERSET_MULTI_INSTANCES },
    { 1, PERF_COUNTER_RAWCOUNT, 0, sizeof(DWORD), PERF_DETAIL_NOVICE, 0, 0 },
    { 2, PERF_COUNTER_RAWCOUNT, 0, sizeof(DWORD), PERF_DETAIL_NOVICE, 0, 8 }
};

ULONG dwCtrSet_PerflibV2SampleProvider_1_1 = sizeof(CTRSET_PerflibV2SampleProvider_1_1);
#else
extern GUID CtrSetGuid_PerflibV2SampleProvider_1_1;
extern CTRSET_PerflibV2SampleProvider_1_1 CtrSet_PerflibV2SampleProvider_1_1;
extern ULONG dwCtrSet_PerflibV2SampleProvider_1_1;
#endif

typedef struct _CTRSET_PerflibV2SampleProvider_1_2 {
    PERF_COUNTERSET_INFO CtSet_PerflibV2SampleProvider_1_2;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_2_1;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_2_2;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_2_3;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_2_4;
    PERF_COUNTER_INFO    Ctr_PerflibV2SampleProvider_1_2_5;
} CTRSET_PerflibV2SampleProvider_1_2, * PCTRSET_PerflibV2SampleProvider_1_2;

#ifdef __INIT_PerflibV2SampleProvider_IMP
#define CTRSET_GUID_PerflibV2SampleProvider_1_2 { 0xFFEEDDBB, 0xEAA6, 0x45BA, 0xBF, 0x6D, 0x4C, 0x7C, 0xB0, 0xD6, 0xEF, 0x73 }
GUID CtrSetGuid_PerflibV2SampleProvider_1_2 = CTRSET_GUID_PerflibV2SampleProvider_1_2;
CTRSET_PerflibV2SampleProvider_1_2 CtrSet_PerflibV2SampleProvider_1_2 = {
    { CTRSET_GUID_PerflibV2SampleProvider_1_2, PROVIDER_GUID_PerflibV2SampleProvider_1, 5, PERF_COUNTERSET_SINGLE_INSTANCE },
    { 3, PERF_COUNTER_RAWCOUNT, PERF_ATTRIB_BY_REFERENCE, sizeof(LPVOID), PERF_DETAIL_NOVICE, 0, 0 },
    { 4, PERF_COUNTER_RAWCOUNT, PERF_ATTRIB_BY_REFERENCE, sizeof(LPVOID), PERF_DETAIL_NOVICE, 0, 8 },
    { 5, PERF_COUNTER_RAWCOUNT, PERF_ATTRIB_BY_REFERENCE, sizeof(LPVOID), PERF_DETAIL_NOVICE, 1, 16 },
    { 6, PERF_RAW_BASE, PERF_ATTRIB_NO_DISPLAYABLE, sizeof(DWORD), PERF_DETAIL_NOVICE, 0, 24 },
    { 7, PERF_RAW_FRACTION, PERF_ATTRIB_BY_REFERENCE, sizeof(LPVOID), PERF_DETAIL_NOVICE, 0, 32 }
};

ULONG dwCtrSet_PerflibV2SampleProvider_1_2 = sizeof(CTRSET_PerflibV2SampleProvider_1_2);
#else
extern GUID CtrSetGuid_PerflibV2SampleProvider_1_2;
extern CTRSET_PerflibV2SampleProvider_1_2 CtrSet_PerflibV2SampleProvider_1_2;
extern ULONG dwCtrSet_PerflibV2SampleProvider_1_2;
#endif

#ifdef __INIT_PerflibV2SampleProvider_IMP
HANDLE hDataSource_PerflibV2SampleProvider_1 = NULL;
#else
extern HANDLE hDataSource_PerflibV2SampleProvider_1;
#endif


ULONG PerfAutoInitialize(void);
ULONG PerfAutoCleanup(void);

#ifdef __cplusplus
}
#endif

#endif // _PerflibV2SampleProvider_H

