#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6379
#define MAX_CLIENTS 10
#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 256

typedef struct KeyValue {
	char key[MAX_KEY_LENGTH];
	char value[MAX_VALUE_LENGTH];
	struct KeyValue *next;
} KeyValue;

KeyValue *hash_table[256];

unsigned char hash(const char *key){
	return (unsigned char)(key[0] % 256);
}

void set_value(const char *key, const char *value){
	unsigned char index = hash(key);
    KeyValue *entry = hash_table[index];

    while(entry != NULL){
        if(strcmp(entry ->key, key) == 0){
            stpncpy(entry -> value, value, MAX_VALUE_LENGTH);
            return;
        }
        entry = entry -> next;
    }

    KeyValue *new_entry = malloc(sizeof(KeyValue));
    strncpy(new_entry->key, key, MAX_KEY_LENGTH);
    strncpy(new_entry -> value, value, MAX_VALUE_LENGTH);
    new_entry->next = hash_table[index];
    hash_table[index] = new_entry;
}

const char *get_value(const char *key){
    unsigned char index = hash(key);
    KeyValue *entry = hash_table[index];

    while (entry != NULL){
        if (strcmp(entry -> key, key) == 0){
            return entry -> value;
        }
        entry = entry -> next;
    }
    return NULL;
}

void handle_client(int client_socket){
    char buffer[512];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0){
        buffer[bytes_read] = '\0';

        char command[4], key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];
        
    }
}
