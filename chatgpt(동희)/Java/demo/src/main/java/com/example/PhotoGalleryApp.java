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

public class PhotoGalleryApp extends JFrame {
    private static final int MAX_IMAGES = 10; // Specifies the maximum number of images.
    private JLabel photoLabel;
    private JLabel filenameLabel;
    private ImageIcon[] images;
    private String[] filenames;
    private int currentIndex;
    private BufferedImage currentImage;
    private int rotationAngle = 0;

    public PhotoGalleryApp() {
        images = new ImageIcon[MAX_IMAGES];
        filenames = new String[MAX_IMAGES];
        setTitle("Photo Gallery Application");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null); // Place the frame in the center of the screen

        photoLabel = new JLabel();
        photoLabel.setHorizontalAlignment(SwingConstants.CENTER);
        JScrollPane scrollPane = new JScrollPane(photoLabel);
        add(scrollPane, BorderLayout.CENTER);

        filenameLabel = new JLabel();
        filenameLabel.setHorizontalAlignment(SwingConstants.CENTER);
        add(filenameLabel, BorderLayout.SOUTH);

        JPanel controlPanel = new JPanel();
        JButton prevButton = new JButton("Previous");
        JButton nextButton = new JButton("Next");
        JButton openButton = new JButton("Open Photo");
        JButton rotateButton = new JButton("Rotate");
        JButton zoomInButton = new JButton("Zoom In");
        JButton zoomOutButton = new JButton("Zoom Out");
        JButton infoButton = new JButton("Info");
        JButton saveButton = new JButton("Save");
        JButton deleteButton = new JButton("Delete");

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

    // Display the previous image
    private void showPrevious() {
        if (currentIndex > 0) {
            currentIndex--;
            showImage(currentIndex);
        }
    }

    // Display the next image
    private void showNext() {
        if (currentIndex < images.length - 1 && images[currentIndex + 1] != null) {
            currentIndex++;
            showImage(currentIndex);
        }
    }

    // Show the image at the specified index
    private void showImage(int index) {
        photoLabel.setIcon(images[index]);
        filenameLabel.setText(filenames[index]);
        currentImage = convertToBufferedImage(images[index].getImage());
    }

    // Open image files and display them in the gallery
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
                        JOptionPane.showMessageDialog(this, "Failed to open the image.", "Error", JOptionPane.ERROR_MESSAGE);
                    }
                } else {
                    JOptionPane.showMessageDialog(this, "Cannot add more images.", "Error", JOptionPane.ERROR_MESSAGE);
                    break;
                }
            }
            currentIndex = 0;
            showImage(currentIndex);
        }
    }

    // Rotate the current image by 90 degrees
    private void rotateImage() {
        rotationAngle = (rotationAngle + 90) % 360;
        BufferedImage rotatedImage = rotate(currentImage, rotationAngle);
        ImageIcon rotatedIcon = new ImageIcon(rotatedImage);
        photoLabel.setIcon(rotatedIcon);
    }

    // Zoom the current image by a specified factor
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

    // Display information about the current image
    private void showImageInfo() {
        int width = currentImage.getWidth();
        int height = currentImage.getHeight();
        JOptionPane.showMessageDialog(this, "Image Info:\nWidth: " + width + " pixels\nHeight: " + height + " pixels",
                "Image Info", JOptionPane.INFORMATION_MESSAGE);
    }

    // Save the current image to a file
    private void saveImage() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("JPEG Image", "jpg", "jpeg"));
        int result = fileChooser.showSaveDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File output = fileChooser.getSelectedFile();
            try {
                ImageIO.write(currentImage, "jpg", output);
                JOptionPane.showMessageDialog(this, "Image saved successfully.", "Save Complete", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(this, "Error occurred while saving the image.", "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    // Delete the current image from the gallery
    private void deleteImage() {
        int confirm = JOptionPane.showConfirmDialog(this, "Are you sure you want to delete this image?", "Delete Image", JOptionPane.YES_NO_OPTION);
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
    
    // Get the index of the next valid image
    private int getNextValidIndex(int index) {
        int nextIndex = index + 1;
        while (nextIndex < images.length && images[nextIndex] == null){
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

    // Rotate the given image by a specified angle
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

    // Convert Image to BufferedImage
    private BufferedImage convertToBufferedImage(Image image) {
        BufferedImage bufferedImage = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_RGB);
        Graphics2D g2d = bufferedImage.createGraphics();
        g2d.drawImage(image, 0, 0, null);
        g2d.dispose();
        return bufferedImage;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            PhotoGalleryApp app = new PhotoGalleryApp();
            app.setVisible(true);
        });
    }
}

