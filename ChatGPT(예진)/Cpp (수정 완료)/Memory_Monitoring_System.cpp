#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <ctime>

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
    std::cout << "Total memory: " << memory_status.ullTotalPhys << " bytes" << std::endl;
    std::cout << "Used memory: " << memory_status.ullTotalPhys - memory_status.ullAvailPhys << " bytes" << std::endl;
    std::cout << "Memory usage: " << ((double)(memory_status.ullTotalPhys - memory_status.ullAvailPhys) / memory_status.ullTotalPhys) * 100 << "%" << std::endl;
#elif __linux__
    struct sysinfo mem_info;
    sysinfo(&mem_info);
    std::cout << "Total memory: " << mem_info.totalram * mem_info.mem_unit << " bytes" << std::endl;
    std::cout << "Used memory: " << (mem_info.totalram - mem_info.freeram) * mem_info.mem_unit << " bytes" << std::endl;
    std::cout << "Memory usage: " << ((double)(mem_info.totalram - mem_info.freeram) / mem_info.totalram) * 100 << "%" << std::endl;
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
                std::cout << "Warning: Memory usage exceeds " << threshold << "%." << std::endl;
            }
        }

        std::cout << "Press 'q' to quit, or 'p' to continue: ";
        std::cin >> option;
        if (option == 'q') {
            break;
        }
        else if (option != 'p') {
            std::cout << "Please select a valid option." << std::endl;
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
