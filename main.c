#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include "storage.h"
#include "data_s.h"
#include "utils.h"

#define PORT 6379
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void handle_client_command(int client_socket, char *buffer) {
    char command[16], key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];


    memset(command, 0, sizeof(command));
    memset(key, 0, sizeof(key));
    memset(value, 0, sizeof(value));


    int parsed = sscanf(buffer, "%15s %255s %255s", command, key, value);

    if (parsed >= 2) {
        char response[BUFFER_SIZE] = {0};

        if (strcmp(command, "SET") == 0 && parsed == 3) {
            hash_set(key, value);
            snprintf(response, sizeof(response), "+OK\r\n");
        }
        else if (strcmp(command, "GET") == 0) {
            const char *result = hash_get(key);
            if (result) {
                snprintf(response, sizeof(response), "$%zu\r\n%s\r\n", strlen(result), result);
            } else {
                snprintf(response, sizeof(response), "$-1\r\n");
            }
        }
        else if (strcmp(command, "PUSH") == 0 && parsed == 3) {
            list_push(key, value);
            snprintf(response, sizeof(response), "+OK\r\n");
        }
        else if (strcmp(command, "POP") == 0) {
            const char *result = list_pop(key);
            if (result) {
                snprintf(response, sizeof(response), "$%zu\r\n%s\r\n", strlen(result), result);
                free((void *)result);
            } else {
                snprintf(response, sizeof(response), "$-1\r\n");
            }
        }
        else if (strcmp(command, "SADD") == 0 && parsed == 3) {
            set_add(key, value);
            snprintf(response, sizeof(response), "+OK\r\n");
        }
        else if (strcmp(command, "SISMEMBER") == 0 && parsed == 3) {
            bool result = set_contains(key, value);
            snprintf(response, sizeof(response), ":%d\r\n", result);
        }
        else {
            snprintf(response, sizeof(response), "-ERR Unknown command\r\n");
        }


        if (send(client_socket, response, strlen(response), 0) < 0) {
            perror("Error sending response");
        }
    }
}

int main() {
    int server_socket, max_sd;
    fd_set master_set, read_fds;
    struct sockaddr_in server_addr;
    int client_sockets[MAX_CLIENTS] = {0};


    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }


    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }


    FD_ZERO(&master_set);
    FD_SET(server_socket, &master_set);
    max_sd = server_socket;

    printf("Redis Server listening on port %d\n", PORT);

    while (1) {
        // Copy master set to read set
        read_fds = master_set;

        // Wait for activity on sockets
        if (select(max_sd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Check for new connection
        if (FD_ISSET(server_socket, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

            if (new_socket < 0) {
                perror("Accept failed");
                continue;
            }

            FD_SET(new_socket, &master_set);
            if (new_socket > max_sd) {
                max_sd = new_socket;
            }

            printf("New connection, socket fd is %d\n", new_socket);
        }
        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &read_fds) && i != server_socket) {
                char buffer[BUFFER_SIZE] = {0};
                int bytes_read = recv(i, buffer, BUFFER_SIZE - 1, 0);

                if (bytes_read <= 0) {
                    // Connection closed or error
                    close(i);
                    FD_CLR(i, &master_set);
                } else {
                    buffer[bytes_read] = '\0';
                    handle_client_command(i, buffer);
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
