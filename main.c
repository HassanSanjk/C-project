#include <stdio.h>
#include <stdlib.h>
#include "product.h"
#include "category.h"
#include "supplier.h"
#include "advance.h"
#include "inventory.h"
#include "users.h"
#include "transactions.h"

/* Function prototypes for menu system */
void displayMainMenu(void);
void displayProductMenu(void);
void displaySupplierMenu(void);
void displayInventoryMenu(void);
void displayUserTransactionMenu(void);  // Updated function name
void displayUserMenu(void);              // Separate user menu
void displayTransactionMenu(void);       // New transaction menu
void handleMainMenuChoice(int choice);
void handleProductMenuChoice(int choice);
void handleSupplierMenuChoice(int choice);
void handleInventoryMenuChoice(int choice);
void handleUserTransactionMenuChoice(int choice);  // Updated function name
void handleUserMenuChoice(int choice);
void handleTransactionMenuChoice(int choice);      // New transaction handler
void displayWelcomeMessage(void);
void displayExitMessage(void);
void clearScreen(void);
void pause(void);

/* Placeholder function prototypes for other team members' modules */
void categoryManagementMenu(void);
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

/* Function to display product management menu */
void displayProductMenu(void) {
    printf("\n--- Product Menu ---\n");
    printf("1. Add Product\n");
    printf("2. View All Products\n");
    printf("3. View Product Details\n");
    printf("4. Update Product\n");
    printf("5. Delete Product\n");
    printf("0. Back to Main Menu\n");
}

/* Function to display inventory management menu */
void displayInventoryMenu(void) {
    printf("\n--- Inventory Menu ---\n");
    printf("1. Add Inventory Item\n");
    printf("2. View All Inventory\n");
    printf("3. Update Inventory Item\n");
    printf("4. Remove Inventory Item\n");
    printf("5. View Low Stock Items\n");
    printf("6. Generate Inventory Report\n");
    printf("0. Back to Main Menu\n");
}

void displaySupplierMenu(void) {
    printf("\n--- Supplier Menu ---\n");
    printf("1. Add Supplier\n");
    printf("2. View All Suppliers\n");
    printf("3. Update Supplier\n");
    printf("4. Delete Supplier\n");
    printf("5. Filter Products by Supplier\n");
    printf("0. Back to Main Menu\n");
}

/* NEW: Function to display user & transaction management menu */
void displayUserTransactionMenu(void) {
    printf("\n--- User & Transaction Management ---\n");
    printf("1. User Management\n");
    printf("2. Transaction Management\n");
    printf("0. Back to Main Menu\n");
}

/* Function to display user management submenu */
void displayUserMenu(void) {
    printf("\n--- User Management ---\n");
    printf("1. Add User\n");
    printf("2. View All Users\n");
    printf("3. Update User\n");
    printf("4. Delete User\n");
    printf("0. Back to User & Transaction Menu\n");
}

/* NEW: Function to display transaction management submenu */
void displayTransactionMenu(void) {
    printf("\n--- Transaction Management ---\n");
    printf("1. Add Transaction\n");
    printf("2. View All Transactions\n");
    printf("3. Update Transaction\n");
    printf("4. Delete Transaction\n");
    printf("0. Back to User & Transaction Menu\n");
}

