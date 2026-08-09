#include "ftp.h"

void print_help() {
    printf("\nAvailable commands:\n");
    printf("  ls          - List server files\n");
    printf("  lls         - List local files\n");
    printf("  get <file>  - Download file from server\n");
    printf("  put <file>  - Upload file to server\n");
    printf("  help        - Show this help\n");
    printf("  quit        - Exit client\n\n");
}

void list_local_files() {
    char buffer[BUFFER_SIZE];
    if (list_directory(buffer, sizeof(buffer)) >= 0) {
        printf("Local files:\n%s", buffer);
    }
    else {
        printf("Error listing local directory\n");
    }
}

int main(int argc, char* argv[]) {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];
    char command[64], filename[MAX_FILENAME];
    char* server_ip;
    int port = DEFAULT_PORT;

    if (argc < 2) {
        printf("Usage: %s <server_ip> [port]\n", argv[0]);
        exit(1);
    }

    server_ip = argv[1];
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf("Invalid server IP address\n");
        close(client_socket);
        exit(1);
    }

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(client_socket);
        exit(1);
    }

    if (receive_response(client_socket, buffer, sizeof(buffer)) > 0) {
        printf("%s\n", buffer);
    }

    printf("Connected to FTP server. Type 'help' for commands.\n");

    while (1) {
        printf("ftp> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        memset(command, 0, sizeof(command));
        memset(filename, 0, sizeof(filename));
        sscanf(input, "%63s %255s", command, filename);

        if (strcmp(command, "quit") == 0) {
            send(client_socket, CMD_QUIT, strlen(CMD_QUIT), 0);
            receive_response(client_socket, buffer, sizeof(buffer));
            printf("%s\n", buffer);
            break;
        }
        else if (strcmp(command, "help") == 0) {
            print_help();
        }
        else if (strcmp(command, "lls") == 0) {
            list_local_files();
        }
        else if (strcmp(command, "ls") == 0) {
            send(client_socket, CMD_LIST, strlen(CMD_LIST), 0);

            if (receive_response(client_socket, buffer, sizeof(buffer)) > 0) {
                if (strncmp(buffer, RESP_OK, 3) == 0) {
                    printf("Server files:\n");
                    memset(buffer, 0, sizeof(buffer));
                    recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                    printf("%s", buffer);
                }
                else {
                    printf("Error: %s\n", buffer);
                }
            }
        }
        else if (strcmp(command, "get") == 0) {
            if (strlen(filename) == 0) {
                printf("Usage: get <filename>\n");
                continue;
            }

            char get_command[BUFFER_SIZE];
            snprintf(get_command, sizeof(get_command), "%s %s", CMD_GET, filename);
            send(client_socket, get_command, strlen(get_command), 0);

            if (receive_response(client_socket, buffer, sizeof(buffer)) > 0) {
                if (strncmp(buffer, RESP_OK, 3) == 0) {
                    if (receive_file(client_socket, filename) == 0) {
                        printf("File '%s' downloaded successfully\n", filename);
                    }
                }
                else {
                    printf("Error: %s\n", buffer);
                }
            }
        }
        else if (strcmp(command, "put") == 0) {
            if (strlen(filename) == 0) {
                printf("Usage: put <filename>\n");
                continue;
            }

            if (access(filename, F_OK) != 0) {
                printf("Local file '%s' not found\n", filename);
                continue;
            }

            char put_command[BUFFER_SIZE];
            snprintf(put_command, sizeof(put_command), "%s %s", CMD_PUT, filename);
            send(client_socket, put_command, strlen(put_command), 0);

            if (receive_response(client_socket, buffer, sizeof(buffer)) > 0) {
                if (strncmp(buffer, RESP_OK, 3) == 0) {
                    if (send_file(client_socket, filename) == 0) {
                        printf("File '%s' uploaded successfully\n", filename);
                    }
                }
                else {
                    printf("Error: %s\n", buffer);
                }
            }
        }
        else {
            printf("Unknown command. Type 'help' for available commands.\n");
        }
    }

    close(client_socket);
    printf("Connection closed.\n");
    return 0;
}