import calendar

def main():
    while True:
        print("1. Select year")
        print("2. Select month")
        print("3. Exit")
        choice = input("Please select: ")

        if choice == "1":
            year = int(input("Enter the year: "))
            month = None
            day = None
            show_month_calendar(year, month, day)
        elif choice == "2":
            year = int(input("Enter the year: "))
            month = int(input("Enter the month: "))
            day = None
            show_day_calendar(year, month, day)
        elif choice == "3":
            print("Exiting the program.")
            break
        else:
            print("Invalid choice. Please choose again.")

def show_month_calendar(year, month, day):
    print("Select a month:")
    for i, month_name in enumerate(calendar.month_name[1:], start=1):
        print(f"{i}. {month_name}")

    month = int(input("Please select: "))
    if month < 1 or month > 12:
        print("Invalid input.")
        return

    _, days_in_month = calendar.monthrange(year, month)
    print(calendar.month(year, month))
    choose_day(year, month, days_in_month)

def show_day_calendar(year, month, day):
    print("Select a date:")
    _, days_in_month = calendar.monthrange(year, month)
    for day in range(1, days_in_month + 1):
        print(day)

    day = int(input("Please select: "))
    if day < 1 or day > days_in_month:
        print("Invalid input.")
        return

    print(f"{year} {month} {day}")
    print(f"Birth flower: {get_birth_flower(month)}")

def choose_day(year, month, days_in_month):
    day = int(input("Select a date: "))
    if day < 1 or day > days_in_month:
        print("Invalid input.")
        return

    print(f"{year} {month} {day}")
    print(f"Birth flower: {get_birth_flower(month)}")

def get_birth_flower(month):
    birth_flowers = {
        1: "Carnation",
        2: "Snowdrop",
        3: "Daffodil",
        4: "Daisy",
        5: "Lily",
        6: "Rose",
        7: "Water Lily",
        8: "Sunflower",
        9: "Poppy",
        10: "Marigold",
        11: "Chrysanthemum",
        12: "Poinsettia"
    }
    return birth_flowers.get(month, "Unknown")

if __name__ == "__main__":
    main()
