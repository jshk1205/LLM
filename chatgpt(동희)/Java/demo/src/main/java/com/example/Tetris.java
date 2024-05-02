package com.example;
import java.util.Random;
import java.util.Scanner;

public class Tetris {
    private static final int WIDTH = 10;
    private static final int HEIGHT = 20;
    private static final char EMPTY = '.';
    private static final char BLOCK = 'X';

    private char[][] grid;
    private int currentRow, currentCol;
    private char currentShape;
    private Random random;
    private boolean gameOver;

    public Tetris() {
        grid = new char[HEIGHT][WIDTH];
        random = new Random();
        gameOver = false;
        initGrid();
        newShape();
    }

    private void initGrid() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                grid[i][j] = EMPTY;
            }
        }
    }

    private void newShape() {
        currentShape = BLOCK;
        currentRow = 0;
        currentCol = WIDTH / 2;
        if (grid[currentRow][currentCol] != EMPTY) {
            gameOver = true;
        } else {
            grid[currentRow][currentCol] = currentShape;
        }
    }

    private void moveShapeDown() {
        if (currentRow < HEIGHT - 1) {
            grid[currentRow][currentCol] = EMPTY;
            currentRow++;
            if (grid[currentRow][currentCol] == EMPTY) {
                grid[currentRow][currentCol] = currentShape;
            } else {
                currentRow--;
                grid[currentRow][currentCol] = currentShape;
                newShape();
            }
        } else {
            newShape();
        }
    }

    private void moveShapeLeft() {
        if (currentCol > 0 && grid[currentRow][currentCol - 1] == EMPTY) {
            grid[currentRow][currentCol] = EMPTY;
            currentCol--;
            grid[currentRow][currentCol] = currentShape;
        }
    }

    private void moveShapeRight() {
        if (currentCol < WIDTH - 1 && grid[currentRow][currentCol + 1] == EMPTY) {
            grid[currentRow][currentCol] = EMPTY;
            currentCol++;
            grid[currentRow][currentCol] = currentShape;
        }
    }

    private void rotateShape() {
        // Store the current shape temporarily
        char[][] tempShape = new char[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tempShape[i][j] = grid[currentRow + i][currentCol + j];
            }
        }
    
        // Rotate the shape
        for (int i = 0; i < 2; i++) {
            for (int j = i; j < 3 - i; j++) {
                char temp = tempShape[i][j];
                tempShape[i][j] = tempShape[3 - j][i];
                tempShape[3 - j][i] = tempShape[3 - i][3 - j];
                tempShape[3 - i][3 - j] = tempShape[j][3 - i];
                tempShape[j][3 - i] = temp;
            }
        }
    
        // Check if the rotated shape fits within the grid boundaries and is not obstructed by existing blocks
        boolean validRotation = true;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tempShape[i][j] == BLOCK) {
                    if (currentRow + i >= HEIGHT || currentCol + j >= WIDTH || currentCol + j < 0 || grid[currentRow + i][currentCol + j] != EMPTY) {
                        validRotation = false;
                        break;
                    }
                }
            }
        }
    
        // If rotation is valid, update the grid with the rotated shape
        if (validRotation) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    grid[currentRow + i][currentCol + j] = tempShape[i][j];
                }
            }
        }
    }
    

    private void printGrid() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                System.out.print(grid[i][j] + " ");
            }
            System.out.println();
        }
    }

    private void handleInput() {
        Scanner scanner = new Scanner(System.in);
        String input = scanner.nextLine();
        switch (input.toLowerCase()) {
            case "a":
                moveShapeLeft();
                break;
            case "d":
                moveShapeRight();
                break;
            case "s":
                moveShapeDown();
                break;
            case "w":
                rotateShape();
                break;
            case "quit":
                gameOver = true;
                break;
            default:
                break;
        }
    }

    private void gameLoop() {
        while (!gameOver) {
            printGrid();
            handleInput();
        }
    }

    public static void main(String[] args) {
        Tetris tetris = new Tetris();
        tetris.gameLoop();
    }
}
