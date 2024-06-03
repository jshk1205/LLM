#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifndef SYMBOLIC_LINK_FLAG_FILE
#define SYMBOLIC_LINK_FLAG_FILE 0x0 // 파일을 가리키는 심볼릭 링크 플래그
#endif

//코드를 수정해달라고 요청해도 제대로 수정해주지 않고, 항상 copy failed가 뜹니다 ㅜㅜ

void create_symbolic_link() {
    char source_file[MAX_PATH];
    printf("Enter the path of the file you want to copy: ");
    fgets(source_file, sizeof(source_file), stdin);
    source_file[strcspn(source_file, "\n")] = 0; // Remove trailing newline

    if (GetFileAttributesA(source_file) == INVALID_FILE_ATTRIBUTES) {
        printf("The specified file does not exist.\n");
        return;
    }

    char target_directory[MAX_PATH];
    printf("Enter the directory where you want to copy the file: ");
    fgets(target_directory, sizeof(target_directory), stdin);
    target_directory[strcspn(target_directory, "\n")] = 0; // Remove trailing newline

    // Check if target directory exists, if not, create it
    if (GetFileAttributesA(target_directory) == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(target_directory, NULL)) {
            printf("Failed to create target directory.\n");
            return;
        }
    }

    char filename[MAX_PATH];
    char link_name[MAX_PATH];

    // Get filename from source_file
    char* token = strtok(source_file, "\\");
    while (token != NULL) {
        strcpy(filename, token);
        token = strtok(NULL, "\\");
    }

    // Copy files
    if (!CopyFileA(source_file, strcat(target_directory, "\\"), FALSE)) {
        printf("Failed to copy file.\n");
        return;
    }
    printf("File copied to %s\n", target_directory);

    // Creating a Symbolic Link
    printf("Enter the name for the symbolic link: ");
    fgets(link_name, sizeof(link_name), stdin);
    link_name[strcspn(link_name, "\n")] = 0; // Remove trailing newline

    if (!CreateSymbolicLinkA(link_name, strcat(target_directory, filename), SYMBOLIC_LINK_FLAG_FILE)) {
        printf("Failed to create symbolic link.\n");
        return;
    }
    printf("Symbolic link created from %s to %s\n", link_name, target_directory);
}

// Examples of Use
int main() {
    create_symbolic_link();
    return 0;
}
