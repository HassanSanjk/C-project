#include "product.h"
#include "utils.h"

// Check if an ID exists in a file (generic) - from your friend's code
int exists_in_file(const char *filename, const char *targetID) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char line[256], id[20];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]", id);
        if (strcmp(id, targetID) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Validation functions
int isValidProduct(const char *id) {
    if (id[0] != 'P' || strlen(id) != 6) return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

int isValidCategoryID(const char *id) {
    if (id[0] != 'C' || strlen(id) != 6) return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

int isValidSupplierID(const char *id) {
    if (id[0] != 'S' || strlen(id) != 6) return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}


// Safe string input function
void getStringInput(char *buffer, int size, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        // Remove newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

// Safe integer input function
int getIntInput(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                if (value >= min && value <= max) {
                    return value;
                }
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

// Safe float input function
float getFloatInput(const char *prompt, float min) {
    float value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%f", &value) == 1) {
                if (value >= min) {
                    return value;
                }
            }
        }
        printf("Invalid input. Please enter a number greater than %.2f.\n", min);
    }
}

// Add Product - Enhanced version of your friend's code
void addProduct() {
    Product p;
    char idInput[10];

    printf("\n=== ADD NEW PRODUCT ===\n");
    
    // Product ID validation
    getStringInput(idInput, sizeof(idInput), "Enter Product ID (e.g., P00001): ");
    
    if (!isValidProduct(idInput)) {
        printf("Invalid Product ID format. Must start with 'P' followed by 5 digits.\n");
        return;
    }
    
    if (exists_in_file(PRODUCT_FILE, idInput)) {
        printf("Product ID already exists.\n");
        return;
    }
    strcpy(p.id, idInput);

    // Product name
    getStringInput(p.name, sizeof(p.name), "Enter Product Name: ");
    if (strlen(p.name) == 0) {
        printf("Product name cannot be empty.\n");
        return;
    }

    // Category ID validation
    getStringInput(p.category, sizeof(p.category), "Enter Category ID (e.g., C00001): ");
    
    if (!isValidCategoryID(p.category)) {
        printf("Invalid Category ID format. Must start with 'C' followed by 5 digits.\n");
        return;
    }
    
    if (!exists_in_file(CATEGORY_FILE, p.category)) {
        printf("Warning: Category ID does not exist. Creating product anyway.\n");
        printf("Note: Please ensure the category exists for proper referential integrity.\n");
    }

    // Supplier ID validation
    getStringInput(p.supplierID, sizeof(p.supplierID), "Enter Supplier ID (e.g., S00001): ");
    
    if (!isValidSupplierID(p.supplierID)) {
        printf("Invalid Supplier ID format. Must start with 'S' followed by 5 digits.\n");
        return;
    }
    
    if (!exists_in_file(SUPPLIER_FILE, p.supplierID)) {
        printf("Warning: Supplier ID does not exist. Creating product anyway.\n");
        printf("Note: Please ensure the supplier exists for proper referential integrity.\n");
    }

    // Quantity validation
    p.quantity = getIntInput("Enter Quantity: ", 0, 999999);

    // Price validation
    p.price = getFloatInput("Enter Price (RM): ", 0.01);

    // Save to file
    FILE *fp = fopen(PRODUCT_FILE, "a");
    if (!fp) {
        printf("Error opening product file.\n");
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%d|%.2f\n", p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
    fclose(fp);

    printf("\nProduct added successfully!\n");
    printf("Product ID: %s\n", p.id);
    printf("Product Name: %s\n", p.name);
    printf("Category: %s\n", p.category);
    printf("Supplier: %s\n", p.supplierID);
    printf("Quantity: %d\n", p.quantity);
    printf("Price: RM%.2f\n", p.price);
}

// View All Products - Enhanced version
void viewProducts() {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp) {
        printf("Cannot open product file or no products exist yet.\n");
        printf("Note: File '%s' will be created when you add your first product.\n", PRODUCT_FILE);
        return;
    }

    Product p;
    char line[256];
    int count = 0;
    
    printf("\n================================================================================\n");
    printf("                              PRODUCT LIST                                     \n");
    printf("================================================================================\n");
    printf("%-8s %-20s %-8s %-8s %-8s %-10s\n", 
           "ID", "Name", "Category", "Supplier", "Quantity", "Price (RM)");
    printf("================================================================================\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            printf("%-8s %-20.19s %-8s %-8s %-8d RM%-7.2f\n",
                   p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
            count++;
        }
    }
    
    printf("================================================================================\n");
    if (count == 0) {
        printf("No products found in the database.\n");
    } else {
        printf("Total Products: %d\n", count);
    }
    fclose(fp);
}

// View Single Product
void viewProduct() {
    char targetID[10];
    printf("\n=== VIEW PRODUCT DETAILS ===\n");
    getStringInput(targetID, sizeof(targetID), "Enter Product ID: ");

    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp) {
        printf("Cannot open product file or no products exist yet.\n");
        return;
    }

    Product p;
    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            if (strcmp(p.id, targetID) == 0) {
                found = 1;
                break;
            }
        }
    }
    fclose(fp);

    if (found) {
        printf("\n=== PRODUCT DETAILS ===\n");
        printf("Product ID: %s\n", p.id);
        printf("Name: %s\n", p.name);
        printf("Category ID: %s\n", p.category);
        printf("Supplier ID: %s\n", p.supplierID);
        printf("Quantity: %d\n", p.quantity);
        printf("Price: RM%.2f\n", p.price);
        printf("Total Value: RM%.2f\n", p.quantity * p.price);
        
        // Low stock warning
        if (p.quantity <= 10) {
            printf("\n*** LOW STOCK WARNING! ***\n");
        }
    } else {
        printf("Product with ID '%s' not found.\n", targetID);
    }
}

