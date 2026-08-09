CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = lexer
SRC = lexer.c
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
