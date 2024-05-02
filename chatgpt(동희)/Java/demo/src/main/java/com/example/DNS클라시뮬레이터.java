package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.*;

public class DNS클라시뮬레이터 extends JFrame {
    private JTextField queryTypeField;
    private JTextField domainNameField;
    private JTextField recordTypeField;
    private JTextField recordValueField;
    private JTextArea responseTextArea;

    public DNS클라시뮬레이터() {
        setTitle("DNS 클라이언트 시뮬레이터");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridLayout(6, 2));

        panel.add(new JLabel("Query Type:"));
        queryTypeField = new JTextField();
        panel.add(queryTypeField);

        panel.add(new JLabel("Domain Name:"));
        domainNameField = new JTextField();
        panel.add(domainNameField);

        panel.add(new JLabel("Record Type:"));
        recordTypeField = new JTextField();
        panel.add(recordTypeField);

        panel.add(new JLabel("Record Value:"));
        recordValueField = new JTextField();
        panel.add(recordValueField);

        JButton sendButton = new JButton("Send Query");
        sendButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                sendQuery();
            }
        });
        panel.add(sendButton);

        responseTextArea = new JTextArea();
        responseTextArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(responseTextArea);
        panel.add(scrollPane);

        getContentPane().add(panel, BorderLayout.CENTER);
    }

    private void sendQuery() {
        try {
            DatagramSocket clientSocket = new DatagramSocket();
            InetAddress serverAddress = InetAddress.getByName("localhost");
            byte[] sendData = new byte[1024];
            byte[] receiveData = new byte[1024];

            // DNS 쿼리 입력
            String queryType = queryTypeField.getText().trim();
            String domainName = domainNameField.getText().trim();
            String recordType = recordTypeField.getText().trim();
            String recordValue = recordValueField.getText().trim();
            String requestData = queryType + "," + domainName + "," + recordType + "," + recordValue;

            // DNS 쿼리 전송
            sendData = requestData.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, 53);
            clientSocket.send(sendPacket);

            // DNS 응답 수신
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            clientSocket.receive(receivePacket);
            String response = new String(receivePacket.getData(), 0, receivePacket.getLength());
            responseTextArea.setText("서버 응답: " + response);

            clientSocket.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            DNS클라시뮬레이터 client = new DNS클라시뮬레이터();
            client.setLocationRelativeTo(null); // 화면 가운데 정렬
            client.setVisible(true);
        });
    }
}
