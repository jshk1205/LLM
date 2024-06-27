// ���� �����͸� ���� �Է��ϰ�, �Է��� �����͸� �ع� �ڵ�, CRC �ڵ�, RSA ��ȣȭ�� �����ϴ� �ڵ带 �ۼ��� �ּ���.
// �Է� �����ʹ� 0�� 1�θ� �̷���� ���� �����ͷ�, 0�� 1�θ� �̷���� �����͸� �Է¹޾ƾ� �մϴ�.
// �ع� �ڵ�� �Է� �������� ������ �����ϴ� �ڵ��̸�, CRC �ڵ�� �Է� �������� ������ ���� �� �����ϴ� �ڵ��Դϴ�.
// RSA ��ȣȭ�� �Է� �����͸� ��ȣȭ�ϴ� �ڵ��Դϴ�.
// c���� �ڵ带 �ۼ��� �ּ���.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// �ع� �ڵ� ���
void hamming(char* data, char* hamming_code) {
    int data_len = strlen(data);
    int r = 0;
    while ((1 << r) < (data_len + r + 1)) {
        r++;
    }
    int total_length = data_len + r;
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
    hamming_code[total_length] = '\0';
}

// CRC �ڵ� ���
void crc(char* data, char* crc_code, const char* divisor) {
    int data_len = strlen(data);
    int div_len = strlen(divisor);
    char* padded_data = (char*)malloc(data_len + div_len);
    strcpy(padded_data, data);
    for (int i = data_len; i < data_len + div_len - 1; i++) {
        padded_data[i] = '0';
    }
    padded_data[data_len + div_len - 1] = '\0';

    for (int i = 0; i < data_len; i++) {
        if (padded_data[i] == '1') {
            for (int j = 0; j < div_len; j++) {
                padded_data[i + j] = (padded_data[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    strncpy(crc_code, padded_data + data_len, div_len - 1);
    crc_code[div_len - 1] = '\0';
    free(padded_data);
}

// RSA ��ȣȭ
unsigned long long bin_to_int(char* binary) {
    unsigned long long num = 0;
    while (*binary) {
        num = (num << 1) | (*binary++ - '0');
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

unsigned long long rsa_encrypt(char* data, unsigned long long e, unsigned long long n) {
    unsigned long long data_int = bin_to_int(data);
    return mod_exp(data_int, e, n);
}

int main() {
    char data[256];
    printf("���� �����͸� �Է��ϼ���: ");
    scanf("%255s", data);

    int data_len = strlen(data);
    for (int i = 0; i < data_len; i++) {
        if (data[i] != '0' && data[i] != '1') {
            printf("0�� 1�θ� �̷���� ���� �����͸� �Է��ϼ���.\n");
            return 1;
        }
    }

    // �ع� �ڵ� ���
    char hamming_code[256];
    hamming(data, hamming_code);
    printf("�ع� �ڵ�: %s\n", hamming_code);

    // CRC �ڵ� ���
    char crc_code[256];
    crc(data, crc_code, "1101");
    printf("CRC �ڵ�: %s\n", crc_code);

    // RSA ��ȣȭ
    // ���� RSA Ű ���
    unsigned long long e = 17; // ���� Ű ����
    unsigned long long n_mod = 3233; // ��ⷯ (���� �� ���)
    unsigned long long encrypted = rsa_encrypt(data, e, n_mod);
    printf("RSA ��ȣȭ: %llu\n", encrypted);

    return 0;
}
