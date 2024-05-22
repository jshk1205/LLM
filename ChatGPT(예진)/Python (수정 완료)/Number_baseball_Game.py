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
    print("Let's start the Number Baseball Game!")
    print("Game Rules: Guess a 5-digit number with digits from 1 to 9 without repetition.")

    random_number = generate_random_number()
    attempts = 0
    game_record = []

    while True:
        user_input = input("Enter a 5-digit number (type 'q' to quit): ")
        if user_input.lower() == 'q':
            break
        if len(user_input) != 5 or not user_input.isdigit():
            print("Invalid input. Please enter a 5-digit number.")
            continue

        user_guess = [int(digit) for digit in user_input]
        attempts += 1
        strikes, balls = check_guess(random_number, user_guess)

        if strikes == 5:
            print(f"Congratulations! You guessed the correct answer in {attempts} attempts!")
            break

        print(f"{strikes} Strike, {balls} Ball")
        game_record.append((user_input, strikes, balls))

    print("Game Records:")
    print("Guess\t\tStrikes\tBalls")
    for record in game_record:
        print(f"{record[0]}\t\t{record[1]}\t\t{record[2]}")

if __name__ == "__main__":
    main()
