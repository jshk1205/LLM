package com.example.DNS;

import javax.swing.*;
import java.awt.*;
import java.net.*;
import java.util.HashMap;

public class DNS_Server_Simulator extends JFrame {
    private static HashMap<String, HashMap<String, String>> dnsRecords = new HashMap<>();
    private JTextArea logTextArea;
    private DatagramSocket serverSocket;

    public DNS_Server_Simulator() {
        setTitle("DNS Server Simulator");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new BorderLayout());

        logTextArea = new JTextArea();
        logTextArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(logTextArea);
        panel.add(scrollPane, BorderLayout.CENTER);

        getContentPane().add(panel);

        // Initialize DNS records
        initializeDNSRecords();

        try {
            serverSocket = new DatagramSocket(53);

            // Server execution
            log("DNS server is running...");

            while (true) {
                byte[] receiveData = new byte[1024];
                byte[] sendData = new byte[1024];

                DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                serverSocket.receive(receivePacket);

                // Receive DNS query
                String requestData = new String(receivePacket.getData(), 0, receivePacket.getLength());
                String[] requestDataParts = requestData.split(",");
                String queryType = requestDataParts[0];
                String domainName = requestDataParts[1];
                String recordType = requestDataParts[2];

                log("DNS query type: " + queryType);
                log("DNS query domain: " + domainName);
                log("DNS query record type: " + recordType);

                // Process based on DNS query type
                String response;
                switch (queryType) {
                    case "GET":
                        // Retrieve the record from DNS records based on domain name
                        HashMap<String, String> domainRecords = dnsRecords.getOrDefault(domainName, new HashMap<>());
                        String recordValue = domainRecords.getOrDefault(recordType, "Record not found.");
                        response = recordValue;
                        break;
                    case "PUT":
                        // Add or update DNS record
                        String recordValueToUpdate = requestDataParts[3];
                        HashMap<String, String> updatedDomainRecords = dnsRecords.getOrDefault(domainName, new HashMap<>());
                        updatedDomainRecords.put(recordType, recordValueToUpdate);
                        dnsRecords.put(domainName, updatedDomainRecords);
                        response = "Record successfully added or updated.";
                        break;
                    case "DELETE":
                        // Delete DNS record
                        HashMap<String, String> domainRecordsToDelete = dnsRecords.getOrDefault(domainName, new HashMap<>());
                        domainRecordsToDelete.remove(recordType);
                        dnsRecords.put(domainName, domainRecordsToDelete);
                        response = "Record successfully deleted.";
                        break;
                    default:
                        response = "Invalid query type.";
                }

                // Send response to the client
                InetAddress clientAddress = receivePacket.getAddress();
                int clientPort = receivePacket.getPort();
                sendData = response.getBytes();
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, clientAddress, clientPort);
                serverSocket.send(sendPacket);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
            }
        }
    }

    private void log(String message) {
        logTextArea.append(message + "\n");
    }

    private static void initializeDNSRecords() {
        // Add example DNS records
        HashMap<String, String> exampleComRecords = new HashMap<>();
        exampleComRecords.put("A", "192.0.2.1");
        exampleComRecords.put("MX", "mail.example.com");
        dnsRecords.put("example.com", exampleComRecords);

        HashMap<String, String> exampleNetRecords = new HashMap<>();
        exampleNetRecords.put("A", "192.0.2.2");
        dnsRecords.put("example.net", exampleNetRecords);

        HashMap<String, String> exampleOrgRecords = new HashMap<>();
        exampleOrgRecords.put("A", "192.0.2.3");
        dnsRecords.put("example.org", exampleOrgRecords);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            DNS_Server_Simulator server = new DNS_Server_Simulator();
            server.setLocationRelativeTo(null); // Center the frame on the screen
            server.setVisible(true);
        });
    }
}
