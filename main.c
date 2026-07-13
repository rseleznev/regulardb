#include "page_carrier.h"
#include <stdio.h>
#include <string.h>

int create_file(char* file_name);

int main(void) {
    int res = create_file("db/tables/users.rdb");
    if (res != 0) {
        return 1;
    }
    
    Page* page = get_page("db/tables/users.rdb", 2);
    if (page == NULL) {
        page = new_page("db/tables/users.rdb");
        if (page == NULL) {
            return 1;    
        }
    }
    printf("page num: %d \n", page->page_num);

    int i;
    for (i = 0; i < 100; i++) {
        printf("%c", page->data[i]);
    }
    printf("\n");

    strcpy(page->data, "page 1 value");
    page->changed = true;
    save_page(page);

    /* Разобраться, почему удаляется страница 2, когда записываем страницу 1 */

    return 0;
}

int create_file(char* file_name) {
    FILE* f = fopen("db/tables/users.rdb", "wb");
    if (!f) {
        return -1;
    }

    char zeros[PAGE_LEN] = {0};
    size_t n = fwrite(zeros, 1, PAGE_LEN, f);
    if (n != PAGE_LEN) {
        return -1;
    }

    return 0;
}