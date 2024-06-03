#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <windows.h>

// copy failed 오류뜨는 거 gpt가 수정 제대로 안 해 줍니다...

#ifndef SYMBOLIC_LINK_FLAG_FILE
#define SYMBOLIC_LINK_FLAG_FILE 0x0 // 파일을 가리키는 심볼릭 링크 플래그
#endif

void create_symbolic_link() {
    char source_file[MAX_PATH];
    std::cout << "Enter the path of the file you want to copy: ";
    std::cin.getline(source_file, sizeof(source_file)); // Use cin.getline for input

    if (GetFileAttributesA(source_file) == INVALID_FILE_ATTRIBUTES) {
        std::cout << "The specified file does not exist." << std::endl;
        return;
    }

    char target_directory[MAX_PATH];
    std::cout << "Enter the directory where you want to copy the file: ";
    std::cin.getline(target_directory, sizeof(target_directory)); // Use cin.getline for input

    // Check if target directory exists, if not, create it
    if (!std::filesystem::exists(target_directory)) {
        if (!std::filesystem::create_directories(target_directory)) {
            std::cout << "Failed to create target directory." << std::endl;
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
    char target_file[MAX_PATH];
    sprintf(target_file, "%s\\%s", target_directory, filename); // Construct full path for target file
    if (!CopyFileA(source_file, target_file, FALSE)) {
        std::cout << "Failed to copy file." << std::endl;
        return;
    }
    std::cout << "File copied to " << target_file << std::endl;

    // Creating a Symbolic Link
    std::cout << "Enter the name for the symbolic link: ";
    std::cin.getline(link_name, sizeof(link_name)); // Use cin.getline for input

    if (!CreateSymbolicLinkA(link_name, target_file, SYMBOLIC_LINK_FLAG_FILE)) {
        std::cout << "Failed to create symbolic link." << std::endl;
        return;
    }
    std::cout << "Symbolic link created from " << link_name << " to " << target_file << std::endl;
}

// Examples of Use
int main() {
    create_symbolic_link();
    return 0;
}
