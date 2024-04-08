import random
from collections import Counter

# 함수: 로또 번호 생성
def generate_lotto_numbers():
    return sorted(random.sample(range(1, 46), 6))

# 함수: 당첨 번호 확인
def check_winner(user_numbers, winning_numbers):
    matched_numbers = set(user_numbers) & set(winning_numbers)
    num_matched = len(matched_numbers)

    if num_matched == 6:
        return "1등"
    elif num_matched == 5 and winning_numbers[5] in user_numbers:
        return "2등"
    elif num_matched == 5:
        return "3등"
    elif num_matched == 4:
        return "4등"
    elif num_matched == 3:
        return "5등"
    else:
        return "꽝"

# 함수: 당첨금 계산
def calculate_prize(rank):
    if rank == "1등":
        return 1000000000
    elif rank == "2등":
        return 50000000
    elif rank == "3등":
        return 1500000
    elif rank == "4등":
        return 50000
    elif rank == "5등":
        return 5000
    else:
        return 0

# 함수: 번호 분석
def analyze_numbers(lotto_numbers_list):
    all_numbers = [num for sublist in lotto_numbers_list for num in sublist]
    freq_count = Counter(all_numbers)
    most_common_number, most_common_count = freq_count.most_common(1)[0]
    print("가장 많이 나온 번호:", most_common_number)

# 함수: 당첨 확률 계산
def calculate_winning_probabilities(winning_numbers_list):
    winning_counts = Counter(tuple(sorted(numbers)) for numbers in winning_numbers_list)
    total_games = sum(winning_counts.values())
    for rank, count in winning_counts.most_common():
        probability = (count / total_games) * 100
        print(f" {rank}에 당첨될 확률: {probability:.2f}%")

# 메인 함수
def main():
    try:
        num_sets = int(input("생성할 로또 번호 세트의 수를 입력하세요: "))
        user_numbers = [int(num) for num in input("구매할 번호를 선택하세요 (6개의 숫자, 공백으로 구분): ").split()]

        winning_numbers_list = [generate_lotto_numbers() for _ in range(num_sets)]
        analyze_numbers(winning_numbers_list)

        for idx, numbers in enumerate(winning_numbers_list, start=1):
            print(f"\n{idx}번째 로또 번호: {numbers}")
            rank = check_winner(user_numbers, numbers)
            prize = calculate_prize(rank)
            print(f"당첨 결과: {rank}, 당첨금: {prize}원")

        print("\n당첨 확률 분석:")
        calculate_winning_probabilities(winning_numbers_list)

    except ValueError:
        print("올바른 값을 입력하세요.")

if __name__ == "__main__":
    main()