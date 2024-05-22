#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 10 // Size of the board
#define MINES 10 // Number of mines

char board[SIZE][SIZE]; // Game board
char visible[SIZE][SIZE]; // Board visible to the player

void initialize() {
    int i, j;
    // Initialize the board
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = '0'; // Initialize the board with '0' indicating no mines
            visible[i][j] = '.'; // Initialize the visible board with '.' 
        }
    }
    // Randomly place mines
    std::srand(std::time(nullptr));
    for (i = 0; i < MINES; i++) {
        int x = std::rand() % SIZE;
        int y = std::rand() % SIZE;
        board[x][y] = '*'; // '*' represents a mine
    }
}

void printBoard() {
    int i, j;
    // Print the current board state
    std::cout << "  ";
    for (i = 0; i < SIZE; i++) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    for (i = 0; i < SIZE; i++) {
        std::cout << i << " ";
        for (j = 0; j < SIZE; j++) {
            std::cout << visible[i][j] << " ";
        }
        std::cout << "\n";
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
    // Check the number of mines around the selected coordinates and reveal
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
    std::cout << "Welcome to Minesweeper!\n";
    std::cout << "Enter coordinates to reveal a cell.\n";
    std::cout << "Example: 'x y', where x and y are integers between 0 and " << SIZE - 1 << ".\n";
    std::cout << "Let's start!\n";

    printBoard();

    int x, y;
    while (true) {
        std::cout << "Enter coordinates (x y): ";
        std::cin >> x >> y;
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            std::cout << "Invalid coordinates. Please try again.\n";
            continue;
        }
        if (board[x][y] == '*') {
            std::cout << "Game Over! You hit a mine.\n";
            break;
        }
        reveal(x, y);
        printBoard();
    }

    return 0;
}
