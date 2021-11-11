#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {
    char *string = malloc(100);
    sprintf(string, "%s jest pog w %s", "XD", "XDDD");
    printf("%s", string);
}
