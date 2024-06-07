//c언어로 공룡게임을 만들어줘
// 화면 오른쪽에서 장애물이 왼쪽으로옴
// 장애물을 피하면 점수가 올라감
// 장애물을 맞으면 게임오버
// 장애물은 점프로 피할 수 있음
// 점프는 스페이스바로 함
// 점프는 2칸이동함
// 장애물은 1칸씩 이동함
// 장애물은 1초마다 이동함
//장애물의 속도는 점점 빨라짐
//장애물의 속도는 1초마다 0.1씩 빨라짐
//스페이스바로 점프를함
//장애물은 하나씩만 이동해야됨
//장애물은 랜덤한 위치에서 시작함
//더블 버퍼링을 사용해서 화면의 깜박임을 해결해줘
//움직이는 개체는 하나만 보여야됨(하나만 움직여야됨)

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 5
#define OBSTACLE '@'
#define PLAYER 'D'
#define JUMP_HEIGHT 2
#define SPEED 100
#define MAX_SPEED 50

void gotoxy(int x, int y)
{
    COORD Pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void drawPlayer(int x, int y)
{
    gotoxy(x, y);
    printf("%c", PLAYER);
}
void clearPlayer(int x, int y)
{
    gotoxy(x, y);
    printf(" ");
}
void drawObstacle(int x, int y)
{
    gotoxy(x, y);
    printf("%c", OBSTACLE);
}

void clearObstacle(int x, int y)
{
    gotoxy(x, y);
    printf(" ");
}

int main()
{
    int playerX = 0;
    int playerY = HEIGHT - 1;
    int obstacleX = WIDTH - 1;
    int obstacleY = HEIGHT - 1;
    int score = 0;
    int speed = SPEED;
    int jump = 0;
    int jumpHeight = JUMP_HEIGHT;
    int jumpDirection = 1;
    int jumpCount = 0;
    int jumpMaxCount = jumpHeight * 2;
    int jumpSpeed = speed / 2;
    int jumpSpeedCount = 0;
    int jumpSpeedMaxCount = jumpSpeed / 2;
    int prevPlayerY = playerY;
    int gameOver = 0;

    srand(time(NULL));

    while (!gameOver)
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == ' ')
            {
                jump = 1;
            }
        }

        if (jump)
        {
            if (jumpCount < jumpMaxCount)
            {
                clearPlayer(playerX, prevPlayerY);
                playerY -= jumpDirection;
                prevPlayerY = playerY;
                jumpCount++;
            }
            
            else
            {
                jump = 0;
                jumpCount = 0;
            }
        }
        else
        {
            if (playerY < HEIGHT - 1)
            {
                clearPlayer(playerX, prevPlayerY);
                playerY++;
                prevPlayerY = playerY;
            }
        }


        if (jumpSpeedCount < jumpSpeedMaxCount)
        {
            jumpSpeedCount++;
        }
        else
        {
            jumpSpeedCount = 0;
            if (speed > MAX_SPEED)
            {
                speed -= 10;
            }
        }

        if (obstacleX == 0)
        {
            obstacleX = WIDTH - 1;
            obstacleY = rand() % HEIGHT;
            score++;
        }
        else
        {
            clearObstacle(obstacleX, obstacleY);
            obstacleX--;
        }

        if (playerX == obstacleX && playerY == obstacleY)
        {
            gameOver = 1;
        }

        drawPlayer(playerX, playerY);
        drawObstacle(obstacleX, obstacleY);
        gotoxy(WIDTH + 1, 0);
        printf("Score: %d", score);
        Sleep(speed);
    }

    gotoxy(0, HEIGHT + 1);
    printf("Game Over\n");
    printf("Score: %d\n", score);

    return 0;
}

