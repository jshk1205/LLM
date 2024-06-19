//개인 지출 관리 애플리케이션을 만들어줘
//1. 지출 내역을 입력받아서 저장할 수 있어야 해
//2. 지출 내역을 삭제할 수 있어야 해
//3. 지출 내역을 조회할 수 있어야 해
//4. 지출 내역을 수정할 수 있어야 해
//5. 총 지출 금액을 확인할 수 있어야 해
//6. 지출 내역을 파일로 저장하고 불러올 수 있어야 해
//7. 지출 내역을 날짜별로 정렬할 수 있어야 해
//8. 지출 내역을 카테고리별로 정렬할 수 있어야 해


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX 100

typedef struct {
    char date[20];
    char category[20];
    char content[100];
    int price;
} Expense;

Expense expenses[MAX];
int count = 0;

void addExpense();
void deleteExpense();
void showExpense();
void editExpense();
void showTotalPrice();
void saveExpense();
void loadExpense();
void sortExpenseByDate();
void sortExpenseByCategory();

int main() {
    int menu;
    loadExpense();
    while (1) {
        system("cls");
        printf("1. 지출 내역 추가\n");
        printf("2. 지출 내역 삭제\n");
        printf("3. 지출 내역 조회\n");
        printf("4. 지출 내역 수정\n");
        printf("5. 총 지출 금액 확인\n");
        printf("6. 지출 내역 저장\n");
        printf("7. 지출 내역 불러오기\n");
        printf("8. 지출 내역 날짜별 정렬\n");
        printf("9. 지출 내역 카테고리별 정렬\n");
        printf("0. 종료\n");
        printf("메뉴를 선택하세요: ");
        scanf("%d", &menu);
        switch (menu) {
        case 1:
            addExpense();
            break;
        case 2:
            deleteExpense();
            break;
        case 3:
            showExpense();
            break;
        case 4:
            editExpense();
            break;
        case 5:
            showTotalPrice();
            break;
        case 6:
            saveExpense();
            break;
        case 7:
            loadExpense();
            break;
        case 0:
            return 0;
        }
    }
}

void addExpense() {
    system("cls");
    printf("날짜: ");
    scanf("%s", expenses[count].date);
    printf("카테고리: ");
    scanf("%s", expenses[count].category);
    printf("내용: ");
    scanf("%s", expenses[count].content);
    printf("금액: ");
    scanf("%d", &expenses[count].price);
    count++;
}

void deleteExpense() {
    system("cls");
    int i;
    char date[20];
    printf("삭제할 지출 내역의 날짜를 입력하세요: ");
    scanf("%s", date);
    for (i = 0; i < count; i++) {
        if (strcmp(expenses[i].date, date) == 0) {
            break;
        }
    }
    if (i == count) {
        printf("일치하는 지출 내역이 없습니다.\n");
        return;
    }
    for (; i < count - 1; i++) {
        expenses[i] = expenses[i + 1];
    }
    count--;
}

void showExpense() {
    system("cls");
    for (int i = 0; i < count; i++) {
        printf("날짜: %s\n", expenses[i].date);
        printf("카테고리: %s\n", expenses[i].category);
        printf("내용: %s\n", expenses[i].content);
        printf("금액: %d\n", expenses[i].price);
        printf("\n");
    }
    printf("아무 키나 누르세요...");
    _getch();
}

void editExpense() {
    system("cls");
    int i;
    char date[20];
    printf("수정할 지출 내역의 날짜를 입력하세요: ");
    scanf("%s", date);
    for (i = 0; i < count; i++) {
        if (strcmp(expenses[i].date, date) == 0) {
            break;
        }
    }
    if (i == count) {
        printf("일치하는 지출 내역이 없습니다.\n");
        return;
    }
    printf("날짜: ");
    scanf("%s", expenses[i].date);
    printf("카테고리: ");
    scanf("%s", expenses[i].category);
    printf("내용: ");
    scanf("%s", expenses[i].content);
    printf("금액: ");
    scanf("%d", &expenses[i].price);
}

void showTotalPrice() {
    system("cls");
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += expenses[i].price;
    }
    printf("총 지출 금액: %d\n", total);
    printf("아무 키나 누르세요...");
    _getch();
}

void saveExpense() {
    FILE* file = fopen("expenses.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %s %d\n", expenses[i].date, expenses[i].category, expenses[i].content, expenses[i].price);
    }
    fclose(file);
}

void loadExpense() {
    FILE* file = fopen("expenses.txt", "r");
    if (!file) {
        return;
    }
    count = 0;
    while (fscanf(file, "%s %s %s %d", expenses[count].date, expenses[count].category, expenses[count].content, &expenses[count].price) != EOF) {
        count++;
    }
    fclose(file);
}






