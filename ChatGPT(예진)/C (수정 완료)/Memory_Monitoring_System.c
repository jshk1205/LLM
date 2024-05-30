#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif __linux__
#include <sys/sysinfo.h>
#else
#error "Platform not supported"
#endif

void print_memory_info() {
#ifdef _WIN32
    MEMORYSTATUSEX memory_status;
    memory_status.dwLength = sizeof(memory_status);
    GlobalMemoryStatusEx(&memory_status);
    printf("Total memory: %I64u bytes\n", memory_status.ullTotalPhys);
    printf("Used memory: %I64u bytes\n", memory_status.ullTotalPhys - memory_status.ullAvailPhys);
    printf("Memory usage: %.2f%%\n", ((double)(memory_status.ullTotalPhys - memory_status.ullAvailPhys) / memory_status.ullTotalPhys) * 100);
#elif __linux__
    struct sysinfo mem_info;
    sysinfo(&mem_info);
    printf("Total memory: %lu bytes\n", mem_info.totalram * mem_info.mem_unit);
    printf("Used memory: %lu bytes\n", (mem_info.totalram - mem_info.freeram) * mem_info.mem_unit);
    printf("Memory usage: %.2f%%\n", ((double)(mem_info.totalram - mem_info.freeram) / mem_info.totalram) * 100);
#endif
}

int main() {
    double interval = 1; // Monitoring interval in seconds
    int threshold = 90;  // Memory usage warning threshold (%)
    char option;

    while (1) {
        print_memory_info();

        // If a threshold is set and memory usage exceeds the threshold, print a warning message
        if (threshold != 0) {
#ifdef _WIN32
            MEMORYSTATUSEX memory_status;
            memory_status.dwLength = sizeof(memory_status);
            GlobalMemoryStatusEx(&memory_status);
            double memory_usage = ((double)(memory_status.ullTotalPhys - memory_status.ullAvailPhys) / memory_status.ullTotalPhys) * 100;
#elif __linux__
            struct sysinfo mem_info;
            sysinfo(&mem_info);
            double memory_usage = ((double)(mem_info.totalram - mem_info.freeram) / mem_info.totalram) * 100;
#endif

            if (memory_usage > threshold) {
                printf("Warning: Memory usage exceeds %d%%.\n", threshold);
            }
        }

        printf("Press 'q' to quit, or 'p' to continue: ");
        scanf(" %c", &option);
        if (option == 'q') {
            break;
        }
        else if (option != 'p') {
            printf("Please select a valid option.\n");
        }

        // Sleep for the specified interval
#ifdef _WIN32
        Sleep(interval * 1000); // Convert seconds to milliseconds
#elif __linux__
        sleep(interval);
#endif
    }

    return 0;
}
