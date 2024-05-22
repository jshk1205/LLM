#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#define _CRT_SECURE_NO_WARNINGS

// �Լ� ����
void create_process();
void terminate_process(DWORD pid);
void check_process_status(DWORD pid);
void get_process_info(DWORD pid);
void set_process_priority(DWORD pid, DWORD priority);

int main() {
    DWORD pid;
    create_process();

    printf("���μ��� ID�� �Է��ϼ���: ");
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
        fprintf(stderr, "CreateProcess ����. ���� �ڵ�: %lu\n", GetLastError());
        return;
    }

    printf("�ڽ� ���μ��� ������. PID: %lu\n", pi.dwProcessId);

    // �ڽ� ���μ����� ����� ������ ���
    WaitForSingleObject(pi.hProcess, INFINITE);

    // �ڵ� �ݱ�
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void terminate_process(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "���μ��� �ڵ��� �� �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
        return;
    }

    if (!TerminateProcess(hProcess, 1)) {
        fprintf(stderr, "���μ����� ������ �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
    }
    else {
        printf("���μ��� �����. PID: %lu\n", pid);
    }

    CloseHandle(hProcess);
}

void check_process_status(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "���μ��� �ڵ��� �� �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
        return;
    }

    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        if (exitCode == STILL_ACTIVE) {
            printf("���μ��� ���� ��. PID: %lu\n", pid);
        }
        else {
            printf("���μ��� �����. PID: %lu, ���� �ڵ�: %lu\n", pid, exitCode);
        }
    }
    else {
        fprintf(stderr, "���μ��� ���¸� ������ �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
    }

    CloseHandle(hProcess);
}

void get_process_info(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "���μ��� �ڵ��� �� �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
        return;
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        printf("���μ��� �޸� ��뷮: %lu KB\n", pmc.WorkingSetSize / 1024);
    }
    else {
        fprintf(stderr, "���μ��� �޸� ������ ������ �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
    }

    CloseHandle(hProcess);
}

void set_process_priority(DWORD pid, DWORD priority) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, "���μ��� �ڵ��� �� �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
        return;
    }

    if (!SetPriorityClass(hProcess, priority)) {
        fprintf(stderr, "���μ��� �켱 ������ ������ �� ����. PID: %lu. ���� �ڵ�: %lu\n", pid, GetLastError());
    }
    else {
        printf("���μ��� �켱 ���� ������. PID: %lu, �켱 ����: %lu\n", pid, priority);
    }

    CloseHandle(hProcess);
}
