import java.util.Scanner;
import java.util.Random;

public class landmine_hunt_Game {
    private static final int SIZE = 10; // Board size
    private static final int MINES = 10; // Number of mines

    private static char[][] board = new char[SIZE][SIZE]; // Game board
    private static char[][] visible = new char[SIZE][SIZE]; // Board visible to the user

    private static Random random = new Random();

    public static void initialize() {
        // Initialize board
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = '0'; // Initialize board with '0' indicating no mines
                visible[i][j] = '.'; // Initialize visible board with '.'
            }
        }
        // Randomly place mines
        for (int i = 0; i < MINES; i++) {
            int x = random.nextInt(SIZE);
            int y = random.nextInt(SIZE);
            board[x][y] = '*'; // '*' indicates a mine
        }
    }

    public static void printBoard() {
        // Print current board state
        System.out.print("  ");
        for (int i = 0; i < SIZE; i++) {
            System.out.print(i + " ");
        }
        System.out.println();
        for (int i = 0; i < SIZE; i++) {
            System.out.print(i + " ");
            for (int j = 0; j < SIZE; j++) {
                System.out.print(visible[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static int countAdjacentMines(int x, int y) {
        int count = 0;
        // Count adjacent mines
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE && board[i][j] == '*') {
                    count++;
                }
            }
        }
        return count;
    }

    public static void reveal(int x, int y) {
        // Reveal the number of mines around the selected coordinates
        if (visible[x][y] == '.') {
            int mines = countAdjacentMines(x, y);
            visible[x[y] = (char) (mines + '0');
            if (mines == 0) {
                for (int i = x - 1; i <= x + 1; i++) {
                    for (int j = y - 1; j <= y + 1; j++) {
                        if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                            reveal(i, j);
                        }
                    }
                }
            }
        }
    }

    public static void main(String[] args) {
        initialize();
        System.out.println("Welcome to Minesweeper!");
        System.out.println("Enter coordinates to reveal a cell.");
        System.out.println("Example: 'x y', where x and y are integers between 0 and " + (SIZE - 1) + ".");
        System.out.println("Let's start!");

        printBoard();

        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("Enter coordinates (x y): ");
            int x = scanner.nextInt();
            int y = scanner.nextInt();
            if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
                System.out.println("Invalid coordinates. Please try again.");
                continue;
            }
            if (board[x][y] == '*') {
                System.out.println("Game Over! You hit a mine.");
                break;
            }
            reveal(x, y);
            printBoard();
        }
        scanner.close();
    }
}
