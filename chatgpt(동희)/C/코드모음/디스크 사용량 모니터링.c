#include <stdio.h>
#include <Windows.h>

// ��ũ ��뷮 ����͸� �Լ�
// directory: ����͸��� ���丮 ���
void monitorDiskUsage(const char* directory);

// ���丮 �� ���� ��� ��� �Լ�
// directory: ���� ����� ����� ���丮 ���
void listFiles(const char* directory);

// ���� �� ���丮 �Ӽ� ��� �Լ�
// filePath: �Ӽ��� Ȯ���� ���� �Ǵ� ���丮 ���
void printFileAttributes(const char* filePath);

// ���� ũ�� �������� �Լ�
// filePath: ũ�⸦ Ȯ���� ���� ���
void getFileSize(const char* filePath);

// Ư�� Ȯ���ڸ� ���� ���� ���� ���� �Լ�
// directory: ������ �˻��� ���丮 ���
// extension: ���� ���� Ȯ����
void countFilesWithExtension(const char* directory, const char* extension);

// ���� ū ���� ũ�⸦ ã�� �Լ�
// directory: ������ �˻��� ���丮 ���
void findLargestFile(const char* directory);

// ���� ������ ������ ã�� �Լ�
// directory: ������ �˻��� ���丮 ���
void findOldestFile(const char* directory);

// ���� ���� �ð� �������� �Լ�
// filePath: ���� �ð��� Ȯ���� ���� ���
void getFileCreationTime(const char* filePath);

int main() {
    // ����͸��� ���丮 ���
    const char* directory = "C:\\";

    // ��ũ ��뷮 ����͸�
    monitorDiskUsage(directory);

    // ���丮 �� ���� ��� ���
    listFiles(directory);

    // Ư�� Ȯ���ڸ� ���� ���� ���� ����
    const char* extension = "txt";
    countFilesWithExtension(directory, extension);

    // ���� ū ���� ã��
    findLargestFile(directory);

    // ���� ������ ���� ã��
    findOldestFile(directory);

    return 0;
}

// ��ũ ��뷮 ����͸� �Լ�
void monitorDiskUsage(const char* directory) {
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

    // ��ũ ��뷮 ��������
    if (!GetDiskFreeSpaceExA(directory, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        printf("Failed to get disk usage.\n");
        return;
    }

    // ��ũ ��뷮 ���
    printf("Total disk space: %llu bytes\n", totalNumberOfBytes.QuadPart);
    printf("Free disk space: %llu bytes\n", totalNumberOfFreeBytes.QuadPart);
}

// ���丮 �� ���� ��� ��� �Լ�
void listFiles(const char* directory) {
    // ���丮 ��� ����
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
        // ���丮 ��ο� ���� �̸��� �����Ͽ� ������ ���� ��� ����
        char filePath[MAX_PATH];
        sprintf_s(filePath, "%s\\%s", directory, findFileData.cFileName);
        printf("%s\n", findFileData.cFileName);
        printFileAttributes(filePath);
        getFileSize(filePath);
        getFileCreationTime(filePath);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// ���� �� ���丮 �Ӽ� ��� �Լ�
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
    // �߰����� �Ӽ��� �ʿ信 ���� ���⿡ �߰��� �� �ֽ��ϴ�.
}

// ���� ũ�� �������� �Լ�
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

// Ư�� Ȯ���ڸ� ���� ���� ���� ���� �Լ�
void countFilesWithExtension(const char* directory, const char* extension) {
    // ���丮 ��� ����
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

// ���� ū ���� ũ�⸦ ã�� �Լ�


// ���� ū ���� ũ�⸦ ã�� �Լ�
void findLargestFile(const char* directory) {
    // ���丮 ��� ����
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

// ���� ������ ������ ã�� �Լ�
void findOldestFile(const char* directory) {
    // ���丮 ��� ����
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

// ���� ���� �ð� �������� �Լ�
void getFileCreationTime(const char* filePath) {
    HANDLE hFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file %s.\n", filePath);
        return;
    }

    FILETIME creationTime, lastAccessTime, lastWriteTime;
    if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime)) {
        // FileTime�� �ý��� �ð����� ��ȯ
        SYSTEMTIME sysCreationTime;
        FileTimeToSystemTime(&creationTime, &sysCreationTime);

        // ��ȯ�� �ð� ���
        printf("Creation time of %s: %d/%02d/%02d %02d:%02d:%02d\n", filePath,
            sysCreationTime.wYear, sysCreationTime.wMonth, sysCreationTime.wDay,
            sysCreationTime.wHour, sysCreationTime.wMinute, sysCreationTime.wSecond);
    }
    else {
        printf("Failed to get creation time of file %s.\n", filePath);
    }

    CloseHandle(hFile);
}
