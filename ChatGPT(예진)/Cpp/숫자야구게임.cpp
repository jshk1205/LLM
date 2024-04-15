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
    std::cout << "숫자 야구 게임을 시작합니다!" << std::endl;
    std::cout << "게임 규칙: 1~9까지의 서로 다른 5자리 숫자를 맞추세요." << std::endl;

    auto random_number = generateRandomNumber();
    int attempts = 0;
    std::vector<std::tuple<std::vector<int>, int, int>> game_record;

    while (true) {
        std::string user_input;
        std::cout << "5자리 숫자를 입력하세요 (종료하려면 'q' 입력): ";
        std::cin >> user_input;

        if (user_input == "q") {
            break;
        }

        if (user_input.size() != 5 || !std::all_of(user_input.begin(), user_input.end(), ::isdigit)) {
            std::cout << "잘못된 입력입니다. 5자리 숫자를 입력하세요." << std::endl;
            continue;
        }

        std::vector<int> user_guess;
        for (char digit : user_input) {
            user_guess.push_back(digit - '0');
        }

        attempts++;
        auto [strikes, balls] = checkGuess(random_number, user_guess);

        if (strikes == 5) {
            std::cout << "축하합니다! " << attempts << "번째 시도만에 정답을 맞추셨습니다!" << std::endl;
            break;
        }

        std::cout << strikes << " Strike, " << balls << " Ball" << std::endl;
        game_record.emplace_back(user_guess, strikes, balls);
    }

    std::cout << "게임 기록:" << std::endl;
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
