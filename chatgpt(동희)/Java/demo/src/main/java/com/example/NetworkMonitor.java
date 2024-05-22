package com.example;
import javax.swing.*;
import java.awt.*;
import java.io.FileWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.io.File;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;
import java.lang.management.OperatingSystemMXBean;


import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.category.DefaultCategoryDataset;

public class NetworkMonitor extends JFrame implements Customizable, Multithreadable, ResourceMonitorable {
    private DefaultCategoryDataset dataset;
    private Timer timer;
    private String[] labels;
    private int[] values;
    private JLabel statusLabel;
    private FileWriter logFileWriter;
    private JTextArea infoTextArea; // 실시간 정보를 표시할 텍스트 영역

    public NetworkMonitor() {
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

        // Info text area for displaying real-time information
        infoTextArea = new JTextArea(5, 20);
        infoTextArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(infoTextArea);
        add(scrollPane, BorderLayout.SOUTH);

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

        // Update info text area
        updateInfoTextArea(downloadSpeed, uploadSpeed);

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
        // based on user preferences and configured settings
        JOptionPane.showMessageDialog(this, "Network speed exceeded threshold. Download: " + downloadSpeed + " KB/s, Upload: " + uploadSpeed + " KB/s");
    }

    private void updateInfoTextArea(int downloadSpeed, int uploadSpeed) {
        // Update info text area with real-time network speed information
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String currentTime = dateFormat.format(new Date());
        String info = currentTime + " - Download: " + downloadSpeed + " KB/s, Upload: " + uploadSpeed + " KB/s\n";

        // Append the new information to the text area
        infoTextArea.append(info);

        // Ensure that the new information is visible by scrolling to the bottom
        infoTextArea.setCaretPosition(infoTextArea.getDocument().getLength());
    }

    @Override
    public void customizeSettings() {
    // 사용자에게 설정을 입력하도록 다이얼로그를 띄웁니다.
    String input = JOptionPane.showInputDialog(this, "Enter your custom setting:");

    // 사용자가 입력한 설정을 콘솔에 출력합니다.
    if (input != null && !input.isEmpty()) {
        System.out.println("Custom setting entered: " + input);
        // 설정을 파일에 저장합니다.
        saveSettingsToFile(input);
    } else {
        System.out.println("No custom setting entered.");
    }
    }

// 설정을 파일에 저장하는 메서드
    private void saveSettingsToFile(String setting) {
    File file = new File("settings.txt");
    try (FileWriter writer = new FileWriter(file)) {
        writer.write(setting);
        writer.flush();
        System.out.println("Settings saved to file.");
    } catch (IOException e) {
        e.printStackTrace();
    }
    }

    @Override
    public void multiThreading() {
        // Move time-consuming tasks to background threads to improve responsiveness
        new Thread(() -> {
            SwingUtilities.invokeLater(() -> {
                JOptionPane.showMessageDialog(this, "Multi-threading feature coming soon!");
            });
        }).start();
    }

    @Override
    public void systemResourceMonitoring() {
        // Monitor CPU, memory, disk usage, etc.
        try {
            InetAddress localhost = InetAddress.getLocalHost();
            String hostName = localhost.getHostName();
            String hostAddress = localhost.getHostAddress();
            String message = "System resource monitoring:\nHost Name: " + hostName + "\nHost Address: " + hostAddress;

            // Get CPU usage
            double cpuUsage = getCPUUsage();
            message += "\nCPU Usage: " + cpuUsage + "%";

            // Get memory usage
            double memoryUsage = getMemoryUsage();
            message += "\nMemory Usage: " + memoryUsage + "%";

            // Get disk usage
            double diskUsage = getDiskUsage();
            message += "\nDisk Usage: " + diskUsage + "%";

            // Update info text area with system resource information
            infoTextArea.append(message + "\n");

            // Ensure that the new information is visible by scrolling to the bottom
            infoTextArea.setCaretPosition(infoTextArea.getDocument().getLength());
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }

    private double getCPUUsage() {
        OperatingSystemMXBean osBean = (OperatingSystemMXBean) ManagementFactory.getOperatingSystemMXBean();
        return osBean.getSystemLoadAverage();
    }

    private double getMemoryUsage() {
        MemoryMXBean memoryBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heapMemoryUsage = memoryBean.getHeapMemoryUsage();
        long usedMemory = heapMemoryUsage.getUsed();
        long maxMemory = heapMemoryUsage.getMax();
        return (double) usedMemory / maxMemory * 100;
        }
    
    private double getDiskUsage() {
            File diskPartition = new File("/");
            long totalSpace = diskPartition.getTotalSpace();
            long freeSpace = diskPartition.getFreeSpace();
            return ((double) (totalSpace - freeSpace) / totalSpace) * 100;
        }
        
        public static void main(String[] args) {
            SwingUtilities.invokeLater(NetworkMonitor::new);
        }
    }

    interface Customizable {
    void customizeSettings();
    }
    
    interface Multithreadable {
    void multiThreading();
    }
    
    interface ResourceMonitorable {
    void systemResourceMonitoring();
    }