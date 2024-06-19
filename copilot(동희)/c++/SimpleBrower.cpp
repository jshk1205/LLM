//간단한 인터넷 브라우저 만들기
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace std;

class Browser
{
public:
    Browser();
    ~Browser();
    void OpenURL(string url);
    void Display();
    void Close();
private:
    string m_url;
    string m_html;
};


Browser::Browser()
{
    m_url = "";
    m_html = "";
}

Browser::~Browser()
{
    Close();
}

void Browser::OpenURL(string url)
{
    m_url = url;
    m_html = "";
    ifstream ifs(url.c_str());
    if (!ifs.is_open())
    {
        cout << "URL을 열 수 없습니다." << endl;
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        m_html += line + "\n";
    }
    ifs.close();
}

void Browser::Display()
{
    cout << m_html;
}

void Browser::Close()
{
    m_url = "";
    m_html = "";
}

int main()
{
    Browser browser;
    browser.OpenURL("http://www.google.com");
    browser.Display();
    return 0;
}
//컴파일 및 실행

