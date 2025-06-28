#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supplier.h"
#include "utils.h"


/* Function to display supplier management menu */
void displaySupplierMenu(void) {
    printf("\n--- Supplier Management ---\n");
    printf("1. Add Supplier\n");
    printf("2. View All Suppliers\n");
    printf("3. Update Supplier\n");
    printf("4. Delete Supplier\n");
    printf("5. Filter Products by Supplier\n");
    printf("0. Back to Main Menu\n");
}

/* Function to handle supplier menu choices */
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

/* Main supplier management menu function */
void supplierManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displaySupplierMenu();
        printf("Select: ");
        scanf("%d", &choice);
        handleSupplierMenuChoice(choice);
        
    } while (choice != 0);
}


void addSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "a");
    Supplier s;
    printf("ID: "); scanf("%d", &s.id);
    getchar();
    printf("Name: "); fgets(s.name, 50, stdin); strtok(s.name, "\n");
    printf("Contact: "); fgets(s.contact, 100, stdin); strtok(s.contact, "\n");
    fprintf(fp, "%d|%s|%s\n", s.id, s.name, s.contact);
    fclose(fp);
}

void viewSuppliers() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    Supplier s;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%99[^\n]", &s.id, s.name, s.contact);
        printf("ID: %d | Name: %s | Contact: %s\n", s.id, s.name, s.contact);
    }
    fclose(fp);
}

void updateSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    Supplier s;
    int id, found = 0;
    char line[200];
    printf("Update ID: "); scanf("%d", &id); getchar();
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%99[^\n]", &s.id, s.name, s.contact);
        if (s.id == id) {
            found = 1;
            printf("New Name: "); fgets(s.name, 50, stdin); strtok(s.name, "\n");
            printf("New Contact: "); fgets(s.contact, 100, stdin); strtok(s.contact, "\n");
        }
        fprintf(temp, "%d|%s|%s\n", s.id, s.name, s.contact);
    }
    fclose(fp); fclose(temp);
    remove(SUPPLIER_FILE); rename("data/temp.txt", SUPPLIER_FILE);
    if (!found) printf("ID not found.\n");
}

void deleteSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    Supplier s;
    int id, found = 0;
    char line[200];
    printf("Delete ID: "); scanf("%d", &id);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%99[^\n]", &s.id, s.name, s.contact);
        if (s.id != id)
            fprintf(temp, "%d|%s|%s\n", s.id, s.name, s.contact);
        else
            found = 1;
    }
    fclose(fp); fclose(temp);
    remove(SUPPLIER_FILE); rename("data/temp.txt", SUPPLIER_FILE);
    if (!found) printf("ID not found.\n");
}

void filterProductsBySupplier(int supplierID) {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (fp == NULL) {
        perror("Failed to open product file");
        return;
    }
    Product p;
    char line[256];
    int found = 0;
    // Format the supplier ID string (e.g., 1 → "S00001")
    char supplierIDStr[10];
    snprintf(supplierIDStr, sizeof(supplierIDStr), "S%05d", supplierID);
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%9[^|]|%49[^|]|%9[^|]|%9[^|]|%f|%d",
                   p.id, p.name, p.category, p.supplierID, &p.price, &p.quantity) == 6) {
            if (strcmp(p.supplierID, supplierIDStr) == 0) {
                printf("ID: %s | Name: %s | Price: %.2f | Qty: %d\n",
                       p.id, p.name, p.price, p.quantity);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("No products found for supplier %s.\n", supplierIDStr);
    }
    fclose(fp);
}
