package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;

public class FileEncryptionDecryption extends JFrame {
    private JTextField textField;
    private JTextArea textArea;
    private JButton selectFileButton;
    private JButton encryptButton;
    private JButton decryptButton;
    private File selectedFile;

    public FileEncryptionDecryption() {
        setTitle("File Encryption and Decryption");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // Display at the center of the screen

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        textField = new JTextField();
        textArea = new JTextArea();
        JScrollPane scrollPane = new JScrollPane(textArea);

        selectFileButton = new JButton("Select File");
        selectFileButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                selectFile();
            }
        });

        encryptButton = new JButton("Encrypt");
        encryptButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (selectedFile != null)
                    encryptFile(selectedFile);
                else
                    JOptionPane.showMessageDialog(FileEncryptionDecryption.this, "Please select a file.");
            }
        });

        decryptButton = new JButton("Decrypt");
        decryptButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (selectedFile != null)
                    decryptFile(selectedFile);
                else
                    JOptionPane.showMessageDialog(FileEncryptionDecryption.this, "Please select a file.");
            }
        });

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(selectFileButton);
        buttonPanel.add(encryptButton);
        buttonPanel.add(decryptButton);

        panel.add(textField, BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);

        add(panel);
        setVisible(true);
    }

    // Selects a file using JFileChooser
    private void selectFile() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            selectedFile = fileChooser.getSelectedFile();
            textField.setText(selectedFile.getAbsolutePath());
        }
    }

    // Encrypts the specified file using the AES encryption algorithm
    private void encryptFile(File file) {
        try {
            // Generating the encryption key (16-byte key for AES)
            byte[] keyBytes = "1234567890123456".getBytes();
            SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
            
            // Initializing the AES cipher in encryption mode
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.ENCRYPT_MODE, key);

            // Reading data from the input file, encrypting it, and writing to the output file
            FileInputStream fis = new FileInputStream(file);
            FileOutputStream fos = new FileOutputStream(file.getAbsolutePath() + ".enc");
            CipherOutputStream cos = new CipherOutputStream(fos, cipher);
            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = fis.read(buffer)) != -1) {
                cos.write(buffer, 0, bytesRead);
            }

            // Closing streams after encryption
            cos.close();
            fos.close();
            fis.close();

            // Displaying a message indicating successful encryption
            JOptionPane.showMessageDialog(this, "Encryption completed.");
        } catch (Exception e) {
            // Handling encryption failures and displaying error messages
            JOptionPane.showMessageDialog(this, "Encryption failed: " + e.getMessage());
        }
    }

    // Decrypts the specified file using the AES decryption algorithm
    private void decryptFile(File file) {
        try {
            // Generating the encryption key (16-byte key for AES)
            byte[] keyBytes = "1234567890123456".getBytes();
            SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
            
            // Initializing the AES cipher in decryption mode
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.DECRYPT_MODE, key);

            // Reading encrypted data from the input file, decrypting it, and storing in memory
            FileInputStream fis = new FileInputStream(file);
            CipherInputStream cis = new CipherInputStream(fis, cipher);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = cis.read(buffer)) != -1) {
                baos.write(buffer, 0, bytesRead);
            }

            // Converting decrypted data to string and displaying in the text area
            byte[] decryptedBytes = baos.toByteArray();
            String decryptedText = new String(decryptedBytes);
            textArea.setText(decryptedText); // Displaying decrypted content in the text area

            // Closing streams after decryption
            cis.close();
            fis.close();

            // Displaying a message indicating successful decryption
            JOptionPane.showMessageDialog(this, "Decryption completed.");
        } catch (Exception e) {
            // Handling decryption failures and displaying error messages
            JOptionPane.showMessageDialog(this, "Decryption failed: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new FileEncryptionDecryption();
            }
        });
    }
}
