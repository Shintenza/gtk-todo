# GTK TODO APP 
![Preview](./preview.png)

## Dependencies 

- [GTK4](https://gitlab.gnome.org/GNOME/gtk)
- [sqlite3](https://www.sqlite.org/index.html)
- [make](https://www.gnu.org/software/make/)
- [gcc](https://gcc.gnu.org/)

## Project structure
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
│   │       ├── date_validator.h
│   │       ├── db_error.h
│   │       ├── dump_to_file.h
│   │       ├── first_time_launch.h
│   │       ├── get_main_dir.h
│   │       └── messages.h
│   ├── main.c
│   ├── task_handling.c
│   ├── task_loading.c
│   └── utils
│       ├── date_validator.c
│       ├── db_error.c
│       ├── dump_to_file.c
│       ├── first_time_launch.c
│       ├── get_main_dir.c
│       └── messages.c
└── style.css
```
## Building from source

Make sure that you have installed all required dependencies. After that you can simlpy type make inside the project.

ArchLinux:
> sudo pacman -S sqlite gtk4 make gcc

Debian:
> sudo apt install libsqlite3-dev libgtk-4-dev make gcc

