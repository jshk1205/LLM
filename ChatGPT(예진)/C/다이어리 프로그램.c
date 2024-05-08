#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENTRIES 100

struct User {
    char name[50];
    char username[50];
    char password[50];
    struct tm diaryEntries[MAX_ENTRIES];
    char entries[MAX_ENTRIES][500];
    int numEntries;
};

void displayCalendar(struct tm date) {
    int month = date.tm_mon + 1;
    int year = date.tm_year + 1900;
    int numDays = 31; // You may need to calculate this based on the month and year

    printf("%s %d\n", month == 1 ? "January" : month == 2 ? "February" : month == 3 ? "March" : month == 4 ? "April" : month == 5 ? "May" : month == 6 ? "June" : month == 7 ? "July" : month == 8 ? "August" : month == 9 ? "September" : month == 10 ? "October" : month == 11 ? "November" : "December", year);
    printf("  M  T  W  T  F  S  S\n");

    // Implement the calendar display logic here
}

void diaryPage(struct User* user, struct tm* date) {
    printf("\n--- Diary Entry: %02d-%02d-%d ---\n", date->tm_mon + 1, date->tm_mday, date->tm_year + 1900);
    printf("Write your diary entry (Press Enter to finish):\n");
    fgets(user->entries[user->numEntries], 500, stdin);

    // Remove the trailing newline character if exists
    size_t len = strlen(user->entries[user->numEntries]);
    if (user->entries[user->numEntries][len - 1] == '\n') {
        user->entries[user->numEntries][len - 1] = '\0';
    }

    if (strlen(user->entries[user->numEntries]) > 0) {
        memcpy(&user->diaryEntries[user->numEntries], date, sizeof(struct tm));
        user->numEntries++;
        printf("Entry saved successfully!\n");
    }
    else {
        printf("No entry to save.\n");
    }

    getchar(); // Consume the newline character left in the input buffer
}

void diaryListPage(struct User* user) {
    printf("\n--- Diary List ---\n");
    for (int i = 0; i < user->numEntries; i++) {
        printf("%02d-%02d-%d\n", user->diaryEntries[i].tm_mon + 1, user->diaryEntries[i].tm_mday, user->diaryEntries[i].tm_year + 1900);
    }
}

void mainPage(struct User* currentUser) {
    int choice;
    while (1) {
        printf("\nWelcome, %s's Diary\n", currentUser->name);
        struct tm today = { 0 }; // Current date initialization
        displayCalendar(today);

        printf("3. My Diary\n");
        printf("4. My Diary List\n");
        printf("5. Logout\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left in the input buffer

        switch (choice) {
        case 3:
            diaryPage(currentUser, &today);
            break;
        case 4:
            diaryListPage(currentUser);
            break;
        case 5:
            return; // Return to the main menu
        default:
            printf("Invalid option.\n");
        }
    }
}

void login(struct User* users, int numUsers, struct User** currentUser) {
    printf("\n--- Login ---\n");
    char username[50], password[50];
    printf("Enter your username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove the trailing newline character
    printf("Enter your password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove the trailing newline character

    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *currentUser = &users[i];
            printf("Login successful!\n");
            return;
        }
    }
    printf("Invalid username or password.\n");
}

void signUp(struct User* users, int* numUsers) {
    printf("\n--- Sign Up ---\n");
    char name[50], username[50], password[50];
    printf("Enter your name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove the trailing newline character
    printf("Enter a username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove the trailing newline character

    // Check if username already exists

    printf("Enter a password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove the trailing newline character

    strcpy(users[*numUsers].name, name);
    strcpy(users[*numUsers].username, username);
    strcpy(users[*numUsers].password, password);
    (*numUsers)++;
}

int main() {
    struct User users[10]; // Assuming there can be at most 10 users
    int numUsers = 0;
    struct User* currentUser = NULL;
    int choice;

    while (1) {
        printf("1. Login\n");
        printf("2. Sign Up\n");
        printf("3. Quit\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left in the input buffer

        switch (choice) {
        case 1:
            login(users, numUsers, &currentUser);
            if (currentUser != NULL) {
                mainPage(currentUser);
            }
            break;
        case 2:
            signUp(users, &numUsers);
            break;
        case 3:
            printf("Goodbye!\n");
            exit(0); // 프로그램 종료
        default:
            printf("Invalid option.\n");
        }
    }

    return 0;
}
