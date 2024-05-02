import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

// 상품 정보를 담는 클래스
class Product {
    String name;
    int quantity;
    String location;
    boolean urgent;

    public Product(String name, int quantity, String location, boolean urgent) {
        this.name = name;
        this.quantity = quantity;
        this.location = location;
        this.urgent = urgent;
    }
}

// 장바구니 클래스
class Cart {
    List<Product> products;

    public Cart() {
        this.products = new ArrayList<>();
    }

    public void addProduct(Product product) {
        products.add(product);
    }

    public void printCart() {
        for (Product product : products) {
            System.out.println("{Name: " + product.name + ", Quantity: " + product.quantity + ", Location: " + product.location + ", Urgent: " + product.urgent + "}");
        }
    }
}

// 물류 창고 클래스
class Warehouse {
    List<Product> products;

    public Warehouse() {
        this.products = new ArrayList<>();
    }

    public void deliverToWarehouse(Cart cart) {
        products.addAll(cart.products);
        List<Product> urgentProducts = new ArrayList<>();
        List<Product> nonUrgentProducts = new ArrayList<>();

        for (Product product : products) {
            if (product.urgent)
                urgentProducts.add(product);
            else
                nonUrgentProducts.add(product);
        }

        products.clear();
        products.addAll(urgentProducts);
        products.addAll(nonUrgentProducts);

        if (products.size() > 3) {
            cart.products = new ArrayList<>(products.subList(3, products.size()));
            products = products.subList(0, 3);
        } else {
            cart.products = new ArrayList<>();
        }
    }

    public List<Product> deliverToCustomer() {
        List<Product> deliveredProducts = new ArrayList<>(products.subList(0, Math.min(3, products.size())));
        products.subList(0, Math.min(3, products.size())).clear();
        return deliveredProducts;
    }
}

// 메인 클래스
public class 주문및배송시스템시뮬레이션프로그램 {
    public static void main(String[] args) {
        Cart cart = new Cart();
        Warehouse warehouseSeoul = new Warehouse();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\n1: Add product to cart\n2: Deliver products to warehouses\n3: Deliver products to customers\n4: Print cart and warehouse information\n5: Exit");
            System.out.print("Enter your choice: ");
            int choice = scanner.nextInt();
            scanner.nextLine(); // consume newline

            if (choice == 1) {
                System.out.print("Enter product name: ");
                String name = scanner.nextLine();
                System.out.print("Enter quantity: ");
                int quantity = scanner.nextInt();
                scanner.nextLine(); // consume newline
                System.out.print("Enter location: ");
                String location = scanner.nextLine();
                System.out.print("Is it urgent delivery? (y/n): ");
                boolean urgent = scanner.nextLine().equalsIgnoreCase("y");
                Product product = new Product(name, quantity, location, urgent);
                cart.addProduct(product);
                System.out.println("Product added to cart.");
            } else if (choice == 2) {
                warehouseSeoul.deliverToWarehouse(cart);
                System.out.println("Products delivered to warehouses.");
            } else if (choice == 3) {
                List<Product> deliveredProducts = warehouseSeoul.deliverToCustomer();
                System.out.println("Products delivered to customers:");
                for (Product product : deliveredProducts) {
                    System.out.println("{Name: " + product.name + ", Quantity: " + product.quantity + ", Location: " + product.location + ", Urgent: " + product.urgent + "}");
                }
            } else if (choice == 4) {
                System.out.println("\nCart Information:");
                cart.printCart();
                System.out.println("\nWarehouse Information:");
                System.out.println("Seoul Warehouse:");
                for (Product product : warehouseSeoul.products) {
                    System.out.println("{Name: " + product.name + ", Quantity: " + product.quantity + ", Location: " + product.location + ", Urgent: " + product.urgent + "}");
                }
            } else if (choice == 5) {
                System.out.println("Exiting...");
                break;
            } else {
                System.out.println("Invalid choice. Please enter a valid option.");
            }
        }
    }
}
