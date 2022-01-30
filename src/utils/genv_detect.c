#include <stdlib.h>
#include <stdio.h>

void genv_detect () {
    char *display_env_var = getenv("DISPLAY");
    if (!display_env_var) {
        fprintf(stderr, "Niestety nie jestem w stanie wyświetlić aplikacji graficznej! Dostępna jest za to wersja tekstowa, wpisz --help aby dowiedzieć się więcej\n");
        exit(1);
    }
    return;
}
