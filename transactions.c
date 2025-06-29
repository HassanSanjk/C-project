#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "transactions.h"

/* Function to display transaction management menu */
void displayTransactionMenu(void) {
    printf("\n========================================\n");
    printf("      TRANSACTION MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. Add Transaction                   |\n");
    printf("| 2. View All Transactions             |\n");
    printf("| 3. Update Transaction                |\n");
    printf("| 4. Delete Transaction                |\n");
    printf("| 0. Back to Main Menu                 |\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

/* Function to handle transaction menu choices */
void handleTransactionMenuChoice(int choice) {
    switch (choice) {
        case 1:
            printf("\n=== ADD TRANSACTION ===\n");
            addTransaction();
            break;
            
        case 2:
            printf("\n=== ALL TRANSACTIONS ===\n");
            viewTransactions();
            break;
            
        case 3:
            printf("\n=== UPDATE TRANSACTION ===\n");
            updateTransaction();
            break;
            
        case 4:
            printf("\n=== DELETE TRANSACTION ===\n");
            deleteTransaction();
            break;
            
        case 0:
            /* Back to main menu - handled in loop */
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 4.\n");
            pause();
            break;
    }
}

/* Main transactions management menu function */
void transactionManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displayTransactionMenu();
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();
        
        handleTransactionMenuChoice(choice);
        
    } while (choice != 0);
}

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
        if (sscanf(line, "%[^|]", id) == 1) {
            if (strcmp(id, transactionID) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void addTransaction() {
    Transaction t;
    Product p;

    printf("Enter Transaction ID (e.g., T00001): ");
    if (scanf("%19s", t.transactionID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }
    
    if (!isValidTransactionID(t.transactionID)) {
        printf("Invalid Transaction ID format. Use T followed by 5 digits.\n");
        pause();
        return;
    }
    
    if (transactionExists(t.transactionID)) {
        printf("Transaction ID already exists.\n");
        pause();
        return;
    }

    printf("Enter Product ID: ");
    if (scanf("%19s", t.productID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }

    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Cannot open products file.\n");
        pause();
        return;
    }
    
    int found = 0;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
            if (strcmp(p.id, t.productID) == 0) {
                found = 1;
                break;
            }
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
        clearInputBuffer();
        pause();
        return;
    }

    if (t.quantity > p.quantity) {
        printf("Not enough stock. Available: %d\n", p.quantity);
        pause();
        return;
    }

    printf("Enter Date (YYYY-MM-DD): ");
    if (scanf("%19s", t.date) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }
    
    if (!isValidDate(t.date)) {
        printf("Invalid date format. Use YYYY-MM-DD.\n");
        pause();
        return;
    }

    t.totalPrice = t.quantity * p.price;

    // Save transaction
    fp = fopen("transactions.txt", "a");
    if (!fp) {
        printf("Cannot open transactions file for writing.\n");
        pause();
        return;
    }
    fprintf(fp, "%s|%s|%d|%s|%.2f\n", t.transactionID, t.productID, t.quantity, t.date, t.totalPrice);
    fclose(fp);

    // Update product stock
    FILE *orig = fopen("products.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!orig || !temp) {
        printf("Error updating product stock.\n");
        if (orig) fclose(orig);
        if (temp) fclose(temp);
        pause();
        return;
    }

    while (fgets(line, sizeof(line), orig)) {
        Product tempProd;
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                   tempProd.id, tempProd.name, tempProd.category, tempProd.supplierID, 
                   &tempProd.quantity, &tempProd.price) == 6) {
            if (strcmp(tempProd.id, t.productID) == 0) {
                tempProd.quantity -= t.quantity;
            }
            fprintf(temp, "%s|%s|%s|%s|%d|%.2f\n", tempProd.id, tempProd.name, 
                    tempProd.category, tempProd.supplierID, tempProd.quantity, tempProd.price);
        }
    }
    fclose(orig);
    fclose(temp);
    
    if (remove("products.txt") != 0 || rename("temp.txt", "products.txt") != 0) {
        printf("Warning: Error updating product file.\n");
    }

    // Log inventory movement
    fp = fopen("inventory.txt", "a");
    if (fp) {
        fprintf(fp, "%s|-%d|%s|OUT\n", t.productID, t.quantity, t.date);
        fclose(fp);
    }

    printf("Transaction recorded successfully and stock updated.\n");
    pause();
}

