#include "sudoku.h"

int MAX_TRANS_COUNT = 46656; //max count of transversals

static void generate_transversals(struct array2d *tF, struct array2d *fF, struct array2d *uDR, struct array2d *uDC, struct array2d *uDS, struct array2d **trans, struct array *transCount, int d,  int square) {
    int row, column;
    if (square == NINE) {
        struct array2d nextTrans;
        init_array2d(&nextTrans, tF->r, tF->c);
        transCount->a[d]++;
        trans[d][transCount->a[d] - 1] = nextTrans; 
        copy_array2d(tF, &nextTrans);
        return;
    }
   
    if (uDS->a[square][d] == 1) {
        return generate_transversals(tF, fF, uDR, uDC, uDS, trans, transCount, d, square + 1);
    } 

    for (int pos = 0; pos < 9; ++pos) {
        row = 3 * (square / 3) + pos / 3;
        column = 3 * (square % 3) + pos % 3;

        if ((tF->a[row][column] == 0) && (fF->a[row][column] == 0) && (uDR->a[row][d] == 0) && (uDC->a[column][d] == 0) && (uDS->a[square][d] == 0)) {
            tF->a[row][column] = d;
            uDR->a[row][d] = 1;
            uDC->a[column][d] = 1; 
            uDS->a[square][d] = 1;
            generate_transversals(tF, fF, uDR, uDC, uDS, trans, transCount, d, square + 1);
            tF->a[row][column] = 0;
            uDR->a[row][d] = 0;
            uDC->a[column][d] = 0; 
            uDS->a[square][d] = 0;
        }
    }
}

static int find_digit_with_min_trans(struct array *uD, struct array *trans) {
    int min, idx;
    min = MAX_TRANS_COUNT;
    idx = -1; 
    for (int i = 1; i < uD->size; ++i) {
        if ((uD->a[i] == 0) && (trans->a[i] <= min)) {
            min = trans->a[i];
            idx = i;
        }
    } 

    if (min == 0) {
        idx = -1;
    }

    return idx;
}

static bool can_be_covered(struct array2d *f1, struct array2d *f2) {
    for (int i = 0; i < f1->r; ++i) {
        for (int j = 0; j < f1->c; ++j) {
            if ((f1->a[i][j] != 0) && (f2->a[i][j] != 0) && (f1->a[i][j] != f2->a[i][j])) {
                return false;
            }
        }
    }
    return true;
}

static void cover(struct array2d *f1, struct array2d *f2) {
    for (int i = 0; i < f1->r; ++i) {
        for (int j = 0; j < f1->c; ++j) {
            if ((f2->a[i][j] != 0)) {
                f1->a[i][j] = f2->a[i][j];
            }
        }
    }
}

static void uncover(struct array2d *f1, struct array2d *f2, struct array2d *iF) {
    for (int i = 0; i < f1->r; ++i) {
        for (int j = 0; j < f1->c; ++j) {
            if ((iF->a[i][j] == 0) && (f1->a[i][j] == f2->a[i][j])) {
                f1->a[i][j] = 0;
            }
        }
    }
}

static bool find_solution(struct array2d *iF, struct array2d *fF, struct array* uD, struct array2d **trans, struct array *transCount) {
    int digit;
    if (uD->a[0] == NINE) {
        return true;
    } 

    digit = find_digit_with_min_trans(uD, transCount);

    if (digit == -1) {
        return false;
    }

    for (int i = 0; i < transCount->a[digit]; ++i) {
        if (can_be_covered(fF, &trans[digit][i])) {
            cover(fF, &trans[digit][i]);
            uD->a[0]++;
            uD->a[digit] = 1;
            if (find_solution(iF, fF, uD, trans, transCount)) {
                return true;
            } else {
                uncover(fF, &trans[digit][i], iF);
                uD->a[digit] = 0;
                uD->a[0]--;
            }
        }
    }

    return false;
}

static bool fill_used_digits(struct array2d *inF, struct array2d *uDR, struct array2d *uDC, struct array2d *uDS) {
    for (int i = 0; i < inF->r; ++i) {
        for (int j = 0; j < inF->c; ++j) {
            int d = inF->a[i][j];
            if (d != 0) {
                uDR->a[i][d]++;
                uDC->a[j][d]++;
                uDS->a[(i / 3) * 3 + (j / 3)][d]++;
            }
        }
    }

    for (int i = 0; i < inF->r; ++i) {
        for (int d = 1; d <= NINE; ++d) {
            if ((uDR->a[i][d] > 1) || (uDC->a[i][d] > 1) || (uDS->a[i][d] > 1)) {
                return false;
            }
        }
    }
    return true;
}

struct array2d ** init_trans_array(int n) {
    struct array2d **trans;
    trans = (struct array2d **) calloc(n, sizeof(struct array2d *));
    for (int i = 0; i < n; ++i) {
        trans[i] = (struct array2d *) calloc(MAX_TRANS_COUNT, sizeof(struct array2d));
    }
    return trans;
}


static void free_trans_array(struct array2d **trans, struct array *transCount, int n) {
    for (int d = 0; d < n; ++d) {
        for (int j = 0; j < transCount->a[d]; ++j) {
            free_array2d(&trans[d][j]);
        }
        free(trans[d]);
    }
    free(trans);
}

static void fill_trans_field(struct array2d *inF, struct array2d *tF, int d) {
    for (int i = 0; i < tF->r; ++i) {
        for (int j = 0 ; j < tF->c; ++j) {
            if (inF->a[i][j] == d) {
                tF->a[i][j] = d;
            }
        }
    }
}

void solve(struct array2d *initialField) {
    int digit, row, column;
    struct array2d usedDigitsRows, usedDigitsColumns, usedDigitsSquares, *filledField, **transversals;
    struct array usedDigits, transCount;
    init_array2d(&usedDigitsRows, NINE, TEN);
    init_array2d(&usedDigitsColumns, NINE, TEN);
    init_array2d(&usedDigitsSquares, NINE, TEN);
    init_array(&usedDigits, TEN); 
    init_array(&transCount, TEN); 
    transversals = init_trans_array(TEN);

    filledField = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(filledField, initialField->r, initialField->c);

    if((initialField->r != NINE) || (initialField->c != NINE)) {
        printf("Initial field size is incorrect (should be %d): rows = %d, columns = %d\n", NINE, initialField->r, initialField->c);
        abort();
    }
    
    copy_array2d(initialField, filledField);
    if (!fill_used_digits(initialField, &usedDigitsRows, &usedDigitsColumns, &usedDigitsSquares)) {
        printf("Solution does not exist.\n");
    } else {
        for (int d = 1; d <= 9; ++d) {
            struct array2d transField; 
            init_array2d(&transField, NINE, NINE);
            fill_trans_field(initialField, &transField, d);
            generate_transversals(&transField, filledField, &usedDigitsRows, &usedDigitsColumns, &usedDigitsSquares, transversals, &transCount, d, 0);
            free_array2d(&transField);
        }

        if (find_solution(initialField, filledField, &usedDigits, transversals, &transCount)) {
            print_array2d(filledField);
        } else {
            printf("Solution does not exist.\n");
        }
    }


    free_array2d(&usedDigitsRows);
    free_array2d(&usedDigitsColumns);
    free_array2d(&usedDigitsSquares);
    free_array2d(filledField);
    free(filledField);
    free_array(&usedDigits);
    free_trans_array(transversals, &transCount, TEN);
    free_array(&transCount);
}

