#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inventory.h"
#include "product.h"
#include "utils.h"



/* Function to display inventory management menu */
void displayInventoryMenu(void) {
    printf("\n--- Inventory Menu ---\n");
    printf("1. Add Inventory Item\n");
    printf("2. View All Inventory\n");
    printf("3. Update Inventory Item\n");
    printf("4. Remove Inventory Item\n");
    printf("5. View Low Stock Items\n");
    printf("6. Generate Inventory Report\n");
    printf("0. Back to Main Menu\n");
}

void handleInventoryMenuChoice(int choice) {
    static InventoryNode *inventoryList = NULL;
    
    // Load inventory on first access
    if (inventoryList == NULL && choice > 0 && choice < 7) {
        inventoryList = loadInventoryFromFile();
    }
    
    switch (choice) {
        case 1:
            printf("\n=== ADD INVENTORY ITEM ===\n");
            addInventoryItem(&inventoryList);
            saveInventoryToFile(inventoryList);
            printf("Inventory item added successfully!\n");
            pause();
            break;
            
        case 2:
            printf("\n=== ALL INVENTORY ITEMS ===\n");
            viewInventory(inventoryList);
            pause();
            break;
            
        case 3:
            printf("\n=== UPDATE INVENTORY ITEM ===\n");
            updateInventoryItem(inventoryList);
            saveInventoryToFile(inventoryList);
            printf("Inventory item updated successfully!\n");
            pause();
            break;
            
        case 4:
            printf("\n=== REMOVE INVENTORY ITEM ===\n");
            removeInventoryItem(&inventoryList);
            saveInventoryToFile(inventoryList);
            printf("Inventory item removed successfully!\n");
            pause();
            break;
            
        case 5:
            printf("\n=== LOW STOCK ITEMS ===\n");
            showLowStockOnly(inventoryList);
            pause();
            break;
            
        case 6:
            printf("\n=== INVENTORY REPORT ===\n");
            generateInventoryReport(inventoryList);
            pause();
            break;
            
        case 0:
            /* Save and free memory before returning to main menu */
            if (inventoryList != NULL) {
                saveInventoryToFile(inventoryList);
                freeInventoryList(inventoryList);
                inventoryList = NULL;
            }
            break;
            
        default:
            printf("Invalid option. Please choose a number between 0 and 6.\n");
            pause();
            break;
    }
}

/* Main inventory management menu function */
void inventoryManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displayInventoryMenu();
        printf("Select: ");
        scanf("%d", &choice);
        handleInventoryMenuChoice(choice);
        
    } while (choice != 0);
}


// Inventory Struct
typedef struct {
    char productID[10];
    int stockChange;
    char date[20];
    char action[4]; // IN or OUT
} InventoryRecord;


// Add Inventory Record
void addInventoryRecord() {
    InventoryRecord r;

    printf("Enter Product ID (e.g., P00001): ");
    scanf("%s", r.productID);
    if (!isValidProductID(r.productID)) {
        printf("Invalid Product ID format.\n");
        pause();
        return;
    }
    if (!existsInFile("products.txt", r.productID)) {
        printf("Product ID not found.\n");
        pause();
        return;
    }

    printf("Enter Stock Change (number only): ");
    if (scanf("%d", &r.stockChange) != 1) {
        printf("Invalid stock change input.\n");
        pause();
        return;
    }

    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", r.date);
    if (!isValidDate(r.date)) {
        printf("Invalid date format or year too early. Must be YYYY-MM-DD and year must be at least 2025.\n");
        pause();
        return;
    }

    printf("Enter Action (IN/OUT): ");
    scanf("%s", r.action);
    if (strcmp(r.action, "IN") != 0 && strcmp(r.action, "OUT") != 0) {
        printf("Action must be 'IN' or 'OUT' only.\n");
        pause();
        return;
    }

    FILE *fp = fopen("inventory.txt", "a");
    if (!fp) {
        printf("Cannot open inventory file.\n");
        pause();
        return;
    }
    fprintf(fp, "%s|%+d|%s|%s\n", r.productID, r.stockChange, r.date, r.action);
    fclose(fp);
    printf("Inventory record added.\n");
    pause();
}

// View Records
void readInventoryRecords() {
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("Cannot open inventory file.\n");
        pause();
        return;
    }

    InventoryRecord r;
    char line[256];
    printf("\n--- Inventory Records ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action);
        printf("Product: %s | Change: %d | Date: %s | Action: %s\n",
               r.productID, r.stockChange, r.date, r.action);
    }
    fclose(fp);
    pause();
}

