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

    // ���� ��� �Է� �ޱ�
    printf("Enter the file path: ");
    scanf_s("%s", file_path, BUFFER_SIZE);

    // ���� ����
    if (fopen_s(&file_ptr, file_path, "rb+") != 0) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // ���� ũ�� ���
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    // ���� ���� �б�
    buffer = (char*)malloc(file_size * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file_ptr);
        return 1;
    }

    fread(buffer, sizeof(char), file_size, file_ptr);

    // �޴� ǥ��
    printf("1. Read from file\n");
    printf("2. Write to file\n");
    printf("Enter your choice: ");
    scanf_s("%d", &choice);

    switch (choice) {
    case 1:
        // ���� ���� ���
        printf("File content: \n");
        for (int i = 0; i < file_size; i++) {
            printf("%c", buffer[i]);
        }
        printf("\n");
        break;
    case 2:
        // ���Ͽ� ����
        printf("Enter position to write: ");
        scanf_s("%d", &position);
        printf("Enter string to write: ");

        // ���� ���� �Է� �ޱ�
        input_string = (char*)malloc(BUFFER_SIZE * sizeof(char));
        if (input_string == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file_ptr);
            free(buffer);
            return 1;
        }
        scanf_s(" %[^\n]s", input_string, BUFFER_SIZE);

        // �Է��� ���ڿ��� ���Ͽ� ����
        fseek(file_ptr, position, SEEK_SET);
        fwrite(input_string, sizeof(char), strlen(input_string), file_ptr);
        printf("String written successfully.\n");

        free(input_string);
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }

    // ���� ������ �� �޸� ����
    fclose(file_ptr);
    free(buffer);

    return 0;
}
