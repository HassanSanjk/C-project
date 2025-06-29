#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_ID_LENGTH 10
#define MAX_DATE_LENGTH 20
#define MAX_ACTION_LENGTH 4

// Original Inventory struct for product data
typedef struct {
    char productID[MAX_ID_LENGTH];
    int quantity;
    int lowStockThreshold;
} Inventory;

// Inventory record struct for file I/O operations
typedef struct {
    char productID[MAX_ID_LENGTH];
    int stockChange;
    char date[MAX_DATE_LENGTH];
    char action[MAX_ACTION_LENGTH];
} InventoryRecord;

// Linked list node (keeping for reference if needed)
typedef struct InventoryNode {
    Inventory data;
    struct InventoryNode *next;
} InventoryNode;

// Function declarations
void displayInventoryMenu(void);
void handleInventoryMenuChoice(int choice);
void inventoryManagementMenu(void);
int exists_in_file(const char *filename, const char *productID);
int isValidProduct(const char *productID);
void addInventoryRecord(void);
void readInventoryRecords(void);
void updateInventoryRecord(void);
void deleteInventoryRecord(void);
void showLowStockItems(void);
void generateInventoryReport(void);

#endif