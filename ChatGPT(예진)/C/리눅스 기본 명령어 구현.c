#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

void ls(const char* directory, int show_hidden, int long_format, int human_readable);
void cat(const char* filename, int number, int start, int end);
void echo(const char* text, const char* file, int append);
void human_readable_size(double size, char* result);

int main() {
    // ls 명령어 테스트
    printf("ls 명령어 실행 결과:\n");
    ls(".", 1, 1, 1);

    // cat 명령어 테스트
    printf("\ncat 명령어 실행 결과:\n");
    cat("example.txt", 1, 2, 4);

    // echo 명령어 테스트
    printf("\necho 명령어 실행 결과:\n");
    echo("Hello, world!\nThis is a test.\n", "output.txt", 1);
    echo("Another line", "output.txt", 1);
    cat("output.txt", 0, 0, 0);

    return 0;
}

void ls(const char* directory, int show_hidden, int long_format, int human_readable) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char szDir[MAX_PATH];
    snprintf(szDir, sizeof(szDir), "%s\\*", directory);

    hFind = FindFirstFileA(szDir, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to find files in directory.\n");
        return;
    }

    do {
        if (!show_hidden && (findFileData.cFileName[0] == '.' || (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))) {
            continue;
        }

        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s\\%s", directory, findFileData.cFileName);

        struct stat file_stat;
        stat(path, &file_stat);

        if (long_format) {
            char size_str[20];
            if (human_readable) {
                human_readable_size(file_stat.st_size, size_str);
            }
            else {
                snprintf(size_str, sizeof(size_str), "%ld", (long)file_stat.st_size);
            }
            printf("%o %ld %d %d %s %ld %s\n", file_stat.st_mode, (long)file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, size_str, (long)file_stat.st_mtime, findFileData.cFileName);
        }
        else {
            printf("%s\n", findFileData.cFileName);
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void cat(const char* filename, int number, int start, int end) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror(filename);
        return;
    }

    char buffer[BUFSIZ];
    int line_num = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (line_num >= start && line_num <= end) {
            if (number) {
                printf("%d: %s", line_num, buffer);
            }
            else {
                printf("%s", buffer);
            }
        }
        if (line_num >= end) {
            break;
        }
        line_num++;
    }

    fclose(file);
}

void echo(const char* text, const char* file, int append) {
    FILE* output_file = fopen(file, append ? "a" : "w");
    if (output_file == NULL) {
        perror(file);
        return;
    }

    fprintf(output_file, "%s", text);
    fclose(output_file);
}

void human_readable_size(double size, char* result) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    int index = 0;

    while (size >= 1024 && index < sizeof(units) / sizeof(units[0]) - 1) {
        size /= 1024;
        index++;
    }

    snprintf(result, 20, "%.2f %s", size, units[index]);
}
