#include "page_carrier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct PageHeader {
    int upper_idx;
    int lower_idx;
} PageHeader;

typedef struct RecordInfo {
    int end_offset;
    size_t data_len;
    char status;
} RecordInfo;

int create_file(char* file_name);
void init_page_header(Page* page);
PageHeader* read_page_header(Page* page);

int main(void) {
    int res;
    // res = create_file("db/tables/users.rdb");
    // if (res != 0) {
    //     return 1;
    // }
    
    Page* page = get_page("db/tables/users.rdb", 1);
    if (page == NULL) {
        printf("creating new page... \n");
        page = new_page("db/tables/users.rdb");
        if (page == NULL) {
            return 1;    
        }
    }
    printf("page num: %d \n", page->page_num);

    PageHeader* hdr = read_page_header(page);
    if (hdr == NULL) {
        return 1;
    }
    printf("header.upper: %d \n", hdr->upper_idx);
    printf("header.lower: %d \n", hdr->lower_idx);

    // init_page_header(page);
    // page->changed = true;
    // save_page(page);

    return 0;
}

int create_file(char* file_name) {
    FILE* f = fopen("db/tables/users.rdb", "wb");
    if (!f) {
        return -1;
    }

    char zeros[PAGE_LEN] = {0};
    size_t n = fwrite(zeros, 1, PAGE_LEN, f);
    if (n != PAGE_LEN) {
        return -1;
    }

    return 0;
}

void init_page_header(Page* page) {
    PageHeader header;
    header.upper_idx = 0 + sizeof(PageHeader);
    header.lower_idx = PAGE_LEN-1;

    char upper1, upper2, upper3, upper4;
    char lower1, lower2, lower3, lower4;

    upper1 = (char)header.upper_idx;
    upper2 = (char)(header.upper_idx >> 8);
    upper3 = (char)(header.upper_idx >> 16);
    upper4 = (char)(header.upper_idx >> 24);
    page->data[0] = upper1;
    page->data[1] = upper2;
    page->data[2] = upper3;
    page->data[3] = upper4;

    lower1 = (char)header.lower_idx;
    lower2 = (char)(header.lower_idx >> 8);
    lower3 = (char)(header.lower_idx >> 16);
    lower4 = (char)(header.lower_idx >> 24);
    page->data[4] = lower1;
    page->data[5] = lower2;
    page->data[6] = lower3;
    page->data[7] = lower4;
}

PageHeader* read_page_header(Page* page) {
    PageHeader* header = malloc(sizeof(PageHeader));
    if (header == NULL) {
        return NULL;
    }

    header->upper_idx = page->data[0] | page->data[1] << 8 | page->data[2] << 16 | page->data[3] << 24;
    header->lower_idx = page->data[4] | page->data[5] << 8 | page->data[6] << 16 | page->data[7] << 24;

    return header;
}