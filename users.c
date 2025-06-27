#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// === User Management ===

typedef struct {
    char userID[10];
    char firstName[30];
    char lastName[30];
    char contact[15];
} User;

void pause() {
    printf("Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
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
        if (!isdigit(contact[i])) return 0;
    }
    return 1;
}

int userExists(const char *userID) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;
    char line[200], id[20];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]", id);
        if (strcmp(id, userID) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addUser() {
    User u;
    printf("Enter User ID (e.g., U00001): ");
    scanf("%s", u.userID);
    if (!isValidUserID(u.userID)) {
        printf("Invalid User ID format.\n");
        pause();
        return;
    }
    if (userExists(u.userID)) {
        printf("User ID already exists.\n");
        pause();
        return;
    }

    printf("Enter First Name: ");
    scanf(" %[^\n]", u.firstName);
    printf("Enter Last Name: ");
    scanf(" %[^\n]", u.lastName);

    printf("Enter Contact Number (5 to 15 digits): ");
    scanf(" %[^\n]", u.contact);
    if (!isValidContact(u.contact)) {
        printf("Invalid contact number.\n");
        pause();
        return;
    }

    FILE *fp = fopen("users.txt", "a");
    if (!fp) {
        printf("Cannot open users file.\n");
        pause();
        return;
    }

    fprintf(fp, "%s|%s|%s|%s\n", u.userID, u.firstName, u.lastName, u.contact);
    fclose(fp);
    printf("User added.\n");
    pause();
}

void viewUsers() {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        printf("Cannot open users file.\n");
        pause();
        return;
    }

    User u;
    char line[200];
    printf("\n--- User List ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%s", u.userID, u.firstName, u.lastName, u.contact);
        printf("ID: %s | Name: %s %s | Contact: %s\n", u.userID, u.firstName, u.lastName, u.contact);
    }
    fclose(fp);
    pause();
}

void updateUser() {
    char targetID[10];
    printf("Enter User ID to update: ");
    scanf("%s", targetID);

    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    User u;
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%s", u.userID, u.firstName, u.lastName, u.contact);
        if (strcmp(u.userID, targetID) == 0) {
            found = 1;
            printf("Enter new First Name: ");
            scanf(" %[^\n]", u.firstName);
            printf("Enter new Last Name: ");
            scanf(" %[^\n]", u.lastName);
            printf("Enter new Contact Number (5 to 15 digits): ");
            scanf(" %[^\n]", u.contact);
            if (!isValidContact(u.contact)) {
                printf("Invalid contact number.\n");
                fclose(fp); fclose(temp); remove("temp.txt"); pause(); return;
            }
        }
        fprintf(temp, "%s|%s|%s|%s\n", u.userID, u.firstName, u.lastName, u.contact);
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User updated.\n");
    else
        printf("User not found.\n");
    pause();
}

void deleteUser() {
    char targetID[10];
    printf("Enter User ID to delete: ");
    scanf("%s", targetID);

    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        pause();
        return;
    }

    User u;
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%s", u.userID, u.firstName, u.lastName, u.contact);
        if (strcmp(u.userID, targetID) != 0) {
            fprintf(temp, "%s", line);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User deleted.\n");
    else
        printf("User not found.\n");
    pause();
}

int main() {
    int choice;
    do {
        printf("\n--- User Menu ---\n");
        printf("1. Add User\n");
        printf("2. View Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); break;
            case 2: viewUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 0: printf("Exiting User Menu.\n"); break;
            default: printf("Invalid option. Please choose a number between 0 and 4.\n"); pause(); break;
        }
    } while (choice != 0);

    return 0;
}
