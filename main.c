#include "catalog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
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

    return 0;
}