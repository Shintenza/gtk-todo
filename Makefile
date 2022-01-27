CC = gcc
CFLAGS = -Wall
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/utils/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HEADERS = $(wildcard $(SRC)/include/*.h)
LDFLAGS = -lsqlite3 `pkg-config --cflags --libs gtk4`
BIN_DIR = bin
BIN = $(BIN_DIR)/c_todo
INSTALL_DIR = /opt/c_todo
INSTALL_DIST = /usr/bin/c_todo

all: $(BIN)
$(BIN): $(OBJS) | structure
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) | structure
	$(CC) $(CFLAGS) -c $< -o $@	$(LDFLAGS)
structure:
	mkdir -p $(OBJ)/utils $(BIN_DIR)
install: all
	mkdir -p $(INSTALL_DIR)
	cp -rf ./* $(INSTALL_DIR) 
	chmod -R 755 $(INSTALL_DIR)
	ln -sf $(INSTALL_DIR)/$(BIN) $(INSTALL_DIST)
	chmod -R 755 $(INSTALL_DIST)
uninstall:
	rm -rf /opt/c_todo
	rm -rf /usr/bin/c_todo
clean:
	rm -rf $(BIN_DIR)/* $(OBJ)/*
