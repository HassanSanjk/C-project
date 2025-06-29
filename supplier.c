#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supplier.h"
#include "utils.h"

/* Function to display supplier management menu */
void displaySupplierMenu(void) {
    printf("\n========================================\n");
    printf("      SUPPLIER MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. Add Supplier                  |\n");
    printf("| 2. View All Suppliers            |\n");
    printf("| 3. Update Supplier               |\n");
    printf("| 4. Delete Supplier               |\n");
    printf("| 5. Filter Products by Supplier   |\n");
    printf("| 0. Back to Main Menu             |\n");
    printf("========================================");
    printf("\nEnter your choice: ");
}

/* Function to handle supplier menu choices */
void handleSupplierMenuChoice(int choice) {
    int supplierID;
    
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
            printf("Enter Supplier ID: ");
            if (scanf("%d", &supplierID) != 1) {
                printf("Invalid input!\n");
                clearInputBuffer();
                pause();
                return;
            }
            clearInputBuffer();
            printf("\nProducts from Supplier ID %d:\n", supplierID);
            printf("----------------------------------------\n");
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

void addSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "a");
    if (!fp) {
        printf("Cannot open supplier file.\n");
        pause();
        return;
    }
    
    Supplier s;
    printf("Enter Supplier ID: ");
    if (scanf("%d", &s.id) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        fclose(fp);
        pause();
        return;
    }
    clearInputBuffer();
    
    // Check if ID already exists
    FILE *check = fopen(SUPPLIER_FILE, "r");
    if (check) {
        char line[200];
        Supplier temp;
        while (fgets(line, sizeof(line), check)) {
            if (sscanf(line, "%d|%49[^|]|%99[^\n]", &temp.id, temp.name, temp.contact) == 3) {
                if (temp.id == s.id) {
                    printf("Supplier ID %d already exists!\n", s.id);
                    fclose(check);
                    fclose(fp);
                    pause();
                    return;
                }
            }
        }
        fclose(check);
    }
    
    printf("Enter Supplier Name: ");
    if (!fgets(s.name, sizeof(s.name), stdin)) {
        printf("Invalid input!\n");
        fclose(fp);
        pause();
        return;
    }
    s.name[strcspn(s.name, "\n")] = 0; // Remove newline
    
    printf("Enter Contact Information: ");
    if (!fgets(s.contact, sizeof(s.contact), stdin)) {
        printf("Invalid input!\n");
        fclose(fp);
        pause();
        return;
    }
    s.contact[strcspn(s.contact, "\n")] = 0; // Remove newline
    
    fprintf(fp, "%d|%s|%s\n", s.id, s.name, s.contact);
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
    char line[200];
    int count = 0;
    
    printf("%-5s %-20s %-30s\n", "ID", "Name", "Contact");
    printf("--------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%99[^\n]", &s.id, s.name, s.contact) == 3) {
            printf("%-5d %-20s %-30s\n", s.id, s.name, s.contact);
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
    
    FILE *temp = fopen("data/temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        pause();
        return;
    }
    
    Supplier s;
    int id, found = 0;
    char line[200];
    
    printf("Enter Supplier ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("data/temp.txt");
        pause();
        return;
    }
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%99[^\n]", &s.id, s.name, s.contact) == 3) {
            if (s.id == id) {
                found = 1;
                printf("Current: ID: %d | Name: %s | Contact: %s\n", s.id, s.name, s.contact);
                
                printf("Enter new name: ");
                if (!fgets(s.name, sizeof(s.name), stdin)) {
                    printf("Invalid input!\n");
                    fclose(fp); fclose(temp); remove("data/temp.txt");
                    pause(); return;
                }
                s.name[strcspn(s.name, "\n")] = 0;
                
                printf("Enter new contact: ");
                if (!fgets(s.contact, sizeof(s.contact), stdin)) {
                    printf("Invalid input!\n");
                    fclose(fp); fclose(temp); remove("data/temp.txt");
                    pause(); return;
                }
                s.contact[strcspn(s.contact, "\n")] = 0;
            }
            fprintf(temp, "%d|%s|%s\n", s.id, s.name, s.contact);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(SUPPLIER_FILE);
        rename("data/temp.txt", SUPPLIER_FILE);
        printf("Supplier updated successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("Supplier ID %d not found.\n", id);
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
    
    FILE *temp = fopen("data/temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        pause();
        return;
    }
    
    Supplier s;
    int id, found = 0;
    char line[200];
    
    printf("Enter Supplier ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("data/temp.txt");
        pause();
        return;
    }
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%99[^\n]", &s.id, s.name, s.contact) == 3) {
            if (s.id == id) {
                found = 1;
                printf("Deleting: ID: %d | Name: %s | Contact: %s\n", s.id, s.name, s.contact);
            } else {
                fprintf(temp, "%d|%s|%s\n", s.id, s.name, s.contact);
            }
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(SUPPLIER_FILE);
        rename("data/temp.txt", SUPPLIER_FILE);
        printf("Supplier deleted successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("Supplier ID %d not found.\n", id);
    }
    
    pause();
}

void filterProductsBySupplier(int supplierID) {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp) {
        printf("Cannot open product file.\n");
        return;
    }
    
    Product p;
    char line[256];
    int found = 0;
    
    // Format the supplier ID string (e.g., 1 → "S00001")
    char supplierIDStr[10];
    snprintf(supplierIDStr, sizeof(supplierIDStr), "S%05d", supplierID);
    
    printf("%-10s %-20s %-10s %-8s\n", "Product ID", "Name", "Price", "Quantity");
    printf("-----------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%9[^|]|%49[^|]|%9[^|]|%9[^|]|%f|%d",
                   p.id, p.name, p.category, p.supplierID, &p.price, &p.quantity) == 6) {
            if (strcmp(p.supplierID, supplierIDStr) == 0) {
                printf("%-10s %-20s $%-9.2f %-8d\n", p.id, p.name, p.price, p.quantity);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No products found for supplier %s.\n", supplierIDStr);
    }
    
    fclose(fp);
}