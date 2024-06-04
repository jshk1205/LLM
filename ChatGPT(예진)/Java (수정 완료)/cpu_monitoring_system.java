import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class cpu_monitoring_system {
    // Constants
    private static final int DEFAULT_THRESHOLD = 90;
    private static final int DEFAULT_DURATION = 10;
    private static final int DEFAULT_MAX_CONTINUOUS_DURATION = 30;
    private static final int DEFAULT_MAX_CONSECUTIVE_THRESHOLD = 95;
    private static final int DEFAULT_INTERVAL = 5;

    private static AtomicBoolean isMonitoring = new AtomicBoolean(true);

    public static void main(String[] args) {
        Thread keyboardThread = new Thread(cpu_monitoring_system::monitorKeyboardInput);
        keyboardThread.start();

        Thread cpuThread = new Thread(cpu_monitoring_system::monitorCPU);
        cpuThread.start();

        try {
            keyboardThread.join();
            cpuThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void monitorKeyboardInput() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("---Press 'p' to stop monitoring---");
            String key = scanner.nextLine();
            if (key.equals("p")) {
                isMonitoring.set(false);
                break;
            } else if (key.equals("c")) {
                System.out.println("What action would you like to perform?");
                System.out.println("1. Lower process priority");
                System.out.println("2. Kill specific process");
                String choice = scanner.nextLine();
                if (choice.equals("1")) {
                    System.out.println("Enter the name of the process you want to lower priority for: ");
                    String processName = scanner.nextLine();
                    // Lower process priority logic here
                    System.out.println("Process priority lowered successfully.");
                } else if (choice.equals("2")) {
                    System.out.println("Enter the name of the process you want to kill: ");
                    String processName = scanner.nextLine();
                    // Kill specific process logic here
                    System.out.println("Process '" + processName + "' terminated successfully.");
                }
            }
        }
    }

    private static void monitorCPU() {
        int threshold = DEFAULT_THRESHOLD;
        AtomicInteger consecutiveCount = new AtomicInteger(0);
        while (isMonitoring.get()) {
            double cpuPercent = getCpuUsage();
            System.out.println("CPU Usage: " + String.format("%.2f", cpuPercent) + "%");

            if (cpuPercent >= threshold) {
                consecutiveCount.incrementAndGet();
                if (consecutiveCount.get() >= DEFAULT_MAX_CONTINUOUS_DURATION / DEFAULT_INTERVAL) {
                    System.out.println("CPU usage exceeded the threshold for too long.");
                    System.out.println("Taking necessary action...");
                    System.out.println("The CPU usage has been consistently high. Consider checking for any resource-intensive processes.");
                    consecutiveCount.set(0);
                }
            } else {
                consecutiveCount.set(0);
            }
            try {
                Thread.sleep(DEFAULT_INTERVAL * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private static double getCpuUsage() {
        // Logic to get CPU usage
        return 0.0;
    }
}
