#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <iomanip>

using namespace std;

// 함수: 로또 번호 생성
vector<int> generateLottoNumbers() {
    vector<int> numbers;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 45);

    while (numbers.size() < 6) {
        int num = dis(gen);
        if (find(numbers.begin(), numbers.end(), num) == numbers.end()) {
            numbers.push_back(num);
        }
    }

    sort(numbers.begin(), numbers.end());
    return numbers;
}

// 함수: 당첨 번호 확인
string checkWinner(vector<int>& userNumbers, vector<int>& winningNumbers) {
    int numMatched = 0;
    for (int num : userNumbers) {
        if (find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
            numMatched++;
        }
    }

    if (numMatched == 6) {
        return "1등";
    }
    else if (numMatched == 5 && winningNumbers[5] == userNumbers[5]) {
        return "2등";
    }
    else if (numMatched == 5) {
        return "3등";
    }
    else if (numMatched == 4) {
        return "4등";
    }
    else if (numMatched == 3) {
        return "5등";
    }
    else {
        return "꽝";
    }
}

// 함수: 당첨금 계산
int calculatePrize(string rank) {
    if (rank == "1등") {
        return 1000000000;
    }
    else if (rank == "2등") {
        return 50000000;
    }
    else if (rank == "3등") {
        return 1500000;
    }
    else if (rank == "4등") {
        return 50000;
    }
    else if (rank == "5등") {
        return 5000;
    }
    else {
        return 0;
    }
}

// 함수: 번호 분석
void analyzeNumbers(vector<vector<int>>& lottoNumbersList) {
    map<int, int> freqCount;
    for (auto& numbers : lottoNumbersList) {
        for (int num : numbers) {
            freqCount[num]++;
        }
    }

    int mostCommonNumber = -1;
    int mostCommonCount = -1;
    for (auto& entry : freqCount) {
        if (entry.second > mostCommonCount) {
            mostCommonNumber = entry.first;
            mostCommonCount = entry.second;
        }
    }

    cout << "가장 많이 나온 번호: " << mostCommonNumber << endl;
}

// 함수: 당첨 확률 계산
void calculateWinningProbabilities(vector<vector<int>>& winningNumbersList) {
    map<vector<int>, int> winningCounts;
    for (auto& numbers : winningNumbersList) {
        sort(numbers.begin(), numbers.end());
        winningCounts[numbers]++;
    }

    int totalGames = winningNumbersList.size();
    for (auto& entry : winningCounts) {
        double probability = (static_cast<double>(entry.second) / totalGames) * 100;
        cout << setw(10) << "당첨번호: ";
        for (int num : entry.first) {
            cout << num << " ";
        }
        cout << "에 당첨될 확률: " << fixed << setprecision(2) << probability << "%" << endl;
    }
}

// 메인 함수
int main() {
    try {
        int numSets;
        cout << "생성할 로또 번호 세트의 수를 입력하세요: ";
        cin >> numSets;

        vector<int> userNumbers;
        cout << "구매할 번호를 선택하세요 (6개의 숫자, 공백으로 구분): ";
        for (int i = 0; i < 6; i++) {
            int num;
            cin >> num;
            userNumbers.push_back(num);
        }

        vector<vector<int>> winningNumbersList;
        for (int i = 0; i < numSets; i++) {
            winningNumbersList.push_back(generateLottoNumbers());
        }

        analyzeNumbers(winningNumbersList);

        for (int i = 0; i < numSets; i++) {
            cout << "\n" << (i + 1) << "번째 로또 번호: ";
            for (int num : winningNumbersList[i]) {
                cout << num << " ";
            }
            cout << endl;
            string rank = checkWinner(userNumbers, winningNumbersList[i]);
            int prize = calculatePrize(rank);
            cout << "당첨 결과: " << rank << ", 당첨금: " << prize << "원" << endl;
        }

        cout << "\n당첨 확률 분석:" << endl;
        calculateWinningProbabilities(winningNumbersList);

    }
    catch (exception& e) {
        cout << "올바른 값을 입력하세요." << endl;
    }

    return 0;
}
