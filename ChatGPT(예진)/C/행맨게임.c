#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char* choose_word() {
    char* word_list[] = { "apple", "banana", "orange", "grape", "melon", "strawberry", "peach" };
    int n = sizeof(word_list) / sizeof(word_list[0]);
    return word_list[rand() % n];
}

void display_word(char* secret_word, char* guessed_letters) {
    int len = strlen(secret_word);
    for (int i = 0; i < len; i++) {
        if (strchr(guessed_letters, secret_word[i]) != NULL) {
            printf("%c", secret_word[i]);
        }
        else {
            printf("_");
        }
    }
    printf("\n");
}

int hangman() {
    srand(time(NULL));
    char* secret_word = choose_word();
    char guessed_letters[26] = "";
    int attempts = 6;

    printf("Welcome to Hangman!\n");
    printf("Try to guess the secret word.\n");

    while (attempts > 0) {
        printf("Attempts left: %d\n", attempts);
        display_word(secret_word, guessed_letters);

        char guess;
        printf("Guess a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (!isalpha(guess)) {
            printf("Please enter a single letter.\n");
            continue;
        }

        if (strchr(guessed_letters, guess) != NULL) {
            printf("You've already guessed that letter.\n");
            continue;
        }

        guessed_letters[strlen(guessed_letters)] = guess;
        guessed_letters[strlen(guessed_letters) + 1] = '\0';

        int found = 0;
        for (int i = 0; i < strlen(secret_word); i++) {
            if (secret_word[i] == guess) {
                found = 1;
                break;
            }
        }

        if (found) {
            printf("Correct!\n");
        }
        else {
            printf("Incorrect!\n");
            attempts--;
        }

        int all_found = 1;
        for (int i = 0; i < strlen(secret_word); i++) {
            if (strchr(guessed_letters, secret_word[i]) == NULL) {
                all_found = 0;
                break;
            }
        }

        if (all_found) {
            printf("Congratulations! You've guessed the word: %s\n", secret_word);
            return 1; // Return 1 to indicate success
        }
    }

    printf("You've run out of attempts! The secret word was: %s\n", secret_word);
    return 0; // Return 0 to indicate failure
}

int main() {
    if (hangman()) {
        printf("You win!\n");
    }
    else {
        printf("Game over!\n");
    }
    return 0;
}
