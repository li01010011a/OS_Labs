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

BOOL CALLBACK SeventhLabWndProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK SeventhLabWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) { // pārbaudīt ziņojuma identifikatoru  
    case WM_INITDIALOG: // ja tas ir inicializācijas paziņojums     
        SetTimer(hWnd, NULL, 200, NULL);
        return TRUE;
    case WM_COMMAND: // ja tas ir paziņojums no vadības elementa   
        switch (LOWORD(wParam)) { // pārbaudīt vadības elementa ID   
        case IDOK: // ja bija uzspiesta Close poga    
            CloseWindow(hWnd); // aizvērt logu    
            return TRUE;
        }
        return FALSE;
        //case WM_TIMER:
          //  ShowInfo(hWnd);
            //return TRUE;
    case WM_DESTROY: // ja tas ir pabeigšanas pieprasījums   
        CloseWindow(hWnd); // close ->this<- window   
        return TRUE;
    case WM_CLOSE: // Close button
    {
        if (MessageBox(hWnd, L"Do you not wanna see info about your computer?(", L"Your coumputer is wondering", MB_OKCANCEL) == IDOK) {
            CloseWindow(hWnd);
        }
        return TRUE;
    }
    }
    return FALSE;
}