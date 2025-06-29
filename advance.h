#ifndef ADVANCE_H
#define ADVANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Data file paths
#define PRODUCTS_FILE "products.txt"
#define TRANSACTIONS_FILE "transactions.txt"
#define USERS_FILE "users.txt"
#define SUPPLIERS_FILE "suppliers.txt"

// Threshold for low stock alert
#define LOW_STOCK_THRESHOLD 5

// Advanced features function prototypes
void pause(void);
void lowStockAlert(void);
void searchProductByName(void);
void transactionSummary(void);
void findUserContact(void);
void supplierEmailFinder(void);
void displayAdvancedMenu(void);
void advancedFeaturesMenu(void);
void handleAdvancedMenuChoice(int choice);

#endif // ADVANCE_H
