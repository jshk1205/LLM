# CHATGPT
"""
파이썬으로 코드를 생성해 주세요.
주제는 기억력 테스트 프로그램입니다.
코드 생성 조건은 다음과 같습니다.
- 실행이 되어야 합니다.
- 한국어로 구성되어 있습니다.
- 최초로 실행한 경우, 사용자에게 프로그램을 소개합니다.
- 사용자로부터 입력 받은 시퀀스의 길이를 기반으로 랜덤한 숫자 시퀀스를 생성합니다.
- 생성된 숫자 시퀀스를 사용자에게 표시합니다.
- 이후 사용자가 시퀀스를 기억할 수 있도록 5초의 시간을 제공합니다.
- 5초의 시간이 흐른 후, 사용자가 화면에 출력된 숫자 시퀀스를 확인하지 못하도록 조치를 취합니다.
- 화면에 숫자 시퀀스가 보여지지 않게 되었다면, 사용자로부터 시퀀스를 입력받습니다.
- 사용자가 입력한 시퀀스와 생성된 시퀀스를 비교하여 정확성을 확인합니다.
- 사용자가 올바른 시퀀스를 입력할 때까지 여러 번 시도할 수 있도록 합니다.
- 올바른 시퀀스를 입력했다면, 입력 횟수를 출력합니다.
- 사용자에게 프로그램 재시작 여부를 묻고, 사용자가 "yes"를 입력할 경우 프로그램을 다시 시작합니다.
"""

import random
import time


def introduce():
    print("안녕하세요! 기억력 테스트 프로그램에 오신 것을 환영합니다.")
    print("이 프로그램은 숫자 시퀀스를 생성하고, 그것을 기억할 수 있는지 테스트합니다.")
    print("시작하려면 아무 키나 눌러주세요.")
    input()


def generate_sequence(length):
    return [random.randint(1, 9) for _ in range(length)]


def display_sequence(sequence):
    print("이제 숫자 시퀀스를 확인하세요:")
    print(" ".join(map(str, sequence)))
    time.sleep(5)
    clear_screen()


def clear_screen():
    # 이 코드는 화면을 지우는 명령어로, 여기서는 예시로 제공합니다.
    # 실제로는 사용자의 환경에 맞는 방법으로 화면을 지울 수 있습니다.
    print("\n" * 100)


def get_user_sequence():
    return list(map(int, input("숫자 시퀀스를 입력하세요: ").split()))


def compare_sequences(seq1, seq2):
    return seq1 == seq2


def main():
    introduce()

    while True:
        sequence_length = int(input("생성할 숫자 시퀀스의 길이를 입력하세요: "))
        sequence = generate_sequence(sequence_length)
        display_sequence(sequence)

        user_attempt = 1
        while True:
            user_sequence = get_user_sequence()
            if compare_sequences(sequence, user_sequence):
                print(f"정확한 시퀀스를 {user_attempt}번 만에 맞추셨습니다!")
                break
            else:
                print("틀렸습니다. 다시 시도해보세요.")
                user_attempt += 1

        restart = input("프로그램을 다시 시작하시겠습니까? (yes/no): ")
        if restart.lower() != "yes":
            print("프로그램을 종료합니다. 안녕히 가십시오!")
            break


if __name__ == "__main__":
    main()
