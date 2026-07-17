#include "cache.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

Cache* new_cache(void);
void cache_replace(Cache* cache, char* key, void* data);
bool cache_has_key(Cache* cache, char* key);
void* cache_get(Cache* cache, char* key);
void cache_delete(Cache* cache, char* key);
void cache_free(Cache* cache);

void free_list_for_idx(int idx);
CacheRecord* get_record(Cache* cache, char* key);
unsigned long get_hash(char* str);


Cache* new_cache(void) {
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
    
    unsigned long hash = get_hash(key);
    int i = hash & CACHE_LEN-1;

    CacheRecord* new_record = malloc(sizeof(CacheRecord));
    if (new_record == NULL) {
        return;
    }
    new_record->data = data;
    strcpy(new_record->key, key);

    if (cache->records[i] == NULL) {
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
    CacheRecord* record = get_record(cache, key);
    if (record == NULL) {
        return NULL;
    }
    return record->data;
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
    unsigned long hash = get_hash(key);
    int i = hash & CACHE_LEN-1;

    if (cache->records[i] == NULL) {
        return NULL;
    }
    if (!strcmp(cache->records[i]->key, key)) {
        return cache->records[i];
    }

    // ищем в списке
    CacheRecord* next = cache->records[i]->next;
    while (next) {
        if (next->key == key) {
            return next;
        }
        next = next->next;
    }

    return NULL;
}

unsigned long get_hash(char* str) {
    unsigned long hash = 1469598103934665603UL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211UL;
    }
    return hash;
}