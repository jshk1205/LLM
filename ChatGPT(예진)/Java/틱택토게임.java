import java.util.Scanner;

public class 틱택토게임 {

    public static void printBoard(String[][] board) {
        for (String[] row : board) {
            System.out.println(String.join("|", row));
            System.out.println("-".repeat(5));
        }
    }

    public static boolean checkWinner(String[][] board, String player) {
        for (String[] row : board) {
            boolean win = true;
            for (String cell : row) {
                if (!cell.equals(player)) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        for (int col = 0; col < 3; col++) {
            boolean win = true;
            for (int row = 0; row < 3; row++) {
                if (!board[row][col].equals(player)) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        if (board[0][0].equals(player) && board[1][1].equals(player) && board[2][2].equals(player)) return true;
        if (board[0][2].equals(player) && board[1][1].equals(player) && board[2][0].equals(player)) return true;

        return false;
    }

    public static void ticTacToe() {
        String[][] board = new String[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = " ";
            }
        }

        String[] players = {"X", "O"};
        String[] playerNames = {"Player 1", "Player 2"};
        int currentPlayer = 0;

        int movesLeft = 9;
        Scanner scanner = new Scanner(System.in);
        while (movesLeft > 0) {
            printBoard(board);
            System.out.println("\n" + playerNames[currentPlayer] + "'s turn (" + players[currentPlayer] + ")");

            System.out.print("Enter row number (1-3): ");
            int row = scanner.nextInt() - 1;
            System.out.print("Enter column number (1-3): ");
            int col = scanner.nextInt() - 1;

            if (row < 0 || row >= 3 || col < 0 || col >= 3 || !board[row][col].equals(" ")) {
                System.out.println("Invalid move! Try again.");
                continue;
            }

            board[row][col] = players[currentPlayer];
            movesLeft--;

            if (checkWinner(board, players[currentPlayer])) {
                printBoard(board);
                System.out.println("\nCongratulations! " + playerNames[currentPlayer] + " (" + players[currentPlayer] + ") wins!");
                break;
            }

            currentPlayer = (currentPlayer + 1) % 2;
        }

        if (movesLeft == 0) {
            printBoard(board);
            System.out.println("\nIt's a draw!");
        }
    }

    public static void main(String[] args) {
        ticTacToe();
    }
}
