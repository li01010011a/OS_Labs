//Author: Lidija Sokolova
//Labs Operational Systems
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <stdio.h> 
#include "stdio.h"//stdio
#include "resource.h" 
#include <tchar.h>
#include <processthreadsapi.h>
#include "FirstLab.h"
#include "SecondLab.h"
#include "ThirdLab.h"
#include "FourthLab.h"
#include "FifthLab.h"
#include "SixthLab.h"
#include "SeventhLab.h"
#pragma warning(disable : 4996)  // Ziņojumu apstrādātāja prototips 
#define IDC_INFO_BUTTON 101
#define IDC_SECOND_LAB_BUTTON 102
#define IDC_THIRD_LAB_BUTTON 103
#define IDC_FOURTH_LAB_BUTTON 104
#define IDC_FIFTH_LAB_BUTTON 105
#define IDC_SIXTH_LAB_BUTTON 106
#define IDC_SEVENTH_LAB_BUTTON 107

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = { };
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    
    wc.lpfnWndProc = MainWndProc;  //points to WindowProc func
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = hIcon;
    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class . This name determines type of created window
        L"Main Window (created with 1st approach)",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    if (hwnd == NULL)
    {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow); //hwnd - window descriptor returned by CreateWindowsEx

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg); //keyboard press->to symbols
        DispatchMessage(&msg);  //manage WindowProc func, calling it once per msg
    }
    return 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        TCHAR author_info[] = _T("Author: Lidija Sokolova\nSubject: Operational Systems (Labs)");
        // All painting occurs here, between BeginPaint and EndPaint.
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        TextOut(hdc, 400, 250, author_info, _tcslen(author_info));
        EndPaint(hwnd, &ps);
        //return TRUE;
    }
    case WM_CREATE:
    {
        HWND info_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Show Info",
            WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,
            50,300,100,100,hwnd, (HMENU)IDC_INFO_BUTTON, NULL, NULL
        );
        HWND second_lab_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Create Process",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            200, 300, 100, 100, hwnd, (HMENU)IDC_SECOND_LAB_BUTTON, NULL, NULL
        );
        HWND third_lab_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Threads",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            350, 300, 100, 100, hwnd, (HMENU)IDC_THIRD_LAB_BUTTON, NULL, NULL
        );
        HWND fourth_lab_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Critical sections",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            500, 300, 100, 100, hwnd, (HMENU)IDC_FOURTH_LAB_BUTTON, NULL, NULL
        );
        HWND fifth_lab_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Win notific",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            650, 300, 100, 100, hwnd, (HMENU)IDC_FIFTH_LAB_BUTTON, NULL, NULL
        );
        HWND sixth_lab_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Semafori",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            800, 300, 100, 100, hwnd, (HMENU)IDC_SIXTH_LAB_BUTTON, NULL, NULL
        );
        HWND seventh_lab_button = CreateWindowEx(
            0,
            L"BUTTON",
            L"Post slots",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            950, 300, 100, 100, hwnd, (HMENU)IDC_SEVENTH_LAB_BUTTON, NULL, NULL
        );
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_INFO_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwndinfo = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAINCOMPINFO),
                hwnd,
                DLGPROC(CompInfoWndProc)
            );
            ShowWindow(hwndinfo, SW_SHOW);
            return TRUE;
        }
        case IDC_SECOND_LAB_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwnd_second_lab = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAIN_SECOND_LAB),
                hwnd,
                DLGPROC(SecondLabWndProc)
            );
            ShowWindow(hwnd_second_lab, SW_SHOW);
            return TRUE;
        }
        case IDC_THIRD_LAB_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwnd_third_lab = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAIN_THIRD_LAB),
                hwnd,
                DLGPROC(ThirdLabWndProc)
            );
            ShowWindow(hwnd_third_lab, SW_SHOW);
            return TRUE;
        }
        case IDC_FOURTH_LAB_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwnd_fourth_lab = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAIN_FOURTH_LAB),
                hwnd,
                DLGPROC(FourthLabWndProc)
            );
            ShowWindow(hwnd_fourth_lab, SW_SHOW);
            return TRUE;
        }
        case IDC_FIFTH_LAB_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwnd_fifth_lab = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAIN_FIFTH_LAB),
                hwnd,
                DLGPROC(FifthLabWndProc)
            );
            ShowWindow(hwnd_fifth_lab, SW_SHOW);
            return TRUE;
        }
        case IDC_SIXTH_LAB_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwnd_sixth_lab = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAIN_SIXTH_LAB),
                hwnd,
                DLGPROC(SixthLabWndProc)
            );
            ShowWindow(hwnd_sixth_lab, SW_SHOW);
            return TRUE;
        }
        case IDC_SEVENTH_LAB_BUTTON:
        {
            //SendMessage(hwnd, WM_CLOSE,0,0);
            HWND hwnd_seventh_lab = CreateDialog(
                NULL,
                MAKEINTRESOURCE(IDD_MAIN_SEVENTH_LAB),
                hwnd,
                DLGPROC(SeventhLabWndProc)
            );
            ShowWindow(hwnd_seventh_lab, SW_SHOW);
            return TRUE;
        }
        }
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
