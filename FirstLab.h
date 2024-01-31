#pragma once
//Author: Lidija Sokolova
//First Lab Operational Systems
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <stdio.h> 
#include "stdio.h"//stdio
#include "resource.h" 
#include <tchar.h>
#include <processthreadsapi.h>

BOOL CALLBACK CompInfoWndProc(HWND, UINT, WPARAM, LPARAM);
void ShowInfo(HWND wnd);

BOOL CALLBACK CompInfoWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) { // pārbaudīt ziņojuma identifikatoru  
    case WM_INITDIALOG: // ja tas ir inicializācijas paziņojums     
        SetTimer(hWnd, NULL, 200, NULL);
        ShowInfo(hWnd);				// izpildīt inicializācijas darbības   
        return TRUE;
    case WM_COMMAND: // ja tas ir paziņojums no vadības elementa   
        switch (LOWORD(wParam)) { // pārbaudīt vadības elementa ID   
        case IDOK: // ja bija uzspiesta Close poga    
            CloseWindow(hWnd); // aizvērt logu    
            return TRUE;
        }
        return FALSE;
    case WM_TIMER:
        ShowInfo(hWnd);
        return TRUE;
    case WM_DESTROY: // ja tas ir pabeigšanas pieprasījums   
        CloseWindow(hWnd); // close ->this<- window   
        return TRUE;
    case WM_CLOSE: // Close button
    {
        if (MessageBox(hWnd, L"Do you not wanna see info about your computer?(", L"Your coumputer is wondering", MB_OKCANCEL) == IDOK) {
            CloseWindow(hWnd);
        }
        return 0;
    }
    }
    return FALSE;
}

void ShowInfo(HWND wnd) {
    // Computer Name
    TCHAR buf[100];
    DWORD size = 100;
    //if (!GetComputerName(buf, &size)) strcpy_s(buf, "error");
    GetComputerName(buf, &size);
    SetDlgItemText(wnd, IDC_COMPNAME, buf);
    // User Name
    //if (!GetUserName(buf, &size)) strcpy_s(buf, "error");
    GetUserName(buf, &size);
    SetDlgItemText(wnd, IDC_USERNAME, buf);
    // OS version

    swprintf_s(buf, L"%s", L"Windows 10");
    SetDlgItemText(wnd, IDC_VERSION, buf);
    // Physical memory
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    swprintf_s(buf, L"%I64d MB", statex.ullTotalPhys / 1024);
    SetDlgItemText(wnd, IDC_TOTALRAM, buf);
    // Free memory
    swprintf_s(buf, L"%I64d MB", statex.ullAvailPhys / 1024);
    SetDlgItemText(wnd, IDC_FREERAM, buf);
    // Percent of memory in use. GlobalMemoryStatusEx
    swprintf_s(buf, L"%I64d MB", statex.dwMemoryLoad);
    SetDlgItemText(wnd, IDC_PERCENTRAM, buf);
    // Current Date and Time. GetLocalTime
    SYSTEMTIME sm;
    GetLocalTime(&sm);
    swprintf_s(buf, L"%d:%d:%d %d.%d.%d", sm.wHour, sm.wMinute, sm.wSecond, sm.wDay, sm.wMonth, sm.wYear);
    SetDlgItemText(wnd, IDC_DATETIME, buf);
    // Time since power on. GetTickCount
    DWORD tm = GetTickCount();
    swprintf_s(buf, L"%d minutes", tm / 1000 / 60);
    SetDlgItemText(wnd, IDC_TIMEON, buf);
}