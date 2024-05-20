#include <stdio.h>
#include <Windows.h>

// 디스크 사용량 모니터링 함수
// directory: 모니터링할 디렉토리 경로
void monitorDiskUsage(const char* directory);

// 디렉토리 내 파일 목록 출력 함수
// directory: 파일 목록을 출력할 디렉토리 경로
void listFiles(const char* directory);

// 파일 및 디렉토리 속성 출력 함수
// filePath: 속성을 확인할 파일 또는 디렉토리 경로
void printFileAttributes(const char* filePath);

// 파일 크기 가져오는 함수
// filePath: 크기를 확인할 파일 경로
void getFileSize(const char* filePath);

// 특정 확장자를 가진 파일 개수 세는 함수
// directory: 파일을 검색할 디렉토리 경로
// extension: 세고 싶은 확장자
void countFilesWithExtension(const char* directory, const char* extension);

// 가장 큰 파일 크기를 찾는 함수
// directory: 파일을 검색할 디렉토리 경로
void findLargestFile(const char* directory);

// 가장 오래된 파일을 찾는 함수
// directory: 파일을 검색할 디렉토리 경로
void findOldestFile(const char* directory);

// 파일 생성 시간 가져오는 함수
// filePath: 생성 시간을 확인할 파일 경로
void getFileCreationTime(const char* filePath);

int main() {
    // 모니터링할 디렉토리 경로
    const char* directory = "C:\\";

    // 디스크 사용량 모니터링
    monitorDiskUsage(directory);

    // 디렉토리 내 파일 목록 출력
    listFiles(directory);

    // 특정 확장자를 가진 파일 개수 세기
    const char* extension = "txt";
    countFilesWithExtension(directory, extension);

    // 가장 큰 파일 찾기
    findLargestFile(directory);

    // 가장 오래된 파일 찾기
    findOldestFile(directory);

    return 0;
}

// 디스크 사용량 모니터링 함수
void monitorDiskUsage(const char* directory) {
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

    // 디스크 사용량 가져오기
    if (!GetDiskFreeSpaceExA(directory, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        printf("Failed to get disk usage.\n");
        return;
    }

    // 디스크 사용량 출력
    printf("Total disk space: %llu bytes\n", totalNumberOfBytes.QuadPart);
    printf("Free disk space: %llu bytes\n", totalNumberOfFreeBytes.QuadPart);
}

// 디렉토리 내 파일 목록 출력 함수
void listFiles(const char* directory) {
    // 디렉토리 경로 조합
    char searchPath[MAX_PATH];
    sprintf_s(searchPath, "%s\\*", directory);

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to find files in directory.\n");
        return;
    }

    printf("Files in directory:\n");
    do {
        // 디렉토리 경로와 파일 이름을 조합하여 완전한 파일 경로 생성
        char filePath[MAX_PATH];
        sprintf_s(filePath, "%s\\%s", directory, findFileData.cFileName);
        printf("%s\n", findFileData.cFileName);
        printFileAttributes(filePath);
        getFileSize(filePath);
        getFileCreationTime(filePath);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// 파일 및 디렉토리 속성 출력 함수
void printFileAttributes(const char* filePath) {
    DWORD attributes = GetFileAttributesA(filePath);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Failed to get file attributes.\n");
        return;
    }

    printf("File attributes of %s:\n", filePath);
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        printf("Directory\n");
    }
    if (attributes & FILE_ATTRIBUTE_ARCHIVE) {
        printf("Archive\n");
    }
    if (attributes & FILE_ATTRIBUTE_HIDDEN) {
        printf("Hidden\n");
    }
    // 추가적인 속성은 필요에 따라 여기에 추가할 수 있습니다.
}

// 파일 크기 가져오는 함수
void getFileSize(const char* filePath) {
    HANDLE hFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file %s.\n", filePath);
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Failed to get file size of %s.\n", filePath);
    }
    else {
        printf("File size of %s: %u bytes\n", filePath, fileSize);
    }

    CloseHandle(hFile);
}

// 특정 확장자를 가진 파일 개수 세는 함수
void countFilesWithExtension(const char* directory, const char* extension) {
    // 디렉토리 경로 조합
    char searchPath[MAX_PATH];
    sprintf_s(searchPath, "%s\\*.%s", directory, extension);

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);
    int count = 0;

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to find files in directory.\n");
        return;
    }

    do {
        count++;
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    printf("Number of files with extension '%s': %d\n", extension, count);
}

// 가장 큰 파일 크기를 찾는 함수


// 가장 큰 파일 크기를 찾는 함수
void findLargestFile(const char* directory) {
    // 디렉토리 경로 조합
    char searchPath[MAX_PATH];
    sprintf_s(searchPath, "%s\\*", directory);

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);
    ULONGLONG maxSize = 0;
    char largest[MAX_PATH];

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to find files in directory.\n");
        return;
    }

    do {
        ULONGLONG fileSize = (ULONGLONG)findFileData.nFileSizeHigh << 32 | findFileData.nFileSizeLow;
        if (fileSize > maxSize) {
            maxSize = fileSize;
            strcpy_s(largest, MAX_PATH, findFileData.cFileName);
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    printf("Largest file: %s, Size: %llu bytes\n", largest, maxSize);
}

// 가장 오래된 파일을 찾는 함수
void findOldestFile(const char* directory) {
    // 디렉토리 경로 조합
    char searchPath[MAX_PATH];
    sprintf_s(searchPath, "%s\\*", directory);

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);
    FILETIME oldestTime = findFileData.ftCreationTime;
    char oldestFile[MAX_PATH];

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to find files in directory.\n");
        return;
    }

    do {
        FILETIME creationTime = findFileData.ftCreationTime;
        if (CompareFileTime(&creationTime, &oldestTime) < 0) {
            oldestTime = creationTime;
            strcpy_s(oldestFile, MAX_PATH, findFileData.cFileName);
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    printf("Oldest file: %s\n", oldestFile);
}

// 파일 생성 시간 가져오는 함수
void getFileCreationTime(const char* filePath) {
    HANDLE hFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file %s.\n", filePath);
        return;
    }

    FILETIME creationTime, lastAccessTime, lastWriteTime;
    if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime)) {
        // FileTime을 시스템 시간으로 변환
        SYSTEMTIME sysCreationTime;
        FileTimeToSystemTime(&creationTime, &sysCreationTime);

        // 변환된 시간 출력
        printf("Creation time of %s: %d/%02d/%02d %02d:%02d:%02d\n", filePath,
            sysCreationTime.wYear, sysCreationTime.wMonth, sysCreationTime.wDay,
            sysCreationTime.wHour, sysCreationTime.wMinute, sysCreationTime.wSecond);
    }
    else {
        printf("Failed to get creation time of file %s.\n", filePath);
    }

    CloseHandle(hFile);
}
