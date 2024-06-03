#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_NAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100

typedef struct {
    char item_name[MAX_NAME_LENGTH];
    int price;
    int stock;
} MenuItem;

MenuItem menu[MAX_ITEMS];
int menu_size = 0;
char admin_password[MAX_PASSWORD_LENGTH] = "";
int logged_in_as_admin = 0;

void set_admin_password() {
    while (1) {
        char password[MAX_PASSWORD_LENGTH], confirm_password[MAX_PASSWORD_LENGTH];
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
    while (1) {
        char item_name[MAX_NAME_LENGTH];
        int price;
        printf("추가할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ");
        scanf(" %[^\n]", item_name); // Read until newline
        if (strcmp(item_name, "메뉴 관리 완료!") == 0)
            break;
        if (menu_size >= MAX_ITEMS) {
            printf("더 이상 메뉴를 추가할 수 없습니다. 최대 개수에 도달했습니다.\n");
            break;
        }
        int already_exists = 0;
        for (int i = 0; i < menu_size; i++) {
            if (strcmp(menu[i].item_name, item_name) == 0) {
                already_exists = 1;
                printf("이미 존재하는 메뉴입니다.\n");
                break;
            }
        }
        if (already_exists)
            continue;
        printf("가격을 입력하세요: ");
        scanf("%d", &price);
        if (price > 10000) {
            printf("가격이 너무 높습니다. 10000원을 초과할 수 없습니다.\n");
        }
        else if (price < 1000) {
            printf("가격이 너무 낮습니다. 1000원 이상이어야 합니다.\n");
        }
        else {
            strcpy(menu[menu_size].item_name, item_name);
            menu[menu_size].price = price;
            menu[menu_size].stock = 10;
            menu_size++;
        }
    }
}

void delete_menu() {
    while (1) {
        char item_name[MAX_NAME_LENGTH];
        printf("삭제할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ");
        scanf(" %[^\n]", item_name); // Read until newline
        if (strcmp(item_name, "메뉴 관리 완료!") == 0)
            break;
        int found = 0;
        for (int i = 0; i < menu_size; i++) {
            if (strcmp(menu[i].item_name, item_name) == 0) {
                found = 1;
                for (int j = i; j < menu_size - 1; j++) {
                    menu[j] = menu[j + 1];
                }
                menu_size--;
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
    for (int i = 0; i < menu_size; i++) {
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
            return 0;
        default:
            printf("잘못된 입력입니다. 다시 시도하세요.\n");
        }
    }
    return 0;
}
