CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lqrencode -lpng
TARGET = qr_gen
SRCS = main.c png_writer.c display.c

OBJ_DIR = obj
BIN_DIR = bin
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: %.c qr_code.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

install-deps:
	sudo apt-get install libqrencode-dev libpng-dev

run: all
	./$(BIN_DIR)/$(TARGET) "https://example.com"

.PHONY: all clean install-deps run