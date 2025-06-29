#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearInputBuffer(void);

// Validate date format YYYY-MM-DD
int isValidDate(const char *date) {
    if (strlen(date) != 10) return 0;
    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3) return 0;
    if (year < 2025 || month < 1 || month > 12 || day < 1 || day > 31) return 0;
    return 1;
}

// Pause for user input
void pause(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer(); // Clear leftover input
    getchar(); // Wait for Enter
}

// Clear terminal screen
void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Flush remaining input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
