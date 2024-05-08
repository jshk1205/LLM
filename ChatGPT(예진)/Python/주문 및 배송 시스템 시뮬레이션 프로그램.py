# 필요한 헤더
import copy

# 상품 정보를 담는 구조체를 클래스로 정의합니다.
class Product:
    def __init__(self, name, quantity, location, urgent):
        self.name = name
        self.quantity = quantity
        self.location = location
        self.urgent = urgent

# 장바구니 클래스를 정의합니다.
class Cart:
    def __init__(self):
        self.products = []

    # 장바구니에 상품을 추가하는 메서드
    def add_product(self, product):
        self.products.append(product)

    # 장바구니 내용을 출력하는 메서드
    def print_cart(self):
        for product in self.products:
            print(f"{{Name: {product.name}, Quantity: {product.quantity}, Location: {product.location}, Urgent: {product.urgent}}}")

# 지역 물류 창고 클래스를 정의합니다.
class Warehouse:
    def __init__(self):
        self.products = []

    # 물류 창고로 상품을 이동시키는 메서드
    def deliver_to_warehouse(self, cart):
        for product in cart.products:
            self.products.append(product)

        # 긴급 상품을 먼저 배치합니다.
        urgent_products = [product for product in self.products if product.urgent]
        non_urgent_products = [product for product in self.products if not product.urgent]
        self.products = urgent_products + non_urgent_products

        # 물류 창고의 최대 크기를 초과하는 상품은 다시 장바구니로 돌려놓습니다.
        if len(self.products) > 3:
            cart.products = copy.deepcopy(self.products[3:])
            self.products = self.products[:3]
        else:
            cart.products = []

    # 고객에게 배송하는 메서드
    def deliver_to_customer(self):
        delivered_products = self.products[:3]
        self.products = self.products[3:]
        return delivered_products

# 메인 함수
def main():
    cart = Cart()
    warehouse_seoul = Warehouse()

    while True:
        print("\n1: Add product to cart\n2: Deliver products to warehouses\n3: Deliver products to customers\n4: Print cart and warehouse information\n5: Exit")
        choice = int(input("Enter your choice: "))

        if choice == 1:
            name = input("Enter product name: ")
            quantity = int(input("Enter quantity: "))
            location = input("Enter location: ")
            urgent = input("Is it urgent delivery? (y/n): ").lower() == 'y'
            product = Product(name, quantity, location, urgent)
            cart.add_product(product)
            print("Product added to cart.")

        elif choice == 2:
            warehouse_seoul.deliver_to_warehouse(cart)
            print("Products delivered to warehouses.")

        elif choice == 3:
            delivered_products = warehouse_seoul.deliver_to_customer()
            print("Products delivered to customers:")
            for product in delivered_products:
                print(f"{{Name: {product.name}, Quantity: {product.quantity}, Location: {product.location}, Urgent: {product.urgent}}}")

        elif choice == 4:
            print("\nCart Information:")
            cart.print_cart()
            print("\nWarehouse Information:")
            print("Seoul Warehouse:")
            for product in warehouse_seoul.products:
                print(f"{{Name: {product.name}, Quantity: {product.quantity}, Location: {product.location}, Urgent: {product.urgent}}}")

        elif choice == 5:
            print("Exiting...")
            break

        else:
            print("Invalid choice. Please enter a valid option.")

if __name__ == "__main__":
    main()
