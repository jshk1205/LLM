package com.example;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class StudentGradeManagementSystem extends JFrame {
    private static final String DB_URL = "jdbc:h2:~/test";
    private static final String DB_USER = "sa";
    private static final String DB_PASSWORD = "";

    private JTable table;

    public StudentGradeManagementSystem() {
        setTitle("Student Grade Management System");
        setSize(800, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new BorderLayout());

        table = new JTable();
        JScrollPane scrollPane = new JScrollPane(table);
        panel.add(scrollPane, BorderLayout.CENTER);

        JButton loadDataButton = new JButton("Load Data");
        loadDataButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadDataFromDatabase();
            }
        });
        panel.add(loadDataButton, BorderLayout.SOUTH);

        JPanel buttonPanel = new JPanel(new FlowLayout());

        JButton deleteButton = new JButton("Delete");
        deleteButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                deleteDataFromDatabase();
            }
        });
        buttonPanel.add(deleteButton);

        JButton updateButton = new JButton("Update");
        updateButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                updateDataInDatabase();
            }
        });
        buttonPanel.add(updateButton);

        JButton insertButton = new JButton("Insert");
        insertButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                insertDataToDatabase();
            }
        });
        buttonPanel.add(insertButton);

        panel.add(buttonPanel, BorderLayout.NORTH);

        add(panel);
        setLocationRelativeTo(null); // Center the frame on the screen
    }

    // Function to load data from the database
    private void loadDataFromDatabase() {
        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
             Statement stmt = conn.createStatement()) {

            DefaultTableModel model = new DefaultTableModel();
            model.addColumn("Student Number");
            model.addColumn("Name");
            model.addColumn("Subject");
            model.addColumn("Grade");

            ResultSet rs = stmt.executeQuery("SELECT s.student_number, s.name, g.subject, g.grade FROM Students s INNER JOIN Grades g ON s.id = g.student_id");
            while (rs.next()) {
                model.addRow(new Object[]{rs.getString("student_number"), rs.getString("name"), rs.getString("subject"), rs.getDouble("grade")});
            }

            table.setModel(model);

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // Function to delete data from the database
    private void deleteDataFromDatabase() {
        int row = table.getSelectedRow();
        if (row == -1) {
            JOptionPane.showMessageDialog(this, "Please select a row to delete.");
            return;
        }

        String studentNumber = (String) table.getValueAt(row, 0);
        String subject = (String) table.getValueAt(row, 2);

        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            // Delete grade data
            PreparedStatement deleteGradeStmt = conn.prepareStatement("DELETE FROM Grades WHERE student_id = (SELECT id FROM Students WHERE student_number = ?) AND subject = ?");
            deleteGradeStmt.setString(1, studentNumber);
            deleteGradeStmt.setString(2, subject);
            int deletedGrades = deleteGradeStmt.executeUpdate();
            deleteGradeStmt.close();

            if (deletedGrades > 0) {
                JOptionPane.showMessageDialog(this, "Grade data has been deleted!");
                loadDataFromDatabase(); // Load data again
            } else {
                JOptionPane.showMessageDialog(this, "No grade data found for the given student number and subject.");
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // Function to update data in the database
    private void updateDataInDatabase() {
        int row = table.getSelectedRow();
        if (row == -1) {
            JOptionPane.showMessageDialog(this, "Please select a row to update.");
            return;
        }

        String studentNumber = (String) table.getValueAt(row, 0);
        String subject = (String) table.getValueAt(row, 2);
        String gradeStr = (String) table.getValueAt(row, 3); // Read as a string

        // Convert the string to Double
        double grade = 0.0;
        try {
            grade = Double.parseDouble(gradeStr);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Grade must be in numeric format.");
            return;
        }

        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            // Update grade
            PreparedStatement updateStmt = conn.prepareStatement("UPDATE Grades SET grade = ? WHERE student_id = (SELECT id FROM Students WHERE student_number = ?) AND subject = ?");
            updateStmt.setDouble(1, grade);
            updateStmt.setString(2, studentNumber);
            updateStmt.setString(3, subject);
            int affectedRows = updateStmt.executeUpdate();
            updateStmt.close();

            if (affectedRows > 0) {
                JOptionPane.showMessageDialog(this, "Data has been successfully updated!");
                loadDataFromDatabase(); // Load data again
            } else {
                JOptionPane.showMessageDialog(this, "No data found for the given student number and subject.");
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // Function to insert data into the database
    private void insertDataToDatabase() {
        // Add new student and grade data
        String studentNumber = JOptionPane.showInputDialog(this, "Enter student number:");
        if (studentNumber == null || studentNumber.isEmpty()) {
            return;
        }
        String name = JOptionPane.showInputDialog(this, "Enter name:");
        if (name == null || name.isEmpty()) {
            return;
        }
        String subject = JOptionPane.showInputDialog(this, "Enter subject:");
        if (subject == null || subject.isEmpty()) {
            return;
        }
        String gradeStr = JOptionPane.showInputDialog(this, "Enter grade:");
        if (gradeStr == null || gradeStr.isEmpty()) {
            return;
        }
        double grade = Double.parseDouble(gradeStr);

        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            // Add new student
            PreparedStatement insertStudentStmt = conn.prepareStatement("INSERT INTO Students (student_number, name) VALUES (?, ?)");
            insertStudentStmt.setString(1, studentNumber);
            insertStudentStmt.setString(2, name);
            insertStudentStmt.executeUpdate();
            insertStudentStmt.close();

            // Add new grade
            PreparedStatement insertGradeStmt = conn.prepareStatement("INSERT INTO Grades (student_id, subject, grade) SELECT id, ?, ? FROM Students WHERE student_number = ?");
            insertGradeStmt.setString(1, subject);
            insertGradeStmt.setDouble(2, grade);
            insertGradeStmt.setString(3, studentNumber);
            insertGradeStmt.executeUpdate();
            insertGradeStmt.close();

            JOptionPane.showMessageDialog(this, "New data has been added!");
            loadDataFromDatabase(); // Load data again

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new StudentGradeManagementSystem().setVisible(true);
            }
        });
    }
}
