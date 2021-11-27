CC = gcc
CFLAGS = -Wall
LDFLAGS = -lsqlite3 `pkg-config --cflags --libs gtk4`

all: main.o task_handling.o task_loading.o activate.o db_init.o cli_handling.o messages.o
	$(CC) $(CFLAGS) -o c_todo -O main.o task_handling.o task_loading.o activate.o db_init.o cli_handling.o messages.o $(LDFLAGS)

main.o: main.c include/structs.h include/activate.h include/db_init.h include/cli_handling.h
	$(CC) $(CFLAGS) -c main.c $(LDFLAGS)

task_handling.o: task_handling.c include/structs.h include/task_handling.h
	$(CC) $(CFLAGS) -c task_handling.c $(LDFLAGS)

task_loading.o: task_loading.c include/structs.h include/task_handling.h
	$(CC) $(CFLAGS) -c task_loading.c $(LDFLAGS)

activate.o: activate.c include/structs.h include/task_handling.h include/task_loading.h
	$(CC) $(CFLAGS) -c activate.c $(LDFLAGS)

db_init.o: db_init.c include/structs.h
	$(CC) $(CFLAGS) -c db_init.c $(LDFLAGS)

cli_handling.o: cli_handling.c include/structs.h
	$(CC) $(CFLAGS) -c cli_handling.c $(LDFLAGS)
messages.o: messages.c include/messages.h
	$(CC) $(CFLAGS) -c messages.c $(LDFLAGS)
clean:
	rm -f *.o
