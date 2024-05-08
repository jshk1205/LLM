package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;

public class 파일암호화복호화 extends JFrame {
    private JTextField textField;
    private JTextArea textArea;
    private JButton selectFileButton;
    private JButton encryptButton;
    private JButton decryptButton;
    private File selectedFile;

    public 파일암호화복호화() {
        setTitle("파일 암호화 및 복호화");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // 화면 중앙에 표시

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        textField = new JTextField();
        textArea = new JTextArea();
        JScrollPane scrollPane = new JScrollPane(textArea);

        selectFileButton = new JButton("파일 선택");
        selectFileButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                selectFile();
            }
        });

        encryptButton = new JButton("암호화");
        encryptButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (selectedFile != null)
                    encryptFile(selectedFile);
                else
                    JOptionPane.showMessageDialog(파일암호화복호화.this, "파일을 선택하세요.");
            }
        });

        decryptButton = new JButton("복호화");
        decryptButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (selectedFile != null)
                    decryptFile(selectedFile);
                else
                    JOptionPane.showMessageDialog(파일암호화복호화.this, "파일을 선택하세요.");
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

    private void selectFile() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            selectedFile = fileChooser.getSelectedFile();
            textField.setText(selectedFile.getAbsolutePath());
        }
    }

    private void encryptFile(File file) {
        try {
            byte[] keyBytes = "1234567890123456".getBytes(); // 16-byte key for AES
            SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.ENCRYPT_MODE, key);

            FileInputStream fis = new FileInputStream(file);
            FileOutputStream fos = new FileOutputStream(file.getAbsolutePath() + ".enc");
            CipherOutputStream cos = new CipherOutputStream(fos, cipher);

            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = fis.read(buffer)) != -1) {
                cos.write(buffer, 0, bytesRead);
            }

            cos.close();
            fos.close();
            fis.close();

            JOptionPane.showMessageDialog(this, "암호화가 완료되었습니다.");
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "암호화 실패: " + e.getMessage());
        }
    }

    private void decryptFile(File file) {
        try {
            byte[] keyBytes = "1234567890123456".getBytes(); // 16-byte key for AES
            SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.DECRYPT_MODE, key);

            FileInputStream fis = new FileInputStream(file);
            CipherInputStream cis = new CipherInputStream(fis, cipher);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();

            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = cis.read(buffer)) != -1) {
                baos.write(buffer, 0, bytesRead);
            }

            byte[] decryptedBytes = baos.toByteArray();
            String decryptedText = new String(decryptedBytes);
            textArea.setText(decryptedText); // 복호화된 내용을 텍스트 영역에 표시

            cis.close();
            fis.close();

            JOptionPane.showMessageDialog(this, "복호화가 완료되었습니다.");
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "복호화 실패: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new 파일암호화복호화();
            }
        });
    }
}
