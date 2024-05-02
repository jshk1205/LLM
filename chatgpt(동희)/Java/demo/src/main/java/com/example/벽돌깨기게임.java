package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class 벽돌깨기게임 extends JFrame implements KeyListener, ActionListener {
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

    public 벽돌깨기게임() {
        map = new MapGenerator(3, 7);

        addKeyListener(this);
        setFocusable(true);
        setFocusTraversalKeysEnabled(false);
        timer = new Timer(delay, this);
        timer.start();
    }

    public void paint(Graphics g) {
        // 더블 버퍼링을 위한 off-screen 이미지 생성
        Image offScreenImage = createImage(getWidth(), getHeight());
        Graphics offScreenGraphics = offScreenImage.getGraphics();
        // off-screen 이미지에 그림
        paintOffScreen(offScreenGraphics);
        // off-screen 이미지를 화면에 그림
        g.drawImage(offScreenImage, 0, 30, this);
    }

    public void paintOffScreen(Graphics g) {
        // 배경 그리기
        g.setColor(Color.black);
        g.fillRect(1, 31, 692, 592);
    
        // 벽돌 그리기
        map.draw((Graphics2D) g);
    
        // 경계 그리기
        g.setColor(Color.yellow);
        g.fillRect(0, 30, 3, 592);
        g.fillRect(0, 30, 692, 3);
        g.fillRect(691, 30, 3, 592);
    
        // 게임 종료
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
                g.drawString("다음 스테이지로 이동합니다!", 190, 300);
                startNextStage();
            } else {
                g.setColor(Color.red);
                g.setFont(new Font("serif", Font.BOLD, 30));
                g.drawString("게임 승리! 최종 점수: " + score, 190, 300);
            }
        }
    
        if (ballPosY > 570) {
            timer.stop();
            g.setColor(Color.red);
            g.setFont(new Font("serif", Font.BOLD, 30));
            g.drawString("실패! 최종 점수: " + score, 190, 300);
        }
    
        // 점수 표시
        g.setColor(Color.white);
        g.setFont(new Font("serif", Font.BOLD, 25));
        g.drawString("점수: " + score, 590, 30);
    
        // 플레이어 패드 그리기
        g.setColor(Color.green);
        g.fillRect(playerX, 550, 100, 8);
    
        // 공 그리기
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
            Thread.sleep(2000); // 다음 스테이지로 이동하는 메시지를 보여주기 위한 잠시 멈춤
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        ballPosX = 120;
        ballPosY = 350;
        timer.restart();
    }

    public static void main(String[] args) {
        벽돌깨기게임 game = new 벽돌깨기게임();
        game.setTitle("벽돌깨기 게임");
        game.setSize(700, 630); // 타이틀 바 크기만큼 화면 크기를 더 크게 설정
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
