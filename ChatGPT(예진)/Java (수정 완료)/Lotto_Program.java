import java.util.*;

public class Lotto_Program {

    private static final int MAX_NUM_SETS = 1000;
    private static final int MAX_NUMBERS = 6;
    private static final int MAX_NUM = 45;

    // Function: Generate Lotto Numbers
    private static List<Integer> generateLottoNumbers() {
        int[] chosen = new int[MAX_NUM];
        List<Integer> numbers = new ArrayList<>();
        int i = 0;
        Random random = new Random();
        while (i < MAX_NUMBERS) {
            int num = random.nextInt(MAX_NUM) + 1;
            if (chosen[num - 1] == 0) {
                numbers.add(num);
                chosen[num - 1] = 1;
                i++;
            }
        }
        Collections.sort(numbers);
        return numbers;
    }

    // Function: Check Winning Numbers
    private static String checkWinner(List<Integer> userNumbers, List<Integer> winningNumbers) {
        int numMatched = 0;
        for (int num : userNumbers) {
            if (winningNumbers.contains(num)) {
                numMatched++;
            }
        }

        if (numMatched == 6) {
            return "1st Prize";
        } else if (numMatched == 5 && winningNumbers.get(MAX_NUMBERS - 1).equals(userNumbers.get(MAX_NUMBERS - 1))) {
            return "2nd Prize";
        } else if (numMatched == 5) {
            return "3rd Prize";
        } else if (numMatched == 4) {
            return "4th Prize";
        } else if (numMatched == 3) {
            return "5th Prize";
        } else {
            return "No Prize";
        }
    }

    // Function: Calculate Prize
    private static int calculatePrize(String rank) {
        if (rank.startsWith("1")) {
            return 1000000000;
        } else if (rank.startsWith("2")) {
            return 50000000;
        } else if (rank.startsWith("3")) {
            return 1500000;
        } else if (rank.startsWith("4")) {
            return 50000;
        } else if (rank.startsWith("5")) {
            return 5000;
        } else {
            return 0;
        }
    }

    // Main Function
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the number of Lotto sets to generate: ");
        int numSets = scanner.nextInt();

        System.out.print("Choose your numbers (6 numbers, separated by space): ");
        List<Integer> userNumbers = new ArrayList<>();
        for (int i = 0; i < MAX_NUMBERS; i++) {
            userNumbers.add(scanner.nextInt());
        }

        List<List<Integer>> winningNumbersList = new ArrayList<>();
        for (int i = 0; i < numSets; i++) {
            winningNumbersList.add(generateLottoNumbers());
        }

        // Analyze and print numbers
        System.out.print("Most frequently drawn numbers: ");
        Map<Integer, Integer> mostFrequentNumbers = new HashMap<>();
        for (List<Integer> winningNumbers : winningNumbersList) {
            for (int number : winningNumbers) {
                mostFrequentNumbers.put(number, mostFrequentNumbers.getOrDefault(number, 0) + 1);
            }
        }
        int mostFrequent = Collections.max(mostFrequentNumbers.entrySet(), Map.Entry.comparingByValue()).getKey();
        System.out.println(mostFrequent + " (" + mostFrequentNumbers.get(mostFrequent) + " times)");

        for (int i = 0; i < winningNumbersList.size(); i++) {
            System.out.print("\nLotto numbers for " + (i + 1) + " set: ");
            for (int number : winningNumbersList.get(i)) {
                System.out.print(number + " ");
            }
            System.out.println();
            String rank = checkWinner(userNumbers, winningNumbersList.get(i));
            int prize = calculatePrize(rank);
            System.out.println("Winning Result: " + rank + ", Prize: " + prize + " won");
        }
    }
}
