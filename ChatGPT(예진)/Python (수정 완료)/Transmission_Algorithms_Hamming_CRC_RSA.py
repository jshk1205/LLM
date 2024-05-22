import random

# 해밍 코드 구현
def generate_hamming_code(data):
    n = len(data)
    m = 0
    while 2 ** m < n + m + 1:
        m += 1
    hamming_code = [None] * (n + m)
    j = 0
    for i in range(n + m):
        if i + 1 == 2 ** j:
            hamming_code[i] = 0
            j += 1
        else:
            if j < n:  # j가 데이터의 길이를 벗어나지 않도록 확인
                hamming_code[i] = int(data[j])
                j += 1
            else:
                hamming_code[i] = 0  # 데이터 길이를 벗어나면 0으로 채움
    for i in range(m):
        p = 2 ** i - 1
        count = 0
        for j in range(p, n + m, 2 * (p + 1)):
            for k in range(j, min(j + p + 1, n + m)):
                if hamming_code[k] == 1:
                    count += 1
        if count % 2 == 0:
            hamming_code[p] = 0
        else:
            hamming_code[p] = 1
    return ''.join(map(str, hamming_code))

# CRC 코드 구현
def generate_crc_code(data, divisor):
    crc = list(data)
    divisor_length = len(divisor)
    for i in range(len(data) - divisor_length + 1):
        if crc[i] == '1':
            for j in range(divisor_length):
                crc[i + j] = str(int(crc[i + j]) ^ int(divisor[j]))
    return ''.join(crc)

# RSA 암호화 알고리즘 구현
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
    return x1 + m0 if x1 < 0 else x1

def generate_rsa_keypair(p, q):
    n = p * q
    phi = (p - 1) * (q - 1)
    e = random.randint(2, phi - 1)
    while gcd(e, phi) != 1:
        e = random.randint(2, phi - 1)
    d = mod_inverse(e, phi)
    return ((e, n), (d, n))

# 예제 데이터 생성
data = "101010"
divisor = "1101"
p = 61
q = 53

# 해밍 코드 결과 출력
print("Hamming Code:")
hamming_code = generate_hamming_code(data)
print("Original Data:", data)
print("Hamming Code:", hamming_code)

# CRC 코드 결과 출력
print("\nCRC Code:")
crc_code = generate_crc_code(data + "0" * (len(divisor) - 1), divisor)
print("Original Data:", data)
print("CRC Code:", crc_code)

# RSA 암호화 알고리즘 결과 출력
print("\nRSA Encryption:")
public_key, private_key = generate_rsa_keypair(p, q)
plaintext = 42
encrypted_text = pow(plaintext, public_key[0], public_key[1])
print("Plaintext:", plaintext)
print("Encrypted Text:", encrypted_text)

# RSA 복호화 알고리즘 결과 출력
print("\nRSA Decryption:")
decrypted_text = pow(encrypted_text, private_key[0], private_key[1])
print("Decrypted Text:", decrypted_text)
