// 바이너리에디터를 만들려고해
// 바이너리에디터는 파일을 열어서 16진수로 출력이되어야해 수정할수있게해
// 파일을 열면 파일의 크기와 파일의 16진수로된 내용을 보여줘야해
// 파일을 열때 고정경로가 아닌 사용자가 직접 파일을 선택할수있게해
// 파일을 수정하면 파일의 크기와 파일의 16진수로된 내용이 수정되어야해
// 파일을 저장하면 수정된 내용이 파일에 저장되어야해
// 파일을 닫으면 파일이 닫혀야해
// 파일을 열때 파일이 없으면 파일이 없다고 알려줘야해
// 파일을 열때 파일이 없으면 파일을 생성할수있게해
// LPWSTR, LPCWSTR, std::streamoff에 대한 형식을 잘 맞춰야돼


//현재 수정중입니다
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <commdlg.h>
#include <tchar.h>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>

using namespace std;
class BinaryEditor
{
public:
    BinaryEditor()
    {
        this->fileName = L"";
        this->fileSize = 0;
        this->fileData = nullptr;
    }
    ~BinaryEditor()
    {
        if (fileData != nullptr)
        {
            delete[] fileData;
        }
    }
    void openFile()
    {
        OPENFILENAME ofn;
        TCHAR szFile[260];
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        if (GetOpenFileName(&ofn) == TRUE)
        {
            this->fileName = szFile;
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            std::string narrowFileName = converter.to_bytes(this->fileName); // 변환된 부분
            ifstream file(narrowFileName, ios::binary | ios::ate);
            if (file.is_open())
            {
                this->fileSize = file.tellg();
                file.seekg(0, ios::beg);
                this->fileData = new char[this->fileSize];
                file.read(this->fileData, this->fileSize);
                file.close(); // 파일을 닫습니다.

                // 파일의 내용을 16진수로 변환하여 문자열에 저장합니다.
                std::ostringstream oss;
                for (std::streamoff i = 0; i < this->fileSize; ++i)
                {
                    oss << std::hex << std::setw(2) << std::setfill('0') << (0xff & static_cast<unsigned>(this->fileData[i])) << ' ';
                }
                this->hexData = oss.str();
            }
        }
    }
    void saveHexFile(const std::string &fileName)
    {
        ofstream file(fileName);
        if (file.is_open())
        {
            file << this->hexData;
            file.close();
        }
    }
    void editFile()
    {
        if (this->fileData == nullptr)
        {
            cout << "파일이 없습니다" << endl;
            return;
        }
        while (true)
        {
            cout << "1. 파일크기" << endl;
            cout << "2. 파일내용" << endl;
            cout << "3. 저장" << endl;
            cout << "4. 닫기" << endl;
            int menu;
            cin >> menu;
            if (menu == 1)
            {
                cout << "파일크기: " << this->fileSize << endl;
            }
            else if (menu == 2)
            {
                for (int i = 0; i < this->fileSize; i++)
                {
                    cout << hex << setw(2) << setfill('0') << (int)(unsigned char)this->fileData[i] << " ";
                    if (i % 16 == 15)
                    {
                        cout << endl;
                    }
                }
                cout << endl;
            }
            else if (menu == 3)
            {
                saveHexFile("test.txt");
            }
            else if (menu == 4)
            {
                break;
            }
        }
    }

private:
    wstring fileName;
    std::streamoff fileSize;
    char *fileData; // 수정된 부분
    std::string hexData;
};

int main()
{
    BinaryEditor binaryEditor;
    while (true)
    {
        cout << "1. 파일열기" << endl;
        cout << "2. 파일수정" << endl;
        cout << "3. 종료" << endl;
        int menu;
        cin >> menu;
        if (menu == 1)
        {
            binaryEditor.openFile();
        }
        else if (menu == 2)
        {
            binaryEditor.editFile();
        }
        else if (menu == 3)
        {
            break;
        }
    }
    return 0;
}
