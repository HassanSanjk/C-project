#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"


// 1. Low Stock Alert (less than 5)
// 1. Low Stock Alert (less than 5) - FIXED
void lowStockAlert() {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Error: Cannot open products.txt or file doesn't exist.\n");
        printf("Please add some products first.\n");
        pause();
        return;
    }
    
    char line[256], id[10], name[50], category[10], supplier[10];
    int qty;
    float price;
    int lowStockCount = 0;
    
    printf("\n========================================\n");
    printf("     LOW STOCK ALERT (Qty < 5)\n");
    printf("========================================\n");
    printf("%-8s %-20s %-8s\n", "ID", "Product Name", "Quantity");
    printf("----------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        // Better parsing - handle potential parsing errors
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   id, name, category, supplier, &qty, &price) == 6) {
            if (qty < 5) {
                printf("%-8s %-20.19s %-8d\n", id, name, qty);
                lowStockCount++;
            }
        }
    }
    
    if (lowStockCount == 0) {
        printf("No products with low stock found.\n");
    } else {
        printf("----------------------------------------\n");
        printf("Total low stock items: %d\n", lowStockCount);
    }
    
    fclose(fp);
    pause();
}

// 2. Search Product by Name
void searchProductByName() {
    char keyword[50];
    printf("Enter keyword to search product name: ");
    clearInputBuffer(); // Clear buffer before reading
    if (fgets(keyword, sizeof(keyword), stdin) != NULL) {
        // Remove newline if present
        size_t len = strlen(keyword);
        if (len > 0 && keyword[len-1] == '\n') {
            keyword[len-1] = '\0';
        }
    }
    
    if (strlen(keyword) == 0) {
        printf("No keyword entered.\n");
        pause();
        return;
    }

    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Error: Cannot open products.txt or file doesn't exist.\n");
        pause();
        return;
    }

    char line[256], id[10], name[50], category[10], supplier[10];
    int qty;
    float price;
    int found = 0;
    
    printf("\n========================================\n");
    printf("    SEARCH RESULTS FOR: '%s'\n", keyword);
    printf("========================================\n");
    printf("%-8s %-20s %-10s\n", "ID", "Product Name", "Price (RM)");
    printf("----------------------------------------\n");
    
    // Convert keyword to lowercase for case-insensitive search
    char lowerKeyword[50];
    strcpy(lowerKeyword, keyword);
    for (int i = 0; lowerKeyword[i]; i++) {
        lowerKeyword[i] = tolower(lowerKeyword[i]);
    }
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   id, name, category, supplier, &qty, &price) == 6) {
            
            // Convert product name to lowercase for comparison
            char lowerName[50];
            strcpy(lowerName, name);
            for (int i = 0; lowerName[i]; i++) {
                lowerName[i] = tolower(lowerName[i]);
            }
            
            if (strstr(lowerName, lowerKeyword)) {
                found = 1;
                printf("%-8s %-20.19s RM%-7.2f\n", id, name, price);
            }
        }
    }
    
    if (!found) {
        printf("No matching products found.\n");
    }
    
    fclose(fp);
    pause();
}

// 3. Transaction Summary (Total Count)
void transactionSummary() {
    FILE *fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("Error: Cannot open transactions.txt or file doesn't exist.\n");
        printf("No transactions recorded yet.\n");
        pause();
        return;
    }
    
    char line[256];
    int count = 0;
    float totalValue = 0.0;
    
    printf("\n========================================\n");
    printf("       TRANSACTION SUMMARY\n");
    printf("========================================\n");
    
    while (fgets(line, sizeof(line), fp)) {
        // Skip empty lines
        if (strlen(line) > 1) {
            count++;
            // If transaction format includes amount, try to extract it
            // Format might be: TransactionID|UserID|ProductID|Quantity|Amount
            char tid[10], uid[10], pid[10];
            int qty;
            float amount;
            if (sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%f", tid, uid, pid, &qty, &amount) == 5) {
                totalValue += amount;
            }
        }
    }
    
    printf("Total Transactions: %d\n", count);
    if (totalValue > 0) {
        printf("Total Transaction Value: RM%.2f\n", totalValue);
        printf("Average Transaction Value: RM%.2f\n", totalValue / count);
    }
    
    fclose(fp);
    pause();
}

