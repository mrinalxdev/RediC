#include "utils.h"

#define HASH_TABLE_SIZE 256

unsigned char hash(const char *key) {
    return (unsigned char)(key[0] % HASH_TABLE_SIZE);
}
