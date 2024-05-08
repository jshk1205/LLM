#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> // 추가된 헤더 파일
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <windows.h>

void ls(const std::string& directory, int show_hidden, int long_format, int human_readable);
void cat(const std::string& filename, int number, int start, int end);
void echo(const std::string& text, const std::string& file, int append);
std::string human_readable_size(off_t size);

int main() {
    // ls 명령어 테스트
    std::cout << "ls 명령어 실행 결과:\n";
    ls(".", 1, 1, 1);

    // cat 명령어 테스트
    std::cout << "\ncat 명령어 실행 결과:\n";
    cat("example.txt", 1, 2, 4);

    // echo 명령어 테스트
    std::cout << "\necho 명령어 실행 결과:\n";
    echo("Hello, world!\nThis is a test.\n", "output.txt", 1);
    echo("Another line", "output.txt", 1);
    cat("output.txt", 0, 0, 0);

    return 0;
}

void ls(const std::string& directory, int show_hidden, int long_format, int human_readable) {
    struct _finddata_t file_info;
    intptr_t handle;
    std::string search_path = directory + "/*";
    if ((handle = _findfirst(search_path.c_str(), &file_info)) == -1L) {
        perror("_findfirst");
        return;
    }
    do {
        if (!show_hidden && (strcmp(file_info.name, ".") == 0 || strcmp(file_info.name, "..") == 0))
            continue;
        struct stat file_stat;
        std::string filepath = directory + "/" + file_info.name;
        if (stat(filepath.c_str(), &file_stat) < 0) {
            perror("stat");
            continue;
        }
        if (long_format) {
            std::string size_str = human_readable_size(file_stat.st_size);
            std::cout << std::oct << file_stat.st_mode << " " << file_stat.st_nlink << " " << file_stat.st_uid << " " << file_stat.st_gid << " " << size_str << " " << file_stat.st_mtime << " " << file_info.name << "\n";
        }
        else {
            std::cout << file_info.name << "\n";
        }
    } while (_findnext(handle, &file_info) == 0);
    _findclose(handle);
}

void cat(const std::string& filename, int number, int start, int end) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        perror("fopen");
        return;
    }
    std::string line;
    int line_number = 0;
    while (std::getline(file, line)) {
        line_number++;
        if (line_number >= start && line_number <= end) {
            if (number) {
                std::cout << line_number << ": " << line;
            }
            else {
                std::cout << line;
            }
        }
    }
    file.close();
}

void echo(const std::string& text, const std::string& file, int append) {
    std::ofstream output;
    if (append)
        output.open(file, std::ios::app);
    else
        output.open(file);
    if (!output.is_open()) {
        perror("fopen");
        return;
    }
    output << text;
    output.close();
}

std::string human_readable_size(off_t size) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    int index = 0;
    double size_temp = static_cast<double>(size);
    while (size_temp >= 1024 && index < sizeof(units) / sizeof(units[0]) - 1) {
        size_temp /= 1024;
        index++;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size_temp << " " << units[index];
    return oss.str();
}
