#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inventory.h"
#include "product.h"
#include "utils.h"

/* Function to display inventory management menu */
void displayInventoryMenu(void) {
    printf("\n========================================\n");
    printf("   INVENTORY & STOCK MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. Add Inventory Record              |\n");
    printf("| 2. View All Inventory Records        |\n");
    printf("| 3. Update Inventory Record           |\n");
    printf("| 4. Delete Inventory Record           |\n");
    printf("| 5. View Low Stock Items              |\n");
    printf("| 6. Generate Inventory Report         |\n");
    printf("| 0. Back to Main Menu                 |\n");
    printf("========================================");
    printf("\nEnter your choice: ");
}

void handleInventoryMenuChoice(int choice) {
    switch (choice) {
        case 1:
            printf("\n=== ADD INVENTORY RECORD ===\n");
            addInventoryRecord();
            pause();
            break;
            
        case 2:
            printf("\n=== ALL INVENTORY RECORDS ===\n");
            readInventoryRecords();
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE INVENTORY RECORD ===\n");
            updateInventoryRecord();
            pause();
            break;
            
        case 4:
            printf("\n=== DELETE INVENTORY RECORD ===\n");
            deleteInventoryRecord();
            pause();
            break;
            
        case 5:
            printf("\n=== LOW STOCK ITEMS ===\n");
            showLowStockItems();
            pause();
            break;
            
        case 6:
            printf("\n=== INVENTORY REPORT ===\n");
            generateInventoryReport();
            pause();
            break;
            
        case 0:
            /* Back to main menu */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 6.\n");
            pause();
            break;
    }
}

/* Main inventory management menu function */
void inventoryManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displayInventoryMenu();
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();
        
        handleInventoryMenuChoice(choice);
        
    } while (choice != 0);
}

/* Helper function to check if product ID exists */
int existsInFile(const char *filename, const char *productID) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, productID, strlen(productID)) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* Helper function to validate product ID format */
int isValidProductID(const char *productID) {
    if (strlen(productID) != 6) return 0;
    if (productID[0] != 'P') return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(productID[i])) return 0;
    }
    return 1;
}

// Add Inventory Record
void addInventoryRecord() {
    InventoryRecord r;

    printf("Enter Product ID (e.g., P00001): ");
    if (scanf("%9s", r.productID) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (!isValidProductID(r.productID)) {
        printf("Invalid Product ID format.\n");
        return;
    }
    if (!existsInFile("products.txt", r.productID)) {
        printf("Product ID not found.\n");
        return;
    }

    printf("Enter Stock Change (number only): ");
    if (scanf("%d", &r.stockChange) != 1) {
        printf("Invalid stock change input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter Date (YYYY-MM-DD): ");
    if (scanf("%19s", r.date) != 1) {
        printf("Invalid date input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (!isValidDate(r.date)) {
        printf("Invalid date format. Must be YYYY-MM-DD and year must be at least 2025.\n");
        return;
    }

    printf("Enter Action (IN/OUT): ");
    if (scanf("%3s", r.action) != 1) {
        printf("Invalid action input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (strcmp(r.action, "IN") != 0 && strcmp(r.action, "OUT") != 0) {
        printf("Action must be 'IN' or 'OUT' only.\n");
        return;
    }

    FILE *fp = fopen("inventory.txt", "a");
    if (!fp) {
        printf("Cannot open inventory file.\n");
        return;
    }
    fprintf(fp, "%s|%+d|%s|%s\n", r.productID, r.stockChange, r.date, r.action);
    fclose(fp);
    printf("Inventory record added successfully!\n");
}

// View Records
void readInventoryRecords() {
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("No inventory records found or cannot open inventory file.\n");
        return;
    }

    InventoryRecord r;
    char line[256];
    int count = 0;
    
    printf("%-10s %-8s %-12s %-8s\n", "Product", "Change", "Date", "Action");
    printf("--------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action) == 4) {
            printf("%-10s %-8d %-12s %-8s\n", r.productID, r.stockChange, r.date, r.action);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No inventory records found.\n");
    } else {
        printf("\nTotal records: %d\n", count);
    }
    
    fclose(fp);
}

// Update Record
void updateInventoryRecord() {
    char targetID[10], targetDate[20];
    
    printf("Enter Product ID to update: ");
    if (scanf("%9s", targetID) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    printf("Enter Date of record to update (YYYY-MM-DD): ");
    if (scanf("%19s", targetDate) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("Cannot open inventory file.\n");
        return;
    }
    
    FILE *temp = fopen("data/temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    InventoryRecord r;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action) == 4) {
            if (strcmp(r.productID, targetID) == 0 && strcmp(r.date, targetDate) == 0) {
                found = 1;
                printf("Current record: %s | %d | %s | %s\n", 
                       r.productID, r.stockChange, r.date, r.action);

                printf("New Stock Change: ");
                if (scanf("%d", &r.stockChange) != 1) {
                    printf("Invalid input.\n");
                    clearInputBuffer();
                    fclose(fp); fclose(temp); remove("data/temp.txt"); 
                    pause(); return;
                }
                clearInputBuffer();

                printf("New Action (IN/OUT): ");
                if (scanf("%3s", r.action) != 1) {
                    printf("Invalid input.\n");
                    clearInputBuffer();
                    fclose(fp); fclose(temp); remove("data/temp.txt"); 
                    pause(); return;
                }
                clearInputBuffer();
                
                if (strcmp(r.action, "IN") != 0 && strcmp(r.action, "OUT") != 0) {
                    printf("Invalid action.\n");
                    fclose(fp); fclose(temp); remove("data/temp.txt"); 
                    pause(); return;
                }
            }
            fprintf(temp, "%s|%+d|%s|%s\n", r.productID, r.stockChange, r.date, r.action);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("inventory.txt");
        rename("data/temp.txt", "inventory.txt");
        printf("Inventory record updated successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("Record not found.\n");
    }

}

// Delete Record
void deleteInventoryRecord() {
    char targetID[10], targetDate[20];
    
    printf("Enter Product ID to delete: ");
    if (scanf("%9s", targetID) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (!existsInFile("products.txt", targetID)) {
        printf("Product ID not found in products.\n");
        return;
    }

    printf("Enter Date of record to delete (YYYY-MM-DD): ");
    if (scanf("%19s", targetDate) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("Cannot open inventory file.\n");
        return;
    }
    
    FILE *temp = fopen("data/temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    InventoryRecord r;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action) == 4) {
            if (strcmp(r.productID, targetID) == 0 && strcmp(r.date, targetDate) == 0) {
                found = 1;
                printf("Deleting record: %s | %d | %s | %s\n", 
                       r.productID, r.stockChange, r.date, r.action);
                continue;
            }
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("inventory.txt");
        rename("data/temp.txt", "inventory.txt");
        printf("Inventory record deleted successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("Record not found.\n");
    }

}

// Show Low Stock Items
void showLowStockItems() {
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("No inventory records found.\n");
        return;
    }

    InventoryRecord r;
    char line[256];
    int found = 0;
    int lowStockThreshold = 10; // Default threshold
    
    printf("Low Stock Items (threshold: %d):\n", lowStockThreshold);
    printf("%-10s %-8s %-12s %-8s\n", "Product", "Change", "Date", "Action");
    printf("--------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action) == 4) {
            if (abs(r.stockChange) <= lowStockThreshold) {
                printf("%-10s %-8d %-12s %-8s\n", r.productID, r.stockChange, r.date, r.action);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No low stock items found.\n");
    }

    fclose(fp);
}

// Generate Inventory Report
void generateInventoryReport() {
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("No inventory records found.\n");
        return;
    }

    InventoryRecord r;
    char line[256];
    int totalRecords = 0, inCount = 0, outCount = 0;
    int totalIn = 0, totalOut = 0;

    printf("=== INVENTORY REPORT ===\n");
    printf("%-10s %-8s %-12s %-8s\n", "Product", "Change", "Date", "Action");
    printf("--------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action) == 4) {
            printf("%-10s %-8d %-12s %-8s\n", r.productID, r.stockChange, r.date, r.action);
            totalRecords++;
            
            if (strcmp(r.action, "IN") == 0) {
                inCount++;
                totalIn += r.stockChange;
            } else if (strcmp(r.action, "OUT") == 0) {
                outCount++;
                totalOut += abs(r.stockChange);
            }
        }
    }

    printf("\n=== SUMMARY ===\n");
    printf("Total Records: %d\n", totalRecords);
    printf("IN Records: %d (Total: +%d)\n", inCount, totalIn);
    printf("OUT Records: %d (Total: -%d)\n", outCount, totalOut);
    printf("Net Stock Change: %d\n", totalIn - totalOut);

    fclose(fp);
}