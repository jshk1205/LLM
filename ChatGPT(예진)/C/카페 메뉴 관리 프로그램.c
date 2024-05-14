#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ITEMS 100

typedef struct {
    char item_name[50];
    int price;
    int stock;
} MenuItem;

MenuItem menu[MAX_ITEMS];
int menu_count = 0;

char admin_password[50] = "";
bool logged_in_as_admin = false;

void set_admin_password() {
    while (1) {
        char password[50];
        char confirm_password[50];
        printf("관리자 비밀번호를 설정하세요: ");
        scanf("%s", password);
        printf("비밀번호를 다시 한 번 입력하세요: ");
        scanf("%s", confirm_password);
        if (strcmp(password, confirm_password) == 0) {
            strcpy(admin_password, password);
            printf("관리자 비밀번호가 설정되었습니다.\n");
            break;
        }
        else {
            printf("비밀번호가 일치하지 않습니다. 다시 시도하세요.\n");
        }
    }
}

void add_menu() {
    char item_name[50];
    int price;
    while (1) {
        printf("추가할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ");
        //메뉴 관리 완료!를 입력해도 원래 창으로 돌아가지 않고 계속 메뉴를 치도록 합니다. (GPT가 같은 오류를 반복해냄...)
        scanf("%s", item_name);
        if (strcmp(item_name, "메뉴 관리 완료!") == 0) {
            break;
        }
        if (menu_count >= MAX_ITEMS) {
            printf("더 이상 메뉴를 추가할 수 없습니다. 최대 개수에 도달했습니다.\n");
            break;
        }
        bool already_exists = false;
        for (int i = 0; i < menu_count; ++i) {
            if (strcmp(menu[i].item_name, item_name) == 0) {
                already_exists = true;
                printf("이미 존재하는 메뉴입니다.\n");
                break;
            }
        }
        if (already_exists) {
            continue;
        }
        printf("가격을 입력하세요: ");
        scanf("%d", &price);
        if (price > 10000) {
            printf("가격이 너무 높습니다. 10000원을 초과할 수 없습니다.\n");
        }
        else if (price < 1000) {
            printf("가격이 너무 낮습니다. 1000원 이상이어야 합니다.\n");
        }
        else {
            strcpy(menu[menu_count].item_name, item_name);
            menu[menu_count].price = price;
            menu[menu_count].stock = 10;
            menu_count++;
        }
    }
}

void delete_menu() {
    char item_name[50];
    while (1) {
        printf("삭제할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ");
        scanf("%s", item_name);
        if (strcmp(item_name, "메뉴 관리 완료!") == 0) {
            break;
        }
        bool found = false;
        for (int i = 0; i < menu_count; ++i) {
            if (strcmp(menu[i].item_name, item_name) == 0) {
                found = true;
                for (int j = i; j < menu_count - 1; ++j) {
                    menu[j] = menu[j + 1];
                }
                menu_count--;
                printf("메뉴가 삭제되었습니다.\n");
                break;
            }
        }
        if (!found) {
            printf("존재하지 않는 메뉴입니다.\n");
        }
    }
}

void show_menu() {
    printf("\n메뉴 목록:\n");
    for (int i = 0; i < menu_count; ++i) {
        printf("%s: %d원 - 재고: %d개\n", menu[i].item_name, menu[i].price, menu[i].stock);
    }
}

int main() {
    set_admin_password();
    int choice;
    while (1) {
        printf("\n1. 메뉴 추가\n2. 메뉴 삭제\n3. 메뉴 조회\n4. 종료\n");
        printf("원하는 작업을 선택하세요: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            add_menu();
            break;
        case 2:
            delete_menu();
            break;
        case 3:
            show_menu();
            break;
        case 4:
            printf("프로그램을 종료합니다.\n");
            return 0; // main 함수 종료
        default:
            printf("잘못된 입력입니다. 다시 시도하세요.\n");
        }
    }
}
