// rdb.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rdb.h"
#include "data_s.h"
#include "storage.h"

#define RDB_VERSION 1

void save_rdb(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open RDB file for writing");
        return;
    }
    fwrite(&RDB_VERSION, sizeof(int), 1, file);

    // Save hashes
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = hashes[i].head;
        while (node) {
            // Write key length and key
            int key_length = strlen(node->key);
            fwrite(&key_length, sizeof(int), 1, file);
            fwrite(node->key, sizeof(char), key_length, file);

            // Write value length and value
            int value_length = strlen(node->value);
            fwrite(&value_length, sizeof(int), 1, file);
            fwrite(node->value, sizeof(char), value_length, file);

            node = node->next;
        }
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ListNode *node = lists[i].head;
        while (node) {
            // key length and key
            int key_length = strlen(node->value); // Using value as key for lists
            fwrite(&key_length, sizeof(int), 1, file);
            fwrite(node->value, sizeof(char), key_length, file);

            node = node->next;
        }
    }


    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        SetNode *node = sets[i].head;
        while (node) {
            int key_length = strlen(node->value);
            fwrite(&key_length, sizeof(int), 1, file);
            fwrite(node->value, sizeof(char), key_length, file);

            node = node->next;
        }
    }

    fclose(file);
}

void load_rdb(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open RDB file for reading");
        return;
    }

    // Read RDB version
    int version;
    fread(&version, sizeof(int), 1, file);
    if (version != RDB_VERSION) {
        fprintf(stderr, "Unsupported RDB version: %d\n", version);
        fclose(file);
        return;
    }

    // Load hashes
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        while (1) {
            int key_length;
            if (fread(&key_length, sizeof(int), 1, file) != 1) break;

            char *key = malloc(key_length + 1);
            fread(key, sizeof(char), key_length, file);
            key[key_length] = '\0';

            int value_length;
            fread(&value_length, sizeof(int), 1, file);
            char *value = malloc(value_length + 1);
            fread(value, sizeof(char), value_length, file);
            value[value_length] = '\0';

            hash_set(key, value); // Use your existing hash_set function

            free(key);
            free(value);
        }
    }

    // Load lists
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        while (1) {
            int key_length;
            if (fread(&key_length, sizeof(int), 1, file) != 1) break;

            char *key = malloc(key_length + 1);
            fread(key, sizeof(char), key_length, file);
            key[key_length] = '\0';

            // Read value for the list
            int value_length;
            fread(&value_length, sizeof(int), 1, file);
            char *value = malloc(value_length + 1);
            fread(value, sizeof(char), value_length, file);
            value[value_length] = '\0';

            list_push(key, value); // Use your existing list_push function

            free(key);
            free(value);
        }
    }

    // Load sets
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        while (1) {
            int key_length;
            if (fread(&key_length, sizeof(int), 1, file) != 1) break;

            char *key = malloc(key_length + 1);
            fread(key, sizeof(char), key_length, file);
            key[key_length] = '\0';

            // Read value for the set
            int value_length;
            fread(&value_length, sizeof(int), 1, file);
            char *value = malloc(value_length + 1);
            fread(value, sizeof(char), value_length, file);
            value[value_length] = '\0';

            set_add(key, value); // Use your existing set_add function

            free(key);
            free(value);
        }
    }

    fclose(file);
}
