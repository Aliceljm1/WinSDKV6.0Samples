#include <windows.h>

BOOL CreateAndVerifyReason(HWND hWnd, WCHAR *pwszReason)
{
    BOOL fSuccess = FALSE;
    DWORD cch = 0;

    // Register a blocking reason for the window.
    ShutdownBlockReasonCreate(hWnd, pwszReason);

    // Verify that the registration went through.
    // Query the length first.
    if (ShutdownBlockReasonQuery(hWnd, NULL, &cch))
    {
        // Allocate memory space to retrive the string.
        WCHAR *pch = new WCHAR[cch];
        if (NULL != pch)
        {
            // Get the string and compare.
            if (ShutdownBlockReasonQuery(hWnd, pch, &cch))
            {
                fSuccess = (0 == lstrcmpW(pwszReason, pch));
            }

            delete[] pch;
        }
    }

    return fSuccess;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    switch (nMsg)
    {
    case WM_QUERYENDSESSION:
        if (!CreateAndVerifyReason(hWnd, L"You have unsaved data."))
        {
            // Handle the error here.
        }

        // Tell the system we like to block the shutdown.
        return FALSE;

    case WM_DESTROY:
        // Unregister the reason string.
        ShutdownBlockReasonDestroy(hWnd);

        // Stop the message pump.
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex = {0};
    HWND hWnd;
    MSG msg;

    // Register a window class and create a window from it.
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_GLOBALCLASS;
    wcex.hInstance = hInst;
    wcex.hCursor = LoadCursor(NULL, (LPTSTR)IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = TEXT("Sample Class");
    wcex.lpfnWndProc = WndProc;

    if (RegisterClassEx(&wcex))
    {
        hWnd = CreateWindow(TEXT("Sample Class"), TEXT("Sample Program"),
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
                            NULL, NULL, NULL, NULL);
        if (NULL != hWnd)
        {
            ShowWindow(hWnd, nCmdShow);

            // Run the message pump.
            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}
