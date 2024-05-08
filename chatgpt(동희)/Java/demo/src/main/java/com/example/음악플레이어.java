package com.example;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.sound.sampled.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class 음악플레이어 extends JFrame implements ActionListener {

    private JButton openButton, playButton, pauseButton, skipButton;
    private JLabel statusLabel, timeLabel, durationLabel;
    private JList<String> playlist;
    private DefaultListModel<String> playlistModel;
    private ArrayList<File> files;
    private Clip clip;
    private Timer timer;
    private JProgressBar progressBar;

    public 음악플레이어() {
        setTitle("음악 플레이어");
        setSize(1000, 300);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        JPanel mainPanel = new JPanel(new BorderLayout());
        JPanel controlPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        JPanel playlistPanel = new JPanel(new BorderLayout());

        openButton = new JButton("파일 열기");
        playButton = new JButton("재생");
        pauseButton = new JButton("멈춤");
        skipButton = new JButton("건너뛰기");
        statusLabel = new JLabel("상태: ");
        timeLabel = new JLabel("시간: 00:00");
        durationLabel = new JLabel("전체 시간: 00:00");
        progressBar = new JProgressBar(0, 100);

        openButton.addActionListener(this);
        playButton.addActionListener(this);
        pauseButton.addActionListener(this);
        skipButton.addActionListener(this);

        controlPanel.add(openButton);
        controlPanel.add(playButton);
        controlPanel.add(pauseButton);
        controlPanel.add(skipButton);
        controlPanel.add(statusLabel);
        controlPanel.add(timeLabel);
        controlPanel.add(durationLabel);
        controlPanel.add(progressBar);

        playlistModel = new DefaultListModel<>();
        playlist = new JList<>(playlistModel);
        JScrollPane playlistScrollPane = new JScrollPane(playlist);
        playlistPanel.add(new JLabel("플레이리스트"), BorderLayout.NORTH);
        playlistPanel.add(playlistScrollPane, BorderLayout.CENTER);

        mainPanel.add(controlPanel, BorderLayout.NORTH);
        mainPanel.add(playlistPanel, BorderLayout.CENTER);
        mainPanel.setBorder(new EmptyBorder(10, 10, 10, 10));
        add(mainPanel);

        files = new ArrayList<>();

        timer = new Timer(1000, e -> updateProgressBar());
        timer.setInitialDelay(0);
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == openButton) {
            openFile();
        } else if (e.getSource() == playButton) {
            play();
        } else if (e.getSource() == pauseButton) {
            pause();
        } else if (e.getSource() == skipButton) {
            skip();
        }
    }

    private void openFile() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setMultiSelectionEnabled(true);
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File[] selectedFiles = fileChooser.getSelectedFiles();
            for (File file : selectedFiles) {
                if (file.isFile()) {
                    files.add(file);
                    playlistModel.addElement(file.getName());
                }
            }
        }
    }

    private void play() {
        if (!files.isEmpty()) {
            File selectedFile = files.get(playlist.getSelectedIndex());
            try {
                AudioInputStream audioIn = AudioSystem.getAudioInputStream(selectedFile);
                clip = AudioSystem.getClip();
                clip.open(audioIn);
                clip.start();
                updateTime();
                updateDuration();
                timer.start();
                statusLabel.setText("상태: 재생 중");
            } catch (UnsupportedAudioFileException | IOException | LineUnavailableException ex) {
                ex.printStackTrace();
            }
        }
    }

    private void pause() {
        if (clip != null && clip.isRunning()) {
            clip.stop();
            timer.stop();
            statusLabel.setText("상태: 일시정지");
        }
    }

    private void skip() {
        if (clip != null && clip.isRunning()) {
            clip.stop();
            timer.stop();
            statusLabel.setText("상태: 멈춤");
        }
    }

    private void updateTime() {
        if (clip != null) {
            long microsecondPosition = clip.getMicrosecondPosition();
            long totalSeconds = microsecondPosition / 1_000_000;
            long minutes = totalSeconds / 60;
            long seconds = totalSeconds % 60;
            timeLabel.setText(String.format("시간: %02d:%02d", minutes, seconds));
        }
    }

    private void updateDuration() {
        if (clip != null) {
            long totalMicroseconds = clip.getMicrosecondLength();
            long totalSeconds = totalMicroseconds / 1_000_000;
            long minutes = totalSeconds / 60;
            long seconds = totalSeconds % 60;
            durationLabel.setText(String.format("전체 시간: %02d:%02d", minutes, seconds));
        }
    }

    private void updateProgressBar() {
        updateTime();
        if (!clip.isRunning()) {
            timer.stop();
            progressBar.setValue(0);
            statusLabel.setText("상태: 멈춤");
        } else {
            long microsecondPosition = clip.getMicrosecondPosition();
            long totalMicroseconds = clip.getMicrosecondLength();
            double percentage = (double) microsecondPosition / totalMicroseconds * 100;
            progressBar.setValue((int) percentage);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            음악플레이어 player = new 음악플레이어();
            player.setVisible(true);
        });
    }
}
