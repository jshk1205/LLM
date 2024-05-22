import random
import time


def introduce():
    print("Welcome to the Memory Test Program.")
    print("This program generates a sequence of numbers and tests if you can remember it.")
    print("Press any key to start.")
    input()


def generate_sequence(length):
    return [random.randint(1, 9) for _ in range(length)]


def display_sequence(sequence):
    print("Now, check the number sequence:")
    print(" ".join(map(str, sequence)))
    time.sleep(5)
    clear_screen()


def clear_screen():
    # This code clears the screen, you may need to adjust it according to your environment.
    print("\n" * 100)


def get_user_sequence():
    return list(map(int, input("Enter the number sequence: ").split()))


def compare_sequences(seq1, seq2):
    return seq1 == seq2


def main():
    introduce()

    while True:
        sequence_length = int(input("Enter the length of the number sequence to generate: "))
        sequence = generate_sequence(sequence_length)
        display_sequence(sequence)

        user_attempt = 1
        while True:
            user_sequence = get_user_sequence()
            if compare_sequences(sequence, user_sequence):
                print(f"Congratulations! You got the correct sequence in {user_attempt} attempts!")
                break
            else:
                print("Incorrect. Try again.")
                user_attempt += 1

        restart = input("Do you want to restart the program? (yes/no): ")
        if restart.lower() != "yes":
            print("Exiting the program. Goodbye!")
            break


if __name__ == "__main__":
    main()
