package com.example;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class 사진갤러리앱 extends JFrame {
    private static final int MAX_IMAGES = 10; // 최대 이미지 수를 지정합니다.
    private JLabel photoLabel;
    private JLabel filenameLabel;
    private ImageIcon[] images;
    private String[] filenames;
    private int currentIndex;
    private BufferedImage currentImage;
    private int rotationAngle = 0;

    public 사진갤러리앱() {
        images = new ImageIcon[MAX_IMAGES];
        filenames = new String[MAX_IMAGES];
        setTitle("사진 갤러리 애플리케이션");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null); // 화면 중앙에 프레임 배치

        photoLabel = new JLabel();
        photoLabel.setHorizontalAlignment(SwingConstants.CENTER);
        JScrollPane scrollPane = new JScrollPane(photoLabel);
        add(scrollPane, BorderLayout.CENTER);

        filenameLabel = new JLabel();
        filenameLabel.setHorizontalAlignment(SwingConstants.CENTER);
        add(filenameLabel, BorderLayout.SOUTH);

        JPanel controlPanel = new JPanel();
        JButton prevButton = new JButton("이전");
        JButton nextButton = new JButton("다음");
        JButton openButton = new JButton("사진 열기");
        JButton rotateButton = new JButton("회전");
        JButton zoomInButton = new JButton("확대");
        JButton zoomOutButton = new JButton("축소");
        JButton infoButton = new JButton("정보");
        JButton saveButton = new JButton("저장");
        JButton deleteButton = new JButton("삭제");

        prevButton.addActionListener(e -> showPrevious());
        nextButton.addActionListener(e -> showNext());
        openButton.addActionListener(e -> openImage());
        rotateButton.addActionListener(e -> rotateImage());
        zoomInButton.addActionListener(e -> zoomImage(1.2));
        zoomOutButton.addActionListener(e -> zoomImage(0.8));
        infoButton.addActionListener(e -> showImageInfo());
        saveButton.addActionListener(e -> saveImage());
        deleteButton.addActionListener(e -> deleteImage());

        controlPanel.add(prevButton);
        controlPanel.add(nextButton);
        controlPanel.add(openButton);
        controlPanel.add(rotateButton);
        controlPanel.add(zoomInButton);
        controlPanel.add(zoomOutButton);
        controlPanel.add(infoButton);
        controlPanel.add(saveButton);
        controlPanel.add(deleteButton);

        add(controlPanel, BorderLayout.NORTH);
    }

    private void showPrevious() {
        if (currentIndex > 0) {
            currentIndex--;
            showImage(currentIndex);
        }
    }

    private void showNext() {
        if (currentIndex < images.length - 1 && images[currentIndex + 1] != null) {
            currentIndex++;
            showImage(currentIndex);
        }
    }

    private void showImage(int index) {
        photoLabel.setIcon(images[index]);
        filenameLabel.setText(filenames[index]);
        currentImage = convertToBufferedImage(images[index].getImage());
    }

    private void openImage() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("Image files", "jpg", "jpeg", "png", "gif"));
        fileChooser.setMultiSelectionEnabled(true);
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File[] selectedFiles = fileChooser.getSelectedFiles();
            for (int i = 0; i < selectedFiles.length; i++) {
                if (currentIndex + i < images.length) {
                    try {
                        ImageIcon imageIcon = new ImageIcon(ImageIO.read(selectedFiles[i]));
                        Image image = imageIcon.getImage().getScaledInstance(600, 400, Image.SCALE_SMOOTH);
                        images[currentIndex + i] = new ImageIcon(image);
                        filenames[currentIndex + i] = selectedFiles[i].getName();
                    } catch (IOException ex) {
                        ex.printStackTrace();
                        JOptionPane.showMessageDialog(this, "이미지를 열 수 없습니다.", "오류", JOptionPane.ERROR_MESSAGE);
                    }
                } else {
                    JOptionPane.showMessageDialog(this, "더 이상 이미지를 추가할 수 없습니다.", "오류", JOptionPane.ERROR_MESSAGE);
                    break;
                }
            }
            currentIndex = 0;
            showImage(currentIndex);
        }
    }

    private void rotateImage() {
        rotationAngle = (rotationAngle + 90) % 360;
        BufferedImage rotatedImage = rotate(currentImage, rotationAngle);
        ImageIcon rotatedIcon = new ImageIcon(rotatedImage);
        photoLabel.setIcon(rotatedIcon);
    }

    private void zoomImage(double factor) {
        int width = currentImage.getWidth();
        int height = currentImage.getHeight();

        AffineTransform tx = new AffineTransform();
        tx.scale(factor, factor);
        AffineTransformOp op = new AffineTransformOp(tx, AffineTransformOp.TYPE_BILINEAR);
        BufferedImage zoomedImage = new BufferedImage((int)(width * factor), (int)(height * factor), currentImage.getType());
        op.filter(currentImage, zoomedImage);

        ImageIcon zoomedIcon = new ImageIcon(zoomedImage);
        photoLabel.setIcon(zoomedIcon);
        currentImage = zoomedImage;
    }

    private void showImageInfo() {
        int width = currentImage.getWidth();
        int height = currentImage.getHeight();
        JOptionPane.showMessageDialog(this, "이미지 정보:\n너비: " + width + " 픽셀\n높이: " + height + " 픽셀",
                "이미지 정보", JOptionPane.INFORMATION_MESSAGE);
    }

    private void saveImage() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("JPEG 이미지", "jpg", "jpeg"));
        int result = fileChooser.showSaveDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File output = fileChooser.getSelectedFile();
            try {
                ImageIO.write(currentImage, "jpg", output);
                JOptionPane.showMessageDialog(this, "이미지가 성공적으로 저장되었습니다.", "저장 완료", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(this, "이미지 저장 중 오류가 발생했습니다.", "오류", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void deleteImage() {
        int confirm = JOptionPane.showConfirmDialog(this, "이 이미지를 삭제하시겠습니까?", "이미지 삭제", JOptionPane.YES_NO_OPTION);
        if (confirm == JOptionPane.YES_OPTION) {
            images[currentIndex] = null;
            filenames[currentIndex] = null;
            int nextIndex = getNextValidIndex(currentIndex);
            if (nextIndex != -1) {
                currentIndex = nextIndex;
                showImage(currentIndex);
            } else {
                photoLabel.setIcon(null);
                filenameLabel.setText("");
            }
        }
    }
    
    private int getNextValidIndex(int index) {
        int nextIndex = index + 1;
        while (nextIndex < images.length && images[nextIndex] == null) {
            nextIndex++;
        }
        if (nextIndex < images.length) {
            return nextIndex;
        } else {
            int prevIndex = index - 1;
            while (prevIndex >= 0 && images[prevIndex] == null) {
                prevIndex--;
            }
            if (prevIndex >= 0) {
                return prevIndex;
            } else {
                return -1;
            }
        }
    }

    private BufferedImage rotate(BufferedImage image, double angle) {
        double radians = Math.toRadians(angle);
        double sin = Math.abs(Math.sin(radians));
        double cos = Math.abs(Math.cos(radians));
        int width = image.getWidth();
        int height = image.getHeight();
        int newWidth = (int) Math.floor(width * cos + height * sin);
        int newHeight = (int) Math.floor(height * cos + width * sin);
        BufferedImage rotatedImage = new BufferedImage(newWidth, newHeight, image.getType());
        Graphics2D g2d = rotatedImage.createGraphics();
        AffineTransform at = new AffineTransform();
        at.translate((newWidth - width) / 2, (newHeight - height) / 2);
        int x = width / 2;
        int y = height / 2;
        at.rotate(radians, x, y);
        g2d.setTransform(at);
        g2d.drawImage(image, 0, 0, null);
        g2d.dispose();
        return rotatedImage;
    }

    private BufferedImage convertToBufferedImage(Image image) {
        BufferedImage bufferedImage = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_RGB);
        Graphics2D g2d = bufferedImage.createGraphics();
        g2d.drawImage(image, 0, 0, null);
        g2d.dispose();
        return bufferedImage;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            사진갤러리앱 app = new 사진갤러리앱();
            app.setVisible(true);
        });
    }
}
