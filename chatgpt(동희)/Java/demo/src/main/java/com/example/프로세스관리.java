package com.example;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class 프로세스관리 extends JFrame {
    private DefaultTableModel processTableModel;
    private JTable processTable;
    private Timer timer;
    private int updateIntervalSeconds = 5; // Default update interval: 5 seconds

    public 프로세스관리() {
        super("Process Manager");

        // Create table to display process list
        processTableModel = new DefaultTableModel();
        processTableModel.addColumn("Image Name");
        processTableModel.addColumn("PID");
        processTableModel.addColumn("Session Name");
        processTableModel.addColumn("Session#");
        processTableModel.addColumn("Mem Usage");

        processTable = new JTable(processTableModel);
        JScrollPane scrollPane = new JScrollPane(processTable);
        add(scrollPane, BorderLayout.CENTER);

        // Create panel for buttons
        JPanel buttonPanel = new JPanel();
        JButton refreshButton = new JButton("Refresh");
        JButton killButton = new JButton("Kill Process");
        JTextField pidField = new JTextField(10);
        buttonPanel.add(refreshButton);
        buttonPanel.add(new JLabel("PID:"));
        buttonPanel.add(pidField);
        buttonPanel.add(killButton);
        add(buttonPanel, BorderLayout.SOUTH);

        // Set up timer to update process list periodically
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                updateProcessList();
            }
        }, 0, updateIntervalSeconds * 1000);

        // Add action listeners to buttons
        refreshButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateProcessList();
            }
        });

        killButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String pid = pidField.getText();
                if (!pid.isEmpty()) {
                    killProcess(pid);
                } else {
                    JOptionPane.showMessageDialog(프로세스관리.this, "Please enter a PID.", "Error", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void updateProcessList() {
        List<String[]> processList = getProcessList();
        processTableModel.setRowCount(0);
        for (String[] process : processList) {
            processTableModel.addRow(process);
        }
    }

    private List<String[]> getProcessList() {
        List<String[]> processes = new ArrayList<>();
        try {
            ProcessBuilder builder = new ProcessBuilder("tasklist", "/fo", "csv");
            Process process = builder.start();
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            // Skip the first line (header)
            reader.readLine();
            while ((line = reader.readLine()) != null) {
                // Split the CSV line
                String[] parts = line.split(",");
                // Add the parts to the processes list
                processes.add(parts);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return processes;
    }

    private void killProcess(String pid) {
        try {
            ProcessBuilder builder = new ProcessBuilder("taskkill", "/F", "/PID", pid);
            builder.redirectErrorStream(true);
            Process process = builder.start();
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            int exitCode = process.waitFor();
            if (exitCode == 0) {
                JOptionPane.showMessageDialog(this, "Process with PID " + pid + " has been terminated.");
            } else {
                JOptionPane.showMessageDialog(this, "Failed to kill process with PID " + pid, "Error", JOptionPane.ERROR_MESSAGE);
            }
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(프로세스관리::new);
    }
}
