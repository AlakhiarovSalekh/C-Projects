CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = asteroid_game

all: $(TARGET)

$(TARGET): game.c
	$(CC) $(CFLAGS) -o $(TARGET) game.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
