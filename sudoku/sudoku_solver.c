#include "sudoku.h"

int main() {
    struct array2d initialField;
    init_array2d(&initialField, NINE, NINE);
    read_digits_to_array2d_stdin(&initialField);
    solve(&initialField);
    free_array2d(&initialField);
    return 0;
}
