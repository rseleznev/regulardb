#ifndef REGULARDB_TABLE_H
#define REGULARDB_TABLE_H

typedef struct {
    char* name;
    char* type;
    unsigned int len;
} TableColomn;

typedef struct {
    char* name;
    char* file_name;
    int record_len;
    int pages_count;
    TableColomn colomns[];
} TableInfo;

int create_table_info(TableInfo* table_info);
int create_table(char* path);
int append_table(char* path, char* data);
int get_table_page(char* path, int page_num, char* buf);

#endif