package com.example;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class 학생성적관리시스템 extends JFrame {
    private static final String DB_URL = "jdbc:h2:~/test";
    private static final String DB_USER = "sa";
    private static final String DB_PASSWORD = "";

    private JTable table;

    public 학생성적관리시스템() {
        setTitle("학생 성적 관리 시스템");
        setSize(800, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new BorderLayout());

        table = new JTable();
        JScrollPane scrollPane = new JScrollPane(table);
        panel.add(scrollPane, BorderLayout.CENTER);

        JButton loadDataButton = new JButton("데이터 불러오기");
        loadDataButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadDataFromDatabase();
            }
        });
        panel.add(loadDataButton, BorderLayout.SOUTH);

        JPanel buttonPanel = new JPanel(new FlowLayout());

        JButton deleteButton = new JButton("삭제");
        deleteButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                deleteDataFromDatabase();
            }
        });
        buttonPanel.add(deleteButton);

        JButton updateButton = new JButton("갱신");
        updateButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                updateDataInDatabase();
            }
        });
        buttonPanel.add(updateButton);

        JButton insertButton = new JButton("추가");
        insertButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                insertDataToDatabase();
            }
        });
        buttonPanel.add(insertButton);

        panel.add(buttonPanel, BorderLayout.NORTH);

        add(panel);
        setLocationRelativeTo(null); // 화면 가운데에 위치하도록 설정
    }

    private void loadDataFromDatabase() {
        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
             Statement stmt = conn.createStatement()) {

            DefaultTableModel model = new DefaultTableModel();
            model.addColumn("학번");
            model.addColumn("이름");
            model.addColumn("과목");
            model.addColumn("성적");

            ResultSet rs = stmt.executeQuery("SELECT s.student_number, s.name, g.subject, g.grade FROM Students s INNER JOIN Grades g ON s.id = g.student_id");
            while (rs.next()) {
                model.addRow(new Object[]{rs.getString("student_number"), rs.getString("name"), rs.getString("subject"), rs.getDouble("grade")});
            }

            table.setModel(model);

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void deleteDataFromDatabase() {
        int row = table.getSelectedRow();
        if (row == -1) {
            JOptionPane.showMessageDialog(this, "삭제할 행을 선택하세요.");
            return;
        }

        String studentNumber = (String) table.getValueAt(row, 0);
        String subject = (String) table.getValueAt(row, 2);

        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            // 성적 데이터 삭제
            PreparedStatement deleteGradeStmt = conn.prepareStatement("DELETE FROM Grades WHERE student_id = (SELECT id FROM Students WHERE student_number = ?) AND subject = ?");
            deleteGradeStmt.setString(1, studentNumber);
            deleteGradeStmt.setString(2, subject);
            int deletedGrades = deleteGradeStmt.executeUpdate();
            deleteGradeStmt.close();

            if (deletedGrades > 0) {
                JOptionPane.showMessageDialog(this, "성적 데이터가 삭제되었습니다!");
                loadDataFromDatabase(); // 데이터 다시 불러오기
            } else {
                JOptionPane.showMessageDialog(this, "주어진 학번과 과목에 대한 성적 데이터를 찾을 수 없습니다.");
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void updateDataInDatabase() {
        int row = table.getSelectedRow();
        if (row == -1) {
            JOptionPane.showMessageDialog(this, "갱신할 행을 선택하세요.");
            return;
        }

        String studentNumber = (String) table.getValueAt(row, 0);
        String subject = (String) table.getValueAt(row, 2);
        String gradeStr = (String) table.getValueAt(row, 3); // 문자열로 변환하여 읽음

        // 문자열로 읽은 값을 Double로 변환
        double grade = 0.0;
        try {
            grade = Double.parseDouble(gradeStr);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "성적은 숫자 형식이어야 합니다.");
            return;
        }

        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            // 성적 업데이트
            PreparedStatement updateStmt = conn.prepareStatement("UPDATE Grades SET grade = ? WHERE student_id = (SELECT id FROM Students WHERE student_number = ?) AND subject = ?");
            updateStmt.setDouble(1, grade);
            updateStmt.setString(2, studentNumber);
            updateStmt.setString(3, subject);
            int affectedRows = updateStmt.executeUpdate();
            updateStmt.close();

            if (affectedRows > 0) {
                JOptionPane.showMessageDialog(this, "데이터가 성공적으로 갱신되었습니다!");
                loadDataFromDatabase(); // 데이터 다시 불러오기
            } else {
                JOptionPane.showMessageDialog(this, "주어진 학번과 과목에 대한 데이터를 찾을 수 없습니다.");
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void insertDataToDatabase() {
        // 새로운 학생 및 성적 데이터 추가
        String studentNumber = JOptionPane.showInputDialog(this, "학번을 입력하세요:");
        if (studentNumber == null || studentNumber.isEmpty()) {
            return;
        }
        String name = JOptionPane.showInputDialog(this, "이름을 입력하세요:");
        if (name == null || name.isEmpty()) {
            return;
        }
        String subject = JOptionPane.showInputDialog(this, "과목을 입력하세요:");
        if (subject == null || subject.isEmpty()) {
            return;
        }
        String gradeStr = JOptionPane.showInputDialog(this, "성적을 입력하세요:");
        if (gradeStr == null || gradeStr.isEmpty()) {
            return;
        }
        double grade = Double.parseDouble(gradeStr);
    
        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            // 새로운 학생 추가
            PreparedStatement insertStudentStmt = conn.prepareStatement("INSERT INTO Students (student_number, name) VALUES (?, ?)");
            insertStudentStmt.setString(1, studentNumber);
            insertStudentStmt.setString(2, name);
            insertStudentStmt.executeUpdate();
            insertStudentStmt.close();
    
            // 새로운 성적 추가
            PreparedStatement insertGradeStmt = conn.prepareStatement("INSERT INTO Grades (student_id, subject, grade) SELECT id, ?, ? FROM Students WHERE student_number = ?");
            insertGradeStmt.setString(1, subject);
            insertGradeStmt.setDouble(2, grade);
            insertGradeStmt.setString(3, studentNumber);
            insertGradeStmt.executeUpdate();
            insertGradeStmt.close();
    
            JOptionPane.showMessageDialog(this, "새로운 데이터가 추가되었습니다!");
            loadDataFromDatabase(); // 데이터 다시 불러오기
    
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new 학생성적관리시스템().setVisible(true);
            }
        });
    }
}
