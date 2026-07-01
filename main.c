#include "table.h"
#include <stdio.h>

int main() {
    char* path = "tables/test_table.rdb";
    
    int res = create_table(path);
    if (res != 0) {
        printf("ошибка создания таблицы \n");
        return 0;
    }
    printf("таблица создана \n");

    res = append_table(path, "test_data");
    if (res != 0) {
        printf("ошибка записи в таблицу \n");
        return 0;
    }
    printf("запись в таблицу успешна \n");

    char buf[1024];
    res = get_table_page(path, 1, buf);
    if (res != 0) {
        printf("ошибка чтения из таблицы \n");
        return 0;
    }
    printf("таблица прочитана: %s \n", buf);

    return 0;
}