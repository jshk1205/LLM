# 파이썬으로 숫자야구 게임을 개발해 주세요.
# 숫자야구 게임은 1부터 9까지의 서로 다른 숫자로 이루어진 3자리의 수를 상대방이 생각하고, 그 숫자를 맞추는 게임입니다.
# 게임은 다음과 같이 진행됩니다.
# 1. 상대방이 1부터 9까지의 서로 다른 숫자로 이루어진 3자리의 수를 정합니다.
# 2. 사용자는 3자리의 숫자를 입력하고, 그 숫자가 상대방이 정한 숫자와 얼마나 비슷한지를 알려줍니다.
# 3. 만약 숫자의 값과 위치가 모두 일치하면, 그것은 스트라이크입니다.
# 4. 숫자의 값은 일치하지만 위치가 틀렸다면, 그것은 볼입니다.
# 5. 숫자의 값과 위치가 모두 일치하지 않으면, 아웃입니다.
# 6. 3스트라이크가 나오면 게임은 종료됩니다.
# 이 때, 상대방이 정한 숫자를 맞추기 위해 몇 번의 시도가 필요했는지 출력해 주세요.
# 코드를 최소 80줄 이상으로 작성해 주세요.

import random

def generate_number():
    number = []
    while len(number) < 3:
        new_number = random.randint(1, 9)
        if new_number not in number:
            number.append(new_number)
    return number

def get_user_input():
    user_input = input('숫자를 입력하세요: ')
    return list(map(int, user_input))

def check_strike_ball_out(user_input, number):
    strike = 0
    ball = 0
    out = 0
    for i in range(3):
        if user_input[i] == number[i]:
            strike += 1
        elif user_input[i] in number:
            ball += 1
        else:
            out += 1
    return strike, ball, out

def print_result(strike, ball, out):
    print(f'{strike}스트라이크 {ball}볼 {out}아웃')

def main():
    number = generate_number()
    try_count = 0
    while True:
        user_input = get_user_input()
        strike, ball, out = check_strike_ball_out(user_input, number)
        print_result(strike, ball, out)
        try_count += 1
        if strike == 3:
            break
    print(f'{try_count}번 만에 맞췄습니다.')

if __name__ == '__main__':
    main()
