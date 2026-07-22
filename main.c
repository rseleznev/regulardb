#include "page_carrier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct PageHeader {
    off_t upper_idx;
    off_t lower_idx;
} PageHeader;

typedef struct RecordInfo {
    off_t end_offset;
    size_t data_len;
    char status;
} RecordInfo;

int create_file(char* file_name);
void init_page_header(Page* page);
PageHeader* read_page_header(Page* page);
void print_8_bits(char c);
void print_64_bits(off_t o);

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
    printf("header.upper: %ld \n", hdr->upper_idx);
    printf("header.lower: %ld \n", hdr->lower_idx);

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
    header.upper_idx = 0 + sizeof(header);
    header.lower_idx = PAGE_LEN-1;

    int i, shift;

    shift = 0;
    for (i = 0; i < sizeof(header.upper_idx); i++) {
        page->data[i] = (char)(header.upper_idx >> shift);
        shift += 8;
    }

    shift = 0;
    for (; i < sizeof(PageHeader); i++) {
        page->data[i] = (char)(header.lower_idx >> shift);
        shift += 8;
    }
}

PageHeader* read_page_header(Page* page) {
    PageHeader* header = malloc(sizeof(PageHeader));
    if (header == NULL) {
        return NULL;
    }

    int i, shift;

    shift = 0;
    for (i = 0; i < sizeof(header->upper_idx); i++) {
        header->upper_idx |= (off_t)(unsigned char)page->data[i] << shift;
        shift += 8;
    }

    shift = 0;
    for (; i < sizeof(PageHeader); i++) {
        header->lower_idx |= (off_t)(unsigned char)page->data[i] << shift;
        shift += 8;
    }

    return header;
}

void print_8_bits(char c) {
    int shift;
    for (shift = 7; shift >= 0; shift--) {
        if ((c >> shift) & 1 == 1) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    printf("\n");
}

void print_64_bits(off_t o) {
    int shift;
    for (shift = 63; shift >= 0; shift--) {
        if ((o >> shift) & 1 == 1) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    printf("\n");
}