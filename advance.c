#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void pause() {
    printf("Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
}

// 1. Low Stock Alert (less than 5)
void lowStockAlert() {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Error opening products.txt\n");
        pause();
        return;
    }
    char line[200], id[10], name[50], category[10], supplier[10];
    int qty;
    float price;
    printf("\n--- Low Stock Products (Qty < 5) ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", id, name, category, supplier, &qty, &price);
        if (qty < 5)
            printf("ID: %s | Name: %s | Qty: %d\n", id, name, qty);
    }
    fclose(fp);
    pause();
}

// 2. Search Product by Name
void searchProductByName() {
    char keyword[50];
    printf("Enter keyword to search product name: ");
    scanf(" %[^\n]", keyword);

    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Error opening products.txt\n");
        pause();
        return;
    }

    char line[200], id[10], name[50], category[10], supplier[10];
    int qty;
    float price;
    int found = 0;
    printf("\n--- Search Results ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%f", id, name, category, supplier, &qty, &price);
        if (strstr(name, keyword)) {
            found = 1;
            printf("ID: %s | Name: %s | Price: %.2f\n", id, name, price);
        }
    }
    if (!found) printf("No matching product found.\n");
    fclose(fp);
    pause();
}

// 3. Transaction Summary (Total Count)
void transactionSummary() {
    FILE *fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("Error opening transactions.txt\n");
        pause();
        return;
    }
    char line[100];
    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        count++;
    }
    fclose(fp);
    printf("\nTotal Transactions: %d\n", count);
    pause();
}

// 4. User Contact Lookup
void findUserContact() {
    char uid[10];
    printf("Enter User ID (e.g., U00001): ");
    scanf("%s", uid);

    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        printf("Error opening users.txt\n");
        pause();
        return;
    }

    char line[100], id[10], fname[30], lname[30], contact[20];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%s", id, fname, lname, contact);
        if (strcmp(id, uid) == 0) {
            printf("User: %s %s | Contact: %s\n", fname, lname, contact);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("User ID not found.\n");

    fclose(fp);
    pause();
}

// 5. Supplier Email Finder
void supplierEmailFinder() {
    char sid[10];
    printf("Enter Supplier ID (e.g., S00001): ");
    scanf("%s", sid);

    FILE *fp = fopen("suppliers.txt", "r");
    if (!fp) {
        printf("Error opening suppliers.txt\n");
        pause();
        return;
    }

    char line[200], id[10], name[30], address[50], email[50];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%s", id, name, address, email);
        if (strcmp(id, sid) == 0) {
            printf("Supplier: %s | Email: %s\n", name, email);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Supplier ID not found.\n");

    fclose(fp);
    pause();
}

// === MAIN MENU ===
int main() {
    int choice;
    do {
        printf("\n--- Advanced Features Menu ---\n");
        printf("1. Show Low Stock Alert\n");
        printf("2. Search Product by Name\n");
        printf("3. Transaction Summary\n");
        printf("4. Find User Contact\n");
        printf("5. Find Supplier Email\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: lowStockAlert(); break;
            case 2: searchProductByName(); break;
            case 3: transactionSummary(); break;
            case 4: findUserContact(); break;
            case 5: supplierEmailFinder(); break;
            case 0: printf("Exiting Advanced Features.\n"); break;
            default: printf("Invalid choice.\n"); pause(); break;
        }
    } while (choice != 0);

    return 0;
}
