//주식거래시뮬레이션을 c++언어로 만들어줘
//1. 함수에다가 함수설명 주석달기(함수에대한 이름만 간단히)
//2. 실행이 되야됨.
//3. 기능은 적어도 최소한 5~10개
//4. 함수끼리의 연관성이 있으면 좋음
//5. 코드는 길수록 좋음
//6. 가능하면 다양한 기능이 있어야됨
//7. 클래스를 사용하면 더 좋음
//8. 주석이 많으면 좋음
//9. 코드가 길어도 상관없음
//10. 코드가 복잡해도 상관없음
//11. 사용자의 소지금이 있어야됨
//12. 주식을 살수있어야됨
//13. 주식을 팔수있어야됨
//14. 주식의 가격이 변동되어야됨
//15. 주식을 살때마다 소지금이 줄어야됨
//16. 주식을 팔때마다 소지금이 늘어나야됨
//17. 주식을 살때마다 주식의 수량이 늘어나야됨
//18. 주식을 팔때마다 주식의 수량이 줄어야됨
//19. 주식을 살때마다 주식의 가격이 변동되어야됨
//20. 주식을 팔때마다 주식의 가격이 변동되어야됨
//21. 주식의 변동률은 랜덤하게 바뀜
//22. 주식의 변동률은 1%이상 5%이하
//2. 변동률은 소수점 2자리수까지만
//23. 주식의 가격은 1000원이상 10000원이하
//24. 주식의 가격은 100원단위로만
//25. 주식의 가격은 변동률에 따라 변동됨
//26. 사용자의 소지금도 주식의 가격에 따라 변동됨
//27. 사용자의 소지금은 0원이상
//28. 사용자의 초기 소지금은 100000원

//29. 주식의 이름은 삼성전자, 카카오, 네이버, 삼성바이오로직스, 셀트리온, 현대차, 기아차, LG전자, SK하이닉스, POSCO


#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class Stock
{
public:
    Stock(string name, int price)
    {
        this->name = name;
        this->price = price;
        this->changeRate = 0;
    }
    void changePrice()
    {
        this->changeRate = (rand() % 401 - 200) / 100.0;
        this->price = this->price + this->price * this->changeRate;
        if (this->price < 1000)
        {
            this->price = 1000;
        }
        else if (this->price > 10000)
        {
            this->price = 10000;
        }
    }
    void print()
    {
        cout << "주식이름: " << name << endl;
        cout << "주식가격: " << price << endl;
        cout << "변동률: " << changeRate << endl;
    }
    string name;
    int price;
    double changeRate;
};


class User
#include <iostream>

{
public:
    User(int money)
    {
        this->money = money;
    }
    void buyStock(Stock* stock, int amount)
    {
        if (stock->price * amount > money)
        {
            cout << "돈이 부족합니다" << endl;
            return;
        }
        money -= stock->price * amount;
        stock->price += amount;
        cout << "주식을 구매하였습니다" << endl;
    }
    void sellStock(Stock* stock, int amount)
    {
        if (amount > stock->price)
        {
            cout << "주식이 부족합니다" << endl;
            return;
        }
        money += stock->price * amount;
        stock->price -= amount;
        cout << "주식을 판매하였습니다" << endl;
    }
    void print()
    {
        cout << "소지금: " << money << endl;
    }
    int money;
};

int main()
{
    srand(time(NULL));
    vector<Stock*> stockList;
    stockList.push_back(new Stock("삼성전자", 5000));
    stockList.push_back(new Stock("카카오", 3000));
    stockList.push_back(new Stock("네이버", 2000));
    stockList.push_back(new Stock("삼성바이오로직스", 7000));
    stockList.push_back(new Stock("셀트리온", 6000));
    stockList.push_back(new Stock("현대차", 4000));
    stockList.push_back(new Stock("기아차", 3000));
    stockList.push_back(new Stock("LG전자", 4000));
    stockList.push_back(new Stock("SK하이닉스", 5000));
    stockList.push_back(new Stock("POSCO", 6000));
    User* user = new User(100000);
    while (true)
    {
        cout << "1. 주식 구매" << endl;
        cout << "2. 주식 판매" << endl;
        cout << "3. 주식 출력" << endl;
        cout << "4. 소지금 출력" << endl;
        cout << "5. 종료" << endl;
        int menu;
        cin >> menu;
        if (menu == 1)
        {
            cout << "주식번호: ";
            int stockNumber;
            cin >> stockNumber;
            cout << "주식수량: ";
            int amount;
            cin >> amount;
            user->buyStock(stockList[stockNumber], amount);
        }
        else if (menu == 2)
        {
            cout << "주식번호: ";
            int stockNumber;
            cin >> stockNumber;
            cout << "주식수량: ";
            int amount;
            cin >> amount;
            user->sellStock(stockList[stockNumber], amount);
        }
        else if (menu == 3)
        {
            for (int i = 0; i < stockList.size(); i++)
            {
                stockList[i]->changePrice();
                stockList[i]->print();
            }
        }
        else if (menu == 4)
        {
            user->print();
        }
        else if (menu == 5)
        {
            break;
        }
    }
    return 0;
}

