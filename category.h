#ifndef CATEGORY_H
#define CATEGORY_H

#define CATEGORY_FILE "data/categories.txt"

typedef struct {
    int id;
    char name[50];
    char description[100];
} Category;

void addCategory();
void viewCategories();
void updateCategory();
void deleteCategory();
void filterProductsByCategory(int);

#endif
