#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 상품 정보를 담는 구조체
typedef struct {
    char name[100];
    int quantity;
    char location[100];
    int urgent; // 1: true, 0: false
} Product;

// 장바구니 구조체
typedef struct {
    Product* products;
    int size;
} Cart;

// 물류 창고 구조체
typedef struct {
    Product* products;
    int size;
} Warehouse;

// 상품을 장바구니에 추가하는 함수
void addProductToCart(Cart* cart, char* name, int quantity, char* location, int urgent) {
    cart->products = (Product*)realloc(cart->products, (cart->size + 1) * sizeof(Product));
    Product product;
    strcpy(product.name, name);
    product.quantity = quantity;
    strcpy(product.location, location);
    product.urgent = urgent;
    cart->products[cart->size++] = product;
}

// 장바구니 내용을 출력하는 함수
void printCart(Cart* cart) {
    for (int i = 0; i < cart->size; i++) {
        printf("{Name: %s, Quantity: %d, Location: %s, Urgent: %s}\n", cart->products[i].name, cart->products[i].quantity, cart->products[i].location, cart->products[i].urgent ? "true" : "false");
    }
}

// 장바구니의 상품을 물류 창고로 이동시키는 함수
void deliverToWarehouse(Cart* cart, Warehouse* warehouse) {
    for (int i = 0; i < cart->size; i++) {
        warehouse->products = (Product*)realloc(warehouse->products, (warehouse->size + 1) * sizeof(Product));
        warehouse->products[warehouse->size++] = cart->products[i];
    }

    // 긴급 상품을 먼저 배치
    for (int i = 0; i < warehouse->size; i++) {
        if (warehouse->products[i].urgent) {
            for (int j = i; j > 0; j--) {
                Product temp = warehouse->products[j];
                warehouse->products[j] = warehouse->products[j - 1];
                warehouse->products[j - 1] = temp;
            }
        }
    }

    // 물류 창고의 최대 크기를 초과하는 상품은 다시 장바구니로 돌려놓음
    if (warehouse->size > 3) {
        cart->size = warehouse->size - 3;
        for (int i = 0; i < cart->size; i++) {
            cart->products[i] = warehouse->products[i + 3];
        }
        warehouse->size = 3;
    }
    else {
        cart->size = 0;
    }
}

// 물류 창고의 상품을 고객에게 배송하는 함수
void deliverToCustomer(Warehouse* warehouse) {
    printf("Products delivered to customers:\n");
    int delivered = warehouse->size < 3 ? warehouse->size : 3;
    for (int i = 0; i < delivered; i++) {
        printf("{Name: %s, Quantity: %d, Location: %s, Urgent: %s}\n", warehouse->products[i].name, warehouse->products[i].quantity, warehouse->products[i].location, warehouse->products[i].urgent ? "true" : "false");
    }
    for (int i = 0; i < warehouse->size - delivered; i++) {
        warehouse->products[i] = warehouse->products[i + delivered];
    }
    warehouse->size -= delivered;
}

// 메인 함수
int main() {
    Cart cart;
    cart.products = NULL;
    cart.size = 0;

    Warehouse warehouse;
    warehouse.products = NULL;
    warehouse.size = 0;

    int choice;
    char name[100], location[100];
    int quantity, urgent;

    while (1) {
        printf("\n1: Add product to cart\n2: Deliver products to warehouses\n3: Deliver products to customers\n4: Print cart and warehouse information\n5: Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter product name: ");
            scanf("%s", name);
            printf("Enter quantity: ");
            scanf("%d", &quantity);
            printf("Enter location: ");
            scanf("%s", location);
            printf("Is it urgent delivery? (1: yes, 0: no): ");
            scanf("%d", &urgent);
            addProductToCart(&cart, name, quantity, location, urgent);
            printf("Product added to cart.\n");
            break;
        case 2:
            deliverToWarehouse(&cart, &warehouse);
            printf("Products delivered to warehouses.\n");
            break;
        case 3:
            deliverToCustomer(&warehouse);
            break;
        case 4:
            printf("\nCart Information:\n");
            printCart(&cart);
            printf("\nWarehouse Information:\n");
            printf("Seoul Warehouse:\n");
            for (int i = 0; i < warehouse.size; i++) {
                printf("{Name: %s, Quantity: %d, Location: %s, Urgent: %s}\n", warehouse.products[i].name, warehouse.products[i].quantity, warehouse.products[i].location, warehouse.products[i].urgent ? "true" : "false");
            }
            break;
        case 5:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}
