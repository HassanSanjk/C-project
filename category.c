#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/category.h"

void addCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "a");
    Category c;
    printf("ID: "); scanf("%d", &c.id);
    getchar();
    printf("Name: "); fgets(c.name, 50, stdin); strtok(c.name, "\n");
    printf("Description: "); fgets(c.description, 100, stdin); strtok(c.description, "\n");
    fprintf(fp, "%d|%s|%s\n", c.id, c.name, c.description);
    fclose(fp);
}

void viewCategories() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    Category c;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%99[^\n]", &c.id, c.name, c.description);
        printf("ID: %d | Name: %s | Desc: %s\n", c.id, c.name, c.description);
    }
    fclose(fp);
}

void updateCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    Category c;
    int id, found = 0;
    char line[200];
    printf("Update ID: "); scanf("%d", &id); getchar();
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%99[^\n]", &c.id, c.name, c.description);
        if (c.id == id) {
            found = 1;
            printf("New Name: "); fgets(c.name, 50, stdin); strtok(c.name, "\n");
            printf("New Desc: "); fgets(c.description, 100, stdin); strtok(c.description, "\n");
        }
        fprintf(temp, "%d|%s|%s\n", c.id, c.name, c.description);
    }
    fclose(fp); fclose(temp);
    remove(CATEGORY_FILE); rename("data/temp.txt", CATEGORY_FILE);
    if (!found) printf("ID not found.\n");
}

void deleteCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    Category c;
    int id, found = 0;
    char line[200];
    printf("Delete ID: "); scanf("%d", &id);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%99[^\n]", &c.id, c.name, c.description);
        if (c.id != id)
            fprintf(temp, "%d|%s|%s\n", c.id, c.name, c.description);
        else
            found = 1;
    }
    fclose(fp); fclose(temp);
    remove(CATEGORY_FILE); rename("data/temp.txt", CATEGORY_FILE);
    if (!found) printf("ID not found.\n");
}

void filterProductsByCategory(int categoryID) {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    Product p;
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%d|%d|%f|%d", &p.productID, p.name, &p.categoryID, &p.supplierID, &p.price, &p.quantity);
        if (p.categoryID == categoryID) {
            printf("ID: %d | Name: %s | Price: %.2f | Qty: %d\n", p.productID, p.name, p.price, p.quantity);
            found = 1;
        }
    }
    if (!found) printf("No products found.\n");
    fclose(fp);
}
