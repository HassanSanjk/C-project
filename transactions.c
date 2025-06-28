#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

typedef struct {
    char id[10];
    char name[50];
    char category[30];
    char supplierID[10];
    int quantity;
    float price;
} Product;

typedef struct {
    char transactionID[15];
    char productID[10];
    int quantity;
    char date[20]; // YYYY-MM-DD
    float totalPrice;
} Transaction;

int isValidTransactionID(const char *id) {
    if (strlen(id) != 6 || id[0] != 'T') return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

int transactionExists(const char *transactionID) {
    FILE *fp = fopen("transactions.txt", "r");
    if (!fp) return 0;
    char line[256], id[20];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]", id);
        if (strcmp(id, transactionID) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addTransaction() {
    Transaction t;
    Product p;

    printf("Enter Transaction ID (e.g., T00001): ");
    scanf(" %s", t.transactionID);
    if (!isValidTransactionID(t.transactionID)) {
        printf("Invalid Transaction ID format.\n");
        pause();
        return;
    }
    if (transactionExists(t.transactionID)) {
        printf("Transaction ID already exists.\n");
        pause();
        return;
    }

    printf("Enter Product ID: ");
    scanf(" %s", t.productID);

    FILE *fp = fopen("products.txt", "r");
    int found = 0;
    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f\n", p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) != EOF) {
        if (strcmp(p.id, t.productID) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Product not found.\n");
        pause();
        return;
    }

    printf("Enter Quantity Sold: ");
    if (scanf("%d", &t.quantity) != 1 || t.quantity <= 0) {
        printf("Quantity must be a positive number.\n");
        pause();
        return;
    }

    if (t.quantity > p.quantity) {
        printf("Not enough stock. Available: %d\n", p.quantity);
        pause();
        return;
    }

    printf("Enter Date (YYYY-MM-DD): ");
    scanf(" %s", t.date);
    if (!isValidDate(t.date)) {
        printf("Invalid date format.\n");
        pause();
        return;
    }

    t.totalPrice = t.quantity * p.price;

    fp = fopen("transactions.txt", "a");
    fprintf(fp, "%s|%s|%d|%s|%.2f\n", t.transactionID, t.productID, t.quantity, t.date, t.totalPrice);
    fclose(fp);

    FILE *orig = fopen("products.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(orig, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f\n", p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) != EOF) {
        if (strcmp(p.id, t.productID) == 0) {
            p.quantity -= t.quantity;
        }
        fprintf(temp, "%s|%s|%s|%s|%d|%.2f\n", p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
    }
    fclose(orig);
    fclose(temp);
    remove("products.txt");
    rename("temp.txt", "products.txt");

    fp = fopen("inventory.txt", "a");
    fprintf(fp, "%s|-%d|%s|OUT\n", t.productID, t.quantity, t.date);
    fclose(fp);

    printf("Transaction recorded and stock updated.\n");
    pause();
}

void viewTransactions() {
    FILE *fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("Cannot open transactions file.\n");
        pause();
        return;
    }

    Transaction t;
    char line[256];
    printf("\n--- Transaction Records ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%f", t.transactionID, t.productID, &t.quantity, t.date, &t.totalPrice);
        printf("ID: %s | Product: %s | Qty: %d | Date: %s | Total: RM%.2f\n",
               t.transactionID, t.productID, t.quantity, t.date, t.totalPrice);
    }
    fclose(fp);
    pause();
}

void updateTransaction() {
    char targetID[15];
    printf("Enter Transaction ID to update: ");
    scanf(" %s", targetID);

    FILE *fp = fopen("transactions.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    Transaction t;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%f", t.transactionID, t.productID, &t.quantity, t.date, &t.totalPrice);
        if (strcmp(t.transactionID, targetID) == 0) {
            found = 1;
            printf("Updating transaction %s\n", t.transactionID);

            printf("New Quantity: ");
            if (scanf("%d", &t.quantity) != 1 || t.quantity <= 0) {
                printf("Quantity must be a positive number.\n");
                fclose(fp); fclose(temp); remove("temp.txt"); pause(); return;
            }

            printf("New Date (YYYY-MM-DD): ");
            scanf(" %s", t.date);
            if (!isValidDate(t.date)) {
                printf("Invalid date format.\n");
                fclose(fp); fclose(temp); remove("temp.txt"); pause(); return;
            }

            FILE *pf = fopen("products.txt", "r");
            Product p;
            while (fscanf(pf, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f\n", p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) != EOF) {
                if (strcmp(p.id, t.productID) == 0) break;
            }
            fclose(pf);
            t.totalPrice = t.quantity * p.price;
        }
        fprintf(temp, "%s|%s|%d|%s|%.2f\n", t.transactionID, t.productID, t.quantity, t.date, t.totalPrice);
    }

    fclose(fp);
    fclose(temp);
    remove("transactions.txt");
    rename("temp.txt", "transactions.txt");

    if (found)
        printf("Transaction updated.\n");
    else
        printf("Transaction not found.\n");
    pause();
}

void deleteTransaction() {
    char targetID[15];
    printf("Enter Transaction ID to delete: ");
    scanf("%s", targetID);

    FILE *fp = fopen("transactions.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    Transaction t;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%f", t.transactionID, t.productID, &t.quantity, t.date, &t.totalPrice);
        if (strcmp(t.transactionID, targetID) != 0) {
            fprintf(temp, "%s", line);
        } else {
            found = 1;

            // Restore quantity to product stock
            Product p;
            FILE *orig = fopen("products.txt", "r");
            FILE *prodTemp = fopen("prod_temp.txt", "w");

            while (fscanf(orig, "%[^|]|%[^|]|%[^|]|%d|%f\n", p.id, p.name, p.category, &p.quantity, &p.price) != EOF) {
                if (strcmp(p.id, t.productID) == 0) {
                    p.quantity += t.quantity; // add back the sold quantity
                }
                fprintf(prodTemp, "%s|%s|%s|%d|%.2f\n", p.id, p.name, p.category, p.quantity, p.price);
            }

            fclose(orig);
            fclose(prodTemp);
            remove("products.txt");
            rename("prod_temp.txt", "products.txt");

            // Optionally log the reversal in inventory
            FILE *inv = fopen("inventory.txt", "a");
            fprintf(inv, "%s|%d|%s|IN\n", t.productID, t.quantity, t.date);
            fclose(inv);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("transactions.txt");
    rename("temp.txt", "transactions.txt");

    if (found)
        printf("Transaction deleted and stock restored.\n");
    else
        printf("Transaction not found.\n");
    pause();
}


void transactionMenu() {
    int choice;
    do {
        printf("\n--- Transaction Menu ---\n");
        printf("1. Add Transaction\n");
        printf("2. View Transactions\n");
        printf("3. Update Transaction\n");
        printf("4. Delete Transaction\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: viewTransactions(); break;
            case 3: updateTransaction(); break;
            case 4: deleteTransaction(); break;
            case 0: printf("Exiting Transaction Menu.\n"); break;
            default: printf("Invalid option. Please choose 0–4.\n"); pause(); break;
        }
    } while (choice != 0);
}
