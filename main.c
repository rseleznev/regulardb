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
    Catalog* catalog = malloc(sizeof(Catalog)); // разобраться с выделением памяти
    res = get_catalog(catalog);
    if (res != 0) {
        return 1;
    }

    int n;
    for (n = 0; n < catalog->tables_len; n++) {
        printf("Прочитанная таблица из каталога \n");
        printf("Table name: %s \n", catalog->tables[n].name);
        printf("Table file name: %s \n", catalog->tables[n].file_name);
        printf("Table columns len: %d \n", catalog->tables[n].columns_len);
        printf("Table columns: \n");
        int k;
        for (k = 0; k < catalog->tables[n].columns_len; k++) {
            printf("\t Column name: %s \n", catalog->tables[n].columns[k].name);
            printf("\t Column type: %s \n", catalog->tables[n].columns[k].type);
            printf("\t Column value len: %d \n", catalog->tables[n].columns[k].value_len);
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}