#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

// ���μ��� ����ü
struct Process {
    int id;          // ���μ��� ID
    string name;     // ���μ��� �̸�
    int priority;    // ���μ��� �켱����
    bool isActive;   // ���μ��� Ȱ��ȭ ����
};

// ���μ��� ���� �Լ�
// �Ű�����: name - string, ���μ��� �̸�
// ��ȯ��: Process - ������ ���μ���
Process createProcess(const string& name) {
    static int idCounter = 1;
    Process newProcess;
    newProcess.id = idCounter++;
    newProcess.name = name;
    newProcess.priority = rand() % 10; // ������ �켱���� ����
    newProcess.isActive = true;
    return newProcess;
}

// ���μ����� �����ϴ� �Լ�
// �Ű�����: process - Process&, ������ ���μ���
void runProcess(Process& process) {
    cout << "���μ��� " << process.name << " ���� ��..." << endl;
    cout << "���μ��� �̸�: " << process.name << ", ���� ��: true" << endl;
    this_thread::sleep_for(chrono::seconds(2)); // 2�ʰ� ���μ����� �����Ѵٰ� ����
    cout << "���μ��� " << process.name << " ���� �Ϸ�." << endl;
}

// ���μ����� �����ϴ� �Լ�
// �Ű�����: process - Process&, ������ ���μ���
void killProcess(Process& process) {
    cout << "���μ��� " << process.name << " �����." << endl;
    process.isActive = false;
}

// ���μ��� ������ ����ϴ� �Լ�
// �Ű�����: process - const Process&, ����� ���μ���
void displayProcessInfo(const Process& process) {
    cout << "���μ��� ID: " << process.id << ", �̸�: " << process.name << ", �켱����: " << process.priority;
    if (process.isActive)
        cout << ", Ȱ��ȭ ����: Ȱ��ȭ" << endl;
    else
        cout << ", Ȱ��ȭ ����: ��Ȱ��ȭ" << endl;
}

// Ȱ��ȭ�� ���μ��� ����� ����ϴ� �Լ�
// �Ű�����: processes - const vector<Process>&, ����� ���μ��� ���
void displayActiveProcesses(const vector<Process>& processes) {
    cout << "Ȱ��ȭ�� ���μ��� ���:" << endl;
    for (const auto& process : processes) {
        if (process.isActive)
            displayProcessInfo(process);
    }
}

int main() {
    srand(time(0)); // ���� �߻��� �ʱ�ȭ

    vector<Process> processes;

    // ���÷� ���μ��� �� �� �����Ͽ� �߰�
    processes.push_back(createProcess("Process1"));
    processes.push_back(createProcess("Process2"));
    processes.push_back(createProcess("Process3"));

    // ������ ���μ��� ��� ���
    displayActiveProcesses(processes);

    // ������ ���μ��� �� �ϳ��� �����ϰ� ����
    runProcess(processes[1]);
    killProcess(processes[1]);

    // ������Ʈ�� ���μ��� ��� ���
    displayActiveProcesses(processes);

    return 0;
}
