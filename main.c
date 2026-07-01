#include "table.h"
#include <stdio.h>

int main() {
    int res = create_table("tables/test_table.rdb");
    if (res != 0) {
        printf("ошибка создания таблицы \n");
        return 0;
    }
    printf("таблица создана \n");

    return 0;
}