//주식거래시뮬레이션을 c언어로 만들어줘
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_STOCK 10
#define MAX_NAME 20

typedef struct Stock
{
    char name[MAX_NAME];
    int price;
    double changeRate;
} Stock;

void changePrice(Stock *stock)
{
    stock->changeRate = (rand() % 401 - 200) / 100.0;
    stock->price = stock->price + stock->price * stock->changeRate;
    if (stock->price < 1000)
    {
        stock->price = 1000;
    }
    else if (stock->price > 10000)
    {
        stock->price = 10000;
    }
}

void buyStock(Stock *stock, int *money, int *stockCount)
{
    int count;
    printf("몇 주를 구매하시겠습니까? ");
    scanf("%d", &count);
    if (*money < stock->price * count)
    {
        printf("소지금이 부족합니다.\n");
        return;
    }
    *money -= stock->price * count;
    *stockCount += count;
}

void sellStock(Stock *stock, int *money, int *stockCount)
{
    int count;
    printf("몇 주를 판매하시겠습니까? ");
    scanf("%d", &count);
    if (*stockCount < count)
    {
        printf("주식이 부족합니다.\n");
        return;
    }
    *money += stock->price * count;
    *stockCount -= count;
}

int main()
{
    srand(time(NULL));
    Stock stocks[MAX_STOCK] = {
        {"삼성전자", 5000, 0},
        {"카카오", 5000, 0},
        {"네이버", 5000, 0},
        {"삼성바이오", 5000, 0},
        {"셀트리온", 5000, 0},
        {"현대차", 5000, 0},
        {"기아차", 5000, 0},
        {"LG전자", 5000, 0},
        {"SK하이닉스", 5000, 0},
        {"POSCO", 5000, 0},
    };
    int money = 100000;
    int stockCount = 0;
    while (1)
    {
        for (int i = 0; i < MAX_STOCK; i++)
        {
            changePrice(&stocks[i]);
        }
        printf("소지금: %d\n", money);
        printf("주식 보유량: %d\n", stockCount);
        printf("주식 목록\n");
        for (int i = 0; i < MAX_STOCK; i++)
        {
            printf("%d. %s: %d\n", i + 1, stocks[i].name, stocks[i].price);
        }
        printf("0. 종료\n");
        int menu;
        printf("메뉴를 선택하세요: ");
        scanf("%d", &menu);
        if (menu == 0)
        {
            break;
        }
        menu--;
        if (menu < 0 || menu >= MAX_STOCK)
        {
            printf("잘못된 메뉴입니다.\n");
            continue;
        }
        printf("1. 구매\n");
        printf("2. 판매\n");
        printf("메뉴를 선택하세요: ");
        scanf("%d", &menu);
        if (menu == 1)
        {
            buyStock(&stocks[menu], &money, &stockCount);
        }
        else if (menu == 2)
        {
            sellStock(&stocks[menu], &money, &stockCount);
        }
        else
        {
            printf("잘못된 메뉴입니다.\n");
        }
    }
    return 0;
}


