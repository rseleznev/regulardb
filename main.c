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

    TableInfo* table_info = malloc(sizeof(TableInfo) + sizeof(TableColumn)*2);
    if (table_info == NULL) {
        printf("ошибка выделения памяти для table_info \n");
        return 1;
    }
    strcpy(table_info->name, "test_table");
    strcpy(table_info->file_name, "test_table");
    table_info->columns_len = 2;
    strcpy(table_info->columns[0].name, "event");
    strcpy(table_info->columns[0].type, "integer");
    table_info->columns[0].value_len = 8;
    strcpy(table_info->columns[1].name, "mode");
    strcpy(table_info->columns[1].type, "string");
    table_info->columns[1].value_len = 12;

    res = append_catalog(table_info);
    if (res != 0) {
        return 1;
    }

    free(table_info);

    // чтение каталога
    Catalog catalog;
    res = get_catalog(&catalog);
    if (res != 0) {
        return 1;
    }
    printf("Кол-во таблиц в каталоге: %d \n", catalog.tables_len);

    return 0;
}