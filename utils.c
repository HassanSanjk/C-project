#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isValidDate(const char *date) {
    if (strlen(date) != 10) return 0;
    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3) return 0;
    if (year < 2025 || month < 1 || month > 12 || day < 1 || day > 31) return 0;
    return 1;
}

/* Function to pause and wait for user input */
void pause(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
    getchar(); // Wait for Enter
}

/* Function to clear screen */
void clearScreen(void) {
    /* Cross-platform screen clearing */
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* Add clearInputBuffer function if not in utils.h */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}