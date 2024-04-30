#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

// 해밍 코드 구현
std::string generateHammingCode(const std::string& data) {
    int n = data.length();
    int m = 0;
    while (pow(2, m) < n + m + 1) {
        m++;
    }
    std::string hammingCode(n + m, '0');
    int j = 0;
    for (int i = 0; i < n + m; i++) {
        if (i + 1 == pow(2, j)) {
            j++;
        }
        else {
            if (j < n) {  // j가 데이터의 길이를 벗어나지 않도록 확인
                hammingCode[i] = data[j];
                j++;
            }
        }
    }
    for (int i = 0; i < m; i++) {
        int p = pow(2, i) - 1;
        int count = 0;
        for (int j = p; j < n + m; j += 2 * (p + 1)) {
            for (int k = j; k < std::min(j + p + 1, n + m); k++) {
                if (hammingCode[k] == '1') {
                    count++;
                }
            }
        }
        if (count % 2 != 0) {
            hammingCode[p] = '1';
        }
    }
    return hammingCode;
}

// CRC 코드 구현
std::string generateCrcCode(const std::string& data, const std::string& divisor) {
    std::string crc = data;
    int dataLength = data.length();
    int divisorLength = divisor.length();
    for (int i = 0; i < dataLength - divisorLength + 1; i++) {
        if (crc[i] == '1') {
            for (int j = 0; j < divisorLength; j++) {
                crc[i + j] = ((crc[i + j] - '0') ^ (divisor[j] - '0')) + '0';
            }
        }
    }
    return crc;
}

// RSA 암호화 알고리즘 구현
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int a, int m) {
    int m0 = m, x0 = 0, x1 = 1;
    while (a > 1) {
        int q = a / m;
        int temp = m;
        m = a % m;
        a = temp;
        temp = x0;
        x0 = x1 - q * x0;
        x1 = temp;
    }
    return x1 < 0 ? x1 + m0 : x1;
}

std::pair<int, int> generateRsaKeyPair(int p, int q) {
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    std::srand(std::time(0));
    int e = std::rand() % (phi - 2) + 2;
    while (gcd(e, phi) != 1) {
        e = std::rand() % (phi - 2) + 2;
    }
    int d = modInverse(e, phi);
    return std::make_pair(e, d);
}

int main() {
    // 예제 데이터 생성
    std::string data = "101010";
    std::string divisor = "1101";
    int p = 61;
    int q = 53;

    // 해밍 코드 결과 출력
    std::cout << "Hamming Code:" << std::endl;
    std::string hammingCode = generateHammingCode(data);
    std::cout << "Original Data: " << data << std::endl;
    std::cout << "Hamming Code: " << hammingCode << std::endl;

    // CRC 코드 결과 출력
    std::cout << "\nCRC Code:" << std::endl;
    std::string crcCode = generateCrcCode(data + std::string(divisor.length() - 1, '0'), divisor);
    std::cout << "Original Data: " << data << std::endl;
    std::cout << "CRC Code: " << crcCode << std::endl;

    // RSA 암호화 알고리즘 결과 출력
    std::cout << "\nRSA Encryption:" << std::endl;
    auto keyPair = generateRsaKeyPair(p, q);
    int plaintext = 42;
    int encryptedText = static_cast<int>(pow(plaintext, keyPair.first)) % (p * q);
    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Encrypted Text: " << encryptedText << std::endl;

    // RSA 복호화 알고리즘 결과 출력
    std::cout << "\nRSA Decryption:" << std::endl;
    int decryptedText = static_cast<int>(pow(encryptedText, keyPair.second)) % (p * q);
    std::cout << "Decrypted Text: " << decryptedText << std::endl;

    return 0;
}
