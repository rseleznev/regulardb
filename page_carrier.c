#include "page_carrier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_HDR_LEN 0

typedef enum {
    ERR_OPEN = -2,
    ERR_NO_SUCH_PAGE = -3,
    ERR_SEEK = -4,
    ERR_EOF = -5,
    ERR_READ = -6,
    ERR_WRITE = -7
} ErrorCode;

Page* get_page(char* file_name, int page_num);
int read_page(char* file_name, long start_pos, char buf[], size_t limit);
int write_page(char* file_name, long start_pos, char data[], size_t data_len);
void save_page(Page* page);
long get_file_len(FILE* f);
int count_pages(char* file_name);

Page* get_page(char* file_name, int page_num) {
    // сначала проверяем кеш
    // если есть в кеше - отдаем его

    // иначе читаем с диска
    Page* page = malloc(sizeof(Page));
    if (page == NULL) {
        printf("page_carrier: get_page mem alloc fail \n");
        return NULL;
    }
    strcpy(page->file_name, file_name);
    page->page_num = page_num;
    page->changed = false;

    long start_pos;
    start_pos = FILE_HDR_LEN + page_num * PAGE_LEN - PAGE_LEN;

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
        printf("page_carrier: read_page open file fail \n");
        return ERR_OPEN;
    }
    if (get_file_len(f) < start_pos) {
        printf("page_carrier: read_page start pos out of range \n");
        fclose(f);
        return ERR_NO_SUCH_PAGE;
    }
    if (fseek(f, start_pos, SEEK_SET) != 0) {
        printf("page_carrier: read_page fseek fail \n");
        fclose(f);
        return ERR_SEEK;
    }
    if (feof(f)) {
        printf("page_carrier: read_page EOF fail \n");
        fclose(f);
        return ERR_EOF;
    }

    size_t n = fread(buf, 1, limit, f);
    if (n <= 0) {
        printf("page_carrier: read_page read fail \n");
        fclose(f);
        return ERR_READ;
    }
    fclose(f);
    
    return 0;
}

int write_page(char* file_name, long start_pos, char data[], size_t data_len) {
    FILE* f = fopen(file_name, "wb");
    if (!f) {
        printf("page_carrier: write_page open file fail \n");
        return ERR_OPEN;
    }
    if (get_file_len(f) < start_pos) {
        printf("page_carrier: write_page start pos out of range \n");
        fclose(f);
        return ERR_NO_SUCH_PAGE;
    }
    if (fseek(f, start_pos, SEEK_SET) != 0) {
        printf("page_carrier: write_page fseek fail \n");
        fclose(f);
        return ERR_SEEK;
    }

    size_t n = fwrite(data, 1, data_len, f);
    if (n != data_len) {
        printf("page_carrier: write_page write fail \n");
        fclose(f);
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
    start_pos = FILE_HDR_LEN + page->page_num * PAGE_LEN - PAGE_LEN;

    int res = write_page(page->file_name, start_pos, page->data, PAGE_LEN);
    if (res != 0) {
        if (res == ERR_NO_SUCH_PAGE) {
            int pages_counted = count_pages(page->file_name);
            if (pages_counted < 0) {
                printf("page_carrier: save_page fail \n");
                return;
            }

            page->page_num = pages_counted + 1;
            return save_page(page);
        }
    }
}

long get_file_len(FILE* f) {
    int fd = fileno(f);
    struct stat st;
    if (fstat(fd, &st) != 0) {
        return -1;
    }

    return st.st_size;
}

int count_pages(char* file_name) {
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        printf("page_carrier: count_pages open file fail \n");
        return ERR_OPEN;
    }
    
    long file_len = get_file_len(f);
    if (file_len == -1) {
        printf("page_carrier: count_pages file len fail \n");
        fclose(f);
        return -1;
    }
    fclose(f);

    return (file_len - FILE_HDR_LEN) / PAGE_LEN;
}