package com.example;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableRowSorter;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class 시스템이벤트로그모니터 extends JFrame {
    private DefaultTableModel tableModel;
    private JTable eventTable;
    private JButton refreshButton;
    private JTextField filterField;
    private JButton filterButton;
    private Timer timer;
    private int updateIntervalSeconds = 5; // Default update interval: 5 seconds

    public 시스템이벤트로그모니터() {
        super("System Event Log Monitor");

        // Create table to display event log
        tableModel = new DefaultTableModel();
        tableModel.addColumn("Time");
        tableModel.addColumn("Event ID");
        tableModel.addColumn("Type");
        tableModel.addColumn("Message");
        eventTable = new JTable(tableModel);
        JScrollPane scrollPane = new JScrollPane(eventTable);
        add(scrollPane, BorderLayout.CENTER);

        // Create panel for buttons and filter field
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        buttonPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
        refreshButton = new JButton("Refresh");
        filterField = new JTextField(20);
        filterButton = new JButton("Filter");
        buttonPanel.add(refreshButton);
        buttonPanel.add(new JLabel("Filter:"));
        buttonPanel.add(filterField);
        buttonPanel.add(filterButton);
        add(buttonPanel, BorderLayout.SOUTH);

        // Set up timer to update event log periodically
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                updateEventLog();
            }
        }, 0, updateIntervalSeconds * 1000);

        // Add action listeners to buttons
        refreshButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateEventLog();
            }
        });

        filterButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                applyFilter();
            }
        });

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void updateEventLog() {
        List<String[]> eventList = getEventLog();
        tableModel.setRowCount(0); // Clear existing data
        for (String[] event : eventList) {
            tableModel.addRow(event);
        }
    }

    private List<String[]> getEventLog() {
        List<String[]> events = new ArrayList<>();
        try {
            ProcessBuilder builder = new ProcessBuilder("powershell", "-Command", "$OutputEncoding = [System.Console]::OutputEncoding; Get-EventLog -LogName System -Newest 50 | Select TimeGenerated, EventID, EntryType, Message");
            Process process = builder.start();
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream(), "MS949")); // 인코딩을 UTF-8로 설정
            String line;
            while ((line = reader.readLine()) != null) {
                if (!line.trim().isEmpty()) {
                    String[] eventData = line.trim().split("\\s+", 4);
                    events.add(eventData);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return events;
    }
    
    

    private void applyFilter() {
        String filterText = filterField.getText().trim();
        if (!filterText.isEmpty()) {
            TableRowSorter<DefaultTableModel> sorter = new TableRowSorter<>(tableModel);
            eventTable.setRowSorter(sorter);
            RowFilter<DefaultTableModel, Object> rowFilter = RowFilter.regexFilter("(?i)" + filterText); // Case-insensitive filter
            sorter.setRowFilter(rowFilter);
        } else {
            eventTable.setRowSorter(null); // Remove filter
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(시스템이벤트로그모니터::new);
    }
}

