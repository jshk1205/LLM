package com.example.소켓프로그래밍을이용한파일전송프로그램;

import java.io.*;
import java.net.*;

public class FileServer {
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        Socket socket = null;
        InputStream inputStream = null;
        FileOutputStream fileOutputStream = null;

        try {
            serverSocket = new ServerSocket(12345);
            System.out.println("Server is running...");

            socket = serverSocket.accept();
            System.out.println("Client connected.");

            inputStream = socket.getInputStream();

            // Read file name
            ObjectInputStream objectInputStream = new ObjectInputStream(inputStream);
            String fileName = (String) objectInputStream.readObject();

            // Save file
            fileOutputStream = new FileOutputStream(fileName); // 현재 디렉터리에 저장
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                fileOutputStream.write(buffer, 0, bytesRead);
            }

            System.out.println("File saved: " + fileName);
        } catch (IOException | ClassNotFoundException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (fileOutputStream != null) fileOutputStream.close();
                if (inputStream != null) inputStream.close();
                if (socket != null) socket.close();
                if (serverSocket != null) serverSocket.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }
}
