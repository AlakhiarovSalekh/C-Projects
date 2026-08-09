#include "ftp.h"

int send_file(int socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        send_response(socket, RESP_NOT_FOUND, "File not found");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (send(socket, &file_size, sizeof(file_size), 0) < 0) {
        perror("send file size");
        fclose(file);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    long total_sent = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(socket, buffer, bytes_read, 0) < 0) {
            perror("send file content");
            fclose(file);
            return -1;
        }
        total_sent += bytes_read;
    }

    fclose(file);
    printf("Sent %ld bytes\n", total_sent);
    return 0;
}

int receive_file(int socket, const char* filename) {

    long file_size;
    if (recv(socket, &file_size, sizeof(file_size), 0) < 0) {
        perror("receive file size");
        return -1;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("create file");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    long total_received = 0;

    while (total_received < file_size) {
        size_t to_receive = (file_size - total_received < BUFFER_SIZE) ?
            (file_size - total_received) : BUFFER_SIZE;

        ssize_t bytes_received = recv(socket, buffer, to_receive, 0);
        if (bytes_received <= 0) {
            perror("receive file content");
            fclose(file);
            return -1;
        }

        fwrite(buffer, 1, bytes_received, file);
        total_received += bytes_received;
    }

    fclose(file);
    printf("Received %ld bytes\n", total_received);
    return 0;
}

int list_directory(char* buffer, size_t buffer_size) {
    DIR* dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return -1;
    }

    struct dirent* entry;
    struct stat file_stat;
    size_t offset = 0;

    while ((entry = readdir(dir)) != NULL && offset < buffer_size - 1) {

        if (entry->d_name[0] == '.') {
            continue;
        }

        if (stat(entry->d_name, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            int written = snprintf(buffer + offset, buffer_size - offset,
                "%s\n", entry->d_name);
            if (written > 0 && offset + written < buffer_size) {
                offset += written;
            }
            else {
                break;
            }
        }
    }

    closedir(dir);
    buffer[offset] = '\0';
    return offset;
}