#ifndef REGULARDB_PAGE_H
#define REGULARDB_PAGE_H

#define PAGE_LEN 1024 * 4

#include <stdio.h>

typedef struct FileInfo {
    char* file_name;
} FileInfo;

typedef struct PageInfo {
    char data[PAGE_LEN];
    size_t data_len;
    FileInfo* file;
} PageInfo;

PageInfo* read_page(FileInfo*);
void free_page(PageInfo*);

#endif