#ifndef CACHE_H
#define CACHE_H

#define CACHE_LEN 1024

#include <stdbool.h>

typedef struct CacheRecord {
    char key[30];
    void* data;
    struct CacheRecord* next;
} CacheRecord;

typedef struct Cache {
    CacheRecord* records[CACHE_LEN];
    int records_count;
} Cache;

Cache* new_cache(void);
void cache_replace(Cache* cache, char* key, void* data);
bool cache_has_key(Cache* cache, char* key);
void* cache_get(Cache* cache, char* key);
void cache_delete(Cache* cache, char* key);
void cache_free(Cache* cache);

#endif