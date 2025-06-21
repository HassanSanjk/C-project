#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Symbolic Constants */
#define MAX_PRODUCT_NAME 50
#define MAX_DESCRIPTION 100
#define MAX_SUPPLIER_NAME 50
#define PRODUCT_FILE "products.txt"
#define MAX_PRODUCTS 1000
#define MIN_PRICE 0.01
#define MAX_PRICE 99999.99

/* Product Status Enumeration */
typedef enum {
    ACTIVE = 1,
    DISCONTINUED = 0
} ProductStatus;

/* Product Structure */
typedef struct {
    int productID;
    char productName[MAX_PRODUCT_NAME];
    char description[MAX_DESCRIPTION];
    double price;
    int categoryID;
    int supplierID;
    int stockQuantity;
    int minStockLevel;
    ProductStatus status;
    char dateAdded[11]; // Format: YYYY-MM-DD
} Product;

/* Function Prototypes */
void addProduct(void);
void updateProduct(void);
void deleteProduct(void);
void viewProduct(void);
void viewAllProducts(void);
int validateProductData(Product *product);
int generateProductID(void);
int findProductByID(int productID, Product *product);
void displayProductHeader(void);
void displayProduct(const Product *product);
int saveProductToFile(const Product *product);
int loadProductsFromFile(void);
void getCurrentDate(char *date);
void clearInputBuffer(void);
int getValidInteger(const char *prompt, int min, int max);
double getValidDouble(const char *prompt, double min, double max);
void getValidString(const char *prompt, char *str, int maxLen);

#endif /* PRODUCT_H */