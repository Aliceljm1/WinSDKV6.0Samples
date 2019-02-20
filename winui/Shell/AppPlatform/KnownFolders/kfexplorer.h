
#ifndef _KFE_H_
#define _KFE_H_

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <shellapi.h>
#include <Psapi.h>

#include "resource.h"

#define GUID_SIZE               128
#define SZ_REG_PATH_HISTORY     L"Software\\Microsoft\\KnownFolderSample"

// command line args
#define SZ_CLA_REGISTER         L"/register"
#define SZ_CLA_ENUM             L"/enum"
#define SZ_CLA_UNREGISTER       L"/unregister"
#define SZ_CLA_CATEGORY         L"/category:"
#define SZ_CLA_DEFFLAG          L"/defFlag:"
#define SZ_CLA_ID               L"/id:"
#define SZ_CLA_PSZNAME          L"/pszName:"
#define SZ_CLA_PSZDESCRIPTION   L"/pszDescription:"
#define SZ_CLA_PSZRELPATH       L"/pszRelativePath:"
#define SZ_CLA_PSZPARSENAME     L"/pszParsingName:"
#define SZ_CLA_PSZLOCALIZEDNAME L"/pszLocalizedName:"
#define SZ_CLA_PSZICON          L"/pszIcon:"
#define SZ_CLA_PSZTOOLTIP       L"/pszTooltip:"
#define SZ_CLA_PSZSECURITY      L"/pszSecurity:"
#define SZ_CLA_FINDFORPATH      L"/pszFindForPath:"
#define SZ_CLA_CLEAN            L"/clean"
#define SZ_CLA_SHOW_USAGE       L"/?"


typedef enum
{
    ACT_UNDEFINED,
    ACT_REGISTER,
    ACT_ENUM,
    ACT_UNREGISTER,
    ACT_CLEAN,
    ACT_SHOW_USAGE,
    ACT_FIND_FOR_PATH
} ACTION_TYPE;

typedef const struct _tagKF_FLAGS
{
    DWORD dwFlag;
    LPWSTR pszKFID;
    LPWSTR pszFriendlyName;
} KF_FLAGS;


static KF_FLAGS c_rgKFDefFlagInfo[] =
{
    { KFDF_LOCAL_REDIRECT_ONLY, L"KFDF_LOCAL_REDIRECT_ONLY",    L"local" },
    { KFDF_ROAMABLE,            L"KFDF_ROAMABLE",               L"roam" },
    { KFDF_PRECREATE,           L"KFDF_PRECREATE",              L"precreate" }
};

static KF_FLAGS c_rgKFCategoryInfo[] =
{
    { KF_CATEGORY_VIRTUAL,      L"KF_CATEGORY_VIRTUAL",         L"virtual" },
    { KF_CATEGORY_FIXED,        L"KF_CATEGORY_FIXED",           L"fixed" },
    { KF_CATEGORY_COMMON,       L"KF_CATEGORY_COMMON",          L"common" },
    { KF_CATEGORY_PERUSER,      L"KF_CATEGORY_PERUSER",         L"user" },
};

BOOL g_fVerbose = FALSE;


// function declarations necessary for improved sample readability
void DumpKnownFolderDef(KNOWNFOLDERID kfid, KNOWNFOLDER_DEFINITION kfd);
void DumpKnownFolderInfo(IKnownFolder *pkf);
BOOL ParseAndValidateCommandLine(LPWSTR *ppszArgs, int iArgs, ACTION_TYPE *at, KNOWNFOLDERID *pkfid, KNOWNFOLDER_DEFINITION *pkfd, LPWSTR *ppszFindForPath);
void CompleteKnownFolderDef(KNOWNFOLDER_DEFINITION *pkfd);
HRESULT CreatePhysicalFolderIfNecessary(KNOWNFOLDERID kfidParent, LPWSTR pszRelativePath);
void DumpUsage();
void UnregisterAllKFsAddedByThisTool(DWORD *pdwKFs);
void AddRegisteredFolderToHistory(KNOWNFOLDERID kfid);


#endif