//테트리스를 만들려고해
//테트리스는 4개의 블록으로 이루어져있어
//블록은 4개의 정사각형으로 이루어져있어
//블록은 7가지가 있어
//블럭은 아래로 떨어져야돼
//블록은 회전할 수 있어
//블록은 아래로 이동할 수 있어
//블록은 좌우로 이동할 수 있어
//블록은 아래로 이동할 수 없을 때 더이상 움직일 수 없어
//블록은 아래로 이동할 수 없을 때 블록을 고정시켜야해
//블록은 고정되면 새로운 블록이 나와
//블록은 고정되면 가로줄이 꽉 차있으면 가로줄이 사라져
//블록은 고정되면 가로줄이 꽉 차있으면 가로줄 위에 있는 블록들이 아래로 내려와
//블록은 고정되면 가로줄이 꽉 차있으면 가로줄 위에 있는 블록들이 아래로 내려올 때 점수를 얻어
//BLOCKS 배열이 수정 가능해야되어야해
//테트리스는 10x20 크기의 보드에서 플레이해
//블럭은 하나만 나와야하며 다음 블럭은 미리 보여야해
//블럭은 랜덤으로 나와야해
//블럭은 4x4 크기의 배열로 표현해야해
//블럭은 1과 0으로 이루어져있어
//블럭은 1은 블럭이고 0은 빈칸이야
//블럭은 보드에 그려질 때는 1이 있는 부분만 그려져야해
//블럭은 보드에 그려질 때는 블럭의 왼쪽 상단이 기준이야
//블럭은 보드의 가장 위에서 시작해야해
//블럭은 보드의 가장 위에서 시작할 때 블럭이 보드 밖으로 나가면 안돼
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 안돼
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 블럭이 고정되어야해
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 새로운 블럭이 나와야해
//블럭은 보드의 가장 위에서 시작할 때 블럭이 이미 채워진 부분에 닿으면 가로줄이 꽉 차있으면 가로줄이 사라져야해
//블럭이 지나가면서 블럭이 채워지면 안돼
//자바로 만들어야해


