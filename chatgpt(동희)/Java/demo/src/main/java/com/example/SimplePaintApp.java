package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class SimplePaintApp extends JFrame {

    private BufferedImage image;
    private Graphics2D g2;
    private Color currentColor = Color.BLACK;
    private int lastX, lastY;

    public SimplePaintApp() {
        setTitle("Simple Paint App");
        setSize(800, 600);

        // Menu bar and items for saving the image and selecting color
        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("File");
        JMenuItem saveItem = new JMenuItem("Save");
        saveItem.addActionListener(e -> saveImage());
        fileMenu.add(saveItem);
        menuBar.add(fileMenu);
        JMenu colorMenu = new JMenu("Color");
        JMenuItem selectColorItem = new JMenuItem("Select Color");
        selectColorItem.addActionListener(e -> selectColor());
        colorMenu.add(selectColorItem);
        menuBar.add(colorMenu);
        setJMenuBar(menuBar);

        // Drawing panel
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
                // Record the initial coordinates when the mouse is pressed
                lastX = e.getX();
                lastY = e.getY();
            }
        });
        panel.addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                // Draw a line while the mouse is being dragged
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

        // Initialize the image and its graphics context
        image = new BufferedImage(800, 600, BufferedImage.TYPE_INT_ARGB);
        g2 = image.createGraphics();
    }

    // Save the image to a file
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

    // Open a color chooser dialog to select a new color
    private void selectColor() {
        Color newColor = JColorChooser.showDialog(this, "Select Color", currentColor);
        if (newColor != null) {
            currentColor = newColor;
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SimplePaintApp());
    }
}
