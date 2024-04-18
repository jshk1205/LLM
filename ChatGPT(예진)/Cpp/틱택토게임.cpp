#include <iostream>
#include <vector>
#include <string>

using namespace std;

void print_board(vector<vector<char>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) {
                cout << "|";
            }
        }
        cout << endl;
        if (i < 2) {
            cout << "-----" << endl;
        }
    }
}

bool check_winner(vector<vector<char>>& board, char player) {
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
    vector<vector<char>> board(3, vector<char>(3, ' '));
    vector<char> players = { 'X', 'O' };
    vector<string> player_names = { "Player 1", "Player 2" };
    int current_player = 0;

    int moves_left = 9;
    while (moves_left > 0) {
        print_board(board);
        cout << endl << player_names[current_player] << "'s turn (" << players[current_player] << ")" << endl;

        int row, col;
        cout << "Enter row number (1-3): ";
        cin >> row;
        cout << "Enter column number (1-3): ";
        cin >> col;
        row--;
        col--;

        if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != ' ') {
            cout << "Invalid move! Try again." << endl;
            continue;
        }

        board[row][col] = players[current_player];
        moves_left--;

        if (check_winner(board, players[current_player])) {
            print_board(board);
            cout << endl << "Congratulations! " << player_names[current_player] << " (" << players[current_player] << ") wins!" << endl;
            return;
        }

        current_player = (current_player + 1) % 2;
    }

    print_board(board);
    cout << endl << "It's a draw!" << endl;
}

int main() {
    tic_tac_toe();
    return 0;
}
