#include "catalog.h"
#include "page.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int create_catalog(void);
int append_catalog(TableInfo* table_info);
Catalog* get_catalog(void);
long get_file_len(FILE* f);

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
    n = fwrite();
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
Catalog* get_catalog(void) {
    // FILE* f = fopen("db/catalog.rdb", "rb");
    // if (f == NULL) {
    //     printf("get_catalog: failed to open catalog file \n");
    //     return NULL;
    // }
    // unsigned char buf[1024];
    // size_t n = fread(buf, 1, 1024, f);
    // if (n <= 0) {
    //     printf("get_catalog: no read bytes \n");
    //     fclose(f);
    //     return NULL;
    // }

    FileInfo* f = malloc(sizeof(FileInfo));
    if (f == NULL) {
        printf("get_catalog: failed memory alloc for FileInfo \n");
        fclose(f);
        return NULL;
    }
    strcpy(f->file_name, "db/catalog.rdb");

    Page* page = read_page(f);
    if (page == NULL) {
        printf("get_catalog: failed to read the page \n");
        fclose(f);
        return NULL;
    }

    long file_size = get_file_len(f);
    if (file_size <= 0) {
        printf("get_catalog: too short file len \n");
        fclose(f);
        return NULL;
    }

    Catalog* catalog = malloc((size_t)file_size);
    if (catalog == NULL) {
        printf("get_catalog: failed to cat_buf mem alloc \n");
        return NULL;
    }

    int i, t;
    for (i = 0, t = 0; i < n; i++, t++) {
        unsigned int table_columns_len;
        table_columns_len = (buf[53] << 24) | (buf[52] << 16) | (buf[51] << 8) | buf[50];    
        
        // TableInfo* table = malloc(sizeof(TableInfo) + sizeof(TableColumn)*table_columns_len);
        // if (table == NULL) {
        //     printf("get_catalog: failed to table mem alloc \n");
        //     return -1;
        // }

        int j;
        for (j = 0; j < 25; j++,i++) {
            // table->name[j] = buf[i];
            catalog->tables[t].name[j] = buf[i];
        }
        for (j = 0; j < 25; j++,i++) {
            // table->file_name[j] = buf[i];
            catalog->tables[t].file_name[j] = buf[i];
        }
        // table->columns_len = table_columns_len;
        catalog->tables[t].columns_len = table_columns_len;
        i += 4;

        int columns_filled = 0;
        while (columns_filled < table_columns_len) {
            for (j = 0; j < 25; j++,i++) {
                // table->columns[columns_filled].name[j] = buf[i];
                catalog->tables[t].columns[columns_filled].name[j] = buf[i];
            }
            for (j = 0; j < 25; j++,i++) {
                // table->columns[columns_filled].type[j] = buf[i];
                catalog->tables[t].columns[columns_filled].type[j] = buf[i];
            }
            unsigned int column_value_len;
            column_value_len = (buf[i+3] << 24) | (buf[i+2] << 16) | (buf[i+1] << 8) | buf[i];
            // table->columns[columns_filled].value_len = column_value_len;
            catalog->tables[t].columns[columns_filled].value_len = column_value_len;
            
            columns_filled++;
            i += 4;
        }
        catalog->tables_len++;
    }
    fclose(f);

    return catalog;
}

long get_file_len(FILE* f) {
    int fd = fileno(f);
    struct stat st;
    if (fstat(fd, &st) != 0) {
        return -1;
    }

    return st.st_size;
}