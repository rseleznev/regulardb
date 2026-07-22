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

    char upper1, upper2, upper3, upper4, upper5, upper6, upper7, upper8;
    char lower1, lower2, lower3, lower4, lower5, lower6, lower7, lower8;

    upper1 = (char)header.upper_idx;
    upper2 = (char)(header.upper_idx >> 8);
    upper3 = (char)(header.upper_idx >> 16);
    upper4 = (char)(header.upper_idx >> 24);
    upper5 = (char)(header.upper_idx >> 32);
    upper6 = (char)(header.upper_idx >> 40);
    upper7 = (char)(header.upper_idx >> 48);
    upper8 = (char)(header.upper_idx >> 56);
    page->data[0] = upper1;
    page->data[1] = upper2;
    page->data[2] = upper3;
    page->data[3] = upper4;
    page->data[4] = upper5;
    page->data[5] = upper6;
    page->data[6] = upper7;
    page->data[7] = upper8;

    lower1 = (char)header.lower_idx;
    lower2 = (char)(header.lower_idx >> 8);
    lower3 = (char)(header.lower_idx >> 16);
    lower4 = (char)(header.lower_idx >> 24);
    lower5 = (char)(header.lower_idx >> 32);
    lower6 = (char)(header.lower_idx >> 40);
    lower7 = (char)(header.lower_idx >> 48);
    lower8 = (char)(header.lower_idx >> 56);
    page->data[8] = lower1;
    page->data[9] = lower2;
    page->data[10] = lower3;
    page->data[11] = lower4;
    page->data[12] = lower5;
    page->data[13] = lower6;
    page->data[14] = lower7;
    page->data[15] = lower8;
}

PageHeader* read_page_header(Page* page) {
    PageHeader* header = malloc(sizeof(PageHeader));
    if (header == NULL) {
        return NULL;
    }

    header->upper_idx = (off_t)(unsigned char)page->data[0] | (off_t)(unsigned char)page->data[1] << 8 |
        (off_t)(unsigned char)page->data[2] << 16 | (off_t)(unsigned char)page->data[3] << 24 |
        (off_t)(unsigned char)page->data[4] << 32 | (off_t)(unsigned char)page->data[5] << 40 |
        (off_t)(unsigned char)page->data[6] << 48 | (off_t)(unsigned char)page->data[7] << 56;
    header->lower_idx = (off_t)(unsigned char)page->data[8] | (off_t)(unsigned char)page->data[9] << 8 |
        (off_t)(unsigned char)page->data[10] << 16 | (off_t)(unsigned char)page->data[11] << 24 |
        (off_t)(unsigned char)page->data[12] << 32 | (off_t)(unsigned char)page->data[13] << 40 |
        (off_t)(unsigned char)page->data[14] << 48 | (off_t)(unsigned char)page->data[15] << 56;

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