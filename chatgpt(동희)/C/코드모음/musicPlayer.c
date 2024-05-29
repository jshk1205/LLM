// 프로젝트 -> 속성 - > 링커 - > 입력 - > winm.lib 종속성추가해야됨

#include <windows.h>
#include <mmsystem.h>
#include <commdlg.h>

#pragma comment(lib, "winmm.lib")

// Global variables
HWND hPlayButton, hPauseButton, hStopButton, hSelectButton;
TCHAR szFileName[MAX_PATH] = TEXT("");

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void PlayMusic();
void PauseMusic();
void StopMusic();
BOOL GetFileName(HWND hwnd, TCHAR* szFileName, DWORD dwSize);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = NULL; // Set background to NULL
    wc.lpszClassName = TEXT("MusicPlayer");

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create window
    HWND hWnd = CreateWindow(TEXT("MusicPlayer"), TEXT("Simple Music Player"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL); // Increase window size

    if (hWnd == NULL) {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create buttons
    hPlayButton = CreateWindow(TEXT("BUTTON"), TEXT("Play"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        30, 50, 80, 30, hWnd, NULL, hInstance, NULL);

    hPauseButton = CreateWindow(TEXT("BUTTON"), TEXT("Pause"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        130, 50, 80, 30, hWnd, NULL, hInstance, NULL);

    hStopButton = CreateWindow(TEXT("BUTTON"), TEXT("Stop"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        230, 50, 80, 30, hWnd, NULL, hInstance, NULL);

    hSelectButton = CreateWindow(TEXT("BUTTON"), TEXT("Select"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        330, 50, 80, 30, hWnd, NULL, hInstance, NULL); // Adjust position

    // Show window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if ((HWND)lParam == hPlayButton) {
            PlayMusic();
        }
        else if ((HWND)lParam == hPauseButton) {
            PauseMusic();
        }
        else if ((HWND)lParam == hStopButton) {
            StopMusic();
        }
        else if ((HWND)lParam == hSelectButton) {
            if (GetFileName(hWnd, szFileName, MAX_PATH)) {
                MessageBox(hWnd, szFileName, TEXT("Selected File"), MB_OK);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

void PlayMusic() {
    if (szFileName[0] != TEXT('\0')) {
        PlaySound(szFileName, NULL, SND_FILENAME | SND_ASYNC);
    }
}

void PauseMusic() {
    PlaySound(NULL, NULL, SND_ASYNC);
}

void StopMusic() {
    PlaySound(NULL, NULL, 0);
}

BOOL GetFileName(HWND hwnd, TCHAR* szFileName, DWORD dwSize) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = TEXT("Audio Files\0*.wav;*.mp3;*.ogg;*.flac\0All Files\0*.*\0");
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = dwSize;
    ofn.lpstrTitle = TEXT("Select an audio file");
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    return GetOpenFileName(&ofn);
}
