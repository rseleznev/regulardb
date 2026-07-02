#include "table.h"
#include <stdio.h>

#define TABLE_PAGE_LEN 1024

int create_catalog(void);
int append_catalog(TableInfo* table_info);
int get_catalog(char* buf);
int create_table(char* path);
int append_table(char* path, char* data);
int get_table_page(char* path, int page_num, char* buf);

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

// create_table создает файл таблицы
int create_table(char* path) {
    FILE* table = fopen(path, "w");
    if (table == NULL) {
        printf("create_table: fail to open table file \n");
        return -1;
    }
    printf("create_table: table file created \n");
    fclose(table);
    
    return 0;
}

// append_table добавляет данные в файл таблицы
int append_table(char* path, char* data) {
    FILE* table = fopen(path, "a");
    if (table == NULL) {
        printf("append_table: fail to open table file \n");
        return -1;
    }
    size_t n = fwrite(data, sizeof(data), 1, table);
    if (n <= 0) {
        printf("append_table: no written bytes \n");
        return -1;
    }
    printf("append_table: table file appended \n");
    fclose(table);
    
    return 0;
}

// get_table_page читает целую страницу из файла таблицы
int get_table_page(char* path, int page_num, char* buf) {
    FILE* table = fopen(path, "r");
    if (table == NULL) {
        printf("get_table_page: fail to open table file \n");
        return -1;
    }
    size_t n = fread(buf, sizeof(buf), 1, table);
    if (n <= 0) {
        printf("get_table_page: no read bytes \n");
        return -1;
    }
    printf("get_table_page: table file read \n");
    fclose(table);

    return 0;
}