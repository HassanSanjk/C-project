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
        
        // Better input handling - use scanf return value check
        if (scanf("%d", &choice) != 1) {
            choice = -1; // Set to invalid choice
        }
        clearInputBuffer(); // Clear any remaining input
        
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
    printf("\n========================================\n");
    printf("    INVENTORY MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("| 1. Product Management            |\n");
    printf("| 2. Inventory & Stock Management  |\n");
    printf("| 3. Category Management           |\n");
    printf("| 4. Supplier Management           |\n");
    printf("| 5. User & Transaction Management |\n");
    printf("| 6. Advanced Features & Analytics |\n");
    printf("| 7. Generate System Reports       |\n");
    printf("| 0. Exit System                   |\n");
    printf("========================================");
    printf("\nEnter your choice: ");
}

/* Function to display user & transaction management menu */
void displayUserTransactionMenu(void) {
    printf("\n========================================\n");
    printf("   USER & TRANSACTION MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. User Management               |\n");
    printf("| 2. Transaction Management        |\n");
    printf("| 0. Back to Main Menu             |\n");
    printf("========================================\n");
    printf("\nEnter your choice: ");
}


/* Function to handle main menu choices */
void handleMainMenuChoice(int choice) {
    int subChoice; // Declare variable for submenu
    
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
            /* User & Transaction Management - Fixed nested loop issue */
            do {
                clearScreen();
                displayUserTransactionMenu();
                
                if (scanf("%d", &subChoice) != 1) {
                    subChoice = -1;
                }
                clearInputBuffer();
                
                handleUserTransactionMenuChoice(subChoice);
                
            } while (subChoice != 0);
            break;
        case 6:
            /* Advanced Features & Analytics */
            advancedFeaturesMenu();
            break;
        case 7:
            /* Generate System Reports */
            printf("\n=== SYSTEM REPORTS ===\n");
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
    pause();
}


void generateReports(void) {
    printf("=== COMPREHENSIVE SYSTEM REPORTS ===\n\n");
    
    printf("1. PRODUCT INVENTORY REPORT\n");
    printf("----------------------------\n");
    viewProducts();
    
    printf("\n\n2. SUPPLIER INFORMATION REPORT\n");
    printf("-------------------------------\n");
    // Check if viewSuppliers function exists and file is accessible
    FILE *supplierCheck = fopen("suppliers.txt", "r");
    if (supplierCheck) {
        fclose(supplierCheck);
        viewSuppliers();
    } else {
        printf("No supplier data available or file not found.\n");
    }
    
    printf("\n\n3. TRANSACTION HISTORY REPORT\n");
    printf("------------------------------\n");
    // Check if viewTransactions function exists and file is accessible
    FILE *transactionCheck = fopen("transactions.txt", "r");
    if (transactionCheck) {
        fclose(transactionCheck);
        viewTransactions();
    } else {
        printf("No transaction data available or file not found.\n");
    }
    
    printf("\n=== END OF REPORTS ===\n");
}