// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// Link this against sdk\lib\StructuredQuery.lib.

// This sample program reads lines from the console, parses them using the system
// schema, and displays the resulting condition trees. A real application might
// feed the condition trees to a graphical UI and/or translate them into a query
// language to be executed.

#include <stdio.h>
#include <StructuredQuery.h>

// Return a label for a condition operation. Do not free the result.

PCWSTR GetLabel(CONDITION_OPERATION cop)
{
    PCWSTR pszOperation;
    switch (cop)
    {
        case COP_IMPLICIT:
            pszOperation = L"IMPLICIT";
            break;
        case COP_EQUAL:
            pszOperation = L"=";
            break;
        case COP_NOTEQUAL:
            pszOperation = L"!=";
            break;
        case COP_LESSTHAN:
            pszOperation = L"<";
            break;
        case COP_GREATERTHAN:
            pszOperation = L">";
            break;
        case COP_LESSTHANOREQUAL:
            pszOperation = L"<=";
            break;
        case COP_GREATERTHANOREQUAL:
            pszOperation = L">=";
            break;
        case COP_VALUE_STARTSWITH:
            pszOperation = L"VALUE_STARTS_WITH";
            break;
        case COP_VALUE_ENDSWITH:
            pszOperation = L"VALUE_ENDS_WITH";
            break;
        case COP_VALUE_CONTAINS:
            pszOperation = L"VALUE_CONTAINS";
            break;
        case COP_VALUE_NOTCONTAINS:
            pszOperation = L"VALUE_DOES_NOT_CONTAIN";
            break;
        case COP_DOSWILDCARDS:
            pszOperation = L"MATCHES";
            break;
        case COP_WORD_EQUAL:
            pszOperation = L"CONTAINS_WORD";
            break;
        case COP_WORD_STARTSWITH:
            pszOperation = L"CONTAINS_WORD_STARTING_WITH";
            break;
        case COP_APPLICATION_SPECIFIC:
            pszOperation = L"APPLICATION_SPECIFIC";
            break;
        default:
            pszOperation = L"???";
            break;
    }
    return pszOperation;
}

// Display one condition tree, with indentation.

HRESULT DisplayQuery(__in ICondition* pCondition, __in LONG lIndent)
{
    CONDITION_TYPE ct;
    HRESULT hr = pCondition->GetConditionType(&ct);
    if (SUCCEEDED(hr))
    {
        switch (ct)
        {
        case CT_AND_CONDITION:
        case CT_OR_CONDITION:
            {
                wprintf(L"%*s%s\n", 2*lIndent, L"", (ct == CT_AND_CONDITION ? L"AND" : L"OR"));
                IEnumUnknown* peuSubs;
                hr = pCondition->GetSubConditions(IID_PPV_ARGS(&peuSubs));
                if (SUCCEEDED(hr))
                {
                    IUnknown* punk;
                    while ((hr = peuSubs->Next(1, &punk, NULL)) == S_OK)
                    {
                        ICondition* pSubCondition;
                        hr = punk->QueryInterface(IID_PPV_ARGS(&pSubCondition));
                        if(SUCCEEDED(hr))
                        {
                            DisplayQuery(pSubCondition, lIndent+1);
                            pSubCondition->Release();
                        }
                        punk->Release();
                    }
                    peuSubs->Release();
                }
            }
            break;
        case CT_NOT_CONDITION:
            {
                wprintf(L"%*s%s\n", 2*lIndent, L"", L"NOT");
                // ICondition::GetSubConditions can return the single subcondition of
                // a negation node directly.
                ICondition* pSubCondition;
                hr = pCondition->GetSubConditions(IID_PPV_ARGS(&pSubCondition));
                if (SUCCEEDED(hr))
                {
                    DisplayQuery(pSubCondition, lIndent+1);
                    pSubCondition->Release();
                }
            }
            break;
        case CT_LEAF_CONDITION:
            {
                PWSTR pszPropertyName;
                CONDITION_OPERATION cop;
                PROPVARIANT propvar;
                hr = pCondition->GetComparisonInfo(&pszPropertyName, &cop, &propvar);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszNormalizedValue;
                    hr = pCondition->GetValueNormalization(&pszNormalizedValue);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszSemanticType;
                        hr = pCondition->GetValueType(&pszSemanticType);
                        if (SUCCEEDED(hr))
                        {
                            // It is permitted to have a leaf condition with null semantic type,
                            // so let us guard against that.
                            if (!pszSemanticType)
                            {
                                pszSemanticType = (WCHAR*)CoTaskMemAlloc(4 * sizeof WCHAR);
                                pszSemanticType[0] = pszSemanticType[1] = pszSemanticType[2] = L'?';
                                pszSemanticType[3] = L'\0';
                            }
                            wprintf(L"%*sLEAF %s %s %s (%s)\n", 2*lIndent, L"", pszPropertyName, GetLabel(cop), pszNormalizedValue, pszSemanticType);
                            CoTaskMemFree(pszSemanticType);
                        }
                        CoTaskMemFree(pszNormalizedValue);
                    }
                    CoTaskMemFree(pszPropertyName);
                    (void) PropVariantClear(&propvar);
                }
            }
            break;
        }
    }
    return hr;
}

