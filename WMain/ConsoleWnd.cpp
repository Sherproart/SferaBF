#include "stdafx.h"
#include <stdio.h>
bool fConsole = false;
FILE* stream;

void ShowConsoleWindow() {
    AllocConsole();
    freopen_s(&stream,"CONOUT$", "wb", stdout);
    freopen_s(&stream,"CONIN$", "rb", stdin);
    // Заголовок консоли совпадает с полным именем exe-шника
    TCHAR fullName[MAX_PATH];
    GetModuleFileName(NULL, fullName, MAX_PATH);
    HWND hWnd = FindWindow(NULL, fullName);
    SetConsoleTitle(_T("Attached to workGL.exe"));
    RECT rect;
    GetWindowRect(hWnd, &rect);
    //Shell_TrayWnd
    RECT desktopArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopArea, 0);

    //int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    //int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hWnd, HWND_TOP, 0, desktopArea.bottom - rect.bottom + rect.top, 0, 0, SWP_NOSIZE);
    fConsole = true;
}
