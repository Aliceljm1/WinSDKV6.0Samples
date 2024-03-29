/* Copyright (c) Microsoft Corporation. All rights reserved. */

#include "Erase2Event.h"

// ICDWriteEngine2Event methods

VOID DeleteCurrentLine2()
{
    printf(//"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
           "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
           "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
           "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
           );
    return;
}
VOID OverwriteCurrentLine2()
{
    printf("                                        "
           "                                        "
           );
    return;
}
VOID
UpdatePercentageDisplay2(
    IN ULONG Numerator,
    IN ULONG Denominator
    )
{
    ULONG percent;
    ULONG i;

    if (Numerator > Denominator) {
        return;
    }

    // NOTE: Overflow possibility exists for large numerators.

    percent = (Numerator * 100) / Denominator;

    // each block is 2%
    // ----=----1----=----2----=----3----=----4----=----5----=----6----=----7----=----8
    // �.....................

    for (i=1; i<100; i+=2) {
        if (i < percent) {
            putchar(178);
        } else if (i == percent) {
            putchar(177);
        } else {
            putchar(176);
        }
    }
    printf(" %d%%", percent);
}

STDMETHODIMP_(VOID) CTestErase2Event::Update(IDispatch* object, LONG elapsedTime, LONG totalTime)
{
    UNREFERENCED_PARAMETER (object);

    DeleteCurrentLine2();
    UpdatePercentageDisplay2(elapsedTime, totalTime);

    return;
}
