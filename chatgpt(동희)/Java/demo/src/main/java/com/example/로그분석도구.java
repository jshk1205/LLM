package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class 로그분석도구 extends JFrame {
    private JTextField filePathTextField;
    private JTextArea logTextArea;
    private JButton selectFileButton;
    private JButton searchButton;
    private JButton patternSearchButton;
    private JButton statisticsButton;
    private JButton filterButton;
    private JButton graphButton;
    private File selectedFile;

    public 로그분석도구() {
        setTitle("로그 분석 도구");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // 화면 중앙에 표시

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        filePathTextField = new JTextField();
        logTextArea = new JTextArea();
        JScrollPane scrollPane = new JScrollPane(logTextArea);

        selectFileButton = new JButton("파일 선택");
        selectFileButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                selectFile();
            }
        });

        searchButton = new JButton("단어 검색");
        searchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                searchWord();
            }
        });

        patternSearchButton = new JButton("패턴 검색");
        patternSearchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                searchPattern();
            }
        });

        statisticsButton = new JButton("로그 통계");
        statisticsButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                showStatistics();
            }
        });

        filterButton = new JButton("로그 필터링");
        filterButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                filterLogs();
            }
        });

        graphButton = new JButton("로그 그래프화");
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

    private void selectFile() {
        JFileChooser fileChooser = new JFileChooser();
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            selectedFile = fileChooser.getSelectedFile();
            filePathTextField.setText(selectedFile.getAbsolutePath());
            loadLogFile(selectedFile);
        }
    }

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
            JOptionPane.showMessageDialog(this, "파일을 읽는 도중 오류가 발생했습니다.");
        }
    }

    private void searchWord() {
        String word = JOptionPane.showInputDialog(this, "검색할 단어를 입력하세요.");
        if (word != null && !word.isEmpty()) {
            String log = logTextArea.getText();
            int count = countOccurrences(log, word);
            JOptionPane.showMessageDialog(this, "단어 '" + word + "'의 출현 횟수: " + count);
        }
    }

    private int countOccurrences(String text, String word) {
        int count = 0;
        int index = text.indexOf(word);
        while (index != -1) {
            count++;
            index = text.indexOf(word, index + 1);
        }
        return count;
    }

    private void searchPattern() {
        String pattern = JOptionPane.showInputDialog(this, "검색할 패턴을 입력하세요.");
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
                JOptionPane.showMessageDialog(this, scrollPane, "패턴에 일치하는 내용", JOptionPane.PLAIN_MESSAGE);
            } else {
                JOptionPane.showMessageDialog(this, "일치하는 내용을 찾을 수 없습니다.");
            }
        }
    }
    

    private void showStatistics() {
        String log = logTextArea.getText();
        String[] lines = log.split("\n");
        int totalLines = lines.length;
        int totalWords = log.split("\\s+").length;
        int totalCharacters = log.length();
        JOptionPane.showMessageDialog(this, 
            "총 라인 수: " + totalLines + "\n" +
            "총 단어 수: " + totalWords + "\n" +
            "총 문자 수: " + totalCharacters);
    }

    private void filterLogs() {
        String filterText = JOptionPane.showInputDialog(this, "필터링할 키워드를 입력하세요.");
        if (filterText != null && !filterText.isEmpty()) {
            String log = logTextArea.getText();
            log = log.replaceAll("(?i)" + Pattern.quote(filterText), "<b>" + filterText + "</b>");
            logTextArea.setText(log);
        }
    }

    private void visualizeLogs() {
        String log = logTextArea.getText();
        String[] lines = log.split("\n");
        HashMap<String, Integer> wordFrequencyMap = new HashMap<>();
        
        // 중요한 단어들
        String[] importantWords = {"error", "exception", "warning", "success", "failed"};

        // 단어 빈도수 계산
        for (String line : lines) {
            for (String word : importantWords) {
                if (line.toLowerCase().contains(word)) {
                    wordFrequencyMap.put(word, wordFrequencyMap.getOrDefault(word, 0) + 1);
                }
            }
        }

        // 그래프 생성
        BarGraphPanel graphPanel = new BarGraphPanel(wordFrequencyMap);

        // 그래프 출력
        JFrame graphFrame = new JFrame("로그 그래프");
        graphFrame.add(graphPanel);
        graphFrame.setSize(600, 400);
        graphFrame.setLocationRelativeTo(this);
        graphFrame.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new 로그분석도구();
            }
        });
    }
}

class BarGraphPanel extends JPanel {
    private HashMap<String, Integer> data;

    public BarGraphPanel(HashMap<String, Integer> data) {
        this.data = data;
    }

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

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(600, 400);
    }
}
