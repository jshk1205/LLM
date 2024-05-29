#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void displayMenu() {
    printf("\n==== 바이너리 파일 에디터 ====\n");
    printf("1. 파일 열기\n");
    printf("2. 파일 읽기\n");
    printf("3. 파일 쓰기\n");
    printf("4. 파일 닫기\n");
    printf("5. 종료\n");
    printf("선택: ");
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
            printf("열 파일 이름: ");
            scanf_s("%s", filename, sizeof(filename));
            file = fopen(filename, "rb+");
            if (file == NULL) {
                printf("파일 열기 실패.\n");
            }
            break;
        case 2:
            if (file == NULL) {
                printf("파일을 먼저 열어야 합니다.\n");
                break;
            }
            fseek(file, 0, SEEK_SET);
            bytesRead = fread(buffer, 1, BUFFER_SIZE, file);
            if (bytesRead > 0) {
                printf("파일 내용:\n");
                for (int i = 0; i < bytesRead; i++) {
                    printf("%02X ", buffer[i]);
                }
                printf("\n");
            }
            break;
        case 3:
            if (file == NULL) {
                printf("파일을 먼저 열어야 합니다.\n");
                break;
            }
            printf("쓸 데이터를 입력하세요 (16진수): ");
            scanf_s("%s", buffer, sizeof(buffer));
            fseek(file, 0, SEEK_SET);
            bytesWritten = fwrite(buffer, 1, strlen(buffer), file);
            if (bytesWritten > 0) {
                printf("파일에 데이터를 성공적으로 썼습니다.\n");
            }
            break;
        case 4:
            if (file != NULL) {
                fclose(file);
                printf("파일을 닫았습니다.\n");
            }
            break;
        case 5:
            printf("프로그램을 종료합니다.\n");
            exit(0);
        default:
            printf("잘못된 선택입니다.\n");
            break;
        }
    }

    return 0;
}
