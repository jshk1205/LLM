package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class SimpleProgrammingLanguageInterpreter extends JFrame {
    private JTextArea codeArea;
    private JTextArea outputArea;
    private Map<String, Integer> variables;
    private Map<String, String> functions;

    public SimpleProgrammingLanguageInterpreter() {
        setTitle("Simple Programming Language Interpreter");
        setSize(600, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // Panel to hold the code area and output area
        JPanel panel = new JPanel(new GridLayout(2, 1));

        // Text area for code input
        codeArea = new JTextArea();
        JScrollPane codeScrollPane = new JScrollPane(codeArea);
        panel.add(codeScrollPane);

        // Text area for output
        outputArea = new JTextArea();
        outputArea.setEditable(false);
        JScrollPane outputScrollPane = new JScrollPane(outputArea);
        panel.add(outputScrollPane);

        // Run button to interpret the code
        JButton runButton = new JButton("Run");
        runButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                interpretCode();
            }
        });

        // Panel to hold the run button
        JPanel buttonPanel = new JPanel(new FlowLayout());
        buttonPanel.add(runButton);

        add(panel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);
        setLocationRelativeTo(null);

        // Initialize variable and function maps
        variables = new HashMap<>();
        functions = new HashMap<>();

        // Usage instructions
        JLabel usageLabel = new JLabel("<html><b>Usage Instructions</b><br>"
                + "1. Variable assignment: 'let variable value' to assign a value to a variable.<br>"
                + "   Example: let a 10 (assigns 10 to a)<br>"
                + "2. Print variable: 'print variable' to print the value of a variable.<br>"
                + "3. Conditional (if): 'if condition' to evaluate a condition.<br>"
                + "4. Loop (for): 'for start to end' to create a for loop.<br>"
                + "5. Loop (while): 'while condition' to create a while loop.<br>"
                + "6. Function definition: 'def function code' to define a function.<br>"
                + "7. Function call: 'call function' to call a function.<br>"
                + "8. File I/O: 'file filename' to read data from a file.<br>"
                + "</html>");
        usageLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        add(usageLabel, BorderLayout.EAST);
    }

    // Method to interpret the code from the code area
    private void interpretCode() {
        String code = codeArea.getText();
        String result = interpret(code);
        outputArea.setText(result);
    }

    // Method to interpret a given block of code
    private String interpret(String code) {
        StringBuilder output = new StringBuilder();
        String[] lines = code.split("\n");

        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty()) continue;

            String[] tokens = line.split(" ");

            switch (tokens[0]) {
                case "let":
                    // Handle variable assignment
                    if (tokens.length != 3 || !tokens[2].matches("\\d+")) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    String variable = tokens[1];
                    int value = Integer.parseInt(tokens[2]);
                    variables.put(variable, value);
                    break;
                case "print":
                    // Handle printing variable value
                    if (tokens.length != 2) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    variable = tokens[1];
                    if (!variables.containsKey(variable)) {
                        output.append("Undefined variable: ").append(variable).append("\n");
                        continue;
                    }
                    output.append(variables.get(variable)).append("\n");
                    break;
                case "if":
                    // Handle if condition
                    if (tokens.length != 2 || !tokens[1].matches("\\d+")) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    value = Integer.parseInt(tokens[1]);
                    if (value != 0) {
                        output.append("Condition is true\n");
                    } else {
                        output.append("Condition is false\n");
                    }
                    break;
                case "for":
                    // Handle for loop
                    if (tokens.length != 4 || !tokens[1].matches("\\d+") || !tokens[3].matches("\\d+")) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    int start = Integer.parseInt(tokens[1]);
                    int end = Integer.parseInt(tokens[3]);
                    for (int i = start; i <= end; i++) {
                        output.append("Iteration: ").append(i).append("\n");
                    }
                    break;
                case "while":
                    // Handle while loop
                    if (tokens.length != 2 || !tokens[1].matches("\\d+")) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    value = Integer.parseInt(tokens[1]);
                    int i = 0;
                    while (i < value) {
                        output.append("Iteration: ").append(i).append("\n");
                        i++;
                    }
                    break;
                case "def":
                    // Handle function definition
                    if (tokens.length < 3) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    String functionName = tokens[1];
                    StringBuilder functionBody = new StringBuilder();
                    for (int j = 2; j < tokens.length; j++) {
                        functionBody.append(tokens[j]).append(" ");
                    }
                    functions.put(functionName, functionBody.toString().trim());
                    break;
                case "call":
                    // Handle function call
                    if (tokens.length != 2) {
                        output.append("Invalid syntax: ").append(line).append("\n");
                        continue;
                    }
                    functionName = tokens[1];
                    if (!functions.containsKey(functionName)) {
                        output.append("Undefined function: ").append(functionName).append("\n");
                        continue;
                    }
                    String functionCall = functions.get(functionName);
                    output.append(interpret(functionCall)).append("\n");
                    break;
                case "file":
                    // Handle file I/O
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
                    break;
                default:
                    output.append("Invalid command: ").append(line).append("\n");
                    break;
            }
        }

        return output.toString();
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new SimpleProgrammingLanguageInterpreter().setVisible(true);
            }
        });
    }
}
