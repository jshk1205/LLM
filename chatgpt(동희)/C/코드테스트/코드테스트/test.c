#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#define _CRT_SECURE_NO_WARNINGS

// 함수 선언
void create_process();
void terminate_process(DWORD pid);
void check_process_status(DWORD pid);
void get_process_info(DWORD pid);
void set_process_priority(DWORD pid, DWORD priority);

int main() {
    DWORD pid;
    create_process();

    printf("프로세스 ID를 입력하세요: ");
    scanf_s("%lu", &pid);

    check_process_status(pid);
    get_process_info(pid);
    set_process_priority(pid, HIGH_PRIORITY_CLASS);
    terminate_process(pid);

    return 0;
}

void create_process() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char cmd[] = "C:\\Windows\\System32\\notepad.exe";

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess 실패. 오류 코드: %lu\n", GetLastError());
        return;
    }

    printf("자식 프로세스 생성됨. PID: %lu\n", pi.dwProcessId);

    // 자식 프로세스가 종료될 때까지 대기
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 핸들 닫기
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void terminate_process(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "프로세스 핸들을 열 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
        return;
    }

    if (!TerminateProcess(hProcess, 1)) {
        fprintf(stderr, "프로세스를 종료할 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
    }
    else {
        printf("프로세스 종료됨. PID: %lu\n", pid);
    }

    CloseHandle(hProcess);
}

void check_process_status(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "프로세스 핸들을 열 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
        return;
    }

    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        if (exitCode == STILL_ACTIVE) {
            printf("프로세스 실행 중. PID: %lu\n", pid);
        }
        else {
            printf("프로세스 종료됨. PID: %lu, 종료 코드: %lu\n", pid, exitCode);
        }
    }
    else {
        fprintf(stderr, "프로세스 상태를 가져올 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
    }

    CloseHandle(hProcess);
}

void get_process_info(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "프로세스 핸들을 열 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
        return;
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        printf("프로세스 메모리 사용량: %lu KB\n", pmc.WorkingSetSize / 1024);
    }
    else {
        fprintf(stderr, "프로세스 메모리 정보를 가져올 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
    }

    CloseHandle(hProcess);
}

void set_process_priority(DWORD pid, DWORD priority) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "프로세스 핸들을 열 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
        return;
    }

    if (!SetPriorityClass(hProcess, priority)) {
        fprintf(stderr, "프로세스 우선 순위를 설정할 수 없음. PID: %lu. 오류 코드: %lu\n", pid, GetLastError());
    }
    else {
        printf("프로세스 우선 순위 설정됨. PID: %lu, 우선 순위: %lu\n", pid, priority);
    }

    CloseHandle(hProcess);
}
