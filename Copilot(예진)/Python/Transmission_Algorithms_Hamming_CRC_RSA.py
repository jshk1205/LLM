# 이진 데이터를 직접 입력하고, 입력한 데이터를 해밍 코드, CRC 코드, RSA 암호화를 구현하는 c언어 코드를 작성해 주세요.
# 입력 데이터는 0과 1로만 이루어진 이진 데이터로, 0과 1로만 이루어진 데이터를 입력받아야 합니다.
# 해밍 코드는 입력 데이터의 오류를 검출하는 코드이며, CRC 코드는 입력 데이터의 오류를 검출 및 수정하는 코드입니다.
# RSA 암호화는 입력 데이터를 암호화하는 코드입니다.
# 코드를 최소 80줄 이상으로 작성해 주세요.

def hamming(data):
    def calculate_parity(bits, power):
        return sum(bits[i] for i in range(len(bits)) if i & (1 << power) != 0) % 2

    n = len(data)
    r = 0
    while (2 ** r) < (n + r + 1):
        r += 1

    hamming_code = [0] * (n + r)
    j = 0
    for i in range(1, len(hamming_code) + 1):
        if (i & (i - 1)) == 0:
            continue
        hamming_code[i - 1] = int(data[j])
        j += 1

    for i in range(r):
        hamming_code[(2 ** i) - 1] = calculate_parity(hamming_code, i)

    return hamming_code

def crc(data, divisor='1101'):
    n = len(data)
    m = len(divisor)
    data = data + '0' * (m - 1)
    data = list(map(int, data))
    divisor = list(map(int, divisor))

    for i in range(n):
        if data[i] == 1:
            for j in range(m):
                data[i + j] ^= divisor[j]

    return data[-(m - 1):]

def rsa_encrypt(data, e, n):
    def bin_to_int(binary):
        return int(''.join(map(str, binary)), 2)

    data_int = bin_to_int(data)
    encrypted = pow(data_int, e, n)
    return encrypted

# 이진 데이터를 입력받음
data = input('이진 데이터를 입력하세요: ')
# 입력 데이터의 길이를 구함
n = len(data)
# 입력 데이터가 0과 1로만 이루어져 있는지 확인
if not set(data).issubset({'0', '1'}):
    print('0과 1로만 이루어진 이진 데이터를 입력하세요.')
else:
    # 해밍 코드를 계산
    hamming_code = hamming(data)
    print(f'해밍 코드: {"".join(map(str, hamming_code))}')
    # CRC 코드를 계산
    crc_code = crc(data)
    print(f'CRC 코드: {"".join(map(str, crc_code))}')
    # RSA 암호화
    e = 65537
    n = 9516311845790656153499716760847001433441357
    encrypted = rsa_encrypt(data, e, n)
    print(f'RSA 암호화: {encrypted}')

