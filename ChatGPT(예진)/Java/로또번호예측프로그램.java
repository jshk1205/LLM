import java.util.Scanner;
import java.util.HashSet;
import java.util.Set;
import java.util.Random;
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class 로또번호예측프로그램 {

    // 메서드: 로또 번호 생성
    public static List<Integer> generateLottoNumbers() {
        List<Integer> numbers = new ArrayList<>();
        Random random = new Random();
        while (numbers.size() < 6) {
            int num = random.nextInt(45) + 1;
            if (!numbers.contains(num)) {
                numbers.add(num);
            }
        }
        Collections.sort(numbers);
        return numbers;
    }

    // 메서드: 당첨 번호 확인
    public static String checkWinner(List<Integer> userNumbers, List<Integer> winningNumbers) {
        Set<Integer> matchedNumbers = new HashSet<>(userNumbers);
        matchedNumbers.retainAll(winningNumbers);
        int numMatched = matchedNumbers.size();

        if (numMatched == 6) {
            return "1등";
        } else if (numMatched == 5 && winningNumbers.contains(userNumbers.get(5))) {
            return "2등";
        } else if (numMatched == 5) {
            return "3등";
        } else if (numMatched == 4) {
            return "4등";
        } else if (numMatched == 3) {
            return "5등";
        } else {
            return "꽝";
        }
    }

    // 메서드: 당첨금 계산
    public static int calculatePrize(String rank) {
        switch (rank) {
            case "1등":
                return 1000000000;
            case "2등":
                return 50000000;
            case "3등":
                return 1500000;
            case "4등":
                return 50000;
            case "5등":
                return 5000;
            default:
                return 0;
        }
    }

    // 메서드: 번호 분석
    public static void analyzeNumbers(List<List<Integer>> lottoNumbersList) {
        Map<Integer, Integer> freqCount = new HashMap<>();
        for (List<Integer> numbers : lottoNumbersList) {
            for (int num : numbers) {
                freqCount.put(num, freqCount.getOrDefault(num, 0) + 1);
            }
        }
        int mostCommonNumber = -1;
        int mostCommonCount = -1;
        for (Map.Entry<Integer, Integer> entry : freqCount.entrySet()) {
            if (entry.getValue() > mostCommonCount) {
                mostCommonNumber = entry.getKey();
                mostCommonCount = entry.getValue();
            }
        }
        System.out.println("가장 많이 나온 번호: " + mostCommonNumber);
    }

    // 메서드: 당첨 확률 계산
    public static void calculateWinningProbabilities(List<List<Integer>> winningNumbersList) {
        Map<List<Integer>, Integer> winningCounts = new HashMap<>();
        for (List<Integer> numbers : winningNumbersList) {
            Collections.sort(numbers);
            winningCounts.put(numbers, winningCounts.getOrDefault(numbers, 0) + 1);
        }
        int totalGames = winningNumbersList.size();
        for (Map.Entry<List<Integer>, Integer> entry : winningCounts.entrySet()) {
            double probability = ((double) entry.getValue() / totalGames) * 100;
            System.out.printf("%s에 당첨될 확률: %.2f%%\n", entry.getKey(), probability);
        }
    }

    // 메인 메서드
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        try {
            System.out.print("생성할 로또 번호 세트의 수를 입력하세요: ");
            int numSets = Integer.parseInt(scanner.nextLine());

            System.out.print("구매할 번호를 선택하세요 (6개의 숫자, 공백으로 구분): ");
            String[] numbersInput = scanner.nextLine().split("\\s+");
            List<Integer> userNumbers = new ArrayList<>();
            for (String num : numbersInput) {
                userNumbers.add(Integer.parseInt(num));
            }

            List<List<Integer>> winningNumbersList = new ArrayList<>();
            for (int i = 0; i < numSets; i++) {
                winningNumbersList.add(generateLottoNumbers());
            }
            analyzeNumbers(winningNumbersList);

            for (int i = 0; i < numSets; i++) {
                List<Integer> numbers = winningNumbersList.get(i);
                System.out.printf("\n%d번째 로또 번호: %s\n", (i + 1), numbers);
                String rank = checkWinner(userNumbers, numbers);
                int prize = calculatePrize(rank);
                System.out.printf("당첨 결과: %s, 당첨금: %d원\n", rank, prize);
            }

            System.out.println("\n당첨 확률 분석:");
            calculateWinningProbabilities(winningNumbersList);
        } catch (NumberFormatException e) {
            System.out.println("올바른 값을 입력하세요.");
        }
    }
}
