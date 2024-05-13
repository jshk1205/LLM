#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 10 // 보드의 크기
#define MINES 10 // 지뢰의 개수

char board[SIZE][SIZE]; // 게임 보드
char visible[SIZE][SIZE]; // 사용자에게 보여지는 보드

void initialize() {
    int i, j;
    // 보드 초기화
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = '0'; // 보드에는 지뢰가 없음을 표시하는 '0'으로 초기화
            visible[i][j] = '.'; // 사용자에게 보여지는 보드를 '.'으로 초기화
        }
    }
    // 지뢰 랜덤 배치
    std::srand(std::time(nullptr));
    for (i = 0; i < MINES; i++) {
        int x = std::rand() % SIZE;
        int y = std::rand() % SIZE;
        board[x][y] = '*'; // '*'은 지뢰를 나타냄
    }
}

void printBoard() {
    int i, j;
    // 현재 보드 상태 출력
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

void reveal(int x, int y) {
    // 사용자가 선택한 좌표 주변의 지뢰 개수 확인 후 보여주기
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
