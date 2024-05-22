import random

MAX_NUM_SETS = 1000
MAX_NUMBERS = 6
MAX_NUM = 45

# Function: Generate Lotto Numbers
def generate_lotto_numbers():
    chosen = [0] * MAX_NUM
    numbers = []
    i = 0
    while i < MAX_NUMBERS:
        num = random.randint(1, MAX_NUM)
        if not chosen[num - 1]:
            numbers.append(num)
            chosen[num - 1] = 1
            i += 1
    numbers.sort()
    return numbers

# Function: Check Winning Numbers
def check_winner(user_numbers, winning_numbers):
    num_matched = 0
    for num in user_numbers:
        if num in winning_numbers:
            num_matched += 1

    if num_matched == 6:
        return "1st Prize"
    elif num_matched == 5 and winning_numbers[-1] == user_numbers[-1]:
        return "2nd Prize"
    elif num_matched == 5:
        return "3rd Prize"
    elif num_matched == 4:
        return "4th Prize"
    elif num_matched == 3:
        return "5th Prize"
    else:
        return "No Prize"

# Function: Calculate Prize
def calculate_prize(rank):
    if rank[0] == '1':
        return 1000000000
    elif rank[0] == '2':
        return 50000000
    elif rank[0] == '3':
        return 1500000
    elif rank[0] == '4':
        return 50000
    elif rank[0] == '5':
        return 5000
    else:
        return 0

# Main Function
def main():
    num_sets = int(input("Enter the number of Lotto sets to generate: "))
    user_numbers = list(map(int, input("Choose your numbers (6 numbers, separated by space): ").split()))

    winning_numbers_list = [generate_lotto_numbers() for _ in range(num_sets)]

    # Analyze and print numbers
    print("Most frequently drawn numbers: ", end="")
    most_frequent_numbers = {}
    for winning_numbers in winning_numbers_list:
        for number in winning_numbers:
            most_frequent_numbers[number] = most_frequent_numbers.get(number, 0) + 1
    most_frequent = max(most_frequent_numbers, key=most_frequent_numbers.get)
    print(most_frequent, " (", most_frequent_numbers[most_frequent], "times)")

    for i, winning_numbers in enumerate(winning_numbers_list):
        print(f"\nLotto numbers for {i + 1} set: ", end="")
        for number in winning_numbers:
            print(number, end=" ")
        print()
        rank = check_winner(user_numbers, winning_numbers)
        prize = calculate_prize(rank)
        print(f"Winning Result: {rank}, Prize: {prize} won")

if __name__ == "__main__":
    main()
