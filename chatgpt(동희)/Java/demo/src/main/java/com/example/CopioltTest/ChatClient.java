//실시간 채팅주제로 클라이언트에 관한 코드를 만들어줘
//실시간 채팅은 소켓통신을 이용해야해
//서버는 클라이언트의 요청을 받아들이고, 클라이언트들에게 메시지를 전달해줘
//클라이언트에서 메세지를 보내면 서버는 그 메시지를 서버에 출력해줘
//서버는 클라이언트들에게 메시지를 전달해줘
// 자바 swing으로 채팅 GUI를 구현해서  서버에 접속하면 채팅이 가능하게 해줘

package com.example.CopioltTest;
import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ChatClient extends JFrame {
    private JTextArea jta = new JTextArea();
    private JTextField jtf = new JTextField();
    private BufferedReader br;
    private PrintWriter pw;
    private Socket socket;

    public ChatClient() {
        add(new JScrollPane(jta), BorderLayout.CENTER);
        add(jtf, BorderLayout.SOUTH);
        jta.setEditable(false);
        jtf.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String msg = jtf.getText();
                if (msg.equals(""))
                    return;
                pw.println(msg);
                pw.flush();
                jta.append("Client: " + msg + "\n");
                jtf.setText("");
            }
        });
        setTitle("Chat Client");
        setSize(400, 200);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
        try {
            socket = new Socket("localhost", 7777);
            br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            pw = new PrintWriter(socket.getOutputStream(), true);
            while (true) {
                String msg = br.readLine();
                jta.append("Server: " + msg + "\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new ChatClient();
    }
}
