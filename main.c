#include "page_carrier.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    Page* page = get_page("db/tables/users.rdb", 1);
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

    strcpy(page->data, "test value");
    page->changed = true;
    save_page(page);

    return 0;
}