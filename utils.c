#include <stdio.h>
#include <string.h>

int isValidDate(const char *date) {
    if (strlen(date) != 10) return 0;
    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3) return 0;
    if (year < 2025 || month < 1 || month > 12 || day < 1 || day > 31) return 0;
    return 1;
}
