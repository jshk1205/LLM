//실시간 채팅주제로 서버에 관한 코드를 만들어줘
//실시간 채팅은 소켓통신을 이용해야해
//서버는 클라이언트의 요청을 받아들이고, 클라이언트들에게 메시지를 전달해줘
//클라이언트에서 메세지를 보내면 서버는 그 메시지를 서버에 출력해줘
//서버는 클라이언트들에게 메시지를 전달해줘
// 자바 swing으로 채팅 GUI를 구현해서  서버에 접속하면 채팅이 가능하게 해줘

import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ChatServer extends JFrame {
    private JTextArea jta = new JTextArea();
    private JTextField jtf = new JTextField();
    private ServerSocket serverSocket;
    private Socket socket;
    private HashMap<String, PrintWriter> hm = new HashMap<String, PrintWriter>();

    public ChatServer() {
        add(new JScrollPane(jta), BorderLayout.CENTER);
        add(jtf, BorderLayout.SOUTH);
        jta.setEditable(false);
        jtf.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                PrintWriter pw;
                String msg = jtf.getText();
                if (msg.equals(""))
                    return;
                jta.append("Server: " + msg + "\n");
                for (String key : hm.keySet()) {
                    pw = (PrintWriter) hm.get(key);
                    pw.println(msg);
                    pw.flush();
                }
                jtf.setText("");
            }
        });
        setTitle("Chat Server");
        setSize(400, 200);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
        try {
            serverSocket = new ServerSocket(7777);
            jta.append("Server ready...\n");
            while (true) {
                socket = serverSocket.accept();
                ChatServerThread chatServerThread = new ChatServerThread();
                chatServerThread.start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    class ChatServerThread extends Thread {
        private String id;
        private BufferedReader br;
        private PrintWriter pw;

        public void run() {
            try {
                br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                pw = new PrintWriter(socket.getOutputStream());
                id = br.readLine();
                hm.put(id, pw);
                jta.append("New client connected: " + id + "\n");
                while (true) {
                    String msg = br.readLine();
                    if (msg == null) {
                        break;
                    }
                    jta.append(id + ": " + msg + "\n");
                    for (String key : hm.keySet()) {
                        pw = (PrintWriter) hm.get(key);
                        pw.println(id + ": " + msg);
                        pw.flush();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                hm.remove(id);
                jta.append(id + " has left\n");
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[]
            args) {
        new ChatServer();
    }
}

