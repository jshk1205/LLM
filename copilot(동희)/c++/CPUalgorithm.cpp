//CPU스케줄러알고리즘을 만들려고해요
//비선점, 선점, 우선순위, 라운드로빈, SJF, SRTF, FCFS, HRRN을 만들려고해요
//우선순위는 1이 가장높고 10이 가장낮아요
//라운드로빈은 1초입니다
//프로세스는 최대 10개입니다
//프로세스는 이름, 도착시간, 실행시간, 우선순위를 가지고있어요
//프로세스는 이름은 1글자입니다
//프로세스는 도착시간은 0초이상입니다
//프로세스는 실행시간은 1초이상입니다
//프로세스는 우선순위는 1이상 10이하입니다
//프로세스는 이름은 중복되지않아요
//프로세스는 도착시간은 중복되지않아요
//프로세스는 실행시간은 중복되지않아요
//프로세스는 우선순위는 중복되지않아요
//프로세스는 도착시간이 빠른순으로 정렬되어있어요
//스케줄러알고리즘을 선택해서 알고리즘을 확인하는 방법으로 할래
//스케줄러알고리즘을 선택하면 프로세스들의 이름, 도착시간, 실행시간, 우선순위, 대기시간, 반환시간, Gantt Chart, 평균 대기시간, 평균 반환시간을 출력할래

//1. 비선점
//2. 선점
//3. 우선순위
//4. 라운드로빈
//5. SJF
//6. SRTF
//7. FCFS
//8. HRRN
//9. 종료

//예시로 입력하는 방법
//1
//프로세스의 개수를 입력하세요: 3
//프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: A 0 3 1
//프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: B 1 2 2
//프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: C 2 1 3
//프로세스들의 이름, 도착시간, 실행시간, 우선순위, 대기시간, 반환시간, Gantt Chart, 평균 대기시간, 평균 반환시간
//A 0 3 1 0 3 A A A 3.0 3.0
//B 1 2 2 2 4 B B 4.0 4.0
//C 2 1 3 3 4 C 4.0 4.0
//Gantt Chart: A A A B B C
//평균 대기시간: 3.0
//평균 반환시간: 3.66667


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;

class Process
{
public:
    char name;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    Process(char name, int arrivalTime, int burstTime, int priority)
    {
        this->name = name;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->priority = priority;
        this->waitingTime = 0;
        this->turnaroundTime = 0;
    }
};

bool compareArrivalTime(Process a, Process b)
{
    return a.arrivalTime < b.arrivalTime;
}

bool comparePriority(Process a, Process b)
{
    return a.priority < b.priority;
}

bool compareBurstTime(Process a, Process b)
{
    return a.burstTime < b.burstTime;
}

bool compareName(Process a, Process b)
{
    return a.name < b.name;
}

void printProcess(vector<Process> processes)
{
    cout << "프로세스들의 이름, 도착시간, 실행시간, 우선순위, 대기시간, 반환시간, Gantt Chart, 평균 대기시간, 평균 반환시간" << endl;
    for (int i = 0; i < processes.size(); i++)
    {
        cout << processes[i].name << " " << processes[i].arrivalTime << " " << processes[i].burstTime << " " << processes[i].priority << " " << processes[i].waitingTime << " " << processes[i].turnaroundTime << " ";
        if (i == processes.size() - 1)
        {
            cout << processes[i].name;
        }
        else
        {
            cout << processes[i].name << " ";
        }
    }
    cout << endl;
}

void printGanttChart(vector<Process> processes)
{
    cout << "Gantt Chart: ";
    for (int i = 0; i < processes.size(); i++)
    {
        cout << processes[i].name << " ";
    }
    cout << endl;
}

void printAverageTime(vector<Process> processes)
{
    float sumWaitingTime = 0;
    float sumTurnaroundTime = 0;
    for (int i = 0; i < processes.size(); i++)
    {
        sumWaitingTime += processes[i].waitingTime;
        sumTurnaroundTime += processes[i].turnaroundTime;
    }
    cout << "평균 대기시간: " << sumWaitingTime / processes.size() << endl;
    cout << "평균 반환시간: " << sumTurnaroundTime / processes.size() << endl;
}


