#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "category.h"
#include "utils.h"

/* Function to display category management menu */
void displayCategoryMenu(void) {
    printf("\n========================================\n");
    printf("      CATEGORY MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("| 1. Add Category                      |\n");
    printf("| 2. View All Categories               |\n");
    printf("| 3. Update Category                   |\n");
    printf("| 4. Delete Category                   |\n");
    printf("| 5. Filter Products by Category       |\n");
    printf("| 0. Back to Main Menu                 |\n");
    printf("========================================");
    printf("\nEnter your choice: ");
}

/* Function to handle category menu choices */
void handleCategoryMenuChoice(int choice) {
    char categoryID[10];
    
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
            printf("Enter Category ID (e.g., C00001): ");
            scanf("%9s", categoryID);
            clearInputBuffer();
            printf("\nProducts in Category ID %s:\n", categoryID);
            printf("-----------------------------------------------------\n");
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
        
        if (scanf("%d", &choice) != 1) {
            choice = -1; // Set to invalid choice
        }
        clearInputBuffer();
        
        handleCategoryMenuChoice(choice);
        
    } while (choice != 0);
}

int isValidCategory(const char *id) {
    if (strlen(id) != 6 || id[0] != 'C') return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

void addCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "a");
    if (fp == NULL) {
        printf("Error: Unable to open category file!\n");
        return;
    }
    
    Category c;
    printf("Enter Category ID (e.g., C00001): ");
    scanf("%9s", c.id);
    if (!isValidCategory(c.id)) {
        printf("Invalid Category ID format. Must start with 'C' and 5 digits.\n");
        fclose(fp);
        return;
    }
    
    // Check if ID already exists
    FILE *check = fopen(CATEGORY_FILE, "r");
    if (check) {
        char line[100];
        Category temp;
        while (fgets(line, sizeof(line), check)) {
            if (sscanf(line, "%[^|]|%[^\n]", temp.id, temp.name) == 2) {
                if (strcmp(temp.id, c.id) == 0) {
                    printf("Category ID %s already exists!\n", c.id);
                    fclose(check);
                    fclose(fp);
                    return;
                }
            }
        }
        fclose(check);
    }
    
    printf("Enter Category Name: ");
    scanf(" %49[^\n]", c.name);
    
    fprintf(fp, "%s|%s\n", c.id, c.name);
    fclose(fp);
}

void viewCategories() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    if (fp == NULL) {
        printf("No categories found or unable to open category file.\n");
        return;
    }
    
    Category c;
    char line[100];
    int count = 0;
    
    printf("%-8s %-30s\n", "ID", "Name");
    printf("------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^\n]", c.id, c.name) == 2) {
            printf("%-8s %-30s\n", c.id, c.name);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No categories found.\n");
    } else {
        printf("\nTotal categories: %d\n", count);
    }
    
    fclose(fp);
}

void updateCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    if (fp == NULL) {
        printf("No categories found or unable to open category file.\n");
        return;
    }
    
    // Use current directory instead of data/ subdirectory
    FILE *temp = fopen("temp_category.txt", "w");
    if (temp == NULL) {
        printf("Error: Unable to create temporary file!\n");
        fclose(fp);
        return;
    }
    
    Category c;
    char id[10];
    int found = 0;
    char line[100];
    
    printf("Enter Category ID to update: ");
    scanf("%9s", id);
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^\n]", c.id, c.name) == 2) {
            if (strcmp(c.id, id) == 0) {
                found = 1;
                printf("Current Name: %s\n", c.name);
                printf("New Name: ");
                scanf(" %49[^\n]", c.name);
            }
            fprintf(temp, "%s|%s\n", c.id, c.name);
        }
    }
    
    fclose(fp); 
    fclose(temp);
    
    if (found) {
        remove(CATEGORY_FILE); 
        rename("temp_category.txt", CATEGORY_FILE);
        printf("Category updated successfully!\n");
    } else {
        remove("temp_category.txt");
        printf("Category ID %s not found.\n", id);
    }
}

void deleteCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    if (fp == NULL) {
        printf("No categories found or unable to open category file.\n");
        return;
    }
    
    // Use current directory instead of data/ subdirectory
    FILE *temp = fopen("temp_category.txt", "w");
    if (temp == NULL) {
        printf("Error: Unable to create temporary file!\n");
        fclose(fp);
        return;
    }
    
    Category c;
    char id[10];
    int found = 0;
    char line[100];
    
    printf("Enter Category ID to delete: ");
    scanf("%9s", id);
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^\n]", c.id, c.name) == 2) {
            if (strcmp(c.id, id) != 0) {
                fprintf(temp, "%s|%s\n", c.id, c.name);
            } else {
                found = 1;
                printf("Deleting category: %s\n", c.name);
            }
        }
    }
    
    fclose(fp); 
    fclose(temp);
    
    if (found) {
        remove(CATEGORY_FILE); 
        rename("temp_category.txt", CATEGORY_FILE);
        printf("Category deleted successfully!\n");
    } else {
        remove("temp_category.txt");
        printf("Category ID %s not found.\n", id);
    }
}

void filterProductsByCategory(const char *categoryID) {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (fp == NULL) {
        printf("No products found or unable to open product file.\n");
        return;
    }
    
    Product p;
    char line[256];
    int found = 0;
    
    printf("%-10s %-20s %-10s %-10s %-8s %-5s\n", 
           "ID", "Name", "Category", "Supplier", "Price", "Qty");
    printf("-----------------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        // Fixed: Parse quantity first, then price to match expected data format
        if (sscanf(line, "%9[^|]|%49[^|]|%9[^|]|%9[^|]|%d|%f",
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            if (strcmp(p.category, categoryID) == 0) {
                printf("%-10s %-20s %-10s %-10s $%-7.2f %-5d\n",
                       p.id, p.name, p.category, p.supplierID, p.price, p.quantity);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No products found for category %s.\n", categoryID);
    }
    
    fclose(fp);
}