#include "record.h"
#include <stdio.h>

int write_record(record r);
int read_record(record* buf);
char* file_name = "test_name_0001";

int write_record(record r) {
    FILE* file = fopen(file_name, "a");
    
    return 0;
}

int read_record(record* buf) {
    buf->key = "old_key";
    buf->key_len = 8;
    buf->value = "old_value";
    buf->value_len = 10;
    return 0;
}