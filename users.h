#ifndef USERS_H
#define USERS_H

// User structure
typedef struct {
    char userID[10];
    char firstName[30];
    char lastName[30];
    char contact[15];
} User;

// User operations
void addUser(void);
void viewUsers(void);
void updateUser(void);
void deleteUser(void);

// User menu handling
void displayUserMenu(void);
void handleUserMenuChoice(int choice);
void userManagementMenu(void);

// Utilities
int isValidUserID(const char *id);
int isValidContact(const char *contact);
int userExists(const char *userID);

#endif // USERS_H
