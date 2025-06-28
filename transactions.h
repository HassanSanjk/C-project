#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "product.h"

typedef struct {
    char transactionID[15];
    char productID[10];
    int quantity;
    char date[20]; // YYYY-MM-DD
    float totalPrice;
} Transaction;

// Function declarations
void pause(void);
int isValidTransactionID(const char *id);
int isValidDate(const char *date);
int transactionExists(const char *transactionID);
void addTransaction(void);
void viewTransactions(void);
void updateTransaction(void);
void deleteTransaction(void);
void transactionMenu(void);

#endif // TRANSACTIONS_H