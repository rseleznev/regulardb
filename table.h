#ifndef REGULARDB_TABLE_H
#define REGULARDB_TABLE_H

int create_table(char* path);
int append_table(char* path, char* data);
int get_table_page(char* path, int page_num, char* buf);

#endif