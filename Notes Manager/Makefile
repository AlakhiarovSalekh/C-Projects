CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lsqlite3
TARGET = notes
SOURCES = main.c db.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install