// Update Product - Enhanced version of your friend's code
void updateProduct() {
    char targetID[10];
    printf("\n=== UPDATE PRODUCT ===\n");
    getStringInput(targetID, sizeof(targetID), "Enter Product ID to update: ");

    FILE *fp = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp) {
        printf("Cannot open product file or no products exist yet.\n");
        if (temp) fclose(temp);
        return;
    }
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Product p;
    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            
            if (strcmp(p.id, targetID) == 0) {
                found = 1;
                
                printf("\nCurrent Product Details:\n");
                printf("ID: %s | Name: %s | Category: %s | Supplier: %s | Qty: %d | Price: RM%.2f\n",
                       p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
                
                printf("\nEnter new details (or press Enter to keep current value):\n");
                
                // Update name
                char tempInput[100];
                printf("New Product Name (current: %s): ", p.name);
                if (fgets(tempInput, sizeof(tempInput), stdin) && strlen(tempInput) > 1) {
                    tempInput[strlen(tempInput)-1] = '\0'; // Remove newline
                    if (strlen(tempInput) > 0) {
                        strcpy(p.name, tempInput);
                    }
                }

                // Update category
                printf("New Category ID (current: %s): ", p.category);
                if (fgets(tempInput, sizeof(tempInput), stdin) && strlen(tempInput) > 1) {
                    tempInput[strlen(tempInput)-1] = '\0';
                    if (strlen(tempInput) > 0) {
                        if (isValidCategoryID(tempInput)) {
                            strcpy(p.category, tempInput);
                        } else {
                            printf("Invalid Category ID format. Keeping current value.\n");
                        }
                    }
                }

                // Update supplier
                printf("New Supplier ID (current: %s): ", p.supplierID);
                if (fgets(tempInput, sizeof(tempInput), stdin) && strlen(tempInput) > 1) {
                    tempInput[strlen(tempInput)-1] = '\0';
                    if (strlen(tempInput) > 0) {
                        if (isValidSupplierID(tempInput)) {
                            strcpy(p.supplierID, tempInput);
                        } else {
                            printf("Invalid Supplier ID format. Keeping current value.\n");
                        }
                    }
                }

                // Update quantity
                printf("New Quantity (current: %d): ", p.quantity);
                if (fgets(tempInput, sizeof(tempInput), stdin) && strlen(tempInput) > 1) {
                    int newQty;
                    if (sscanf(tempInput, "%d", &newQty) == 1 && newQty >= 0) {
                        p.quantity = newQty;
                    } else if (strlen(tempInput) > 1) {
                        printf("Invalid quantity. Keeping current value.\n");
                    }
                }

                // Update price
                printf("New Price (current: RM%.2f): RM", p.price);
                if (fgets(tempInput, sizeof(tempInput), stdin) && strlen(tempInput) > 1) {
                    float newPrice;
                    if (sscanf(tempInput, "%f", &newPrice) == 1 && newPrice > 0) {
                        p.price = newPrice;
                    } else if (strlen(tempInput) > 1) {
                        printf("Invalid price. Keeping current value.\n");
                    }
                }
            }
            fprintf(temp, "%s|%s|%s|%s|%d|%.2f\n", p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
        }
    }

    fclose(fp);
    fclose(temp);
    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);

    if (found) {
        printf("\nProduct updated successfully!\n");
    } else {
        printf("Product with ID '%s' not found.\n", targetID);
    }
}

