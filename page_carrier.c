#include "page_carrier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    ERR_OPEN = -1,
    ERR_SEEK = -2,
    ERR_EOF = -3,
    ERR_READ = -4,
    ERR_WRITE = -5
} ErrorCode;

Page* get_page(char* file_name, int page_num);
int read_page(char* file_name, long start_pos, char buf[], size_t limit);
int write_page(char* file_name, long start_pos, char data[], size_t data_len);
void save_page(Page* page);

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
        /* Нужно подумать, как поступать в случае, когда указана несуществующая страница.
        В том числе, когда номер желаемой страницы сильно больше текущей последней страницы */
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
        return ERR_OPEN;
    }
    if (fseek(f, start_pos, SEEK_SET) != 0) {
        return ERR_SEEK;
    }
    if (feof(f)) {
        return ERR_EOF;
    }

    size_t n = fread(buf, 1, limit, f);
    if (n <= 0) {
        return ERR_READ;
    }
    fclose(f);
    
    return 0;
}

int write_page(char* file_name, long start_pos, char data[], size_t data_len) {
    FILE* f = fopen(file_name, "wb");
    if (!f) {
        return ERR_OPEN;
    }
    if (fseek(f, start_pos, SEEK_SET) != 0) {
        return ERR_SEEK;
    }

    size_t n = fwrite(data, 1, data_len, f);
    if (n != data_len) {
        return ERR_WRITE;
    }
    fclose(f);
    
    return 0;
}

void save_page(Page* page) {
    if (!page->changed) {
        return;
    }

    long start_pos;
    start_pos = page->page_num * PAGE_LEN - PAGE_LEN; // в будущем добавится смещение от заголовка файла

    int res = write_page(page->file_name, start_pos, page->data, PAGE_LEN);
    if (res != 0) {
        printf("page_carrier: save_page fail \n");
    }
}