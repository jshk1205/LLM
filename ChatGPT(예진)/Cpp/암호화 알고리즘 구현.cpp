#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstring>

using namespace std;

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
    for (int i = 0; i < exp; i++) {
        result *= base;
        result %= mod;
    }
    return result;
}

int* generate_keypair(int p, int q) {
    int* keypair = new int[4];
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

int* encrypt(int* public_key, char* plaintext) {
    int e = public_key[0];
    int n = public_key[1];
    int len = strlen(plaintext);
    int* cipher = new int[len];
    for (int i = 0; i < len; i++)
        cipher[i] = pow_mod((int)plaintext[i], e, n);
    return cipher;
}

char* decrypt(int* private_key, int* ciphertext, int len) {
    int d = private_key[0];
    int n = private_key[1];
    char* plain = new char[len];
    for (int i = 0; i < len; i++)
        plain[i] = (char)pow_mod(ciphertext[i], d, n);
    return plain;
}

int main() {
    int p = 61;
    int q = 53;
    int* public_key, * private_key;
    public_key = generate_keypair(p, q);
    private_key = public_key + 2;

    char message[100];
    cout << "Enter your message: ";
    cin.getline(message, 100);

    cout << "Original Message: " << message << endl;

    int* encrypted_msg = encrypt(public_key, message);
    cout << "Encrypted Message: ";
    for (int i = 0; i < sizeof(encrypted_msg); i++)
        cout << encrypted_msg[i] << " ";
    cout << endl;

    char* decrypted_msg = decrypt(private_key, encrypted_msg, sizeof(encrypted_msg));
    cout << "Decrypted Message: " << decrypted_msg << endl;

    delete[] public_key;
    delete[] encrypted_msg;
    delete[] decrypted_msg;

    return 0;
}
