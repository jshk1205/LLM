import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class Implement_symbolic_link {
    public static void main(String[] args) {
        createSymbolicLink();
    }

    public static void createSymbolicLink() {
        try {
            // Get input for the source file path from the user
            System.out.print("Enter the path of the file you want to copy: ");
            String sourceFile = System.console().readLine();

            // Get input for the target directory path from the user
            System.out.print("Enter the directory where you want to copy the file: ");
            String targetDirectory = System.console().readLine();

            // Copy the file
            String fileName = Paths.get(sourceFile).getFileName().toString();
            Path copiedFilePath = Paths.get(targetDirectory, fileName);
            Files.copy(Paths.get(sourceFile), copiedFilePath);
            System.out.println("File copied to " + copiedFilePath);

            // Create a symbolic link
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