// 4. User Contact Lookup
void findUserContact() {
    char uid[10];
    printf("Enter User ID (e.g., U00001): ");
    clearInputBuffer();
    if (fgets(uid, sizeof(uid), stdin) != NULL) {
        size_t len = strlen(uid);
        if (len > 0 && uid[len-1] == '\n') {
            uid[len-1] = '\0';
        }
    }

    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        printf("Error: Cannot open users.txt or file doesn't exist.\n");
        pause();
        return;
    }

    char line[256], id[10], fname[30], lname[30], contact[20];
    int found = 0;
    
    printf("\n========================================\n");
    printf("       USER CONTACT LOOKUP\n");
    printf("========================================\n");
    
    while (fgets(line, sizeof(line), fp)) {
        // Better parsing with proper field separation
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|\n]", id, fname, lname, contact) >= 4) {
            if (strcmp(id, uid) == 0) {
                printf("User ID: %s\n", id);
                printf("Name: %s %s\n", fname, lname);
                printf("Contact: %s\n", contact);
                found = 1;
                break;
            }
        }
    }
    
    if (!found) {
        printf("User ID '%s' not found.\n", uid);
    }

    fclose(fp);
    pause();
}

// 5. Supplier Email Finder
void supplierEmailFinder() {
    char sid[10];
    printf("Enter Supplier ID (e.g., S00001): ");
    clearInputBuffer();
    if (fgets(sid, sizeof(sid), stdin) != NULL) {
        size_t len = strlen(sid);
        if (len > 0 && sid[len-1] == '\n') {
            sid[len-1] = '\0';
        }
    }

    FILE *fp = fopen("suppliers.txt", "r");
    if (!fp) {
        printf("Error: Cannot open suppliers.txt or file doesn't exist.\n");
        pause();
        return;
    }

    char line[256], id[10], name[50], address[100], email[50];
    int found = 0;
    
    printf("\n========================================\n");
    printf("      SUPPLIER EMAIL LOOKUP\n");
    printf("========================================\n");
    
    while (fgets(line, sizeof(line), fp)) {
        // Better parsing - handle longer addresses
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|\n]", id, name, address, email) >= 4) {
            if (strcmp(id, sid) == 0) {
                printf("Supplier ID: %s\n", id);
                printf("Company: %s\n", name);
                printf("Address: %s\n", address);
                printf("Email: %s\n", email);
                found = 1;
                break;
            }
        }
    }
    
    if (!found) {
        printf("Supplier ID '%s' not found.\n", sid);
    }

    fclose(fp);
    pause();
}

void advancedFeaturesMenu() {
    int choice;
    
    do {
        clearScreen();
        printf("\n========================================\n");
        printf("       ADVANCED FEATURES MENU\n");
        printf("========================================\n");
        printf("| 1. Show Low Stock Alert          |\n");
        printf("| 2. Search Product by Name        |\n");
        printf("| 3. Transaction Summary            |\n");
        printf("| 4. Find User Contact              |\n");
        printf("| 5. Find Supplier Email           |\n");
        printf("| 0. Back to Main Menu              |\n");
        printf("========================================\n");
        printf("\nEnter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: 
                lowStockAlert(); 
                break;
            case 2: 
                searchProductByName(); 
                break;
            case 3: 
                transactionSummary(); 
                break;
            case 4: 
                findUserContact(); 
                break;
            case 5: 
                supplierEmailFinder(); 
                break;
            case 0: 
                break;
            default: 
                printf("Invalid choice. Please choose a number between 0 and 5.\n"); 
                pause(); 
                break;
        }
    } while (choice != 0);
}