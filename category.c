#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "category.h"
#include "utils.h"

/* Function to display category management menu */
void displayCategoryMenu(void) {
    printf("\n========================================\n");
    printf("      CATEGORY MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("| 1. Add Category                  |\n");
    printf("| 2. View All Categories           |\n");
    printf("| 3. Update Category               |\n");
    printf("| 4. Delete Category               |\n");
    printf("| 5. Filter Products by Category   |\n");
    printf("| 0. Back to Main Menu             |\n");
    printf("========================================");
    printf("\nEnter your choice: ");
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
            if (scanf("%d", &categoryID) != 1) {
                printf("Invalid input!\n");
                clearInputBuffer();
                pause();
                break;
            }
            clearInputBuffer();
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
        
        if (scanf("%d", &choice) != 1) {
            choice = -1; // Set to invalid choice
        }
        clearInputBuffer();
        
        handleCategoryMenuChoice(choice);
        
    } while (choice != 0);
}

void addCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "a");
    if (fp == NULL) {
        printf("Error: Unable to open category file!\n");
        return;
    }
    
    Category c;
    printf("ID: "); 
    if (scanf("%d", &c.id) != 1) {
        printf("Invalid ID input!\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }
    clearInputBuffer();
    
    printf("Name: "); 
    if (fgets(c.name, sizeof(c.name), stdin) != NULL) {
        // Remove newline if present
        char *newlinePos = strchr(c.name, '\n');
        if (newlinePos) *newlinePos = '\0';
    }
    
    printf("Description: "); 
    if (fgets(c.description, sizeof(c.description), stdin) != NULL) {
        // Remove newline if present
        char *newlinePos = strchr(c.description, '\n');
        if (newlinePos) *newlinePos = '\0';
    }
    
    fprintf(fp, "%d|%s|%s\n", c.id, c.name, c.description);
    fclose(fp);
}

void viewCategories() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    if (fp == NULL) {
        printf("No categories found or unable to open category file.\n");
        return;
    }
    
    Category c;
    char line[200];
    int count = 0;
    
    printf("%-5s %-20s %-30s\n", "ID", "Name", "Description");
    printf("-------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%99[^\n]", &c.id, c.name, c.description) == 3) {
            printf("%-5d %-20s %-30s\n", c.id, c.name, c.description);
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
    
    FILE *temp = fopen("data/temp.txt", "w");
    if (temp == NULL) {
        printf("Error: Unable to create temporary file!\n");
        fclose(fp);
        return;
    }
    
    Category c;
    int id, found = 0;
    char line[200];
    
    printf("Enter Category ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID input!\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("data/temp.txt");
        return;
    }
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%99[^\n]", &c.id, c.name, c.description) == 3) {
            if (c.id == id) {
                found = 1;
                printf("Current Name: %s\n", c.name);
                printf("New Name: "); 
                if (fgets(c.name, sizeof(c.name), stdin) != NULL) {
                    char *newlinePos = strchr(c.name, '\n');
                    if (newlinePos) *newlinePos = '\0';
                }
                
                printf("Current Description: %s\n", c.description);
                printf("New Description: "); 
                if (fgets(c.description, sizeof(c.description), stdin) != NULL) {
                    char *newlinePos = strchr(c.description, '\n');
                    if (newlinePos) *newlinePos = '\0';
                }
            }
            fprintf(temp, "%d|%s|%s\n", c.id, c.name, c.description);
        }
    }
    
    fclose(fp); 
    fclose(temp);
    
    if (found) {
        remove(CATEGORY_FILE); 
        rename("data/temp.txt", CATEGORY_FILE);
        printf("Category updated successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("Category ID %d not found.\n", id);
    }
}

void deleteCategory() {
    FILE *fp = fopen(CATEGORY_FILE, "r");
    if (fp == NULL) {
        printf("No categories found or unable to open category file.\n");
        return;
    }
    
    FILE *temp = fopen("data/temp.txt", "w");
    if (temp == NULL) {
        printf("Error: Unable to create temporary file!\n");
        fclose(fp);
        return;
    }
    
    Category c;
    int id, found = 0;
    char line[200];
    
    printf("Enter Category ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID input!\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("data/temp.txt");
        return;
    }
    clearInputBuffer();
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%99[^\n]", &c.id, c.name, c.description) == 3) {
            if (c.id != id) {
                fprintf(temp, "%d|%s|%s\n", c.id, c.name, c.description);
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
        rename("data/temp.txt", CATEGORY_FILE);
        printf("Category deleted successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("Category ID %d not found.\n", id);
    }
}

void filterProductsByCategory(int categoryID) {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (fp == NULL) {
        printf("No products found or unable to open product file.\n");
        return;
    }
    
    Product p;
    char line[256];
    int found = 0;
    
    // Format category ID string (e.g., 1 → "C00001")
    char categoryIDStr[10];
    snprintf(categoryIDStr, sizeof(categoryIDStr), "C%05d", categoryID);
    
    printf("%-10s %-20s %-10s %-10s %-8s %-5s\n", 
           "ID", "Name", "Category", "Supplier", "Price", "Qty");
    printf("-----------------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%9[^|]|%49[^|]|%9[^|]|%9[^|]|%f|%d",
                   p.id, p.name, p.category, p.supplierID, &p.price, &p.quantity) == 6) {
            if (strcmp(p.category, categoryIDStr) == 0) {
                printf("%-10s %-20s %-10s %-10s $%-7.2f %-5d\n",
                       p.id, p.name, p.category, p.supplierID, p.price, p.quantity);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No products found for category %s.\n", categoryIDStr);
    }
    
    fclose(fp);
}