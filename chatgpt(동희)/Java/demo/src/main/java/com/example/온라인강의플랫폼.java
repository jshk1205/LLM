package com.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class 온라인강의플랫폼 extends JFrame {
    private JComboBox<String> categoryComboBox;
    private JTextField searchField;
    private JList<String> courseList;
    private JTextArea courseDetailsArea;

    private static final String JDBC_URL = "jdbc:h2:~/test";
    private static final String USERNAME = "sa";
    private static final String PASSWORD = "";

    public 온라인강의플랫폼() {
        setTitle("Online Course Platform");
        setSize(800, 600);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel topPanel = new JPanel();
        categoryComboBox = new JComboBox<>();
        categoryComboBox.addItem("All Categories");
        topPanel.add(categoryComboBox);
        searchField = new JTextField(20);
        topPanel.add(searchField);
        JButton searchButton = new JButton("Search");
        searchButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                searchCourses(searchField.getText());
            }
        });
        topPanel.add(searchButton);
        add(topPanel, BorderLayout.NORTH);

        JPanel centerPanel = new JPanel(new GridLayout(1, 2));
        courseList = new JList<>();
        JScrollPane courseListScrollPane = new JScrollPane(courseList);
        centerPanel.add(courseListScrollPane);
        courseDetailsArea = new JTextArea();
        JScrollPane courseDetailsScrollPane = new JScrollPane(courseDetailsArea);
        centerPanel.add(courseDetailsScrollPane);
        add(centerPanel, BorderLayout.CENTER);

        // Load categories into categoryComboBox
        loadCategories();

        // Load courses into courseList
        loadCourses();

        // Add ActionListener to courseList to display course details when clicked
        courseList.addListSelectionListener(e -> displayCourseDetails());

        // Add ActionListener to categoryComboBox to filter courses by category
        categoryComboBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedCategory = (String) categoryComboBox.getSelectedItem();
                if (selectedCategory.equals("All Categories")) {
                    loadCourses();
                } else {
                    loadCoursesByCategory(selectedCategory);
                }
            }
        });
    }

    private void loadCategories() {
        try (Connection conn = getConnection();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM Categories")) {
            while (rs.next()) {
                categoryComboBox.addItem(rs.getString("Category_Name"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void loadCourses() {
        List<Course> courses = new ArrayList<>();
        try (Connection conn = getConnection();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM Courses")) {
            while (rs.next()) {
                Course course = new Course(
                        rs.getInt("ID"),
                        rs.getString("Title"),
                        rs.getString("Instructor"),
                        rs.getString("Content"),
                        rs.getString("Video_URL")
                );
                courses.add(course);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        DefaultListModel<String> courseListModel = new DefaultListModel<>();
        for (Course course : courses) {
            courseListModel.addElement(course.getTitle());
        }
        courseList.setModel(courseListModel);
    }

    private void loadCoursesByCategory(String categoryName) {
        List<Course> courses = new ArrayList<>();
        try (Connection conn = getConnection();
             PreparedStatement pstmt = conn.prepareStatement(
                     "SELECT c.* FROM Courses c " +
                             "INNER JOIN Course_Category_Mapping m ON c.ID = m.Course_ID " +
                             "INNER JOIN Categories cat ON m.Category_ID = cat.Category_ID " +
                             "WHERE cat.Category_Name = ?")) {
            pstmt.setString(1, categoryName);
            ResultSet rs = pstmt.executeQuery();
            while (rs.next()) {
                Course course = new Course(
                        rs.getInt("ID"),
                        rs.getString("Title"),
                        rs.getString("Instructor"),
                        rs.getString("Content"),
                        rs.getString("Video_URL")
                );
                courses.add(course);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        DefaultListModel<String> courseListModel = new DefaultListModel<>();
        for (Course course : courses) {
            courseListModel.addElement(course.getTitle());
        }
        courseList.setModel(courseListModel);
    }

    private void searchCourses(String keyword) {
        // Implement search functionality here
    }

    private void displayCourseDetails() {
        Course selectedCourse = getCourses().get(courseList.getSelectedIndex());
        courseDetailsArea.setText(selectedCourse.toString());
    }

    private Connection getConnection() throws SQLException {
        return DriverManager.getConnection(JDBC_URL, USERNAME, PASSWORD);
    }

    private List<Course> getCourses() {
        List<Course> courses = new ArrayList<>();
        try (Connection conn = getConnection();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM Courses")) {
            while (rs.next()) {
                Course course = new Course(
                        rs.getInt("ID"),
                        rs.getString("Title"),
                        rs.getString("Instructor"),
                        rs.getString("Content"),
                        rs.getString("Video_URL")
                );
                courses.add(course);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return courses;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new 온라인강의플랫폼().setVisible(true));
    }
}

class Course {
    private int id;
    private String title;
    private String instructor;
    private String content;
    private String videoUrl;

    public Course(int id, String title, String instructor, String content, String videoUrl) {
        this.id = id;
        this.title = title;
        this.instructor = instructor;
        this.content = content;
        this.videoUrl = videoUrl;
    }

    public int getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public String getInstructor() {
        return instructor;
    }

    public String getContent() {
        return content;
    }

    public String getVideoUrl() {
        return videoUrl;
    }

    @Override
    public String toString() {
        return "Title: " + title + "\nInstructor: " + instructor + "\nContent: " + content + "\nVideo URL: " + videoUrl;
    }
}
