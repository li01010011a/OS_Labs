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
#include <Commctrl.h>

#pragma warning(disable : 4996)
/*************** [ FUNCTION DECLARATIONS ] *************/
BOOL CALLBACK ThirdLabWndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ClockThread(LPVOID lpParameter);
DWORD WINAPI ScheduleThread(LPVOID lpParameter);
void AddButtonPressed(HWND hWnd);
void DeleteButtonPressed(HWND hWnd);

/****************[ GLOBAL VARIABLES ]*******************/
bool Terminate = false;     // pavedienu pabeigšanas pieprasījuma karodziņš
HWND hMainWnd = 0;          // galvenā loga turis, kas jāsaglabā
                            // ziņojuma WM_INITDIALOG apstrādāšanas laikā
HANDLE hClock = 0;          // pulksteņa pavediena turis
bool ClockPaused = false;   // vai pulkstenis ir apstādināts
HBITMAP hBitmapClockPaused = NULL;  // Pause
HBITMAP hBitmapClockRunning = NULL; // Play
unsigned __int64 SystemTimeToInt(SYSTEMTIME Time);
bool LessThanCurrentTime(SYSTEMTIME ScheduleTime);

/* [ All FONT Stuff ] */
HANDLE hFont = CreateFont(
    20,                 // Augstums
    0,                  // Platums
    0,                  // Leņķis
    0,                  // Leņķa punkts
    FW_BOLD,          // Biezums
    FALSE,              // Itālija
    FALSE,              // Pasvītrojums
    FALSE,              // Podeinums
    DEFAULT_CHARSET,    // Rakstzīmju kopa
    OUT_DEFAULT_PRECIS, // Precizitāte
    CLIP_DEFAULT_PRECIS,// Izgriezums
    DEFAULT_QUALITY,    // Kvalitāte
    DEFAULT_PITCH | FF_DONTCARE, // Izstiepums un šrifta tips
    L"Arial"            // Šrifta nosaukums
);

BOOL CALLBACK ThirdLabWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    HANDLE hPauseIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON3));
    HANDLE hPlayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    switch (Msg) {   
    case WM_INITDIALOG:     
        SetTimer(hWnd, NULL, 200, NULL);
        hMainWnd = hWnd;
        hClock = CreateThread(NULL, 0, ClockThread, NULL, 0, NULL);
        SendMessage(GetDlgItem(hWnd, IDC_PAUSE), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hPauseIcon);
        if (hFont != NULL) SendDlgItemMessage(hWnd, IDC_CLOCK, WM_SETFONT, (WPARAM)hFont, TRUE);
        //SetTextColor((HDC)wParam, RGB(0, 0, 250));
        return TRUE;
    case WM_COMMAND:    
        switch (LOWORD(wParam)) { 
        case IDC_BROWSE:
            BrowseButtonPressed(hWnd);
            return TRUE;
        case IDC_ADD:
            AddButtonPressed(hWnd);
            return TRUE;
        case IDC_DELETE:
            DeleteButtonPressed(hWnd);
            return TRUE;
        case IDC_PAUSE:
            InvalidateRect(hWnd, NULL, TRUE);
            if (!ClockPaused) {
                SuspendThread(hClock);
                SendMessage(GetDlgItem(hWnd, IDC_PAUSE), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hPlayIcon);
                ClockPaused = true;
            }
            else
            {
                ResumeThread(hClock);
                SendMessage(GetDlgItem(hWnd, IDC_PAUSE), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hPauseIcon);
                ClockPaused = false;
            }
            return TRUE;
        case WM_CTLCOLORSTATIC:
            if (GetDlgCtrlID((HWND)lParam) == IDC_PAUSE) {
                SetTextColor((HDC)wParam, RGB(0, 0, 250));
                return (INT_PTR)GetSysColorBrush(COLOR_BTNFACE);
            }
            return TRUE;
        case IDOK:    
            CloseWindow(hWnd);   
            return TRUE;
        }
        return FALSE;
    case WM_TIMER:
        return TRUE;
    case WM_CTLCOLORSTATIC:
        if (GetDlgCtrlID((HWND)lParam) == IDC_CLOCK) {
            if (ClockPaused) {
                SetTextColor((HDC)wParam, RGB(255, 0, 0));
                return (INT_PTR)GetSysColorBrush(COLOR_BTNFACE);
            }
                
            else {
                SetTextColor((HDC)wParam, RGB(0, 0, 255));
                return (INT_PTR)GetSysColorBrush(COLOR_BTNFACE);
            }
                
        }
        return TRUE;
    case WM_DESTROY: // ja tas ir pabeigšanas pieprasījums  
        Terminate = true;
        Sleep(500);
        CloseHandle(hClock);
        CloseWindow(hMainWnd); // close ->this<- window   
        DeleteObject(hFont);
        return TRUE;
    case WM_CLOSE: // Close button
    {
        if (MessageBox(hWnd, L"Do you actually want leave?(", L"Your coumputer is wondering", MB_OKCANCEL) == IDOK) {
            CloseWindow(hWnd);
        }
        return TRUE;
    }
    }
    return FALSE;
}

