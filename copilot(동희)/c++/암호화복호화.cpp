//파일의 암호화 복호화 프로그램을 만들어줘
// 평문을 입력하면 암호화된 문자를 출력해서 보여줘
// 암호화된 문자를 입력하면 평문을 출력해서 보여줘
// 암호화 복호화는 대칭키를 사용해서 진행해줘

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include <tchar.h>
#include <wincrypt.h>
#include <sstream>

using namespace std;    

class Cryptography
{
public:
    Cryptography()
    {
        this->hProv = 0;
        this->hKey = 0;
    }
    ~Cryptography()
    {
        if (hKey != 0)
        {
            CryptDestroyKey(hKey);
        }
        if (hProv != 0)
        {
            CryptReleaseContext(hProv, 0);
        }
    }
    void init()
    {
        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        {
            if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
            {
                return;
            }
        }
    }
    void generateKey()
    {
        if (!CryptGenKey(hProv, CALG_RC4, CRYPT_EXPORTABLE, &hKey))
        {
            return;
        }
    }
    string encrypt(string plainText)
    {
        DWORD dwDataLen = plainText.length();
        DWORD dwBufLen = dwDataLen;
        if (!CryptEncrypt(hKey, 0, TRUE, 0, NULL, &dwBufLen, dwDataLen))
        {
            return "";
        }
        vector<BYTE> buffer(dwBufLen);
        memcpy(&buffer[0], plainText.c_str(), dwDataLen);
        if (!CryptEncrypt(hKey, 0, TRUE, 0, &buffer[0], &dwDataLen, dwBufLen))
        {
            return "";
        }
        stringstream ss;
        for (int i = 0; i < dwDataLen; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)buffer[i];
        }
        return ss.str();
    }
    string decrypt(string cipherText)
    {
        vector<BYTE> buffer(cipherText.length() / 2);
        for (int i = 0; i < buffer.size(); i++)
        {
            istringstream iss(cipherText.substr(i * 2, 2));
            int value;
            iss >> hex >> value;
            buffer[i] = value;
        }
        DWORD dwDataLen = buffer.size();
        if (!CryptDecrypt(hKey, 0, TRUE, 0, &buffer[0], &dwDataLen))
        {
            return "";
        }
        return string((char *)&buffer[0],
                        dwDataLen); 
    }

private:
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;
};  

int main()
{
    Cryptography cryptography;
    cryptography.init();
    cryptography.generateKey();
    string plainText = "Hello, World!";
    string cipherText = cryptography.encrypt(plainText);
    cout << "암호화된 문자: " << cipherText << endl;
    string decryptedText = cryptography.decrypt(cipherText);
    cout << "복호화된 문자: " << decryptedText << endl;
    return 0;
}   


