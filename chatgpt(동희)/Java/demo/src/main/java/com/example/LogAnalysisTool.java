package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class LogAnalysisTool extends JFrame {
    private JTextField filePathTextField;
    private JTextArea logTextArea;
    private JButton selectFileButton;
    private JButton searchButton;
    private JButton patternSearchButton;
    private JButton statisticsButton;
    private JButton filterButton;
    private JButton graphButton;
    private File selectedFile;

    public LogAnalysisTool() {
        setTitle("Log Analysis Tool");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // Center on screen

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        filePathTextField = new JTextField();
        logTextArea = new JTextArea();
        JScrollPane scrollPane = new JScrollPane(logTextArea);

        selectFileButton = new JButton("Select File");
        selectFileButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                selectFile();
            }
        });

        searchButton = new JButton("Search Word");
        searchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                searchWord();
            }
        });

        patternSearchButton = new JButton("Search Pattern");
        patternSearchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                searchPattern();
            }
        });

        statisticsButton = new JButton("Log Statistics");
        statisticsButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                showStatistics();
            }
        });

        filterButton = new JButton("Filter Logs");
        filterButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                filterLogs();
            }
        });

        graphButton = new JButton("Visualize Logs");
        graphButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                visualizeLogs();
            }
        });

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(selectFileButton);
        buttonPanel.add(searchButton);
        buttonPanel.add(patternSearchButton);
        buttonPanel.add(statisticsButton);
        buttonPanel.add(filterButton);
        buttonPanel.add(graphButton);

        panel.add(filePathTextField, BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);

        add(panel);
        setVisible(true);
    }

    // Function to handle file selection
    private void selectFile() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            selectedFile = fileChooser.getSelectedFile();
            filePathTextField.setText(selectedFile.getAbsolutePath());
            loadLogFile(selectedFile);
        }
    }

    // Function to load log file content into the text area
    private void loadLogFile(File file) {
        try {
            FileInputStream fis = new FileInputStream(file);
            BufferedInputStream bis = new BufferedInputStream(fis);
            StringBuilder content = new StringBuilder();
            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = bis.read(buffer)) != -1) {
                content.append(new String(buffer, 0, bytesRead));
            }
            bis.close();
            fis.close();
            logTextArea.setText(content.toString());
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Error reading the file.");
        }
    }

    // Function to search for a specific word in the log content
    private void searchWord() {
        String word = JOptionPane.showInputDialog(this, "Enter the word to search for:");
        if (word != null && !word.isEmpty()) {
            String log = logTextArea.getText();
            int count = countOccurrences(log, word);
            JOptionPane.showMessageDialog(this, "Occurrences of word '" + word + "': " + count);
        }
    }

    // Function to count occurrences of a word in the log content
    private int countOccurrences(String text, String word) {
        int count = 0;
        int index = text.indexOf(word);
        while (index != -1) {
            count++;
            index = text.indexOf(word, index + 1);
        }
        return count;
    }

    // Function to search for a pattern in the log content
    private void searchPattern() {
        String pattern = JOptionPane.showInputDialog(this, "Enter the pattern to search for:");
        if (pattern != null && !pattern.isEmpty()) {
            String log = logTextArea.getText();
            Pattern regex = Pattern.compile(pattern);
            Matcher matcher = regex.matcher(log);
            StringBuffer matches = new StringBuffer();
            while (matcher.find()) {
                matches.append(matcher.group()).append("\n");
            }
            if (matches.length() > 0) {
                JTextArea matchesTextArea = new JTextArea(matches.toString());
                JScrollPane scrollPane = new JScrollPane(matchesTextArea);
                JOptionPane.showMessageDialog(this, scrollPane, "Pattern Matches", JOptionPane.PLAIN_MESSAGE);
            } else {
                JOptionPane.showMessageDialog(this, "No matches found.");
            }
        }
    }

    // Function to display log statistics
    private void showStatistics() {
        String log = logTextArea.getText();
        String[] lines = log.split("\n");
        int totalLines = lines.length;
        int totalWords = log.split("\\s+").length;
        int totalCharacters = log.length();
        JOptionPane.showMessageDialog(this, 
            "Total Lines: " + totalLines + "\n" +
            "Total Words: " + totalWords + "\n" +
            "Total Characters: " + totalCharacters);
    }

    // Function to filter logs based on a keyword
    private void filterLogs() {
        String filterText = JOptionPane.showInputDialog(this, "Enter the keyword to filter:");
        if (filterText != null && !filterText.isEmpty()) {
            String log = logTextArea.getText();
            log = log.replaceAll("(?i)" + Pattern.quote(filterText), "<b>" + filterText + "</b>");
            logTextArea.setText(log);
        }
    }

    // Function to visualize logs by generating a bar graph
    private void visualizeLogs() {
        String log = logTextArea.getText();
        String[] lines = log.split("\n");
        HashMap<String, Integer> wordFrequencyMap = new HashMap<>();
        
        // Important words
        String[] importantWords = {"error", "exception", "warning", "success", "failed"};

        // Calculate word frequencies
        for (String line : lines) {
            for (String word : importantWords) {
                if (line.toLowerCase().contains(word)) {
                    wordFrequencyMap.put(word, wordFrequencyMap.getOrDefault(word, 0) + 1);
                }
            }
        }

        // Create graph
        BarGraphPanel graphPanel = new BarGraphPanel(wordFrequencyMap);

        // Display graph
        JFrame graphFrame = new JFrame("Log Graph");
        graphFrame.add(graphPanel);
        graphFrame.setSize(600, 400);
        graphFrame.setLocationRelativeTo(this);
        graphFrame.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new LogAnalysisTool();
            }
        });
    }
}

class BarGraphPanel extends JPanel {
    private HashMap<String, Integer> data;

    public BarGraphPanel(HashMap<String, Integer> data) {
        this.data = data;
    }

    // Override the paintComponent method to draw the bar graph
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (data != null) {
            int barWidth = 50;
            int spacing = 20;
            int x = 30;
            int y = getHeight() - 50;

            int maxValue = Collections.max(data.values());
            double scaleFactor = (double) (getHeight() - 100) / maxValue;

            // Iterate over the data to draw bars
            for (Map.Entry<String, Integer> entry : data.entrySet()) {
                String key = entry.getKey();
                int value = entry.getValue();

                int barHeight = (int) (value * scaleFactor);
                g.setColor(Color.BLUE);
                g.fillRect(x, y - barHeight, barWidth, barHeight);
                g.setColor(Color.BLACK);
                g.drawRect(x, y - barHeight, barWidth, barHeight);

                g.drawString(key, x - 5, y + 15);

                x += (barWidth + spacing);
            }
        }
    }

    // Override the getPreferredSize method to set the preferred size of the panel
    @Override
    public Dimension getPreferredSize() {
        return new Dimension(600, 400);
    }
}
