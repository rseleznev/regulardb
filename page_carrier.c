#include "page_carrier.h"
#include <stdlib.h>
#include <stdio.h>

Page* get_page(char* file_name, int page_num);
int read_page(char* file_name, long start_pos, char buf[], size_t limit);

Page* get_page(char* file_name, int page_num) {
    // сначала проверяем кеш
    // если есть в кеше - отдаем его

    // иначе читаем с диска
    Page* page = malloc(sizeof(Page));
    if (page == NULL) {
        return NULL;
    }

    // рассчитываем start_pos
    long start_pos;

    int res = read_page(file_name, start_pos, page->data, 4096);
    if (res != 0) {
        return NULL;
    }

    return page;
}

int read_page(char* file_name, long start_pos, char buf[], size_t limit) {
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        return -1;
    }
    if (fseek(f, start_pos, SEEK_SET) != 0) {
        return -1;
    }

    size_t n = fread(buf, 1, limit, f);
    if (n <= 0) {
        return -1;
    }
    fclose(f);
    
    return 0;
}