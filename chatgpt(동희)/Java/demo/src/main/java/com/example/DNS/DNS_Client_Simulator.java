package com.example.DNS;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.*;

public class DNS_Client_Simulator extends JFrame {
    private JTextField queryTypeField;
    private JTextField domainNameField;
    private JTextField recordTypeField;
    private JTextField recordValueField;
    private JTextArea responseTextArea;

    public DNS_Client_Simulator() {
        setTitle("DNS Client Simulator");
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

            // Input DNS query
            String queryType = queryTypeField.getText().trim();
            String domainName = domainNameField.getText().trim();
            String recordType = recordTypeField.getText().trim();
            String recordValue = recordValueField.getText().trim();
            String requestData = queryType + "," + domainName + "," + recordType + "," + recordValue;

            // Send DNS query
            sendData = requestData.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, 53);
            clientSocket.send(sendPacket);

            // Receive DNS response
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            clientSocket.receive(receivePacket);
            String response = new String(receivePacket.getData(), 0, receivePacket.getLength());
            responseTextArea.setText("Server Response: " + response);

            clientSocket.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            DNS_Client_Simulator client = new DNS_Client_Simulator();
            client.setLocationRelativeTo(null); // Center the frame on the screen
            client.setVisible(true);
        });
    }
}
