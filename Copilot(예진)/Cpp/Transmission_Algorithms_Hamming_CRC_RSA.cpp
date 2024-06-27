// ���� �����͸� ���� �Է��ϰ�, �Է��� �����͸� �ع� �ڵ�, CRC �ڵ�, RSA ��ȣȭ�� �����ϴ� �ڵ带 �ۼ��� �ּ���.
// �Է� �����ʹ� 0�� 1�θ� �̷���� ���� �����ͷ�, 0�� 1�θ� �̷���� �����͸� �Է¹޾ƾ� �մϴ�.
// �ع� �ڵ�� �Է� �������� ������ �����ϴ� �ڵ��̸�, CRC �ڵ�� �Է� �������� ������ ���� �� �����ϴ� �ڵ��Դϴ�.
// RSA ��ȣȭ�� �Է� �����͸� ��ȣȭ�ϴ� �ڵ��Դϴ�.
// �ڵ带 �ּ� 80�� �̻����� �ۼ��� �ּ���.
// c++�� �ڵ带 �ۼ��� �ּ���.

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// �ع� �ڵ� ���
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

// CRC �ڵ� ���
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

// RSA ��ȣȭ
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
    std::cout << "���� �����͸� �Է��ϼ���: ";
    std::cin >> data;

    int data_len = data.length();
    for (char c : data) {
        if (c != '0' && c != '1') {
            std::cout << "0�� 1�θ� �̷���� ���� �����͸� �Է��ϼ���." << std::endl;
            return 1;
        }
    }

    // �ع� �ڵ� ���
    std::string hamming_code;
    hamming(data, hamming_code);
    std::cout << "�ع� �ڵ�: " << hamming_code << std::endl;

    // CRC �ڵ� ���
    std::string crc_code;
    crc(data, crc_code, "1101");
    std::cout << "CRC �ڵ�: " << crc_code << std::endl;

    // RSA ��ȣȭ
    // ���� RSA Ű ���
    unsigned long long e = 17; // ���� Ű ����
    unsigned long long n_mod = 3233; // ��ⷯ (���� �� ���)
    unsigned long long encrypted = rsa_encrypt(data, e, n_mod);
    std::cout << "RSA ��ȣȭ: " << encrypted << std::endl;

    return 0;
}
