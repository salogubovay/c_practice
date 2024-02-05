#include "exact_cover.h"

static void read_int(int * pn) {
    int res = scanf("%d", pn);
    if (res != 1) {
        printf("Number has not been recognized");
        abort();
    }
}

int main() {
    int rows, columns;
    struct array2d initialMatrix;
    struct ArrayListOfArrays *list;
    read_int(&rows);
    read_int(&columns); 
    init_array2d(&initialMatrix, rows, columns);
    read_array2d(&initialMatrix);
    list = find_exact_cover(&initialMatrix);
    printf("Solutions found: %d\n", list->size);
    print_arraylist(list);
    free_array2d(&initialMatrix);
    free_arraylist(list);
    free(list);
}
