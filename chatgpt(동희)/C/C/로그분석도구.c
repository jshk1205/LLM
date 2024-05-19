#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CATEGORIES 10

typedef struct {
    char category[50];
    int count;
} LogCategory;

void filterLogs(char* filename, char* keyword) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, keyword) != NULL) {
            printf("%s", line);
        }
    }

    fclose(file);
}

void classifyLogs(char* filename) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    LogCategory categories[MAX_CATEGORIES];
    int numCategories = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Assume each line contains a log with format: "category: message"
        char* category = strtok(line, ":");
        int found = 0;
        for (int i = 0; i < numCategories; i++) {
            if (strcmp(categories[i].category, category) == 0) {
                categories[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy_s(categories[numCategories].category, sizeof(categories[numCategories].category), category);
            categories[numCategories].count = 1;
            numCategories++;
        }
    }

    fclose(file);

    // Print category counts
    printf("Log Category Counts:\n");
    for (int i = 0; i < numCategories; i++) {
        printf("%s: %d\n", categories[i].category, categories[i].count);
    }
}

void analyzeLogs(char* filename) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int numEvents = 0;
    int totalTime = 0;
    int maxTime = 0;
    int minTime = INT_MAX;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Assume each line contains a log with format: "timestamp: message"
        char* timeStr = strtok(line, ":");
        int time = atoi(timeStr);
        numEvents++;
        totalTime += time;
        if (time > maxTime) {
            maxTime = time;
        }
        if (time < minTime) {
            minTime = time;
        }
    }

    fclose(file);

    printf("Total Events: %d\n", numEvents);
    printf("Average Event Time: %.2f\n", (float)totalTime / numEvents);
    printf("Max Event Time: %d\n", maxTime);
    printf("Min Event Time: %d\n", minTime);
}

int main() {
    char filename[] = "logfile.txt";
    char keyword[] = "ERROR";

    printf("Filtered Logs:\n");
    filterLogs(filename, keyword);

    printf("\n");

    printf("Classified Logs:\n");
    classifyLogs(filename);

    printf("\n");

    printf("Log Analysis:\n");
    analyzeLogs(filename);

    return 0;
}
