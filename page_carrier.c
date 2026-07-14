#include "page_carrier.h"
#include "cache.h"
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

Page* new_page(char* file_name);
Page* get_page(char* file_name, int page_num);
void save_page(Page* page);

int read_page_from_file(FILE* f, long page_start_pos, char buf[], size_t limit);
int write_page_to_file(FILE* f, long page_start_pos, char data[], size_t data_len);
long get_file_len(FILE* f);
int count_pages(FILE* f);

static Cache* cache;

/* Логика работы со страницами:

    Перед любым вызовом страницы должен быть создан сам файл.
    Новую страницу необходимо создать через new_page, странице присвоится свободный порядковый номер.
    get_page возвращает только уже созданные страницы, при запросе несуществующей страницы вернется NULL.
    save_page сохраняет только уже созданные страницы, о которых знает page_carrier
*/

Page* new_page(char* file_name) {
    FILE* f = fopen(file_name, "r+b");
    if (!f) {
        // файл должен быть создан отдельно
        printf("page_carrier: new_page open file fail \n");
        return NULL;
    }
    
    Page* page = malloc(sizeof(Page));
    if (page == NULL) {
        printf("page_carrier: new_page mem alloc fail \n");
        fclose(f);
        return NULL;
    }

    int pages_counted = count_pages(f);
    if (pages_counted < 0) {
        printf("page_carrier: new_page pages count fail \n");
        fclose(f);
        return NULL;
    }
    strcpy(page->file_name, file_name);
    page->changed = false;
    page->page_num = ++pages_counted;

    // заполняем страницу нулями на диске
    char zeros[PAGE_LEN] = {0};
    long start_pos;
    start_pos = FILE_HDR_LEN + page->page_num * PAGE_LEN - PAGE_LEN;
    int res = write_page_to_file(f, start_pos, zeros, PAGE_LEN);
    if (res != 0) {
        printf("page_carrier: new_page write page fail \n");
        fclose(f);
        return NULL;
    }

    fclose(f);

    if (cache == NULL) {
        cache = new_cache();
        if (cache == NULL) {
            printf("page_carrier: new_page new cache fail \n");
            return page;
        }
    }
    char* page_cache_key = page->file_name; // + page_num
    cache_replace(cache, page_cache_key, page);

    return page;
}

Page* get_page(char* file_name, int page_num) {
    // сначала проверяем кеш
    if (cache == NULL) {
        cache = new_cache();
        if (cache == NULL) {
            printf("page_carrier: get_page new cache fail \n");
            goto from_file; // придумать что-то получше
        }
    }
    char* page_cache_key = file_name; // + page_num
    Page* page = (Page*)cache_get(cache, page_cache_key);
    if (page != NULL) {
        return page;
    }

    // иначе читаем с диска
from_file:
    FILE* f = fopen(file_name, "r+b");
    if (!f) {
        printf("page_carrier: get_page open file fail \n");
        /* Если страницы нет ни в кеше, ни на диске (в том числе самого файла), ее сначала надо создать через new_page */
        return NULL;
    }

    int pages_counted = count_pages(f);
    if (pages_counted < page_num) {
        printf("page_carrier: get_page less pages counted then desired \n");
        fclose(f);
        return NULL;
    }

    page = malloc(sizeof(Page));
    if (page == NULL) {
        printf("page_carrier: get_page mem alloc fail \n");
        fclose(f);
        return NULL;
    }

    long start_pos;
    start_pos = FILE_HDR_LEN + page_num * PAGE_LEN - PAGE_LEN;

    int res = read_page_from_file(f, start_pos, page->data, PAGE_LEN);
    if (res != 0) {
        printf("page_carrier: get_page read page fail \n");
        fclose(f);
        return NULL;
    }

    strcpy(page->file_name, file_name);
    page->page_num = page_num;
    page->changed = false;

    fclose(f);

    if (cache == NULL) {
        cache = new_cache();
        if (cache == NULL) {
            printf("page_carrier: get_page new cache fail \n");
            return page;
        }
    }
    cache_replace(cache, page_cache_key, page);

    return page;
}

void save_page(Page* page) {
    // сначала проверяем страницу в кеше, если в кеше нет - конец
    if (cache == NULL) {
        cache = new_cache();
        if (cache == NULL) {
            printf("page_carrier: save_page new cache fail \n");
            return;
        }
    }
    char* page_cache_key = page->file_name; // + page_num
    Page* page = (Page*)cache_get(cache, page_cache_key);
    if (page == NULL) {
        printf("page_carrier: save_page no such page in cache \n");
        return;
    }
    
    if (!page->changed) {
        printf("page_carrier: save_page no changes to save \n");
        return;
    }

    FILE* f = fopen(page->file_name, "r+b");
    if (!f) {
        printf("page_carrier: save_page open file fail \n");
        return;
    }

    long start_pos;
    start_pos = FILE_HDR_LEN + page->page_num * PAGE_LEN - PAGE_LEN;

    int res = write_page_to_file(f, start_pos, page->data, PAGE_LEN);
    if (res != 0) {
        printf("page_carrier: save_page write page fail \n");
    }

    fclose(f);
}

int read_page_from_file(FILE* f, long page_start_pos, char buf[], size_t limit) {
    if (get_file_len(f)-1 < page_start_pos) {
        printf("page_carrier: read_page_from_file start pos out of range \n");
        return ERR_NO_SUCH_PAGE;
    }
    if (fseek(f, page_start_pos, SEEK_SET) != 0) {
        printf("page_carrier: read_page_from_file fseek fail \n");
        return ERR_SEEK;
    }
    if (feof(f)) {
        printf("page_carrier: read_page_from_file EOF fail \n");
        return ERR_EOF;
    }

    size_t n = fread(buf, 1, limit, f);
    if (n <= 0) {
        printf("page_carrier: read_page_from_file read fail \n");
        return ERR_READ;
    }
    
    return 0;
}

int write_page_to_file(FILE* f, long page_start_pos, char data[], size_t data_len) {
    if (fseek(f, page_start_pos, SEEK_SET) != 0) {
        printf("page_carrier: write_page_to_file fseek fail \n");
        return ERR_SEEK;
    }

    size_t n = fwrite(data, 1, data_len, f);
    if (n != data_len) {
        printf("page_carrier: write_page_to_file write fail \n");
        return ERR_WRITE;
    }
    
    return 0;
}

long get_file_len(FILE* f) {
    int fd = fileno(f);
    struct stat st;
    if (fstat(fd, &st) != 0) {
        printf("page_carrier: get_file_len stat fail \n");
        return -1;
    }

    return st.st_size;
}

int count_pages(FILE* f) {
    long file_len = get_file_len(f);
    if (file_len == -1) {
        printf("page_carrier: count_pages file len fail \n");
        return -1;
    }

    return (file_len - FILE_HDR_LEN) / PAGE_LEN;
}