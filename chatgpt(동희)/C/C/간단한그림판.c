#include <windows.h>
#include <commdlg.h>
#include <stdio.h>  // 파일 입출력을 위한 헤더

#define ID_FILE_SAVE 1
#define ID_COLOR_PICKER 2

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SaveBitmap(HWND);
void PickColor(HWND);

COLORREF currentColor = RGB(0, 0, 0); // Black
HPEN hPen;
HBITMAP hBitmap;
HDC hdcMem;
PAINTSTRUCT ps;
POINT ptPrevious = { -1, -1 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("SimplePaint");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("간단한 그림판"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HDC hdc;
    static RECT rect;
    static int cxClient, cyClient;

    switch (message) {
    case WM_CREATE:
        hPen = CreatePen(PS_SOLID, 1, currentColor);
        break;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        hdc = GetDC(hwnd);
        hBitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);
        hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);
        PatBlt(hdcMem, 0, 0, cxClient, cyClient, WHITENESS);
        ReleaseDC(hwnd, hdc);
        break;

    case WM_LBUTTONDOWN:
        ptPrevious.x = LOWORD(lParam);
        ptPrevious.y = HIWORD(lParam);
        return 0;

    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) {
            hdc = GetDC(hwnd);
            SelectObject(hdcMem, hPen);
            MoveToEx(hdcMem, ptPrevious.x, ptPrevious.y, NULL);
            LineTo(hdcMem, LOWORD(lParam), HIWORD(lParam));
            ptPrevious.x = LOWORD(lParam);
            ptPrevious.y = HIWORD(lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            ReleaseDC(hwnd, hdc);
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_FILE_SAVE:
            SaveBitmap(hwnd);
            break;
        case ID_COLOR_PICKER:
            PickColor(hwnd);
            break;
        }
        break;

    case WM_DESTROY:
        DeleteObject(hPen);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void SaveBitmap(HWND hwnd) {
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = TEXT("\0");

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = TEXT("Bitmap Files (*.bmp)\0*.bmp\0");
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = TEXT("bmp");

    if (GetSaveFileName(&ofn)) {
        BITMAPFILEHEADER fileHeader;
        BITMAPINFOHEADER infoHeader;
        HDC hdc;
        FILE* fp;
        errno_t err;

        hdc = GetDC(hwnd);

        int cxClient = GetDeviceCaps(hdc, HORZRES);
        int cyClient = GetDeviceCaps(hdc, VERTRES);

        fileHeader.bfType = 0x4D42;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        infoHeader.biSize = sizeof(BITMAPINFOHEADER);
        infoHeader.biWidth = cxClient;
        infoHeader.biHeight = cyClient;
        infoHeader.biPlanes = 1;
        infoHeader.biBitCount = 24;
        infoHeader.biCompression = BI_RGB;
        infoHeader.biSizeImage = ((cxClient * 3 + 3) & ~3) * cyClient;
        infoHeader.biXPelsPerMeter = 0;
        infoHeader.biYPelsPerMeter = 0;
        infoHeader.biClrUsed = 0;
        infoHeader.biClrImportant = 0;

        fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

        if ((err = fopen_s(&fp, szFile, "wb")) == 0) {
            fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
            fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

            BYTE* pPixels = (BYTE*)malloc(infoHeader.biSizeImage);

            GetDIBits(hdc, hBitmap, 0, cyClient, pPixels, (BITMAPINFO*)&infoHeader, DIB_RGB_COLORS);

            fwrite(pPixels, infoHeader.biSizeImage, 1, fp);

            free(pPixels);
            fclose(fp);
        }

        ReleaseDC(hwnd, hdc);
    }
}

void PickColor(HWND hwnd) {
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16];

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = currentColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        currentColor = cc.rgbResult;
        DeleteObject(hPen);
        hPen = CreatePen(PS_SOLID, 1, currentColor);
    }
}
