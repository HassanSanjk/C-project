#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/supplier.h"

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
    Product p;
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%d|%d|%f|%d", &p.productID, p.name, &p.categoryID, &p.supplierID, &p.price, &p.quantity);
        if (p.supplierID == supplierID) {
            printf("ID: %d | Name: %s | Price: %.2f | Qty: %d\n", p.productID, p.name, p.price, p.quantity);
            found = 1;
        }
    }
    if (!found) printf("No products found.\n");
    fclose(fp);
}
