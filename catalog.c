#include "catalog.h"
#include <stdio.h>

int create_catalog(void);
int append_catalog(TableInfo* table_info);
int get_catalog(char* buf);

// create_catalog создает каталог таблиц со всей информацией о них
int create_catalog(void) {
    FILE* catalog = fopen("db/catalog.rdb", "w");
    if (catalog == NULL) {
        printf("create_catalog: failed to open catalog file \n");
        return -1;
    }
    printf("create_catalog: catalog file created \n");
    fclose(catalog);
    
    return 0;
}

// append_catalog добавляет в каталог таблиц информацию о новой таблице
int append_catalog(TableInfo* table_info) {
    FILE* catalog = fopen("db/catalog.rdb", "a");
    if (catalog == NULL) {
        printf("append_catalog: failed to open catalog file \n");
        return -1;
    }
    int n = fwrite(table_info, sizeof(*table_info), 1, catalog);
    if (n <= 0) {
        printf("append_catalog: failed to write catalog file \n");
        return -1;
    }
    printf("append_catalog: catalog file appended \n");
    fclose(catalog);
    
    return 0;
}

// get_catalog возвращает весь каталог таблиц
int get_catalog(char* buf) {
    FILE* catalog = fopen("db/catalog.rdb", "r");
    if (catalog == NULL) {
        printf("get_catalog: failed to open catalog file \n");
        return -1;
    }
    size_t n = fread(buf, sizeof(buf), 1, catalog);
     if (n <= 0) {
        printf("get_catalog: no read bytes \n");
        return -1;
    }
    
    return 0;
}