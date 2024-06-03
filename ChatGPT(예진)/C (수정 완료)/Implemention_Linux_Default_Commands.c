#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <time.h>

void ls(const char* directory, int show_hidden, int long_format, int human_readable);
void cat(const char* filename, int number, int start, int end);
void echo(const char* text, const char* file, int append);
char* human_readable_size(off_t size);
char* format_time(time_t raw_time);

int main() {
    // ls 명령어 테스트
    printf("ls 명령어 실행 결과:\n");
    ls(".", 1, 1, 1);

    // echo 명령어 테스트
    printf("\necho 명령어 실행 결과:\n");
    echo("Hello, world!\nThis is a test.\n", "output.txt", 0);  // 파일 덮어쓰기
    echo("Another line\n", "output.txt", 1);  // 파일에 덧붙이기
    cat("output.txt", 0, 0, 0);

    // cat 명령어 테스트 (존재하는 파일로 교체)
    printf("\ncat 명령어 실행 결과:\n");
    cat("output.txt", 1, 2, 4);

    return 0;
}

void ls(const char* directory, int show_hidden, int long_format, int human_readable) {
    struct _finddata_t file_info;
    intptr_t handle;
    char search_path[FILENAME_MAX];
    sprintf(search_path, "%s/*", directory);
    if ((handle = _findfirst(search_path, &file_info)) == -1L) {
        perror("_findfirst");
        return;
    }
    do {
        if (!show_hidden && (file_info.attrib & _A_HIDDEN))
            continue;
        if (strcmp(file_info.name, ".") == 0 || strcmp(file_info.name, "..") == 0)
            continue;

        struct _stat file_stat;
        char filepath[FILENAME_MAX];
        sprintf(filepath, "%s/%s", directory, file_info.name);
        if (_stat(filepath, &file_stat) < 0) {
            perror("stat");
            continue;
        }
        if (long_format) {
            char* size_str = human_readable ? human_readable_size(file_stat.st_size) : human_readable_size(file_stat.st_size);
            char* time_str = format_time(file_stat.st_mtime);
            printf("%o %d %d %d %s %s %s\n",
                file_stat.st_mode, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, size_str, time_str, file_info.name);
            free(size_str);
            free(time_str);
        }
        else {
            printf("%s\n", file_info.name);
        }
    } while (_findnext(handle, &file_info) == 0);
    _findclose(handle);
}

void cat(const char* filename, int number, int start, int end) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return;
    }
    char line[BUFSIZ];
    int line_number = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        if ((start == 0 && end == 0) || (line_number >= start && line_number <= end)) {
            if (number) {
                printf("%d: %s", line_number, line);
            }
            else {
                printf("%s", line);
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
    if (!output) {
        perror(file);
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
    char* result = malloc(20); // Sufficiently large buffer
    if (!result) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    snprintf(result, 20, "%.2f %s", size_temp, units[index]);
    return result;
}

char* format_time(time_t raw_time) {
    struct tm* timeinfo = localtime(&raw_time);
    char* buffer = malloc(80); // Sufficiently large buffer
    if (!buffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}
