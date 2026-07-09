#include "storage.h"
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    TableInfo* table = malloc(sizeof(TableInfo)+sizeof(ColumnInfo)*1);
    if (table == NULL) {
        return 1;
    }
    StorageRecord* record = malloc(sizeof(StorageRecord));
    if (record == NULL) {
        return 1;
    }
    record->table = table;

    int res = storage_get_record(record);
    if (res != 0) {
        return 1;
    }

    int i;
    for (i = 0; i < record->data_len; i++) {
        printf("%c", record->data[i]);
    }
    printf("\n");

    return 0;
}