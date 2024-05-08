package 캡디;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class 숫자야구게임 {
    public static void main(String[] args) {
        System.out.println("숫자 야구 게임을 시작합니다!");
        System.out.println("게임 규칙: 1~9까지의 서로 다른 5자리 숫자를 맞추세요.");

        Random random = new Random();
        List<Integer> randomNumbers = generateRandomNumber(random);
        int attempts = 0;
        List<String[]> gameRecord = new ArrayList<>();

        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("5자리 숫자를 입력하세요 (종료하려면 'q' 입력): ");
            String userInput = scanner.nextLine().trim();
            if (userInput.equalsIgnoreCase("q")) {
                break;
            }
            if (userInput.length() != 5 || !userInput.matches("\\d+")) {
                System.out.println("잘못된 입력입니다. 5자리 숫자를 입력하세요.");
                continue;
            }

            String[] userGuess = userInput.split("");
            attempts++;
            int[] result = checkGuess(randomNumbers, userGuess);

            if (result[0] == 5) {
                System.out.println("축하합니다! " + attempts + "번째 시도만에 정답을 맞추셨습니다!");
                break;
            }

            System.out.println(result[0] + " Strike, " + result[1] + " Ball");
            gameRecord.add(new String[]{userInput, Integer.toString(result[0]), Integer.toString(result[1])});
        }

        System.out.println("게임 기록:");
        System.out.println("Guess\t\tStrikes\tBalls");
        for (String[] record : gameRecord) {
            System.out.println(record[0] + "\t\t" + record[1] + "\t\t" + record[2]);
        }
    }

    public static List<Integer> generateRandomNumber(Random random) {
        List<Integer> numbers = new ArrayList<>();
        while (numbers.size() < 5) {
            int num = random.nextInt(9) + 1;
            if (!numbers.contains(num)) {
                numbers.add(num);
            }
        }
        return numbers;
    }

    public static int[] checkGuess(List<Integer> randomNumbers, String[] userGuess) {
        int strikes = 0;
        int balls = 0;
        for (int i = 0; i < randomNumbers.size(); i++) {
            int randomNumber = randomNumbers.get(i);
            int userNumber = Integer.parseInt(userGuess[i]);
            if (userNumber == randomNumber) {
                strikes++;
            } else if (randomNumbers.contains(userNumber)) {
                balls++;
            }
        }
        return new int[]{strikes, balls};
    }
}
