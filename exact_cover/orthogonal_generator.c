#include "incidence_matrix.h"
#include "exact_cover.h"

static void read_int(int *pn) {
    int res = scanf("%d", pn);
    if ((res != 1) || (*pn <= 0)) {
        printf("Size is incorrect\n");
        abort();
    }
}

int main() {
    struct array2d initialField, *incidenceMatrix;
    struct ArrayListOfArrays *list;
    int size;

    read_int(&size);
    init_array2d(&initialField, size, size);
    read_array2d(&initialField);
    incidenceMatrix = convert_latin_square_to_matrix(&initialField);
    list = find_exact_cover(incidenceMatrix);
    for (int i = 0; i < list->size; ++i) {
        struct array2d *lsquare;
        lsquare = convert_matrix_rows_to_latin_square(incidenceMatrix, list->arrays[i], size);

        printf("solution %d:\n", i + 1);
        print_array2d(lsquare); 
        free_array2d(lsquare);
        free(lsquare);
    }

    free_arraylist(list);
    free(list);
    free_array2d(incidenceMatrix);
    free_array2d(&initialField);
    free(incidenceMatrix);
    return 0;
}
