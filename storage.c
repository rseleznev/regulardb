#include "storage.h"
#include <stdlib.h>

int storage_get_record(StorageRecord* record);

int storage_get_record(StorageRecord* record) {
    if (record == NULL) {
        return -1;
    }
    
    record->data = "test_value";
    record->data_len = 11;
    
    return 0;
}