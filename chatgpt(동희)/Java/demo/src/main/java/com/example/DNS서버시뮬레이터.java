package com.example;
import javax.swing.*;
import java.awt.*;
import java.net.*;
import java.util.HashMap;

public class DNS서버시뮬레이터 extends JFrame {
    private static HashMap<String, HashMap<String, String>> dnsRecords = new HashMap<>();
    private JTextArea logTextArea;
    private DatagramSocket serverSocket;

    public DNS서버시뮬레이터() {
        setTitle("DNS 서버 시뮬레이터");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new BorderLayout());

        logTextArea = new JTextArea();
        logTextArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(logTextArea);
        panel.add(scrollPane, BorderLayout.CENTER);

        getContentPane().add(panel);

        // DNS 레코드 초기화
        initializeDNSRecords();

        try {
            serverSocket = new DatagramSocket(53);

            // 서버 실행
            log("DNS 서버가 실행 중입니다...");

            while (true) {
                byte[] receiveData = new byte[1024];
                byte[] sendData = new byte[1024];

                DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                serverSocket.receive(receivePacket);

                // DNS 쿼리 수신
                String requestData = new String(receivePacket.getData(), 0, receivePacket.getLength());
                String[] requestDataParts = requestData.split(",");
                String queryType = requestDataParts[0];
                String domainName = requestDataParts[1];
                String recordType = requestDataParts[2];

                log("DNS 쿼리 타입: " + queryType);
                log("DNS 쿼리 도메인: " + domainName);
                log("DNS 쿼리 레코드 유형: " + recordType);

                // DNS 쿼리 타입에 따라 처리
                String response;
                switch (queryType) {
                    case "GET":
                        // DNS 레코드에서 도메인 이름에 해당하는 레코드 조회
                        HashMap<String, String> domainRecords = dnsRecords.getOrDefault(domainName, new HashMap<>());
                        String recordValue = domainRecords.getOrDefault(recordType, "레코드를 찾을 수 없습니다.");
                        response = recordValue;
                        break;
                    case "PUT":
                        // DNS 레코드 추가 또는 수정
                        String recordValueToUpdate = requestDataParts[3];
                        HashMap<String, String> updatedDomainRecords = dnsRecords.getOrDefault(domainName, new HashMap<>());
                        updatedDomainRecords.put(recordType, recordValueToUpdate);
                        dnsRecords.put(domainName, updatedDomainRecords);
                        response = "레코드가 성공적으로 추가 또는 수정되었습니다.";
                        break;
                    case "DELETE":
                        // DNS 레코드 삭제
                        HashMap<String, String> domainRecordsToDelete = dnsRecords.getOrDefault(domainName, new HashMap<>());
                        domainRecordsToDelete.remove(recordType);
                        dnsRecords.put(domainName, domainRecordsToDelete);
                        response = "레코드가 성공적으로 삭제되었습니다.";
                        break;
                    default:
                        response = "잘못된 쿼리 타입입니다.";
                }

                // 클라이언트에게 응답 전송
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
        // 예시 DNS 레코드 추가
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
            DNS서버시뮬레이터 server = new DNS서버시뮬레이터();
            server.setLocationRelativeTo(null); // 화면 가운데 정렬
            server.setVisible(true);
        });
    }
}
