#ifndef REGULARDB_CATALOG_H
#define REGULARDB_CATALOG_H

typedef struct TableColumn {
    char* name;
    char* type;
    unsigned int len;
} TableColumn;

typedef struct TableInfo {
    char* name;
    char* file_name;
    int columns_count;
    TableColumn columns[];
} TableInfo;

int create_catalog(void);
int append_catalog(TableInfo* table_info);
int get_catalog(TableInfo* buf);

#endif