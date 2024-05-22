package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class BrickBreakerGame extends JFrame implements KeyListener, ActionListener {
    private Timer timer;
    private int delay = 8;
    private int playerX = 310;
    private int ballPosX = 120;
    private int ballPosY = 350;
    private int ballXDir = -1;
    private int ballYDir = -2;
    private int score = 0;
    private int bricksCount = 21;
    private int stage = 1;
    private MapGenerator map;

    public BrickBreakerGame() {
        map = new MapGenerator(3, 7);

        addKeyListener(this);
        setFocusable(true);
        setFocusTraversalKeysEnabled(false);
        timer = new Timer(delay, this);
        timer.start();
    }

    public void paint(Graphics g) {
        // Create off-screen image for double buffering
        Image offScreenImage = createImage(getWidth(), getHeight());
        Graphics offScreenGraphics = offScreenImage.getGraphics();
        // Draw on off-screen image
        paintOffScreen(offScreenGraphics);
        // Draw off-screen image to the screen
        g.drawImage(offScreenImage, 0, 30, this);
    }

    public void paintOffScreen(Graphics g) {
        // Draw background
        g.setColor(Color.black);
        g.fillRect(1, 31, 692, 592);
    
        // Draw bricks
        map.draw((Graphics2D) g);
    
        // Draw boundaries
        g.setColor(Color.yellow);
        g.fillRect(0, 30, 3, 592);
        g.fillRect(0, 30, 692, 3);
        g.fillRect(691, 30, 3, 592);
    
        // Game over
        if (bricksCount <= 0) {
            timer.stop();
            stage++;
            if (stage <= 3) {
                bricksCount = 21;
                map = new MapGenerator(stage + 2, 7);
                ballPosX = 120;
                ballPosY = 350;
                ballXDir = -1;
                ballYDir = -2;
                g.setColor(Color.red);
                g.setFont(new Font("serif", Font.BOLD, 30));
                g.drawString("Moving to the next stage!", 190, 300);
                startNextStage();
            } else {
                g.setColor(Color.red);
                g.setFont(new Font("serif", Font.BOLD, 30));
                g.drawString("Game won! Final score: " + score, 190, 300);
            }
        }
    
        if (ballPosY > 570) {
            timer.stop();
            g.setColor(Color.red);
            g.setFont(new Font("serif", Font.BOLD, 30));
            g.drawString("Failure! Final score: " + score, 190, 300);
        }
    
        // Display score
        g.setColor(Color.white);
        g.setFont(new Font("serif", Font.BOLD, 25));
        g.drawString("Score: " + score, 590, 30);
    
        // Draw player paddle
        g.setColor(Color.green);
        g.fillRect(playerX, 550, 100, 8);
    
        // Draw ball
        g.setColor(Color.yellow);
        g.fillOval(ballPosX, ballPosY, 20, 20);
    }
    
    @Override
    public void actionPerformed(ActionEvent e) {
        timer.start();

        if (new Rectangle(ballPosX, ballPosY, 20, 20).intersects(new Rectangle(playerX, 550, 100, 8))) {
            ballYDir = -ballYDir;
        }

        A: for (int i = 0; i < map.map.length; i++) {
            for (int j = 0; j < map.map[0].length; j++) {
                if (map.map[i][j] > 0) {
                    int brickX = j * map.brickWidth + 80;
                    int brickY = i * map.brickHeight + 50;
                    int brickWidth = map.brickWidth;
                    int brickHeight = map.brickHeight;

                    Rectangle brickRect = new Rectangle(brickX, brickY, brickWidth, brickHeight);
                    Rectangle ballRect = new Rectangle(ballPosX, ballPosY, 20, 20);

                    if (ballRect.intersects(brickRect)) {
                        map.setBrickValue(0, i, j);
                        bricksCount--;
                        score += 5;

                        if (ballPosX + 19 <= brickRect.x || ballPosX + 1 >= brickRect.x + brickRect.width) {
                            ballXDir = -ballXDir;
                        } else {
                            ballYDir = -ballYDir;
                        }

                        break A;
                    }
                }
            }
        }

        ballPosX += ballXDir;
        ballPosY += ballYDir;

        if (ballPosX < 0) {
            ballXDir = -ballXDir;
        }
        if (ballPosY < 0) {
            ballYDir = -ballYDir;
        }
        if (ballPosX > 670) {
            ballXDir = -ballXDir;
        }

        repaint();
    }

    @Override
    public void keyTyped(KeyEvent e) {}

    @Override
    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
            if (playerX >= 600) {
                playerX = 600;
            } else {
                moveRight();
            }
        }
        if (e.getKeyCode() == KeyEvent.VK_LEFT) {
            if (playerX < 10) {
                playerX = 10;
            } else {
                moveLeft();
            }
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {}

    public void moveRight() {
        playerX += 20;
    }

    public void moveLeft() {
        playerX -= 20;
    }

    public void startNextStage() {
        try {
            Thread.sleep(2000); // Pause for a moment to show the message for moving to the next stage
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        ballPosX = 120;
        ballPosY = 350;
        timer.restart();
    }

    public static void main(String[] args) {
        BrickBreakerGame game = new BrickBreakerGame();
        game.setTitle("Brick Breaker Game");
        game.setSize(700, 630); // Set screen size slightly larger than the title bar size
        game.setResizable(false);
        game.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        game.setVisible(true);
    }
}

class MapGenerator {
    public int map[][];
    public int brickWidth;
    public int brickHeight;

    public MapGenerator(int row, int col) {
        map = new int[row][col];
        for (int i = 0; i < map.length; i++) {
            for (int j = 0; j < map[0].length; j++) {
                map[i][j] = 1;
            }
        }

        brickWidth = 540 / col;
        brickHeight = 150 / row;
    }

    public void draw(Graphics2D g) {
        for (int i = 0; i < map.length; i++) {
            for (int j = 0; j < map[0].length; j++) {
                if (map[i][j] > 0) {
                    g.setColor(Color.white);
                    g.fillRect(j * brickWidth + 80, i * brickHeight + 50, brickWidth, brickHeight);

                    g.setStroke(new BasicStroke(3));
                    g.setColor(Color.black);
                    g.drawRect(j * brickWidth + 80, i * brickHeight + 50, brickWidth, brickHeight);
                }
            }
        }
    }

    public void setBrickValue(int value, int row, int col) {
        map[row][col] = value;
    }
}

