import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.util.*;

public class 리눅스기본명령어구현 {
    
    public static void ls(String directory, boolean showHidden, boolean longFormat, boolean humanReadable) {
        File[] files = new File(directory).listFiles();
        if (files != null) {
            for (File file : files) {
                if (!showHidden && file.isHidden()) {
                    continue;
                }
                if (longFormat) {
                    String size = humanReadable ? humanReadableSize(file.length()) : Long.toString(file.length());
                    try {
                        PosixFileAttributes attrs = Files.getFileAttributeView(file.toPath(), PosixFileAttributeView.class).readAttributes();
                        System.out.printf("%s %d %d %s %s %s%n",
                                PosixFilePermissions.toString(attrs.permissions()),
                                attrs.owner().getName(), attrs.group().getName(),
                                size, attrs.lastModifiedTime(), file.getName());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else {
                    System.out.println(file.getName());
                }
            }
        }
    }

    public static void cat(String filename, boolean number, Integer start, Integer end) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            List<String> lines = new ArrayList<>();
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
            if (start == null) {
                start = 0;
            }
            if (end == null) {
                end = lines.size();
            }
            for (int i = start; i < Math.min(end, lines.size()); i++) {
                if (number) {
                    System.out.printf("%d: %s%n", i + 1, lines.get(i));
                } else {
                    System.out.println(lines.get(i));
                }
            }
        } catch (IOException e) {
            System.out.printf("%s: 해당 파일 또는 디렉토리가 없습니다%n", filename);
        }
    }

    public static void echo(String text, String file, boolean append) {
        if (file != null) {
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(file, append))) {
                writer.write(text);
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            System.out.println(text);
        }
    }

    private static String humanReadableSize(long size) {
        String[] units = {"B", "KB", "MB", "GB", "TB"};
        int index = 0;
        while (size >= 1024 && index < units.length - 1) {
            size /= 1024;
            index++;
        }
        return String.format("%.2f %s", size, units[index]);
    }

    public static void main(String[] args) {
        // ls 명령어 테스트
        System.out.println("ls 명령어 실행 결과:");
        ls(".", true, true, true);

        // cat 명령어 테스트
        System.out.println("\ncat 명령어 실행 결과:");
        cat("example.txt", true, 2, 4);

        // echo 명령어 테스트
        System.out.println("\necho 명령어 실행 결과:");
        echo("Hello, world!\nThis is a test.\n", "output.txt", true);
        echo("Another line", "output.txt", true);
        cat("output.txt", false, null, null);
    }
}
