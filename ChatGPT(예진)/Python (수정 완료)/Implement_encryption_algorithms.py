import random

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def mod_inverse(a, m):
    m0, x0, x1 = m, 0, 1

    while a > 1:
        q = a // m
        m, a = a % m, m
        x0, x1 = x1 - q * x0, x0

    if x1 < 0:
        x1 += m0
    return x1

def pow_mod(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp % 2 == 1:
            result = (result * base) % mod
        exp //= 2
        base = (base * base) % mod
    return result

def generate_keypair(p, q):
    n = p * q
    phi = (p - 1) * (q - 1)
    e = random.randint(2, phi - 1)

    while gcd(e, phi) != 1:
        e = random.randint(2, phi - 1)

    d = mod_inverse(e, phi)
    return (e, n), (d, n)

def encrypt(public_key, plaintext):
    e, n = public_key
    cipher = [pow_mod(ord(char), e, n) for char in plaintext]
    return cipher

def decrypt(private_key, ciphertext):
    d, n = private_key
    plain = ''.join(chr(pow_mod(char, d, n)) for char in ciphertext)
    return plain

def main():
    p = 61
    q = 53
    public_key, private_key = generate_keypair(p, q)

    message = input("Enter your message: ")

    encrypted_msg = encrypt(public_key, message)
    print("Encrypted Message:", ' '.join(map(str, encrypted_msg)))

    decrypted_msg = decrypt(private_key, encrypted_msg)
    print("Decrypted Message:", decrypted_msg)

if __name__ == "__main__":
    main()
