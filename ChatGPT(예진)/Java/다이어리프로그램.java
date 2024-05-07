import java.util.Scanner;
import java.util.HashMap;
import java.util.Date;
import java.util.Calendar;
import java.util.Map;

class User {
    String name;
    String username;
    String password;
    Map<Date, String> diaryEntries;

    public User(String name, String username, String password) {
        this.name = name;
        this.username = username;
        this.password = password;
        this.diaryEntries = new HashMap<>();
    }
}

public class 다이어리프로그램 {
    private Map<String, User> users;
    private User currentUser;

    public 다이어리프로그램() {
        this.users = new HashMap<>();
        this.currentUser = null;
    }

    public void run() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("1. Login");
            System.out.println("2. Sign Up");
            System.out.print("Select an option: ");
            String choice = scanner.nextLine();

            if (choice.equals("1")) {
                login(scanner);
                if (currentUser != null) {
                    mainPage(scanner);
                }
            } else if (choice.equals("2")) {
                signUp(scanner);
            }
        }
    }

    private void login(Scanner scanner) {
        System.out.println("\n--- Login ---");
        System.out.print("Enter your username: ");
        String username = scanner.nextLine();
        System.out.print("Enter your password: ");
        String password = scanner.nextLine();

        if (users.containsKey(username) && users.get(username).password.equals(password)) {
            currentUser = users.get(username);
            System.out.println("Login successful!");
        } else {
            System.out.println("Invalid username or password.");
        }
    }

    private void signUp(Scanner scanner) {
        System.out.println("\n--- Sign Up ---");
        System.out.print("Enter your name: ");
        String name = scanner.nextLine();
        System.out.print("Enter a username: ");
        String username = scanner.nextLine();

        if (username.matches("\\d+")) {
            System.out.println("Username cannot be all digits.");
            return;
        }

        if (users.containsKey(username)) {
            System.out.println("Username already exists.");
            return;
        }

        System.out.print("Enter a password: ");
        String password = scanner.nextLine();
        if (name.isEmpty() || username.isEmpty() || password.isEmpty()) {
            System.out.println("All fields are required.");
            return;
        }

        users.put(username, new User(name, username, password));
        System.out.println("Sign up successful!");
    }

    private void mainPage(Scanner scanner) {
        while (true) {
            System.out.println("\nWelcome, " + currentUser.name + "'s Diary");
            Calendar calendar = Calendar.getInstance();
            displayCalendar(calendar);

            System.out.println("3. My Diary");
            System.out.println("4. My Diary List");
            System.out.println("5. Logout");
            System.out.print("Select an option: ");
            String choice = scanner.nextLine();

            if (choice.equals("3")) {
                diaryPage(scanner, calendar);
            } else if (choice.equals("4")) {
                diaryListPage(scanner);
            } else if (choice.equals("5")) {
                currentUser = null;
                return;
            }
        }
    }

    private void displayCalendar(Calendar calendar) {
        int month = calendar.get(Calendar.MONTH);
        int year = calendar.get(Calendar.YEAR);
        int numDays = calendar.getActualMaximum(Calendar.DAY_OF_MONTH);

        System.out.printf("%tB %d\n", calendar, year);
        System.out.println("  M  T  W  T  F  S  S");

        calendar.set(Calendar.DAY_OF_MONTH, 1);
        int offset = calendar.get(Calendar.DAY_OF_WEEK) - 1;

        for (int i = 0; i < offset; i++) {
            System.out.print("   ");
        }

        for (int day = 1; day <= numDays; day++) {
            if (day == Calendar.getInstance().get(Calendar.DAY_OF_MONTH) && month == Calendar.getInstance().get(Calendar.MONTH)) {
                System.out.printf("\033[91m%2d\033[00m ", day);
            } else {
                System.out.printf("%2d ", day);
            }

            if ((offset + day) % 7 == 0) {
                System.out.println();
            }
        }
    }

    private void diaryPage(Scanner scanner, Calendar calendar) {
        System.out.printf("\n--- Diary Entry: %tB %d, %d ---\n", calendar, calendar.get(Calendar.DAY_OF_MONTH), calendar.get(Calendar.YEAR));
        System.out.print("Write your diary entry (Press Enter to finish):\n");
        String entry = scanner.nextLine();
        if (!entry.trim().isEmpty()) {
            currentUser.diaryEntries.put(calendar.getTime(), entry);
            System.out.println("Entry saved successfully!");
        } else {
            System.out.println("No entry to save.");
        }
    }

    private void diaryListPage(Scanner scanner) {
        System.out.println("\n--- Diary List ---");
        for (Date date : currentUser.diaryEntries.keySet()) {
            Calendar calendar = Calendar.getInstance();
            calendar.setTime(date);
            System.out.printf("%tB %d, %d\n", calendar, calendar.get(Calendar.DAY_OF_MONTH), calendar.get(Calendar.YEAR));
        }

        System.out.print("Select a date to view entry (MM/DD/YYYY), or type 'back' to return: ");
        String selectedDate = scanner.nextLine();
        if (selectedDate.equalsIgnoreCase("back")) {
            return;
        }

        try {
            Calendar selectedCalendar = Calendar.getInstance();
            selectedCalendar.setTime(new Date(selectedDate));
            if (currentUser.diaryEntries.containsKey(selectedCalendar.getTime())) {
                System.out.printf("\n--- Diary Entry: %tB %d, %d ---\n", selectedCalendar, selectedCalendar.get(Calendar.DAY_OF_MONTH), selectedCalendar.get(Calendar.YEAR));
                System.out.println(currentUser.diaryEntries.get(selectedCalendar.getTime()));
            } else {
                System.out.println("No entry found for selected date.");
            }
        } catch (IllegalArgumentException e) {
            System.out.println("Invalid date format.");
        }
    }

    public static void main(String[] args) {
        다이어리프로그램 diaryProgram = new 다이어리프로그램();
        diaryProgram.run();
    }
}
