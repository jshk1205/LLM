#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// 해밍 코드 구현
char* generateHammingCode(const char* data) {
    int n = strlen(data);
    int m = 0;
    while (pow(2, m) < n + m + 1) {
        m++;
    }
    char* hammingCode = (char*)malloc((n + m + 1) * sizeof(char));
    int j = 0;
    for (int i = 0; i < n + m; i++) {
        if (i + 1 == pow(2, j)) {
            hammingCode[i] = '0';
            j++;
        }
        else {
            if (j < n) {  // j가 데이터의 길이를 벗어나지 않도록 확인
                hammingCode[i] = data[j];
                j++;
            }
            else {
                hammingCode[i] = '0';  // 데이터 길이를 벗어나면 0으로 채움
            }
        }
    }
    for (int i = 0; i < m; i++) {
        int p = pow(2, i) - 1;
        int count = 0;
        for (int j = p; j < n + m; j += 2 * (p + 1)) {
            for (int k = j; k < (j + p + 1 < n + m ? j + p + 1 : n + m); k++) {
                if (hammingCode[k] == '1') {
                    count++;
                }
            }
        }
        if (count % 2 == 0) {
            hammingCode[p] = '0';
        }
        else {
            hammingCode[p] = '1';
        }
    }
    hammingCode[n + m] = '\0';
    return hammingCode;
}

// CRC 코드 구현
char* generateCrcCode(const char* data, const char* divisor) {
    int dataLength = strlen(data);
    int divisorLength = strlen(divisor);
    char* crc = (char*)malloc((dataLength + divisorLength) * sizeof(char));
    strcpy(crc, data);
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

// 모듈러 지수 계산 함수
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

int* generateRsaKeyPair(int p, int q) {
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = rand() % (phi - 2) + 2;
    while (gcd(e, phi) != 1) {
        e = rand() % (phi - 2) + 2;
    }
    int d = modInverse(e, phi);
    int* keyPair = (int*)malloc(3 * sizeof(int));
    keyPair[0] = e;
    keyPair[1] = n;
    keyPair[2] = d;
    return keyPair;
}

int main() {
    // 예제 데이터 생성
    const char* data = "101010";
    const char* divisor = "1101";
    int p = 61;
    int q = 53;

    // 해밍 코드 결과 출력
    printf("Hamming Code:\n");
    char* hammingCode = generateHammingCode(data);
    printf("Original Data: %s\n", data);
    printf("Hamming Code: %s\n", hammingCode);
    free(hammingCode);

    // CRC 코드 결과 출력
    printf("\nCRC Code:\n");
    char* crcCode = generateCrcCode(strcat(strdup(data), "0"), divisor);
    printf("Original Data: %s\n", data);
    printf("CRC Code: %s\n", crcCode);
    free(crcCode);

    // RSA 암호화 알고리즘 결과 출력
    printf("\nRSA Encryption:\n");
    int* keyPair = generateRsaKeyPair(p, q);
    int plaintext = 42;
    int encryptedText = modExp(plaintext, keyPair[0], keyPair[1]);
    printf("Plaintext: %d\n", plaintext);
    printf("Encrypted Text: %d\n", encryptedText);

    // RSA 복호화 알고리즘 결과 출력
    printf("\nRSA Decryption:\n");
    int decryptedText = modExp(encryptedText, keyPair[2], keyPair[1]);
    printf("Decrypted Text: %d\n", decryptedText);

    free(keyPair);

    return 0;
}
