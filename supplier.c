#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "supplier.h"
#include "utils.h"

/* Function to display supplier management menu */
void displaySupplierMenu(void) {
    printf("\n========================================\n");
    printf("      SUPPLIER MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. Add Supplier                      |\n");
    printf("| 2. View All Suppliers                |\n");
    printf("| 3. Update Supplier                   |\n");
    printf("| 4. Delete Supplier                   |\n");
    printf("| 5. Filter Products by Supplier       |\n");
    printf("| 0. Back to Main Menu                 |\n");
    printf("========================================");
    printf("\nEnter your choice: ");
}

/* Function to handle supplier menu choices */
void handleSupplierMenuChoice(int choice) {
    char supplierID[10];
    
    switch (choice) {
        case 1:
            printf("\n=== ADD SUPPLIER ===\n");
            addSupplier();
            break;
            
        case 2:
            printf("\n=== ALL SUPPLIERS ===\n");
            viewSuppliers();
            break;
            
        case 3:
            printf("\n=== UPDATE SUPPLIER ===\n");
            updateSupplier();
            break;
            
        case 4:
            printf("\n=== DELETE SUPPLIER ===\n");
            deleteSupplier();
            break;
            
        case 5:
            printf("\n=== FILTER PRODUCTS BY SUPPLIER ===\n");
            printf("Enter Supplier ID (e.g., S00001): ");
            scanf("%9s", supplierID);
            clearInputBuffer();
            printf("\nProducts from Supplier ID %s:\n", supplierID);
            printf("-----------------------------------------------------\n");
            filterProductsBySupplier(supplierID);
            pause();
            break;
            
        case 0:
            /* Back to main menu */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 5.\n");
            pause();
            break;
    }
}

/* Main supplier management menu function */
void supplierManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displaySupplierMenu();
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();
        
        handleSupplierMenuChoice(choice);
        
    } while (choice != 0);
}

int isValidSupplier(const char *id) {
    if (strlen(id) != 6 || id[0] != 'S') return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

int isValidPhone(const char *phone) {
    int len = strlen(phone);
    if (len < 5 || len > 15) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int isValidEmail(const char *email) {
    return strchr(email, '@') && strchr(email, '.');
}

void addSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "a");
    if (!fp) {
        printf("Cannot open supplier file.\n");
        pause();
        return;
    }
    
    Supplier s;
    printf("Enter Supplier ID (e.g., S00001): ");
    scanf("%9s", s.id);
    if (!isValidSupplier(s.id)) {
        printf("Invalid Supplier ID format. Must start with 'S' and 5 digits.\n");
        fclose(fp);
        pause();
        return;
    }
    
    // Check if ID already exists
    FILE *check = fopen(SUPPLIER_FILE, "r");
    if (check) {
        char line[256];
        Supplier temp;
        while (fgets(line, sizeof(line), check)) {
            if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^\n]", temp.id, temp.name, temp.phone, temp.email) == 4) {
                if (strcmp(temp.id, s.id) == 0) {
                    printf("Supplier ID %s already exists!\n", s.id);
                    fclose(check);
                    fclose(fp);
                    pause();
                    return;
                }
            }
        }
        fclose(check);
    }
    
    printf("Enter Name: ");
    scanf(" %49[^\n]", s.name);
    
    printf("Enter Phone (5 to 15 digits only): ");
    scanf(" %19[^\n]", s.phone);
    if (!isValidPhone(s.phone)) {
        printf("Invalid phone. Must be 5 to 15 digits.\n");
        fclose(fp);
        pause();
        return;
    }
    
    printf("Enter Email: ");
    scanf(" %49[^\n]", s.email);
    if (!isValidEmail(s.email)) {
        printf("Invalid email format.\n");
        fclose(fp);
        pause();
        return;
    }
    
    fprintf(fp, "%s|%s|%s|%s\n", s.id, s.name, s.phone, s.email);
    fclose(fp);
    printf("Supplier added successfully!\n");
    pause();
}

