from datetime import datetime

menu = {}
inventory = {}
orders = []
sales = {"일별": {}, "주별": {}, "월별": {}}
admin_password = ""
logged_in_as_admin = False

def set_admin_password():
    global admin_password
    while True:
        password = input("관리자 비밀번호를 설정하세요: ")
        confirm_password = input("비밀번호를 다시 한 번 입력하세요: ")
        if password == confirm_password:
            admin_password = password
            print("관리자 비밀번호가 설정되었습니다.")
            break
        else:
            print("비밀번호가 일치하지 않습니다. 다시 시도하세요.")

def add_menu():
    global inventory
    while True:
        item = input("추가할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ")
        if item == "메뉴 관리 완료!":
            break
        if item in menu:
            print("이미 존재하는 메뉴입니다.")
            continue
        price = int(input("가격을 입력하세요: "))
        if price > 10000:
            print("가격이 너무 높습니다. 10000원을 초과할 수 없습니다.")
        elif price < 1000:
            print("가격이 너무 낮습니다. 1000원 이상이어야 합니다.")
        else:
            inventory[item] = 10  # 초기 재고 설정
            menu[item] = {"가격": price, "재고": inventory[item]}

def delete_menu():
    global inventory
    while True:
        item = input("삭제할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ")
        if item == "메뉴 관리 완료!":
            break
        if item not in menu:
            print("존재하지 않는 메뉴입니다.")
        else:
            del menu[item]
            del inventory[item]

def show_menu():
    print("\n메뉴 목록:")
    for item, details in menu.items():
        print(f"{item}: {details['가격']}원 - 재고: {details['재고']}개")

def add_inventory():
    while True:
        item = input("재고를 추가할 메뉴를 입력하세요 (종료하려면 '재고 관리 완료!' 입력): ")
        if item == "재고 관리 완료!":
            break
        if item not in inventory:
            print("존재하지 않는 메뉴입니다.")
            continue
        quantity = int(input("추가할 재고 수량을 입력하세요: "))
        if quantity <= 0:
            print("유효하지 않은 재고 수량입니다.")
        else:
            inventory[item] += quantity
            menu[item]["재고"] += quantity

def manage_inventory():
    global logged_in_as_admin
    if not logged_in_as_admin:
        password = input("관리자 비밀번호를 입력하세요: ")
        if password != admin_password:
            print("잘못된 비밀번호입니다.")
            return
        else:
            logged_in_as_admin = True
            print("관리자 권한으로 로그인되었습니다.")
    while True:
        print("\n1. 재고 추가\n2. 종료")
        choice = input("원하는 작업을 선택하세요: ")
        if choice == "1":
            add_inventory()
        elif choice == "2":
            print("재고 관리를 종료합니다.")
            logged_in_as_admin = False
            break
        else:
            print("잘못된 입력입니다. 다시 시도하세요.")

def take_order():
    global orders
    while True:
        item = input("주문할 메뉴를 입력하세요 (종료하려면 '주문 완료!' 입력): ")
        if item == "주문 완료!":
            break
        if item not in menu:
            print("존재하지 않는 메뉴입니다.")
            continue
        quantity = int(input("주문할 수량을 입력하세요: "))
        if quantity <= 0:
            print("유효하지 않은 수량입니다.")
        elif quantity > menu[item]["재고"]:
            print("재고가 부족합니다.")
        else:
            orders.append({"메뉴": item, "수량": quantity, "주문 시간": datetime.now()})
            menu[item]["재고"] -= quantity

def process_order():
    total_price = 0
    print("\n주문 내역:")
    for order in orders:
        item = order["메뉴"]
        quantity = order["수량"]
        price = menu[item]["가격"]
        total_price += price * quantity
        print(f"{item}: {price}원 x {quantity}개")
    print(f"총 주문 가격: {total_price}원")
    payment_method = input("결제 방법을 선택하세요 (현금 또는 카드): ")
    print(f"{payment_method} 결제가 완료되었습니다.")
    orders.clear()  # 주문 내역 초기화

def main():
    global admin_password
    if not admin_password:
        set_admin_password()
    while True:
        print("\n1. 메뉴 추가\n2. 메뉴 삭제\n3. 메뉴 조회\n4. 재고 관리\n5. 주문\n6. 결제\n7. 종료")
        choice = input("원하는 작업을 선택하세요: ")
        if choice == "1":
            add_menu()
        elif choice == "2":
            delete_menu()
        elif choice == "3":
            show_menu()
        elif choice == "4":
            manage_inventory()
        elif choice == "5":
            take_order()
        elif choice == "6":
            process_order()
        elif choice == "7":
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 입력입니다. 다시 시도하세요.")

if __name__ == "__main__":
    main()
