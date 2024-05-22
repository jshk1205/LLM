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

public class MusicPlayer extends JFrame implements ActionListener {

    private JButton openButton, playButton, pauseButton, skipButton;
    private JLabel statusLabel, timeLabel, durationLabel;
    private JList<String> playlist;
    private DefaultListModel<String> playlistModel;
    private ArrayList<File> files;
    private Clip clip;
    private Timer timer;
    private JProgressBar progressBar;

    public MusicPlayer() {
        setTitle("Music Player");
        setSize(1000, 300);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        JPanel mainPanel = new JPanel(new BorderLayout());
        JPanel controlPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        JPanel playlistPanel = new JPanel(new BorderLayout());

        openButton = new JButton("Open File");
        playButton = new JButton("Play");
        pauseButton = new JButton("Pause");
        skipButton = new JButton("Skip");
        statusLabel = new JLabel("Status: ");
        timeLabel = new JLabel("Time: 00:00");
        durationLabel = new JLabel("Duration: 00:00");
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
        playlistPanel.add(new JLabel("Playlist"), BorderLayout.NORTH);
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

    // Open audio files
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

    // Play the selected audio file
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
                statusLabel.setText("Status: Playing");
            } catch (UnsupportedAudioFileException | IOException | LineUnavailableException ex) {
                ex.printStackTrace();
            }
        }
    }

    // Pause the currently playing audio
    private void pause() {
        if (clip != null && clip.isRunning()) {
            clip.stop();
            timer.stop();
            statusLabel.setText("Status: Paused");
        }
    }

    // Skip the currently playing audio
    private void skip() {
        if (clip != null && clip.isRunning()) {
            clip.stop();
            timer.stop();
            statusLabel.setText("Status: Stopped");
        }
    }

    // Update the current playback time
    private void updateTime() {
        if (clip != null) {
            long microsecondPosition = clip.getMicrosecondPosition();
            long totalSeconds = microsecondPosition / 1_000_000;
            long minutes = totalSeconds / 60;
            long seconds = totalSeconds % 60;
            timeLabel.setText(String.format("Time: %02d:%02d", minutes, seconds));
        }
    }

    // Update the total duration of the audio
    private void updateDuration() {
        if (clip != null) {
            long totalMicroseconds = clip.getMicrosecondLength();
            long totalSeconds = totalMicroseconds / 1_000_000;
            long minutes = totalSeconds / 60;
            long seconds = totalSeconds % 60;
            durationLabel.setText(String.format("Duration: %02d:%02d", minutes, seconds));
        }
    }

    // Update the progress bar based on the current playback position
    private void updateProgressBar() {
        updateTime();
        if (!clip.isRunning()) {
            timer.stop();
            progressBar.setValue(0);
            statusLabel.setText("Status: Stopped");
        } else {
            long microsecondPosition = clip.getMicrosecondPosition();
            long totalMicroseconds = clip.getMicrosecondLength();
            double percentage = (double) microsecondPosition / totalMicroseconds * 100;
            progressBar.setValue((int) percentage);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            MusicPlayer player = new MusicPlayer();
            player.setVisible(true);
        });
    }
}
