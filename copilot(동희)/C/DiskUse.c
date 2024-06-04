

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUFFER_SIZE 1024
#define CRT_SECURE_NO_WARNINGS

typedef struct
{
    char dirname[MAX_BUFFER_SIZE];
    int diskSize;
    int diskType;
} DiskMonitor;

DiskMonitor *createDiskMonitor()
{
    DiskMonitor *monitor = (DiskMonitor *)malloc(sizeof(DiskMonitor));
    if (monitor == NULL)
    {
        return NULL;
    }

    monitor->diskSize = 0;
    monitor->diskType = 0;

    return monitor;
}

void destroyDiskMonitor(DiskMonitor *monitor)
{
    if (monitor == NULL)
    {
        return;
    }

    free(monitor);
}

void getDiskSize(char *dirname, int *diskSize)
{
    struct stat st;
    if (stat(dirname, &st) == 0)
    {
        *diskSize = st.st_size;
    }
}

void getDiskType(char *dirname, int *diskType)
{
    struct stat st;
    if (stat(dirname, &st) == 0)
    {
        *diskType = st.st_mode;
    }
}

void getDiskUsage(char *dirname, int *diskSize, int *diskType)
{
    getDiskSize(dirname, diskSize);
    getDiskType(dirname, diskType);
}

void printDiskSize(int diskSize)
{
    printf("Disk Size: %d bytes\n", diskSize);
}

void printDiskType(int diskType)
{
    printf("Disk Type: %d\n", diskType);
}

int main()
{
    DiskMonitor *monitor = createDiskMonitor();
    if (monitor == NULL)
    {
        return 1;
    }

    strcpy(monitor->dirname, "C:\\");
    getDiskUsage(monitor->dirname, &monitor->diskSize, &monitor->diskType);
    printDiskSize(monitor->diskSize);
    printDiskType(monitor->diskType);

    destroyDiskMonitor(monitor);

    return 0;
}

// Path: copilot%28%EB%8F%99%ED%9D%AC%29/C/DiskUse.c