#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

// 프로세스 구조체
struct Process {
    int id;          // 프로세스 ID
    string name;     // 프로세스 이름
    int priority;    // 프로세스 우선순위
    bool isActive;   // 프로세스 활성화 여부
};

// 프로세스 생성 함수
// 매개변수: name - string, 프로세스 이름
// 반환값: Process - 생성된 프로세스
Process createProcess(const string& name) {
    static int idCounter = 1;
    Process newProcess;
    newProcess.id = idCounter++;
    newProcess.name = name;
    newProcess.priority = rand() % 10; // 임의의 우선순위 설정
    newProcess.isActive = true;
    return newProcess;
}

// 프로세스를 실행하는 함수
// 매개변수: process - Process&, 실행할 프로세스
void runProcess(Process& process) {
    cout << "프로세스 " << process.name << " 실행 중..." << endl;
    cout << "프로세스 이름: " << process.name << ", 실행 중: true" << endl;
    this_thread::sleep_for(chrono::seconds(2)); // 2초간 프로세스를 실행한다고 가정
    cout << "프로세스 " << process.name << " 실행 완료." << endl;
}

// 프로세스를 종료하는 함수
// 매개변수: process - Process&, 종료할 프로세스
void killProcess(Process& process) {
    cout << "프로세스 " << process.name << " 종료됨." << endl;
    process.isActive = false;
}

// 프로세스 정보를 출력하는 함수
// 매개변수: process - const Process&, 출력할 프로세스
void displayProcessInfo(const Process& process) {
    cout << "프로세스 ID: " << process.id << ", 이름: " << process.name << ", 우선순위: " << process.priority;
    if (process.isActive)
        cout << ", 활성화 상태: 활성화" << endl;
    else
        cout << ", 활성화 상태: 비활성화" << endl;
}

// 활성화된 프로세스 목록을 출력하는 함수
// 매개변수: processes - const vector<Process>&, 출력할 프로세스 목록
void displayActiveProcesses(const vector<Process>& processes) {
    cout << "활성화된 프로세스 목록:" << endl;
    for (const auto& process : processes) {
        if (process.isActive)
            displayProcessInfo(process);
    }
}

int main() {
    srand(time(0)); // 난수 발생기 초기화

    vector<Process> processes;

    // 예시로 프로세스 몇 개 생성하여 추가
    processes.push_back(createProcess("Process1"));
    processes.push_back(createProcess("Process2"));
    processes.push_back(createProcess("Process3"));

    // 생성된 프로세스 목록 출력
    displayActiveProcesses(processes);

    // 생성된 프로세스 중 하나를 실행하고 종료
    runProcess(processes[1]);
    killProcess(processes[1]);

    // 업데이트된 프로세스 목록 출력
    displayActiveProcesses(processes);

    return 0;
}
