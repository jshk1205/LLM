#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> generateRandomNumber() {
    std::vector<int> numbers;
    for (int i = 1; i <= 9; ++i) {
        numbers.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
    std::vector<int> random_number(numbers.begin(), numbers.begin() + 5);
    return random_number;
}

std::pair<int, int> checkGuess(const std::vector<int>& random_number, const std::vector<int>& user_guess) {
    int strikes = 0;
    int balls = 0;
    for (size_t i = 0; i < random_number.size(); ++i) {
        if (user_guess[i] == random_number[i]) {
            strikes++;
        }
        else if (std::find(random_number.begin(), random_number.end(), user_guess[i]) != random_number.end()) {
            balls++;
        }
    }
    return std::make_pair(strikes, balls);
}

int main() {
    std::cout << "���� �߱� ������ �����մϴ�!" << std::endl;
    std::cout << "���� ��Ģ: 1~9������ ���� �ٸ� 5�ڸ� ���ڸ� ���߼���." << std::endl;

    auto random_number = generateRandomNumber();
    int attempts = 0;
    std::vector<std::tuple<std::vector<int>, int, int>> game_record;

    while (true) {
        std::string user_input;
        std::cout << "5�ڸ� ���ڸ� �Է��ϼ��� (�����Ϸ��� 'q' �Է�): ";
        std::cin >> user_input;

        if (user_input == "q") {
            break;
        }

        if (user_input.size() != 5 || !std::all_of(user_input.begin(), user_input.end(), ::isdigit)) {
            std::cout << "�߸��� �Է��Դϴ�. 5�ڸ� ���ڸ� �Է��ϼ���." << std::endl;
            continue;
        }

        std::vector<int> user_guess;
        for (char digit : user_input) {
            user_guess.push_back(digit - '0');
        }

        attempts++;
        auto [strikes, balls] = checkGuess(random_number, user_guess);

        if (strikes == 5) {
            std::cout << "�����մϴ�! " << attempts << "��° �õ����� ������ ���߼̽��ϴ�!" << std::endl;
            break;
        }

        std::cout << strikes << " Strike, " << balls << " Ball" << std::endl;
        game_record.emplace_back(user_guess, strikes, balls);
    }

    std::cout << "���� ���:" << std::endl;
    std::cout << "Guess\t\tStrikes\tBalls" << std::endl;
    for (const auto& record : game_record) {
        auto [user_input, strikes, balls] = record;
        for (int digit : user_input) {
            std::cout << digit;
        }
        std::cout << "\t\t" << strikes << "\t\t" << balls << std::endl;
    }

    return 0;
}
