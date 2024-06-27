// 이진 데이터를 직접 입력하고, 입력한 데이터를 해밍 코드, CRC 코드, RSA 암호화를 구현하는 코드를 작성해 주세요.
// 입력 데이터는 0과 1로만 이루어진 이진 데이터로, 0과 1로만 이루어진 데이터를 입력받아야 합니다.
// 해밍 코드는 입력 데이터의 오류를 검출하는 코드이며, CRC 코드는 입력 데이터의 오류를 검출 및 수정하는 코드입니다.
// RSA 암호화는 입력 데이터를 암호화하는 코드입니다.
// 코드를 최소 80줄 이상으로 작성해 주세요.

import java.util.Scanner;

public class Transmission_Algorithms_Hamming_CRC_RSA {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter binary data: ");
        String binaryData = sc.nextLine();
        System.out.println("Binary data: " + binaryData);

        // Hamming Code
        String hammingCode = hammingCode(binaryData);
        System.out.println("Hamming Code: " + hammingCode);

        // CRC Code
        String crcCode = crcCode(binaryData);
        System.out.println("CRC Code: " + crcCode);

        // RSA Encryption
        String rsaEncryptedData = rsaEncryption(binaryData);
        System.out.println("RSA Encrypted Data: " + rsaEncryptedData);
    }

    public static String hammingCode(String binaryData) {
        int r = 0;
        while (Math.pow(2, r) < binaryData.length() + r + 1) {
            r++;
        }

        StringBuilder sb = new StringBuilder();
        int j = 0;
        for (int i = 1; i <= binaryData.length() + r; i++) {
            if ((i & (i - 1)) == 0) {
                sb.append('0');
            } else {
                sb.append(binaryData.charAt(j));
                j++;
            }
        }

        for (int i = 0; i < r; i++) {
            int parity = 0;
            for (int k = 1; k <= binaryData.length() + r; k++) {
                if (((k >> i) & 1) == 1) {
                    parity ^= (sb.charAt(k - 1) - '0');
                }
            }
            sb.setCharAt((int) Math.pow(2, i) - 1, (char) (parity + '0'));
        }

        return sb.toString();
    }

    public static String crcCode(String binaryData) {
        int[] data = new int[binaryData.length() + 16];
        for (int i = 0; i < binaryData.length(); i++) {
            data[i] = binaryData.charAt(i) - '0';
        }

        int[] divisor = {1, 0, 0, 1, 1};
        for (int i = 0; i < 16; i++) {
            data[binaryData
                    .length() + i] = 0;
        }

        for (int i = 0; i < binaryData.length(); i++) {
            if (data[i] == 1) {
                for (int j = 0; j < 5; j++) {
                    data[i + j] ^= divisor[j];
                }
            }
        }

        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < data.length; i++) {
            sb.append(data[i]);
        }
        
        return sb.toString();
    }
    
    public static String rsaEncryption(String binaryData) {
        int p = 61;
        int q = 53;
        int n = p * q;
        int phi = (p - 1) * (q - 1);
        int e = 17;
        int d = 413;

        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < binaryData.length(); i++) {
            int m = binaryData.charAt(i) - '0';
            int c = (int) (Math.pow(m, e) % n);
            sb.append(c);
        }

        return sb.toString();
    }
}