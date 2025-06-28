#include "product.h"

#ifndef SUPPLIER_H
#define SUPPLIER_H

#define SUPPLIER_FILE "suppliers.txt"

typedef struct {
    int id;
    char name[50];
    char contact[100];
} Supplier;


void addSupplier();
void viewSuppliers();
void updateSupplier();
void deleteSupplier();
void filterProductsBySupplier(int);
void displaySupplierMenu(void);
void handleSupplierMenuChoice(int choice);
void supplierManagementMenu(void);

#endif
