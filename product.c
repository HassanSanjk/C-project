#include "product.h"

// Pause function - taken from your friend's code
void pause() {
    printf("Press Enter to continue...");
    while (getchar() != '\n'); // Flush leftover input
    getchar(); // Wait for real Enter
}

// Check if an ID exists in a file (generic) - from your friend's code
int existsInFile(const char *filename, const char *targetID) {
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
int isValidProductID(const char *id) {
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

// Add Product - Enhanced version of your friend's code
void addProduct() {
    Product p;
    char idInput[10];

    printf("\n=== ADD NEW PRODUCT ===\n");
    
    // Product ID validation
    printf("Enter Product ID (e.g., P00001): ");
    scanf("%s", idInput);
    
    if (!isValidProductID(idInput)) {
        printf("Invalid Product ID format. Must start with 'P' followed by 5 digits.\n");
        pause();
        return;
    }
    
    if (existsInFile(PRODUCT_FILE, idInput)) {
        printf("Product ID already exists.\n");
        pause();
        return;
    }
    strcpy(p.id, idInput);

    // Product name
    printf("Enter Product Name: ");
    scanf(" %[^\n]", p.name);
    if (strlen(p.name) == 0) {
        printf("Product name cannot be empty.\n");
        pause();
        return;
    }

    // Category ID validation
    printf("Enter Category ID (e.g., C00001): ");
    scanf("%s", p.category);
    
    if (!isValidCategoryID(p.category)) {
        printf("Invalid Category ID format. Must start with 'C' followed by 5 digits.\n");
        pause();
        return;
    }
    
    if (!existsInFile(CATEGORY_FILE, p.category)) {
        printf("Category ID does not exist. Please add the category first.\n");
        pause();
        return;
    }

    // Supplier ID validation
    printf("Enter Supplier ID (e.g., S00001): ");
    scanf("%s", p.supplierID);
    
    if (!isValidSupplierID(p.supplierID)) {
        printf("Invalid Supplier ID format. Must start with 'S' followed by 5 digits.\n");
        pause();
        return;
    }
    
    if (!existsInFile(SUPPLIER_FILE, p.supplierID)) {
        printf("Supplier ID does not exist. Please add the supplier first.\n");
        pause();
        return;
    }

    // Quantity validation
    printf("Enter Quantity: ");
    if (scanf("%d", &p.quantity) != 1 || p.quantity <= 0) {
        printf("Invalid quantity. Must be a positive number.\n");
        pause();
        return;
    }

    // Price validation
    printf("Enter Price (RM): ");
    if (scanf("%f", &p.price) != 1 || p.price <= 0) {
        printf("Invalid price. Must be a positive number.\n");
        pause();
        return;
    }

    // Save to file
    FILE *fp = fopen(PRODUCT_FILE, "a");
    if (!fp) {
        printf("Error opening product file.\n");
        pause();
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%d|%.2f\n", p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
    fclose(fp);

    printf("\nProduct added successfully!\n");
    printf("Product ID: %s\n", p.id);
    printf("Product Name: %s\n", p.name);
    pause();
}

// View All Products - Enhanced version
void viewProducts() {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp) {
        printf("Cannot open product file or no products exist yet.\n");
        pause();
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
    printf("Total Products: %d\n", count);
    fclose(fp);
    pause();
}

// View Single Product
void viewProduct() {
    char targetID[10];
    printf("\n=== VIEW PRODUCT DETAILS ===\n");
    printf("Enter Product ID: ");
    scanf("%s", targetID);

    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp) {
        printf("Cannot open product file.\n");
        pause();
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
        
        // Low stock warning
        if (p.quantity <= 10) {
            printf("\n*** LOW STOCK WARNING! ***\n");
        }
    } else {
        printf("Product with ID %s not found.\n", targetID);
    }
    
    pause();
}

// Update Product - Enhanced version of your friend's code
void updateProduct() {
    char targetID[10];
    printf("\n=== UPDATE PRODUCT ===\n");
    printf("Enter Product ID to update: ");
    scanf("%s", targetID);

    FILE *fp = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
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
                
                printf("\nEnter new details:\n");
                
                // Update name
                printf("New Product Name (current: %s): ", p.name);
                scanf(" %[^\n]", p.name);
                if (strlen(p.name) == 0) {
                    printf("Product name cannot be empty.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }

                // Update category
                printf("New Category ID (current: %s): ", p.category);
                scanf("%s", p.category);
                if (!isValidCategoryID(p.category)) {
                    printf("Invalid Category ID format.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }
                if (!existsInFile(CATEGORY_FILE, p.category)) {
                    printf("Category ID not found.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }

                // Update supplier
                printf("New Supplier ID (current: %s): ", p.supplierID);
                scanf("%s", p.supplierID);
                if (!isValidSupplierID(p.supplierID)) {
                    printf("Invalid Supplier ID format.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }
                if (!existsInFile(SUPPLIER_FILE, p.supplierID)) {
                    printf("Supplier ID not found.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }

                // Update quantity
                printf("New Quantity (current: %d): ", p.quantity);
                if (scanf("%d", &p.quantity) != 1 || p.quantity < 0) {
                    printf("Invalid quantity. Must be a non-negative number.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }

                // Update price
                printf("New Price (current: RM%.2f): RM", p.price);
                if (scanf("%f", &p.price) != 1 || p.price <= 0) {
                    printf("Invalid price. Must be a positive number.\n");
                    fclose(fp); fclose(temp); remove("temp.txt");
                    pause();
                    return;
                }
            }
        }
        fprintf(temp, "%s|%s|%s|%s|%d|%.2f\n", p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
    }

    fclose(fp);
    fclose(temp);
    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);

    if (found) {
        printf("\nProduct updated successfully!\n");
    } else {
        printf("Product with ID %s not found.\n", targetID);
    }

    pause();
}

// Delete Product - Your friend's code with enhancements
void deleteProduct() {
    char targetID[10];
    printf("\n=== DELETE PRODUCT ===\n");
    printf("Enter Product ID to delete: ");
    scanf("%s", targetID);

    // First, show the product to be deleted
    FILE *fpView = fopen(PRODUCT_FILE, "r");
    if (fpView) {
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
            printf("Product with ID %s not found.\n", targetID);
            pause();
            return;
        }
        
        // Confirmation
        char confirm;
        printf("\nAre you sure you want to delete this product? (y/N): ");
        scanf(" %c", &confirm);
        if (confirm != 'y' && confirm != 'Y') {
            printf("Delete operation cancelled.\n");
            pause();
            return;
        }
    }

    FILE *fp = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    Product p;
    char line[256];
    int found = 0;
    
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

    pause();
}