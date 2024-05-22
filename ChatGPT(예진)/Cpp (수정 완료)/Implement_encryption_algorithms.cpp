#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>

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

std::vector<int> generate_keypair(int p, int q) {
    std::vector<int> keypair(4);
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    srand(time(NULL));
    int e;

    while (true) {
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

std::vector<int> encrypt(std::vector<int> public_key, const std::string& plaintext) {
    int e = public_key[0];
    int n = public_key[1];
    std::vector<int> cipher;
    for (char c : plaintext)
        cipher.push_back(pow_mod((int)c, e, n));
    return cipher;
}

std::string decrypt(std::vector<int> private_key, const std::vector<int>& ciphertext) {
    int d = private_key[0];
    int n = private_key[1];
    std::string plain;
    for (int c : ciphertext)
        plain += (char)pow_mod(c, d, n);
    return plain;
}

int main() {
    int p = 61;
    int q = 53;
    std::vector<int> public_key, private_key;
    public_key = generate_keypair(p, q);
    private_key.assign(public_key.begin() + 2, public_key.end());

    std::string message;
    std::cout << "Enter your message: ";
    std::getline(std::cin, message);

    std::cout << "Original Message: " << message << std::endl;

    std::vector<int> encrypted_msg = encrypt(public_key, message);
    std::cout << "Encrypted Message: ";
    for (int i : encrypted_msg)
        std::cout << i << " ";
    std::cout << std::endl;

    std::string decrypted_msg = decrypt(private_key, encrypted_msg);
    std::cout << "Decrypted Message: " << decrypted_msg << std::endl;

    return 0;
}
