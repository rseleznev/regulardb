#ifndef PAGE_CARRIER_H
#define PAGE_CARRIER_H

#include <stdbool.h>

#define PAGE_LEN 4096

typedef struct Page {
    char file_name[25];
    int page_num;
    char data[PAGE_LEN];
    bool changed;
} Page;

Page* get_page(char* file_name, int page_num);
void save_page(Page* p);

#endif