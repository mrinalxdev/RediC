#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdbool.h>

#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 256

typedef struct ListNode {
    char value[MAX_VALUE_LENGTH];
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
} List;

typedef struct SetNode {
    char value[MAX_VALUE_LENGTH];
    struct SetNode *next;
} SetNode;

typedef struct {
    SetNode *head;
} Set;

typedef struct HashNode {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *head;
} Hash;

void list_push(const char *key, const char *value);
const char *list_pop(const char *key);
void set_add(const char *key, const char *value);
bool set_contains(const char *key, const char *value);
void hash_set(const char *key, const char *value);
const char *hash_get(const char *key);

#endif
