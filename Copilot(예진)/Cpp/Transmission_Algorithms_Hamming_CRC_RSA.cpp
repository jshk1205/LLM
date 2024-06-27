// 이진 데이터를 직접 입력하고, 입력한 데이터를 해밍 코드, CRC 코드, RSA 암호화를 구현하는 코드를 작성해 주세요.
// 입력 데이터는 0과 1로만 이루어진 이진 데이터로, 0과 1로만 이루어진 데이터를 입력받아야 합니다.
// 해밍 코드는 입력 데이터의 오류를 검출하는 코드이며, CRC 코드는 입력 데이터의 오류를 검출 및 수정하는 코드입니다.
// RSA 암호화는 입력 데이터를 암호화하는 코드입니다.
// 코드를 최소 80줄 이상으로 작성해 주세요.
// c++로 코드를 작성해 주세요.

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// 해밍 코드 계산
void hamming(const std::string& data, std::string& hamming_code) {
    int data_len = data.length();
    int r = 0;
    while ((1 << r) < (data_len + r + 1)) {
        r++;
    }
    int total_length = data_len + r;
    hamming_code.resize(total_length, '0');
    int j = 0;

    // Initialize hamming_code with data bits and 0 for parity bits
    for (int i = 1; i <= total_length; i++) {
        if ((i & (i - 1)) == 0) {
            hamming_code[i - 1] = '0';
        }
        else {
            hamming_code[i - 1] = data[j++];
        }
    }

    // Calculate parity bits
    for (int i = 0; i < r; i++) {
        int parity = 0;
        for (int k = 1; k <= total_length; k++) {
            if (k & (1 << i)) {
                if (hamming_code[k - 1] == '1') {
                    parity = !parity;
                }
            }
        }
        hamming_code[(1 << i) - 1] = parity ? '1' : '0';
    }
}

// CRC 코드 계산
void crc(const std::string& data, std::string& crc_code, const std::string& divisor) {
    int data_len = data.length();
    int div_len = divisor.length();
    std::string padded_data = data + std::string(div_len - 1, '0');

    for (int i = 0; i < data_len; i++) {
        if (padded_data[i] == '1') {
            for (int j = 0; j < div_len; j++) {
                padded_data[i + j] = (padded_data[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    crc_code = padded_data.substr(data_len, div_len - 1);
}

// RSA 암호화
unsigned long long bin_to_int(const std::string& binary) {
    unsigned long long num = 0;
    for (char bit : binary) {
        num = (num << 1) | (bit - '0');
    }
    return num;
}

unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

unsigned long long rsa_encrypt(const std::string& data, unsigned long long e, unsigned long long n) {
    unsigned long long data_int = bin_to_int(data);
    return mod_exp(data_int, e, n);
}

int main() {
    std::string data;
    std::cout << "이진 데이터를 입력하세요: ";
    std::cin >> data;

    int data_len = data.length();
    for (char c : data) {
        if (c != '0' && c != '1') {
            std::cout << "0과 1로만 이루어진 이진 데이터를 입력하세요." << std::endl;
            return 1;
        }
    }

    // 해밍 코드 계산
    std::string hamming_code;
    hamming(data, hamming_code);
    std::cout << "해밍 코드: " << hamming_code << std::endl;

    // CRC 코드 계산
    std::string crc_code;
    crc(data, crc_code, "1101");
    std::cout << "CRC 코드: " << crc_code << std::endl;

    // RSA 암호화
    // 작은 RSA 키 사용
    unsigned long long e = 17; // 공개 키 지수
    unsigned long long n_mod = 3233; // 모듈러 (작은 수 사용)
    unsigned long long encrypted = rsa_encrypt(data, e, n_mod);
    std::cout << "RSA 암호화: " << encrypted << std::endl;

    return 0;
}
