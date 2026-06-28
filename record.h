#ifndef REGULARDB_RECORD_H
#define REGULARDB_RECORD_H

typedef struct {
    char* key;
    char* value;
} record;

int write_record(record r);
int read_record(record* buf);

#endif