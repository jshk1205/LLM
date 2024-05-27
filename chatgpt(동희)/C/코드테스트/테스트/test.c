#include <windows.h>

#define BALL_SIZE 20
#define PADDLE_WIDTH 90
#define PADDLE_HEIGHT 20
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define BRICK_ROWS 3
#define BRICK_COLS 7
#define TIMER_ID 1

// Global variables
HWND hwnd;
HINSTANCE hInstance;
int playerX = 310;
int ballPosX = 120;
int ballPosY = 350;
int ballXDir = -1;
int ballYDir = -2;
int score = 0;
int bricksCount = 21;
int stage = 1;
int map[BRICK_ROWS][BRICK_COLS];
int WINDOW_WIDTH = 700;
int WINDOW_HEIGHT = 800; // 초기 창 크기

// Function prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitializeMap();
void DrawBricks(HDC hdc);
void MoveBall(HDC hdc);
void CheckCollision();
void StartNextStage(HDC hdc);
void MovePaddleWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); // MovePaddleWndProc 함수 선언

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("BrickBreakerGame");

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Brick Breaker Game"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message) {
    case WM_CREATE:
        SetTimer(hwnd, TIMER_ID, 8, NULL);
        InitializeMap();
        return 0;

    case WM_TIMER:
        hdc = GetDC(hwnd);
        MoveBall(hdc);
        CheckCollision();
        ReleaseDC(hwnd, hdc);
        return 0;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            if (playerX >= 10)
                playerX -= 20;
            break;
        case VK_RIGHT:
            if (playerX <= WINDOW_WIDTH - PADDLE_WIDTH - 10)
                playerX += 20;
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawBricks(hdc);
        Rectangle(hdc, 0, 0, 3, WINDOW_HEIGHT); // 좌측 경계
        Rectangle(hdc, 0, 0, WINDOW_WIDTH, 3); // 상단 경계
        Rectangle(hdc, WINDOW_WIDTH - 3, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // 우측 경계
        Rectangle(hdc, 0, WINDOW_HEIGHT - 3, WINDOW_WIDTH, WINDOW_HEIGHT); // 하단 경계
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, WINDOW_WIDTH - 150, 10, TEXT("점수: "), 7);
        TCHAR scoreText[5];
        wsprintf(scoreText, TEXT("%d"), score);
        TextOut(hdc, WINDOW_WIDTH - 60, 10, scoreText, lstrlen(scoreText));
        Rectangle(hdc, playerX, WINDOW_HEIGHT - PADDLE_HEIGHT - 10, playerX + PADDLE_WIDTH, WINDOW_HEIGHT - 10); // 패들
        Ellipse(hdc, ballPosX, ballPosY, ballPosX + BALL_SIZE, ballPosY + BALL_SIZE); // 공
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        return 0;

    case WM_SIZE: // 창 크기 조정 이벤트
        WINDOW_WIDTH = LOWORD(lParam);
        WINDOW_HEIGHT = HIWORD(lParam);

        // 패들 위치 업데이트
        if (playerX > WINDOW_WIDTH - PADDLE_WIDTH)
            playerX = WINDOW_WIDTH - PADDLE_WIDTH;
        if (ballPosX > WINDOW_WIDTH - BALL_SIZE)
            ballPosX = WINDOW_WIDTH - BALL_SIZE;
        if (ballPosY > WINDOW_HEIGHT - BALL_SIZE)
            ballPosY = WINDOW_HEIGHT - BALL_SIZE;

        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
void InitializeMap() {
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            map[i][j] = 1;
        }
    }
}

void DrawBricks(HDC hdc) {
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            if (map[i][j] == 1) {
                int brickX = j * BRICK_WIDTH + 80;
                int brickY = i * BRICK_HEIGHT + 50;
                Rectangle(hdc, brickX, brickY, brickX + BRICK_WIDTH, brickY + BRICK_HEIGHT);
            }
        }
    }
}

void MoveBall(HDC hdc) {
    ballPosX += ballXDir;
    ballPosY += ballYDir;

    if (ballPosX <= 0 || ballPosX >= WINDOW_WIDTH - BALL_SIZE)
        ballXDir = -ballXDir;
    if (ballPosY <= 0)
        ballYDir = -ballYDir;
    if (ballPosY >= WINDOW_HEIGHT - BALL_SIZE) {
        MessageBox(hwnd, TEXT("You Lost!"), TEXT("Game Over"), MB_OK);
        PostQuitMessage(0);
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

void CheckCollision() {
    // 공과 패들의 충돌 검사
    if (ballPosX + BALL_SIZE >= playerX && ballPosX <= playerX + PADDLE_WIDTH && ballPosY + BALL_SIZE >= WINDOW_HEIGHT - PADDLE_HEIGHT - 10) {
        ballYDir = -ballYDir;
    }

    // 공과 벽돌의 충돌 검사
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            if (map[i][j] == 1) {
                int brickX = j * BRICK_WIDTH + 80;
                int brickY = i * BRICK_HEIGHT + 50;
                int ballCenterX = ballPosX + BALL_SIZE / 2;
                int ballCenterY = ballPosY + BALL_SIZE / 2;

                if (ballCenterX >= brickX && ballCenterX <= brickX + BRICK_WIDTH && ballCenterY >= brickY && ballCenterY <= brickY + BRICK_HEIGHT) {
                    ballYDir = -ballYDir;
                    map[i][j] = 0;
                    score += 5;
                    bricksCount--;
                }
            }
        }
    }

    // 공이 바닥에 닿았을 때 게임 종료
    if (ballPosY >= WINDOW_HEIGHT - BALL_SIZE) {
        MessageBox(hwnd, TEXT("You Lost!"), TEXT("Game Over"), MB_OK);
        PostQuitMessage(0);
    }

    // 모든 벽돌을 깼을 때 게임 승리
    if (bricksCount == 0) {
        MessageBox(hwnd, TEXT("You Won!"), TEXT("Congratulations"), MB_OK);
        PostQuitMessage(0);
    }
}


void MovePaddleWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            if (playerX >= 10)
                playerX -= 20;
            break;
        case VK_RIGHT:
            if (playerX <= WINDOW_WIDTH - PADDLE_WIDTH - 10)
                playerX += 20;
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    default:
        break;
    }
}
