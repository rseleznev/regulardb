#ifndef REGULARDB_CATALOG_H
#define REGULARDB_CATALOG_H

typedef struct TableColumn {
    char name[25];
    char type[25];
    unsigned int value_len;
} TableColumn;

typedef struct TableInfo {
    char name[25];
    char file_name[25];
    unsigned int columns_len;
    TableColumn columns[];
} TableInfo;

typedef struct Catalog {
    unsigned int tables_len;
    TableInfo tables[];
} Catalog;

int create_catalog(void);
int append_catalog(TableInfo* table_info);
int get_catalog(Catalog* catalog);

#endif