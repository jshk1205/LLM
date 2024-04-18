#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

void print_board(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%c", board[i][j]);
            if (j < 2) {
                printf("|");
            }
        }
        printf("\n");
        if (i < 2) {
            printf("-----\n");
        }
    }
}

bool check_winner(char board[3][3], char player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true;
        }
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }
    return false;
}

void tic_tac_toe() {
    char board[3][3];
    char players[] = { 'X', 'O' };
    char* player_names[] = { "Player 1", "Player 2" };
    int current_player = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

    int moves_left = 9;
    while (moves_left > 0) {
        print_board(board);
        printf("\n%s's turn (%c)\n", player_names[current_player], players[current_player]);

        int row, col;
        printf("Enter row number (1-3): ");
        scanf("%d", &row);
        printf("Enter column number (1-3): ");
        scanf("%d", &col);
        row--;
        col--;

        if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != ' ') {
            printf("Invalid move! Try again.\n");
            continue;
        }

        board[row][col] = players[current_player];
        moves_left--;

        if (check_winner(board, players[current_player])) {
            print_board(board);
            printf("\nCongratulations! %s (%c) wins!\n", player_names[current_player], players[current_player]);
            return;
        }

        current_player = (current_player + 1) % 2;
    }

    print_board(board);
    printf("\nIt's a draw!\n");
}

int main() {
    tic_tac_toe();
    return 0;
}
