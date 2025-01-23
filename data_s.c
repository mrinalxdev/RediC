#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data_s.h"

#define HASH_TABLE_SIZE 256

List lists[HASH_TABLE_SIZE];
Set sets[HASH_TABLE_SIZE];
Hash hashes[HASH_TABLE_SIZE];

unsigned char hash(const char *key){
    return (unsigned char)(key[0] % HASH_TABLE_SIZE);
}

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
