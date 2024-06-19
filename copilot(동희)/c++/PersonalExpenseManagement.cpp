//개인 지출 관리 애플리케이션을 만들어줘
// C++ 언어로 만들것
//1. 지출 내역을 입력받아서 저장할 수 있어야 해
//2. 지출 내역을 삭제할 수 있어야 해
//3. 지출 내역을 조회할 수 있어야 해
//4. 지출 내역을 수정할 수 있어야 해
//5. 총 지출 금액을 확인할 수 있어야 해
//6. 지출 내역을 파일로 저장하고 불러올 수 있어야 해
//7. 지출 내역을 날짜별로 정렬할 수 있어야 해
//8. 지출 내역을 카테고리별로 정렬할 수 있어야 해


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace std;

class Expense
{
public:
    string date;
    string category;
    string content;
    int price;
};

vector<Expense> expenses;

void addExpense();
void deleteExpense();
void showExpense();
void editExpense();
void showTotalPrice();
void saveExpense();
void loadExpense();
void sortExpenseByDate();
void sortExpenseByCategory();

int main()
{
    int menu;
    loadExpense();
    while (1)
    {
        system("cls");
        cout << "1. 지출 내역 추가" << endl;
        cout << "2. 지출 내역 삭제" << endl;
        cout << "3. 지출 내역 조회" << endl;
        cout << "4. 지출 내역 수정" << endl;
        cout << "5. 총 지출 금액 확인" << endl;
        cout << "6. 지출 내역 저장" << endl;
        cout << "7. 지출 내역 불러오기" << endl;
        cout << "8. 지출 내역 날짜별 정렬" << endl;
        cout << "9. 지출 내역 카테고리별 정렬" << endl;
        cout << "0. 종료" << endl;
        cout << "메뉴를 선택하세요: ";
        cin >> menu;
        switch (menu)
        {
        case 1:
            addExpense();
            break;
        case 2:
            deleteExpense();
            break;
        case 3:
            showExpense();
            break;
        case 4:
            editExpense();
            break;
        case 5:
            showTotalPrice();
            break;
        case 6:
            saveExpense();
            break;
        case 7:
            loadExpense();
            break;
        case 8:
            sortExpenseByDate();
            break;
        case 9:
            sortExpenseByCategory();
            break;
        case 0:
            return 0;
        }
    }
}

void addExpense()
{
    system("cls");
    Expense expense;
    cout << "날짜: ";
    cin >> expense.date;
    cout << "카테고리: ";
    cin >> expense.category;
    cout << "내용: ";
    cin.ignore();
    getline(cin, expense.content);
    cout << "금액: ";
    cin >> expense.price;
    expenses.push_back(expense);
}

void deleteExpense()
{
    system("cls");
    int index;
    cout << "삭제할 지출 내역 번호: ";
    cin >> index;
    if (index < 1 || index > expenses.size())
    {
        cout << "잘못된 번호입니다." << endl;
        return;
    }
    expenses.erase(expenses.begin() + index - 1);
}

void showExpense()
{
    system("cls");
    for (int i = 0; i < expenses.size(); i++)
    {
        cout << i + 1 << ". " << expenses[i].date << " " << expenses[i].category << " " << expenses[i].content << " " << expenses[i].price << endl;
    }
    system("pause");
}

void editExpense()
{
    system("cls");
    int index;
    cout << "수정할 지출 내역 번호: ";
    cin >> index;
    if (index < 1 || index > expenses.size())
    {
        cout << "잘못된 번호입니다." << endl;
        return;
    }
    cout << "날짜: ";
    cin >> expenses[index - 1].date;
    cout << "카테고리: ";
    cin >> expenses[index - 1].category;
    cout << "내용: ";
    cin.ignore();
    getline(cin, expenses[index - 1].content);
    cout << "금액: ";
    cin >> expenses[index - 1].price;
}

void showTotalPrice()
{
    system("cls");
    int totalPrice = 0;
    for (int i = 0; i < expenses.size(); i++)
    {
        totalPrice += expenses[i].price;
    }
    cout << "총 지출 금액: " << totalPrice << endl;
    system("pause");
}

void saveExpense()
{
    system("cls");
    ofstream ofs("expenses.txt");
    for (int i = 0; i < expenses.size(); i++)
    {
        ofs << expenses[i].date << " " << expenses[i].category << " " << expenses[i].content << " " << expenses[i].price << endl;
    }
    ofs.close();
}

void loadExpense()
{
    expenses.clear();
    ifstream ifs("expenses.txt");
    if (!ifs.is_open())
    {
        return;
    }
    while (!ifs.eof())
    {
        Expense expense;
        ifs >> expense.date >> expense.category >> expense.content >> expense.price;
        expenses.push_back(expense);
    }
    ifs.close();
}

void sortExpenseByDate()
{
    system("cls");
    sort(expenses.begin(), expenses.end(), [](const Expense &a, const Expense &b) {
        return a.date < b.date;
    });
}

void sortExpenseByCategory()
{
    system("cls");
    sort(expenses.begin(), expenses.end(), [](const Expense &a, const Expense &b) {
        return a.category < b.category;
    });
}





