# GTK TODO APP - Projekt zaliczeniowy z języka C


## Spis treści:
1. [Kilka słów o projekcie](#kilka-słów-o-projekcie)
2. Zależności
3. Struktura projektu
4. Podstawowe informacje o obsłudze programu

## Kilka słów o projekcie

Przy wyborze tematu projektu kierowałem się głównie chęcią nauczenia się biblioteki do tworzenia interfejsów graficznych, jaką jest GTK. Todo-like aplikacja była pierwszym pomysłem, który wpadł mi do głowy. Chciałem go zrealizować, ponieważ opiera sie na prostych założeniach i będzie dobrym początkiem do pisania poważniejszych programów w przyszłości. GTK nie jest jedyną biblioteką, z której skorzystałem. Zapis i odczyt zadań jest oparty o bazę danych. Wybór padł tutaj na sqlite3, ponieważ nie wymaga żadnej konfiguracji, a co za tym idzie użytkownik programu nie jest zmuszony do podejmowania żadnych dodatakowych kroków przed jego uruchomieniem. Jako dodatkowe ćwiczenie napisałem także wersję aplikacji sterowaną z wiersza poleceń. Interfejs graficzny doczekał się także swojego własnego arkusza stylów.

## Zależności

Do poprawnego działania programu wymagane są poniższe zależności:

- [GTK4](https://gitlab.gnome.org/GNOME/gtk)
- [sqlite3](https://www.sqlite.org/index.html)
- [make](https://www.gnu.org/software/make/)

Wszystkie z powyższych zależności dostępne są chyba dla każdej dystrybucji GNU/Linuxa.

## Struktura projektu
```
├── bin
├── compile_commands.json
├── Makefile
├── obj
├── README.md
├── src
│   ├── activate.c
│   ├── cli_handling.c
│   ├── db_init.c
│   ├── include
│   │   ├── activate.h
│   │   ├── cli_handling.h
│   │   ├── db_init.h
│   │   ├── messages.h
│   │   ├── structs.h
│   │   ├── task_handling.h
│   │   └── task_loading.h
│   ├── main.c
│   ├── messages.c
│   ├── task_handling.c
│   └── task_loading.c
├── style.css
├── test.c
└── todo.db
```

Skrótowy opis poszczególnych plików:
- compile_commands.json - plik potrzebny do pracy z [clangd](#https://clangd.llvm.org/)
- activate.c - tutaj tworzona jest baza interfejsu graficznego 
- db_init.c - plik odpowiedzialny za inicjalizację bazy danych
- cli_handling.c - wersja cli programu
- messages.c - zawiera treść wiadomości wysłanej po użyciu --help
- main.c - inicjalizacja biblioteki graficznej, uruchamianie gui/cli
- task_handling.c - plik odpowiedzialny za wszelkie działania związane z dodawaniem/usuwaniem/edytowaniem zadań
- task_loading.c - plik odpowiedzialny za ładowanie zadań z bazy danych
- style.css - tutaj zdefiniowany jest wygląd poszczególnych widgetów interfejsu graficznego


