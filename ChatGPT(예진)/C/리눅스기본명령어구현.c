#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h> // Windows용 디렉토리 관련 기능을 위한 헤더 파일
#include <windows.h>

void ls(const char* directory, int show_hidden, int long_format, int human_readable);
void cat(const char* filename, int number, int start, int end);
void echo(const char* text, const char* file, int append);
char* human_readable_size(off_t size);

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
    struct _finddata_t file_info;
    intptr_t handle;
    char search_path[1024];
    snprintf(search_path, sizeof(search_path), "%s/*", directory);

    if ((handle = _findfirst(search_path, &file_info)) == -1L) {
        perror("_findfirst");
        return;
    }

    do {
        if (!show_hidden && (strcmp(file_info.name, ".") == 0 || strcmp(file_info.name, "..") == 0))
            continue;

        struct stat file_stat;
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, file_info.name);
        if (stat(filepath, &file_stat) < 0) {
            perror("stat");
            continue;
        }

        if (long_format) {
            char* size_str = human_readable_size(file_stat.st_size);
            printf("%lo %lu %u %u %s %ld %s\n", (unsigned long)file_stat.st_mode, (unsigned long)file_stat.st_nlink, (unsigned int)file_stat.st_uid, (unsigned int)file_stat.st_gid, size_str, (long)file_stat.st_mtime, file_info.name);
            free(size_str);
        }
        else {
            printf("%s\n", file_info.name);
        }
    } while (_findnext(handle, &file_info) == 0);

    _findclose(handle);
}

void cat(const char* filename, int number, int start, int end) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char buffer[1024];
    int line_number = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_number++;
        if (line_number >= start && line_number <= end) {
            if (number) {
                printf("%d: %s", line_number, buffer);
            }
            else {
                printf("%s", buffer);
            }
        }
    }

    fclose(file);
}

void echo(const char* text, const char* file, int append) {
    FILE* output;
    if (append)
        output = fopen(file, "a");
    else
        output = fopen(file, "w");

    if (output == NULL) {
        perror("fopen");
        return;
    }

    fprintf(output, "%s", text);
    fclose(output);
}

char* human_readable_size(off_t size) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    int index = 0;

    double size_temp = (double)size;
    while (size_temp >= 1024 && index < sizeof(units) / sizeof(units[0]) - 1) {
        size_temp /= 1024;
        index++;
    }

    char* size_str = (char*)malloc(20 * sizeof(char));
    sprintf(size_str, "%.2f %s", size_temp, units[index]);

    return size_str;
}