void nonPreemptive(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    vector<Process> result;
    int currentTime = 0;
    while (!processes.empty())
    {
        Process currentProcess = processes[0];
        processes.erase(processes.begin());
        currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
        currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
        currentTime += currentProcess.burstTime;
        result.push_back(currentProcess);
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}


void preemptive(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    vector<Process> result;
    int currentTime = 0;
    while (!processes.empty())
    {
        Process currentProcess = processes[0];
        processes.erase(processes.begin());
        currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
        currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
        currentTime += currentProcess.burstTime;
        result.push_back(currentProcess);
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}

void priority(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), comparePriority);
    vector<Process> result;
    int currentTime = 0;
    while (!processes.empty())
    {
        Process currentProcess = processes[0];
        processes.erase(processes.begin());
        currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
        currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
        currentTime += currentProcess.burstTime;
        result.push_back(currentProcess);
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}

void roundRobin(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    vector<Process> result;
    int currentTime = 0;
    queue<Process> q;
    while (!processes.empty() || !q.empty())
    {
        if (!processes.empty())
        {
            if (processes[0].arrivalTime <= currentTime)
            {
                q.push(processes[0]);
                processes.erase(processes.begin());
            }
        }
        if (!q.empty())
        {
            Process currentProcess = q.front();
            q.pop();
            currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
            if (currentProcess.burstTime > 1)
            {
                currentTime += 1;
                currentProcess.burstTime -= 1;
                q.push(currentProcess);
            }
            else
            {
                currentTime += currentProcess.burstTime;
                currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
                result.push_back(currentProcess);
            }
        }
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}

void SJF(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareBurstTime);
    vector<Process> result;
    int currentTime = 0;
    while (!processes.empty())
    {
        Process currentProcess = processes[0];
        processes.erase(processes.begin());
        currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
        currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
        currentTime += currentProcess.burstTime;
        result.push_back(currentProcess);
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}

void SRTF(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    vector<Process> result;
    int currentTime = 0;
    queue<Process> q;
    while (!processes.empty() || !q.empty())
    {
        if (!processes.empty())
        {
            if (processes[0].arrivalTime <= currentTime)
            {
                q.push(processes[0]);
                processes.erase(processes.begin());
            }
        }
        if (!q.empty())
        {
            sort(processes.begin(), processes.end(), compareBurstTime);
            Process currentProcess = q.front();
            q.pop();
            currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
            if (currentProcess.burstTime > 1)
            {
                currentTime += 1;
                currentProcess.burstTime -= 1;
                q.push(currentProcess);
            }
            else
            {
                currentTime += currentProcess.burstTime;
                currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
                result.push_back(currentProcess);
            }
        }
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}

void FCFS(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    vector<Process> result;
    int currentTime = 0;
    while (!processes.empty())
    {
        Process currentProcess = processes[0];
        processes.erase(processes.begin());
        currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
        currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
        currentTime += currentProcess.burstTime;
        result.push_back(currentProcess);
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}

void HRRN(vector<Process> processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);
    vector<Process> result;
    int currentTime = 0;
    while (!processes.empty())
    {
        Process currentProcess = processes[0];
        processes.erase(processes.begin());
        currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
        currentProcess.turnaroundTime = currentProcess.waitingTime + currentProcess.burstTime;
        currentTime += currentProcess.burstTime;
        result.push_back(currentProcess);
    }
    printProcess(result);
    printGanttChart(result);
    printAverageTime(result);
}


int main()
{
    while (true)
    {
        cout << "1. 비선점" << endl;
        cout << "2. 선점" << endl;
        cout << "3. 우선순위" << endl;
        cout << "4. 라운드로빈" << endl;
        cout << "5. SJF" << endl;
        cout << "6. SRTF" << endl;
        cout << "7. FCFS" << endl;
        cout << "8. HRRN" << endl;
        cout << "9. 종료" << endl;
        int menu;
        cin >> menu;
        if (menu == 1)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            nonPreemptive(processes);
        }
        else if (menu == 2)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            preemptive(processes);
        }
        else if (menu == 3)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            priority(processes);
        }
        else if (menu == 4)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            roundRobin(processes);
        }
        else if (menu == 5)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            SJF(processes);
        }
        else if (menu == 6)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            SRTF(processes);
        }
        else if (menu == 7)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector
            <Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            FCFS(processes);
        }
        else if (menu == 8)
        {
            int n;
            cout << "프로세스의 개수를 입력하세요: ";
            cin >> n;
            vector<Process> processes;
            for (int i = 0; i < n; i++)
            {
                char name;
                int arrivalTime;
                int burstTime;
                int priority;
                cout << "프로세스의 이름, 도착시간, 실행시간, 우선순위를 입력하세요: ";
                cin >> name >> arrivalTime >> burstTime >> priority;
                processes.push_back(Process(name, arrivalTime, burstTime, priority));
            }
            HRRN(processes);
        }
        else if (menu == 9)
        {
            break;
        }
    }
    return 0;
}
// Compare this snippet from copilot%28%EB%8F%99%ED%9D%AC%29/c%2B%2B/BinaryEditor.cpp: