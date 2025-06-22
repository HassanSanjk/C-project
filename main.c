#include "product.h"

/* Function prototypes for menu system */
void displayMainMenu(void);
void displayProductMenu(void);
void handleMainMenuChoice(int choice);
void handleProductMenuChoice(int choice);
void displayWelcomeMessage(void);
void displayExitMessage(void);
void clearScreen(void);

/* Placeholder function prototypes for other team members' modules */
void inventoryManagementMenu(void);
void categorySupplierMenu(void);
void userTransactionMenu(void);
void fileOperationsMenu(void);
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
    getchar();
}

/* Function to display main menu */
void displayMainMenu(void) {
    printf("\n--- MAIN MENU ---\n");
    printf("1. Product Management\n");
    printf("2. Inventory & Stock Management\n");
    printf("3. Category & Supplier Management\n");
    printf("4. User & Transaction Management\n");
    printf("5. File Operations & Reports\n");
    printf("6. Generate System Reports\n");
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
            /* Inventory & Stock Management - Role 2 */
            printf("\n=== INVENTORY & STOCK MANAGEMENT ===\n");
            printf("This module will be implemented by Role 2 team member.\n");
            printf("Features will include:\n");
            printf("- Add stock inventory records\n");
            printf("- Update stock levels (increase/decrease)\n");
            printf("- Remove discontinued stock items\n");
            printf("- View current inventory levels\n");
            printf("- Low stock alerts and threshold management\n");
            inventoryManagementMenu(); /* Placeholder call */
            pause();
            break;
            
        case 3:
            /* Category & Supplier Management - Role 3 */
            printf("\n=== CATEGORY & SUPPLIER MANAGEMENT ===\n");
            printf("This module will be implemented by Role 3 team member.\n");
            printf("Features will include:\n");
            printf("- Add new categories and suppliers\n");
            printf("- Update category and supplier information\n");
            printf("- Delete categories and suppliers\n");
            printf("- View category and supplier details\n");
            printf("- Category/supplier filtering and reporting\n");
            categorySupplierMenu(); /* Placeholder call */
            pause();
            break;
            
        case 4:
            /* User & Transaction Management - Role 4 */
            printf("\n=== USER & TRANSACTION MANAGEMENT ===\n");
            printf("This module will be implemented by Role 4 team member.\n");
            printf("Features will include:\n");
            printf("- Add new users and transaction records\n");
            printf("- Update user information and transaction status\n");
            printf("- Delete/archive transactions and user accounts\n");
            printf("- View user details and transaction history\n");
            printf("- User authentication and access control\n");
            userTransactionMenu(); /* Placeholder call */
            pause();
            break;
            
        case 5:
            /* File Operations & Advanced Features - Role 5 */
            printf("\n=== FILE OPERATIONS & ADVANCED FEATURES ===\n");
            printf("This module will be implemented by Role 5 team member.\n");
            printf("Features will include:\n");
            printf("- Advanced file operations for all modules\n");
            printf("- Data backup and restore functionality\n");
            printf("- File integrity checks\n");
            printf("- Data import/export capabilities\n");
            fileOperationsMenu(); /* Placeholder call */
            pause();
            break;
            
        case 6:
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
            printf("Invalid option. Please choose a number between 0 and 6.\n");
            pause();
            break;
    }
}

/* Function to handle product menu choices */
void handleProductMenuChoice(int choice) {
    switch (choice) {
        case 1:
            addProduct();
            break;
            
        case 2:
            viewProducts();
            break;
            
        case 3:
            viewProduct();
            break;
            
        case 4:
            updateProduct();
            break;
            
        case 5:
            deleteProduct();
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

/* Placeholder functions for other team members' modules */
/* These functions will be replaced when other modules are integrated */

void inventoryManagementMenu(void) {
    printf("Inventory Management functionality will be available once Role 2 integrates their module.\n");
}

void categorySupplierMenu(void) {
    printf("Category & Supplier Management functionality will be available once Role 3 integrates their module.\n");
}

void userTransactionMenu(void) {
    printf("User & Transaction Management functionality will be available once Role 4 integrates their module.\n");
}

void fileOperationsMenu(void) {
    printf("Advanced File Operations will be available once Role 5 integrates their module.\n");
}

void generateReports(void) {
    printf("Comprehensive reporting will be available once all modules are integrated.\n");
    printf("Current available data:\n");
    viewProducts(); /* Show current product data as a basic report */
}