void viewTransactions() {
    FILE *fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("No transactions found or cannot open file.\n");
        pause();
        return;
    }

    Transaction t;
    char line[256];
    int count = 0;
    
    printf("\n%-10s %-12s %-8s %-12s %-10s\n", "Trans ID", "Product ID", "Quantity", "Date", "Total (RM)");
    printf("================================================================\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%f", 
                   t.transactionID, t.productID, &t.quantity, t.date, &t.totalPrice) == 5) {
            printf("%-10s %-12s %-8d %-12s %-10.2f\n",
                   t.transactionID, t.productID, t.quantity, t.date, t.totalPrice);
            count++;
        }
    }
    fclose(fp);
    
    if (count == 0) {
        printf("No transactions found.\n");
    } else {
        printf("================================================================\n");
        printf("Total transactions: %d\n", count);
    }
    pause();
}

void updateTransaction() {
    char targetID[20];
    printf("Enter Transaction ID to update: ");
    if (scanf("%19s", targetID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }

    FILE *fp = fopen("transactions.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        pause();
        return;
    }

    Transaction t;
    char line[256];
    int found = 0;
    int oldQuantity = 0; // Store original quantity

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%f", 
                   t.transactionID, t.productID, &t.quantity, t.date, &t.totalPrice) == 5) {
            if (strcmp(t.transactionID, targetID) == 0) {
                found = 1;
                oldQuantity = t.quantity; // Save original quantity
                
                printf("Current transaction: %s | Product: %s | Qty: %d | Date: %s\n",
                       t.transactionID, t.productID, t.quantity, t.date);

                printf("Enter new quantity: ");
                int newQty;
                if (scanf("%d", &newQty) != 1 || newQty <= 0) {
                    printf("Invalid quantity.\n");
                    fclose(fp); fclose(temp); remove("temp.txt"); 
                    clearInputBuffer(); pause(); return;
                }

                // Check if new quantity is available in stock
                FILE *pf = fopen("products.txt", "r");
                Product p;
                float price = 0.0;
                int currentStock = 0;
                char prodLine[512];
                
                if (pf) {
                    while (fgets(prodLine, sizeof(prodLine), pf)) {
                        if (sscanf(prodLine, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
                            if (strcmp(p.id, t.productID) == 0) {
                                price = p.price;
                                currentStock = p.quantity;
                                break;
                            }
                        }
                    }
                    fclose(pf);
                }

                // Calculate available stock (current stock + old transaction quantity)
                int availableStock = currentStock + oldQuantity;
                if (newQty > availableStock) {
                    printf("Not enough stock. Available: %d (current: %d + returned: %d)\n", 
                           availableStock, currentStock, oldQuantity);
                    fclose(fp); fclose(temp); remove("temp.txt"); 
                    pause(); return;
                }

                printf("Enter new date (YYYY-MM-DD): ");
                char newDate[20];
                if (scanf("%19s", newDate) != 1 || !isValidDate(newDate)) {
                    printf("Invalid date format.\n");
                    fclose(fp); fclose(temp); remove("temp.txt"); 
                    pause(); return;
                }

                t.quantity = newQty;
                strcpy(t.date, newDate);
                t.totalPrice = t.quantity * price;

                // Update product stock - restore old quantity, then subtract new quantity
                FILE *orig = fopen("products.txt", "r");
                FILE *prodTemp = fopen("prod_temp.txt", "w");

                if (orig && prodTemp) {
                    char stockLine[512];
                    while (fgets(stockLine, sizeof(stockLine), orig)) {
                        Product tempProd;
                        if (sscanf(stockLine, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                                   tempProd.id, tempProd.name, tempProd.category, 
                                   tempProd.supplierID, &tempProd.quantity, &tempProd.price) == 6) {
                            if (strcmp(tempProd.id, t.productID) == 0) {
                                // Restore old quantity and subtract new quantity
                                tempProd.quantity = tempProd.quantity + oldQuantity - newQty;
                            }
                            fprintf(prodTemp, "%s|%s|%s|%s|%d|%.2f\n", 
                                    tempProd.id, tempProd.name, tempProd.category,
                                    tempProd.supplierID, tempProd.quantity, tempProd.price);
                        }
                    }
                    fclose(orig);
                    fclose(prodTemp);
                    
                    if (remove("products.txt") != 0 || rename("prod_temp.txt", "products.txt") != 0) {
                        printf("Warning: Error updating product stock.\n");
                    }

                    // Log inventory movement
                    FILE *inv = fopen("inventory.txt", "a");
                    if (inv) {
                        int quantityDiff = newQty - oldQuantity;
                        if (quantityDiff != 0) {
                            fprintf(inv, "%s|%+d|%s|UPDATED\n", t.productID, -quantityDiff, newDate);
                        }
                        fclose(inv);
                    }
                } else {
                    if (orig) fclose(orig);
                    if (prodTemp) fclose(prodTemp);
                    printf("Warning: Could not update product stock.\n");
                }
            }
            fprintf(temp, "%s|%s|%d|%s|%.2f\n", t.transactionID, t.productID, t.quantity, t.date, t.totalPrice);
        }
    }

    fclose(fp);
    fclose(temp);
    
    if (remove("transactions.txt") != 0 || rename("temp.txt", "transactions.txt") != 0) {
        printf("Error updating transaction file.\n");
    } else if (found) {
        printf("Transaction updated successfully and stock adjusted.\n");
    } else {
        printf("Transaction not found.\n");
    }
    pause();
}

