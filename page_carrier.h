#ifndef PAGE_CARRIER_H
#define PAGE_CARRIER_H

typedef struct Page {
    char file_name[25];
    int page_num;
    char data[4096];
    bool changed;
} Page;

Page* get_page(char* file_name, int page_num);

#endif