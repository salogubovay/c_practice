#pragma once
#include <stdio.h>
#include "../array/array.h"
#include "../graeco_latin_squares/latin_squares.h"

struct array2d * convert_latin_square_to_matrix(const struct array2d *lsquare);
struct array2d * convert_matrix_rows_to_latin_square(const struct array2d *matrix, const struct array *rows, int size);
