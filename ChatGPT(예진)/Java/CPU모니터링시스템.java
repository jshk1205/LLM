import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;

public class CPU모니터링시스템 {

    private static final int DEFAULT_THRESHOLD = 90;
    private static final int DEFAULT_DURATION = 10;
    private static final int DEFAULT_MAX_CONTINUOUS_DURATION = 30;
    private static final int DEFAULT_MAX_CONSECUTIVE_THRESHOLD = 95;
    private static final int DEFAULT_INTERVAL = 5;

    private static AtomicBoolean isMonitoring = new AtomicBoolean(true);

    public static void main(String[] args) {
        Thread keyboardThread = new Thread(CPU모니터링시스템::monitorKeyboardInput);
        keyboardThread.start();
        monitorCPU("notepad.exe");
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
                System.out.print("Enter your choice (1/2): ");
                String choice = scanner.nextLine();
                if (choice.equals("1")) {
                    System.out.print("Enter the name of the process you want to lower priority for: ");
                    String processName = scanner.nextLine();
                    lowerProcessPriority(processName);
                } else if (choice.equals("2")) {
                    System.out.print("Enter the name of the process you want to kill: ");
                    String processName = scanner.nextLine();
                    killProcess(processName);
                }
            }
        }
    }

    private static void lowerProcessPriority(String processName) {
        // Implement process priority lowering in Java
        System.out.println("Process priority lowering not implemented in Java.");
    }

    private static void killProcess(String processName) {
        // Implement process killing in Java
        System.out.println("Process killing not implemented in Java.");
    }

    private static void monitorCPU(String processName) {
        try (FileWriter fileWriter = new FileWriter("cpu_monitor.log");
             PrintWriter printWriter = new PrintWriter(fileWriter)) {

            while (isMonitoring.get()) {
                double cpuPercent = getCpuPercent();
                System.out.println("CPU Usage: " + String.format("%.2f", cpuPercent) + "%");
                printWriter.println("CPU Usage: " + String.format("%.2f", cpuPercent) + "%");

                // Implement CPU monitoring logic
                // You can refer to the logic in the original Python code and translate it to Java
                // Note: psutil functions used in Python have no direct equivalents in Java, you'll need to find alternatives

                try {
                    Thread.sleep(DEFAULT_INTERVAL * 1000); // Sleep for interval in milliseconds
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static double getCpuPercent() {
        // Implement getting CPU percentage in Java
        return 0.0; // Placeholder return value
    }
}
