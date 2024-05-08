#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#define MAX_ENTRIES 100

struct User {
    std::string name;
    std::string username;
    std::string password;
    std::vector<tm> diaryEntries;
    std::vector<std::string> entries;
};

void displayCalendar(const tm& date) {
    int month = date.tm_mon + 1;
    int year = date.tm_year + 1900;
    int numDays = 31; // You may need to calculate this based on the month and year

    std::cout << (month == 1 ? "January" : month == 2 ? "February" : month == 3 ? "March" : month == 4 ? "April" : month == 5 ? "May" : month == 6 ? "June" : month == 7 ? "July" : month == 8 ? "August" : month == 9 ? "September" : month == 10 ? "October" : month == 11 ? "November" : "December") << " " << year << std::endl;
    std::cout << "  M  T  W  T  F  S  S" << std::endl;

    // Implement the calendar display logic here
}

void diaryPage(User& user, const tm& date) {
    std::cout << "\n--- Diary Entry: " << date.tm_mon + 1 << "-" << date.tm_mday << "-" << date.tm_year + 1900 << " ---" << std::endl;
    std::cout << "Write your diary entry (Press Enter to finish):" << std::endl;
    std::string entry;
    std::getline(std::cin, entry);

    if (!entry.empty()) {
        user.entries.push_back(entry);
        user.diaryEntries.push_back(date);
        std::cout << "Entry saved successfully!" << std::endl;
    }
    else {
        std::cout << "No entry to save." << std::endl;
    }
}

void diaryListPage(const User& user) {
    std::cout << "\n--- Diary List ---" << std::endl;
    for (const auto& entry : user.diaryEntries) {
        std::cout << entry.tm_mon + 1 << "-" << entry.tm_mday << "-" << entry.tm_year + 1900 << std::endl;
    }
}

void mainPage(User& currentUser) {
    int choice;
    while (true) {
        std::cout << "\nWelcome, " << currentUser.name << "'s Diary" << std::endl;
        tm today = { 0 }; // Current date initialization
        displayCalendar(today);

        std::cout << "3. My Diary\n";
        std::cout << "4. My Diary List\n";
        std::cout << "5. Logout\n";
        std::cout << "Select an option: ";
        std::cin >> choice;
        std::cin.ignore(); // Consume the newline character left in the input buffer

        switch (choice) {
        case 3:
            diaryPage(currentUser, today);
            break;
        case 4:
            diaryListPage(currentUser);
            break;
        case 5:
            return; // Return to the main menu
        default:
            std::cout << "Invalid option." << std::endl;
        }
    }
}

void login(std::vector<User>& users, User*& currentUser) {
    std::cout << "\n--- Login ---" << std::endl;
    std::string username, password;
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    std::cout << "Enter your password: ";
    std::getline(std::cin, password);

    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            currentUser = &user;
            std::cout << "Login successful!" << std::endl;
            return;
        }
    }
    std::cout << "Invalid username or password." << std::endl;
}

void signUp(std::vector<User>& users) {
    std::cout << "\n--- Sign Up ---" << std::endl;
    User newUser;
    std::cout << "Enter your name: ";
    std::getline(std::cin, newUser.name);
    std::cout << "Enter a username: ";
    std::getline(std::cin, newUser.username);

    // Check if username already exists

    std::cout << "Enter a password: ";
    std::getline(std::cin, newUser.password);

    users.push_back(newUser);
}

int main() {
    std::vector<User> users; // Vector to store users
    User* currentUser = nullptr;
    int choice;

    while (true) {
        std::cout << "1. Login\n";
        std::cout << "2. Sign Up\n";
        std::cout << "3. Quit\n";
        std::cout << "Select an option: ";
        std::cin >> choice;
        std::cin.ignore(); // Consume the newline character left in the input buffer

        switch (choice) {
        case 1:
            login(users, currentUser);
            if (currentUser != nullptr) {
                mainPage(*currentUser);
            }
            break;
        case 2:
            signUp(users);
            break;
        case 3:
            std::cout << "Goodbye!" << std::endl;
            return 0; // 프로그램 종료
        default:
            std::cout << "Invalid option." << std::endl;
        }
    }

    return 0;
}
