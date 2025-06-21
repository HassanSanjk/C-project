#include "product.h"
#include <time.h>

/* Global variables for product management */
static Product products[MAX_PRODUCTS];
static int productCount = 0;

/* Function to clear input buffer */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Function to get current date */
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(date, 11, "%Y-%m-%d", tm_info);
}

/* Function to get valid integer input */
int getValidInteger(const char *prompt, int min, int max) {
    int value;
    int result;
    
    do {
        printf("%s", prompt);
        result = scanf("%d", &value);
        
        if (result != 1) {
            printf("Error: Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        
        if (value < min || value > max) {
            printf("Error: Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        
        clearInputBuffer();
        return value;
        
    } while (1);
}

/* Function to get valid double input */
double getValidDouble(const char *prompt, double min, double max) {
    double value;
    int result;
    
    do {
        printf("%s", prompt);
        result = scanf("%lf", &value);
        
        if (result != 1) {
            printf("Error: Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        
        if (value < min || value > max) {
            printf("Error: Please enter a number between %.2f and %.2f.\n", min, max);
            continue;
        }
        
        clearInputBuffer();
        return value;
        
    } while (1);
}

/* Function to get valid string input */
void getValidString(const char *prompt, char *str, int maxLen) {
    do {
        printf("%s", prompt);
        if (fgets(str, maxLen, stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        
        /* Remove newline character if present */
        str[strcspn(str, "\n")] = '\0';
        
        if (strlen(str) == 0) {
            printf("Error: Input cannot be empty. Please try again.\n");
            continue;
        }
        
        break;
    } while (1);
}

/* Function to generate unique product ID */
int generateProductID(void) {
    int maxID = 0;
    int i;
    
    /* Load existing products to find the highest ID */
    loadProductsFromFile();
    
    for (i = 0; i < productCount; i++) {
        if (products[i].productID > maxID) {
            maxID = products[i].productID;
        }
    }
    
    return maxID + 1;
}

/* Function to find product by ID */
int findProductByID(int productID, Product *product) {
    int i;
    
    loadProductsFromFile();
    
    for (i = 0; i < productCount; i++) {
        if (products[i].productID == productID) {
            if (product != NULL) {
                *product = products[i];
            }
            return i; /* Return index */
        }
    }
    
    return -1; /* Product not found */
}

/* Function to validate product data */
int validateProductData(Product *product) {
    /* Validate product name */
    if (strlen(product->productName) == 0) {
        printf("Error: Product name cannot be empty.\n");
        return 0;
    }
    
    /* Validate price */
    if (product->price < MIN_PRICE || product->price > MAX_PRICE) {
        printf("Error: Price must be between %.2f and %.2f.\n", MIN_PRICE, MAX_PRICE);
        return 0;
    }
    
    /* Validate stock quantity */
    if (product->stockQuantity < 0) {
        printf("Error: Stock quantity cannot be negative.\n");
        return 0;
    }
    
    /* Validate minimum stock level */
    if (product->minStockLevel < 0) {
        printf("Error: Minimum stock level cannot be negative.\n");
        return 0;
    }
    
    return 1; /* Validation successful */
}

/* Function to display product header */
void displayProductHeader(void) {
    printf("\n");
    printf("================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-10s %-10s\n", 
           "ID", "Name", "Description", "Price", "Stock", "MinStock", "Status", "Date");
    printf("================================================================================\n");
}

/* Function to display a single product */
void displayProduct(const Product *product) {
    printf("%-5d %-20.19s %-15.14s $%-7.2f %-8d %-8d %-10s %-10s\n",
           product->productID,
           product->productName,
           product->description,
           product->price,
           product->stockQuantity,
           product->minStockLevel,
           (product->status == ACTIVE) ? "Active" : "Inactive",
           product->dateAdded);
}

/* Function to save product to file */
int saveProductToFile(const Product *product) {
    FILE *file;
    int found = 0;
    int i;
    
    /* Load existing products */
    loadProductsFromFile();
    
    /* Check if product already exists and update it */
    for (i = 0; i < productCount; i++) {
        if (products[i].productID == product->productID) {
            products[i] = *product;
            found = 1;
            break;
        }
    }
    
    /* If not found, add new product */
    if (!found && productCount < MAX_PRODUCTS) {
        products[productCount] = *product;
        productCount++;
    } else if (!found) {
        printf("Error: Maximum number of products reached.\n");
        return 0;
    }
    
    /* Write all products back to file */
    file = fopen(PRODUCT_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return 0;
    }
    
    for (i = 0; i < productCount; i++) {
        fprintf(file, "%d|%s|%s|%.2f|%d|%d|%d|%d|%d|%s\n",
                products[i].productID,
                products[i].productName,
                products[i].description,
                products[i].price,
                products[i].categoryID,
                products[i].supplierID,
                products[i].stockQuantity,
                products[i].minStockLevel,
                products[i].status,
                products[i].dateAdded);
    }
    
    fclose(file);
    return 1;
}

/* Function to load products from file */
int loadProductsFromFile(void) {
    FILE *file;
    char line[512];
    
    file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        /* File doesn't exist yet, create it */
        file = fopen(PRODUCT_FILE, "w");
        if (file != NULL) {
            fclose(file);
        }
        productCount = 0;
        return 1;
    }
    
    productCount = 0;
    while (fgets(line, sizeof(line), file) && productCount < MAX_PRODUCTS) {
        /* Remove newline character */
        line[strcspn(line, "\n")] = '\0';
        
        /* Parse the line */
        char *token = strtok(line, "|");
        if (token != NULL) {
            products[productCount].productID = atoi(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(products[productCount].productName, token);
            
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(products[productCount].description, token);
            
            token = strtok(NULL, "|");
            if (token != NULL) products[productCount].price = atof(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) products[productCount].categoryID = atoi(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) products[productCount].supplierID = atoi(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) products[productCount].stockQuantity = atoi(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) products[productCount].minStockLevel = atoi(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) products[productCount].status = atoi(token);
            
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(products[productCount].dateAdded, token);
            
            productCount++;
        }
    }
    
    fclose(file);
    return 1;
}

/* Function to add new product */
void addProduct(void) {
    Product newProduct;
    
    printf("\n=== ADD NEW PRODUCT ===\n");
    
    /* Generate unique product ID */
    newProduct.productID = generateProductID();
    printf("Product ID: %d (Auto-generated)\n", newProduct.productID);
    
    /* Get product information from user */
    getValidString("Enter product name: ", newProduct.productName, MAX_PRODUCT_NAME);
    getValidString("Enter product description: ", newProduct.description, MAX_DESCRIPTION);
    
    newProduct.price = getValidDouble("Enter product price: $", MIN_PRICE, MAX_PRICE);
    
    newProduct.categoryID = getValidInteger("Enter category ID: ", 1, 999);
    newProduct.supplierID = getValidInteger("Enter supplier ID: ", 1, 999);
    newProduct.stockQuantity = getValidInteger("Enter initial stock quantity: ", 0, 99999);
    newProduct.minStockLevel = getValidInteger("Enter minimum stock level: ", 0, 9999);
    
    newProduct.status = ACTIVE;
    getCurrentDate(newProduct.dateAdded);
    
    /* Validate product data */
    if (!validateProductData(&newProduct)) {
        printf("Product validation failed. Product not added.\n");
        return;
    }
    
    /* Save product to file */
    if (saveProductToFile(&newProduct)) {
        printf("\nProduct added successfully!\n");
        printf("Product ID: %d\n", newProduct.productID);
    } else {
        printf("Error: Failed to save product.\n");
    }
}

/* Function to update existing product */
void updateProduct(void) {
    int productID;
    Product product;
    int index;
    int choice;
    
    printf("\n=== UPDATE PRODUCT ===\n");
    
    productID = getValidInteger("Enter Product ID to update: ", 1, 99999);
    
    index = findProductByID(productID, &product);
    if (index == -1) {
        printf("Error: Product with ID %d not found.\n", productID);
        return;
    }
    
    printf("\nCurrent Product Information:\n");
    displayProductHeader();
    displayProduct(&product);
    
    do {
        printf("\nWhat would you like to update?\n");
        printf("1. Product Name\n");
        printf("2. Description\n");
        printf("3. Price\n");
        printf("4. Category ID\n");
        printf("5. Supplier ID\n");
        printf("6. Stock Quantity\n");
        printf("7. Minimum Stock Level\n");
        printf("8. Status\n");
        printf("9. Save and Exit\n");
        printf("0. Cancel\n");
        
        choice = getValidInteger("Enter your choice: ", 0, 9);
        
        switch (choice) {
            case 1:
                getValidString("Enter new product name: ", product.productName, MAX_PRODUCT_NAME);
                break;
            case 2:
                getValidString("Enter new description: ", product.description, MAX_DESCRIPTION);
                break;
            case 3:
                product.price = getValidDouble("Enter new price: $", MIN_PRICE, MAX_PRICE);
                break;
            case 4:
                product.categoryID = getValidInteger("Enter new category ID: ", 1, 999);
                break;
            case 5:
                product.supplierID = getValidInteger("Enter new supplier ID: ", 1, 999);
                break;
            case 6:
                product.stockQuantity = getValidInteger("Enter new stock quantity: ", 0, 99999);
                break;
            case 7:
                product.minStockLevel = getValidInteger("Enter new minimum stock level: ", 0, 9999);
                break;
            case 8:
                printf("1. Active\n");
                printf("2. Discontinued\n");
                choice = getValidInteger("Enter status: ", 1, 2);
                product.status = (choice == 1) ? ACTIVE : DISCONTINUED;
                break;
            case 9:
                if (validateProductData(&product)) {
                    if (saveProductToFile(&product)) {
                        printf("Product updated successfully!\n");
                    } else {
                        printf("Error: Failed to update product.\n");
                    }
                } else {
                    printf("Product validation failed. Changes not saved.\n");
                }
                return;
            case 0:
                printf("Update cancelled.\n");
                return;
        }
        
        if (choice >= 1 && choice <= 8) {
            printf("Field updated. Choose another field or select 9 to save.\n");
        }
        
    } while (choice != 0 && choice != 9);
}

/* Function to delete product */
void deleteProduct(void) {
    int productID;
    Product product;
    int index;
    char confirm;
    int i;
    FILE *file;
    
    printf("\n=== DELETE PRODUCT ===\n");
    
    productID = getValidInteger("Enter Product ID to delete: ", 1, 99999);
    
    index = findProductByID(productID, &product);
    if (index == -1) {
        printf("Error: Product with ID %d not found.\n", productID);
        return;
    }
    
    printf("\nProduct to be deleted:\n");
    displayProductHeader();
    displayProduct(&product);
    
    printf("\nAre you sure you want to delete this product? (y/N): ");
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        /* Remove product from array */
        for (i = index; i < productCount - 1; i++) {
            products[i] = products[i + 1];
        }
        productCount--;
        
        /* Write updated products back to file */
        file = fopen(PRODUCT_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to open file for writing.\n");
            return;
        }
        
        for (i = 0; i < productCount; i++) {
            fprintf(file, "%d|%s|%s|%.2f|%d|%d|%d|%d|%d|%s\n",
                    products[i].productID,
                    products[i].productName,
                    products[i].description,
                    products[i].price,
                    products[i].categoryID,
                    products[i].supplierID,
                    products[i].stockQuantity,
                    products[i].minStockLevel,
                    products[i].status,
                    products[i].dateAdded);
        }
        
        fclose(file);
        printf("Product deleted successfully!\n");
    } else {
        printf("Delete operation cancelled.\n");
    }
}

/* Function to view specific product */
void viewProduct(void) {
    int productID;
    Product product;
    
    printf("\n=== VIEW PRODUCT DETAILS ===\n");
    
    productID = getValidInteger("Enter Product ID: ", 1, 99999);
    
    if (findProductByID(productID, &product) == -1) {
        printf("Error: Product with ID %d not found.\n", productID);
        return;
    }
    
    printf("\n=== PRODUCT DETAILS ===\n");
    printf("Product ID: %d\n", product.productID);
    printf("Name: %s\n", product.productName);
    printf("Description: %s\n", product.description);
    printf("Price: $%.2f\n", product.price);
    printf("Category ID: %d\n", product.categoryID);
    printf("Supplier ID: %d\n", product.supplierID);
    printf("Stock Quantity: %d\n", product.stockQuantity);
    printf("Minimum Stock Level: %d\n", product.minStockLevel);
    printf("Status: %s\n", (product.status == ACTIVE) ? "Active" : "Discontinued");
    printf("Date Added: %s\n", product.dateAdded);
    
    if (product.stockQuantity <= product.minStockLevel) {
        printf("\n*** LOW STOCK ALERT! ***\n");
    }
}

/* Function to view all products */
void viewAllProducts(void) {
    int i;
    
    printf("\n=== ALL PRODUCTS ===\n");
    
    loadProductsFromFile();
    
    if (productCount == 0) {
        printf("No products found in the system.\n");
        return;
    }
    
    displayProductHeader();
    
    for (i = 0; i < productCount; i++) {
        displayProduct(&products[i]);
    }
    
    printf("================================================================================\n");
    printf("Total Products: %d\n", productCount);
}