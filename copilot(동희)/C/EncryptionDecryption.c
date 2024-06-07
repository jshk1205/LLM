//파일의 암호화 복호화 프로그램을 만들어줘
// 평문을 입력하면 암호화된 문자를 출력해서 보여줘
// 암호화된 문자를 입력하면 평문을 출력해서 보여줘
// 암호화 복호화는 대칭키를 사용해서 진행해줘
// 대칭키는 사용자가 입력한 값을 사용해줘

#include <stdio.h>
#include <string.h>

#define MAX_DATA 100

void encrypt(char *plainText, char *key)
{
    int keyLen = strlen(key);
    for (int i = 0; plainText[i]; i++)
    {
        plainText[i] ^= key[i % keyLen];
    }
    printf("암호화된 문자: ");
    for (int i = 0; plainText[i]; i++)
    {
        printf("%02X", (unsigned char)plainText[i]);
    }
    printf("\n");
}

void decrypt(char *encryptedText, char *key)
{
    int keyLen = strlen(key);
    for (int i = 0; i < strlen(encryptedText); i += 2)
    {
        char hex[3];
        hex[0] = encryptedText[i];
        hex[1] = encryptedText[i + 1];
        hex[2] = 0;
        unsigned char byte = strtol(hex, NULL, 16);
        printf("%c", byte ^ key[(i / 2) % keyLen]);
    }
    printf("\n");
}

int main()
{
    char plainText[MAX_DATA];
    char encryptedText[MAX_DATA * 2 + 1];  // 2배의 공간이 필요합니다.
    char key[MAX_DATA];
    printf("평문을 입력하세요: ");
    fgets(plainText, sizeof(plainText), stdin);
    plainText[strcspn(plainText, "\n")] = 0;
    printf("대칭키를 입력하세요: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;
    encrypt(plainText, key);
    printf("암호화된 문자를 입력하세요: ");
    fgets(encryptedText, sizeof(encryptedText), stdin);
    encryptedText[strcspn(encryptedText, "\n")] = 0;
    printf("대칭키를 입력하세요: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;
    decrypt(encryptedText, key);
    return 0;
}