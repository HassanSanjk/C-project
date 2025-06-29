#include "product.h"

#ifndef SUPPLIER_H
#define SUPPLIER_H

#define SUPPLIER_FILE "suppliers.txt"

typedef struct {
    char id[10];
    char name[50];
    char phone[20];
    char email[50];
} Supplier;

int isValidSupplier(const char *id);
int isValidPhone(const char *phone);
int isValidEmail(const char *email);
void addSupplier();
void viewSuppliers();
void updateSupplier();
void deleteSupplier();
void filterProductsBySupplier(const char *supplierID);
void displaySupplierMenu(void);
void handleSupplierMenuChoice(int choice);
void supplierManagementMenu(void);

#endif