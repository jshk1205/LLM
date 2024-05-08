# CHATGPT
"""
파이썬으로 코드를 생성해 주세요.
주제는 행맨 게임입니다.
코드 생성 조건은 다음과 같습니다.
- 실행이 되어야 합니다.
- 게임을 실행하면, 단어 목록에서 무작위로 단어를 선택합니다. 이 단어는 플레이어가 추측해야 하는 비밀 단어이므로 화면에 출력하지 않습니다.
- 비밀 단어의 각 글자는 밑줄("_")로 표시되며, 사용자는 단어를 추측해야 합니다.
- 사용자는 한 번에 한 글자씩 알파벳을 추측합니다.
- 추측한 글자가 비밀 단어에 포함되어 있다면, 그 글자가 올바른 것으로 인정되어 비밀 단어에 해당 글자가 위치한 곳에 표시됩니다.
- 사용자가 6번 초과로 추측에 실패할 시 게임은 종료됩니다.
- 사용자가 올바르지 않은 글자를 추측하면 시도 횟수가 감소합니다. 올바른 글자를 추측하면 감소하지 않습니다.
- 사용자가 모든 글자를 추측하여 비밀 단어를 맞춘다면 사용자가 승리합니다.
- 사용자의 시도 횟수가 0이 되는 경우 사용자가 패배합니다.
- 게임이 종료되면, 시도 횟수를 화면에 출력합니다.
"""
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