// Update Record
void updateInventoryRecord() {
    char targetID[10], targetDate[20];
    printf("Enter Product ID to update: ");
    scanf("%s", targetID);
    printf("Enter Date of record to update: ");
    scanf("%s", targetDate);

    FILE *fp = fopen("inventory.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    InventoryRecord r;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action);
        if (strcmp(r.productID, targetID) == 0 && strcmp(r.date, targetDate) == 0) {
            found = 1;

            printf("New Stock Change: ");
            if (scanf("%d", &r.stockChange) != 1) {
                printf("Invalid input.\n");
                fclose(fp); fclose(temp); remove("temp.txt"); pause(); return;
            }

            printf("New Action (IN/OUT): ");
            scanf("%s", r.action);
            if (strcmp(r.action, "IN") != 0 && strcmp(r.action, "OUT") != 0) {
                printf("Invalid action.\n");
                fclose(fp); fclose(temp); remove("temp.txt"); pause(); return;
            }
        }
        fprintf(temp, "%s|%+d|%s|%s\n", r.productID, r.stockChange, r.date, r.action);
    }

    fclose(fp);
    fclose(temp);
    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (found)
        printf("Inventory record updated.\n");
    else
        printf("Record not found.\n");

    pause();
}

// Delete Record
void deleteInventoryRecord() {
    char targetID[10], targetDate[20];
    printf("Enter Product ID to delete: ");
    scanf("%s", targetID);

    // ✅ Check if Product ID exists in products.txt
    if (!existsInFile("products.txt", targetID)) {
        printf("Product ID not found.\n");
        pause();
        return;
    }

    printf("Enter Date of record to delete: ");
    scanf("%s", targetDate);

    FILE *fp = fopen("inventory.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    InventoryRecord r;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%d|%[^|]|%s", r.productID, &r.stockChange, r.date, r.action);
        if (strcmp(r.productID, targetID) == 0 && strcmp(r.date, targetDate) == 0) {
            found = 1;
            continue;
        }
        fprintf(temp, "%s", line);
    }

    fclose(fp);
    fclose(temp);
    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (found)
        printf("Inventory record deleted.\n");
    else
        printf("Record not found.\n");

    pause();
}


void inventoryMenu() {
    int choice;
    do {
        printf("\n--- Inventory Menu ---\n");
        printf("1. Add Inventory Record\n");
        printf("2. View Inventory Records\n");
        printf("3. Update Inventory Record\n");
        printf("4. Delete Inventory Record\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addInventoryRecord(); break;
            case 2: readInventoryRecords(); break;
            case 3: updateInventoryRecord(); break;
            case 4: deleteInventoryRecord(); break;
            case 0: printf("Exiting Inventory Menu.\n"); break;
            default: printf("Invalid option. Please choose 0–4.\n"); pause(); break;
        }
    } while (choice != 0);
}





