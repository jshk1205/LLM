#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM_SETS 1000
#define MAX_NUMBERS 6
#define MAX_NUM 45

// �Լ�: �ζ� ��ȣ ����
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

// �Լ�: ��÷ ��ȣ Ȯ��
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
        return "1��";
    }
    else if (numMatched == 5 && winningNumbers[MAX_NUMBERS - 1] == userNumbers[MAX_NUMBERS - 1]) {
        return "2��";
    }
    else if (numMatched == 5) {
        return "3��";
    }
    else if (numMatched == 4) {
        return "4��";
    }
    else if (numMatched == 3) {
        return "5��";
    }
    else {
        return "��";
    }
}

// �Լ�: ��÷�� ���
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

// ���� �Լ�
int main() {
    srand(time(NULL));

    int numSets;
    printf("������ �ζ� ��ȣ ��Ʈ�� ���� �Է��ϼ���: ");
    scanf("%d", &numSets);

    int userNumbers[MAX_NUMBERS];
    printf("������ ��ȣ�� �����ϼ��� (6���� ����, �������� ����): ");
    for (int i = 0; i < MAX_NUMBERS; i++) {
        scanf("%d", &userNumbers[i]);
    }

    int winningNumbersList[MAX_NUM_SETS][MAX_NUMBERS];
    for (int i = 0; i < numSets; i++) {
        generateLottoNumbers(winningNumbersList[i]);
    }

    // ��ȣ �м� �� ���
    printf("���� ���� ���� ��ȣ: %d\n", 0); // ��ȣ �м� �Լ��� �����ؾ� ��

    for (int i = 0; i < numSets; i++) {
        printf("\n%d��° �ζ� ��ȣ: ", i + 1);
        for (int j = 0; j < MAX_NUMBERS; j++) {
            printf("%d ", winningNumbersList[i][j]);
        }
        printf("\n");
        char* rank = checkWinner(userNumbers, winningNumbersList[i]);
        int prize = calculatePrize(rank);
        printf("��÷ ���: %s, ��÷��: %d��\n", rank, prize);
    }

    // ��÷ Ȯ�� �м� �� ���
    // ��÷ Ȯ�� ��� �Լ��� �����ؾ� ��

    return 0;
}
