//로그파일을 열어서 로그를 분석하는 프로그램을 작성하시오.
//로그파일의 각 줄은 다음과 같이 구성되어 있다.
//로그시간 로그내용
//로그시간은 24시간제 시:분:초 형식이다.
//로그내용은 공백으로 구분된 단어들로 구성되어 있다.
//로그파일을 읽어서 다음과 같은 정보를 출력하시오.

//1. 로그파일에 포함된 로그의 개수
//2. 로그파일에 포함된 로그의 시간대별 개수
//3. 로그파일에 포함된 로그의 시간대별 개수를 시간순으로 정렬하여 출력
//4. 로그파일에 포함된 로그의 단어별 개수
//5. 로그파일에 포함된 로그의 단어별 개수를 단어순으로 정렬하여 출력
//6. 로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력
//7. 로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력하는데, 개수가 같은 경우에는 단어순으로 정렬하여 출력

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

class LogAnalyzer
{

public:
    LogAnalyzer()
    {
    }
    ~LogAnalyzer()
    {
    }

    void analyzeLog(string fileName)
    {
        ifstream file(fileName);
        string line;
        vector<string> logs;
        map<string, int> logCount;
        map<string, map<string, int>> logTimeCount;
        map<string, map<string, int>> logWordCount;
        while (getline(file, line))
        {
            logs.push_back(line);
            istringstream iss(line);
            string time;
            iss >> time;
            string word;
            while (iss >> word)
            {
                logCount[word]++;
                logTimeCount[time][word]++;
                logWordCount[word][time]++;
            }
        }
        cout << "1. 로그파일에 포함된 로그의 개수" << endl;
        cout << logs.size() << endl;
        cout << "2. 로그파일에 포함된 로그의 시간대별 개수" << endl;
        for (auto& time : logTimeCount)
        {
            cout << time.first << " " << time.second.size() << endl;
        }
        cout << "3. 로그파일에 포함된 로그의 시간대별 개수를 시간순으로 정렬하여 출력" << endl;
        vector<pair<string, int>> timeCount;
        for (auto& time : logTimeCount)
        {
            timeCount.push_back({ time.first, time.second.size() });
        }
        sort(timeCount.begin(), timeCount.end(), [](pair<string, int> a, pair<string, int> b) {
            return a.second < b.second;
            });
        for (auto& time : timeCount)
        {
            cout << time.first << " " << time.second << endl;
        }
        cout << "4. 로그파일에 포함된 로그의 단어별 개수" << endl;
        for (auto& word : logCount)
        {
            cout << word.first << " " << word.second << endl;
        }
        cout << "5. 로그파일에 포함된 로그의 단어별 개수를 단어순으로 정렬하여 출력" << endl;
        vector<pair<string, int>> wordCount;
        for (auto& word
            : logCount)
        {
            wordCount.push_back({ word.first, word.second });
        }
        sort(wordCount.begin(), wordCount.end(), [](pair<string, int> a, pair<string, int> b) {
            return a.first < b.first;
            });
        for (auto& word : wordCount)
        {
            cout << word.first << " " << word.second << endl;
        }
        cout << "6. 로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력" << endl;
        sort(wordCount.begin(), wordCount.end(), [](pair<string, int> a, pair<string, int> b) {
            return a.second < b.second;
            });
        for (auto& word : wordCount)
        {
            cout << word.first << " " << word.second << endl;
        }
        cout << "7. 로그파일에 포함된 로그의 단어별 개수를 개수순으로 정렬하여 출력하는데, 개수가 같은 경우에는 단어순으로 정렬하여 출력" << endl;
        sort(wordCount.begin(), wordCount.end(), [](pair<string, int> a, pair<string, int> b) {
            if (a.second == b.second)
            {
                return a.first < b.first;
            }
            return a.second < b.second;
            });
        for (auto& word : wordCount)
        {
            cout << word.first << " " << word.second << endl;
        }
     
    }
};

int main()
{
    LogAnalyzer logAnalyzer;
    logAnalyzer.analyzeLog("log.txt");
    return 0;
}







