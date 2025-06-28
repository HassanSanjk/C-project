#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "category.h"
#include "utils.h"

/* Function to display category management menu */
void displayCategoryMenu(void) {
    printf("\n--- Category Management ---\n");
    printf("1. Add Category\n");
    printf("2. View All Categories\n");
    printf("3. Update Category\n");
    printf("4. Delete Category\n");
    printf("5. Filter Products by Category\n");
    printf("0. Back to Main Menu\n");
}

/* Function to handle category menu choices */
void handleCategoryMenuChoice(int choice) {
    int categoryID;
    
    switch (choice) {
        case 1:
            printf("\n=== ADD CATEGORY ===\n");
            addCategory();
            printf("Category added successfully!\n");
            pause();
            break;
            
        case 2:
            printf("\n=== ALL CATEGORIES ===\n");
            viewCategories();
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE CATEGORY ===\n");
            updateCategory();
            printf("Category update completed!\n");
            pause();
            break;
            
        case 4:
            printf("\n=== DELETE CATEGORY ===\n");
            deleteCategory();
            printf("Category deletion completed!\n");
            pause();
            break;
            
        case 5:
            printf("\n=== FILTER PRODUCTS BY CATEGORY ===\n");
            printf("Enter Category ID: ");
            scanf("%d", &categoryID);
            printf("\nProducts in Category ID %d:\n", categoryID);
            printf("----------------------------------------\n");
            filterProductsByCategory(categoryID);
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

/* Main category management menu function */
void categoryManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displayCategoryMenu();
        printf("Select: ");
        scanf("%d", &choice);
        handleCategoryMenuChoice(choice);
        
    } while (choice != 0);
}

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
    if (fp == NULL) {
        perror("Failed to open product file");
        return;
    }
    Product p;
    char line[256];
    int found = 0;
    // Format category ID string (e.g., 1 → "C00001")
    char categoryIDStr[10];
    snprintf(categoryIDStr, sizeof(categoryIDStr), "C%05d", categoryID);
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%9[^|]|%49[^|]|%9[^|]|%9[^|]|%f|%d",
                   p.id, p.name, p.category, p.supplierID, &p.price, &p.quantity) == 6) {
            if (strcmp(p.category, categoryIDStr) == 0) {
                printf("ID: %s | Name: %s | Price: %.2f | Qty: %d\n",
                       p.id, p.name, p.price, p.quantity);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("No products found for category %s.\n", categoryIDStr);
    }
    fclose(fp);
}