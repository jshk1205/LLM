package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class 간단한프로그래밍언어처리기 extends JFrame {
    private JTextArea codeArea;
    private JTextArea outputArea;
    private Map<String, Integer> variables;
    private Map<String, String> functions;

    public 간단한프로그래밍언어처리기() {
        setTitle("간단한 프로그래밍 언어 해석기");
        setSize(600, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridLayout(2, 1));

        codeArea = new JTextArea();
        JScrollPane codeScrollPane = new JScrollPane(codeArea);
        panel.add(codeScrollPane);

        outputArea = new JTextArea();
        outputArea.setEditable(false);
        JScrollPane outputScrollPane = new JScrollPane(outputArea);
        panel.add(outputScrollPane);

        JButton runButton = new JButton("실행");
        runButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                interpretCode();
            }
        });

        JPanel buttonPanel = new JPanel(new FlowLayout());
        buttonPanel.add(runButton);

        add(panel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);
        setLocationRelativeTo(null);

        variables = new HashMap<>();
        functions = new HashMap<>();

        // 사용 방법 안내 추가
        JLabel usageLabel = new JLabel("<html><b>사용 방법</b><br>"
                + "1. 변수 할당: 'let 변수명 값' 형식으로 변수에 값을 할당합니다.<br>"
                + "   예시: let a 10 (a에 10을 할당)<br>"
                + "2. 변수 출력: 'print 변수명' 형식으로 변수의 값을 출력합니다.<br>"
                + "3. 조건문(if): 'if 조건' 형식으로 조건을 평가합니다.<br>"
                + "4. 반복문(for): 'for 시작값 to 종료값' 형식으로 반복문을 사용합니다.<br>"
                + "5. 반복문(while): 'while 조건' 형식으로 조건이 참일 때 반복합니다.<br>"
                + "6. 함수 정의: 'def 함수명 코드' 형식으로 함수를 정의합니다.<br>"
                + "7. 함수 호출: 'call 함수명' 형식으로 함수를 호출합니다.<br>"
                + "8. 파일 입출력: 'file 파일명' 형식으로 파일에서 데이터를 읽어옵니다.<br>"
                + "</html>");
        usageLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        add(usageLabel, BorderLayout.EAST);
    }

    private void interpretCode() {
        String code = codeArea.getText();
        String result = interpret(code);
        outputArea.setText(result);
    }

    private String interpret(String code) {
        StringBuilder output = new StringBuilder();

        String[] lines = code.split("\n");

        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty()) continue;

            String[] tokens = line.split(" ");

            if (tokens[0].equals("let")) {
                // 변수 할당 처리
                if (tokens.length != 3 || !tokens[2].matches("\\d+")) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                String variable = tokens[1];
                int value = Integer.parseInt(tokens[2]);
                variables.put(variable, value);
            } else if (tokens[0].equals("print")) {
                // 변수 출력 처리
                if (tokens.length != 2) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                String variable = tokens[1];
                if (!variables.containsKey(variable)) {
                    output.append("Undefined variable: ").append(variable).append("\n");
                    continue;
                }
                output.append(variables.get(variable)).append("\n");
            } else if (tokens[0].equals("if")) {
                // if 문 처리
                if (tokens.length != 2 || !tokens[1].matches("\\d+")) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                int value = Integer.parseInt(tokens[1]);
                if (value != 0) {
                    output.append("Condition is true\n");
                } else {
                    output.append("Condition is false\n");
                }
            } else if (tokens[0].equals("for")) {
                // for 루프 처리
                if (tokens.length != 4 || !tokens[1].matches("\\d+") || !tokens[3].matches("\\d+")) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                int start = Integer.parseInt(tokens[1]);
                int end = Integer.parseInt(tokens[3]);
                for (int i = start; i <= end; i++) {
                    output.append("Iteration: ").append(i).append("\n");
                }
            } else if (tokens[0].equals("while")) {
                // while 루프 처리
                if (tokens.length != 2 || !tokens[1].matches("\\d+")) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                int value = Integer.parseInt(tokens[1]);
                int i = 0;
                while (i < value) {
                    output.append("Iteration: ").append(i).append("\n");
                    i++;
                }
            } else if (tokens[0].equals("def")) {
                // 함수 정의 처리
                if (tokens.length < 3) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                String functionName = tokens[1];
                StringBuilder functionBody = new StringBuilder();
                for (int i = 2; i < tokens.length; i++) {
                    functionBody.append(tokens[i]).append(" ");
                }
                functions.put(functionName, functionBody.toString().trim());
            } else if (tokens[0].equals("call")) {
                // 함수 호출 처리
                if (tokens.length != 2) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                String functionName = tokens[1];
                if (!functions.containsKey(functionName)) {
                    output.append("Undefined function: ").append(functionName).append("\n");
                    continue;
                }
                String functionCall = functions.get(functionName);
                output.append(interpret(functionCall)).append("\n");
            } else if (tokens[0].equals("file")) {
                // 파일 입출력 처리
                if (tokens.length != 2) {
                    output.append("Invalid syntax: ").append(line).append("\n");
                    continue;
                }
                String fileName = tokens[1];
                try {
                    File file = new File(fileName);
                    Scanner scanner = new Scanner(file);
                    StringBuilder fileContent = new StringBuilder();
                    while (scanner.hasNextLine()) {
                        fileContent.append(scanner.nextLine()).append("\n");
                    }
                    scanner.close();
                    output.append(fileContent.toString());
                } catch (FileNotFoundException ex) {
                    output.append("File not found: ").append(fileName).append("\n");
                }
            } else {
                output.append("Invalid command: ").append(line).append("\n");
            }
        }

        return output.toString();
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new 간단한프로그래밍언어처리기().setVisible(true);
            }
        });
    }
}
