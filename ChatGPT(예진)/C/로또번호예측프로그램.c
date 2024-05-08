#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM_SETS 1000
#define MAX_NUMBERS 6
#define MAX_NUM 45

// 함수: 로또 번호 생성
void generateLottoNumbers(int* numbers) {
    int chosen[MAX_NUM] = { 0 };
    int i = 0;
    while (i < MAX_NUMBERS) {
        int num = rand() % MAX_NUM + 1;
        if (!chosen[num - 1]) {
            numbers[i] = num;
            chosen[num - 1] = 1;
            i++;
        }
    }
    for (int i = 0; i < MAX_NUMBERS - 1; i++) {
        for (int j = 0; j < MAX_NUMBERS - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
}

// 함수: 당첨 번호 확인
char* checkWinner(int* userNumbers, int* winningNumbers) {
    int matchedNumbers[MAX_NUMBERS] = { 0 };
    int numMatched = 0;

    for (int i = 0; i < MAX_NUMBERS; i++) {
        for (int j = 0; j < MAX_NUMBERS; j++) {
            if (userNumbers[i] == winningNumbers[j]) {
                matchedNumbers[numMatched++] = userNumbers[i];
                break;
            }
        }
    }

    if (numMatched == 6) {
        return "1등";
    }
    else if (numMatched == 5 && winningNumbers[MAX_NUMBERS - 1] == userNumbers[MAX_NUMBERS - 1]) {
        return "2등";
    }
    else if (numMatched == 5) {
        return "3등";
    }
    else if (numMatched == 4) {
        return "4등";
    }
    else if (numMatched == 3) {
        return "5등";
    }
    else {
        return "꽝";
    }
}

// 함수: 당첨금 계산
int calculatePrize(char* rank) {
    if (rank[0] == '1') {
        return 1000000000;
    }
    else if (rank[0] == '2') {
        return 50000000;
    }
    else if (rank[0] == '3') {
        return 1500000;
    }
    else if (rank[0] == '4') {
        return 50000;
    }
    else if (rank[0] == '5') {
        return 5000;
    }
    else {
        return 0;
    }
}

// 메인 함수
int main() {
    srand(time(NULL));

    int numSets;
    printf("생성할 로또 번호 세트의 수를 입력하세요: ");
    scanf("%d", &numSets);

    int userNumbers[MAX_NUMBERS];
    printf("구매할 번호를 선택하세요 (6개의 숫자, 공백으로 구분): ");
    for (int i = 0; i < MAX_NUMBERS; i++) {
        scanf("%d", &userNumbers[i]);
    }

    int winningNumbersList[MAX_NUM_SETS][MAX_NUMBERS];
    for (int i = 0; i < numSets; i++) {
        generateLottoNumbers(winningNumbersList[i]);
    }

    // 번호 분석 및 출력
    printf("가장 많이 나온 번호: %d\n", 0); // 번호 분석 함수를 구현해야 함

    for (int i = 0; i < numSets; i++) {
        printf("\n%d번째 로또 번호: ", i + 1);
        for (int j = 0; j < MAX_NUMBERS; j++) {
            printf("%d ", winningNumbersList[i][j]);
        }
        printf("\n");
        char* rank = checkWinner(userNumbers, winningNumbersList[i]);
        int prize = calculatePrize(rank);
        printf("당첨 결과: %s, 당첨금: %d원\n", rank, prize);
    }

    // 당첨 확률 분석 및 출력
    // 당첨 확률 계산 함수를 구현해야 함

    return 0;
}
