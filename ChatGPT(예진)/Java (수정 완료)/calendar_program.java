import java.util.Scanner;

public class calendar_program {
    private static final int MAX_MONTH_NAME_LENGTH = 20;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String choice;
        int year, month = 0, day = 0;

        while (true) {
            System.out.println("1. Select Year");
            System.out.println("2. Select Month");
            System.out.println("3. Exit");
            System.out.print("Make a choice: ");
            choice = scanner.nextLine();

            if (choice.equals("1")) {
                System.out.print("Enter year: ");
                year = scanner.nextInt();
                scanner.nextLine(); // consume newline
                showMonthCalendar(year);
            } else if (choice.equals("2")) {
                System.out.print("Enter year: ");
                year = scanner.nextInt();
                System.out.print("Enter month: ");
                month = scanner.nextInt();
                scanner.nextLine(); // consume newline
                showDayCalendar(year, month);
            } else if (choice.equals("3")) {
                System.out.println("Exiting program.");
                break;
            } else {
                System.out.println("Invalid choice. Please select again.");
            }
        }

        scanner.close();
    }

    private static void showMonthCalendar(int year) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Select a month:");
        String[] monthNames = {
                "January", "February", "March", "April", "May", "June", "July",
                "August", "September", "October", "November", "December"
        };

        for (int i = 0; i < 12; i++) {
            System.out.printf("%d. %s\n", i + 1, monthNames[i]);
        }

        System.out.print("Make a choice: ");
        int month = scanner.nextInt();
        scanner.nextLine(); // consume newline

        if (month < 1 || month > 12) {
            System.out.println("Invalid input.");
            return;
        }

        int daysInMonth = getDaysInMonth(year, month);

        System.out.printf("Calendar for %d %s\n", year, monthNames[month - 1]);
        chooseDay(year, month, daysInMonth);
    }

    private static void showDayCalendar(int year, int month) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Select a day:");
        int daysInMonth = getDaysInMonth(year, month);

        for (int i = 1; i <= daysInMonth; i++) {
            System.out.println(i);
        }

        System.out.print("Make a choice: ");
        int day = scanner.nextInt();
        scanner.nextLine(); // consume newline

        if (day < 1 || day > daysInMonth) {
            System.out.println("Invalid input.");
            return;
        }

        System.out.printf("%d-%d-%d\n", year, month, day);
        System.out.printf("Birth flower: %s\n", getBirthFlower(month));
    }

    private static void chooseDay(int year, int month, int daysInMonth) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Select a day: ");
        int day = scanner.nextInt();
        scanner.nextLine(); // consume newline

        if (day < 1 || day > daysInMonth) {
            System.out.println("Invalid input.");
            return;
        }

        System.out.printf("%d-%d-%d\n", year, month, day);
        System.out.printf("Birth flower: %s\n", getBirthFlower(month));
    }

    private static int getDaysInMonth(int year, int month) {
        int daysInMonth;
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                daysInMonth = 29;
            } else {
                daysInMonth = 28;
            }
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            daysInMonth = 30;
        } else {
            daysInMonth = 31;
        }
        return daysInMonth;
    }

    private static String getBirthFlower(int month) {
        String[] birthFlowers = {
                "", "Carnation", "Violet", "Daffodil", "Daisy", "Lily of the Valley",
                "Rose", "Larkspur", "Gladiolus", "Aster", "Marigold", "Chrysanthemum", "Poinsettia"
        };
        return birthFlowers[month];
    }
}