/* Function to handle main menu choices */
void handleMainMenuChoice(int choice) {
    switch (choice) {
        case 1:
            /* Product Management - Your Role */
            do {
                clearScreen();
                displayProductMenu();
                printf("Select: ");
                scanf("%d", &choice);
                handleProductMenuChoice(choice);
                
            } while (choice != 0);
            break;
            
        case 2:
            /* Inventory & Stock Management - Now Implemented */
            do {
                clearScreen();
                displayInventoryMenu();
                printf("Select: ");
                scanf("%d", &choice);
                handleInventoryMenuChoice(choice);
                
            } while (choice != 0);
            break;
            
        case 3:
            /* Category Management - Role 3 */
            printf("\n=== CATEGORY MANAGEMENT ===\n");
            printf("This module will be implemented by Role 3 team member.\n");
            printf("Features will include:\n");
            printf("- Add new categories\n");
            printf("- Update category information\n");
            printf("- Delete categories\n");
            printf("- View category details\n");
            printf("- Category filtering and reporting\n");
            categoryManagementMenu(); /* Placeholder call */
            pause();
            break;
            
        case 4:
            /* Supplier Management - Now Implemented */
            do {
                clearScreen();
                displaySupplierMenu();
                printf("Select: ");
                scanf("%d", &choice);
                handleSupplierMenuChoice(choice);
                
            } while (choice != 0);
            break;
            
        case 5:
            /* User & Transaction Management - Updated */
            do {
                clearScreen();
                displayUserTransactionMenu();
                printf("Select: ");
                scanf("%d", &choice);
                handleUserTransactionMenuChoice(choice);
                
            } while (choice != 0);
            break;
            
        case 6:
            /* Advanced Features & Analytics - Now Implemented */
            printf("\n=== ADVANCED FEATURES & ANALYTICS ===\n");
            advancedFeaturesMenu();
            break;
            
        case 7:
            /* Generate System Reports */
            printf("\n=== SYSTEM REPORTS ===\n");
            printf("Generating comprehensive system reports...\n");
            printf("This will integrate data from all modules.\n");
            generateReports(); /* Placeholder call */
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

/* Function to handle product menu choices */
void handleProductMenuChoice(int choice) {
    switch (choice) {
        case 1:
            addProduct();
            pause();
            break;
            
        case 2:
            viewProducts();
            pause();
            break;
            
        case 3:
            viewProduct();
            pause();
            break;
            
        case 4:
            updateProduct();
            pause();
            break;
            
        case 5:
            deleteProduct();
            pause();
            break;
            
        case 0:
            /* Back to main menu - handled in loop */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 5.\n");
            pause();
            break;
    }
}

/* NEW: Function to handle user & transaction menu choices */
void handleUserTransactionMenuChoice(int choice) {
    int subChoice;
    
    switch (choice) {
        case 1:
            /* User Management Submenu */
            do {
                clearScreen();
                displayUserMenu();
                printf("Select: ");
                scanf("%d", &subChoice);
                handleUserMenuChoice(subChoice);
                
            } while (subChoice != 0);
            break;
            
        case 2:
            /* Transaction Management Submenu */
            do {
                clearScreen();
                displayTransactionMenu();
                printf("Select: ");
                scanf("%d", &subChoice);
                handleTransactionMenuChoice(subChoice);
                
            } while (subChoice != 0);
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

/* Function to handle user menu choices */
void handleUserMenuChoice(int choice) {
    switch (choice) {
        case 1:
            printf("\n=== ADD USER ===\n");
            addUser();
            printf("User added successfully!\n");
            pause();
            break;
            
        case 2:
            printf("\n=== ALL USERS ===\n");
            viewUsers();
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE USER ===\n");
            updateUser();
            printf("User updated successfully!\n");
            pause();
            break;
            
        case 4:
            printf("\n=== DELETE USER ===\n");
            deleteUser();
            printf("User deletion completed!\n");
            pause();
            break;
            
        case 0:
            /* Back to user & transaction menu - handled in loop */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 4.\n");
            pause();
            break;
    }
}

/* NEW: Function to handle transaction menu choices */
void handleTransactionMenuChoice(int choice) {
    switch (choice) {
        case 1:
            printf("\n=== ADD TRANSACTION ===\n");
            addTransaction();
            printf("Transaction processing completed!\n");
            pause();
            break;
            
        case 2:
            printf("\n=== ALL TRANSACTIONS ===\n");
            viewTransactions();
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE TRANSACTION ===\n");
            updateTransaction();
            printf("Transaction update completed!\n");
            pause();
            break;
            
        case 4:
            printf("\n=== DELETE TRANSACTION ===\n");
            deleteTransaction();
            printf("Transaction deletion completed!\n");
            pause();
            break;
            
        case 0:
            /* Back to user & transaction menu - handled in loop */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 4.\n");
            pause();
            break;
    }
}

void handleInventoryMenuChoice(int choice) {
    static InventoryNode *inventoryList = NULL;
    
    // Load inventory on first access
    if (inventoryList == NULL && choice > 0 && choice < 7) {
        inventoryList = loadInventoryFromFile();
    }
    
    switch (choice) {
        case 1:
            printf("\n=== ADD INVENTORY ITEM ===\n");
            addInventoryItem(&inventoryList);
            saveInventoryToFile(inventoryList);
            printf("Inventory item added successfully!\n");
            pause();
            break;
            
        case 2:
            printf("\n=== ALL INVENTORY ITEMS ===\n");
            viewInventory(inventoryList);
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE INVENTORY ITEM ===\n");
            updateInventoryItem(inventoryList);
            saveInventoryToFile(inventoryList);
            printf("Inventory item updated successfully!\n");
            pause();
            break;
            
        case 4:
            printf("\n=== REMOVE INVENTORY ITEM ===\n");
            removeInventoryItem(&inventoryList);
            saveInventoryToFile(inventoryList);
            printf("Inventory item removed successfully!\n");
            pause();
            break;
            
        case 5:
            printf("\n=== LOW STOCK ITEMS ===\n");
            showLowStockOnly(inventoryList);
            pause();
            break;
            
        case 6:
            printf("\n=== INVENTORY REPORT ===\n");
            generateInventoryReport(inventoryList);
            pause();
            break;
            
        case 0:
            /* Save and free memory before returning to main menu */
            if (inventoryList != NULL) {
                saveInventoryToFile(inventoryList);
                freeInventoryList(inventoryList);
                inventoryList = NULL;
            }
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 6.\n");
            pause();
            break;
    }
}

void handleSupplierMenuChoice(int choice) {
    int supplierID;
    
    switch (choice) {
        case 1:
            printf("\n=== ADD SUPPLIER ===\n");
            addSupplier();
            printf("Supplier added successfully!\n");
            pause();
            break;
            
        case 2:
            printf("\n=== ALL SUPPLIERS ===\n");
            viewSuppliers();
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE SUPPLIER ===\n");
            updateSupplier();
            printf("Supplier updated successfully!\n");
            pause();
            break;
            
        case 4:
            printf("\n=== DELETE SUPPLIER ===\n");
            deleteSupplier();
            printf("Supplier deletion completed!\n");
            pause();
            break;
            
        case 5:
            printf("\n=== FILTER PRODUCTS BY SUPPLIER ===\n");
            printf("Enter Supplier ID: ");
            scanf("%d", &supplierID);
            printf("\nProducts from Supplier ID %d:\n", supplierID);
            printf("----------------------------------------\n");
            filterProductsBySupplier(supplierID);
            pause();
            break;
            
        case 0:
            /* Back to main menu - handled in loop */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 5.\n");
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

/* Function to clear screen */
void clearScreen(void) {
    /* Cross-platform screen clearing */
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* Function to pause and wait for user input */
void pause(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
    getchar(); // Wait for Enter
}

/* Placeholder functions for other team members' modules */
/* These functions will be replaced when other modules are integrated */

void inventoryManagementMenu(void) {
    printf("Inventory Management functionality will be available once Role 2 integrates their module.\n");
}

void categoryManagementMenu(void) {
    printf("Category Management functionality will be available once Role 3 integrates their module.\n");
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