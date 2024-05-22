package com.example;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionListener;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.awt.event.ActionEvent;

public class FileBinaryEditor extends JFrame {
    private JTextArea binaryTextArea;
    private JTextArea textTextArea;
    private JFileChooser fileChooser;

    public FileBinaryEditor() {
        setTitle("File Binary Editor");
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // Set the window size
        setSize(1200, 800);
        
        // Set the window to be displayed in the center of the screen
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

        // Set the sizes of the left and right panels to be equal
        splitPane.setResizeWeight(0.5);

        fileChooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter("All Files", "*.*");
        fileChooser.setFileFilter(filter);

        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("File");
        JMenuItem openMenuItem = new JMenuItem("Open");
        JMenuItem saveMenuItem = new JMenuItem("Save");
        fileMenu.add(openMenuItem);
        fileMenu.add(saveMenuItem);
        menuBar.add(fileMenu);
        openMenuItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                openFile();
            }
        });
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
                JOptionPane.showMessageDialog(this, "Could not open file: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    // Remove the unused method saveFile()
    // private void saveFile() {
    //     int returnVal = fileChooser.showSaveDialog(this);
    //     if (returnVal == JFileChooser.APPROVE_OPTION) {
    //         File file = fileChooser.getSelectedFile();
    //         try {
    //             byte[] data = textTextArea.getText().getBytes(StandardCharsets.UTF_8);
    //             Files.write(Paths.get(file.getPath()), data);
    //         } catch (IOException ex) {
    //             ex.printStackTrace();
    //             JOptionPane.showMessageDialog(this, "Could not save file: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
    //         }
    //     }
    // }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            FileBinaryEditor editor = new FileBinaryEditor();
            editor.setVisible(true);
        });
    }
}
