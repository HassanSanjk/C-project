#ifndef INVENTORY_H
#define INVENTORY_H

#define INVENTORY_FILE "inventory.txt"
#define MAX_ID_LENGTH 20
#define MAX_NAME_LENGTH 50

typedef struct {
    char productID[MAX_ID_LENGTH];
    int quantity;
    int lowStockThreshold;
} Inventory;

typedef struct InventoryNode {
    Inventory data;
    struct InventoryNode *next;
} InventoryNode;

// Function prototypes
InventoryNode* loadInventoryFromFile();
void saveInventoryToFile(InventoryNode *head);
void addInventoryItem(InventoryNode **head);
void updateInventoryItem(InventoryNode *head);
void removeInventoryItem(InventoryNode **head);
void viewInventory(InventoryNode *head);
void freeInventoryList(InventoryNode *head);
void generateInventoryReport(InventoryNode *head);
void showLowStockOnly(InventoryNode *head);
void inventoryMenu(void);

#endif
