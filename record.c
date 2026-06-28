#include "record.h"

int write_record(record r);
int read_record(record* buf);

int write_record(record r) {
    return 0;
}

int read_record(record* buf) {
    buf->key = "old_key";
    buf->value = "old_value";
    return 0;
}