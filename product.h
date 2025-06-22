#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Symbolic Constants */
#define MAX_PRODUCT_NAME 50
#define PRODUCT_FILE "products.txt"
#define CATEGORY_FILE "category.txt"
#define SUPPLIER_FILE "suppliers.txt"

/* Product Structure - Matching your friend's format */
typedef struct {
    char id[10];        // Format: P00001
    char name[50];      // Product name
    char category[10];  // Category ID: C00001
    char supplierID[10]; // Supplier ID: S00001
    int quantity;       // Stock quantity
    float price;        // Product price
} Product;

/* Function Prototypes */
void addProduct(void);
void updateProduct(void);
void deleteProduct(void);
void viewProduct(void);
void viewProducts(void);
void pause(void);
int existsInFile(const char *filename, const char *targetID);
int isValidProductID(const char *id);
int isValidCategoryID(const char *id);
int isValidSupplierID(const char *id);

#endif /* PRODUCT_H */