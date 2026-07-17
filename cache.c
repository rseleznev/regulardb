#include "cache.h"
#include "stdlib.h"

Cache* new_cache(void);
void cache_replace(Cache* cache, char* key, void* data);
bool cache_has_key(Cache* cache, char* key);
void* cache_get(Cache* cache, char* key);
void cache_delete(Cache* cache, char* key);
void cache_free(Cache* cache);

void free_list_for_idx(int idx);
CacheRecord* get_record(Cache* cache, char* key);

Cache* new_cache(void) {
    return NULL;
    Cache* cache = (Cache*)calloc(1, sizeof(Cache));
    if (cache == NULL) {
        return NULL;
    }

    return cache;
}

void cache_replace(Cache* cache, char* key, void* data) {
    // проверяем, есть ли запись для данного ключа
    CacheRecord* old_record = get_record(cache, key);
    if (old_record != NULL) {
        // если запись есть, обновляем данные
        old_record->data = data;
        return;
    }
    
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

bool cache_has_key(Cache* cache, char* key) {
    return true;
}

void* cache_get(Cache* cache, char* key) {
    return NULL;
}

void cache_delete(Cache* cache, char* key) {

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

CacheRecord* get_record(Cache* cache, char* key) {
    // получаем хеш ключа и индекс от него
    int i;

    if (cache->records[i]->data == NULL) {
        return NULL;
    }
    if (cache->records[i]->next == NULL) {
        return cache->records[i];
    }
    // ищем в списке

    return NULL;
}