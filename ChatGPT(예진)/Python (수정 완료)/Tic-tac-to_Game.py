def print_board(board):
    for row in board:
        print("|".join(row))
        print("-" * 5)


def check_winner(board, player):
    for row in board:
        if all(cell == player for cell in row):
            return True

    for col in range(3):
        if all(board[row][col] == player for row in range(3)):
            return True

    if all(board[i][i] == player for i in range(3)) or all(board[i][2 - i] == player for i in range(3)):
        return True

    return False


def tic_tac_toe():
    board = [[" " for _ in range(3)] for _ in range(3)]
    players = ['X', 'O']
    player_names = ['Player 1', 'Player 2']
    current_player = 0

    moves_left = 9
    while moves_left > 0:
        print_board(board)
        print(f"\n{player_names[current_player]}'s turn ({players[current_player]})")

        row = int(input("Enter row number (1-3): ")) - 1
        col = int(input("Enter column number (1-3): ")) - 1

        if row not in range(3) or col not in range(3) or board[row][col] != " ":
            print("Invalid move! Try again.")
            continue

        board[row][col] = players[current_player]
        moves_left -= 1

        if check_winner(board, players[current_player]):
            print_board(board)
            print(f"\nCongratulations! {player_names[current_player]} ({players[current_player]}) wins!")
            break

        current_player = (current_player + 1) % 2

    else:
        print_board(board)
        print("\nIt's a draw!")


if __name__ == "__main__":
    tic_tac_toe()
