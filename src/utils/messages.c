char* help_message() {
    return "Użycie:\n\
    [Opcje] [Flagi wybranej opcji]\n\n\
[Opcje]\n\
    -a, -A dodawanie nowego zadania\n\
        [Flagi wybranej opcji]\n\
            -t(wymagane) \"Nazwa zadania\"\n\
            -d(wymagane) \"Opis zadania\"\n\
            -tm(wymagane) \"czas wykonania zadania format 'dd/mm/rrrr/gg:mm'\"\n\n\
            [Przykładowe użycie]\n\
                c_todo -A -t \"Zaliczenie projektu z języka C\" -d \"Przykładowy opis zadania\" -tm \"28/12/2021/21:37\"\n\
    -l, L wyświetlanie zadań\n\
            -a (domyślne) - wyświetla aktywne zadania\n\
            -o - wyświetla zadania archiwalne\n\
            -v - wyświetla szczegółowe informacje o zadaniu\n\
    -d, D archiwizowanie/kasowanie zadań\n\
            -n (domyślne) - archiwizowanie po numerze zadania (dotyczy zadań aktywnych)\n\
            -b - archiwizowanie po id z bazy danych (jeżeli podano id zadania ukończonego zostanie ono usunięte na stale z bazy)\n\
            -Ao - kasowanie wszystkich zadań archiwalnych\n\
    -w, W wypisanie zadań do pliku w formacie markdown\n\
            -a (domyślne) - wypisuje wszystkie aktywne zadania do pliku\n\
            -o - wypisuje wszystkie zadania archiwalne do pliku\n\
            [Składnia]\n\
                c_todo -W [dostępne flagi] [ścieżka, gdzie ma być zapisany plik]\n\
            [Przykładowe użycie]\n\
                c_todo -w -o ~/zadanka_archiwalne\n\
            [Informacje dodatkowe]\n\
                Program samodzielnie dopisuje .md na końcu pliku (złośliwym zamienia inne rozszerzenia na markdown).\n\
    -F, f zmiana położnia bazy danych z plikami\n\
            -n - obecne położenie pliku z zadaniami (zwrócenie nazwy todo.db oznacza położenie domyślne)\n\
            [Przykładowe użycie]\n\
            c_todo -F stara/ścieżka/do_pliku/todo.db nowa/ścieżka/do_pliku/todo.db (zmiana położenia bazy danycj)\n\
            c_todo -F -n (sprawdzenie obecnej ścieżki do bazy danych)\n\
            [Dodatkowe informacje]\n\
            Pamiętaj aby przy podawaniu ścieżki do nowego położenia bazy zakończyć ją faktyczną nazwą pliku (nie musi to być todo.db)\n";
}
