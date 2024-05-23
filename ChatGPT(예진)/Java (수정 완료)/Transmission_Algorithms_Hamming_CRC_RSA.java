import java.util.Random;

public class Transmission_Algorithms_Hamming_CRC_RSA {
    // Hamming code implementation
    public static String generateHammingCode(String data) {
        int n = data.length();
        int m = 0;
        while (Math.pow(2, m) < n + m + 1) {
            m++;
        }
        char[] hammingCode = new char[n + m];
        int j = 0;
        for (int i = 0; i < n + m; i++) {
            if (i + 1 == Math.pow(2, j)) {
                hammingCode[i] = '0';
                j++;
            } else {
                if (j < n) {
                    hammingCode[i] = data.charAt(j);
                    j++;
                } else {
                    hammingCode[i] = '0';
                }
            }
        }
        for (int k = 0; k < m; k++) {
            int p = (int) Math.pow(2, k) - 1;
            int count = 0;
            for (int l = p; l < n + m; l += 2 * (p + 1)) {
                for (int o = l; o < Math.min(l + p + 1, n + m); o++) {
                    if (hammingCode[o] == '1') {
                        count++;
                    }
                }
            }
            if (count % 2 == 0) {
                hammingCode[p] = '0';
            } else {
                hammingCode[p] = '1';
            }
        }
        return new String(hammingCode);
    }

    // CRC code implementation
    public static String generateCrcCode(String data, String divisor) {
        char[] crc = data.toCharArray();
        int divisorLength = divisor.length();
        for (int i = 0; i < data.length() - divisorLength + 1; i++) {
            if (crc[i] == '1') {
                for (int j = 0; j < divisorLength; j++) {
                    crc[i + j] = (char) (((crc[i + j] - '0') ^ (divisor.charAt(j) - '0')) + '0');
                }
            }
        }
        return new String(crc);
    }

    // RSA encryption algorithm implementation
    public static int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    public static int modInverse(int a, int m) {
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

    public static int[] generateRsaKeyPair(int p, int q) {
        int n = p * q;
        int phi = (p - 1) * (q - 1);
        Random random = new Random();
        int e = random.nextInt(phi - 2) + 2;
        while (gcd(e, phi) != 1) {
            e = random.nextInt(phi - 2) + 2;
        }
        int d = modInverse(e, phi);
        return new int[]{e, n, d};
    }

    public static void main(String[] args) {
        // Example data creation
        String data = "101010";
        String divisor = "1101";
        int p = 61;
        int q = 53;

        // Hamming code result output
        System.out.println("Hamming Code:");
        String hammingCode = generateHammingCode(data);
        System.out.println("Original Data: " + data);
        System.out.println("Hamming Code: " + hammingCode);

        // CRC code result output
        System.out.println("\nCRC Code:");
        String crcCode = generateCrcCode(data + "0".repeat(divisor.length() - 1), divisor);
        System.out.println("Original Data: " + data);
        System.out.println("CRC Code: " + crcCode);

        // RSA encryption algorithm result output
        System.out.println("\nRSA Encryption:");
        int[] keyPair = generateRsaKeyPair(p, q);
        int plaintext = 42;
        int encryptedText = (int) Math.pow(plaintext, keyPair[0]) % keyPair[1];
        System.out.println("Plaintext: " + plaintext);
        System.out.println("Encrypted Text: " + encryptedText);

        // RSA decryption algorithm result output
        System.out.println("\nRSA Decryption:");
        int decryptedText = (int) Math.pow(encryptedText, keyPair[2]) % keyPair[1];
        System.out.println("Decrypted Text: " + decryptedText);
    }
}
