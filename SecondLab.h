#pragma once
//Author: Lidija Sokolova
//Labs Operational Systems
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <stdio.h> 
#include "resource.h" 
#include <processthreadsapi.h>

#pragma warning(disable : 4996)

BOOL CALLBACK SecondLabWndProc(HWND, UINT, WPARAM, LPARAM);//Second Lab <Procesa Palaisana>
void BrowseButtonPressed(HWND nWnd);
BOOL BrowseFileName(HWND Wnd, wchar_t* fileName);
BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint);
bool CreateNewProcess(wchar_t* commandLine, HANDLE& hProc);
void TerminateButtonPressed(HWND hWnd, HANDLE& hProc);
void ProcessFinishedState(HWND hWnd);
void ProcessStartedState(HWND hWnd);
void UpdateButtonState(HWND hWnd, BOOL procState);
void StartButtonPressed(HWND hWnd, HANDLE& hProc);
void ProcessState(HWND hWnd, BOOL procState);

//typedef wchar_t* LPWSTR, * PWSTR;
HANDLE hProc = NULL;
BOOL CALLBACK SecondLabWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
    case WM_INITDIALOG:
        SetTimer(hWnd, NULL, 200, NULL);
        UpdateButtonState(hWnd, 0);
        return TRUE;
    case WM_COMMAND: 
        switch (LOWORD(wParam)) { 
        case IDOK: 
            CloseWindow(hWnd);  
            return TRUE;
        case IDC_START:
            StartButtonPressed(hWnd, hProc);
            return TRUE;
        case IDC_BROWSE:
            BrowseButtonPressed(hWnd);
            return TRUE;
        case IDC_TERMINATE:
            TerminateButtonPressed(hWnd, hProc);
            return TRUE;
        case WM_TIMER:
            DWORD lpExitCode = 0;
            GetExitCodeProcess(hProc, &lpExitCode);
            UpdateButtonState(hWnd, lpExitCode == STILL_ACTIVE);
            ProcessState(hWnd, lpExitCode == STILL_ACTIVE);
            return TRUE;
        }
        return FALSE;
    case WM_DESTROY: //    
        CloseWindow(hWnd); // close ->this<- window   
        return TRUE;
    case WM_CLOSE: // Close button
    {
        if (MessageBox(hWnd, L"Do you actually wanna leave?:(", L"Your coumputer is wondering", MB_OKCANCEL) == IDOK) {
            CloseWindow(hWnd);
        }
        return 0;
    }
    }
    return FALSE;
}

void ProcessStartedState(HWND hWnd) {
    SetDlgItemText(hWnd, IDC_STATE, L"Process is running");
    FILETIME lpCreationTime;
    FILETIME lpExitTime;
    FILETIME lpKernelTime;
    FILETIME lpUserTime;
    GetProcessTimes(hProc, &lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime);
    SYSTEMTIME lpSystemTime;
    FileTimeToSystemTime(&lpCreationTime, &lpSystemTime);
    wchar_t startTimeStr[100];
    swprintf(startTimeStr, sizeof(startTimeStr), L"%02d:%02d:%02d", lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
    SetDlgItemText(hWnd, IDC_STARTTIME, startTimeStr);
}

void ProcessFinishedState(HWND hWnd) {
    SetDlgItemText(hWnd, IDC_STATE, L"Process is finished");

    // Current Time
    SYSTEMTIME finishTime;
    GetLocalTime(&finishTime);

    wchar_t finishTimeStr[100];
    swprintf(finishTimeStr, sizeof(finishTimeStr), L"%02d:%02d:%02d", finishTime.wHour, finishTime.wMinute, finishTime.wSecond);
    SetDlgItemText(hWnd, IDC_FINISHTIME, finishTimeStr);
}

void ProcessState(HWND hWnd, BOOL procState) {
    if (procState) {
        SetDlgItemText(hWnd, IDC_STATE, L"Process is running");
    }
    else {
        SetDlgItemText(hWnd, IDC_STATE, L"Process is finished");
    }
}

void UpdateButtonState(HWND hWnd, BOOL procState) {
    HWND hStartButton = GetDlgItem(hWnd, IDC_START);
    HWND hTerminateButton = GetDlgItem(hWnd, IDC_TERMINATE);
    if (procState) {
        EnableWindow(hStartButton, FALSE);
        EnableWindow(hTerminateButton, TRUE);
    }
    else { 
        EnableWindow(hStartButton, TRUE); 
        EnableWindow(hTerminateButton, FALSE);
    }
}

void StartButtonPressed(HWND hWnd, HANDLE& hProc) {
    wchar_t commandLine[MAX_PATH];
    GetDlgItemText(hWnd, IDC_COMMANDLINE, commandLine, MAX_PATH);
    CreateNewProcess(commandLine, hProc);
    ProcessStartedState(hWnd);
    SetDlgItemInt(hWnd, IDC_PID, GetProcessId(hProc), FALSE);
    DWORD lpExitCode = 0;
    GetExitCodeProcess(hProc, &lpExitCode);
    UpdateButtonState(hWnd, lpExitCode==STILL_ACTIVE);
}

void TerminateButtonPressed(HWND hWnd, HANDLE& hProc) {
    if (hProc) {
        TerminateProcess(hProc, 0);
        CloseHandle(hProc);
        hProc = NULL;
    }
    ProcessFinishedState(hWnd);
    DWORD lpExitCode = 0;
    GetExitCodeProcess(hProc, &lpExitCode);
    UpdateButtonState(hWnd, lpExitCode == STILL_ACTIVE);
    SetDlgItemText(hWnd, IDC_PID, L"-");
}

void BrowseButtonPressed(HWND hWnd) {
    wchar_t fileName[MAX_PATH] = L"";
    if (BrowseFileName(hWnd, fileName)) {
        SetDlgItemText(hWnd, IDC_COMMANDLINE, fileName);
    }
}

BOOL BrowseFileName(HWND Wnd, wchar_t* fileName) {
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = Wnd;
    ofn.lpstrFilter = L"Executable Files (*.exe)\0*.exe\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = L"exe";
    return GetOpenFileName(&ofn);
}

bool CreateNewProcess(wchar_t* commandLine, HANDLE& hProc) {
    STARTUPINFO si; // struktūras mainīgais STARTUPINFO tipa
    PROCESS_INFORMATION pi; // struktūras mainīgais PROCESS_INFORMATION tipa
    ZeroMemory(&si, sizeof(si)); // ierakstīt nulles visos si laukos
    si.cb = sizeof(si); // ierakstīt STARTUPINFO izmēru laukā cb
    ZeroMemory(&pi, sizeof(pi));
    if (CreateProcess( // palaist procesu
        NULL, // faila vārds būs norādīts komandrindā
        commandLine, // komandrinda procesa palaišanai
        NULL, // procesa turis nav mantojams
        NULL, // pavediena turis nav mantojams
        FALSE, // tekoša procesa objekti nav mantojami
        0, // nav papildu parametru
        NULL, // izmantot tekošo apkārtnes bloku
        NULL, // izmantot tekošo darba direktoriju
        &si, // rādītājs uz STARTUPINFO struktūru
        &pi) // rādītājs uz PROCESS_INFORMATION struktūru
        )
    { // ja jaunais process palaists veiksmīgi
        if (hProc)
            CloseHandle(hProc);
        hProc = pi.hProcess;
        CloseHandle(pi.hThread); // aizvērt pavediena turu
        return true;
    }
    return false;
}