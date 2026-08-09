#ifndef FTP_H
#define FTP_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 4096
#define MAX_FILENAME 256
#define MAX_CLIENTS 10

#define CMD_LIST "LIST"
#define CMD_GET "RETR"
#define CMD_PUT "STOR"
#define CMD_QUIT "QUIT"

#define RESP_OK "200"
#define RESP_ERROR "500"
#define RESP_NOT_FOUND "404"

int send_file(int socket, const char* filename);
int receive_file(int socket, const char* filename);
int list_directory(char* buffer, size_t buffer_size);
int send_response(int socket, const char* code, const char* message);
int receive_response(int socket, char* buffer, size_t buffer_size);

#endif