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
        printf("������ ��й�ȣ�� �����ϼ���: ");
        scanf("%s", password);
        printf("��й�ȣ�� �ٽ� �� �� �Է��ϼ���: ");
        scanf("%s", confirm_password);
        if (strcmp(password, confirm_password) == 0) {
            strcpy(admin_password, password);
            printf("������ ��й�ȣ�� �����Ǿ����ϴ�.\n");
            break;
        }
        else {
            printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�. �ٽ� �õ��ϼ���.\n");
        }
    }
}

void add_menu() {
    while (1) {
        char item_name[MAX_NAME_LENGTH];
        int price;
        printf("�߰��� �޴��� �Է��ϼ��� (�����Ϸ��� '�޴� ���� �Ϸ�!' �Է�): ");
        scanf(" %[^\n]", item_name); // Read until newline
        if (strcmp(item_name, "�޴� ���� �Ϸ�!") == 0)
            break;
        if (menu_size >= MAX_ITEMS) {
            printf("�� �̻� �޴��� �߰��� �� �����ϴ�. �ִ� ������ �����߽��ϴ�.\n");
            break;
        }
        int already_exists = 0;
        for (int i = 0; i < menu_size; i++) {
            if (strcmp(menu[i].item_name, item_name) == 0) {
                already_exists = 1;
                printf("�̹� �����ϴ� �޴��Դϴ�.\n");
                break;
            }
        }
        if (already_exists)
            continue;
        printf("������ �Է��ϼ���: ");
        scanf("%d", &price);
        if (price > 10000) {
            printf("������ �ʹ� �����ϴ�. 10000���� �ʰ��� �� �����ϴ�.\n");
        }
        else if (price < 1000) {
            printf("������ �ʹ� �����ϴ�. 1000�� �̻��̾�� �մϴ�.\n");
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
        printf("������ �޴��� �Է��ϼ��� (�����Ϸ��� '�޴� ���� �Ϸ�!' �Է�): ");
        scanf(" %[^\n]", item_name); // Read until newline
        if (strcmp(item_name, "�޴� ���� �Ϸ�!") == 0)
            break;
        int found = 0;
        for (int i = 0; i < menu_size; i++) {
            if (strcmp(menu[i].item_name, item_name) == 0) {
                found = 1;
                for (int j = i; j < menu_size - 1; j++) {
                    menu[j] = menu[j + 1];
                }
                menu_size--;
                printf("�޴��� �����Ǿ����ϴ�.\n");
                break;
            }
        }
        if (!found) {
            printf("�������� �ʴ� �޴��Դϴ�.\n");
        }
    }
}

void show_menu() {
    printf("\n�޴� ���:\n");
    for (int i = 0; i < menu_size; i++) {
        printf("%s: %d�� - ���: %d��\n", menu[i].item_name, menu[i].price, menu[i].stock);
    }
}

int main() {
    set_admin_password();
    int choice;
    while (1) {
        printf("\n1. �޴� �߰�\n2. �޴� ����\n3. �޴� ��ȸ\n4. ����\n");
        printf("���ϴ� �۾��� �����ϼ���: ");
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
            printf("���α׷��� �����մϴ�.\n");
            return 0;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
        }
    }
    return 0;
}
