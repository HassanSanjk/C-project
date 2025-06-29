#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "product.h"

// Transaction structure
typedef struct {
    char transactionID[15];
    char productID[10];
    int quantity;
    char date[20];     // Format: YYYY-MM-DD
    float totalPrice;
} Transaction;

// Transaction operations
void addTransaction(void);
void viewTransactions(void);
void updateTransaction(void);
void deleteTransaction(void);

// Transaction menu handling
void displayTransactionMenu(void);
void handleTransactionMenuChoice(int choice);
void transactionManagementMenu(void);

// Utilities
int isValidTransactionID(const char *id);
int transactionExists(const char *transactionID);

#endif // TRANSACTIONS_H
