#ifndef SUPPLIER_H
#define SUPPLIER_H

#define SUPPLIER_FILE "data/suppliers.txt"
#define PRODUCT_FILE "data/products.txt"

typedef struct {
    int id;
    char name[50];
    char contact[100];
} Supplier;

typedef struct {
    int productID;
    char name[50];
    int categoryID;
    int supplierID;
    float price;
    int quantity;
} Product;

void addSupplier();
void viewSuppliers();
void updateSupplier();
void deleteSupplier();
void filterProductsBySupplier(int);

#endif
