import random
import tkinter as tk
from tkinter import messagebox

class Minesweeper:
    def __init__(self, width, height, num_mines):
        self.width = width
        self.height = height
        self.num_mines = num_mines
        self.board = self.create_board()
        self.revealed = [[False] * width for _ in range(height)]
        self.plant_mines()
        self.calculate_adjacent_mines()

    def create_board(self):
        return [[0] * self.width for _ in range(self.height)]

    def plant_mines(self):
        mines_planted = 0
        while mines_planted < self.num_mines:
            row = random.randint(0, self.height - 1)
            col = random.randint(0, self.width - 1)
            if self.board[row][col] != -1:
                self.board[row][col] = -1
                mines_planted += 1

    def calculate_adjacent_mines(self):
        for row in range(self.height):
            for col in range(self.width):
                if self.board[row][col] != -1:
                    count = 0
                    for i in range(max(0, row - 1), min(self.height, row + 2)):
                        for j in range(max(0, col - 1), min(self.width, col + 2)):
                            if self.board[i][j] == -1:
                                count += 1
                    self.board[row][col] = count

    def reveal(self, row, col):
        if self.board[row][col] == -1:
            return False
        self.revealed[row][col] = True
        if self.board[row][col] == 0:
            for i in range(max(0, row - 1), min(self.height, row + 2)):
                for j in range(max(0, col - 1), min(self.width, col + 2)):
                    if not self.revealed[i][j]:
                        self.reveal(i, j)
        return True

    def is_won(self):
        for row in range(self.height):
            for col in range(self.width):
                if not self.revealed[row][col] and self.board[row][col] != -1:
                    return False
        return True

class MinesweeperGUI:
    def __init__(self, width, height, num_mines):
        self.game = Minesweeper(width, height, num_mines)
        self.window = tk.Tk()
        self.window.title("Minesweeper")
        self.buttons = self.create_buttons()

    def create_buttons(self):
        buttons = []
        for row in range(self.game.height):
            button_row = []
            for col in range(self.game.width):
                button = tk.Button(self.window, width=2, height=1, command=lambda r=row, c=col: self.on_click(r, c))
                button.grid(row=row, column=col)
                button_row.append(button)
            buttons.append(button_row)
        return buttons

    def on_click(self, row, col):
        if not self.game.reveal(row, col):
            self.buttons[row][col].config(text="*", bg="red")
            messagebox.showinfo("Game Over", "You stepped on a mine!")
            self.window.quit()
        else:
            self.update_buttons()
            if self.game.is_won():
                messagebox.showinfo("Congratulations", "You won the game!")
                self.window.quit()

    def update_buttons(self):
        for row in range(self.game.height):
            for col in range(self.game.width):
                if self.game.revealed[row][col]:
                    if self.game.board[row][col] == 0:
                        self.buttons[row][col].config(text="", bg="white")
                    else:
                        self.buttons[row][col].config(text=str(self.game.board[row][col]), bg="white")

    def run(self):
        self.window.mainloop()

def play_game():
    width = 10
    height = 10
    num_mines = 10

    gui = MinesweeperGUI(width, height, num_mines)
    gui.run()

play_game()