// c++�� ���ھ߱� ������ ������ �ּ���.
// 1. 1~9���� ���� �ٸ� ���� �̷���� 3�ڸ��� ���� �����Ѵ�.
// 2. ����ڴ� 3�ڸ��� ���� �Է��Ѵ�.
// 3. ������ ���� �Է��� ���� ���Ͽ�, ���� ���� ���� �ڸ��� ������ strike, �ٸ� �ڸ��� ������ ball, ���� ���� ���� ������ out�� ����Ѵ�.
// 4. 3 strike�� ������ ������ ����ȴ�.
// 5. ������ ����� ��, ����ڿ��� �ٽ� ������ �� ������ �����, �ٽ� ������ �� ��� 1������ �ٽ� �����Ѵ�.

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
        // 1. 1~9���� ���� �ٸ� ���� �̷���� 3�ڸ��� ���� �����Ѵ�.
        while (answer.size() < 3) {
            num = rand() % 9 + 1;
            if (find(answer.begin(), answer.end(), num) == answer.end()) {
                answer.push_back(num);
            }
        }

        // 2. ����ڴ� 3�ڸ��� ���� �Է��Ѵ�.
        while (1) {
            cout << "3�ڸ� ���� �Է��ϼ���: ";
            cin >> num;
            input.push_back(num / 100);
            input.push_back(num / 10 % 10);
            input.push_back(num % 10);

            // 3. ������ ���� �Է��� ���� ���Ͽ�, ���� ���� ���� �ڸ��� ������ strike, �ٸ� �ڸ��� ������ ball, ���� ���� ���� ������ out�� ����Ѵ�.
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

            // 4. 3 strike�� ������ ������ ����ȴ�.
            if (strike == 3) {
                cout << "3 strike! ���� ����!" << endl;
                break;
            }

            // strike, ball, out �ʱ�ȭ
            strike = 0;
            ball = 0;
            out = 0;
            input.clear();
        }

        // 5. ������ ����� ��, ����ڿ��� �ٽ� ������ �� ������ �����, �ٽ� ������ �� ��� 1������ ��
        cout << "�ٽ� �Ͻðڽ��ϱ�? (y/n): ";
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

