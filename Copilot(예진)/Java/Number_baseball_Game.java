// 숫자야구 게임을 개발해 주세요.

import java.util.Scanner;

public class Number_baseball_Game {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("숫자야구 게임을 시작합니다.");
        System.out.println("게임을 시작하려면 1을 입력하세요.");
        int start = sc.nextInt();
        if (start == 1) {
            System.out.println("게임을 시작합니다.");
            int[] answer = new int[3];
            for (int i = 0; i < 3; i++) {
                answer[i] = (int) (Math.random() * 9) + 1;
                for (int j = 0; j < i; j++) {
                    if (answer[i] == answer[j]) {
                        i--;
                        break;
                    }
                }
            }

            int[] guess = new int[3];
            int strike = 0;
            int ball = 0;
            int count = 0;
            while (strike != 3) {
                strike = 0;
                ball = 0;
                System.out.println("세 자리 숫자를 입력하세요.");
                int num = sc.nextInt();
                guess[0] = num / 100;
                guess[1] = (num % 100) / 10;
                guess[2] = num % 10;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (answer[i] == guess[j]) {
                            if (i == j) {
                                strike++;
                            } else {
                                ball++;
                            }
                        }
                    }
                }
                System.out.println("Strike: " + strike + ", Ball: " + ball);
                count++;
            }
            System.out.println("정답입니다. " + count + "번 만에 맞추셨습니다.");
        } else {
            System.out.println("게임을 종료합니다.");
        }
    }
}