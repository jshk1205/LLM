#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>

// Hamming Code Implementation
std::string generateHammingCode(const std::string& data) {
    int n = data.length();
    int m = 0;
    while (std::pow(2, m) < n + m + 1) {
        m++;
    }
    std::string hammingCode(n + m, '0');
    int j = 0;
    for (int i = 0; i < n + m; i++) {
        if (i + 1 == std::pow(2, j)) {
            hammingCode[i] = '0';
            j++;
        }
        else {
            if (j < n) {
                hammingCode[i] = data[j];
                j++;
            }
            else {
                hammingCode[i] = '0';  // Fill with 0 if beyond data length
            }
        }
    }
    for (int i = 0; i < m; i++) {
        int p = std::pow(2, i) - 1;
        int count = 0;
        for (int j = p; j < n + m; j += 2 * (p + 1)) {
            for (int k = j; k < (j + p + 1 < n + m ? j + p + 1 : n + m); k++) {
                if (hammingCode[k] == '1') {
                    count++;
                }
            }
        }
        hammingCode[p] = (count % 2 == 0) ? '0' : '1';
    }
    return hammingCode;
}

// CRC Code Implementation
std::string generateCrcCode(const std::string& data, const std::string& divisor) {
    int dataLength = data.length();
    int divisorLength = divisor.length();
    std::string crc = data;
    crc.append(divisorLength - 1, '0');
    for (int i = 0; i < dataLength; i++) {
        if (crc[i] == '1') {
            for (int j = 0; j < divisorLength; j++) {
                crc[i + j] = ((crc[i + j] - '0') ^ (divisor[j] - '0')) + '0';
            }
        }
    }
    return crc.substr(dataLength);
}

// RSA Encryption Algorithm Implementation
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

// Modular Exponentiation Function
int modExp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

std::tuple<int, int, int> generateRsaKeyPair(int p, int q) {
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = rand() % (phi - 2) + 2;
    while (gcd(e, phi) != 1) {
        e = rand() % (phi - 2) + 2;
    }
    int d = modInverse(e, phi);
    return std::make_tuple(e, n, d);
}

int main() {
    // Example Data
    std::string data = "101010";
    std::string divisor = "1101";
    int p = 61;
    int q = 53;

    // Output Hamming Code Results
    std::cout << "Hamming Code:\n";
    std::string hammingCode = generateHammingCode(data);
    std::cout << "Original Data: " << data << "\n";
    std::cout << "Hamming Code: " << hammingCode << "\n";

    // Output CRC Code Results
    std::cout << "\nCRC Code:\n";
    std::string crcCode = generateCrcCode(data + std::string(divisor.length() - 1, '0'), divisor);
    std::cout << "Original Data: " << data << "\n";
    std::cout << "CRC Code: " << crcCode << "\n";

    // Output RSA Encryption Results
    std::cout << "\nRSA Encryption:\n";
    auto [e, n, d] = generateRsaKeyPair(p, q);
    int plaintext = 42;
    int encryptedText = modExp(plaintext, e, n);
    std::cout << "Plaintext: " << plaintext << "\n";
    std::cout << "Encrypted Text: " << encryptedText << "\n";

    // Output RSA Decryption Results
    std::cout << "\nRSA Decryption:\n";
    int decryptedText = modExp(encryptedText, d, n);
    std::cout << "Decrypted Text: " << decryptedText << "\n";

    return 0;
}