int __cdecl wmain(__in int argc, __in_ecount(argc) WCHAR const* argv[])
{
    HRESULT hr;
    if (argc > 1)
    {
        wprintf(L"Usage: StructuredQuerySample1.exe\n\tThen enter queries to be parsed, one per line.\n\tAn empty line terminates the program.\n");
        hr = E_INVALIDARG;
    }
    else
    {
        hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if (SUCCEEDED(hr))
        {
            // It is possible to CoCreateInstance a query parser directly using __uuidof(QueryParser) but by
            // using a QueryParserManager we can get a query parser prepared for a certain language and catalog.
            IQueryParserManager* pQueryParserManager;
            hr = CoCreateInstance(__uuidof(QueryParserManager), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pQueryParserManager));
            if (SUCCEEDED(hr))
            {
                // The locale given to IQueryParserManager::CreateLoadedParser should be the user interface locale of the
                // application, which is usually the same as the operating system's locale for this user. It is used for
                // deciding what language to be used when interpreting keywords.
                IQueryParser* pQueryparser;
                hr = pQueryParserManager->CreateLoadedParser(L"SystemIndex", LOCALE_USER_DEFAULT, IID_PPV_ARGS(&pQueryparser));
                if (SUCCEEDED(hr))
                {
                    // This sample turns on "natural query syntax", which is the more relaxed and human language like
                    // search notation, and also makes searches automatically look for words beginning with what has been
                    // specified (so an input of "from bill" will search for anything from someone whose name beginns with
                    // "bill".
                    hr = pQueryParserManager->InitializeOptions(TRUE, TRUE, pQueryparser);
                    if (SUCCEEDED(hr))
                    {
                        WCHAR szLine[1024];
                        while (fgetws(szLine, ARRAYSIZE(szLine), stdin)[0] != L'\n')
                        {
                            // The actual work of parsing a query string is done here.
                            IQuerySolution* pQuerySolution;
                            hr = pQueryparser->Parse(szLine, NULL, &pQuerySolution);
                            if (SUCCEEDED(hr))
                            {
                                // In this sample we do not bother distinguishing between various
                                // parse errors though we could. Note that there will be a solution
                                // even though there were parse errors; it just may not be what the
                                // user intended.
                                IEnumUnknown* peu;
                                hr = pQuerySolution->GetErrors(IID_PPV_ARGS(&peu));
                                if (SUCCEEDED(hr))
                                {
                                    if (peu->Skip(1) == S_OK)
                                    {
                                        wprintf(L"Some part of the query string could not be parsed.\n");
                                    }
                                    ICondition* pCondition;
                                    hr = pQuerySolution->GetQuery(&pCondition, NULL);
                                    if (SUCCEEDED(hr))
                                    {
                                        // IQuerySolution::Resolve turns any relative time references (like
                                        // "today") into absolute time references (in the UTC time zone),
                                        // and also simplifies the result in various ways. Note that
                                        // pCondition is unchanged and can be resolved against additional
                                        // dates/times.
                                        SYSTEMTIME st;
                                        GetLocalTime(&st);
                                        ICondition* pConditionResolved;
                                        hr = pQuerySolution->Resolve(pCondition, 0, &st, &pConditionResolved);
                                        if (SUCCEEDED(hr))
                                        {
                                            hr = DisplayQuery(pConditionResolved, 0);
                                            pConditionResolved->Release();
                                        }
                                        pCondition->Release();
                                    }
                                    peu->Release();
                                }
                                pQuerySolution->Release();
                            }
                        }
                    }
                    pQueryparser->Release();
                }
                pQueryParserManager->Release();
            }
            CoUninitialize();
        }
    }
    return SUCCEEDED(hr) ? EXIT_SUCCESS : EXIT_FAILURE;
}
