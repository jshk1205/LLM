//프로세스 시뮬레이션 프로그램을 C언어로 만들어줘
// 프로세스의 도착 시간, 실행 시간, 종료 시간, 대기 시간, 반환 시간을 출력해줘
// 프로세스의 개수는 5개로 가정
// 프로세스의 도착 시간은 0~10 사이의 랜덤한 값으로 설정
// 프로세스의 실행 시간은 1~10 사이의 랜덤한 값으로 설정
// 프로세스의 종료 시간은 실행 시간 + 도착 시간
// 프로세스의 대기 시간은 실행 시간이 0인 경우 0, 아닌 경우 실행 시간 - 도착 시간
// 프로세스의 반환 시간은 종료 시간 - 도착 시간
// 프로세스의 우선순위는 도착 시간이 빠른 순서대로 실행
// 프로세스의 이름이 나와야됨
// FCFS, SJF 등의 여러가지 기법을 활용해야됨

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PROCESS 5
#define MAX_NAME 20

typedef struct Process
{
    char name[MAX_NAME];
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
} Process;

void printProcess(Process *process)
{
    printf("프로세스 이름: %s\n", process->name);
    printf("도착 시간: %d\n", process->arrivalTime);
    printf("실행 시간: %d\n", process->burstTime);
    printf("종료 시간: %d\n", process->completionTime);
    printf("대기 시간: %d\n", process->waitingTime);
    printf("반환 시간: %d\n", process->turnaroundTime);
}

void FCFS(Process *processes)
{
    int i;
    int currentTime = 0;
    for (i = 0; i < MAX_PROCESS; i++)
    {
        processes[i].completionTime = currentTime + processes[i].burstTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        currentTime = processes[i].completionTime;
    }
}

void SJF(Process *processes)
{
    int i, j;
    Process temp;
    for (i = 0; i < MAX_PROCESS - 1; i++)
    {
        for (j = i + 1; j < MAX_PROCESS; j++)
        {
            if (processes[i].burstTime > processes[j].burstTime)
            {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    FCFS(processes);
}

int main()
{
    Process processes[MAX_PROCESS];
    char names[MAX_PROCESS][MAX_NAME] = {"P1", "P2", "P3", "P4", "P5"};
    int i;
    srand(time(NULL));
    for (i = 0; i < MAX_PROCESS; i++)
    {
        strcpy(processes[i].name, names[i]);
        processes[i].arrivalTime = rand() % 11;
        processes[i].burstTime = rand() % 10 + 1;
    }
    printf("FCFS\n");
    FCFS(processes);
    for (i = 0; i < MAX_PROCESS; i++)
    {
        printProcess(&processes[i]);
    }
    printf("\n");
    printf("SJF\n");
    SJF(processes);
    for (i = 0; i < MAX_PROCESS; i++)
    {
        printProcess(&processes[i]);
    }
    return 0;
}
