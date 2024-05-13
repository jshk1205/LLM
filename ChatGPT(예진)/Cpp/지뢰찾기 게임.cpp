#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 10 // ������ ũ��
#define MINES 10 // ������ ����

char board[SIZE][SIZE]; // ���� ����
char visible[SIZE][SIZE]; // ����ڿ��� �������� ����

void initialize() {
    int i, j;
    // ���� �ʱ�ȭ
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = '0'; // ���忡�� ���ڰ� ������ ǥ���ϴ� '0'���� �ʱ�ȭ
            visible[i][j] = '.'; // ����ڿ��� �������� ���带 '.'���� �ʱ�ȭ
        }
    }
    // ���� ���� ��ġ
    std::srand(std::time(nullptr));
    for (i = 0; i < MINES; i++) {
        int x = std::rand() % SIZE;
        int y = std::rand() % SIZE;
        board[x][y] = '*'; // '*'�� ���ڸ� ��Ÿ��
    }
}

void printBoard() {
    int i, j;
    // ���� ���� ���� ���
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
    // �ֺ� ���� ���� ����
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
    // ����ڰ� ������ ��ǥ �ֺ��� ���� ���� Ȯ�� �� �����ֱ�
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
