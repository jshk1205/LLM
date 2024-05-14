import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public class 카페메뉴관리프로그램 {
    static Map<String, Map<String, Object>> menu = new HashMap<>();
    static Map<String, Integer> inventory = new HashMap<>();
    static List<Map<String, Object>> orders = new ArrayList<>();
    static Map<String, Map<String, Integer>> sales = new HashMap<>();
    static String adminPassword = "";
    static boolean loggedInAsAdmin = false;

    public static void setAdminPassword() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("관리자 비밀번호를 설정하세요: ");
            String password = scanner.nextLine();
            System.out.print("비밀번호를 다시 한 번 입력하세요: ");
            String confirmPassword = scanner.nextLine();
            if (password.equals(confirmPassword)) {
                adminPassword = password;
                System.out.println("관리자 비밀번호가 설정되었습니다.");
                break;
            } else {
                System.out.println("비밀번호가 일치하지 않습니다. 다시 시도하세요.");
            }
        }
    }

    public static void addMenu() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("추가할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ");
            String item = scanner.nextLine();
            if (item.equals("메뉴 관리 완료!")) {
                break;
            }
            if (menu.containsKey(item)) {
                System.out.println("이미 존재하는 메뉴입니다.");
                continue;
            }
            System.out.print("가격을 입력하세요: ");
            int price = Integer.parseInt(scanner.nextLine());
            if (price > 10000) {
                System.out.println("가격이 너무 높습니다. 10000원을 초과할 수 없습니다.");
            } else if (price < 1000) {
                System.out.println("가격이 너무 낮습니다. 1000원 이상이어야 합니다.");
            } else {
                inventory.put(item, 10); // 초기 재고 설정
                Map<String, Object> details = new HashMap<>();
                details.put("가격", price);
                details.put("재고", inventory.get(item));
                menu.put(item, details);
            }
        }
    }

    public static void deleteMenu() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("삭제할 메뉴를 입력하세요 (종료하려면 '메뉴 관리 완료!' 입력): ");
            String item = scanner.nextLine();
            if (item.equals("메뉴 관리 완료!")) {
                break;
            }
            if (!menu.containsKey(item)) {
                System.out.println("존재하지 않는 메뉴입니다.");
            } else {
                menu.remove(item);
                inventory.remove(item);
            }
        }
    }

    public static void showMenu() {
        System.out.println("\n메뉴 목록:");
        for (Map.Entry<String, Map<String, Object>> entry : menu.entrySet()) {
            String item = entry.getKey();
            Map<String, Object> details = entry.getValue();
            System.out.println(item + ": " + details.get("가격") + "원 - 재고: " + details.get("재고") + "개");
        }
    }

    public static void manageInventory() {
        if (!loggedInAsAdmin) {
            Scanner scanner = new Scanner(System.in);
            System.out.print("관리자 비밀번호를 입력하세요: ");
            String password = scanner.nextLine();
            if (!password.equals(adminPassword)) {
                System.out.println("잘못된 비밀번호입니다.");
                return;
            } else {
                loggedInAsAdmin = true;
                System.out.println("관리자 권한으로 로그인되었습니다.");
            }
        }
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("\n1. 재고 추가\n2. 종료");
            System.out.print("원하는 작업을 선택하세요: ");
            String choice = scanner.nextLine();
            if (choice.equals("1")) {
                addInventory();
            } else if (choice.equals("2")) {
                System.out.println("재고 관리를 종료합니다.");
                loggedInAsAdmin = false;
                break;
            } else {
                System.out.println("잘못된 입력입니다. 다시 시도하세요.");
            }
        }
    }

    public static void addInventory() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("재고를 추가할 메뉴를 입력하세요 (종료하려면 '재고 관리 완료!' 입력): ");
            String item = scanner.nextLine();
            if (item.equals("재고 관리 완료!")) {
                break;
            }
            if (!inventory.containsKey(item)) {
                System.out.println("존재하지 않는 메뉴입니다.");
                continue;
            }
            System.out.print("추가할 재고 수량을 입력하세요: ");
            int quantity = Integer.parseInt(scanner.nextLine());
            if (quantity <= 0) {
                System.out.println("유효하지 않은 재고 수량입니다.");
            } else {
                inventory.put(item, inventory.get(item) + quantity);
                Map<String, Object> details = menu.get(item);
                details.put("재고", (int)details.get("재고") + quantity);
            }
        }
    }

    public static void takeOrder() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("주문할 메뉴를 입력하세요 (종료하려면 '주문 완료!' 입력): ");
            String item = scanner.nextLine();
            if (item.equals("주문 완료!")) {
                break;
            }
            if (!menu.containsKey(item)) {
                System.out.println("존재하지 않는 메뉴입니다.");
                continue;
            }
            System.out.print("주문할 수량을 입력하세요: ");
            int quantity = Integer.parseInt(scanner.nextLine());
            if (quantity <= 0) {
                System.out.println("유효하지 않은 수량입니다.");
            } else if (quantity > (int)menu.get(item).get("재고")) {
                System.out.println("재고가 부족합니다.");
            } else {
                Map<String, Object> order = new HashMap<>();
                order.put("메뉴", item);
                order.put("수량", quantity);
                order.put("주문 시간", LocalDateTime.now());
                orders.add(order);
                int currentInventory = (int)menu.get(item).get("재고");
                menu.get(item).put("재고", currentInventory - quantity);
            }
        }
    }

    public static void processOrder() {
        int totalPrice = 0;
        System.out.println("\n주문 내역:");
        for (Map<String, Object> order : orders) {
            String item = (String)order.get("메뉴");
            int quantity = (int)order.get("수량");
            int price = (int)menu.get(item).get("가격");
            totalPrice += price * quantity;
            System.out.println(item + ": " + price + "원 x " + quantity + "개");
        }
        System.out.println("총 주문 가격: " + totalPrice + "원");
        Scanner scanner = new Scanner(System.in);
        System.out.print("결제 방법을 선택하세요 (현금 또는 카드): ");
        String paymentMethod = scanner.nextLine();
        System.out.println(paymentMethod + " 결제가 완료되었습니다.");
        orders.clear();  // 주문 내역 초기화
    }

    public static void main(String[] args) {
        if (adminPassword.isEmpty()) {
            setAdminPassword();
        }
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("\n1. 메뉴 추가\n2. 메뉴 삭제\n3. 메뉴 조회\n4. 재고 관리\n5. 주문\n6. 결제\n7. 종료");
            System.out.print("원하는 작업을 선택하세요: ");
            String choice = scanner.nextLine();
            switch (choice) {
                case "1":
                    addMenu();
                    break;
                case "2":
                    deleteMenu();
                    break;
                case "3":
                    showMenu();
                    break;
                case "4":
                    manageInventory();
                    break;
                case "5":
                    takeOrder();
                    break;
                case "6":
                    processOrder();
                    break;
                case "7":
                    System.out.println("프로그램을 종료합니다.");
                    return;
                default:
                    System.out.println("잘못된 입력입니다. 다시 시도하세요.");
            }
        }
    }
}
