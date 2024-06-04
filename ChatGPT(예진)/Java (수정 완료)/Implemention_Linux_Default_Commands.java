import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;

public class Implemention_Linux_Default_Commands {

    public static void main(String[] args) {
        // Test ls command
        System.out.println("ls command output:");
        ls(".", true, true, true);

        // Test echo command
        System.out.println("\necho command output:");
        echo("Hello, world!\nThis is a test.\n", "output.txt", false);  // Overwrite
        echo("Another line\n", "output.txt", true);  // Append
        cat("output.txt", false, 0, 0);

        // Test cat command with specific lines
        System.out.println("\ncat command output:");
        cat("output.txt", true, 2, 4);
    }

    public static void ls(String directory, boolean showHidden, boolean longFormat, boolean humanReadable) {
        try {
            DirectoryStream<Path> stream = Files.newDirectoryStream(Paths.get(directory));
            for (Path entry : stream) {
                BasicFileAttributes attrs = Files.readAttributes(entry, BasicFileAttributes.class);
                if (!showHidden && entry.getFileName().toString().startsWith(".")) {
                    continue;
                }

                if (longFormat) {
                    String sizeStr = humanReadable ? humanReadableSize(attrs.size()) : String.valueOf(attrs.size());
                    String timeStr = formatTime(attrs.lastModifiedTime().toMillis());
                    System.out.printf("%s %s %s%n",
                            attrs.isDirectory() ? "d" : "-",
                            sizeStr,
                            timeStr,
                            entry.getFileName());
                } else {
                    System.out.println(entry.getFileName());
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void cat(String filename, boolean number, int start, int end) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            int lineNumber = 0;
            while ((line = reader.readLine()) != null) {
                lineNumber++;
                if ((start == 0 && end == 0) || (lineNumber >= start && lineNumber <= end)) {
                    if (number) {
                        System.out.printf("%d: %s%n", lineNumber, line);
                    } else {
                        System.out.println(line);
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void echo(String text, String file, boolean append) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(file, append))) {
            writer.write(text);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String humanReadableSize(long size) {
        String[] units = {"B", "KB", "MB", "GB", "TB"};
        int index = 0;
        double sizeTemp = size;
        while (sizeTemp >= 1024 && index < units.length - 1) {
            sizeTemp /= 1024;
            index++;
        }
        return String.format("%.2f %s", sizeTemp, units[index]);
    }

    public static String formatTime(long millis) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return sdf.format(new Date(millis));
    }
}
