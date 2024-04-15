#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

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
                // Implement process priority lowering in C
                printf("Process priority lowering not implemented in C.\n");
            }
            else if (choice == '2') {
                printf("Enter the name of the process you want to kill: ");
                // Implement process killing in C
                printf("Process killing not implemented in C.\n");
            }
        }
    }
    return 0;
}

DWORD WINAPI monitorCPU(LPVOID lpParam) {
    FILE* file = fopen("cpu_monitor.log", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (isMonitoring) {
        // Implement getting CPU percentage in C
        double cpuPercent = 0.0; // Placeholder value
        printf("CPU Usage: %.2f%%\n", cpuPercent);
        fprintf(file, "CPU Usage: %.2f%%\n", cpuPercent);

        // Implement CPU monitoring logic
        // You can refer to the logic in the original Python code and translate it to C

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
