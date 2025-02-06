// rdb.h
#ifndef RDB_H
#define RDB_H

#include "data_s.h"
#include "storage.h"

#define HASH_TABLE_SIZE 256
#define RDB_VERSION 1

extern List lists[HASH_TABLE_SIZE];
extern Set sets[HASH_TABLE_SIZE];
extern Hash hashes[HASH_TABLE_SIZE];

int save_rdb(const char *filename);
void load_rdb(const char *filename);

#endif
