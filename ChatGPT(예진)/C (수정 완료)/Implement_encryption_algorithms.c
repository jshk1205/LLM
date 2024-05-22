#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}

int pow_mod(int base, int exp, int mod) {
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

int* generate_keypair(int p, int q) {
    int* keypair = (int*)malloc(4 * sizeof(int));
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    srand(time(NULL));
    int e;

    while (1) {
        e = rand() % (phi - 2) + 2;
        if (gcd(e, phi) == 1)
            break;
    }

    int d = mod_inverse(e, phi);
    keypair[0] = e;
    keypair[1] = n;
    keypair[2] = d;
    keypair[3] = n;

    return keypair;
}

int* encrypt(int* public_key, char* plaintext, int* len) {
    int e = public_key[0];
    int n = public_key[1];
    *len = 0;
    while (plaintext[*len] != '\0')
        (*len)++;
    int* cipher = (int*)malloc(*len * sizeof(int));
    for (int i = 0; i < *len; i++)
        cipher[i] = pow_mod((int)plaintext[i], e, n);
    return cipher;
}

char* decrypt(int* private_key, int* ciphertext, int len) {
    int d = private_key[0];
    int n = private_key[1];
    char* plain = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++)
        plain[i] = (char)pow_mod(ciphertext[i], d, n);
    plain[len] = '\0'; // Null-terminate the string
    return plain;
}

int main() {
    int p = 61;
    int q = 53;
    int* public_key, * private_key;
    public_key = generate_keypair(p, q);
    private_key = public_key + 2;

    char message[100];
    printf("Enter your message: ");
    fgets(message, 100, stdin);

    // Remove newline character if present
    size_t len = strlen(message);
    if (len > 0 && message[len - 1] == '\n') {
        message[len - 1] = '\0';
    }

    printf("Original Message: %s\n", message);

    int encrypted_len;
    int* encrypted_msg = encrypt(public_key, message, &encrypted_len);
    printf("Encrypted Message: ");
    for (int i = 0; i < encrypted_len; i++)
        printf("%d ", encrypted_msg[i]);
    printf("\n");

    char* decrypted_msg = decrypt(private_key, encrypted_msg, encrypted_len);
    printf("Decrypted Message: %s\n", decrypted_msg);

    free(public_key);
    free(encrypted_msg);
    free(decrypted_msg);

    return 0;
}
