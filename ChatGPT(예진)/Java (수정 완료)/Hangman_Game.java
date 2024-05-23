
import java.util.Scanner;
import java.util.Random;

public class Hangman_Game {
    public static String chooseWord() {
        String[] wordList = {"apple", "banana", "orange", "grape", "melon", "strawberry", "peach"};
        Random random = new Random();
        return wordList[random.nextInt(wordList.length)];
    }

    public static String displayWord(String secretWord, String guessedLetters) {
        StringBuilder displayedWord = new StringBuilder();
        for (int i = 0; i < secretWord.length(); i++) {
            char letter = secretWord.charAt(i);
            if (guessedLetters.contains(String.valueOf(letter))) {
                displayedWord.append(letter);
            } else {
                displayedWord.append("_");
            }
        }
        return displayedWord.toString();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Random random = new Random();

        String secretWord = chooseWord();
        StringBuilder guessedLetters = new StringBuilder();
        int attempts = 6;

        System.out.println("Welcome to Hangman!");
        System.out.println("Try to guess the secret word.");
        System.out.println(displayWord(secretWord, guessedLetters.toString()));

        while (true) {
            System.out.print("Guess a letter: ");
            String guess = scanner.nextLine().toLowerCase();

            if (guess.length() != 1 || !Character.isLetter(guess.charAt(0))) {
                System.out.println("Please enter a single letter.");
                continue;
            }

            if (guessedLetters.toString().contains(guess)) {
                System.out.println("You've already guessed that letter.");
                continue;
            }

            guessedLetters.append(guess);

            if (secretWord.contains(guess)) {
                System.out.println("Correct!");
            } else {
                System.out.println("Incorrect!");
                attempts--;
            }

            String displayedWord = displayWord(secretWord, guessedLetters.toString());
            System.out.println(displayedWord);

            if (!displayedWord.contains("_")) {
                System.out.println("Congratulations! You've guessed the word: " + secretWord);
                break;
            }

            if (attempts == 0) {
                System.out.println("You've run out of attempts! The secret word was: " + secretWord);
                break;
            }

            System.out.println("Attempts left: " + attempts);
        }
    }
}
