#include "catalog.h"
#include <stdio.h>
#include <stdlib.h>

int create_catalog(void);
int append_catalog(TableInfo* table_info);
int get_catalog(Catalog* catalog);

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
    FILE* catalog = fopen("db/catalog.rdb", "ab");
    if (catalog == NULL) {
        printf("append_catalog: failed to open catalog file \n");
        return -1;
    }
    size_t n;

    n = fwrite(table_info->name, sizeof(table_info->name), 1, catalog);
    if (n <= 0) {
        printf("append_catalog: failed to write table name \n");
        return -1;
    }
    n = fwrite(table_info->file_name, sizeof(table_info->file_name), 1, catalog);
    if (n <= 0) {
        printf("append_catalog: failed to write table file_name \n");
        return -1;
    }
    n = fwrite(&table_info->columns_len, sizeof(table_info->columns_len), 1, catalog);
    if (n <= 0) {
        printf("append_catalog: failed to write table columns_len \n");
        return -1;
    }

    int i;
    for (i = 0; i < table_info->columns_len; i++) {
        n = fwrite(table_info->columns[i].name, sizeof(table_info->columns[i].name), 1, catalog);
        if (n <= 0) {
            printf("append_catalog: failed to write column name \n");
            return -1;
        }
        n = fwrite(table_info->columns[i].type, sizeof(table_info->columns[i].type), 1, catalog);
        if (n <= 0) {
            printf("append_catalog: failed to write column type \n");
            return -1;
        }
        n = fwrite(&table_info->columns[i].value_len, sizeof(table_info->columns[i].value_len), 1, catalog);
        if (n <= 0) {
            printf("append_catalog: failed to write column value_len \n");
            return -1;
        }
    }

    printf("append_catalog: catalog file appended \n");
    fclose(catalog);
    
    return 0;
}

// get_catalog возвращает весь каталог таблиц
int get_catalog(Catalog* cat_buf) {
    FILE* catalog = fopen("db/catalog.rdb", "rb");
    if (catalog == NULL) {
        printf("get_catalog: failed to open catalog file \n");
        return -1;
    }
    unsigned char buf[1024];
    size_t n = fread(buf, 1, 1024, catalog);
    if (n <= 0) {
        printf("get_catalog: no read bytes \n");
        return -1;
    }
    if (n < 108) {
        printf("get_catalog: too short value \n");
        return -1;
    }

    int i;
    for (i = 0; i < n; i++) {
        unsigned int table_columns_len;
        table_columns_len = (buf[53] << 24) | (buf[52] << 16) | (buf[51] << 8) | buf[50];    
        
        TableInfo* table = malloc(sizeof(TableInfo) + sizeof(TableColumn)*table_columns_len);
        if (table == NULL) {
            printf("ошибка выделения памяти для table \n");
            return -1;
        }

        int j;
        for (j = 0; j < 25; j++,i++) {
            table->name[j] = buf[i];
        }
        for (j = 0; j < 25; j++,i++) {
            table->file_name[j] = buf[i];
        }
        table->columns_len = table_columns_len;
        i += 4;

        int columns_filled = 0;
        while (columns_filled < table_columns_len) {
            for (j = 0; j < 25; j++,i++) {
                table->columns[columns_filled].name[j] = buf[i];
            }
            for (j = 0; j < 25; j++,i++) {
                table->columns[columns_filled].type[j] = buf[i];
            }
            unsigned int column_value_len;
            column_value_len = (buf[i+3] << 24) | (buf[i+2] << 16) | (buf[i+1] << 8) | buf[i];
            table->columns[columns_filled].value_len = column_value_len;
            
            columns_filled++;
            i += 4;
        }
        cat_buf->tables_len++;

        printf("Прочитанная таблица из каталога \n");
        printf("Table name: %s \n", table->name);
        printf("Table file name: %s \n", table->file_name);
        printf("Table columns len: %d \n", table->columns_len);
        printf("Table columns: \n");
        int k;
        for (k = 0; k < table->columns_len; k++) {
            printf("\t Column name: %s \n", table->columns[k].name);
            printf("\t Column type: %s \n", table->columns[k].type);
            printf("\t Column value len: %d \n", table->columns[k].value_len);
            printf("\n");
        }
    }

    return 0;
}