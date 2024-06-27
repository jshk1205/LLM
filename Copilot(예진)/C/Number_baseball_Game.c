// c언어로 숫자야구 게임을 개발해 주세요.
// 숫자야구 게임은 1부터 9까지의 서로 다른 숫자로 이루어진 3자리의 수를 맞추는 게임입니다.
// 게임을 시작하면 컴퓨터는 1부터 9까지의 서로 다른 숫자로 이루어진 3자리의 수를 생성합니다.
// 사용자는 컴퓨터가 생성한 숫자를 맞추기 위해 3자리의 수를 입력합니다.
// 컴퓨터는 사용자가 입력한 숫자에 대해 다음과 같은 힌트를 제공합니다.
// 1. 사용자가 입력한 숫자와 컴퓨터가 생성한 숫자의 자리와 숫자가 일치하면 스트라이크입니다.
// 2. 사용자가 입력한 숫자와 컴퓨터가 생성한 숫자의 숫자가 일치하지만 자리가 일치하지 않으면 볼입니다.
// 3. 사용자가 입력한 숫자와 컴퓨터가 생성한 숫자의 숫자가 일치하지 않으면 아웃입니다.
// 사용자가 입력한 숫자에 대한 힌트를 제공한 후 사용자는 다시 3자리의 수를 입력합니다.
// 사용자가 3자리의 수를 맞출 때까지 이 과정을 반복합니다.
// 사용자가 3자리의 수를 맞추면 게임은 종료됩니다.
// 사용자가 3자리의 수를 맞추지 못하고 게임을 종료하면 사용자에게 다시 게임을 할 것인지 물어봅니다.
// 사용자가 게임을 계속할 것인지에 대한 답변이 'Y' 또는 'y'이면 게임을 다시 시작합니다.
// 사용자가 게임을 계속할 것인지에 대한 답변이 'N' 또는 'n'이면 게임을 종료합니다.
// 사용자가 게임을 계속할 것인지에 대한 답변이 'Y' 또는 'y' 또는 'N' 또는 'n'이 아니면 사용자에게 다시 입력할 것을 요청합니다.
// 게임을 종료할 때 사용자가 입력한 숫자와 컴퓨터가 생성한 숫자를 출력합니다.
// 게임을 종료할 때 사용자가 몇 번만에 3자리의 수를 맞췄는지 출력합니다.
// 게임을 종료할 때 사용자에게 다시 게임을 할 것인지 물어봅니다.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int computer[3];
    int user[3];
    int strike = 0;
    int ball = 0;
    int out = 0;
    int count = 0;
    char answer;

    srand(time(NULL));

    while (1) {
        computer[0] = rand() % 9 + 1;
        computer[1] = rand() % 9 + 1;
        computer[2] = rand() % 9 + 1;

        if (computer[0] != computer[1] && computer[0] != computer[2] && computer[1] != computer[2]) {
            break;
        }
    }

    while (1) {
        strike = 0;
        ball = 0;
        out = 0;

        printf("3자리의 수를 입력하세요: \n");
        scanf("%d %d %d", &user[0], &user[1], &user[2]);

        if (user[0] == user[1] || user[0] == user[2] || user[1] == user[2]) {
            printf("서로 다른 숫자로 이루어진 3자리의 수를 입력하세요.\n");
            continue;
        }

        count++;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (user[i] == computer[j]) {
                    if (i == j) {
                        strike++;
                    }
                    else {
                        ball++;
                    }
                }
            }
        }

        out = 3 - strike - ball;

        if (strike == 3) {
            printf("축하합니다! %d번만에 3자리의 수를 맞췄습니다.\n", count);
            break;
        }
        else {
            printf("%dS %dB %dO\n", strike, ball, out);
        }
    }

    printf("컴퓨터가 생성한 숫자: %d %d %d\n", computer[0], computer[1], computer[2]);

    while (1) {
        printf("게임을 다시 하시겠습니까? (Y/N): ");
        scanf(" %c", &answer);

        if (
            answer == 'Y' || answer == 'y' ||
            answer == 'N' || answer == 'n'
            ) {
            break;
        }
        else {
            printf("Y 또는 N을 입력하세요.\n");
        }
    }

    if (answer == 'Y' || answer == 'y') {
        main();
    }

    return 0;
}

