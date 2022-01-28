#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void extract_from_datestring (char *date_string, char *day_string, char *month_string, char *year_string, char *hours_string, char *minutes_string) {
    char date_string_cpy[17];
    strcpy(date_string_cpy, date_string);
    strcpy(day_string, strtok(date_string_cpy, "/"));
    strcpy(month_string, strtok(NULL, "/"));
    strcpy(year_string, strtok(NULL, "/"));
    strcpy(hours_string, strtok(NULL, ":"));
    strcpy(minutes_string, strtok(NULL, ":"));
}
void date_validator (int day, int month, int year, int hour, int minutes) {
    if (day < 0 || month < 0 || hour < 0 || minutes < 0 ) {
        fprintf(stderr, "Podane wartości nie mogą być ujemne!\n");
        exit(1);
    }
    if (day > 31) {
        fprintf(stderr, "Podano nieprawidłowy dzień miesiąca!\n");
        exit(1);
    }
    if ( month > 12) {
        fprintf(stderr, "Podano nieprawidłowy miesiąc!\n");
        exit(1);
    } 
    if (hour > 23) {
        fprintf(stderr, "Podano nieprawidłową godzinę!\n");
        exit(1);
    } 
    if (minutes > 59 ) {
        fprintf(stderr, "Podano nieprawidłową liczbę minut!\n");
        exit(1);
    }
}
