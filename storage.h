#ifndef REGULARDB_STORAGE_H
#define REGULARDB_STORAGE_H

#include "table.h"

typedef struct StorageRecord {
    TableInfo* table;
    char* data;
    int data_len;
} StorageRecord;

int storage_get_record(StorageRecord* record);

#endif