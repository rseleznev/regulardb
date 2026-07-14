#include "cache.h"
#include "stdlib.h"

Cache* new_cache(void);
void cache_append(char* key, void* data);
void* cache_get(char* key);
void cache_delete(char* key);
void cache_free(void);

void free_list_for_idx(int idx);

static Cache* cache;

Cache* new_cache(void) {
    if (cache != NULL) {
        return cache;
    }
    
    cache = сalloc(1, sizeof(Cache));
    if (cache == NULL) {
        return NULL;
    }
    cache->records_count = 0;

    return cache;
}

void cache_append(char* key, void* data) {
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

void cache_free(void) {
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