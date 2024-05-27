#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Define Stock structure
typedef struct {
    char name[50];
    double price;
    double initialPrice;
} Stock;

// Global variables
Stock stockList[] = {
    {"Samsung Electronics", 50000.0, 50000.0},
    {"LG Chem", 30000.0, 30000.0},
    {"Hyundai Motor", 40000.0, 40000.0}
};
int holdingStockQuantity[] = { 0, 0, 0 };
double balance = 1000000.0;
double investmentAmount = 0.0;

// Function prototypes
void displayStocks();
void buyStock(int index);
void sellStock(int index);

int main() {
    // Initialize random seed
    srand(time(NULL));

    // Start price fluctuation loop
    while (1) {
        for (int i = 0; i < sizeof(stockList) / sizeof(stockList[0]); i++) {
            double fluctuationRate = ((double)rand() / RAND_MAX) * 0.1 - 0.05;
            double newPrice = stockList[i].price * (1 + fluctuationRate);
            stockList[i].price = newPrice;
        }
        displayStocks();
        // Sleep for 5 seconds
        Sleep(5000);
    }

    return 0;
}

// Display stocks information
void displayStocks() {
    investmentAmount = 0.0;
    printf("\n-------------------------------------------------\n");
    printf("Stock Name\tPrice\t\tQuantity\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < sizeof(stockList) / sizeof(stockList[0]); i++) {
        printf("%s\t\t$%.2f\t\t%d\n", stockList[i].name, stockList[i].price, holdingStockQuantity[i]);
        investmentAmount += stockList[i].price * holdingStockQuantity[i];
    }
    printf("-------------------------------------------------\n");
    printf("Balance: $%.2f, Investment Amount: $%.2f\n", balance, investmentAmount);
    printf("\n");

    // Prompt user to buy or sell
    printf("What do you want to do?\n");
    printf("1. Buy\n");
    printf("2. Sell\n");
    printf("Enter your choice (1 or 2): ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Enter the index of the stock you want to buy: ");
        int index;
        scanf("%d", &index);
        buyStock(index);
    }
    else if (choice == 2) {
        printf("Enter the index of the stock you want to sell: ");
        int index;
        scanf("%d", &index);
        sellStock(index);
    }
    else {
        printf("Invalid choice.\n");
    }
}

// Buy stocks
void buyStock(int index) {
    if (index >= 0 && index < sizeof(stockList) / sizeof(stockList[0])) {
        double price = stockList[index].price;
        printf("Enter the quantity to buy: ");
        int quantity;
        scanf("%d", &quantity);
        double purchaseCost = price * quantity;
        if (balance >= purchaseCost) {
            holdingStockQuantity[index] += quantity;
            balance -= purchaseCost;
            printf("%d shares of %s purchased at $%.2f each.\n", quantity, stockList[index].name, price);
        }
        else {
            printf("Insufficient balance.\n");
        }
    }
    else {
        printf("Invalid stock index.\n");
    }
}

// Sell stocks
void sellStock(int index) {
    if (index >= 0 && index < sizeof(stockList) / sizeof(stockList[0])) {
        double price = stockList[index].price;
        printf("Enter the quantity to sell: ");
        int quantity;
        scanf("%d", &quantity);
        if (holdingStockQuantity[index] >= quantity) {
            double saleProceeds = price * quantity;
            holdingStockQuantity[index] -= quantity;
            balance += saleProceeds;
            printf("%d shares of %s sold at $%.2f each.\n", quantity, stockList[index].name, price);
        }
        else {
            printf("Insufficient stocks.\n");
        }
    }
    else {
        printf("Invalid stock index.\n");
    }
}
