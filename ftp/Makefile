CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
SRCDIR = src
BINDIR = bin
INCDIR = include

SOURCES_SERVER = $(SRCDIR)/server.c $(SRCDIR)/file_ops.c $(SRCDIR)/protocol.c
SOURCES_CLIENT = $(SRCDIR)/client.c $(SRCDIR)/file_ops.c $(SRCDIR)/protocol.c

SERVER_DIR = $(BINDIR)/server
CLIENT_DIR = $(BINDIR)/client
TARGET_SERVER = $(SERVER_DIR)/ftp_server
TARGET_CLIENT = $(CLIENT_DIR)/ftp_client

.PHONY: all clean dirs test-files

all: dirs test-files $(TARGET_SERVER) $(TARGET_CLIENT)

dirs:
	@mkdir -p $(SERVER_DIR) $(CLIENT_DIR)

test-files: dirs
	@echo "Creating test files..."
	@echo "Hello from server!" > $(SERVER_DIR)/server_readme.txt
	@echo "This is a test document from the server side." > $(SERVER_DIR)/document.txt
	@echo "Server log file content here." > $(SERVER_DIR)/server.log
	@echo "#!/bin/bash\necho 'Server script executed'" > $(SERVER_DIR)/script.sh
	@chmod +x $(SERVER_DIR)/script.sh
	@echo "Hello from client!" > $(CLIENT_DIR)/client_readme.txt
	@echo "This file will be uploaded to server." > $(CLIENT_DIR)/upload_me.txt
	@echo "Client configuration file." > $(CLIENT_DIR)/config.conf
	@echo "Sample data for testing transfers." > $(CLIENT_DIR)/data.txt

$(TARGET_SERVER): $(SOURCES_SERVER)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@

$(TARGET_CLIENT): $(SOURCES_CLIENT)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@

clean:
	rm -rf $(BINDIR) *.o *~

install: all
	cp $(TARGET_SERVER) $(TARGET_CLIENT) /usr/local/bin/

run-server: $(TARGET_SERVER)
	@echo "Starting FTP server..."
	@cd $(SERVER_DIR) && ./ftp_server

run-client: $(TARGET_CLIENT)
	@echo "Starting FTP client..."
	@cd $(CLIENT_DIR) && ./ftp_client 127.0.0.1

.SUFFIXES: .c .o