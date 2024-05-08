#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cctype>

std::string choose_word() {
    std::string word_list[] = { "apple", "banana", "orange", "grape", "melon", "strawberry", "peach" };
    int n = sizeof(word_list) / sizeof(word_list[0]);
    return word_list[rand() % n];
}

void display_word(const std::string& secret_word, const std::string& guessed_letters) {
    for (char letter : secret_word) {
        if (guessed_letters.find(letter) != std::string::npos) {
            std::cout << letter;
        }
        else {
            std::cout << "_";
        }
    }
    std::cout << std::endl;
}

int hangman() {
    srand(time(NULL));
    std::string secret_word = choose_word();
    std::string guessed_letters = "";
    int attempts = 6;

    std::cout << "Welcome to Hangman!" << std::endl;
    std::cout << "Try to guess the secret word." << std::endl;

    while (attempts > 0) {
        std::cout << "Attempts left: " << attempts << std::endl;
        display_word(secret_word, guessed_letters);

        char guess;
        std::cout << "Guess a letter: ";
        std::cin >> guess;
        guess = std::tolower(guess);

        if (!std::isalpha(guess)) {
            std::cout << "Please enter a single letter." << std::endl;
            continue;
        }

        if (guessed_letters.find(guess) != std::string::npos) {
            std::cout << "You've already guessed that letter." << std::endl;
            continue;
        }

        guessed_letters += guess;

        bool found = false;
        for (char letter : secret_word) {
            if (letter == guess) {
                found = true;
                break;
            }
        }

        if (found) {
            std::cout << "Correct!" << std::endl;
        }
        else {
            std::cout << "Incorrect!" << std::endl;
            attempts--;
        }

        bool all_found = true;
        for (char letter : secret_word) {
            if (guessed_letters.find(letter) == std::string::npos) {
                all_found = false;
                break;
            }
        }

        if (all_found) {
            std::cout << "Congratulations! You've guessed the word: " << secret_word << std::endl;
            return 1; // Return 1 to indicate success
        }
    }

    std::cout << "You've run out of attempts! The secret word was: " << secret_word << std::endl;
    return 0; // Return 0 to indicate failure
}

int main() {
    if (hangman()) {
        std::cout << "You win!" << std::endl;
    }
    else {
        std::cout << "Game over!" << std::endl;
    }
    return 0;
}
