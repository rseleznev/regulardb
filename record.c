#include "record.h"

int write_record(record r);
int read_record(record* buf);

int write_record(record r) {
    return 0;
}

int read_record(record* buf) {
    buf->key = "old_key";
    buf->key_len = 8;
    buf->value = "old_value";
    buf->value_len = 10;
    return 0;
}