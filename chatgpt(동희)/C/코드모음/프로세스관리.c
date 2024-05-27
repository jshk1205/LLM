#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PROCESSES 10 // 최대 프로세스 수

// 프로세스 구조체 정의
typedef struct {
    int pid;
    char name[50];
    int priority;
    int status; // 0: 대기 중, 1: 실행 중
} Process;

// 전역 변수
Process processes[MAX_PROCESSES]; // 프로세스 배열
int processCount = 0; // 현재 프로세스 수

// 함수 원형
void createProcess();
void killProcess();
void listProcesses();
void setPriority();
void startProcess();
void stopProcess();

int main() {
    int choice;
    do {
        // 메뉴 출력
        printf("\nProcess Management System\n");
        printf("1. Create Process\n");
        printf("2. Kill Process\n");
        printf("3. List Processes\n");
        printf("4. Set Priority\n");
        printf("5. Start Process\n");
        printf("6. Stop Process\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf_s("%d", &choice);

        // 선택된 옵션에 따라 기능 실행
        switch (choice) {
        case 1:
            createProcess();
            break;
        case 2:
            killProcess();
            break;
        case 3:
            listProcesses();
            break;
        case 4:
            setPriority();
            break;
        case 5:
            startProcess();
            break;
        case 6:
            stopProcess();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}

// 새로운 프로세스 생성
void createProcess() {
    if (processCount < MAX_PROCESSES) {
        printf("Enter process name: ");
        scanf_s("%s", processes[processCount].name, sizeof(processes[processCount].name));
        processes[processCount].pid = processCount + 1; // 프로세스 ID는 1부터 시작
        processes[processCount].status = 0; // 대기 상태로 초기화
        printf("Process created with PID: %d\n", processes[processCount].pid);
        processCount++;
    }
    else {
        printf("Cannot create more processes. Maximum limit reached.\n");
    }
}

// 프로세스 종료
void killProcess() {
    int pid;
    printf("Enter PID of the process to kill: ");
    scanf_s("%d", &pid);
    int index = -1;
    for (int i = 0; i < processCount; i++) {
        if (processes[i].pid == pid) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index; i < processCount - 1; i++) {
            processes[i] = processes[i + 1]; // 배열에서 해당 인덱스의 프로세스를 삭제
        }
        processCount--;
        printf("Process with PID %d killed.\n", pid);
    }
    else {
        printf("Process with PID %d not found.\n", pid);
    }
}

// 현재 실행 중인 모든 프로세스 목록 출력
void listProcesses() {
    printf("\nList of Processes:\n");
    printf("PID\tName\tPriority\tStatus\n");
    for (int i = 0; i < processCount; i++) {
        printf("%d\t%s\t%d\t\t%s\n", processes[i].pid, processes[i].name, processes[i].priority, processes[i].status == 0 ? "Waiting" : "Running");
    }
}

// 프로세스의 우선순위 설정
void setPriority() {
    int pid, priority;
    printf("Enter PID of the process: ");
    scanf_s("%d", &pid);
    printf("Enter new priority for the process: ");
    scanf_s("%d", &priority);
    for (int i = 0; i < processCount; i++) {
        if (processes[i].pid == pid) {
            processes[i].priority = priority;
            printf("Priority of process with PID %d set to %d.\n", pid, priority);
            return;
        }
    }
    printf("Process with PID %d not found.\n", pid);
}

// 프로세스 시작
void startProcess() {
    int pid;
    printf("Enter PID of the process to start: ");
    scanf_s("%d", &pid);
    for (int i = 0; i < processCount; i++) {
        if (processes[i].pid == pid) {
            processes[i].status = 1; // 실행 상태로 변경
            printf("Process with PID %d started.\n", pid);
            return;
        }
    }
    printf("Process with PID %d not found.\n", pid);
}

// 프로세스 정지
void stopProcess() {
    int pid;
    printf("Enter PID of the process to stop: ");
    scanf_s("%d", &pid);
    for (int i = 0; i < processCount; i++) {
        if (processes[i].pid == pid) {
            processes[i].status = 0; // 대기 상태로 변경
            printf("Process with PID %d stopped.\n", pid);
            return;
        }
    }
    printf("Process with PID %d not found.\n", pid);
}
