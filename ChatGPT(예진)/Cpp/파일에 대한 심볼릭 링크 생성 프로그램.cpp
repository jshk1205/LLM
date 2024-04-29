#include <iostream>
#include <Windows.h>
#include <string>

int main() {
    wchar_t source_file[MAX_PATH];
    wchar_t link_name[MAX_PATH];

    // 사용자로부터 파일 경로 입력 받기
    std::wcout << L"Enter the path of the file you want to link: ";
    std::wcin.getline(source_file, sizeof(source_file));

    // 사용자로부터 심볼릭 링크의 이름 입력 받기
    std::wcout << L"Enter the name for the symbolic link: ";
    std::wcin.getline(link_name, sizeof(link_name));

    // 심볼릭 링크 생성
    if (CreateSymbolicLinkW(link_name, source_file, 0) != 0) {
        std::wcout << L"Symbolic link created from " << link_name << L" to " << source_file << std::endl;
    }
    else {
        std::wcerr << L"Failed to create symbolic link" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
