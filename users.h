#ifndef USERS_H
#define USERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure definitions
typedef struct {
    char userID[10];
    char firstName[30];
    char lastName[30];
    char contact[15];
} User;

// Function declarations
void pause(void);
int isValidUserID(const char *id);
int isValidContact(const char *contact);
int userExists(const char *userID);
void addUser(void);
void viewUsers(void);
void updateUser(void);
void deleteUser(void);
void userMenu(void);

#endif // USERS_H