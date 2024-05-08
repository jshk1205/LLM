#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MONTH_NAME_LENGTH 20

void show_month_calendar(int year, int month, int day);
void show_day_calendar(int year, int month, int day);
void choose_day(int year, int month, int days_in_month);
const char* get_birth_flower(int month);
int main() {
    char choice[3];
    int year, month, day;

    while (1) {
        printf("1. 년도 선택\n");
        printf("2. 달 선택\n");
        printf("3. 종료\n");
        printf("선택하세요: ");
        fgets(choice, sizeof(choice), stdin);

        // Remove newline character from the choice
        if (choice[strlen(choice) - 1] == '\n') {
            choice[strlen(choice) - 1] = '\0';
        }

        if (strcmp(choice, "1") == 0) {
            printf("년도를 입력하세요: ");
            scanf("%d", &year);
            month = 0;
            day = 0;
            show_month_calendar(year, month, day);
        }
        else if (strcmp(choice, "2") == 0) {
            printf("년도를 입력하세요: ");
            scanf("%d", &year);
            printf("달을 입력하세요: ");
            scanf("%d", &month);
            day = 0;
            show_day_calendar(year, month, day);
        }
        else if (strcmp(choice, "3") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        else {
            printf("잘못된 선택입니다. 다시 선택해 주세요.\n");
        }
        while (getchar() != '\n'); // clear input buffer
    }

    return 0;
}

void show_month_calendar(int year, int month, int day) {
    printf("달을 선택하세요:\n");
    char month_names[12][MAX_MONTH_NAME_LENGTH] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };

    for (int i = 0; i < 12; i++) {
        printf("%d. %s\n", i + 1, month_names[i]);
    }

    printf("선택하세요: ");
    scanf("%d", &month);
    if (month < 1 || month > 12) {
        printf("잘못된 입력입니다.\n");
        return;
    }

    int days_in_month;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_month = 29;
        }
        else {
            days_in_month = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    }
    else {
        days_in_month = 31;
    }

    printf("Calendar for %d %s\n", year, month_names[month - 1]);
    choose_day(year, month, days_in_month);
}

void show_day_calendar(int year, int month, int day) {
    printf("날짜를 선택하세요:\n");
    int days_in_month;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_month = 29;
        }
        else {
            days_in_month = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    }
    else {
        days_in_month = 31;
    }

    for (int i = 1; i <= days_in_month; i++) {
        printf("%d\n", i);
    }

    printf("선택하세요: ");
    scanf("%d", &day);
    if (day < 1 || day > days_in_month) {
        printf("잘못된 입력입니다.\n");
        return;
    }

    printf("%d년 %d월 %d일\n", year, month, day);
    printf("탄생화: %s\n", get_birth_flower(month));
}

void choose_day(int year, int month, int days_in_month) {
    int day;
    printf("날짜를 선택하세요: ");
    scanf("%d", &day);
    if (day < 1 || day > days_in_month) {
        printf("잘못된 입력입니다.\n");
        return;
    }

    printf("%d년 %d월 %d일\n", year, month, day);
    printf("탄생화: %s\n", get_birth_flower(month));
}

const char* get_birth_flower(int month) {
    const char* birth_flowers[] = { "", "양귀비", "수선화", "민들레", "벚꽃", "장미", "라벤더", "수련", "해바라기", "백합", "금잔화", "카네이션", "포인세티아" };
    return birth_flowers[month];
}