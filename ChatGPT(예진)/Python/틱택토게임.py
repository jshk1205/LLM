# CHATGPT
"""
파이썬으로 코드를 생성해 주세요.
주제는 틱택토 게임입니다.
코드 생성 조건은 다음과 같습니다.
- 실행이 되어야 합니다.
- 이 게임은 3x3 크기의 보드에서 두 플레이어 간에 번갈아가며 'X'와 'O'를 차례대로 표시하여 진행됩니다.
- 플레이어는 1에서 3 사이의 숫자를 두 번 입력받아 보드의 위치를 선정합니다.
- 숫자의 값이 적절하지 않을 경우 해당 플레이어에게 경고 메세지를 출력하고 다시 보드를 선택할 수 있도록 합니다.
- 게임이 진행되는 동안, 각 플레이어는 자신의 차례마다 보드의 빈 곳 중 하나를 선택하여 표시합니다.
- 가로, 세로, 대각선 방향으로 동일한 기호가 3개 연속으로 나열되면 해당 플레이어가 승리합니다.
- 게임이 모든 칸이 채워지면 무승부로 종료됩니다.
- 게임 종료 후 게임 기록을 보여주세요.
"""


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
