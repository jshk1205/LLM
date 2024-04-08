# CHATGPT

# 파이썬으로 코드를 생성해 주세요.
# 주제는 숫자 야구 게임입니다.
# 코드 생성 조건은 다음과 같습니다.
# - 실행이 되어야 합니다.
# - Ball, Strike, out 3가지가 무조건 포함되어 있어야 합니다.
# - 사용자의 1회 입력마다 결과를 출력해야 합니다.
# - 숫자는 5자리입니다.
# - 게임 종료 후 게임 기록을 보여주세요.

import random

def generate_random_number():
    return random.sample(range(1, 10), 5)

def check_guess(random_number, user_guess):
    strikes = 0
    balls = 0
    for i in range(len(random_number)):
        if user_guess[i] == random_number[i]:
            strikes += 1
        elif user_guess[i] in random_number:
            balls += 1
    return strikes, balls

def main():
    print("숫자 야구 게임을 시작합니다!")
    print("게임 규칙: 1~9까지의 서로 다른 5자리 숫자를 맞추세요.")

    random_number = generate_random_number()
    attempts = 0
    game_record = []

    while True:
        user_input = input("5자리 숫자를 입력하세요 (종료하려면 'q' 입력): ")
        if user_input.lower() == 'q':
            break
        if len(user_input) != 5 or not user_input.isdigit():
            print("잘못된 입력입니다. 5자리 숫자를 입력하세요.")
            continue

        user_guess = [int(digit) for digit in user_input]
        attempts += 1
        strikes, balls = check_guess(random_number, user_guess)

        if strikes == 5:
            print(f"축하합니다! {attempts}번째 시도만에 정답을 맞추셨습니다!")
            break

        print(f"{strikes} Strike, {balls} Ball")
        game_record.append((user_input, strikes, balls))

    print("게임 기록:")
    print("Guess\t\tStrikes\tBalls")
    for record in game_record:
        print(f"{record[0]}\t\t{record[1]}\t\t{record[2]}")

if __name__ == "__main__":
    main()
