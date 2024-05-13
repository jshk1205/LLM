import java.util.Scanner;
import java.util.Random;

public class 지뢰찾기게임 {
    private static final int SIZE = 10; // 보드의 크기
    private static final int MINES = 10; // 지뢰의 개수

    private static char[][] board = new char[SIZE][SIZE]; // 게임 보드
    private static char[][] visible = new char[SIZE][SIZE]; // 사용자에게 보여지는 보드

    private static Random random = new Random();

    public static void initialize() {
        // 보드 초기화
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = '0'; // 보드에는 지뢰가 없음을 표시하는 '0'으로 초기화
                visible[i][j] = '.'; // 사용자에게 보여지는 보드를 '.'으로 초기화
            }
        }
        // 지뢰 랜덤 배치
        for (int i = 0; i < MINES; i++) {
            int x = random.nextInt(SIZE);
            int y = random.nextInt(SIZE);
            board[x][y] = '*'; // '*'은 지뢰를 나타냄
        }
    }

    public static void printBoard() {
        // 현재 보드 상태 출력
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
        // 주변 지뢰 개수 세기
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
        // 사용자가 선택한 좌표 주변의 지뢰 개수 확인 후 보여주기
        if (visible[x][y] == '.') {
            int mines = countAdjacentMines(x, y);
            visible[x][y] = (char) (mines + '0');
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
