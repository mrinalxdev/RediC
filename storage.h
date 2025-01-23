#ifndef STORAGE_H
#define STORAGE_H

#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 256

typedef struct KeyValue {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    struct KeyValue *next;
} KeyValue;

void set_value(const char *key, const char *value);
const char *get_value(const char *key);

#endif
