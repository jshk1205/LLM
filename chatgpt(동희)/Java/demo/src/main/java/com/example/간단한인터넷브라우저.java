package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class 간단한인터넷브라우저 extends JFrame {
    private JTextField addressBar;
    private JEditorPane pageDisplay;

    public 간단한인터넷브라우저() {
        setTitle("Simple Browser");
        setSize(800, 600);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        addressBar = new JTextField();
        addressBar.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadPage(addressBar.getText());
            }
        });

        JButton refreshButton = new JButton("Refresh");
        refreshButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadPage(addressBar.getText());
            }
        });

        JPanel topPanel = new JPanel();
        topPanel.setLayout(new BorderLayout());
        topPanel.add(addressBar, BorderLayout.CENTER);
        topPanel.add(refreshButton, BorderLayout.EAST);

        pageDisplay = new JEditorPane();
        pageDisplay.setContentType("text/html");

        JScrollPane scrollPane = new JScrollPane(pageDisplay);

        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(topPanel, BorderLayout.NORTH);
        getContentPane().add(scrollPane, BorderLayout.CENTER);

        // 화면 가운데에 위치하도록 설정
        setLocationRelativeTo(null);

        setVisible(true);
    }

    private void loadPage(String url) {
        try {
            URL urlObj = new URL(url);
            HttpURLConnection connection = (HttpURLConnection) urlObj.openConnection();
            connection.setRequestMethod("GET");

            BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            StringBuilder response = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                response.append(line);
            }
            reader.close();

            pageDisplay.setText(response.toString());
        } catch (IOException e) {
            e.printStackTrace();
            pageDisplay.setText("Error loading page: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new 간단한인터넷브라우저();
            }
        });
    }
}
