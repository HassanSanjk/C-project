#include <stdio.h>
#include <stdlib.h>
#include "product.h"
#include "category.h"
#include "supplier.h"
#include "advance.h"
#include "inventory.h"
#include "users.h"
#include "transactions.h"
#include "utils.h"

/* Function prototypes for menu system */
void displayMainMenu(void);
void displayUserTransactionMenu(void);
void handleMainMenuChoice(int choice);
void handleUserTransactionMenuChoice(int choice);
void displayWelcomeMessage(void);
void displayExitMessage(void);
void generateReports(void);

int main(void) {
    int choice;
    
    displayWelcomeMessage();
    
    do {
        clearScreen();
        displayMainMenu();
        printf("Select: ");
        scanf("%d", &choice);
        handleMainMenuChoice(choice);
        
    } while (choice != 0);
    
    displayExitMessage();
    return 0;
}

/* Function to display welcome message */
void displayWelcomeMessage(void) {
    clearScreen();
    printf("================================================================================\n");
    printf("                    INVENTORY MANAGEMENT SYSTEM v1.0                           \n");
    printf("================================================================================\n");
    printf("                         Welcome to our system!                                \n");
    printf("                  Designed for efficient inventory management                  \n");
    printf("================================================================================\n");
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
    getchar(); // Wait for Enter
}

/* Function to display main menu */
void displayMainMenu(void) {
    printf("\n--- MAIN MENU ---\n");
    printf("1. Product Management\n");
    printf("2. Inventory & Stock Management\n");
    printf("3. Category Management\n");
    printf("4. Supplier Management\n");
    printf("5. User & Transaction Management\n");
    printf("6. Advanced Features & Analytics\n");
    printf("7. Generate System Reports\n");
    printf("0. Exit System\n");
}

/* NEW: Function to display user & transaction management menu */
void displayUserTransactionMenu(void) {
    printf("\n--- User & Transaction Management ---\n");
    printf("1. User Management\n");
    printf("2. Transaction Management\n");
    printf("0. Back to Main Menu\n");
}



/* Function to handle main menu choices */
void handleMainMenuChoice(int choice) {
    switch (choice) {
        case 1:
            /* Product Management */
            productManagementMenu();
            break;
        case 2:
            /* Inventory & Stock Management */
            inventoryManagementMenu();
            break;
        case 3:
            /* Category Management */
            categoryManagementMenu();
            break;
        case 4:
            /* Supplier Management */
            supplierManagementMenu();
            break;
        case 5:
            /* User & Transaction Management */
            do {
                clearScreen();
                displayUserTransactionMenu();
                printf("Select: ");
                scanf("%d", &choice);
                handleUserTransactionMenuChoice(choice);
                
            } while (choice != 0);
            break;
        case 6:
            /* Advanced Features & Analytics */
            printf("\n=== ADVANCED FEATURES & ANALYTICS ===\n");
            advancedFeaturesMenu();
            break;
        case 7:
            /* Generate System Reports */
            printf("\n=== SYSTEM REPORTS ===\n");
            printf("Generating comprehensive system reports...\n");
            generateReports();
            pause();
            break;
        case 0:
            /* Exit - handled in main loop */
            break;
        default:
            printf("Invalid option. Please choose a number between 0 and 7.\n");
            pause();
            break;
    }
}

/* Function to handle user & transaction menu choices */
void handleUserTransactionMenuChoice(int choice) {
    switch (choice) {
        case 1:
            /* User Management */
            userManagementMenu();
            break;
        case 2:
            /* Transaction Management */
            transactionManagementMenu();
            break;
        case 0:
            /* Back to main menu - handled in loop */
            break;
        default:
            printf("Invalid option. Please choose a number between 0 and 2.\n");
            pause();
            break;
    }
}

/* Function to display exit message */
void displayExitMessage(void) {
    clearScreen();
    printf("================================================================================\n");
    printf("                    THANK YOU FOR USING OUR SYSTEM!                            \n");
    printf("================================================================================\n");
    printf("                      Inventory Management System v1.0                         \n");
    printf("                            Shutting down safely...                            \n");
    printf("================================================================================\n");
    printf("                              Have a great day!                                \n");
    printf("================================================================================\n");
}


void generateReports(void) {
    printf("Comprehensive reporting will be available once all modules are integrated.\n");
    printf("Current available data:\n");
    printf("\n--- PRODUCTS ---\n");
    viewProducts(); /* Show current product data as a basic report */
    printf("\n--- SUPPLIERS ---\n");
    viewSuppliers(); /* Show current supplier data as a basic report */
    printf("\n--- TRANSACTIONS ---\n");
    viewTransactions(); /* Show current transaction data as a basic report */
}