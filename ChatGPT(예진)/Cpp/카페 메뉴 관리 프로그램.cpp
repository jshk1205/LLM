#include <iostream>
#include <vector>

#define MAX_ITEMS 100

struct MenuItem {
    std::string item_name;
    int price;
    int stock;
};

std::vector<MenuItem> menu;
std::string admin_password = "";
bool logged_in_as_admin = false;

void set_admin_password() {
    while (true) {
        std::string password, confirm_password;
        std::cout << "관리자 비밀번호를 설정하세요: ";
        std::cin >> password;
        std::cout << "비밀번호를 다시 한 번 입력하세요: ";
        std::cin >> confirm_password;
        if (password == confirm_password) {
            admin_password = password;
            std::cout << "관리자 비밀번호가 설정되었습니다.\n";
            break;
        }
        else {
            std::cout << "비밀번호가 일치하지 않습니다. 다시 시도하세요.\n";
        }
    }
}

void add_menu() {
    while (true) {
        std::string item_name;
        int price;
        std::cout << "추가할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ";
        std::cin >> item_name;
        if (item_name == "메뉴 관리 완료!")
            break;
        if (menu.size() >= MAX_ITEMS) {
            std::cout << "더 이상 메뉴를 추가할 수 없습니다. 최대 개수에 도달했습니다.\n";
            break;
        }
        bool already_exists = false;
        for (const auto& menuItem : menu) {
            if (menuItem.item_name == item_name) {
                already_exists = true;
                std::cout << "이미 존재하는 메뉴입니다.\n";
                break;
            }
        }
        if (already_exists)
            continue;
        std::cout << "가격을 입력하세요: ";
        std::cin >> price;
        if (price > 10000) {
            std::cout << "가격이 너무 높습니다. 10000원을 초과할 수 없습니다.\n";
        }
        else if (price < 1000) {
            std::cout << "가격이 너무 낮습니다. 1000원 이상이어야 합니다.\n";
        }
        else {
            menu.push_back({ item_name, price, 10 });
        }
    }
}

void delete_menu() {
    while (true) {
        std::string item_name;
        std::cout << "삭제할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ";
        std::cin >> item_name;
        if (item_name == "메뉴 관리 완료!")
            // 메뉴 관리 완료를 치면 무한루프를 돔... GPT가 똑같은 문제 계속 발생시킴
            break;
        bool found = false;
        for (auto it = menu.begin(); it != menu.end(); ++it) {
            if (it->item_name == item_name) {
                found = true;
                menu.erase(it);
                std::cout << "메뉴가 삭제되었습니다.\n";
                break;
            }
        }
        if (!found) {
            std::cout << "존재하지 않는 메뉴입니다.\n";
        }
    }
}

void show_menu() {
    std::cout << "\n메뉴 목록:\n";
    for (const auto& menuItem : menu) {
        std::cout << menuItem.item_name << ": " << menuItem.price << "원 - 재고: " << menuItem.stock << "개\n";
    }
}

int main() {
    set_admin_password();
    int choice;
    while (true) {
        std::cout << "\n1. 메뉴 추가\n2. 메뉴 삭제\n3. 메뉴 조회\n4. 종료\n";
        std::cout << "원하는 작업을 선택하세요: ";
        std::cin >> choice;
        switch (choice) {
        case 1:
            add_menu();
            break;
        case 2:
            delete_menu();
            break;
        case 3:
            show_menu();
            break;
        case 4:
            std::cout << "프로그램을 종료합니다.\n";
            return 0; // main 함수 종료
        default:
            std::cout << "잘못된 입력입니다. 다시 시도하세요.\n";
        }
    }
}
