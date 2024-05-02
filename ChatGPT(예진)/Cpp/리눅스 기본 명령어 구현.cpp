#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <windows.h>

using namespace std;

void ls(const string& directory, int show_hidden, int long_format, int human_readable);
void cat(const string& filename, int number, int start, int end);
void echo(const string& text, const string& file, int append);
void human_readable_size(double size, string& result);

int main() {
    // ls 명령어 테스트
    cout << "ls 명령어 실행 결과:" << endl;
    ls(".", 1, 1, 1);

    // cat 명령어 테스트
    cout << "\ncat 명령어 실행 결과:" << endl;
    cat("example.txt", 1, 2, 4);

    // echo 명령어 테스트
    cout << "\necho 명령어 실행 결과:" << endl;
    echo("Hello, world!\nThis is a test.\n", "output.txt", 1);
    echo("Another line", "output.txt", 1);
    cat("output.txt", 0, 0, 0);

    return 0;
}

void ls(const string& directory, int show_hidden, int long_format, int human_readable) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char szDir[MAX_PATH];
    snprintf(szDir, sizeof(szDir), "%s\\*", directory.c_str());

    hFind = FindFirstFileA(szDir, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "Failed to find files in directory." << endl;
        return;
    }

    do {
        if (!show_hidden && (findFileData.cFileName[0] == '.' || (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))) {
            continue;
        }

        string path = directory + "\\" + findFileData.cFileName;

        struct stat file_stat;
        stat(path.c_str(), &file_stat);

        if (long_format) {
            string size_str;
            if (human_readable) {
                human_readable_size(file_stat.st_size, size_str);
            }
            else {
                stringstream ss;
                ss << file_stat.st_size;
                size_str = ss.str();
            }
            cout << oct << file_stat.st_mode << " " << file_stat.st_nlink << " " << file_stat.st_uid << " " << file_stat.st_gid << " " << size_str << " " << file_stat.st_mtime << " " << findFileData.cFileName << endl;
        }
        else {
            cout << findFileData.cFileName << endl;
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void cat(const string& filename, int number, int start, int end) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    int line_num = 1;
    while (getline(file, line)) {
        if (line_num >= start && line_num <= end) {
            if (number) {
                cout << line_num << ": " << line << endl;
            }
            else {
                cout << line << endl;
            }
        }
        if (line_num >= end) {
            break;
        }
        line_num++;
    }

    file.close();
}

void echo(const string& text, const string& file, int append) {
    ofstream output_file(file, append ? ios::app : ios::out);
    if (!output_file.is_open()) {
        cerr << "Failed to open file: " << file << endl;
        return;
    }

    output_file << text;
    output_file.close();
}

void human_readable_size(double size, string& result) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    int index = 0;

    while (size >= 1024 && index < sizeof(units) / sizeof(units[0]) - 1) {
        size /= 1024;
        index++;
    }

    stringstream ss;
    ss << fixed << size << " " << units[index];
    result = ss.str();
}
