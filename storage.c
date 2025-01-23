#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "utils.h"

#define HASH_TABLE_SIZE 256

KeyValue *hash_table[HASH_TABLE_SIZE];

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
