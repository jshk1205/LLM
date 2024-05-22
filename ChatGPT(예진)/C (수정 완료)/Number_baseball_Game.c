#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int* generateRandomNumber() {
    static int numbers[9];
    for (int i = 0; i < 9; ++i) {
        numbers[i] = i + 1;
    }
    srand(time(NULL));
    for (int i = 8; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }
    static int random_number[5];
    for (int i = 0; i < 5; ++i) {
        random_number[i] = numbers[i];
    }
    return random_number;
}

int* checkGuess(const int* random_number, const int* user_guess, int* result) {
    int strikes = 0;
    int balls = 0;
    for (int i = 0; i < 5; ++i) {
        if (user_guess[i] == random_number[i]) {
            strikes++;
        }
        else {
            for (int j = 0; j < 5; ++j) {
                if (user_guess[i] == random_number[j]) {
                    balls++;
                    break;
                }
            }
        }
    }
    result[0] = strikes;
    result[1] = balls;
    return result;
}

int main() {
    printf("숫자 야구 게임을 시작합니다!\n");
    printf("게임 규칙: 1~9까지의 서로 다른 5자리 숫자를 맞추세요.\n");

    int* random_number = generateRandomNumber();
    int attempts = 0;
    int game_record[100][3];
    int result[2];

    while (true) {
        printf("5자리 숫자를 입력하세요 (종료하려면 'q' 입력): ");
        char user_input[10];
        if (scanf_s("%s", user_input, sizeof(user_input)) == EOF) {
            break;
        }

        if (user_input[0] == 'q') {
            break;
        }

        bool valid_input = true;
        for (int i = 0; i < 5; ++i) {
            if (user_input[i] < '1' || user_input[i] > '9') {
                printf("잘못된 입력입니다. 1에서 9 사이의 숫자를 입력하세요.\n");
                valid_input = false;
                break;
            }
        }
        if (!valid_input) {
            continue;
        }

        int user_guess[5];
        for (int i = 0; i < 5; ++i) {
            user_guess[i] = user_input[i] - '0';
        }

        attempts++;
        int* result_ptr = checkGuess(random_number, user_guess, result);
        int strikes = result_ptr[0];
        int balls = result_ptr[1];

        if (strikes == 5) {
            printf("축하합니다! %d번째 시도만에 정답을 맞추셨습니다!\n", attempts);
            break;
        }

        printf("%d Strike, %d Ball\n", strikes, balls);
        game_record[attempts - 1][0] = user_guess[0] * 10000 + user_guess[1] * 1000 + user_guess[2] * 100 + user_guess[3] * 10 + user_guess[4];
        game_record[attempts - 1][1] = strikes;
        game_record[attempts - 1][2] = balls;
    }

    printf("게임 기록:\n");
    printf("Guess\t\tStrikes\tBalls\n");
    for (int i = 0; i < attempts-1; ++i) {
        printf("%d\t\t%d\t\t%d\n", game_record[i][0], game_record[i][1], game_record[i][2]);
    }

    return 0;
}