void DeleteButtonPressed(HWND hWnd) {
    int Pos = SendDlgItemMessage(
        hWnd, IDC_TIMELIST, LB_GETCURSEL, 0, 0
    );
    SendDlgItemMessage(
        hWnd, IDC_TIMELIST, LB_DELETESTRING, Pos, 0
    );
}

void AddButtonPressed(HWND hWnd) {
    SYSTEMTIME Time;
    SendDlgItemMessage(
        hWnd, IDC_TIME, DTM_GETSYSTEMTIME, 0,
        (LPARAM)&Time
    );
    if (!LessThanCurrentTime(Time)) {
        wchar_t TimeStr[100];
        swprintf(TimeStr, sizeof(TimeStr), L"%02d:%02d:%02d", Time.wHour, Time.wMinute, Time.wSecond);
        SendDlgItemMessage(
            hWnd, IDC_TIMELIST, LB_ADDSTRING, 0, (LPARAM)TimeStr
        );
        CloseHandle(CreateThread(0, 0, ScheduleThread, 0, 0, 0));
    }
}

bool LessThanCurrentTime(SYSTEMTIME ScheduleTime) {
    SYSTEMTIME CurrentTime;
    GetLocalTime(&CurrentTime);
    unsigned __int64 ftCurrentTime = SystemTimeToInt(CurrentTime);
    unsigned __int64 ftScheduleTime = SystemTimeToInt(ScheduleTime);
    if (ftCurrentTime > ftScheduleTime) return true;
    return false;
}

unsigned __int64 SystemTimeToInt(SYSTEMTIME Time) {
    FILETIME ft;
    SystemTimeToFileTime(&Time, &ft);
    ULARGE_INTEGER fti;
    fti.HighPart = ft.dwHighDateTime;
    fti.LowPart = ft.dwLowDateTime;
    return fti.QuadPart;
}

DWORD WINAPI ClockThread(LPVOID lpParameter) {
    while (!Terminate) {
        wchar_t timestr[9];
        SYSTEMTIME time;
        GetLocalTime(&time);
        swprintf(timestr, L"%.2d:%.2d:%.2d",
            time.wHour, time.wMinute, time.wSecond);
        SetDlgItemText(hMainWnd, IDC_CLOCK, timestr);
        Sleep(250);
    }
    return 0;
}

bool TimeIsInList(SYSTEMTIME &Time) {
    wchar_t TimeStr[100];
    swprintf(TimeStr, sizeof(TimeStr), L"%02d:%02d:%02d", Time.wHour, Time.wMinute, Time.wSecond);

    int Pos = SendDlgItemMessage(
        hMainWnd, IDC_TIMELIST, LB_FINDSTRINGEXACT, -1, (LPARAM)TimeStr
    );
    if (Pos != LB_ERR) return true;
    return false;
}

int GetPosInList(SYSTEMTIME& Time) {
    wchar_t TimeStr[100];
    swprintf(TimeStr, sizeof(TimeStr), L"%02d:%02d:%02d", Time.wHour, Time.wMinute, Time.wSecond);

    int Pos = SendDlgItemMessage(
        hMainWnd, IDC_TIMELIST, LB_FINDSTRINGEXACT, -1, (LPARAM)TimeStr
    );
    return Pos;
}

DWORD WINAPI ScheduleThread(LPVOID lpParameter) {
    SYSTEMTIME ScheduleTime;
    SendDlgItemMessage(
        hMainWnd, IDC_TIME, DTM_GETSYSTEMTIME, 0,
        (LPARAM)&ScheduleTime
    );
    while (!Terminate && !LessThanCurrentTime(ScheduleTime)) {//true if current bigger
        Sleep(10);
    }
    if (!Terminate && TimeIsInList(ScheduleTime)) {
        wchar_t commandLine[MAX_PATH];
        GetDlgItemText(hMainWnd, IDC_COMMANDLINE, commandLine, MAX_PATH);
        STARTUPINFO si; // struktūras mainīgais STARTUPINFO tipa
        PROCESS_INFORMATION pi; // struktūras mainīgais PROCESS_INFORMATION tipa
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        CreateProcess(0, commandLine, 0, 0, 0, 0, 0, 0, &si, &pi);
        HANDLE hProcess = pi.hProcess;
        WaitForSingleObject(hProcess, INFINITE);
        CloseHandle(hProcess);
        SendDlgItemMessage(
            hMainWnd, IDC_TIMELIST, LB_DELETESTRING, GetPosInList(ScheduleTime), 0
        );
    }
    return 0;
}