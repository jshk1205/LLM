package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Random;

public class 공룡게임 extends JFrame implements ActionListener, KeyListener {
    private static final int FRAME_WIDTH = 800;
    private static final int FRAME_HEIGHT = 300;
    private static final int GROUND_HEIGHT = 30;
    private static final int DINO_WIDTH = 30;
    private static final int DINO_HEIGHT = 50;
    private static final int CACTUS_WIDTH = 40; // 너비 확장
    private static final int CACTUS_HEIGHT = 40;
    private static int CACTUS_SPEED = 6;
    private static final int JUMP_HEIGHT_LIMIT = 100; // 점프 높이 제한

    private Timer timer;
    private int dinoX, dinoY;
    private boolean isJumping;
    private boolean canJump; // 추가된 플래그
    private int jumpHeight;
    private ArrayList<Point> cactusList;
    private Random random;
    private int score;
    private BufferedImage buffer;

    public 공룡게임() {
        setTitle("공룡 게임");
        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        dinoX = 100;
        dinoY = FRAME_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT;
        isJumping = false;
        canJump = true; // 초기에 점프 가능하도록 설정
        jumpHeight = 0;
        cactusList = new ArrayList<>();
        random = new Random();
        score = 0;

        addKeyListener(this);
        setFocusable(true);

        timer = new Timer(20, this);
        timer.start();

        // 화면 중앙에 위치시키기
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation(screenSize.width / 2 - FRAME_WIDTH / 2, screenSize.height / 2 - FRAME_HEIGHT / 2);

        buffer = new BufferedImage(FRAME_WIDTH, FRAME_HEIGHT, BufferedImage.TYPE_INT_ARGB);
    }

    @Override
    public void paint(Graphics g) {
        Graphics2D g2d = (Graphics2D) buffer.getGraphics();
        super.paint(g2d);
        drawGround(g2d);
        drawDinosaur(g2d);
        drawCactuses(g2d);

        // 왼쪽 상단에 점수 표시
        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("Arial", Font.BOLD, 20));
        g2d.drawString("Score: " + score, 20, 30);

        g.drawImage(buffer, 0, 0, this);
    }

    private void drawGround(Graphics g) {
        g.setColor(Color.GREEN);
        g.fillRect(0, FRAME_HEIGHT - GROUND_HEIGHT, FRAME_WIDTH, GROUND_HEIGHT);
    }

    private void drawDinosaur(Graphics g) {
        g.setColor(Color.BLACK);
        g.fillRect(dinoX, dinoY - jumpHeight, DINO_WIDTH, DINO_HEIGHT);
    }

    private void drawCactuses(Graphics g) {
        for (Point cactus : cactusList) {
            g.fillRect((int) cactus.getX(), (int) cactus.getY(), CACTUS_WIDTH, CACTUS_HEIGHT);
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (isJumping) {
            jumpHeight += 5;
            if (jumpHeight >= JUMP_HEIGHT_LIMIT) {
                isJumping = false;
                canJump = true; // 점프가 끝나면 다시 점프 가능하도록 설정
            }
        } else {
            if (jumpHeight > 0) {
                jumpHeight -= 5;
            }
        }

        // Move cacti
        for (int i = 0; i < cactusList.size(); i++) {
            Point cactus = cactusList.get(i);
            cactus.setLocation(cactus.getX() - CACTUS_SPEED, cactus.getY());
            if (cactus.getX() < -CACTUS_WIDTH) {
                cactusList.remove(i);
            }
        }

        // Add new cactus
        if (random.nextInt(100) < 3) {
            cactusList.add(new Point(FRAME_WIDTH, FRAME_HEIGHT - GROUND_HEIGHT - CACTUS_HEIGHT)); // 바닥과 겹치지 않도록 변경
        }

        // Check collision
        Rectangle dinoRect = new Rectangle(dinoX, dinoY - jumpHeight, DINO_WIDTH, DINO_HEIGHT);
        for (Point cactus : cactusList) {
            Rectangle cactusRect = new Rectangle((int) cactus.getX(), (int) cactus.getY(), CACTUS_WIDTH, CACTUS_HEIGHT);
            if (dinoRect.intersects(cactusRect)) {
                timer.stop();
                JOptionPane.showMessageDialog(this, "게임 오버! 점수: " + score, "게임 오버", JOptionPane.PLAIN_MESSAGE);
                System.exit(0);
            }
        }

        // Increase score as time passes
        score++;

        // Increase cactus speed over time
        if (score % 500 == 0) {
            CACTUS_SPEED += 1;
        }

        repaint();
    }

    @Override
    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_SPACE && canJump) { // canJump 플래그 확인
            isJumping = true;
            canJump = false; // 점프 중일 때는 다시 점프할 수 없도록 설정
            jumpHeight = 0; // 점프 높이 초기화
        }
    }

    @Override
    public void keyTyped(KeyEvent e) {}

    @Override
    public void keyReleased(KeyEvent e) {}

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                공룡게임 game = new 공룡게임();
                game.setSize(FRAME_WIDTH, FRAME_HEIGHT);
                game.setVisible(true);
            }
        });
    }
}
