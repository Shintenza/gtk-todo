char* help_message() {
    return "Użycie:\n\
    [Opcje] [Flagi wybranej opcji]\n\n\
[Opcje]\n\
    -a, -A dodawanie nowego zadania\n\
        [Flagi wybranej opcji]\n\
            -t(wymagane) \"Nazwa zadania\"\n\
            -d(wymagane) \"Opis zadania\"\n\
            -tm(wymagane) \"czas wykonania zadania format 'dd\\mm\\rrrr\\gg:mm'\"\n\n\
            [Przykładowe użycie]\n\
                c_todo -A -t \"Zaliczenie projektu z języka C\" -d \"Przykładowy opis zadania\" -tm \"28\\12\\2021\\21:37\"\n\
    -l, L wyświetlanie zadań\n\
            -a (domyślne) - wyświetla aktywne zadania\n\
            -o - wyświetla zadania archiwalne\n";
}
