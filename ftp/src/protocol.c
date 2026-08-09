#include "ftp.h"

int send_response(int socket, const char* code, const char* message) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response), "%s %s\n", code, message);

    if (send(socket, response, strlen(response), 0) < 0) {
        perror("send_response");
        return -1;
    }
    return 0;
}

int receive_response(int socket, char* buffer, size_t buffer_size) {
    ssize_t bytes_received = recv(socket, buffer, buffer_size - 1, 0);
    if (bytes_received < 0) {
        perror("receive_response");
        return -1;
    }

    buffer[bytes_received] = '\0';

    char* newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    }

    return bytes_received;
}