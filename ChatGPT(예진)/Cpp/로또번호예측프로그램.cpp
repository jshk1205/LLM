#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <iomanip>

using namespace std;

// �Լ�: �ζ� ��ȣ ����
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

// �Լ�: ��÷ ��ȣ Ȯ��
string checkWinner(vector<int>& userNumbers, vector<int>& winningNumbers) {
    int numMatched = 0;
    for (int num : userNumbers) {
        if (find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
            numMatched++;
        }
    }

    if (numMatched == 6) {
        return "1��";
    }
    else if (numMatched == 5 && winningNumbers[5] == userNumbers[5]) {
        return "2��";
    }
    else if (numMatched == 5) {
        return "3��";
    }
    else if (numMatched == 4) {
        return "4��";
    }
    else if (numMatched == 3) {
        return "5��";
    }
    else {
        return "��";
    }
}

// �Լ�: ��÷�� ���
int calculatePrize(string rank) {
    if (rank == "1��") {
        return 1000000000;
    }
    else if (rank == "2��") {
        return 50000000;
    }
    else if (rank == "3��") {
        return 1500000;
    }
    else if (rank == "4��") {
        return 50000;
    }
    else if (rank == "5��") {
        return 5000;
    }
    else {
        return 0;
    }
}

// �Լ�: ��ȣ �м�
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

    cout << "���� ���� ���� ��ȣ: " << mostCommonNumber << endl;
}

// �Լ�: ��÷ Ȯ�� ���
void calculateWinningProbabilities(vector<vector<int>>& winningNumbersList) {
    map<vector<int>, int> winningCounts;
    for (auto& numbers : winningNumbersList) {
        sort(numbers.begin(), numbers.end());
        winningCounts[numbers]++;
    }

    int totalGames = winningNumbersList.size();
    for (auto& entry : winningCounts) {
        double probability = (static_cast<double>(entry.second) / totalGames) * 100;
        cout << setw(10) << "��÷��ȣ: ";
        for (int num : entry.first) {
            cout << num << " ";
        }
        cout << "�� ��÷�� Ȯ��: " << fixed << setprecision(2) << probability << "%" << endl;
    }
}

// ���� �Լ�
int main() {
    try {
        int numSets;
        cout << "������ �ζ� ��ȣ ��Ʈ�� ���� �Է��ϼ���: ";
        cin >> numSets;

        vector<int> userNumbers;
        cout << "������ ��ȣ�� �����ϼ��� (6���� ����, �������� ����): ";
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
            cout << "\n" << (i + 1) << "��° �ζ� ��ȣ: ";
            for (int num : winningNumbersList[i]) {
                cout << num << " ";
            }
            cout << endl;
            string rank = checkWinner(userNumbers, winningNumbersList[i]);
            int prize = calculatePrize(rank);
            cout << "��÷ ���: " << rank << ", ��÷��: " << prize << "��" << endl;
        }

        cout << "\n��÷ Ȯ�� �м�:" << endl;
        calculateWinningProbabilities(winningNumbersList);

    }
    catch (exception& e) {
        cout << "�ùٸ� ���� �Է��ϼ���." << endl;
    }

    return 0;
}
