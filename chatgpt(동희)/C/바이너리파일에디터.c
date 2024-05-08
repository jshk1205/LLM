#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main() {
    char file_path[BUFFER_SIZE];
    FILE* file_ptr;
    long file_size;
    char* buffer;
    int choice;
    int position;
    char* input_string;

    // 파일 경로 입력 받기
    printf("Enter the file path: ");
    scanf_s("%s", file_path, BUFFER_SIZE);

    // 파일 열기
    if (fopen_s(&file_ptr, file_path, "rb+") != 0) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // 파일 크기 계산
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    // 파일 내용 읽기
    buffer = (char*)malloc(file_size * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file_ptr);
        return 1;
    }

    fread(buffer, sizeof(char), file_size, file_ptr);

    // 메뉴 표시
    printf("1. Read from file\n");
    printf("2. Write to file\n");
    printf("Enter your choice: ");
    scanf_s("%d", &choice);

    switch (choice) {
    case 1:
        // 파일 내용 출력
        printf("File content: \n");
        for (int i = 0; i < file_size; i++) {
            printf("%c", buffer[i]);
        }
        printf("\n");
        break;
    case 2:
        // 파일에 쓰기
        printf("Enter position to write: ");
        scanf_s("%d", &position);
        printf("Enter string to write: ");

        // 여러 문자 입력 받기
        input_string = (char*)malloc(BUFFER_SIZE * sizeof(char));
        if (input_string == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file_ptr);
            free(buffer);
            return 1;
        }
        scanf_s(" %[^\n]s", input_string, BUFFER_SIZE);

        // 입력한 문자열을 파일에 쓰기
        fseek(file_ptr, position, SEEK_SET);
        fwrite(input_string, sizeof(char), strlen(input_string), file_ptr);
        printf("String written successfully.\n");

        free(input_string);
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }

    // 파일 포인터 및 메모리 해제
    fclose(file_ptr);
    free(buffer);

    return 0;
}