void viewSuppliers() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf("No suppliers found or cannot open supplier file.\n");
        pause();
        return;
    }
    
    Supplier s;
    char line[256];
    int count = 0;
    
    printf("%-8s %-20s %-15s %-30s\n", "ID", "Name", "Phone", "Email");
    printf("-------------------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^\n]", s.id, s.name, s.phone, s.email) == 4) {
            printf("%-8s %-20s %-15s %-30s\n", s.id, s.name, s.phone, s.email);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No suppliers found.\n");
    } else {
        printf("\nTotal suppliers: %d\n", count);
    }
    
    fclose(fp);
    pause();
}

void updateSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf("Cannot open supplier file.\n");
        pause();
        return;
    }
    
    // Use current directory instead of data/ subdirectory
    FILE *temp = fopen("temp_supplier.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        pause();
        return;
    }
    
    Supplier s;
    char id[10];
    int found = 0;
    char line[256];
    
    printf("Enter Supplier ID to update: ");
    scanf("%9s", id);
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^\n]", s.id, s.name, s.phone, s.email) == 4) {
            if (strcmp(s.id, id) == 0) {
                found = 1;
                printf("Current: ID: %s | Name: %s | Phone: %s | Email: %s\n", s.id, s.name, s.phone, s.email);
                
                printf("Enter new name: ");
                scanf(" %49[^\n]", s.name);
                
                printf("Enter new phone: ");
                scanf(" %19[^\n]", s.phone);
                if (!isValidPhone(s.phone)) {
                    printf("Invalid phone. Must be 5 to 15 digits.\n");
                    fclose(fp); fclose(temp); remove("temp_supplier.txt");
                    pause(); return;
                }
                
                printf("Enter new email: ");
                scanf(" %49[^\n]", s.email);
                if (!isValidEmail(s.email)) {
                    printf("Invalid email format.\n");
                    fclose(fp); fclose(temp); remove("temp_supplier.txt");
                    pause(); return;
                }
            }
            fprintf(temp, "%s|%s|%s|%s\n", s.id, s.name, s.phone, s.email);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(SUPPLIER_FILE);
        rename("temp_supplier.txt", SUPPLIER_FILE);
        printf("Supplier updated successfully!\n");
    } else {
        remove("temp_supplier.txt");
        printf("Supplier ID %s not found.\n", id);
    }
    
    pause();
}

void deleteSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf("Cannot open supplier file.\n");
        pause();
        return;
    }
    
    // Use current directory instead of data/ subdirectory
    FILE *temp = fopen("temp_supplier.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        pause();
        return;
    }
    
    Supplier s;
    char id[10];
    int found = 0;
    char line[256];
    
    printf("Enter Supplier ID to delete: ");
    scanf("%9s", id);
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^\n]", s.id, s.name, s.phone, s.email) == 4) {
            if (strcmp(s.id, id) == 0) {
                found = 1;
                printf("Deleting: ID: %s | Name: %s | Phone: %s | Email: %s\n", s.id, s.name, s.phone, s.email);
            } else {
                fprintf(temp, "%s|%s|%s|%s\n", s.id, s.name, s.phone, s.email);
            }
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(SUPPLIER_FILE);
        rename("temp_supplier.txt", SUPPLIER_FILE);
        printf("Supplier deleted successfully!\n");
    } else {
        remove("temp_supplier.txt");
        printf("Supplier ID %s not found.\n", id);
    }
    
    pause();
}

void filterProductsBySupplier(const char *supplierID) {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp) {
        printf("Cannot open product file.\n");
        return;
    }
    
    Product p;
    char line[256];
    int found = 0;
    
    printf("%-10s %-20s %-10s %-8s\n", "Product ID", "Name", "Price", "Quantity");
    printf("-----------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        // Fixed: Parse quantity first, then price to match expected data format
        if (sscanf(line, "%9[^|]|%49[^|]|%9[^|]|%9[^|]|%d|%f",
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            if (strcmp(p.supplierID, supplierID) == 0) {
                printf("%-10s %-20s $%-9.2f %-8d\n", p.id, p.name, p.price, p.quantity);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No products found for supplier %s.\n", supplierID);
    }
    
    fclose(fp);
}