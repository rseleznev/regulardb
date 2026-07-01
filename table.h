#ifndef REGULARDB_TABLE_H
#define REGULARDB_TABLE_H

typedef struct {
    unsigned int key_len;
    unsigned int value_len;
    char* key;
    char* value;
} record;

int create_table(char* path);
int append_table(char* path, char* data);
int get_table_page(char* path, int page_num, char* buf);

#endif