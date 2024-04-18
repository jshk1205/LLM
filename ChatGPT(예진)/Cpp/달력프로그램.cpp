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
        std::cout << "1. �⵵ ����\n";
        std::cout << "2. �� ����\n";
        std::cout << "3. ����\n";
        std::cout << "�����ϼ���: ";
        std::cin >> choice;

        if (choice == "1") {
            std::cout << "�⵵�� �Է��ϼ���: ";
            std::cin >> year;
            month = 0;
            day = 0;
            show_month_calendar(year, month, day);
        }
        else if (choice == "2") {
            std::cout << "�⵵�� �Է��ϼ���: ";
            std::cin >> year;
            std::cout << "���� �Է��ϼ���: ";
            std::cin >> month;
            day = 0;
            show_day_calendar(year, month, day);
        }
        else if (choice == "3") {
            std::cout << "���α׷��� �����մϴ�.\n";
            break;
        }
        else {
            std::cout << "�߸��� �����Դϴ�. �ٽ� ������ �ּ���.\n";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
    }

    return 0;
}

void show_month_calendar(int year, int month, int day) {
    std::cout << "���� �����ϼ���:\n";
    std::string month_names[12] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };

    for (int i = 0; i < 12; i++) {
        std::cout << i + 1 << ". " << month_names[i] << '\n';
    }

    std::cout << "�����ϼ���: ";
    std::cin >> month;
    if (month < 1 || month > 12) {
        std::cout << "�߸��� �Է��Դϴ�.\n";
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
    std::cout << "��¥�� �����ϼ���:\n";
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

    std::cout << "�����ϼ���: ";
    std::cin >> day;
    if (day < 1 || day > days_in_month) {
        std::cout << "�߸��� �Է��Դϴ�.\n";
        return;
    }

    std::cout << year << "�� " << month << "�� " << day << "��\n";
    std::cout << "ź��ȭ: " << get_birth_flower(month) << '\n';
}

void choose_day(int year, int month, int days_in_month) {
    int day;
    std::cout << "��¥�� �����ϼ���: ";
    std::cin >> day;
    if (day < 1 || day > days_in_month) {
        std::cout << "�߸��� �Է��Դϴ�.\n";
        return;
    }

    std::cout << year << "�� " << month << "�� " << day << "��\n";
    std::cout << "ź��ȭ: " << get_birth_flower(month) << '\n';
}

const std::string get_birth_flower(int month) {
    const std::string birth_flowers[] = { "", "��ͺ�", "����ȭ", "�ε鷹", "����", "���", "�󺥴�", "����", "�عٶ��", "����", "����ȭ", "ī���̼�", "���μ�Ƽ��" };
    return birth_flowers[month];
}
