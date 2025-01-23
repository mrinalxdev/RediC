#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data_s.h"
#include "utils.h"

#define HASH_TABLE_SIZE 256

List lists[HASH_TABLE_SIZE];
Set sets[HASH_TABLE_SIZE];
Hash hashes[HASH_TABLE_SIZE];


void list_push(const char *key, const char *value){
    unsigned char index = hash(key);
    ListNode *new_node = malloc(sizeof(ListNode));
    strncpy(new_node->value, value, MAX_VALUE_LENGTH);
    new_node->next = lists[index].head;
    lists[index].head=new_node;
}

const char *list_pop(const char *key){
    unsigned char index = hash(key);
    ListNode *head = lists[index].head;

    if (head == NULL){
        return NULL;
        // adding a basic null check
    }

    lists[index].head = head->next;
    const char *value = strdup(head->value);
    free(head);
    return value;
}

void set_add(const char *key, const char *value){
    unsigned char index = hash(key);
    SetNode *new_node = malloc(sizeof(SetNode));
    strncpy(new_node->value, value, MAX_KEY_LENGTH);
    new_node->next = sets[index].head;
    sets[index].head = new_node;

    // while (node != NULL){
    //     if (strcmp(node->value, value) == 1){
    //         return false;
    //     }
    // }
}


bool set_contains(const char *key, const char *value){
    unsigned char index = hash(key);
    SetNode *node = sets[index].head;

    while (node != NULL){
        if (strcmp(node ->value, value) == 0){
            return true;
        }
        node = node->next;
    }
    return false;
}

void hash_set(const char *key, const char *value){
    unsigned char index = hash(key);
    HashNode *new_node = malloc(sizeof(HashNode));
    strncpy(new_node->key, key, MAX_KEY_LENGTH);
    strncpy(new_node->value, value, MAX_VALUE_LENGTH);
    new_node->next = hashes[index].head;
    hashes[index].head = new_node;
}


const char *hash_get(const char *key){
    unsigned char index = hash(key);
    HashNode *node = hashes[index].head;

    while(node != NULL){
        if (strcmp(node->key, key) == 0){
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}
