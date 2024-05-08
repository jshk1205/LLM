package com.example;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.data.category.DefaultCategoryDataset;

import javax.swing.*;
import java.awt.*;
import java.util.Timer;
import java.util.TimerTask;

public class 디스크사용량모니터 extends JFrame {
    private DefaultCategoryDataset dataset;
    private JLabel statusLabel;
    private long threshold;

    public 디스크사용량모니터() {
        super("Disk Usage Monitor");

        // Create dataset for disk usage
        dataset = new DefaultCategoryDataset();

        // Create chart
        JFreeChart chart = ChartFactory.createBarChart(
                "Disk Usage",
                "Type",
                "Disk Space (GB)",
                dataset
        );
        CategoryPlot plot = chart.getCategoryPlot();
        plot.getRenderer().setSeriesPaint(0, Color.BLUE);
        plot.getRenderer().setSeriesPaint(1, Color.GREEN);

        // Create panel to display chart
        ChartPanel chartPanel = new ChartPanel(chart);
        add(chartPanel, BorderLayout.CENTER);

        // Status label
        statusLabel = new JLabel("Status: ");
        add(statusLabel, BorderLayout.SOUTH);

        // Set up timer to update data periodically
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                updateData();
            }
        }, 0, 5000); // Update every 5 seconds

        // Set default threshold to 10% of total disk space
        threshold = getDiskTotalSpace() / 10;

        // Set up menu bar with settings option
        JMenuBar menuBar = new JMenuBar();
        JMenu settingsMenu = new JMenu("Settings");
        JMenuItem setThresholdItem = new JMenuItem("Set Threshold");
        setThresholdItem.addActionListener(e -> setThreshold());
        settingsMenu.add(setThresholdItem);
        menuBar.add(settingsMenu);
        setJMenuBar(menuBar);

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void updateData() {
        // Get disk usage information
        long totalSpace = getDiskTotalSpace();
        long freeSpace = getDiskFreeSpace();
        double usedSpaceGB = (totalSpace - freeSpace) / (1024.0 * 1024.0 * 1024.0); // Convert bytes to GB
        double freeSpaceGB = freeSpace / (1024.0 * 1024.0 * 1024.0); // Convert bytes to GB

        // Clear previous data
        dataset.clear();

        // Update dataset
        dataset.addValue(usedSpaceGB, "Used Space", "Disk");
        dataset.addValue(freeSpaceGB, "Free Space", "Disk");

        // Update status label
        String message = String.format("Used Disk Space: %.2f GB | Free Disk Space: %.2f GB", usedSpaceGB, freeSpaceGB);
        statusLabel.setText(message);

        // Check if free space is below threshold
        if (freeSpace < threshold) {
            notifyLowDiskSpace(freeSpace);
        }
    }

    private long getDiskTotalSpace() {
        return new java.io.File("/").getTotalSpace();
    }

    private long getDiskFreeSpace() {
        return new java.io.File("/").getFreeSpace();
    }

    private void notifyLowDiskSpace(long freeSpace) {
        // Show notification when free disk space is below threshold
        JOptionPane.showMessageDialog(this, "Low disk space warning!\nFree space remaining: " + freeSpace + " bytes",
                "Disk Space Warning", JOptionPane.WARNING_MESSAGE);
    }

    private void setThreshold() {
        String input = JOptionPane.showInputDialog(this, "Enter threshold (in GB):");
        try {
            threshold = (long) (Double.parseDouble(input) * 1024 * 1024 * 1024); // Convert GB to bytes
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Invalid input! Please enter a valid number.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(디스크사용량모니터::new);
    }
}
