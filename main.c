#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "storage.h"

#define PORT 6379
#define MAX_CLIENTS 10

void handle_client(int client_socket) {
    char buffer[512];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        char command[4], key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];

        if (sscanf(buffer, "%3s %255s %255s", command, key, value) >= 2) {
            if (strcmp(command, "SET") == 0) {
                set_value(key, value);
                write(client_socket, "OK\n", 3);
            } else if (strcmp(command, "GET") == 0) {
                const char *result = get_value(key);
                if (result) {
                    write(client_socket, result, strlen(result));
                    write(client_socket, "\n", 1);
                } else {
                    write(client_socket, "NULL\n", 5);
                }
            }
        }
    }
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket >= 0) {
            handle_client(client_socket);
        }
    }

    close(server_socket);
    return 0;
}
