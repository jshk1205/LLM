package com.example;

import javax.swing.*;
import java.awt.*;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.category.DefaultCategoryDataset;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class 네트워크모니터 extends JFrame {
    private DefaultCategoryDataset dataset;
    private Timer timer;
    private String[] labels;
    private int[] values;
    private JLabel statusLabel;
    private FileWriter logFileWriter;

    public 네트워크모니터() {
        super("Network Monitor");

        dataset = new DefaultCategoryDataset();
        labels = new String[]{"Download", "Upload"};
        values = new int[]{0, 0};

        // Create chart
        JFreeChart chart = ChartFactory.createLineChart(
                "Network Monitor",
                "Time",
                "Speed (KB/s)",
                dataset
        );

        // Create panel to display chart
        ChartPanel chartPanel = new ChartPanel(chart);
        add(chartPanel, BorderLayout.CENTER);

        // Status label
        statusLabel = new JLabel("Status: ");
        add(statusLabel, BorderLayout.SOUTH);

        // Set up timer to update data periodically
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                updateData();
            }
        }, 0, 1000);

        // Create log file
        try {
            logFileWriter = new FileWriter("network_log.txt");
        } catch (IOException e) {
            e.printStackTrace();
        }

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void updateData() {
        // Simulate network speed data
        int downloadSpeed = (int) (Math.random() * 1000);
        int uploadSpeed = (int) (Math.random() * 1000);

        // Update dataset
        dataset.addValue(downloadSpeed, "Download", String.valueOf(values[0]++));
        dataset.addValue(uploadSpeed, "Upload", String.valueOf(values[1]++));

        // Update status label
        statusLabel.setText("Status: Data updated");

        // Log network speed
        logNetworkSpeed(downloadSpeed, uploadSpeed);

        // Send notification if speed exceeds threshold
        if (downloadSpeed > 800 || uploadSpeed > 800) {
            sendNotification(downloadSpeed, uploadSpeed);
        }
    }

    private void logNetworkSpeed(int downloadSpeed, int uploadSpeed) {
        // Get current time
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String currentTime = dateFormat.format(new Date());

        // Create log message
        String logMessage = currentTime + " - Download: " + downloadSpeed + " KB/s, Upload: " + uploadSpeed + " KB/s\n";

        // Write log message to file
        try {
            logFileWriter.write(logMessage);
            logFileWriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendNotification(int downloadSpeed, int uploadSpeed) {
        // Implement notification feature here
        // You can use email, SMS, or other notification methods
        // based on user preferences and configured settings
        JOptionPane.showMessageDialog(this, "Network speed exceeded threshold. Download: " + downloadSpeed + " KB/s, Upload: " + uploadSpeed + " KB/s");
    }

    private void customizeSettings() {
        // Implement settings customization feature here
        // Allow users to customize program behavior, thresholds, etc.
        JOptionPane.showMessageDialog(this, "Settings customization feature coming soon!");
    }

    private void multiThreading() {
        // Implement multi-threading for better performance here
        // Move time-consuming tasks to background threads to improve responsiveness
        SwingUtilities.invokeLater(() -> {
            JOptionPane.showMessageDialog(this, "Multi-threading feature coming soon!");
        });
    }

    private void systemResourceMonitoring() {
        // Implement system resource monitoring feature here
        // Monitor CPU, memory, disk usage, etc.
        try {
            InetAddress localhost = InetAddress.getLocalHost();
            String hostName = localhost.getHostName();
            String hostAddress = localhost.getHostAddress();
            JOptionPane.showMessageDialog(this, "System resource monitoring coming soon!\nHost Name: " + hostName + "\nHost Address: " + hostAddress);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(네트워크모니터::new);
    }
}
