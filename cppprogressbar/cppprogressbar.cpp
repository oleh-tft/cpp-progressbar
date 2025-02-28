#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <iostream>
#include <fstream>
#include <commctrl.h>
using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

HWND hInput, hOutput, hStart, hPause, hStop, hProgress;
HANDLE hThread;
TCHAR szInput[50], szOutput[50];

int CountCharacters(TCHAR* filename)
{
    ifstream file(filename, ios::binary);

    int charCounter = 0;
    char c;

    while (file.get(c)) {
        charCounter++;
    }

    file.close();
    return charCounter;
}

DWORD WINAPI Thread(LPVOID lp)
{
    ifstream in;
    ofstream out;
    HWND hWnd = HWND(lp);

    int lengthInput = SendMessage(hInput, WM_GETTEXTLENGTH, 0, 0);
    TCHAR* bufferInput = new TCHAR[lengthInput + 1];
    GetWindowText(hInput, bufferInput, lengthInput + 1);
    int lengthOutput = SendMessage(hOutput, WM_GETTEXTLENGTH, 0, 0);
    TCHAR* bufferOutput = new TCHAR[lengthOutput + 1];
    GetWindowText(hOutput, bufferOutput, lengthOutput + 1);

    int characters = CountCharacters(bufferInput);
    
    in.open(bufferInput);
    out.open(bufferOutput);

    char buffer;
    int pastedCharacters = 0;

    while(in.get(buffer))
    {
        out.put(buffer);
        pastedCharacters++;
        SendMessage(hWnd, PBM_SETPOS, (pastedCharacters / (double)characters) * 100, 0);
        Sleep(1);
    }

    in.close();
    out.close();

    delete[] bufferInput;
    delete[] bufferOutput;

    MessageBox(0, TEXT("Копіювання завершено"), TEXT("Стан"), MB_OK | MB_ICONINFORMATION);

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
        SetWindowText(hInput, TEXT("from.txt"));
        SetWindowText(hOutput, TEXT("to.txt"));

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