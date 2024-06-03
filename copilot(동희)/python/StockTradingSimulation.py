#1. 함수에다가 함수설명 주석달기(함수에대한 이름만 간단히)
#2. 실행이 되야됨.
#3. 기능은 적어도 최소한 5~10개
#4. 함수끼리의 연관성이 있으면 좋음
#5. 코드는 길수록 좋음
#6. 가능하면 다양한 기능이 있어야됨
#7. 클래스를 사용하면 더 좋음
#8. 주석이 많으면 좋음
#9. 코드가 길어도 상관없음
#10. 코드가 복잡해도 상관없음
#11. 사용자의 소지금이 있어야됨
#12. 주식을 살수있어야됨
#13. 주식을 팔수있어야됨
#14. 주식의 가격이 변동되어야됨
#15. 주식을 살때마다 소지금이 줄어야됨
#16. 주식을 팔때마다 소지금이 늘어나야됨
#17. 주식을 살때마다 주식의 수량이 늘어나야됨
#18. 주식을 팔때마다 주식의 수량이 줄어야됨
#19. 주식을 살때마다 주식의 가격이 변동되어야됨
#20. 주식을 팔때마다 주식의 가격이 변동되어야됨
#21. 주식의 변동률은 랜덤하게 바뀜
#22. 주식의 변동률은 1%이상 5%이하
#2. 변동률은 소수점 2자리수까지만
#23. 주식의 가격은 1000원이상 10000원이하
#24. 주식의 가격은 100원단위로만
#25. 주식의 가격은 변동률에 따라 변동됨
#26. 사용자의 소지금도 주식의 가격에 따라 변동됨
#27. 사용자의 소지금은 0원이상
#28. 사용자의 초기 소지금은 100000원

#29. 주식의 이름은 삼성전자, 카카오, 네이버, 삼성바이오로직스, 셀트리온, 현대차, 기아차, LG전자, SK하이닉스, POSCO

import random

class StockTradingSimulation:
    def __init__(self):
        self.stocks = {
            "삼성전자": {"price": 8000, "quantity": 0},
            "카카오": {"price": 5000, "quantity": 0},
            "네이버": {"price": 6000, "quantity": 0},
            "삼성바이오로직스": {"price": 7000, "quantity": 0},
            "셀트리온": {"price": 4000, "quantity": 0},
            "현대차": {"price": 3000, "quantity": 0},
            "기아차": {"price": 2000, "quantity": 0},
            "LG전자": {"price": 1000, "quantity": 0},
            "SK하이닉스": {"price": 9000, "quantity": 0},
            "POSCO": {"price": 10000, "quantity": 0},
        }
        self.balance = 100000

    def buy_stock(self, stock_name, quantity):
        if stock_name not in self.stocks:
            print("주식을 찾을 수 없습니다.")
            return
        stock = self.stocks[stock_name]
        total_price = stock["price"] * quantity
        if self.balance < total_price:
            print("잔액이 부족합니다.")
            return
        stock["quantity"] += quantity
        self.balance -= total_price
        print(f"{stock_name} {quantity}주를 {stock['price']}원에 구매했습니다.")
        print(f"잔액: {self.balance}원")

    def sell_stock(self, stock_name, quantity):
        if stock_name not in self.stocks:
            print("주식을 찾을 수 없습니다.")
            return
        stock = self.stocks[stock_name]
        if stock["quantity"] < quantity:
            print("보유 주식이 부족합니다.")
            return
        total_price = stock["price"] * quantity
        stock["quantity"] -= quantity
        self.balance += total_price
        print(f"{stock_name} {quantity}주를 {stock['price']}원에 판매했습니다.")
        print(f"잔액: {self.balance}원")

    def update_stock_price(self):
        for stock in self.stocks.values():
            stock["price"] *= random.uniform(0.99, 1.05)
            stock["price"] = round(stock["price"] / 100) * 100

    def print_stock_price(self):
        for stock_name, stock in self.stocks.items():
            print(f"{stock_name}: {stock['price']}원")

    def print_balance(self):
        print(f"잔액: {self.balance}원")

    def run(self):
        while True:
            print("1. 주식 구매")
            print("2. 주식 판매")
            print("3. 주식 가격 업데이트")
            print("4. 주식 가격 확인")
            print("5. 잔액 확인")
            print("6. 종료")
            menu = input("메뉴를 선택하세요: ")
            if menu == '1':
                stock_name = input("구매할 주식 이름을 입력하세요: ")
                quantity = int(input("구매할 주식 수량을 입력하세요: "))
                self.buy_stock(stock_name, quantity)
            elif menu == '2':
                stock_name = input("판매할 주식 이름을 입력하세요: ")
                quantity = int(input("판매할 주식 수량을 입력하세요: "))
                self.sell_stock(stock_name, quantity)
            elif menu == '3':
                self.update_stock_price()
                print("주식 가격이 업데이트되었습니다.")
            elif menu == '4':
                self.print_stock_price()
            elif menu == '5':
                self.print_balance()
            elif menu == '6':
                break
            else:
                print("잘못된 메뉴입니다.")

if __name__ == "__main__":
    simulation = StockTradingSimulation()
    simulation.run()

