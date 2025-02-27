#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <iostream>
#include <fstream>
using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

HWND hInput, hOutput, hStart, hPause, hStop, hProgress;
HANDLE hThread;
TCHAR szInput[50], szOutput[50];

DWORD WINAPI Thread(LPVOID lp)
{
    ofstream file;
    HWND hWnd = HWND(lp);
    TCHAR buf[999999999];

    int lengthInput = SendMessage(hInput, WM_GETTEXTLENGTH, 0, 0);
    TCHAR* bufferInput = new TCHAR[lengthInput + 1];
    GetWindowText(hInput, bufferInput, lengthInput + 1);
    
    file.open(bufferInput);
    



    return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        hInput = GetDlgItem(hWnd, IDC_EDIT1);
        hOutput = GetDlgItem(hWnd, IDC_EDIT2);
        hStart = GetDlgItem(hWnd, IDC_BUTTON1);
        hPause = GetDlgItem(hWnd, IDC_BUTTON2);
        hStop = GetDlgItem(hWnd, IDC_BUTTON3);
        hProgress = GetDlgItem(hWnd, IDC_PROGRESS1);
        SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(hProgress, PBM_SETSTEP, 1, 0);
        SendMessage(hProgress, PBM_SETPOS, 0, 0);
        SetWindowText(hInput, TEXT("👆.txt"));
        SetWindowText(hOutput, TEXT("👉.txt"));

        hThread = CreateThread(NULL, 0, Thread, hProgress, CREATE_SUSPENDED, NULL);

        return TRUE;
    case WM_COMMAND:
        if (wParam == IDC_BUTTON1)
        {
            ResumeThread(hThread);
        }
        else if (wParam == IDC_BUTTON2)
        {
            SuspendThread(hThread);
        }
        else if (wParam == IDC_BUTTON3)
        {
            TerminateThread(hThread, 0);
        }

        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}