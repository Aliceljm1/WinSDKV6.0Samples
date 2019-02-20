// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include "strsafe.h"
#include <vector>

#define MAX_LOCALE_NAME_LENGTH 89


#define MUI_SHOW_NAME       TEXT("Picture")
#define EN_US_LOCALE_NAME   TEXT("en-US")


// Structure that contains information about language choice and available
//  resources
struct GlobalConfig
{
    // If bUseOSLangSetting is true, then the application will follow 
    //  User and System Preferred UI Language, rather than an explicitly
    //  chosen language from appLangs.
    bool    bUseOSLangSetting;
    PTCHAR  osDefaultLang;              // The Install Language of this OS
    std::vector<std::wstring> appLangs; // Application-supported
                                        // UI languages
    int     appDefaultLang;             // Indexes the currently chosen
                                        // language from appLangs.
    LCID    currDefaultLCID;            // Corresponding LCID; 
                                        // bUseOSLangSetting determines of
                                        // what language

    GlobalConfig()
    {
        bUseOSLangSetting = true;
        osDefaultLang = NULL;
        appDefaultLang = -1;
        currDefaultLCID = 0;
        appLangs.clear();
    }
};


typedef struct _ENUMUILANGPARAM 
{
    LANGID          uCurrentUserUILanguage;
    int             nCounter;
} ENUMUILANGPARAM ,*PENUMUILANGPARAM ;

typedef struct _CLOSEDCAPTIONLANGTAGTBL
{
    LPTSTR          CCLangTag;
    LPTSTR          CCNameTag;
} CLOSEDCAPTIONLANGTAGTBL;

#endif
