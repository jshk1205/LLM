#include <stdio.h>
#include <stdlib.h>
 
typedef struct {
    int id;
    int arrivalTime; // 프로세스 도착 시간
    int burstTime; // ���μ��� ���࿡ �ʿ��� �ð�
    int priority; // ���μ��� �켱����
    int remainingBurstTime; // ���� ���� �ð�
} Process;

// FCFS �����ٸ�
void fcfsScheduling(Process* processes, int count) {
    int* waitingTime = (int*)malloc(sizeof(int) * count);
    int* turnAroundTime = (int*)malloc(sizeof(int) * count);

    waitingTime[0] = 0;
    turnAroundTime[0] = processes[0].burstTime;
    for (int i = 1; i < count; i++) {
        waitingTime[i] = turnAroundTime[i - 1];
        turnAroundTime[i] = waitingTime[i] + processes[i].burstTime;
    }

    printf("FCFS �����ٸ� ���:\n");
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
            processes[i].burstTime, waitingTime[i], turnAroundTime[i]);
    }

    free(waitingTime);
    free(turnAroundTime);
}

// SJF �����ٸ�
void sjfScheduling(Process* processes, int count) {
    int* waitingTime = (int*)malloc(sizeof(int) * count);
    int* turnAroundTime = (int*)malloc(sizeof(int) * count);

    // ���μ����� ���� �ð� �������� ������������ ����
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

    printf("\nSJF �����ٸ� ���:\n");
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime,
            processes[i].burstTime, waitingTime[i], turnAroundTime[i]);
    }

    free(waitingTime);
    free(turnAroundTime);
}

// ���� �κ� �����ٸ�
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

    printf("\n���� �κ� �����ٸ� ���:\n");
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
    }; // ���μ��� ����

    int count = sizeof(processes) / sizeof(processes[0]);
    int timeQuantum = 2;

    fcfsScheduling(processes, count);
    sjfScheduling(processes, count);
    roundRobinScheduling(processes, count, timeQuantum);

    return 0;
}