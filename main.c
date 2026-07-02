#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    TableInfo* table_info = malloc(sizeof(TableInfo) + sizeof(TableColomn)*1);
    if (table_info == NULL) {
        printf("ошибка выделения памяти \n");
        return 1;
    }

    table_info->name = strdup("test_table");
    table_info->file_name = strdup("test_table.rdb");
    table_info->record_len = 64;
    table_info->pages_count = 1;

    table_info->colomns[0].name = strdup("test_key");
    table_info->colomns[0].type = strdup("string");
    table_info->colomns[0].len = 64;

    int res = append_catalog(table_info);
    if (res != 0) {
        printf("ошибка создания инфо таблицы \n");
        return 1;
    }
    printf("инфо таблица создана \n");

    free(table_info->colomns[0].name);
    free(table_info->colomns[0].type);

    free(table_info->name);
    free(table_info->file_name);
    free(table_info);

    char buf[1024];
    res = get_catalog(buf);
    if (res != 0) {
        printf("ошибка получения инфо таблицы \n");
        return 1;
    }
    printf("инфо таблица получена: %s \n", buf);
    
    char* path = "db/tables/test_table.rdb";
    
    res = create_table(path);
    if (res != 0) {
        printf("ошибка создания таблицы \n");
        return 1;
    }
    printf("таблица создана \n");

    res = append_table(path, "test_data");
    if (res != 0) {
        printf("ошибка записи в таблицу \n");
        return 1;
    }
    printf("запись в таблицу успешна \n");

    res = get_table_page(path, 1, buf);
    if (res != 0) {
        printf("ошибка чтения из таблицы \n");
        return 1;
    }
    printf("таблица прочитана: %s \n", buf);

    return 0;
}