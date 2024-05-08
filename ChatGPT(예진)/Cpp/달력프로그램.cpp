#include <iostream>
#include <string>

#define MAX_MONTH_NAME_LENGTH 20

void show_month_calendar(int year, int month, int day);
void show_day_calendar(int year, int month, int day);
void choose_day(int year, int month, int days_in_month);
const std::string get_birth_flower(int month);

int main() {
    std::string choice;
    int year, month, day;

    while (true) {
        std::cout << "1. 년도 선택\n";
        std::cout << "2. 달 선택\n";
        std::cout << "3. 종료\n";
        std::cout << "선택하세요: ";
        std::cin >> choice;

        if (choice == "1") {
            std::cout << "년도를 입력하세요: ";
            std::cin >> year;
            month = 0;
            day = 0;
            show_month_calendar(year, month, day);
        }
        else if (choice == "2") {
            std::cout << "년도를 입력하세요: ";
            std::cin >> year;
            std::cout << "달을 입력하세요: ";
            std::cin >> month;
            day = 0;
            show_day_calendar(year, month, day);
        }
        else if (choice == "3") {
            std::cout << "프로그램을 종료합니다.\n";
            break;
        }
        else {
            std::cout << "잘못된 선택입니다. 다시 선택해 주세요.\n";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
    }

    return 0;
}

void show_month_calendar(int year, int month, int day) {
    std::cout << "달을 선택하세요:\n";
    std::string month_names[12] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };

    for (int i = 0; i < 12; i++) {
        std::cout << i + 1 << ". " << month_names[i] << '\n';
    }

    std::cout << "선택하세요: ";
    std::cin >> month;
    if (month < 1 || month > 12) {
        std::cout << "잘못된 입력입니다.\n";
        return;
    }

    int days_in_month;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_month = 29;
        }
        else {
            days_in_month = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    }
    else {
        days_in_month = 31;
    }

    std::cout << "Calendar for " << year << ' ' << month_names[month - 1] << '\n';
    choose_day(year, month, days_in_month);
}

void show_day_calendar(int year, int month, int day) {
    std::cout << "날짜를 선택하세요:\n";
    int days_in_month;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_month = 29;
        }
        else {
            days_in_month = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    }
    else {
        days_in_month = 31;
    }

    for (int i = 1; i <= days_in_month; i++) {
        std::cout << i << '\n';
    }

    std::cout << "선택하세요: ";
    std::cin >> day;
    if (day < 1 || day > days_in_month) {
        std::cout << "잘못된 입력입니다.\n";
        return;
    }

    std::cout << year << "년 " << month << "월 " << day << "일\n";
    std::cout << "탄생화: " << get_birth_flower(month) << '\n';
}

void choose_day(int year, int month, int days_in_month) {
    int day;
    std::cout << "날짜를 선택하세요: ";
    std::cin >> day;
    if (day < 1 || day > days_in_month) {
        std::cout << "잘못된 입력입니다.\n";
        return;
    }

    std::cout << year << "년 " << month << "월 " << day << "일\n";
    std::cout << "탄생화: " << get_birth_flower(month) << '\n';
}

const std::string get_birth_flower(int month) {
    const std::string birth_flowers[] = { "", "양귀비", "수선화", "민들레", "벚꽃", "장미", "라벤더", "수련", "해바라기", "백합", "금잔화", "카네이션", "포인세티아" };
    return birth_flowers[month];
}
