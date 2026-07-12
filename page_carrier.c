#include "page_carrier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Page* get_page(char* file_name, int page_num);
int read_page(char* file_name, long start_pos, char buf[], size_t limit);
int write_page(char* file_name, long start_pos, char data[], size_t data_len);

Page* get_page(char* file_name, int page_num) {
    // сначала проверяем кеш
    // если есть в кеше - отдаем его

    // иначе читаем с диска
    Page* page = malloc(sizeof(Page));
    if (page == NULL) {
        return NULL;
    }
    strcpy(page->file_name, file_name);
    page->page_num = page_num;
    page->changed = false;

    long start_pos;
    start_pos = page_num * PAGE_LEN - PAGE_LEN; // в будущем добавится смещение от заголовка файла

    int res = read_page(file_name, start_pos, page->data, PAGE_LEN);
    if (res != 0) {
        res = write_page(file_name, start_pos, page->data, PAGE_LEN);
        if (res != 0) {
            return NULL;
        }
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

int write_page(char* file_name, long start_pos, char data[], size_t data_len) {
    FILE* f = fopen(file_name, "wb");
    if (!f) {
        return -1;
    }
    if (fseek(f, start_pos, SEEK_SET) != 0) {
        return -1;
    }

    size_t n = fwrite(data, 1, data_len, f);
    if (n != data_len) {
        return -1;
    }
    fclose(f);
    
    return 0;
}