#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Process
{
public:
    Process(string name)
    {
        this->name = name;
        this->state = "대기";
        this->id = ++count;
        cout << "생성되었음" << endl;
    }
    ~Process()
    {
        cout << "삭제되었음" << endl;
    }
    void changeState(string state)
    {
        this->state = state;
        cout << "상태변경되었음" << endl;
    }
        void print()
        {
            cout << "프로세스ID: " << id << endl;
            cout << "프로세스이름: " << name << endl;
            cout << "프로세스상태: " << state << endl;
        }
        static int count;
        
    public:
        int id;
        string name;
        string state;
};

int Process::count = 0;

int main()
{
    vector<Process*> processList;
    while (true)
    {
        cout << "1. 프로세스 생성" << endl;
        cout << "2. 프로세스 삭제" << endl;
        cout << "3. 프로세스 상태변경" << endl;
        cout << "4. 프로세스 출력" << endl;
        cout << "5. 종료" << endl;
        int menu;
        cin >> menu;
        if (menu == 1)
        {
            string name;
            cout << "프로세스이름: ";
            cin >> name;
            Process* process = new Process(name);
            processList.push_back(process);
        }
        else if (menu == 2)
        {
            int id;
            cout << "프로세스ID: ";
            cin >> id;
            for (int i = 0; i < processList.size(); i++)
            {
                if (processList[i]->id == id) // Access the "id" member
                {
                    delete processList[i];
                    processList.erase(processList.begin() + i);
                    break;
                }
            }
        }
        else if (menu == 3)
        {
            int id;
            cout << "프로세스ID: ";
            cin >> id;
            for (int i = 0; i < processList.size(); i++)
            {
                if (processList[i]->id == id)
                {
                    string state;
                    cout << "상태: ";
                    cin >> state;
                    processList[i]->changeState(state);
                    break;
                }
            }
        }
        else if (menu == 4)
        {
            for (int i = 0; i < processList.size(); i++)
            {
                processList[i]->print();
            }
        }
        else if (menu == 5)
        {
            break;
        }
    }
}


