// 바이너리 에디터를 만들려고함
// 16진수로 보여주고 파일을 고칠 수 있도록 해야됨
// c언어로 만들것
// CRT_SECURE_NO_WARNINGS를 정의하고 안전한 함수로 바꿔야함
// 파일을 열어서 16진수로 보여주는 함수를 만들어야함
// 파일을 수정할 수 있는 함수를 만들어야함
// 파일을 저장할 수 있는 함수를 만들어야함
// 파일을 닫을 수 있는 함수를 만들어야함
// 파일을 열었는지 여부를 확인할 수 있는 함수를 만들어야함

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024
#define CRT_SECURE_NO_WARNINGS

typedef struct
{
    FILE *file;
    char filename[MAX_BUFFER_SIZE];
    unsigned char *buffer;
    size_t filesize;
} BinaryEditor;

BinaryEditor *createBinaryEditor()
{
    BinaryEditor *editor = (BinaryEditor *)malloc(sizeof(BinaryEditor));
    if (editor == NULL)
    {
        return NULL;
    }

    editor->file = NULL;
    editor->buffer = NULL;
    editor->filesize = 0;

    return editor;
}

void destroyBinaryEditor(BinaryEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->file != NULL)
    {
        fclose(editor->file);
    }

    if (editor->buffer != NULL)
    {
        free(editor->buffer);
    }

    free(editor);
}

void openFile(BinaryEditor *editor, const char *filename)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->file != NULL)
    {
        fclose(editor->file);
    }

    errno_t err = fopen_s(&(editor->file), filename, "rb+");
    if (err != 0 || editor->file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    strncpy_s(editor->filename, sizeof(editor->filename), filename, _TRUNCATE);
    fseek(editor->file, 0, SEEK_END);
    editor->filesize = ftell(editor->file);
    fseek(editor->file, 0, SEEK_SET);

    editor->buffer = (unsigned char *)malloc(editor->filesize);
    if (editor->buffer == NULL)
    {
        fclose(editor->file);
        editor->file = NULL;
        editor->filesize = 0;
        return;
    }

    fread(editor->buffer, 1, editor->filesize, editor->file);
}

void showFile(BinaryEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->file == NULL)
    {
        printf("File is not opened.\n");
        return;
    }

    printf("File: %s\n", editor->filename);
    printf("Size: %zu bytes\n", editor->filesize);
    printf("Offset\tValue\n");
    for (size_t i = 0; i < editor->filesize; i++)
    {
        printf("%zu\t0x%02X\n", i, editor->buffer[i]);
    }
}

void editFile(BinaryEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->file == NULL)
    {
        printf("File is not opened.\n");
        return;
    }

    int offset;
    printf("Enter the offset to edit: ");
    scanf_s("%d", &offset);

    if (offset < 0 || offset >= editor->filesize)
    {
        printf("Invalid offset.\n");
        return;
    }

    int value;

    printf("Enter the value to edit (in hexadecimal): ");
    scanf_s("%x", &value);

    editor->buffer[offset] = (unsigned char)value;
}

void saveFile(BinaryEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->file == NULL)
    {
        printf("File is not opened.\n");
        return;
    }

    fseek(editor->file, 0, SEEK_SET);
    fwrite(editor->buffer, 1, editor->filesize, editor->file);
}

void closeFile(BinaryEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->file != NULL)
    {
        fclose(editor->file);
        editor->file = NULL;
    }

    if (editor->buffer != NULL)
    {
        free(editor->buffer);
        editor->buffer = NULL;
    }

    editor->filesize = 0;
}

int main()
{
    BinaryEditor *editor = createBinaryEditor();
    if (editor == NULL)
    {
        return 1;
    }

    int choice;
    char filename[MAX_BUFFER_SIZE];

    while (1)
    {
        printf("1. Open file\n");
        printf("2. Show file\n");
        printf("3. Edit file\n");
        printf("4. Save file\n");
        printf("5. Close file\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf_s("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the filename: ");
            scanf_s("%s", filename, sizeof(filename));
            openFile(editor, filename);
            break;
        case 2:
            showFile(editor);
            break;
        case 3:
            editFile(editor);
            break;
        case 4:
            saveFile(editor);
            break;
        case 5:
            closeFile(editor);
            break;
        case 6:
            destroyBinaryEditor(editor);
            return 0;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }

    return 0;
}

// Path: copilot%28%EB%8F%99%ED%9D%AC%29/C/BinaryEditor.c