#include "record.h"
#include <stdio.h>

int main() {
    record new_record;
    new_record.key = "new_key";
    new_record.key_len = 8;
    new_record.value = "new_value";
    new_record.value_len = 10;

    int res;
    res = write_record(new_record);
    if (res != 0) {
        printf("ошибка сохранения записи \n");
        return 1;
    }
    printf("запись сохранена; key: %s, value: %s \n", new_record.key, new_record.value);

    record old_record;
    res = read_record(&old_record);
    if (res != 0) {
        printf("ошибка чтения записи \n");
        return 1;
    }
    printf("запись прочитана; key: %s, value: %s \n", old_record.key, old_record.value);

    return 0;
}