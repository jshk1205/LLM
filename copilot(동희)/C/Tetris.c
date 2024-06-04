//테트리스를 만들려고해
//테트리스는 4개의 블록으로 이루어져있어
//블록은 4개의 정사각형으로 이루어져있어
//블록은 7가지가 있어
//블록은 회전할 수 있어
//블록은 아래로 이동할 수 있어
//블록은 좌우로 이동할 수 있어
//블록은 아래로 이동할 수 없을 때 더이상 움직일 수 없어
//블록은 아래로 이동할 수 없을 때 블록을 고정시켜야해
//블록은 고정되면 새로운 블록이 나와
//블록은 고정되면 가로줄이 꽉 차있으면 가로줄이 사라져
//블록은 고정되면 가로줄이 꽉 차있으면 가로줄 위에 있는 블록들이 아래로 내려와
//블록은 고정되면 가로줄이 꽉 차있으면 가로줄 위에 있는 블록들이 아래로 내려올 때 점수를 얻어
//BLOCKS 배열이 수정 가능해야되어야해
//테트리스는 10x20 크기의 보드에서 플레이해
//블럭은 하나만 나와야하며 다음 블럭은 미리 보여야해
//블럭은 랜덤으로 나와야해
//블럭은 4x4 크기의 배열로 표현해야해
//블럭은 1과 0으로 이루어져있어
//블럭은 1은 블럭이고 0은 빈칸이야
//블럭은 보드에 그려질 때는 1이 있는 부분만 그려져야해
//블럭은 보드에 그려질 때는 블럭의 왼쪽 상단이 기준이야
//블럭은 보드의 가장 위에서 시작해야해
//블럭은 보드의 가장 위에서 시작할 때 블럭이 보드 밖으로 나가면 안돼
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 안돼
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 블럭이 고정되어야해
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 새로운 블럭이 나와야해
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 가로줄이 꽉 차있으면 가로줄이 사라져야해
//블럭이 지나가면서 블럭이 채워지면 안돼
//c언어로 테트리스를 만들어보자

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_WIDTH 4

int BLOCKS[7][4][4] = {
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
    },
};

int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

int currentBlock[4][4];

int currentBlockX = 0;
int currentBlockY = 0;
int currentBlockType = 0;

int nextBlock[4][4];

int score = 0;

void init()
{
    currentBlockType = rand() % 7;
    memcpy(currentBlock, BLOCKS[currentBlockType], sizeof(currentBlock));

    currentBlockX = BOARD_WIDTH / 2 - BLOCK_WIDTH / 2;
    currentBlockY = 0;

    memcpy(nextBlock, BLOCKS[rand() % 7], sizeof(nextBlock));
}

void drawBlock(int x, int y, int block[4][4])
{
    for (int i = 0; i < BLOCK_WIDTH; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (block[i][j] == 1)
            {
                board[y + i][x + j] = 1;
            }
        }
    }
}

void clearBlock(int x, int y, int block[4][4])
{
    for (int i = 0; i < BLOCK_WIDTH; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (block[i][j] == 1)
            {
                board[y + i][x + j] = 0;
            }
        }
    }
}

void drawBoard()
{
    system("cls");

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (board[i][j] == 1)
            {
                printf("■");
            }
            else
            {
                printf("□");
            }
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
}

int checkCollision(int x, int y, int block[4][4])
{
    for (int i = 0; i < BLOCK_WIDTH; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (block[i][j] == 1)
            {
                if (x + j < 0 || x + j >= BOARD_WIDTH || y + i >= BOARD_HEIGHT || (y + i >= 0 && board[y + i][x + j] == 1))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void fixBlock()
{
    for (int i = 0; i < BLOCK_WIDTH; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (currentBlock[i][j] == 1)
            {
                board[currentBlockY + i][currentBlockX + j] = 1;
            }
        }
    }
}

void clearLine()
{
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        int isFull = 1;
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (board[i][j] == 0)
            {
                isFull = 0;
                break;
            }
        }

        if (isFull)
        {
            score += 100;
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < BOARD_WIDTH; j++)
                {
                    board[k][j] = board[k - 1][j];
                }
            }
        }
    }
}

void rotateBlock()
{
    int temp[4][4];
    memcpy(temp, currentBlock, sizeof(temp));

    for (int i = 0; i < BLOCK_WIDTH; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            currentBlock[i][j] = temp[BLOCK_WIDTH - j - 1][i];
        }
    }

    if (checkCollision(currentBlockX, currentBlockY, currentBlock))
    {
        // 충돌이 발생하면 블록을 원래대로 돌립니다.
        memcpy(currentBlock, temp, sizeof(temp));
    }
    else
    {
        // 충돌이 발생하지 않으면 블록의 위치를 조정합니다.
        while (currentBlockX < 0 || currentBlockX + BLOCK_WIDTH > BOARD_WIDTH)
        {
            if (currentBlockX < 0)
            {
                currentBlockX++;
            }
            else if (currentBlockX + BLOCK_WIDTH > BOARD_WIDTH)
            {
                currentBlockX--;
            }
        }

        while (currentBlockY + BLOCK_WIDTH > BOARD_HEIGHT)
        {
            currentBlockY--;
        }
    }
}

void moveBlock(int dx, int dy)
{
    clearBlock(currentBlockX, currentBlockY, currentBlock);

    if (!checkCollision(currentBlockX + dx, currentBlockY + dy, currentBlock))
    {
        currentBlockX += dx;
        currentBlockY += dy;
    }

    drawBlock(currentBlockX, currentBlockY, currentBlock);
}

void input()
{
    if (_kbhit())
    {
        int key = _getch();

        switch (key)
        {
        case 'a':
            moveBlock(-1, 0);
            break;
        case 'd':
            moveBlock(1, 0);
            break;
        case 's':
            moveBlock(0, 1);
            break;
        case 'w':
            rotateBlock();
            break;
        }
    }
}

void update()
{
    clearBlock(currentBlockX, currentBlockY, currentBlock);

    if (checkCollision(currentBlockX, currentBlockY + 1, currentBlock))
    {
        fixBlock();
        clearLine();
        init();
    }
    else
    {
        currentBlockY++;
    }

    drawBlock(currentBlockX, currentBlockY, currentBlock);
}

int main()
{
    srand(time(NULL));

    init();

    while (1)
    {
        drawBoard();
        input();
        update();
        Sleep(100);
    }

    return 0;
}

// Path: copilot%28%EB%8F%99%ED%9D%AC%29/C/Tetris.c