import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Random;
import javax.swing.Timer;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class Tetris extends JFrame implements KeyListener {
    private static final long serialVersionUID = 1L;
    private static final int WIDTH = 10;
    private static final int HEIGHT = 20;
    private static final int BLOCK_SIZE = 30;
    private static final int BOARD_X = 50;
    private static final int BOARD_Y = 50;
    private static final int BOARD_WIDTH = WIDTH * BLOCK_SIZE;
    private static final int BOARD_HEIGHT = HEIGHT * BLOCK_SIZE;
    private static final int[][][] BLOCKS = {
            {
                    { 0, 0, 0, 0 },
                    { 1, 1, 1, 1 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            },
            {
                    { 1, 0, 0, 0 },
                    { 1, 1, 1, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            },
            {
                    { 0, 0, 1, 0 },
                    { 1, 1, 1, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            },
            {
                    { 1, 1, 0, 0 },
                    { 0, 1, 1, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            },
            {
                    { 0, 1, 1, 0 },
                    { 1, 1, 0, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            },
            {
                    { 1, 1, 0, 0 },
                    { 1, 1, 0, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            },
            {
                    { 1, 1, 0, 0 },
                    { 0, 1, 1, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 }
            }
    };
    private int[][] board = new int[HEIGHT][WIDTH];
    private int[][] block = new int[4][4];
    private int blockX, blockY;
    private int blockType;
    private int nextBlockType;
    private Random random = new Random();
    private boolean isGameOver = false;
    private int score = 0;

    public Tetris() {
        setTitle("Tetris");
        setSize(BOARD_WIDTH + 300, BOARD_HEIGHT + 100);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setResizable(false);
        addKeyListener(this);
        newBlock();
        newBlock();
        setVisible(true);
        newBlock();
        setVisible(true);

        // 타이머 생성 및 시작
        int delay = 1000; // 1초마다 블록을 아래로 이동
        new Timer(delay, e -> moveDown()).start();

    }

    public void moveDown() {
        if (!isCollision(block, blockX, blockY + 1)) {
            blockY++; // Y 좌표를 증가시켜 블록을 아래로 이동
        } else {
            fixBlockToBoard(); // 블록을 보드에 고정하고 새 블록 생성
            newBlock();
        }
        repaint(); // 화면을 다시 그림
    }

    private void fixBlockToBoard() {
        // 블록을 보드에 고정하는 로직을 구현합니다.
        // 이 메소드는 블록이 다른 블록이나 바닥에 닿았을 때 호출되어야 합니다.
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block[i][j] == 1) {
                    board[blockY + i][blockX + j] = 1;
                }
            }
        }
        checkLines(); // 완성된 라인이 있는지 확인하고 제거
    }

    private boolean isCollision(int[][] block, int x, int y) {
        // 블록이 보드의 경계나 다른 블록과 충돌하는지 확인하는 로직을 구현합니다.
        // 이 메소드는 블록을 이동하거나 회전시킬 때 호출되어야 합니다.
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block[i][j] == 1) {
                    if (y + i >= HEIGHT || x + j < 0 || x + j >= WIDTH || board[y + i][x + j] == 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    private void checkLines() {
        int completeLines = 0;

         for (int row = HEIGHT - 1; row >= 0; row--) {
        boolean lineComplete = true;

        for (int col = 0; col < WIDTH; col++) {
            if (board[row][col] == 0) {
                lineComplete = false; // 빈 칸이 있으면, 라인이 완성되지 않음
                break;
            }
        }

        if (lineComplete) {
            completeLines++;
            for (int moveRow = row; moveRow > 0; moveRow--) {
                for (int col = 0; col < WIDTH; col++) {
                    board[moveRow][col] = board[moveRow - 1][col]; // 한 줄씩 아래로 이동
                }
            }
            for (int col = 0; col < WIDTH; col++) {
                board[0][col] = 0; // 가장 위의 줄을 비움
            }
            row++; // 한 줄이 내려왔으므로, 같은 줄을 다시 확인
        }
    }
    }

    public void newBlock() {
        blockType = nextBlockType;
        nextBlockType = random.nextInt(BLOCKS.length);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                block[i][j] = BLOCKS[blockType][i][j];
            }
        }
        blockX = WIDTH / 2 - 2;
        blockY = 0;
        if (isCollision(block, blockX, blockY)) {
            isGameOver = true;
        }
    }

    public void rotateBlock() {
        int[][] temp = new int[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[i][j] = block[i][j];
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                block[i][j] = temp[3 - j][i];
            }
        }
        if (isCollision(block, blockX, blockY)) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    block[i][j] = temp[i][j];
                }
            }
        }
    }

    public void moveBlock(int dx, int dy) {
        if (!isCollision(block, blockX + dx, blockY + dy)) {
            blockX += dx;
            blockY += dy;
        }
    }

    public void fixBlock() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block[i][j] == 1) {
                    board[blockY + i][blockX + j] = 1;
                }
            }
        }
        int lines = 0;
        for (int i = 0; i < 4; i++) {
            if (blockY + i >= HEIGHT) {
                continue;
            }
            boolean isFull = true;
            for (int j = 0; j < WIDTH; j++) {
                if (board[blockY + i][j] == 0) {
                    isFull = false;
                    break;
                }
            }
            if (isFull) {
                lines++;
                for (int k = blockY + i; k > 0; k--) {
                    for (int j = 0; j < WIDTH; j++) {
                        board[k][j] = board[k - 1][j];
                    }
                }
            }
        }
        score += lines * lines;
        newBlock();
    }
    
    public void paint(Graphics g) {
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, getWidth(), getHeight());
        g.setColor(Color.BLACK);
        g.drawRect(BOARD_X, BOARD_Y, BOARD_WIDTH, BOARD_HEIGHT);
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (board[i][j] == 1) {
                    g.fillRect(BOARD_X + j * BLOCK_SIZE, BOARD_Y + i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block[i][j] == 1) {
                    g.fillRect(BOARD_X + (blockX + j) * BLOCK_SIZE, BOARD_Y + (blockY + i) * BLOCK_SIZE, BLOCK_SIZE,
                            BLOCK_SIZE);
                }
            }
        }
        g.drawString("Score: " + score, BOARD_X, BOARD_Y + BOARD_HEIGHT + 20);
        g.drawString("Next Block", BOARD_X + BOARD_WIDTH + 20, BOARD_Y + 20);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (BLOCKS[nextBlockType][i][j] == 1) {
                    g.fillRect(BOARD_X + BOARD_WIDTH + 20 + j * BLOCK_SIZE, BOARD_Y + 40 + i * BLOCK_SIZE, BLOCK_SIZE,
                            BLOCK_SIZE);
                }
            }
        }
        if (isGameOver) {
            g.drawString("Game Over", BOARD_X + BOARD_WIDTH / 2 - 30, BOARD_Y + BOARD_HEIGHT / 2);
        }
    }

    public void keyPressed(KeyEvent e) {
        if (isGameOver) {
            return;
        }
        switch (e.getKeyCode()) {
            case KeyEvent.VK_UP:
                rotateBlock();
                break;
            case KeyEvent.VK_DOWN:
                moveBlock(0, 1);
                break;
            case KeyEvent.VK_LEFT:
                moveBlock(-1, 0);
                break;
            case KeyEvent.VK_RIGHT:
                moveBlock(1, 0);
                break;
        }
        repaint();
    }

    public void keyReleased(KeyEvent e) {
        if (isGameOver) {
            return;
        }
        if (e.getKeyCode() == KeyEvent.VK_SPACE) {
            while (!isCollision(block, blockX, blockY + 1)) {
                blockY++;
            }
            fixBlock();
        }
        repaint();
    }

    public void keyTyped(KeyEvent e) {

    }

    public static void main(String[] args) {
        new Tetris();
    }

}
