#ifndef PAGE_CARRIER_H
#define PAGE_CARRIER_H

#include <stdbool.h>

#define PAGE_LEN 4096

typedef struct Page {
    char file_name[25];
    int page_num;
    char data[PAGE_LEN];
    bool changed;

    char page_cache_key[30];
    bool has_cache_key;
} Page;

Page* new_page(char* file_name);
Page* get_page(char* file_name, int page_num);
void save_page(Page* p);
void free_page(Page* p);

#endif