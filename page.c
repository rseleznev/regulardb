#include "page.h"
#include <stdio.h>

PageInfo* read_page(FileInfo*);

PageInfo* read_page(FileInfo* f) {
    if (f == NULL) {
        return NULL;
    }
    
    FILE* f = fopen(f->file_name, "rb");
    if (f == NULL) {
        printf("read_page: failed to open file %s \n", f->file_name);
        return NULL;
    }

    PageInfo* page = malloc(sizeof(PageInfo));
    if (page == NULL) {
        printf("read_page: failed memory alloc for Page \n");
        fclose(f);
        return NULL;
    }
    page->file = f;

    size_t n = fread(page->data, 1, PAGE_LEN, f);
    if (n <= 0) {
        printf("read_page: no read bytes \n");
        fclose(f);
        return NULL;
    }
    page->data_len = n;

    return page;
}