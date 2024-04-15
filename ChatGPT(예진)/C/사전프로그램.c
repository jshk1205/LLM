#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define DEFAULT_THRESHOLD 90
#define DEFAULT_DURATION 10
#define DEFAULT_MAX_CONTINUOUS_DURATION 30
#define DEFAULT_MAX_CONSECUTIVE_THRESHOLD 95
#define DEFAULT_INTERVAL 5

volatile sig_atomic_t isMonitoring = 1;

void* monitorKeyboardInput(void* args) {
    while (true) {
        printf("---Press 'p' to stop monitoring---\n");
        char key;
        scanf(" %c", &key);
        if (key == 'p') {
            isMonitoring = 0;
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
    pthread_exit(NULL);
}

void* monitorCPU(void* args) {
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

        sleep(DEFAULT_INTERVAL);
    }

    fclose(file);
    pthread_exit(NULL);
}

int main() {
    pthread_t keyboardThread, cpuThread;
    int keyboardThreadStatus, cpuThreadStatus;

    keyboardThreadStatus = pthread_create(&keyboardThread, NULL, monitorKeyboardInput, NULL);
    if (keyboardThreadStatus) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", keyboardThreadStatus);
        exit(EXIT_FAILURE);
    }

    cpuThreadStatus = pthread_create(&cpuThread, NULL, monitorCPU, NULL);
    if (cpuThreadStatus) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", cpuThreadStatus);
        exit(EXIT_FAILURE);
    }

    pthread_join(keyboardThread, NULL);
    pthread_join(cpuThread, NULL);

    return 0;
}
