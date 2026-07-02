#ifndef REGULARDB_CATALOG_H
#define REGULARDB_CATALOG_H

typedef struct TableColomn {
    char* name;
    char* type;
    unsigned int len;
} TableColomn;

typedef struct TableInfo {
    char* name;
    char* file_name;
    int record_len;
    int pages_count;
    TableColomn colomns[];
} TableInfo;

int create_catalog(void);
int append_catalog(TableInfo* table_info);
int get_catalog(char* buf);

#endif