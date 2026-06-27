typedef struct {
    char* key;
    char* value;
} record;

int write_record(record r);
int read_record(record* buf);

int main() {
    return 0;
}