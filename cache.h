#ifndef CACHE_H
#define CACHE_H

#define CACHE_LEN 1024

typedef struct CacheRecord {
    void* data;
    CacheRecord* next;
} CacheRecord;

typedef struct Cache {
    CacheRecord* records[CACHE_LEN];
    int records_count;
} Cache;

Cache* new_cache(void);
void cache_append(char* key, void* data);
void* cache_get(char* key);
void cache_delete(char* key);
void cache_free(void);

#endif