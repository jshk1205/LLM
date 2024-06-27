// c++로 숫자야구 게임을 개발해 주세요.
// 1. 1~9까지 서로 다른 수로 이루어진 3자리의 수를 생성한다.
// 2. 사용자는 3자리의 수를 입력한다.
// 3. 생성된 수와 입력한 수를 비교하여, 같은 수가 같은 자리에 있으면 strike, 다른 자리에 있으면 ball, 같은 수가 전혀 없으면 out을 출력한다.
// 4. 3 strike가 나오면 게임은 종료된다.
// 5. 게임이 종료된 후, 사용자에게 다시 게임을 할 것인지 물어보고, 다시 게임을 할 경우 1번부터 다시 시작한다.

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    srand((unsigned int)time(0));
    vector<int> answer;
    vector<int> input;
    int strike = 0;
    int ball = 0;
    int out = 0;
    int count = 0;
    int num;
    char again;

    while (1) {
        // 1. 1~9까지 서로 다른 수로 이루어진 3자리의 수를 생성한다.
        while (answer.size() < 3) {
            num = rand() % 9 + 1;
            if (find(answer.begin(), answer.end(), num) == answer.end()) {
                answer.push_back(num);
            }
        }

        // 2. 사용자는 3자리의 수를 입력한다.
        while (1) {
            cout << "3자리 수를 입력하세요: ";
            cin >> num;
            input.push_back(num / 100);
            input.push_back(num / 10 % 10);
            input.push_back(num % 10);

            // 3. 생성된 수와 입력한 수를 비교하여, 같은 수가 같은 자리에 있으면 strike, 다른 자리에 있으면 ball, 같은 수가 전혀 없으면 out을 출력한다.
            for (int i = 0; i < 3; i++) {
                if (answer[i] == input[i]) {
                    strike++;
                }
                else if (find(answer.begin(), answer.end(), input[i]) != answer.end()) {
                    ball++;
                }
                else {
                    out++;
                }
            }

            cout << "strike: " << strike << ", ball: " << ball << ", out: " << out << endl;

            // 4. 3 strike가 나오면 게임은 종료된다.
            if (strike == 3) {
                cout << "3 strike! 게임 종료!" << endl;
                break;
            }

            // strike, ball, out 초기화
            strike = 0;
            ball = 0;
            out = 0;
            input.clear();
        }

        // 5. 게임이 종료된 후, 사용자에게 다시 게임을 할 것인지 물어보고, 다시 게임을 할 경우 1번부터 다
        cout << "다시 하시겠습니까? (y/n): ";
        cin >> again;
        if (again == 'n') {
            break;
        }
        else {
            answer.clear();
        }
    }

    return 0;
}

