#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_THRESHOLD 90
#define DEFAULT_DURATION 10
#define DEFAULT_MAX_CONTINUOUS_DURATION 30
#define DEFAULT_MAX_CONSECUTIVE_THRESHOLD 95
#define DEFAULT_INTERVAL 5

volatile bool isMonitoring = true;

DWORD WINAPI monitorKeyboardInput(LPVOID lpParam) {
    while (true) {
        printf("---Press 'p' to stop monitoring---\n");
        char key;
        scanf(" %c", &key);
        if (key == 'p') {
            isMonitoring = false;
            break;
        }
        else if (key == 'c') {
            printf("What action would you like to perform?\n");
            printf("1. Lower process priority\n");
            printf("2. Kill specific process\n");
            printf("Enter your choice (1/2): ");
            char choice;
            scanf(" %c", &choice);
            if (choice == '1') {
                printf("Enter the name of the process you want to lower priority for: ");
                char processName[256];
                scanf("%s", processName);
                HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot != INVALID_HANDLE_VALUE) {
                    PROCESSENTRY32 pe32;
                    pe32.dwSize = sizeof(PROCESSENTRY32);
                    if (Process32First(hSnapshot, &pe32)) {
                        do {
                            if (lstrcmp(pe32.szExeFile, processName) == 0) { // Modified line
                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                                if (hProcess != NULL) {
                                    BOOL success = SetPriorityClass(hProcess, BELOW_NORMAL_PRIORITY_CLASS);
                                    if (success) {
                                        printf("Process priority lowered successfully.\n");
                                    }
                                    else {
                                        printf("Failed to lower process priority.\n");
                                    }
                                    CloseHandle(hProcess);
                                    break;
                                }
                            }
                        } while (Process32Next(hSnapshot, &pe32));
                    }
                    CloseHandle(hSnapshot);
                }
            }
            else if (choice == '2') {
                printf("Enter the name of the process you want to kill: ");
                char processName[256];
                scanf("%s", processName);
                HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (hSnapshot != INVALID_HANDLE_VALUE) {
                    PROCESSENTRY32 pe32;
                    pe32.dwSize = sizeof(PROCESSENTRY32);
                    if (Process32First(hSnapshot, &pe32)) {
                        do {
                            if (strcmp(pe32.szExeFile, processName) == 0) { //strcmp, lstrcmp, _tcscmp 모두 오류가 나는데 지피티한테 수정해달라고 하면 저 세 개를 반복해 보내줍니다...
                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                                if (hProcess != NULL) {
                                    BOOL success = TerminateProcess(hProcess, 0);
                                    if (success) {
                                        printf("Process '%s' terminated successfully.\n", processName);
                                    }
                                    else {
                                        printf("Failed to terminate process '%s'.\n", processName);
                                    }
                                    CloseHandle(hProcess);
                                    break;
                                }
                            }
                        } while (Process32Next(hSnapshot, &pe32));
                    }
                    CloseHandle(hSnapshot);
                }
            }
        }
    }
    return 0;
}

double getCPUPercentage() {
    FILE* file;
    double cpuPercent;
    unsigned long long totalTicks, idleTicks;
    SYSTEM_INFO sysInfo;
    FILETIME idleTime, kernelTime, userTime;

    GetSystemInfo(&sysInfo);
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    totalTicks = (sysInfo.dwNumberOfProcessors * (kernelTime.dwHighDateTime * (unsigned long long)MAXDWORD + kernelTime.dwLowDateTime) +
        sysInfo.dwNumberOfProcessors * (userTime.dwHighDateTime * (unsigned long long)MAXDWORD + userTime.dwLowDateTime));
    idleTicks = (idleTime.dwHighDateTime * (unsigned long long)MAXDWORD + idleTime.dwLowDateTime);

    cpuPercent = 100.0 - ((idleTicks * 100.0) / totalTicks);
    return cpuPercent;
}


DWORD WINAPI monitorCPU(LPVOID lpParam) {
    FILE* file = fopen("cpu_monitor.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    double threshold = DEFAULT_THRESHOLD;
    int consecutiveCount = 0;

    while (isMonitoring) {
        double cpuPercent = getCPUPercentage();
        printf("CPU Usage: %.2f%%\n", cpuPercent);
        fprintf(file, "CPU Usage: %.2f%%\n", cpuPercent);

        if (cpuPercent >= threshold) {
            consecutiveCount++;
            if (consecutiveCount >= DEFAULT_MAX_CONTINUOUS_DURATION / DEFAULT_INTERVAL) {
                printf("CPU usage exceeded the threshold for too long.\n");
                printf("Taking necessary action...\n");
                printf("The CPU usage has been consistently high. Consider checking for any resource-intensive processes.\n");
                consecutiveCount = 0;
            }
        }
        else {
            consecutiveCount = 0;
        }

        Sleep(DEFAULT_INTERVAL * 1000); // Sleep for interval in milliseconds
    }

    fclose(file);
    return 0;
}

int main() {
    HANDLE keyboardThread, cpuThread;

    keyboardThread = CreateThread(NULL, 0, monitorKeyboardInput, NULL, 0, NULL);
    if (keyboardThread == NULL) {
        fprintf(stderr, "Error - CreateThread() failed\n");
        exit(EXIT_FAILURE);
    }

    cpuThread = CreateThread(NULL, 0, monitorCPU, NULL, 0, NULL);
    if (cpuThread == NULL) {
        fprintf(stderr, "Error - CreateThread() failed\n");
        exit(EXIT_FAILURE);
    }

    WaitForSingleObject(keyboardThread, INFINITE);
    WaitForSingleObject(cpuThread, INFINITE);

    CloseHandle(keyboardThread);
    CloseHandle(cpuThread);

    return 0;
}