void deleteTransaction() {
    char targetID[20];
    printf("Enter Transaction ID to delete: ");
    if (scanf("%19s", targetID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }

    FILE *fp = fopen("transactions.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        pause();
        return;
    }

    Transaction t;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%f", 
                   t.transactionID, t.productID, &t.quantity, t.date, &t.totalPrice) == 5) {
            if (strcmp(t.transactionID, targetID) != 0) {
                fprintf(temp, "%s", line);
            } else {
                found = 1;
                printf("Deleting transaction: %s (Product: %s, Qty: %d)\n", 
                       t.transactionID, t.productID, t.quantity);

                // Restore quantity to product stock
                FILE *orig = fopen("products.txt", "r");
                FILE *prodTemp = fopen("prod_temp.txt", "w");

                if (orig && prodTemp) {
                    Product p;
                    char prodLine[512];
                    while (fgets(prodLine, sizeof(prodLine), orig)) {
                        if (sscanf(prodLine, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", 
                                   p.id, p.name, p.category, p.supplierID, &p.quantity, &p.price) == 6) {
                            if (strcmp(p.id, t.productID) == 0) {
                                p.quantity += t.quantity; // Restore stock
                            }
                            fprintf(prodTemp, "%s|%s|%s|%s|%d|%.2f\n", 
                                    p.id, p.name, p.category, p.supplierID, p.quantity, p.price);
                        }
                    }
                    fclose(orig);
                    fclose(prodTemp);
                    remove("products.txt");
                    rename("prod_temp.txt", "products.txt");

                    // Log the stock restoration
                    FILE *inv = fopen("inventory.txt", "a");
                    if (inv) {
                        fprintf(inv, "%s|+%d|%s|RESTORED\n", t.productID, t.quantity, t.date);
                        fclose(inv);
                    }
                } else {
                    if (orig) fclose(orig);
                    if (prodTemp) fclose(prodTemp);
                    printf("Warning: Could not restore product stock.\n");
                }
            }
        }
    }

    fclose(fp);
    fclose(temp);
    
    if (remove("transactions.txt") != 0 || rename("temp.txt", "transactions.txt") != 0) {
        printf("Error updating transaction file.\n");
    } else if (found) {
        printf("Transaction deleted successfully and stock restored.\n");
    } else {
        printf("Transaction not found.\n");
    }
    pause();
}