// Delete Product - Your friend's code with enhancements
void deleteProduct() {
    char targetID[10];
    printf("\n=== DELETE PRODUCT ===\n");
    getStringInput(targetID, sizeof(targetID), "Enter Product ID to delete: ");

    // First, show the product to be deleted
    FILE *fpView = fopen(PRODUCT_FILE, "r");
    if (!fpView) {
        printf("Cannot open product file or no products exist yet.\n");
        return;
    }

    Product p;
    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), fpView)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            if (strcmp(p.id, targetID) == 0) {
                found = 1;
                printf("\nProduct to be deleted:\n");
                printf("ID: %s | Name: %s | Category: %s | Supplier: %s | Qty: %d | Price: RM%.2f\n",
                       p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
                break;
            }
        }
    }
    fclose(fpView);
    
    if (!found) {
        printf("Product with ID '%s' not found.\n", targetID);
        return;
    }
    
    // Confirmation
    char confirm[10];
    getStringInput(confirm, sizeof(confirm), "\nAre you sure you want to delete this product? (y/N): ");
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("Delete operation cancelled.\n");
        return;
    }

    FILE *fp = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            if (strcmp(p.id, targetID) != 0) {
                fprintf(temp, "%s", line);
            } else {
                found = 1;
            }
        }
    }

    fclose(fp);
    fclose(temp);
    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);

    if (found) {
        printf("Product deleted successfully!\n");
    } else {
        printf("Product not found.\n");
    }
}

// displayProductMenu with box design to match your style
void displayProductMenu(void) {
    printf("\n========================================\n");
    printf("        PRODUCT MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. Add Product                   |\n");
    printf("| 2. View All Products             |\n");
    printf("| 3. View Product Details          |\n");
    printf("| 4. Update Product                |\n");
    printf("| 5. Delete Product                |\n");
    printf("| 0. Back to Main Menu             |\n");
    printf("========================================\n");
}

// Fixed handleProductMenuChoice function - remove duplicate messages
void handleProductMenuChoice(int choice) {
    switch (choice) {
        case 1:
            addProduct();  // Already prints its own success/error messages
            pause();
            break;
            
        case 2:
            viewProducts();
            pause();
            break;
            
        case 3:
            viewProduct();
            pause();
            break;
            
        case 4:
            updateProduct();  // Already prints its own success/error messages
            pause();
            break;
            
        case 5:
            deleteProduct();  // Already prints its own success/error messages
            pause();
            break;
            
        case 0:
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 5.\n");
            pause();
            break;
    }
}

// Fixed productManagementMenu function - add input buffer clearing
void productManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displayProductMenu();
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();
        
        handleProductMenuChoice(choice);
        
    } while (choice != 0);
}