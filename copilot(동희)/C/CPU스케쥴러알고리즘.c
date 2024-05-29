#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrivalTime; // 프로세스 도착 시간
    int burstTime; // 프로세스 실행에 필요한 시간
    int priority; // 프로세스 우선순위
    int remainingBurstTime; // 남은 실행 시간
} Process;

// FCFS 스케줄링
void fcfsScheduling(Process* processes, int count) {
    int* waitingTime = (int*)malloc(sizeof(int) * count);
    int* turnAroundTime = (int*)malloc(sizeof(int) * count);

    waitingTime[0] = 0;
    turnAroundTime[0] = processes[0].burstTime;
    for (int i = 1; i < count; i++) {
        waitingTime[i] = turnAroundTime[i - 1];
        turnAroundTime[i] = waitingTime[i] + processes[i].burstTime;
    }

    printf("FCFS 스케줄링 결과:\n");
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
            processes[i].burstTime, waitingTime[i], turnAroundTime[i]);
    }

    free(waitingTime);
    free(turnAroundTime);
}

// SJF 스케줄링
void sjfScheduling(Process* processes, int count) {
    int* waitingTime = (int*)malloc(sizeof(int) * count);
    int* turnAroundTime = (int*)malloc(sizeof(int) * count);

    // 프로세스를 실행 시간 기준으로 오름차순으로 정렬
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (processes[j].burstTime > processes[j + 1].burstTime) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    waitingTime[0] = 0;
    turnAroundTime[0] = processes[0].burstTime;
    for (int i = 1; i < count; i++) {
        waitingTime[i] = turnAroundTime[i - 1];
        turnAroundTime[i] = waitingTime[i] + processes[i].burstTime;
    }

    printf("\nSJF 스케줄링 결과:\n");
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
            processes[i].burstTime, waitingTime[i], turnAroundTime[i]);
    }

    free(waitingTime);
    free(turnAroundTime);
}

// 라운드 로빈 스케줄링
void roundRobinScheduling(Process* processes, int count, int timeQuantum) {
    int* waitingTime = (int*)malloc(sizeof(int) * count);
    int* turnAroundTime = (int*)malloc(sizeof(int) * count);
    int* remainingBurstTime = (int*)malloc(sizeof(int) * count);

    for (int i = 0; i < count; i++) {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    int currentTime = 0, completed = 0;
    while (completed != count) {
        for (int i = 0; i < count; i++) {
            if (remainingBurstTime[i] > 0) {
                if (remainingBurstTime[i] > timeQuantum) {
                    currentTime += timeQuantum;
                    remainingBurstTime[i] -= timeQuantum;
                }
                else {
                    currentTime += remainingBurstTime[i];
                    waitingTime[i] = currentTime - processes[i].burstTime;
                    turnAroundTime[i] = currentTime;
                    remainingBurstTime[i] = 0;
                    completed++;
                }
            }
        }
    }

    printf("\n라운드 로빈 스케줄링 결과:\n");
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
            processes[i].burstTime, waitingTime[i], turnAroundTime[i]);
    }

    free(waitingTime);
    free(turnAroundTime);
    free(remainingBurstTime);
}

int main() {
    Process processes[] = {
        {1, 0, 6, 2}, {2, 2, 8, 3}, {3, 3, 7, 1}, {4, 4, 3, 2}, {5, 6, 4, 4}
    }; // 프로세스 예제

    int count = sizeof(processes) / sizeof(processes[0]);
    int timeQuantum = 2;

    fcfsScheduling(processes, count);
    sjfScheduling(processes, count);
    roundRobinScheduling(processes, count, timeQuantum);

    return 0;
}
