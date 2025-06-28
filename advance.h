#ifndef ADVANCE_H
#define ADVANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* File paths for data files */
#define PRODUCTS_FILE "products.txt"
#define TRANSACTIONS_FILE "transactions.txt"
#define USERS_FILE "users.txt"
#define SUPPLIERS_FILE "suppliers.txt"

/* Low stock threshold */
#define LOW_STOCK_THRESHOLD 5

/* Function prototypes for advanced features */

/**
 * Displays a pause message and waits for user input
 */
void pause(void);

/**
 * Displays products with quantity below the low stock threshold
 */
void lowStockAlert(void);

/**
 * Searches for products by name using keyword matching
 */
void searchProductByName(void);

/**
 * Displays the total count of transactions in the system
 */
void transactionSummary(void);

/**
 * Finds and displays user contact information by User ID
 */
void findUserContact(void);

/**
 * Finds and displays supplier email by Supplier ID
 */
void supplierEmailFinder(void);

/**
 * Displays the advanced features menu
 */
void displayAdvancedMenu(void);

/**
 * Handles menu choices for advanced features
 * @param choice - The menu option selected by user
 */
void handleAdvancedMenuChoice(int choice);

/**
 * Main function for advanced features module
 * Can be called from main menu system
 */
void advancedFeaturesMenu(void);

#endif /* ADVANCE_H */