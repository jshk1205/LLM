import java.util.Scanner;

public class 달력프로그램 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("1. 년도 선택");
            System.out.println("2. 달 선택");
            System.out.println("3. 종료");
            System.out.print("선택하세요: ");
            String choice = scanner.nextLine();

            if (choice.equals("1")) {
                System.out.print("년도를 입력하세요: ");
                int year = scanner.nextInt();
                int month = 0;
                int day = 0;
                showMonthCalendar(year, month, day);
            } else if (choice.equals("2")) {
                System.out.print("년도를 입력하세요: ");
                int year = scanner.nextInt();
                System.out.print("달을 입력하세요: ");
                int month = scanner.nextInt();
                int day = 0;
                showDayCalendar(year, month, day);
            } else if (choice.equals("3")) {
                System.out.println("프로그램을 종료합니다.");
                break;
            } else {
                System.out.println("잘못된 선택입니다. 다시 선택해 주세요.");
            }
            scanner.nextLine(); // Consume newline left-over
        }
        scanner.close();
    }

    public static void showMonthCalendar(int year, int month, int day) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("달을 선택하세요:");
        for (int i = 1; i <= 12; i++) {
            System.out.println(i + ". " + getMonthName(i));
        }

        System.out.print("선택하세요: ");
        month = scanner.nextInt();
        if (month < 1 || month > 12) {
            System.out.println("잘못된 입력입니다.");
            return;
        }

        int daysInMonth = getDaysInMonth(year, month);
        System.out.println(getCalendar(year, month));
        chooseDay(year, month, daysInMonth);
        scanner.close();
    }

    public static void showDayCalendar(int year, int month, int day) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("날짜를 선택하세요:");
        int daysInMonth = getDaysInMonth(year, month);
        for (int i = 1; i <= daysInMonth; i++) {
            System.out.println(i);
        }

        System.out.print("선택하세요: ");
        day = scanner.nextInt();
        if (day < 1 || day > daysInMonth) {
            System.out.println("잘못된 입력입니다.");
            return;
        }

        System.out.println(year + "년 " + month + "월 " + day + "일");
        System.out.println("탄생화: " + getBirthFlower(month));
        scanner.close();
    }

    public static void chooseDay(int year, int month, int daysInMonth) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("날짜를 선택하세요: ");
        int day = scanner.nextInt();
        if (day < 1 || day > daysInMonth) {
            System.out.println("잘못된 입력입니다.");
            return;
        }

        System.out.println(year + "년 " + month + "월 " + day + "일");
        System.out.println("탄생화: " + getBirthFlower(month));
        scanner.close();
    }

    public static String getBirthFlower(int month) {
        String[] birthFlowers = {
                "양귀비", "수선화", "민들레", "벚꽃",
                "장미", "라벤더", "수련", "해바라기",
                "백합", "금잔화", "카네이션", "포인세티아"
        };
        if (month >= 1 && month <= 12) {
            return birthFlowers[month - 1];
        } else {
            return "알 수 없음";
        }
    }

    public static int getDaysInMonth(int year, int month) {
        java.util.Calendar calendar = java.util.Calendar.getInstance();
        calendar.set(java.util.Calendar.YEAR, year);
        calendar.set(java.util.Calendar.MONTH, month - 1);
        return calendar.getActualMaximum(java.util.Calendar.DAY_OF_MONTH);
    }

    public static String getCalendar(int year, int month) {
        java.util.Calendar cal = java.util.Calendar.getInstance();
        cal.set(java.util.Calendar.YEAR, year);
        cal.set(java.util.Calendar.MONTH, month - 1);
        cal.set(java.util.Calendar.DAY_OF_MONTH, 1);

        StringBuilder sb = new StringBuilder();
        sb.append(cal.getDisplayName(java.util.Calendar.MONTH, java.util.Calendar.LONG, java.util.Locale.getDefault()));
        sb.append(" ");
        sb.append(year);
        sb.append("\n");
        sb.append("일  월  화  수  목  금  토\n");

        int dayOfWeek = cal.get(java.util.Calendar.DAY_OF_WEEK);
        for (int i = 1; i < dayOfWeek; i++) {
            sb.append("    ");
        }

        int maxDay = cal.getActualMaximum(java.util.Calendar.DAY_OF_MONTH);
        for (int i = 1; i <= maxDay; i++) {
            sb.append(String.format("%2d", i));
            sb.append("  ");
            if (dayOfWeek % 7 == 0) {
                sb.append("\n");
            }
            dayOfWeek++;
        }
        sb.append("\n");
        return sb.toString();
    }

    public static String getMonthName(int month) {
        java.text.DateFormatSymbols dfs = new java.text.DateFormatSymbols();
        return dfs.getMonths()[month - 1];
    }
}
