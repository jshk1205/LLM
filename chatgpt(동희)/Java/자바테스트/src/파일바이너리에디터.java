import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

public class 파일바이너리에디터 extends JFrame {
    private JTextArea binaryTextArea;
    private JTextArea textTextArea;
    private JFileChooser fileChooser;

    public 파일바이너리에디터() {
        setTitle("파일 바이너리 편집기");
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // 화면 크기 설정
        setSize(1200, 800);
        
        // 화면 중앙에 표시되도록 설정
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation(screenSize.width / 2 - getWidth() / 2, screenSize.height / 2 - getHeight() / 2);

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
        getContentPane().add(splitPane, BorderLayout.CENTER);

        binaryTextArea = new JTextArea();
        JScrollPane binaryScrollPane = new JScrollPane(binaryTextArea);
        splitPane.setLeftComponent(binaryScrollPane);

        textTextArea = new JTextArea();
        JScrollPane textScrollPane = new JScrollPane(textTextArea);
        splitPane.setRightComponent(textScrollPane);

        // 좌우 패널의 크기를 동일하게 설정
        splitPane.setResizeWeight(0.5);

        fileChooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter("모든 파일", "*.*");
        fileChooser.setFileFilter(filter);

        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("파일");
        JMenuItem openMenuItem = new JMenuItem("열기");
        JMenuItem saveMenuItem = new JMenuItem("저장");
        fileMenu.add(openMenuItem);
        fileMenu.add(saveMenuItem);
        menuBar.add(fileMenu);
        setJMenuBar(menuBar);

        openMenuItem.addActionListener(e -> openFile());
        saveMenuItem.addActionListener(e -> saveFile());
    }

    private void openFile() {
        int returnVal = fileChooser.showOpenDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                byte[] data = Files.readAllBytes(Paths.get(file.getPath()));
                StringBuilder binaryContent = new StringBuilder();
                StringBuilder textContent = new StringBuilder();
                for (byte b : data) {
                    binaryContent.append(String.format("%02X ", b));
                    if (b >= 32 && b <= 126) {
                        textContent.append((char) b);
                    } else {
                        textContent.append(".");
                    }
                }
                binaryTextArea.setText(binaryContent.toString());
                textTextArea.setText(new String(data, StandardCharsets.UTF_8));
            } catch (IOException ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(this, "파일을 열 수 없습니다: " + ex.getMessage(), "오류", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void saveFile() {
        int returnVal = fileChooser.showSaveDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                byte[] data = textTextArea.getText().getBytes(StandardCharsets.UTF_8);
                Files.write(Paths.get(file.getPath()), data);
            } catch (IOException ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(this, "파일을 저장할 수 없습니다: " + ex.getMessage(), "오류", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            파일바이너리에디터 editor = new 파일바이너리에디터();
            editor.setVisible(true);
        });
    }
}
