//로그파일을 열어서 로그를 분석하는 프로그램을 작성하시오.
//로그파일의 각 줄은 다음과 같이 구성되어 있다.
//로그시간 로그내용
//로그시간은 24시간제 시:분:초 형식이다.
//로그내용은 공백으로 구분된 단어들로 구성되어 있다.
//로그파일을 읽어서 다음과 같은 정보를 출력하시오.

//1. 로그파일에 포함된 로그의 개수
//2. 로그파일에 포함된 로그의 시간대별 개수
//3. 로그파일에 포함된 로그의 시간대별 개수를 시간순으로 정렬하여 출력
//4. 로그파일에 포함된 로그의 단어별 개수
//5. 로그파일에 포함된 로그의 단어별 개수를 단어순으로 정렬하여 출력
//6. 로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력
//7. 로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력하는데, 개수가 같은 경우에는 단어순으로 정렬하여 출력
//c언어로 만들어야합니다.
//로그파일은 log.txt로 제공됩니다.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LOG 1000
#define MAX_WORD 1000
#define MAX_TIME 1000

typedef struct {
    char time[9];
    char content[100];
} Log;

typedef struct {
    char word[100];
    int count;
} Word;

int compare(const void* a, const void* b) {
    Word* w1 = (Word*)a;
    Word* w2 = (Word*)b;
    if (w1->count == w2->count) {
        return strcmp(w1->word, w2->word);
    }
    return w2->count - w1->count;
}

int main() {
    FILE* file = fopen("log.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다\n");
        return 0;
    }

    Log logs[MAX_LOG];
    Word words[MAX_WORD];
    int logCount = 0;
    int wordCount = 0;
    int timeCount[24] = { 0 };
    while (fscanf(file, "%s %s", logs[logCount].time, logs[logCount].content) != EOF) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        int hour = (logs[logCount].time[0] - '0') * 10 + logs[logCount].time[1] - '0';
        timeCount[hour]++;
        char* token = strtok(logs[logCount].content, " ");
        while (token != NULL) {
            int found = 0;
            for (int i = 0; i < wordCount; i++) {
                if (strcmp(words[i].word, token) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(words[wordCount].word, token);
                words[wordCount].count = 1;
                wordCount++;
            }
            token = strtok(NULL, " ");
        }
        logCount++;
    }

    printf("로그파일에 포함된 로그의 개수: %d\n", logCount);
    printf("로그파일에 포함된 로그의 시간대별 개수:\n");
    for (int i = 0; i < 24; i++) {
        printf("%02d시: %d\n", i, timeCount[i]);
    }
    printf("로그파일에 포함된 로그의 시간대별 개수를 시간순으로 정렬하여 출력:\n");
    for (int i = 0; i < 24; i++) {
        printf("%02d시: %d\n", i, timeCount[i]);
    }
    printf("로그파일에 포함된 로그의 단어별 개수:\n");
    for (int i = 0; i < wordCount; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    printf("로그파일에 포함된 로그의 단어별 개수를 단어순으로 정렬하여 출력:\n");
    qsort(words, wordCount, sizeof(Word), compare);
    for (int i = 0; i < wordCount; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    printf("로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력:\n");
    for (int i = 0; i < wordCount; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    printf("로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력하는데, 개수가 같은 경우에는 단어순으로 정렬하여 출력:\n");
    qsort(words, wordCount, sizeof(Word), compare);
    for (int i = 0; i < wordCount; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }

    fclose(file);
    return 0;
}

