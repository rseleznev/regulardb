#include "cache.h"
#include "stdlib.h"

Cache* new_cache(void);
void cache_replace(Cache* cache, char* key, void* data);
void* cache_get(Cache* cache, char* key);
void cache_delete(Cache* cache, char* key);
void cache_free(Cache* cache);

void free_list_for_idx(int idx);

Cache* new_cache(void) {
    Cache* cache = сalloc(1, sizeof(Cache));
    if (cache == NULL) {
        return NULL;
    }
    cache->records_count = 0;

    return cache;
}

void cache_replace(Cache* cache, char* key, void* data) {
    // получаем хеш ключа и индекс от него
    int i;

    CacheRecord* new_record = malloc(sizeof(CacheRecord));
    if (new_record == NULL) {
        return;
    }
    new_record->data = data;

    if (cache->records[i]->data == NULL) {
        new_record->next = NULL;
        cache->records[i] = new_record;
        cache->records_count++;
        return;
    }
    new_record->next = cache->records[i];
    cache->records[i] = new_record;
    cache->records_count++;
}

void cache_free(Cache* cache) {
    if (cache == NULL) {
        return;
    }

    int i;
    for (i = 0; i < CACHE_LEN; i++) {
        if (cache->records[i]->next != NULL) {
            free_list_for_idx(i);
            free(cache->records[i]);
        }
    }
    free(cache);
}

void free_list_for_idx(int idx) {
    // очистка связного списка для указанного индекса
}