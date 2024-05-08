package com.example;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;


import org.apache.poi.xwpf.usermodel.XWPFDocument;
import org.apache.poi.xwpf.usermodel.XWPFParagraph;
import org.apache.poi.xwpf.usermodel.XWPFRun;

import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.PDPage;
import org.apache.pdfbox.pdmodel.PDPageContentStream;
import org.apache.pdfbox.pdmodel.font.PDFont;
import org.apache.pdfbox.pdmodel.font.PDType0Font;


public class PDF변환기 extends JFrame {
    private JTextField inputField;
    private JButton browseButton;
    private JButton convertButton;

    public PDF변환기() {
        setTitle("PDF 변환기");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setPreferredSize(new Dimension(400, 150));
        setLayout(new FlowLayout());
        setLocationRelativeTo(null); // 화면 중앙에 표시

        inputField = new JTextField(20);
        browseButton = new JButton("Browse");
        convertButton = new JButton("Convert");

        browseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                fileChooser.setFileFilter(new FileNameExtensionFilter("Word Documents (*.docx)", "docx"));
                fileChooser.setFileFilter(new FileNameExtensionFilter("HWP Documents (*.hwp)", "hwp"));
                int returnVal = fileChooser.showOpenDialog(PDF변환기.this);
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    File file = fileChooser.getSelectedFile();
                    inputField.setText(file.getAbsolutePath());
                }
            }
        });

        convertButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String inputFilePath = inputField.getText();
                if (inputFilePath.isEmpty()) {
                    JOptionPane.showMessageDialog(PDF변환기.this, "Please select a file.", "Error", JOptionPane.ERROR_MESSAGE);
                    return;
                }

                String outputFilePath = inputFilePath.substring(0, inputFilePath.lastIndexOf('.')) + ".pdf";

                try {
                    if (inputFilePath.endsWith(".docx")) {
                        convertWordToPDF(inputFilePath, outputFilePath);
                    } else if (inputFilePath.endsWith(".hwp")) {
                        convertHWPToPDF(inputFilePath, outputFilePath);
                    }
                    JOptionPane.showMessageDialog(PDF변환기.this, "Conversion successful.", "Success", JOptionPane.INFORMATION_MESSAGE);
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(PDF변환기.this, "Error converting file: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        add(inputField);
        add(browseButton);
        add(convertButton);

        pack();
        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new PDF변환기();
            }
        });
    }

    public static void convertWordToPDF(String inputFilePath, String outputFilePath) throws IOException {
        try (XWPFDocument document = new XWPFDocument(new FileInputStream(inputFilePath));
             PDDocument pdfDocument = new PDDocument()) {
            PDPage page = new PDPage();
            pdfDocument.addPage(page);
            try (PDPageContentStream contentStream = new PDPageContentStream(pdfDocument, page)) {
                contentStream.beginText();
                // Arial 폰트 설정
                PDFont font = PDType0Font.load(pdfDocument, new File("C:/Windows/Fonts/gulim.ttc"));
                contentStream.setFont(font, 12);
                for (XWPFParagraph paragraph : document.getParagraphs()) {
                    for (XWPFRun run : paragraph.getRuns()) {
                        contentStream.newLineAtOffset(100, 700);
                        contentStream.showText(run.getText(0));
                    }
                }
                contentStream.endText();
            }
            pdfDocument.save(outputFilePath);
        }
    }

    public static void convertHWPToPDF(String inputFilePath, String outputFilePath) {
        // HWP를 PDF로 변환하는 코드는 추가적인 라이브러리나 서비스를 사용해야 합니다.
        JOptionPane.showMessageDialog(null, "HWP to PDF conversion is not supported in this application.", "Error", JOptionPane.ERROR_MESSAGE);
    }
}
