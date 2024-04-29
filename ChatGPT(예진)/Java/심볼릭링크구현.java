import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class 심볼릭링크구현 {
    public static void main(String[] args) {
        createSymbolicLink();
    }

    public static void createSymbolicLink() {
        try {
            // 사용자로부터 파일 경로 입력 받기
            System.out.print("Enter the path of the file you want to copy: ");
            String sourceFile = System.console().readLine();

            // 사용자로부터 대상 디렉토리 경로 입력 받기
            System.out.print("Enter the directory where you want to copy the file: ");
            String targetDirectory = System.console().readLine();

            // 파일 복사
            String fileName = Paths.get(sourceFile).getFileName().toString();
            Path copiedFilePath = Paths.get(targetDirectory, fileName);
            Files.copy(Paths.get(sourceFile), copiedFilePath);
            System.out.println("File copied to " + copiedFilePath);

            // 심볼릭 링크 생성
            System.out.print("Enter the name for the symbolic link: ");
            String linkName = System.console().readLine();
            Path linkPath = Paths.get(linkName);
            Files.createSymbolicLink(linkPath, copiedFilePath);
            System.out.println("Symbolic link created from " + linkPath + " to " + copiedFilePath);
        } catch (IOException e) {
            System.err.println("Failed to create symbolic link: " + e.getMessage());
        }
    }
}
