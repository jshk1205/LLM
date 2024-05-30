# Necessary imports
import copy

# Class definition for product information
class Product:
    def __init__(self, name, quantity, location, urgent):
        self.name = name
        self.quantity = quantity
        self.location = location
        self.urgent = urgent

# Class definition for the shopping cart
class Cart:
    def __init__(self):
        self.products = []

    # Method to add a product to the cart
    def add_product(self, product):
        self.products.append(product)

    # Method to print the contents of the cart
    def print_cart(self):
        for product in self.products:
            print(f"{{Name: {product.name}, Quantity: {product.quantity}, Location: {product.location}, Urgent: {product.urgent}}}")

# Class definition for the warehouse
class Warehouse:
    def __init__(self):
        self.products = []

    # Method to move products to the warehouse
    def deliver_to_warehouse(self, cart):
        for product in cart.products:
            self.products.append(product)

        # Place urgent products first
        urgent_products = [product for product in self.products if product.urgent]
        non_urgent_products = [product for product in self.products if not product.urgent]
        self.products = urgent_products + non_urgent_products

        # Return products exceeding the maximum size of the warehouse back to the cart
        if len(self.products) > 3:
            cart.products = copy.deepcopy(self.products[3:])
            self.products = self.products[:3]
        else:
            cart.products = []

    # Method to deliver products to the customer
    def deliver_to_customer(self):
        delivered_products = self.products[:3]
        self.products = self.products[3:]
        return delivered_products

# Main function
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
