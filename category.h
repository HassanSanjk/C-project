#include "product.h"

#ifndef CATEGORY_H
#define CATEGORY_H

#define CATEGORY_FILE "categories.txt"

typedef struct {
    char id[10];
    char name[50];
} Category;

int isValidCategory(const char *id);
void addCategory();
void viewCategories();
void updateCategory();
void deleteCategory();
void filterProductsByCategory(const char *categoryID);
void displayCategoryMenu(void);
void handleCategoryMenuChoice(int choice);
void categoryManagementMenu(void);

#endif