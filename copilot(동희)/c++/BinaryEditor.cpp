//바이너리 에디터를 만들려고함
//바이너리 에디터는 파일을 열어서 16진수로 보여주고, 수정할 수 있게 해주는 프로그램
//16진수로 보여주고 파일을 고칠 수 있도록 해야됨


//사용법
//1. 파일 열기 (같은 위치에 파일이 있어야함)
//2. 파일 보기
//3. 파일 수정 (오프셋은 문자의 위치를 나타냄 간격은 16바이트)
//4. 파일 저장
//5. 종료

//예시
//1. 파일 열기
//파일 이름을 입력하세요: test.txt
//2. 파일 보기
//파일 크기: 11바이트
//파일 내용:
//00000000 48 65 6c 6c 6f 2c 20 57 6f 72 6c
//3. 파일 수정
//수정할 오프셋을 입력하세요: 0
//수정할 값(16진수)을 입력하세요: 41
//4. 파일 저장
//5. 종료

//1. 파일 열기
//파일 이름을 입력하세요: test.txt
//2. 파일 보기
//파일 크기: 11바이트
//파일 내용:
//00000000 41 65 6c 6c 6f 2c 20 57 6f 72 6c


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>


using namespace std;

//바이너리 에디터 클래스
class BinaryEditor
{
public:
    BinaryEditor();
    ~BinaryEditor();
    void OpenFile(string filename);
    void Display();
    void Edit();
    void Save();
    void Close();
private:
    ifstream m_ifs;
    ofstream m_ofs;
    string m_filename;
    vector<unsigned char> m_buffer;
    int m_filesize;
};

//생성자
BinaryEditor::BinaryEditor()
{
    m_filesize = 0;
}

//소멸자
BinaryEditor::~BinaryEditor()
{
    Close();
}

//파일 열기

void BinaryEditor::OpenFile(string filename)
{
    m_filename = filename;
    m_ifs.open(filename.c_str(), ios::in | ios::binary);
    if (!m_ifs.is_open())
    {
        cout << "파일을 열 수 없습니다." << endl;
        return;
    }

    m_ifs.seekg(0, ios::end);
    m_filesize = m_ifs.tellg();
    m_ifs.seekg(0, ios::beg);

    m_buffer.resize(m_filesize);
    m_ifs.read((char*)&m_buffer[0], m_filesize);
    m_ifs.close();
}

//파일 보여주기
void BinaryEditor::Display()
{
    if (m_filesize == 0)
    {
        cout << "파일이 열리지 않았습니다." << endl;
        return;
    }

    cout << "파일 크기: " << m_filesize << "바이트" << endl;
    cout << "파일 내용:" << endl;

    for (int i = 0; i < m_filesize; i++)
    {
        if (i % 16 == 0)
        {
            cout << endl;
            cout << hex << setw(8) << setfill('0') << i << " ";
        }

        cout << hex << setw(2) << setfill('0') << (int)m_buffer[i] << " ";
    }

    cout << endl;
}

//파일 수정하기
void BinaryEditor::Edit()
{
    if (m_filesize == 0)
    {
        cout << "파일이 열리지 않았습니다." << endl;
        return;
    }

    int offset;
    cout << "수정할 오프셋을 입력하세요: ";
    cin >> offset;

    if (offset < 0 || offset >= m_filesize)
    {
        cout << "잘못된 오프셋입니다." << endl;
        return;
    }

    int value;
    cout << "수정할 값(16진수)을 입력하세요: ";
    cin >> hex >> value;

    m_buffer[offset] = value;
}

//파일 저장하기
void BinaryEditor::Save()
{
    if (m_filesize == 0)
    {
        cout << "파일이 열리지 않았습니다." << endl;
        return;
    }

    m_ofs.open(m_filename.c_str(), ios::out | ios::binary);
    m_ofs.write((char*)&m_buffer[0], m_filesize);
    m_ofs.close();
}

//파일 닫기
void BinaryEditor::Close()
{
    m_ifs.close();
    m_ofs.close();
    m_filesize = 0;
    m_buffer.clear();
}

int main()
{
    BinaryEditor editor;
    string filename;
    int choice;

    while (true)
    {
        cout << "1. 파일 열기" << endl;
        cout << "2. 파일 보기" << endl;
        cout << "3. 파일 수정" << endl;
        cout << "4. 파일 저장" << endl;
        cout << "5. 종료" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "파일 이름을 입력하세요: ";
            cin >> filename;
            editor.OpenFile(filename);
            break;
        case 2:
            editor.Display();
            break;
        case 3:
            editor.Edit();
            break;
        case 4:
            editor.Save();
            break;
        case 5:
            editor.Close();
            return 0;
        default:
            cout << "잘못된 선택입니다." << endl;
            break;
        }
    }

    return 0;
}






