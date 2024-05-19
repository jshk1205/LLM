#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_ENTRIES 1000
#define MAX_LINE_LENGTH 100

// �α� ��Ʈ�� ����ü
typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

// �α� �м� �Լ�
void analyzeLog(LogEntry logEntries[], int numEntries) {
    // �α� �������� �߻��� Ƚ���� ������ ���� �ʱ�ȭ
    int numErrors = 0;
    int numWarnings = 0;
    int numInfos = 0;

    // �� �α� ��Ʈ���� �м��Ͽ� �α� �������� �߻��� Ƚ�� ����
    for (int i = 0; i < numEntries; i++) {
        if (strcmp(logEntries[i].level, "ERROR") == 0) {
            numErrors++;
        }
        else if (strcmp(logEntries[i].level, "WARNING") == 0) {
            numWarnings++;
        }
        else if (strcmp(logEntries[i].level, "INFO") == 0) {
            numInfos++;
        }
    }

    // �м� ��� ���
    printf("�α� �м� ���:\n");
    printf("Errors: %d\n", numErrors);
    printf("Warnings: %d\n", numWarnings);
    printf("Infos: %d\n", numInfos);
}

int main() {
    FILE* file = NULL;
    char filename[50] = { 0 }; // ���ڿ� �ʱ�ȭ
    char line[MAX_LINE_LENGTH];
    LogEntry logEntries[MAX_LOG_ENTRIES] = { {0} }; // ����ü �迭 �ʱ�ȭ
    int numEntries = 0;

    printf("�α� ���� �̸��� �Է��ϼ���: ");
    if (scanf("%s", filename) != 1) { // scanf �Լ� ���
        printf("�Է� �����Դϴ�.\n");
        return 1;
    }

    if (fopen_s(&file, filename, "r") != 0) { // fopen_s �Լ� ���
        printf("������ �� �� �����ϴ�.\n");
        return 1;
    }

    // �α� ���Ͽ��� ������ �б�
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && numEntries < MAX_LOG_ENTRIES) {
        sscanf(line, "%s %s %[^\n]", logEntries[numEntries].timestamp, logEntries[numEntries].level, logEntries[numEntries].message); // sscanf �Լ� ���
        numEntries++;
    }

    fclose(file);

    // �α� �м�
    analyzeLog(logEntries, numEntries);

    return 0;
}
