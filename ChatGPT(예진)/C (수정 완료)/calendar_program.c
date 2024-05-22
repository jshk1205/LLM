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
        printf("1. Select Year\n");
        printf("2. Select Month\n");
        printf("3. Exit\n");
        printf("Make a choice: ");
        fgets(choice, sizeof(choice), stdin);

        // Remove newline character from the choice
        if (choice[strlen(choice) - 1] == '\n') {
            choice[strlen(choice) - 1] = '\0';
        }

        if (strcmp(choice, "1") == 0) {
            printf("Enter year: ");
            scanf("%d", &year);
            month = 0;
            day = 0;
            show_month_calendar(year, month, day);
        }
        else if (strcmp(choice, "2") == 0) {
            printf("Enter year: ");
            scanf("%d", &year);
            printf("Enter month: ");
            scanf("%d", &month);
            day = 0;
            show_day_calendar(year, month, day);
        }
        else if (strcmp(choice, "3") == 0) {
            printf("Exiting program.\n");
            break;
        }
        else {
            printf("Invalid choice. Please select again.\n");
        }
        while (getchar() != '\n'); // clear input buffer
    }

    return 0;
}

void show_month_calendar(int year, int month, int day) {
    printf("Select a month:\n");
    char month_names[12][MAX_MONTH_NAME_LENGTH] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };

    for (int i = 0; i < 12; i++) {
        printf("%d. %s\n", i + 1, month_names[i]);
    }

    printf("Make a choice: ");
    scanf("%d", &month);
    if (month < 1 || month > 12) {
        printf("Invalid input.\n");
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
    printf("Select a day:\n");
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

    printf("Make a choice: ");
    scanf("%d", &day);
    if (day < 1 || day > days_in_month) {
        printf("Invalid input.\n");
        return;
    }

    printf("%d-%d-%d\n", year, month, day);
    printf("Birth flower: %s\n", get_birth_flower(month));
}

void choose_day(int year, int month, int days_in_month) {
    int day;
    printf("Select a day: ");
    scanf("%d", &day);
    if (day < 1 || day > days_in_month) {
        printf("Invalid input.\n");
        return;
    }

    printf("%d-%d-%d\n", year, month, day);
    printf("Birth flower: %s\n", get_birth_flower(month));
}

const char* get_birth_flower(int month) {
    const char* birth_flowers[] = { "", "Carnation", "Violet", "Daffodil", "Daisy", "Lily of the Valley", "Rose", "Larkspur", "Gladiolus", "Aster", "Marigold", "Chrysanthemum", "Poinsettia" };
    return birth_flowers[month];
}
