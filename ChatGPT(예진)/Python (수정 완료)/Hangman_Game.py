
import random


def choose_word():
    word_list = ["apple", "banana", "orange", "grape", "melon", "strawberry", "peach"]
    return random.choice(word_list)


def display_word(secret_word, guessed_letters):
    displayed_word = ""
    for letter in secret_word:
        if letter in guessed_letters:
            displayed_word += letter
        else:
            displayed_word += "_"
    return displayed_word


def hangman():
    secret_word = choose_word()
    guessed_letters = []
    attempts = 6

    print("Welcome to Hangman!")
    print("Try to guess the secret word.")
    print(display_word(secret_word, guessed_letters))

    while True:
        guess = input("Guess a letter: ").lower()

        if len(guess) != 1 or not guess.isalpha():
            print("Please enter a single letter.")
            continue

        if guess in guessed_letters:
            print("You've already guessed that letter.")
            continue

        guessed_letters.append(guess)

        if guess in secret_word:
            print("Correct!")
        else:
            print("Incorrect!")
            attempts -= 1

        displayed_word = display_word(secret_word, guessed_letters)
        print(displayed_word)

        if "_" not in displayed_word:
            print("Congratulations! You've guessed the word:", secret_word)
            break

        if attempts == 0:
            print("You've run out of attempts! The secret word was:", secret_word)
            break

        print("Attempts left:", attempts)


if __name__ == "__main__":
    hangman()

