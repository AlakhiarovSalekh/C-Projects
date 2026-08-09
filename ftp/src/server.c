#include "ftp.h"

typedef struct {
    int socket;
    struct sockaddr_in address;
} client_info_t;

void* handle_client(void* arg) {
    client_info_t* client = (client_info_t*)arg;
    int client_socket = client->socket;
    char buffer[BUFFER_SIZE];
    char command[64], filename[MAX_FILENAME];

    printf("Client connected from %s\n", inet_ntoa(client->address.sin_addr));

    send_response(client_socket, RESP_OK, "FTP Server Ready");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) <= 0) {
            break;
        }

        if (sscanf(buffer, "%63s %255s", command, filename) < 1) {
            send_response(client_socket, RESP_ERROR, "Invalid command");
            continue;
        }

        printf("Command: %s\n", command);

        if (strcmp(command, CMD_LIST) == 0) {
            char file_list[BUFFER_SIZE];
            if (list_directory(file_list, sizeof(file_list)) >= 0) {
                send_response(client_socket, RESP_OK, "File list follows");
                send(client_socket, file_list, strlen(file_list), 0);
            }
            else {
                send_response(client_socket, RESP_ERROR, "Cannot list directory");
            }
        }
        else if (strcmp(command, CMD_GET) == 0) {
            if (access(filename, F_OK) == 0) {
                send_response(client_socket, RESP_OK, "Sending file");
                send_file(client_socket, filename);
            }
            else {
                send_response(client_socket, RESP_NOT_FOUND, "File not found");
            }
        }
        else if (strcmp(command, CMD_PUT) == 0) {
            send_response(client_socket, RESP_OK, "Ready to receive");
            if (receive_file(client_socket, filename) == 0) {
                printf("File %s uploaded successfully\n", filename);
            }
        }
        else if (strcmp(command, CMD_QUIT) == 0) {
            send_response(client_socket, RESP_OK, "Goodbye");
            break;
        }
        else {
            send_response(client_socket, RESP_ERROR, "Unknown command");
        }
    }

    printf("Client disconnected\n");
    close(client_socket);
    free(client);
    return NULL;
}

int main(int argc, char* argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int port = DEFAULT_PORT;

    if (argc > 1) {
        port = atoi(argv[1]);
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_socket);
        exit(1);
    }

    printf("FTP Server listening on port %d\n", port);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        client_info_t* client = malloc(sizeof(client_info_t));
        client->socket = client_socket;
        client->address = client_addr;

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client) != 0) {
            perror("pthread_create");
            close(client_socket);
            free(client);
        }
        else {
            pthread_detach(thread);
        }
    }

    close(server_socket);
    return 0;
}