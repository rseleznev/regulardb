#include "page_carrier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct PageHeader {
    off_t upper_idx; // позиция свободного байта сверху для записи RecordInfo
    off_t lower_idx; // позиция свободного байта снизу для записи данных
} PageHeader;

typedef struct RecordInfo {
    off_t data_idx;
    size_t data_len;
    char status;
} RecordInfo;

typedef struct Record {
    int data_len;
    char data[];
} Record;

int create_file(char* file_name);
void init_page_header(Page* page);
PageHeader* read_page_header(Page* page);
void update_page_header(Page* page, PageHeader* header);

int append_record(char* data, int len, Page* page);
Record* read_first_record(Page* page);

void print_8_bits(char c);
void print_64_bits(long n);

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
    printf("header.upper_idx: %ld \n", hdr->upper_idx);
    printf("header.lower_idx: %ld \n", hdr->lower_idx);

    Record* record = read_first_record(page);
    if (record == NULL) {
        return 1;
    }

    int n;
    for (n = 0; n < record->data_len; n++) {
        printf("%c", record->data[n]);
    }
    printf("\n");

    // char* data = "random test value";
    // res = append_record(data, 17, page);
    // if (res != 0) {
    //     return 1;
    // }

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
    header.upper_idx = sizeof(PageHeader);
    header.lower_idx = PAGE_LEN-1;

    int i, shift;

    shift = 0;
    for (i = 0; i < sizeof(header.upper_idx); i++) {
        page->data[i] = (unsigned char)(header.upper_idx >> shift);
        shift += 8;
    }

    shift = 0;
    for (; i < sizeof(PageHeader); i++) {
        page->data[i] = (unsigned char)(header.lower_idx >> shift);
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

void update_page_header(Page* page, PageHeader* header) {
    int i, shift;

    shift = 0;
    for (i = 0; i < sizeof(header->upper_idx); i++) {
        page->data[i] = (unsigned char)(header->upper_idx >> shift);
        shift += 8;
    }

    shift = 0;
    for (; i < sizeof(PageHeader); i++) {
        page->data[i] = (unsigned char)(header->lower_idx >> shift);
        shift += 8;
    }
}

int append_record(char* data, int len, Page* page) {
    if (data == NULL) {
        printf("append_record: data is NULL \n");
        return -1;
    }
    if (page == NULL) {
        printf("append_record: page is NULL \n");
        return -1;
    }
    
    PageHeader* hdr = read_page_header(page);
    if (hdr == NULL) {
        printf("append_record: header is NULL \n");
        return -1;
    }

    if (hdr->upper_idx + len > hdr->lower_idx - len) {
        printf("append_record: not enough space for record \n");
        return -1;
    }

    // записываем RecordInfo
    RecordInfo record_info;
    record_info.data_idx = hdr->lower_idx - len;
    record_info.data_len = len;
    record_info.status = 'a';

    int i, shift;

    shift = 0;
    for (i = 0; i < sizeof(record_info.data_idx); i++) {
        page->data[hdr->upper_idx] = (unsigned char)(record_info.data_idx >> shift);
        shift += 8;
        hdr->upper_idx++;
    }

    shift = 0;
    for (i = 0; i < sizeof(record_info.data_len); i++) {
        page->data[hdr->upper_idx] = (unsigned char)(record_info.data_len >> shift);
        shift += 8;
        hdr->upper_idx++;
    }

    page->data[hdr->upper_idx] = record_info.status;
    hdr->upper_idx++;

    // записываем данные
    for (i = 0; i < len; i++) {
        page->data[hdr->lower_idx - len + i] = data[i];
    }

    // обновление индексов заголовка
    hdr->lower_idx = hdr->lower_idx - len - 1;
    update_page_header(page, hdr);

    return 0;
}

Record* read_first_record(Page* page) {
    if (page == NULL) {
        return NULL;
    }

    // читаем RecordInfo
    int i, shift;
    RecordInfo record_info;
    record_info.data_idx = 0;
    record_info.data_len = 0;

    shift = 0;
    for (i = sizeof(PageHeader); i < sizeof(PageHeader) + sizeof(record_info.data_idx); i++) {
        record_info.data_idx |= (off_t)(unsigned char)page->data[i] << shift;
        shift += 8;
    }

    shift = 0;
    for (; i < sizeof(PageHeader) + sizeof(record_info.data_idx) + sizeof(record_info.data_len); i++) {
        record_info.data_len |= (size_t)(unsigned char)page->data[i] << shift;
        shift += 8;
    }

    record_info.status = page->data[i];

    // читаем данные
    Record* record = malloc(sizeof(Record) + record_info.data_len);
    if (record == NULL) {
        return NULL;
    }

    int j;
    for (j = 0, i = record_info.data_idx; i < record_info.data_idx + record_info.data_len; i++) {
        record->data[j] = page->data[i];
    }
    record->data_len = record_info.data_len;

    return record;
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

void print_64_bits(long n) {
    int shift;
    for (shift = 63; shift >= 0; shift--) {
        if ((n >> shift) & 1 == 1) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    printf("\n");
}