// Load inventory from inventory.txt
InventoryNode* loadInventoryFromFile() {
    FILE *file = fopen(INVENTORY_FILE, "r");
    if (file == NULL) {
        printf("Warning: Could not open %s. Starting with empty inventory.\n", INVENTORY_FILE);
        return NULL;
    }

    InventoryNode *head = NULL, *tail = NULL;
    Inventory temp;

    while (fscanf(file, "%s %d %d", temp.productID, &temp.quantity, &temp.lowStockThreshold) == 3) {
        InventoryNode *newNode = (InventoryNode *)malloc(sizeof(InventoryNode));
        if (!newNode) {
            printf("Memory allocation failed.\n");
            fclose(file);
            freeInventoryList(head);
            return NULL;
        }
        newNode->data = temp;
        newNode->next = NULL;

        if (head == NULL)
            head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(file);
    return head;
}

// Free memory used by linked list
void freeInventoryList(InventoryNode *head) {
    InventoryNode *current = head;
    while (current != NULL) {
        InventoryNode *temp = current;
        current = current->next;
        free(temp);
    }
}

// View all inventory items
void viewInventory(InventoryNode *head) {
    if (head == NULL) {
        printf("Inventory is empty.\n");
        return;
    }

    printf("\n%-15s %-10s %-20s\n", "Product ID", "Quantity", "Low Stock Threshold");
    printf("------------------------------------------------------\n");

    InventoryNode *current = head;
    while (current != NULL) {
        printf("%-15s %-10d %-20d\n",
               current->data.productID,
               current->data.quantity,
               current->data.lowStockThreshold);
        current = current->next;
    }
}


void saveInventoryToFile(InventoryNode *head) {
    FILE *file = fopen(INVENTORY_FILE, "w");
    if (file == NULL) {
        printf("Error: Could not open %s for writing.\n", INVENTORY_FILE);
        return;
    }

    InventoryNode *current = head;
    while (current != NULL) {
        fprintf(file, "%s %d %d\n",
                current->data.productID,
                current->data.quantity,
                current->data.lowStockThreshold);
        current = current->next;
    }

    fclose(file);
}


void addInventoryItem(InventoryNode **head) {
    Inventory newInv;
    printf("\nEnter Product ID: ");
    scanf("%s", newInv.productID);

    printf("Enter Quantity: ");
    while (scanf("%d", &newInv.quantity) != 1 || newInv.quantity < 0) {
        printf("Invalid input. Enter a non-negative quantity: ");
        while (getchar() != '\n'); // clear input buffer
    }

    printf("Enter Low Stock Threshold: ");
    while (scanf("%d", &newInv.lowStockThreshold) != 1 || newInv.lowStockThreshold < 0) {
        printf("Invalid input. Enter a non-negative threshold: ");
        while (getchar() != '\n');
    }

    // Create new node
    InventoryNode *newNode = (InventoryNode *)malloc(sizeof(InventoryNode));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->data = newInv;
    newNode->next = NULL;

    // Insert at end of list
    if (*head == NULL) {
        *head = newNode;
    } else {
        InventoryNode *temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    // Save updated list
    saveInventoryToFile(*head);
    printf("Inventory item added successfully.\n");
}

void updateInventoryItem(InventoryNode *head) {
    char productID[10];
    printf("Enter Product ID to update: ");
    scanf("%s", productID);

    InventoryNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.productID, productID) == 0) {
            printf("Current Quantity: %d\n", current->data.quantity);
            printf("Enter new Quantity: ");
            while (scanf("%d", &current->data.quantity) != 1 || current->data.quantity < 0) {
                printf("Invalid input. Enter non-negative quantity: ");
                while (getchar() != '\n');
            }

            printf("Current Threshold: %d\n", current->data.lowStockThreshold);
            printf("Enter new Low Stock Threshold: ");
            while (scanf("%d", &current->data.lowStockThreshold) != 1 || current->data.lowStockThreshold < 0) {
                printf("Invalid input. Enter non-negative threshold: ");
                while (getchar() != '\n');
            }

            saveInventoryToFile(head);
            printf("Inventory item updated successfully.\n");
            return;
        }
        current = current->next;
    }

    printf("Product ID not found.\n");
}

void removeInventoryItem(InventoryNode **head) {
    char productID[10];
    printf("Enter Product ID to remove: ");
    scanf("%s", productID);

    InventoryNode *current = *head, *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->data.productID, productID) == 0) {
            if (prev == NULL) {
                *head = current->next;  // First node
            } else {
                prev->next = current->next;
            }
            free(current);
            saveInventoryToFile(*head);
            printf("Inventory item removed.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Product ID not found.\n");
}

void checkLowStock(InventoryNode *head) {
    int found = 0;
    printf("\nLow Stock Items:\n");
    printf("%-15s %-10s %-10s\n", "Product ID", "Quantity", "Threshold");
    printf("----------------------------------------\n");

    InventoryNode *current = head;
    while (current != NULL) {
        if (current->data.quantity < current->data.lowStockThreshold) {
            found = 1;
            printf("%-15s %-10d %-10d\n",
                   current->data.productID,
                   current->data.quantity,
                   current->data.lowStockThreshold);
        }
        current = current->next;
    }

    if (!found) {
        printf("No low stock items.\n");
    }
}

void searchInventory(InventoryNode *head) {
    char productID[10];
    printf("Enter Product ID to search: ");
    scanf("%s", productID);

    InventoryNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.productID, productID) == 0) {
            printf("\nProduct Found:\n");
            printf("ID: %s\nQuantity: %d\nThreshold: %d\n",
                   current->data.productID,
                   current->data.quantity,
                   current->data.lowStockThreshold);
            return;
        }
        current = current->next;
    }

    printf("Product ID not found.\n");
}

void sortInventoryByQuantity(InventoryNode **head) {
    if (*head == NULL || (*head)->next == NULL) return;

    InventoryNode *i, *j;
    for (i = *head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->data.quantity > j->data.quantity) {
                Inventory temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }

    printf("Inventory sorted by quantity.\n");
}

void showLowStockOnly(InventoryNode *list) {
    printf("Low stock feature coming soon...\n");
}

void generateInventoryReport(InventoryNode *list) {
    printf("Report generation feature coming soon...\n");
}
