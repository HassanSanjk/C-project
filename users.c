#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "users.h"

/* Function to display user management menu */
void displayUserMenu(void) {
    printf("\n========================================\n");
    printf("         USER MANAGEMENT\n");
    printf("========================================\n");
    printf("| 1. Add User                      |\n");
    printf("| 2. View All Users                |\n");
    printf("| 3. Update User                   |\n");
    printf("| 4. Delete User                   |\n");
    printf("| 0. Back to Main Menu             |\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

/* Function to handle user menu choices */
void handleUserMenuChoice(int choice) {
    switch (choice) {
        case 1:
            printf("\n=== ADD USER ===\n");
            addUser();
            break;
            
        case 2:
            printf("\n=== ALL USERS ===\n");
            viewUsers();
            break;
            
        case 3:
            printf("\n=== UPDATE USER ===\n");
            updateUser();
            break;
            
        case 4:
            printf("\n=== DELETE USER ===\n");
            deleteUser();
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

/* Main users management menu function */
void userManagementMenu(void) {
    int choice;
    
    do {
        clearScreen();
        displayUserMenu();
        
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clearInputBuffer();
        
        handleUserMenuChoice(choice);
        
    } while (choice != 0);
}

int isValidUserID(const char *id) {
    if (strlen(id) != 6 || id[0] != 'U') return 0;
    for (int i = 1; i < 6; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

int isValidContact(const char *contact) {
    int len = strlen(contact);
    if (len < 5 || len > 15) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(contact[i]) && contact[i] != '+' && contact[i] != '-' && contact[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int userExists(const char *userID) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;
    
    char line[256], id[20];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]", id) == 1) {
            if (strcmp(id, userID) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void addUser() {
    User u;
    
    printf("Enter User ID (e.g., U00001): ");
    if (scanf("%19s", u.userID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }
    
    if (!isValidUserID(u.userID)) {
        printf("Invalid User ID format. Use U followed by 5 digits.\n");
        pause();
        return;
    }
    
    if (userExists(u.userID)) {
        printf("User ID already exists.\n");
        pause();
        return;
    }

    printf("Enter First Name: ");
    if (scanf(" %49[^\n]", u.firstName) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }
    
    printf("Enter Last Name: ");
    if (scanf(" %49[^\n]", u.lastName) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }

    printf("Enter Contact Number (5-15 digits, +/- allowed): ");
    if (scanf(" %19[^\n]", u.contact) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }
    
    if (!isValidContact(u.contact)) {
        printf("Invalid contact number format.\n");
        pause();
        return;
    }

    FILE *fp = fopen("users.txt", "a");
    if (!fp) {
        printf("Cannot open users file for writing.\n");
        pause();
        return;
    }

    fprintf(fp, "%s|%s|%s|%s\n", u.userID, u.firstName, u.lastName, u.contact);
    fclose(fp);
    
    printf("User added successfully.\n");
    pause();
}

void viewUsers() {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        printf("No users found or cannot open file.\n");
        pause();
        return;
    }

    User u;
    char line[256];
    int count = 0;
    
    printf("\n%-8s %-15s %-15s %-15s\n", "User ID", "First Name", "Last Name", "Contact");
    printf("==========================================================\n");
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%s", u.userID, u.firstName, u.lastName, u.contact) == 4) {
            printf("%-8s %-15s %-15s %-15s\n", u.userID, u.firstName, u.lastName, u.contact);
            count++;
        }
    }
    fclose(fp);
    
    if (count == 0) {
        printf("No users found.\n");
    } else {
        printf("==========================================================\n");
        printf("Total users: %d\n", count);
    }
    pause();
}

void updateUser() {
    char targetID[20];
    printf("Enter User ID to update: ");
    if (scanf("%19s", targetID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }

    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        pause();
        return;
    }

    User u;
    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%s", u.userID, u.firstName, u.lastName, u.contact) == 4) {
            if (strcmp(u.userID, targetID) == 0) {
                found = 1;
                printf("Current user: %s %s (Contact: %s)\n", u.firstName, u.lastName, u.contact);
                
                printf("Enter new First Name: ");
                if (scanf(" %49[^\n]", u.firstName) != 1) {
                    printf("Invalid input.\n");
                    fclose(fp); fclose(temp); remove("temp.txt"); 
                    pause(); return;
                }
                
                printf("Enter new Last Name: ");
                if (scanf(" %49[^\n]", u.lastName) != 1) {
                    printf("Invalid input.\n");
                    fclose(fp); fclose(temp); remove("temp.txt"); 
                    pause(); return;
                }
                
                printf("Enter new Contact Number: ");
                if (scanf(" %19[^\n]", u.contact) != 1 || !isValidContact(u.contact)) {
                    printf("Invalid contact number.\n");
                    fclose(fp); fclose(temp); remove("temp.txt"); 
                    pause(); return;
                }
            }
            fprintf(temp, "%s|%s|%s|%s\n", u.userID, u.firstName, u.lastName, u.contact);
        }
    }

    fclose(fp);
    fclose(temp);
    
    if (remove("users.txt") != 0 || rename("temp.txt", "users.txt") != 0) {
        printf("Error updating user file.\n");
    } else if (found) {
        printf("User updated successfully.\n");
    } else {
        printf("User not found.\n");
    }
    pause();
}

void deleteUser() {
    char targetID[20];
    printf("Enter User ID to delete: ");
    if (scanf("%19s", targetID) != 1) {
        printf("Invalid input.\n");
        pause();
        return;
    }

    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        pause();
        return;
    }

    User u;
    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%s", u.userID, u.firstName, u.lastName, u.contact) == 4) {
            if (strcmp(u.userID, targetID) != 0) {
                fprintf(temp, "%s", line);
            } else {
                found = 1;
                printf("Deleting user: %s %s (ID: %s)\n", u.firstName, u.lastName, u.userID);
            }
        }
    }

    fclose(fp);
    fclose(temp);
    
    if (remove("users.txt") != 0 || rename("temp.txt", "users.txt") != 0) {
        printf("Error updating user file.\n");
    } else if (found) {
        printf("User deleted successfully.\n");
    } else {
        printf("User not found.\n");
    }
    pause();
}