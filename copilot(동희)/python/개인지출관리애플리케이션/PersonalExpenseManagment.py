#개인 지출 관리 애플리케이션을 만들어줘
# 파이썬 언어로 만들것
#1. 지출 내역을 입력받아서 저장할 수 있어야 해
#2. 지출 내역을 삭제할 수 있어야 해
#3. 지출 내역을 조회할 수 있어야 해
#4. 지출 내역을 수정할 수 있어야 해
#5. 총 지출 금액을 확인할 수 있어야 해
#6. 지출 내역을 파일로 저장하고 불러올 수 있어야 해
#7. 지출 내역을 날짜별로 정렬할 수 있어야 해
#8. 지출 내역을 카테고리별로 정렬할 수 있어야 해

import json
import os
import re
import datetime

def personal_expense_management():
    expenses = []
    while True:
        print("1. 지출 내역 입력")
        print("2. 지출 내역 삭제")
        print("3. 지출 내역 조회")
        print("4. 지출 내역 수정")
        print("5. 총 지출 금액 확인")
        print("6. 파일로 저장 및 불러오기")
        print("7. 종료")
        menu = input("메뉴를 선택하세요: ")
        if menu == '1':
            expense = {}
            expense["date"] = input("날짜를 입력하세요(YYYY-MM-DD): ")
            expense["category"] = input("카테고리를 입력하세요: ")
            expense["content"] = input("내용을 입력하세요: ")
            expense["amount"] = int(input("금액을 입력하세요: "))
            expenses.append(expense)
            print("지출 내역이 입력되었습니다.")
        elif menu == '2':
            date = input("삭제할 지출 내역의 날짜를 입력하세요(YYYY-MM-DD): ")
            for expense in expenses:
                if expense["date"] == date:
                    expenses.remove(expense)
                    print("지출 내역이 삭제되었습니다.")
                    break
            else:
                print("지출 내역을 찾을 수 없습니다.")
        elif menu == '3':
            date = input("조회할 지출 내역의 날짜를 입력하세요(YYYY-MM-DD): ")
            for expense in expenses:
                if expense["date"] == date:
                    print("날짜:", expense["date"])
                    print("카테고리:", expense["category"])
                    print("내용:", expense["content"])
                    print("금액:", expense["amount"])
                    break
            else:
                print("지출 내역을 찾을 수 없습니다.")
        elif menu == '4':
            date = input("수정할 지출 내역의 날짜를 입력하세요(YYYY-MM-DD): ")
            for expense in expenses:
                if expense ["date"] == date:
                    expense["category"] = input("카테고리를 입력하세요: ")
                    expense["content"] = input("내용을 입력하세요: ")
                    expense["amount"] = int(input("금액을 입력하세요: "))
                    print("지출 내역이 수정되었습니다.")
                    break
            else:
                print("지출 내역을 찾을 수 없습니다.")
        elif menu == '5':
            total_amount = 0
            for expense in expenses:
                total_amount += expense["amount"]
            print("총 지출 금액:", total_amount)
        elif menu == '6':
            filename = input("파일 이름을 입력하세요: ")
            with open(filename, "w") as file:
                json.dump(expenses, file)
            print("파일이 저장되었습니다.")
        elif menu == '7':
            break
        else:
            print("잘못된 메뉴입니다.")

if __name__ == "__main__":
    personal_expense_management()





