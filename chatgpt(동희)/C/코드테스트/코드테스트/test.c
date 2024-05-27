#include <windows.h>
#include <gdiplusheaders.h>
#include <commdlg.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

// Global variables
HWND hMainWindow, hImageViewer;
Gdiplus::Bitmap* pBitmap = NULL;
Gdiplus::Graphics* pGraphics = NULL;
Gdiplus::Rect rcImage;
Gdiplus::REAL scaleFactor = 1.0f;
Gdiplus::REAL rotationAngle = 0.0f;

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL LoadImageFile(HWND hwnd, TCHAR* szFileName, DWORD dwSize);
void DrawImage();
void ZoomIn();
void ZoomOut();
void RotateClockwise();
void RotateCounterClockwise();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Register window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = TEXT("PhotoGallery");

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create main window
    hMainWindow = CreateWindow(TEXT("PhotoGallery"), TEXT("Photo Gallery"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (hMainWindow == NULL) {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create image viewer
    hImageViewer = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP,
        0, 0, 800, 600, hMainWindow, NULL, hInstance, NULL);

    // Show window
    ShowWindow(hMainWindow, nCmdShow);
    UpdateWindow(hMainWindow);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Shutdown GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_FILE_OPEN:
        {
            TCHAR szFileName[MAX_PATH] = TEXT("");
            if (LoadImageFile(hWnd, szFileName, MAX_PATH)) {
                if (pBitmap != NULL)
                    delete pBitmap;
                pBitmap = Gdiplus::Bitmap::FromFile(szFileName);
                if (pBitmap->GetLastStatus() == Gdiplus::Ok) {
                    DrawImage();
                }
                else {
                    MessageBox(hWnd, TEXT("Failed to load image!"), TEXT("Error"), MB_OK | MB_ICONERROR);
                }
            }
        }
        break;
        case ID_EDIT_ZOOMIN:
            ZoomIn();
            break;
        case ID_EDIT_ZOOMOUT:
            ZoomOut();
            break;
        case ID_EDIT_ROTATECW:
            RotateClockwise();
            break;
        case ID_EDIT_ROTATECCW:
            RotateCounterClockwise();
            break;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (pBitmap != NULL) {
            DrawImage();
        }
        EndPaint(hWnd, &ps);
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

BOOL LoadImageFile(HWND hwnd, TCHAR* szFileName, DWORD dwSize) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = TEXT("Image Files\0*.bmp;*.jpg;*.png;*.gif\0All Files\0*.*\0");
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = dwSize;
    ofn.lpstrTitle = TEXT("Select an image file");
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    return GetOpenFileName(&ofn);
}

void DrawImage() {
    if (pGraphics == NULL) {
        pGraphics = new Gdiplus::Graphics(hImageViewer);
    }

    if (pBitmap != NULL) {
        pGraphics->Clear(Gdiplus::Color(255, 255, 255));
        Gdiplus::RectF rectDst(0, 0, (Gdiplus::REAL)pBitmap->GetWidth() * scaleFactor, (Gdiplus::REAL)pBitmap->GetHeight() * scaleFactor);
        Gdiplus::PointF center((Gdiplus::REAL)hImageViewer->clientRect.right / 2, (Gdiplus::REAL)hImageViewer->clientRect.bottom / 2);
        Gdiplus::PointF offset(-rectDst.Width / 2, -rectDst.Height / 2);

        pGraphics->TranslateTransform(center.X, center.Y);
        pGraphics->RotateTransform(rotationAngle);
        pGraphics->TranslateTransform(offset.X, offset.Y);
        pGraphics->DrawImage(pBitmap, rectDst);
    }
}

void ZoomIn() {
    if (pBitmap != NULL) {
        scaleFactor += 0.1f;
        DrawImage();
    }
}

void ZoomOut() {
    if (pBitmap != NULL) {
        if (scaleFactor > 0.2f) {
            scaleFactor -= 0.1f;
            DrawImage();
        }
    }
}

void RotateClockwise() {
    if (pBitmap != NULL) {
        rotationAngle += 90.0f;
        DrawImage();
    }
}

void RotateCounterClockwise() {
    if (pBitmap != NULL) {
        rotationAngle -= 90.0f;
        DrawImage();
    }
}
