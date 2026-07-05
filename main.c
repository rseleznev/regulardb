#include "catalog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // создание каталога
    int res = create_catalog();
    if (res != 0) {
        return 1;
    }

    TableInfo* table_info = malloc(sizeof(TableInfo) + sizeof(TableColumn)*1);
    if (table_info == NULL) {
        printf("ошибка выделения памяти для table_info \n");
        return 1;
    }
    table_info->name = strdup("test_table");
    table_info->file_name = strdup("test_table.rdb");
    table_info->columns_count = 1;
    table_info->columns[0].name = strdup("event");
    table_info->columns[0].type = strdup("integer");
    table_info->columns[0].len = 8;

    res = append_catalog(table_info);
    if (res != 0) {
        return 1;
    }

    free(table_info->columns[0].name);
    free(table_info->columns[0].type);

    free(table_info->name);
    free(table_info->file_name);

    free(table_info);

    // чтение каталога
    TableInfo* buf = malloc(sizeof(TableInfo) + sizeof(TableColumn)*1);
    if (buf == NULL) {
        printf("ошибка выделения памяти для buf \n");
        return 1;
    }
    buf->name = malloc(100);
    buf->file_name = malloc(100);
    buf->columns[0].name = malloc(100);
    buf->columns[0].type = malloc(100);

    res = get_catalog(buf);
    if (res != 0) {
        return 1;
    }
    printf("name: %s \n", buf->name);
    printf("file_name: %s \n", buf->file_name);
    printf("columns_count: %d \n", buf->columns_count);
    printf("columns: \n");
    printf("\t name: %s \n", buf->columns[0].name);
    printf("\t type: %s \n", buf->columns[0].type);
    printf("\t len: %d \n", buf->columns[0].len);

    free(buf->columns[0].name);
    free(buf->columns[0].type);
    free(buf->name);
    free(buf->file_name);
    free(buf);

    return 0;
}