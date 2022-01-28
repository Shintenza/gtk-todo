# GTK TODO APP - Projekt zaliczeniowy z języka C
![Preview](./preview.png)

## Spis treści:
1. [Kilka słów o projekcie](#kilka-słów-o-projekcie)
2. [Zależności](#zależności)
3. [Struktura projektu](#struktura-projektu)
4. [Kompilacja i użytkowanie programu](#kompilacja-i-użytkowanie-programu)

## Kilka słów o projekcie

Wybierając temat projektu kierowałem się głownie chęcią nauczenia się biblioteki GTK. Zdecydowałem się napisać todo-like aplikację, 
ponieważ wydawała się całkiem prosta koncepcjonalnie. Zapis i odczyt wprowadzonych przez użytkownika zadań odbywa się za pomocą relacyjnej
bazy danych. Nie zdecydowałem się tutaj na zapis i odczyt z pliku tekstowego, ponieważ chciałem (podobnie jak w przypadku GTK) nauczyć się
obsługi bazy danych z poziomu języka C. Przy wyborze systemu zarządzania bazą padło na sqlite3, ponieważ jest najłatwiejszy w przygotowaniu
do pracy (wystarczy sama instalacja programu). Dodatkowo, napisałem jeszcze wersję tekstową tego programu, jako dodatkowe ćwiczenie. Jeśli
chodzi o zawartość pliku Makfile, to zdecydowałem się na takie rozwiązanie, ponieważ jest wygodniejsze w przypadu większej liczby plików
źródłowych (ma też swoje minusy, jak np. ponowna kompilacja całego projektu po zmianie plików nagłówkowych). W ramach projektu przypomniałem 
sobie także, jak działają kaskadowe arkusze stylów, co było potrzebne do ostylowania aplikacji graficznej.

## Wykorzystane nardzędzia/biblioteki

Do stworzenia tego projektu użyłem poniższych narzędzi/bibliotek (wyjaśnienie ich obecności w projekcie znajduje się sekcję wyżej):

- [GTK4](https://gitlab.gnome.org/GNOME/gtk)
- [sqlite3](https://www.sqlite.org/index.html)
- [make](https://www.gnu.org/software/make/)
- [gcc](https://gcc.gnu.org/)

Wszystkie z powyższych zależności dostępne są chyba dla każdej dystrybucji GNU/Linuxa.

## Struktura projektu
```
├── bin
├── compile_commands.json
├── Makefile
├── obj
├── preview.png
├── README.md
├── src
│   ├── activate.c
│   ├── cli_handling.c
│   ├── db_init.c
│   ├── include
│   │   ├── activate.h
│   │   ├── cli_handling.h
│   │   ├── db_init.h
│   │   ├── global.h
│   │   ├── main.h
│   │   ├── structs.h
│   │   ├── task_handling.h
│   │   ├── task_loading.h
│   │   └── utils_h
│   │       ├── db_error.h
│   │       ├── first_time_launch.h
│   │       ├── get_main_dir.h
│   │       ├── initial_task_loading.h
│   │       └── messages.h
│   ├── main.c
│   ├── task_handling.c
│   ├── task_loading.c
│   └── utils
│       ├── db_error.c
│       ├── first_time_launch.c
│       ├── get_main_dir.c
│       └── messages.c
└── style.css
```

Skrótowy opis ważniejszych plików:
- compile_commands.json - plik potrzebny do pracy z [clangd](https://clangd.llvm.org/) i moim edytorem tekstu [nvim-code](https://gitea.shintenza.tk/Shintenza/nvim-code.git)
- activate.c - tutaj tworzona jest baza interfejsu graficznego 
- db_init.c - plik odpowiedzialny za inicjalizację bazy danych
- cli_handling.c - wersja cli programu
- messages.c - zawiera treść wiadomości wysłanej po użyciu --help
- main.c - inicjalizacja biblioteki graficznej, uruchamianie gui/cli
- task_handling.c - plik odpowiedzialny za wszelkie działania związane z dodawaniem/usuwaniem/edytowaniem zadań
- task_loading.c - plik odpowiedzialny za ładowanie zadań z bazy danych
- style.css - tutaj zdefiniowany jest wygląd poszczególnych widgetów interfejsu graficznego
- Makefile - ( ͡° ͜ʖ ͡°)

## Kompilacja i użytkowanie programu

Przed przystąpieniem do kompilacji projektu zainstaluj wymagane zależności:

ArchLinux:
> sudo pacman -S sqlite gtk4 make gcc

Debian:
> sudo apt install libsqlite3-dev libgtk-4-dev make gcc

Po zainstalowaniu odpowiednich zależności należy uruchomić komendę make (cel install także jest dostępny) w głównym folderze projektu. Plik 
binarny znajduje się w folderze bin. Uruchomienie tego pliku bez żadnych argumentów, wystartuje graficzną wersję aplikacji. Podanie 
argumentów skutkuje uruchomieniem wersji cli programu:

```
./bin/c_todo --help
Użycie:
    [Opcje] [Flagi wybranej opcji]

[Opcje]
    -a, -A dodawanie nowego zadania
        [Flagi wybranej opcji]
            -t(wymagane) "Nazwa zadania"
            -d(wymagane) "Opis zadania"
            -tm(wymagane) "czas wykonania zadania format 'dd\mm\rrrr\gg:mm'"

            [Przykładowe użycie]
                c_todo -A -t "Zaliczenie projektu z języka C" -d "Przykładowy opis zadania" -tm "28\12\2021\21:37"
    -l, L wyświetlanie zadań
            -a (domyślne) - wyświetla aktywne zadania
            -o - wyświetla zadania archiwalne
            -v - wyświetla szczegółowe informacje o zadaniu
    -d, D archiwizowanie/kasowanie zadań
            -n (domyślne) - archiwizowanie po numerze zadania (dotyczy zadań aktywnych)
            -b - archiwizowanie po id z bazy danych (jeżeli podano id zadania ukończonego zostanie ono usunięte na stale z bazy)
            -Ao - kasowanie wszystkich zadań archiwalnych
```
