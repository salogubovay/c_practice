#include "incidence_matrix.h"

static void check_latin_square(const struct array2d *lsquare) {
    if (!is_disordered_square(lsquare)) {
        printf("Input square is not latin.\n"); 
        abort();
    }
}

static void subtract_one(const struct array2d *lsquare) {
    int n = lsquare->r;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            --lsquare->a[i][j];
        }
    }
}

static void add_one(const struct array2d *lsquare) {
    int n = lsquare->r;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ++lsquare->a[i][j];
        }
    }
}

static struct array2d * generate_matrix(const struct array2d *lsquare) {
    struct array2d *matrix;
    int n, n2, rows, columns, curRow;

    n = lsquare->r;
    n2 = n * n;
    matrix = (struct array2d *) calloc(1, sizeof(struct array2d));
    rows = n * n * n  - (n2 - n); 
    columns = 4 * n2;
    init_array2d(matrix, rows, columns);
    curRow = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (((i == k) && (j == 0)) || (i != k)) {
                    int r, c, x, y, num; //r(i,k), c(j,k), x(i,j), y(a,k) - columns indices
                    num = lsquare->a[i][j];
                    r = i * n + k;
                    c = n2 + j * n + k; 
                    x = 2 * n2 + i * n + j;
                    y = 3 * n2 + num * n + k; 
                    matrix->a[curRow][r] = 1;
                    matrix->a[curRow][c] = 1;
                    matrix->a[curRow][x] = 1;
                    matrix->a[curRow][y] = 1;
                    ++curRow; 
                }
            }
        }
    }

    return matrix;
}

struct array2d * convert_latin_square_to_matrix(const struct array2d *lsquare) {
    struct array2d *matrix;
    check_latin_square(lsquare);    
    subtract_one(lsquare); 
    matrix = generate_matrix(lsquare);
    add_one(lsquare);
    return  matrix;  
}

static int find_one(const int *a, int start, int end) {
    int idx =-1;

    for (int i = 0; i + start < end; ++i) {
        if (a[start + i] == 1) {
            idx = i;
            break;
        }
    }
    return idx; 
}

struct array2d * convert_matrix_rows_to_latin_square(const struct array2d *matrix, const struct array *rows, int size) {
    struct array2d *lsquare;
    int n, n2;
    n = size;
    n2 = n * n;
    lsquare = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(lsquare, n, n);

    for (int i = 0; i < rows->size; ++i) {
        int li, lj, lk, mrow, pos;
        mrow = rows->a[i];
        if (mrow >= matrix->r) {
            printf("matrix does not contain row = %d\n", mrow);
            abort();
        }
        pos = find_one(matrix->a[mrow], 0, n2);

        li = pos / n;
        lk = pos % n;
        pos = find_one(matrix->a[mrow], n2, 2 * n2);
        lj = pos / n;
        lsquare->a[li][lj] = lk; 
    }
    add_one(lsquare);
    return lsquare;
}
