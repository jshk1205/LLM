import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.*;

public class 리눅스기본명령어구현 {

    public static void ls(String directory, boolean showHidden, boolean longFormat, boolean humanReadable) {
        File dir = new File(directory);
        File[] files = dir.listFiles();
        if (files != null) {
            for (File file : files) {
                if (!showHidden && file.isHidden())
                    continue;
                if (longFormat) {
                    String fileInfo = getFileInfo(file, humanReadable);
                    System.out.println(fileInfo);
                } else {
                    System.out.println(file.getName());
                }
            }
        }
    }

    public static void cat(String filename, boolean number, int start, int end) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line;
            int lineNumber = 1;
            while ((line = reader.readLine()) != null) {
                if (number && lineNumber >= start && lineNumber <= end) {
                    System.out.println(lineNumber + ": " + line);
                } else if (!number && lineNumber >= start && lineNumber <= end) {
                    System.out.println(line);
                }
                lineNumber++;
            }
            reader.close();
        } catch (FileNotFoundException e) {
            System.out.println(filename + ": No such file or directory");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void echo(String text, String file, boolean append) {
        try {
            FileWriter writer = new FileWriter(file, append);
            writer.write(text);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static String getFileInfo(File file, boolean humanReadable) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String formattedDate = sdf.format(new Date(file.lastModified()));
        String fileName = file.getName();
        String fileSize = humanReadable ? humanReadableSize(file.length()) : String.valueOf(file.length());
        return String.format("%s %s %s %s", file.isDirectory() ? "d" : "-", fileSize, formattedDate, fileName);
    }

    private static String humanReadableSize(long size) {
        String[] units = {"B", "KB", "MB", "GB", "TB"};
        int index = 0;
        while (size >= 1024 && index < units.length - 1) {
            size /= 1024;
            index++;
        }
        return new DecimalFormat("#.##").format(size) + " " + units[index];
    }

    public static void main(String[] args) {
        // ls command test
        System.out.println("ls command result:");
        ls(".", true, true, true);

        // cat command test
        System.out.println("\ncat command result:");
        cat("example.txt", true, 2, 4);

        // echo command test
        System.out.println("\necho command result:");
        echo("Hello, world!\nThis is a test.\n", "output.txt", true);
        echo("Another line", "output.txt", true);
        cat("output.txt", false, 0, Integer.MAX_VALUE);
    }
}
