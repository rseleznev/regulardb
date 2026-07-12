#include "page_carrier.h"
#include <stdio.h>

int main(void) {
    Page* page = get_page("db/catalog.rdb", 1);
    if (page == NULL) {
        return 1;
    }

    int i;
    for (i = 0; i < 100; i++) {
        printf("%c", page->data[i]);
    }
    printf("\n");

    return 0;
}