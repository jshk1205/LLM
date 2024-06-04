#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>
#include <stdbool.h>

// 실행은 되도록 수정했으나 값이 첫 번째 출력한 것에서 변경하지 않는 오류 발생

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
                printf("Enter the ID of the process you want to lower priority for: ");
                DWORD processID;
                scanf("%lu", &processID);
                
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
                if (hProcess != NULL) {
                    BOOL success = SetPriorityClass(hProcess, BELOW_NORMAL_PRIORITY_CLASS);
                    if (success) {
                        printf("Process priority lowered successfully.\n");
                    } else {
                        printf("Failed to lower process priority.\n");
                    }
                    CloseHandle(hProcess);
                } else {
                    printf("Failed to open process with ID %lu.\n", processID);
                }
            }
            else if (choice == '2') {
                printf("Enter the ID of the process you want to kill: ");
                DWORD processID;
                scanf("%lu", &processID);
                
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
                if (hProcess != NULL) {
                    BOOL success = TerminateProcess(hProcess, 0);
                    if (success) {
                        printf("Process terminated successfully.\n");
                    } else {
                        printf("Failed to terminate process.\n");
                    }
                    CloseHandle(hProcess);
                } else {
                    printf("Failed to open process with ID %lu.\n", processID);
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
