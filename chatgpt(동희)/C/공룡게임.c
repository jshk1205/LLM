#include <stdio.h>
#include <conio.h> // 윈도우에서는 conio.h 라이브러리를 사용할 수 있습니다.

// 장애물과 공룡의 좌표
int dinoX = 10, dinoY = 10;
int obstacleX = 60;

// 장애물의 이동 속도
int obstacleSpeed = 1;

// 게임 상태 변수
int isGameOver = 0;

void drawDino() {
    printf("    ▄▄██████▄▄\n");
    printf("  ▄▀▀███████████\n");
    printf("  █▄████████████\n");
    printf("   ▀███████████▀\n");
    printf("      ▀▀▀▀▀▀▀▀▀▀\n");
}

void drawObstacle() {
    printf("▄");
}

void clearScreen() {
    system("cls || clear"); // 윈도우와 리눅스/맥OS에서 화면을 지우는 명령어를 선택적으로 실행합니다.
}

void updateGame() {
    if (!isGameOver) {
        obstacleX -= obstacleSpeed;
        if (obstacleX <= dinoX + 4 && obstacleX >= dinoX && dinoY == 10) {
            isGameOver = 1;
        }
    }
}

void drawGame() {
    clearScreen();
    for (int i = 0; i < 5; i++) {
        printf("\n");
    }
    for (int i = 0; i < dinoY; i++) {
        printf("\n");
    }

    // 장애물과 공룡을 그립니다.
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < dinoX; j++) {
            printf(" ");
        }
        drawDino();
    }

    for (int i = 0; i < obstacleX; i++) {
        printf(" ");
    }
    drawObstacle();

    printf("\n");

    // 게임 오버 표시
    if (isGameOver) {
        printf("\n\n\n\n\n\t\t\t\tGAME OVER\n");
    }
}

int main() {
    while (!kbhit()) {
        updateGame();
        drawGame();
        // 게임 속도 조절
        for (int i = 0; i < 10000000; i++) {
            for (int j = 0; j < 1000; j++) {
            }
        }
    }
    return 0;
}
