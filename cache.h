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
void cache_replace(Cache* cache, char* key, void* data);
void* cache_get(Cache* cache, char* key);
void cache_delete(Cache* cache, char* key);
void cache_free(Cache* cache);

#endif