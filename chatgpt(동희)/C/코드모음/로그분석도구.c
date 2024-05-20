#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_ENTRIES 1000
#define MAX_LINE_LENGTH 100

// 로그 엔트리 구조체
typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

// 로그 분석 함수
void analyzeLog(LogEntry logEntries[], int numEntries) {
    // 로그 레벨별로 발생한 횟수를 저장할 변수 초기화
    int numErrors = 0;
    int numWarnings = 0;
    int numInfos = 0;

    // 각 로그 엔트리를 분석하여 로그 레벨별로 발생한 횟수 증가
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

    // 분석 결과 출력
    printf("로그 분석 결과:\n");
    printf("Errors: %d\n", numErrors);
    printf("Warnings: %d\n", numWarnings);
    printf("Infos: %d\n", numInfos);
}

int main() {
    FILE* file = NULL;
    char filename[50] = { 0 }; // 문자열 초기화
    char line[MAX_LINE_LENGTH];
    LogEntry logEntries[MAX_LOG_ENTRIES] = { {0} }; // 구조체 배열 초기화
    int numEntries = 0;

    printf("로그 파일 이름을 입력하세요: ");
    if (scanf("%s", filename) != 1) { // scanf 함수 사용
        printf("입력 오류입니다.\n");
        return 1;
    }

    if (fopen_s(&file, filename, "r") != 0) { // fopen_s 함수 사용
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    // 로그 파일에서 데이터 읽기
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && numEntries < MAX_LOG_ENTRIES) {
        sscanf(line, "%s %s %[^\n]", logEntries[numEntries].timestamp, logEntries[numEntries].level, logEntries[numEntries].message); // sscanf 함수 사용
        numEntries++;
    }

    fclose(file);

    // 로그 분석
    analyzeLog(logEntries, numEntries);

    return 0;
}
