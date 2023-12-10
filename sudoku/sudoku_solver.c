#include "sudoku.h"

int main() {
    struct array2d initialField, *filledField;

    init_array2d(&initialField, NINE, NINE);
    read_digits_to_array2d_stdin(&initialField);
    filledField = solve(&initialField);
    print_array2d(filledField);
    free_array2d(&initialField);
    free_array2d(filledField);
    free(filledField);
    return 0;
}
