#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 20
#define TRUE 1
#define FALSE 0

int field[HEIGHT][WIDTH] = { 0 };
int currentBlock[4][4]; // 현재 블록
int blockX = WIDTH / 2, blockY = 0; // 블록의 위치
int nextBlock[4][4]; // 다음 블록
int score = 0;
int gameOver = FALSE;

// 블록 모양
int blocks[7][4][4] = {
    {
        {0,0,0,0},
        {1,1,1,1}, // I
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {2,0,0,0},
        {2,2,2,0}, // J
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,0,3,0},
        {3,3,3,0}, // L
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {4,4,0,0},
        {0,4,4,0}, // S
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,5,5,0},
        {5,5,0,0}, // Z
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,6,0,0},
        {6,6,6,0}, // T
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {7,7,0,0},
        {7,7,0,0}, // O
        {0,0,0,0},
        {0,0,0,0}
    }
};

void Initialize() {
    srand(time(NULL));
    NewBlock();
}

void Draw() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x] == 0)
                printf(".");
            else
                printf("%c", '0' + field[y][x]);
        }
        printf("\n");
    }

    printf("\nNext block:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (nextBlock[i][j] == 0)
                printf(" ");
            else
                printf("%c", '0' + nextBlock[i][j]);
        }
        printf("\n");
    }
    printf("\nScore: %d\n", score);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': // 왼쪽으로 이동
            if (!CheckCollision(blockX - 1, blockY))
                blockX--;
            break;
        case 'd': // 오른쪽으로 이동
            if (!CheckCollision(blockX + 1, blockY))
                blockX++;
            break;
        case 's': // 빠르게 내려감
            if (!CheckCollision(blockX, blockY + 1))
                blockY++;
            break;
        case 'w': // 회전
            RotateBlock();
            break;
        }
    }
}

void Logic() {
    if (CheckCollision(blockX, blockY + 1)) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentBlock[i][j] != 0)
                    field[blockY + i][blockX + j] = currentBlock[i][j];
            }
        }
        RemoveLine();
        NewBlock();
    }
    else {
        blockY++;
    }
}

int CheckCollision(int nx, int ny) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentBlock[i][j] != 0) {
                int newX = nx + j;
                int newY = ny + i;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || field[newY][newX] != 0)
                    return TRUE;
            }
        }
    }
    return FALSE;
}

void NewBlock() {
    int blockType = rand() % 7;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentBlock[i][j] = blocks[blockType][i][j];
        }
    }
    blockX = WIDTH / 2;
    blockY = 0;
    if (CheckCollision(blockX, blockY))
        gameOver = TRUE;
}

void RotateBlock() {
    int temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = currentBlock[3 - j][i];
        }
    }
    if (!CheckCollision(blockX, blockY)) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                currentBlock[i][j] = temp[i][j];
            }
        }
    }
}

void RemoveLine() {
    for (int y = 0; y < HEIGHT; y++) {
        int lineComplete = TRUE;
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x] == 0) {
                lineComplete = FALSE;
                break;
            }
        }
        if (lineComplete) {
            for (int yy = y; yy > 0; yy--) {
                for (int xx = 0; xx < WIDTH; xx++) {
                    field[yy][xx] = field[yy - 1][xx];
                }
            }
            for (int xx = 0; xx < WIDTH; xx++) {
                field[0][xx] = 0;
            }
            score += 100;
        }
    }
}

int main() {
    Initialize();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(500);
    }

    printf("Game Over! Your score: %d\n", score);
    return 0;
}
