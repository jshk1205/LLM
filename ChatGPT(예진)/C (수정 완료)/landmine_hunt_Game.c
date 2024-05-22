#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10 // Size of the board
#define MINES 10 // Number of mines

char board[SIZE][SIZE]; // Game board
char visible[SIZE][SIZE]; // Board visible to the user

void initialize() {
    int i, j;
    // Initialize the board
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = '0'; // Initialize the board with '0' indicating no mine
            visible[i][j] = '.'; // Initialize the board visible to the user with '.'
        }
    }
    // Randomly place mines
    srand(time(NULL));
    for (i = 0; i < MINES; i++) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        board[x][y] = '*'; // '*' represents a mine
    }
}

void printBoard() {
    int i, j;
    // Print the current board state
    printf("  ");
    for (i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (j = 0; j < SIZE; j++) {
            printf("%c ", visible[i][j]);
        }
        printf("\n");
    }
}

int countAdjacentMines(int x, int y) {
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

void reveal(int x, int y) {
    // Reveal the number of adjacent mines to the user at the selected coordinates
    if (visible[x][y] == '.') {
        int mines = countAdjacentMines(x, y);
        visible[x][y] = mines + '0';
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

int main() {
    initialize();
    printf("Welcome to Minesweeper!\n");
    printf("Enter coordinates to reveal a cell.\n");
    printf("Example: 'x y', where x and y are integers between 0 and %d.\n", SIZE - 1);
    printf("Let's start!\n");

    printBoard();

    int x, y;
    while (1) {
        printf("Enter coordinates (x y): ");
        scanf("%d %d", &x, &y);
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            printf("Invalid coordinates. Please try again.\n");
            continue;
        }
        if (board[x][y] == '*') {
            printf("Game Over! You hit a mine.\n");
            break;
        }
        reveal(x, y);
        printBoard();
    }

    return 0;
}