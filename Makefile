CC = gcc
CFLAGS = -Wall
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HEADERS = $(wildcard $(SRC)/include/*.h)
LDFLAGS = -lsqlite3 `pkg-config --cflags --libs gtk4`
BIN_DIR = bin
BIN = $(BIN_DIR)/c_todo

all: $(BIN)
$(BIN): $(OBJS) | structure
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) | structure
	$(CC) $(CFLAGS) -c $< -o $@	$(LDFLAGS)
structure:
	mkdir -p $(OBJ) $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)/* $(OBJ)/*
