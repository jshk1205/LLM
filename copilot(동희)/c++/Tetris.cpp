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



#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <windows.h>

using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int BLOCK_WIDTH = 4;

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
        {1, 1, 0, 0},
        {0, 1, 1, 0},
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
        {1, 1, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
    },
};

class Tetris
{
public:
    Tetris()
    {
        this->board = vector<vector<int>>(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));
        this->currentBlock = 0;
        this->currentBlockX = 0;
        this->currentBlockY = 0;
        this->nextBlock = 0;
        this->score = 0;
    }
    ~Tetris()
    {
    }

    void start()
    {
        this->currentBlock = rand() % 7;
        this->nextBlock = rand() % 7;
        this->currentBlockX = 3;
        this->currentBlockY = 0;
        while (true)
        {
            this->draw();
            this->input();
            if (!this->move(0, 1))
            {
                this->fix();
                this->clearLine();
                this->currentBlock = this->nextBlock;
                this->nextBlock = rand() % 7;
                this->currentBlockX = 3;
                this->currentBlockY = 0;
                if (!this->move(0, 1))
                {
                    break;
                }
                
            }
            Sleep(100);
        }
        
    }

    void draw()
    {
        system("cls");
        cout << "Score: " << this->score << endl;
        cout << "Next Block: " << endl;
        for (int y = 0; y < BLOCK_WIDTH; y++)
        {
            for (int x = 0; x < BLOCK_WIDTH; x++)
            {
                if (BLOCKS[this->nextBlock][y][x] == 1)
                {
                    cout << "■";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
        cout << endl;
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                if (y >= this->currentBlockY && y < this->currentBlockY + BLOCK_WIDTH && x >= this->currentBlockX && x < this->currentBlockX + BLOCK_WIDTH)
                {
                    if (BLOCKS[this->currentBlock][y - this->currentBlockY][x - this->currentBlockX] == 1)
                    {
                        cout << "■";
                        continue;
                    }
                }
                if (this->board[y][x] == 1)
                {
                    cout << "■";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
    }

    void input()
    {
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            this->move(-1, 0);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            this->move(1, 0);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            this->move(0, 1);
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            while (this->move(0, 1))
            {
            }
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            this->rotate();
        }
    }

    bool move(int x, int y)
    {
        if (this->check(this->currentBlock, this->currentBlockX + x, this->currentBlockY + y))
        {
            this->currentBlockX += x;
            this->currentBlockY += y;
            return true;
        }
        return false;
    }

    void rotate()
    {
        int tempBlock = this->currentBlock;
        for (int i = 0; i < 3; i++)
        {
            tempBlock = (tempBlock + 1) % 4;
            if (this->check(tempBlock, this->currentBlockX, this->currentBlockY))
            {
                this->currentBlock = tempBlock;
                return;
            }
        }
    }

    void fix()
    {
        for (int y = 0; y < BLOCK_WIDTH; y++)
        {
            for (int x = 0; x < BLOCK_WIDTH; x++)
            {
                if (BLOCKS[this->currentBlock][y][x] == 1)
                {
                    this->board[this->currentBlockY + y][this->currentBlockX + x] = 1;
                }
            }
        }
    }

    void clearLine()
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            bool isFull = true;
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                if (this->board[y][x] == 0)
                {
                    isFull = false;
                    break;
                }
            }
            if (isFull)
            {
                this->score += 100;
                for (int i = y; i > 0; i--)
                {
                    for (int x = 0; x < BOARD_WIDTH; x++)
                    {
                        this->board[i][x] = this->board[i - 1][x];
                    }
                }
            }
        }
    }

    bool check(int block, int x, int y)
    {
        for (int i = 0; i < BLOCK_WIDTH; i++)
        {
            for (int j = 0; j < BLOCK_WIDTH; j++)
            {
                if (BLOCKS[block][i][j] == 1)
                {
                    if (y + i >= BOARD_HEIGHT || x + j < 0 || x + j >= BOARD_WIDTH || this->board[y + i][x + j] == 1)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

private:
    vector<vector<int>> board;
    int currentBlock;
    int currentBlockX;
    int currentBlockY;
    int nextBlock;
    int score;
};

int main()
{
    Tetris tetris;
    tetris.start();
    return 0;
}
// Path: copilot%28%EB%8F%99%ED%9D%AC%29/c%2B%2B/%ED%85%8C%ED%8A%B8%EB%A6%AC%EC%8A%A4.cpp


