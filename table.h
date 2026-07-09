#ifndef REGULARDB_TABLE_H
#define REGULARDB_TABLE_H

typedef struct ColumnInfo {
    char name[25];
    char type[25];
    unsigned int value_len;
} ColumnInfo;

typedef struct TableInfo {
    char name[25];
    char file_name[25];
    unsigned int columns_len;
    ColumnInfo columns[];
} TableInfo;

#endif