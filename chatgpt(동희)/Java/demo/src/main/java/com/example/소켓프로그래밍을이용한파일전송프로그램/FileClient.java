package com.example.소켓프로그래밍을이용한파일전송프로그램;

import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.event.*;

public class FileClient extends JFrame {
    private Socket socket;
    private ObjectOutputStream outputStream;
    private ObjectInputStream inputStream;

    public FileClient() {
        setTitle("File Client");
        setSize(300, 200);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        JButton sendButton = new JButton("Select File");
        sendButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                int result = fileChooser.showOpenDialog(null);
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();
                    sendFile(selectedFile);
                }
            }
        });
        add(sendButton);

        setVisible(true);
    }

    private void sendFile(File file) {
        try {
            socket = new Socket("localhost", 12345);
            outputStream = new ObjectOutputStream(socket.getOutputStream());

            // Send file name and content
            outputStream.writeObject(file.getName());
            FileInputStream fileInputStream = new FileInputStream(file);
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
            }
            fileInputStream.close();
            outputStream.flush();
            System.out.println("File sent successfully.");
        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (outputStream != null) outputStream.close();
                if (socket != null) socket.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        new FileClient();
    }
}
