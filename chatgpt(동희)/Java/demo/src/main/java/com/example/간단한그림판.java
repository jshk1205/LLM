package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class 간단한그림판 extends JFrame {

    private BufferedImage image;
    private Graphics2D g2;
    private Color currentColor = Color.BLACK;
    private int lastX, lastY;

    public 간단한그림판() {
        setTitle("간단한 그림판");
        setSize(800, 600);

        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("파일");
        JMenuItem saveItem = new JMenuItem("저장");
        saveItem.addActionListener(e -> saveImage());
        fileMenu.add(saveItem);
        menuBar.add(fileMenu);
        JMenu colorMenu = new JMenu("색상");
        JMenuItem selectColorItem = new JMenuItem("색상 선택");
        selectColorItem.addActionListener(e -> selectColor());
        colorMenu.add(selectColorItem);
        menuBar.add(colorMenu);
        setJMenuBar(menuBar);

        JPanel panel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                g.drawImage(image, 0, 0, null);
            }
        };
        panel.setBackground(Color.WHITE);
        panel.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                lastX = e.getX();
                lastY = e.getY();
            }
        });
        panel.addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                if (g2 != null) {
                    g2.setColor(currentColor);
                    g2.drawLine(lastX, lastY, e.getX(), e.getY());
                    lastX = e.getX();
                    lastY = e.getY();
                    panel.repaint();
                }
            }
        });

        getContentPane().add(panel, BorderLayout.CENTER);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setVisible(true);

        image = new BufferedImage(800, 600, BufferedImage.TYPE_INT_ARGB);
        g2 = image.createGraphics();
    }

    private void saveImage() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showSaveDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                ImageIO.write(image, "PNG", file);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }

    private void selectColor() {
        Color newColor = JColorChooser.showDialog(this, "색상 선택", currentColor);
        if (newColor != null) {
            currentColor = newColor;
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new 간단한그림판());
    }
}

