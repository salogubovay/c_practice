#include "sudoku.h"

static bool find_solution(struct array2d *inF, struct array2d *fF, struct array2d *usedDR, struct array2d *usedDC, struct array2d *usedDS, int pos) {
    if (pos == (NINE * NINE)) {
        return true;
    }

    int row = pos / NINE;
    int column = pos % NINE;
    int square = (row / 3) * 3 + (column / 3);
    
    if (inF->a[row][column] != 0) {
        return find_solution(inF, fF, usedDR, usedDC, usedDS, pos + 1);
    }

    for (int d = 1; d <= 9; ++d) {
        if ((usedDR->a[row][d] == 0) && (usedDC->a[column][d] == 0) && (usedDS->a[square][d] == 0)) {
            usedDR->a[row][d] = 1;
            usedDC->a[column][d] = 1;
            usedDS->a[square][d] = 1;
            fF->a[row][column] = d;
            if (find_solution(inF, fF, usedDR, usedDC, usedDS, pos + 1)) {
                return true; 
            } else {
                usedDR->a[row][d] = 0;
                usedDC->a[column][d] = 0;
                usedDS->a[square][d] = 0;
                fF->a[row][column] = 0;
            }
        }
    }
    return false;
}

static void fill_used_digits(struct array2d *inF, struct array2d *uDR, struct array2d *uDC, struct array2d *uDS) {
    for (int i = 0; i < inF->r; ++i) {
        for (int j = 0; j < inF->c; ++j) {
            int d = inF->a[i][j];
            if (d != 0) {
                uDR->a[i][d] = 1;
                uDC->a[j][d] = 1;
                uDS->a[(i / 3) * 3 + (j / 3)][d] = 1;
            }
        }
    }
}

struct array2d * solve(struct array2d *initialField) {
    struct array2d usedDigitsRows, usedDigitsColumns, usedDigitsSquares, *filledField;
    init_array2d(&usedDigitsRows, NINE, TEN);
    init_array2d(&usedDigitsColumns, NINE, TEN);
    init_array2d(&usedDigitsSquares, NINE, TEN);
    filledField = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(filledField, initialField->r, initialField->c);

    if((initialField->r != NINE) || (initialField->c != NINE)) {
        printf("Initial field size is incorrect (shoul be %d): rows = %d, columns = %d\n", NINE, initialField->r, initialField->c);
        abort();
    }
    
    copy_array2d(initialField, filledField);
    fill_used_digits(initialField, &usedDigitsRows, &usedDigitsColumns, &usedDigitsSquares);
    find_solution(initialField, filledField, &usedDigitsRows, &usedDigitsColumns, &usedDigitsSquares, 0);

    free_array2d(&usedDigitsRows);
    free_array2d(&usedDigitsColumns);
    free_array2d(&usedDigitsSquares);
    return filledField;
}

