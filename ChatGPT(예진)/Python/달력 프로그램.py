# CHATGPT
"""
파이썬으로 코드를 생성해 주세요.
주제는 달력 프로그램입니다.
코드 생성 조건은 다음과 같습니다.
- 실행이 되어야 합니다.
- Year, Month, Day 3가지가 무조건 포함되어 있어야 합니다.
- 년도를 입력하면 화면에 선택 가능한 달이 뜨도록 합니다.
- 달을 입력하면 화면에 선택 가능한 날짜가 뜨도록 합니다.
- 윤년이 있는 경우도 놓치지 말고 구현해 주세요.
- 년도와 달. 날짜까지 모두 선택하면 화면에 출력해 주세요.
- 출력한 날의 탄생화를 아래에 함께 출력해 주세요.
"""

import calendar

def main():
    while True:
        print("1. 년도 선택")
        print("2. 달 선택")
        print("3. 종료")
        choice = input("선택하세요: ")

        if choice == "1":
            year = int(input("년도를 입력하세요: "))
            month = None
            day = None
            show_month_calendar(year, month, day)
        elif choice == "2":
            year = int(input("년도를 입력하세요: "))
            month = int(input("달을 입력하세요: "))
            day = None
            show_day_calendar(year, month, day)
        elif choice == "3":
            print("프로그램을 종료합니다.")
            break
        else:
            print("잘못된 선택입니다. 다시 선택해 주세요.")

def show_month_calendar(year, month, day):
    print("달을 선택하세요:")
    for i, month_name in enumerate(calendar.month_name[1:], start=1):
        print(f"{i}. {month_name}")

    month = int(input("선택하세요: "))
    if month < 1 or month > 12:
        print("잘못된 입력입니다.")
        return

    _, days_in_month = calendar.monthrange(year, month)
    print(calendar.month(year, month))
    choose_day(year, month, days_in_month)

def show_day_calendar(year, month, day):
    print("날짜를 선택하세요:")
    _, days_in_month = calendar.monthrange(year, month)
    for day in range(1, days_in_month + 1):
        print(day)

    day = int(input("선택하세요: "))
    if day < 1 or day > days_in_month:
        print("잘못된 입력입니다.")
        return

    print(f"{year}년 {month}월 {day}일")
    print(f"탄생화: {get_birth_flower(month)}")

def choose_day(year, month, days_in_month):
    day = int(input("날짜를 선택하세요: "))
    if day < 1 or day > days_in_month:
        print("잘못된 입력입니다.")
        return

    print(f"{year}년 {month}월 {day}일")
    print(f"탄생화: {get_birth_flower(month)}")

def get_birth_flower(month):
    birth_flowers = {
        1: "양귀비",
        2: "수선화",
        3: "민들레",
        4: "벚꽃",
        5: "장미",
        6: "라벤더",
        7: "수련",
        8: "해바라기",
        9: "백합",
        10: "금잔화",
        11: "카네이션",
        12: "포인세티아"
    }
    return birth_flowers.get(month, "알 수 없음")

if __name__ == "__main__":
    main()
