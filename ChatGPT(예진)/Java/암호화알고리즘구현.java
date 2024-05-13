import java.util.Random;

public class 암호화알고리즘구현 {
    public static int gcd(int a, int b) {
        int temp;
        while (b != 0) {
            temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    public static int modInverse(int a, int m) {
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

    public static int powMod(int base, int exp, int mod) {
        int result = 1;
        for (int i = 0; i < exp; i++) {
            result *= base;
            result %= mod;
        }
        return result;
    }

    public static int[] generateKeypair(int p, int q) {
        int[] keypair = new int[4];
        int n = p * q;
        int phi = (p - 1) * (q - 1);
        Random rand = new Random();
        int e;

        while (true) {
            e = rand.nextInt(phi - 2) + 2;
            if (gcd(e, phi) == 1)
                break;
        }

        int d = modInverse(e, phi);
        keypair[0] = e;
        keypair[1] = n;
        keypair[2] = d;
        keypair[3] = n;

        return keypair;
    }

    public static int[] encrypt(int[] publicKey, String plaintext) {
        int e = publicKey[0];
        int n = publicKey[1];
        int len = plaintext.length();
        int[] cipher = new int[len];
        for (int i = 0; i < len; i++)
            cipher[i] = powMod((int) plaintext.charAt(i), e, n);
        return cipher;
    }

    public static String decrypt(int[] privateKey, int[] ciphertext) {
        int d = privateKey[0];
        int n = privateKey[1];
        StringBuilder plain = new StringBuilder();
        for (int value : ciphertext)
            plain.append((char) powMod(value, d, n));
        return plain.toString();
    }

    public static void main(String[] args) {
        int p = 61;
        int q = 53;
        int[] publicKey, privateKey;
        publicKey = generateKeypair(p, q);
        privateKey = new int[]{publicKey[2], publicKey[3]};

        String message;
        System.out.print("Enter your message: ");
        message = System.console().readLine();

        System.out.println("Original Message: " + message);

        int[] encryptedMsg = encrypt(publicKey, message);
        System.out.print("Encrypted Message: ");
        for (int value : encryptedMsg)
            System.out.print(value + " ");
        System.out.println();

        String decryptedMsg = decrypt(privateKey, encryptedMsg);
        System.out.println("Decrypted Message: " + decryptedMsg);
    }
}
