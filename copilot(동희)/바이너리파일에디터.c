#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void displayMenu() {
    printf("\n==== ���̳ʸ� ���� ������ ====\n");
    printf("1. ���� ����\n");
    printf("2. ���� �б�\n");
    printf("3. ���� ����\n");
    printf("4. ���� �ݱ�\n");
    printf("5. ����\n");
    printf("����: ");
}

int main() {
    FILE* file;
    char filename[100];
    unsigned char buffer[BUFFER_SIZE];
    int choice;
    int bytesRead, bytesWritten;

    while (1) {
        displayMenu();
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("�� ���� �̸�: ");
            scanf_s("%s", filename, sizeof(filename));
            file = fopen(filename, "rb+");
            if (file == NULL) {
                printf("���� ���� ����.\n");
            }
            break;
        case 2:
            if (file == NULL) {
                printf("������ ���� ����� �մϴ�.\n");
                break;
            }
            fseek(file, 0, SEEK_SET);
            bytesRead = fread(buffer, 1, BUFFER_SIZE, file);
            if (bytesRead > 0) {
                printf("���� ����:\n");
                for (int i = 0; i < bytesRead; i++) {
                    printf("%02X ", buffer[i]);
                }
                printf("\n");
            }
            break;
        case 3:
            if (file == NULL) {
                printf("������ ���� ����� �մϴ�.\n");
                break;
            }
            printf("�� �����͸� �Է��ϼ��� (16����): ");
            scanf_s("%s", buffer, sizeof(buffer));
            fseek(file, 0, SEEK_SET);
            bytesWritten = fwrite(buffer, 1, strlen(buffer), file);
            if (bytesWritten > 0) {
                printf("���Ͽ� �����͸� ���������� ����ϴ�.\n");
            }
            break;
        case 4:
            if (file != NULL) {
                fclose(file);
                printf("������ �ݾҽ��ϴ�.\n");
            }
            break;
        case 5:
            printf("���α׷��� �����մϴ�.\n");
            exit(0);
        default:
            printf("�߸��� �����Դϴ�.\n");
            break;
        }
    }

    return 0;
}
