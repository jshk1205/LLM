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

public class DinosaurGame extends JFrame implements ActionListener, KeyListener {
    private static final int FRAME_WIDTH = 800;
    private static final int FRAME_HEIGHT = 300;
    private static final int GROUND_HEIGHT = 30;
    private static final int DINO_WIDTH = 30;
    private static final int DINO_HEIGHT = 50;
    private static final int CACTUS_WIDTH = 40; // Increased width
    private static final int CACTUS_HEIGHT = 40;
    private static int CACTUS_SPEED = 6;
    private static final int JUMP_HEIGHT_LIMIT = 100; // Jump height limit

    private Timer timer;
    private int dinoX, dinoY;
    private boolean isJumping;
    private boolean canJump; // Added flag
    private int jumpHeight;
    private ArrayList<Point> cactusList;
    private Random random;
    private int score;
    private BufferedImage buffer;

    public DinosaurGame() {
        setTitle("공룡 게임");
        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        dinoX = 100;
        dinoY = FRAME_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT;
        isJumping = false;
        canJump = true; // Initially set to allow jumping
        jumpHeight = 0;
        cactusList = new ArrayList<>();
        random = new Random();
        score = 0;

        addKeyListener(this);
        setFocusable(true);

        timer = new Timer(20, this);
        timer.start();

        // Center the window on the screen
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

        // Display score in the top left corner
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
                canJump = true; // Allow jumping again after the jump ends
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
            cactusList.add(new Point(FRAME_WIDTH, FRAME_HEIGHT - GROUND_HEIGHT - CACTUS_HEIGHT)); // Avoid overlapping with the ground
        }

        // Check collision
        Rectangle dinoRect = new Rectangle(dinoX, dinoY - jumpHeight, DINO_WIDTH, DINO_HEIGHT);
        for (Point cactus : cactusList) {
            Rectangle cactusRect = new Rectangle((int) cactus.getX(), (int) cactus.getY(), CACTUS_WIDTH, CACTUS_HEIGHT);
            if (dinoRect.intersects(cactusRect)) {
                timer.stop();
                JOptionPane.showMessageDialog(this, "Game Over! Score: " + score, "Game Over", JOptionPane.PLAIN_MESSAGE);
                System.exit(0);
            }
        }

        // Increase score over time
        score++;

        // Increase cactus speed as score increases
        if (score % 500 == 0) {
            CACTUS_SPEED += 1;
        }

        repaint();
    }

    @Override
    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_SPACE && canJump) { // Check canJump flag
            isJumping = true;
            canJump = false; // Disable jumping during a jump
            jumpHeight = 0; // Reset jump height
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
                DinosaurGame game = new DinosaurGame();
                game.setSize(FRAME_WIDTH, FRAME_HEIGHT);
                game.setVisible(true);
            }
        });
    }
}
