#include "sudoku.h"

int MAX_TRANS_COUNT = 46656; //max count of transversals

struct FieldState {
    struct array2d *field;
    struct array2d *usedDigitsRows;
    struct array2d *usedDigitsColumns;
    struct array2d *usedDigitsSquares;
};

struct Transversals {
    struct array2d **trans;
    struct array *transCount;
};


static void copy_array2d_numbers(const struct array2d *src, const struct array2d *trg, int num) {
    for (int i = 0; i < src->r; ++i) {
        for (int j = 0; j < src->c; ++j) {
            if (src->a[i][j] == num) {
                trg->a[i][j] = num;
            }
        }
    }
}

static void generate_transversals_for_digit(const struct Transversals *tr, const struct FieldState *fs, int digit, int square) {
    int row, column;
    if (square == NINE) {
        struct array2d nextTrans;
        int transNum = tr->transCount->a[digit];
        init_array2d(&nextTrans, fs->field->r, fs->field->c);
        tr->trans[digit][transNum] = nextTrans;
        copy_array2d_numbers(fs->field, &nextTrans, digit);
        tr->transCount->a[digit]++;
        return;
    }
   
    if (fs->usedDigitsSquares->a[square][digit] == 1) {
        return generate_transversals_for_digit(tr, fs, digit, square + 1);
    } 

    for (int pos = 0; pos < 9; ++pos) {
        row = 3 * (square / 3) + pos / 3;
        column = 3 * (square % 3) + pos % 3;

        if ((fs->field->a[row][column] == 0) && (fs->usedDigitsRows->a[row][digit] == 0) && (fs->usedDigitsColumns->a[column][digit] == 0) && (fs->usedDigitsSquares->a[square][digit] == 0)) {
            fs->field->a[row][column] = digit;
            fs->usedDigitsRows->a[row][digit] = 1;
            fs->usedDigitsColumns->a[column][digit] = 1; 
            fs->usedDigitsSquares->a[square][digit] = 1;
            generate_transversals_for_digit(tr, fs, digit, square + 1);
            fs->field->a[row][column] = 0;
            fs->usedDigitsRows->a[row][digit] = 0;
            fs->usedDigitsColumns->a[column][digit] = 0; 
            fs->usedDigitsSquares->a[square][digit] = 0;
        }
    }
}

static int find_digit_with_min_trans(const struct array *usedDigits, const struct array *transCount) {
    int min, idx;
    min = MAX_TRANS_COUNT;
    idx = -1; 
    for (int i = 1; i < usedDigits->size; ++i) {
        if ((usedDigits->a[i] == 0) && (transCount->a[i] <= min)) {
            min = transCount->a[i];
            idx = i;
        }
    } 

    if (min == 0) {
        idx = -1;
    }

    return idx;
}

static bool can_be_covered(const struct array2d *field1, const struct array2d *field2) {
    for (int i = 0; i < field1->r; ++i) {
        for (int j = 0; j < field1->c; ++j) {
            if ((field1->a[i][j] != 0) && (field2->a[i][j] != 0) && (field1->a[i][j] != field2->a[i][j])) {
                return false;
            }
        }
    }
    return true;
}

static void cover(const struct array2d *field1, const struct array2d *field2) {
    for (int i = 0; i < field1->r; ++i) {
        for (int j = 0; j < field1->c; ++j) {
            if ((field2->a[i][j] != 0)) {
                field1->a[i][j] = field2->a[i][j];
            }
        }
    }
}

static void uncover(const struct array2d *field1, const struct array2d *field2, const struct array2d *initialField) {
    for (int i = 0; i < field1->r; ++i) {
        for (int j = 0; j < field1->c; ++j) {
            if ((initialField->a[i][j] == 0) && (field1->a[i][j] == field2->a[i][j])) {
                field1->a[i][j] = 0;
            }
        }
    }
}

static bool find_solution(const struct array2d *initialField, const struct FieldState *fs, const struct array* usedDigits, const struct Transversals *tr) {
    int digit;
    if (usedDigits->a[0] == NINE) {
        return true;
    } 

    digit = find_digit_with_min_trans(usedDigits, tr->transCount);

    if (digit == -1) {
        return false;
    }

    for (int i = 0; i < tr->transCount->a[digit]; ++i) {
        if (can_be_covered(fs->field, &tr->trans[digit][i])) {
            cover(fs->field, &tr->trans[digit][i]);
            usedDigits->a[0]++;
            usedDigits->a[digit] = 1;
            if (find_solution(initialField, fs, usedDigits, tr)) {
                return true;
            } else {
                uncover(fs->field, &tr->trans[digit][i], initialField);
                usedDigits->a[digit] = 0;
                usedDigits->a[0]--;
            }
        }
    }

    return false;
}

static bool fill_used_digits(const struct FieldState *fs) {
    for (int i = 0; i < fs->field->r; ++i) {
        for (int j = 0; j < fs->field->c; ++j) {
            int d = fs->field->a[i][j];
            if (d != 0) {
                fs->usedDigitsRows->a[i][d]++;
                fs->usedDigitsColumns->a[j][d]++;
                fs->usedDigitsSquares->a[(i / 3) * 3 + (j / 3)][d]++;
            }
        }
    }

    for (int i = 0; i < fs->field->r; ++i) {
        for (int d = 1; d <= NINE; ++d) {
            if ((fs->usedDigitsRows->a[i][d] > 1) || (fs->usedDigitsColumns->a[i][d] > 1) || (fs->usedDigitsSquares->a[i][d] > 1)) {
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

static void free_trans_array(struct array2d **trans, const struct array *transCount, int n) {
    for (int d = 0; d < n; ++d) {
        for (int j = 0; j < transCount->a[d]; ++j) {
            free_array2d(&trans[d][j]);
        }
        free(trans[d]);
    }
    free(trans);
}

static void init_field_state(struct FieldState *fs, int size, int numbers) {
    fs->field = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->field, size, size);
    fs->usedDigitsRows = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->usedDigitsRows, size, numbers);
    fs->usedDigitsColumns = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->usedDigitsColumns, size, numbers);
    fs->usedDigitsSquares = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->usedDigitsSquares, size, numbers);
}

static void free_field_state(struct FieldState *fs) {
    free_array2d(fs->field);
    free_array2d(fs->usedDigitsRows);
    free_array2d(fs->usedDigitsColumns);
    free_array2d(fs->usedDigitsSquares);
    free(fs->field);
    free(fs->usedDigitsRows);
    free(fs->usedDigitsColumns);
    free(fs->usedDigitsSquares);
}

static void init_transversals(struct Transversals *tr, int n) {
    tr->trans = init_trans_array(n);
    tr->transCount = (struct array *) calloc(1, sizeof(struct array));
    init_array(tr->transCount, n);
}


static void free_transversals(struct Transversals *tr) {
    free_trans_array(tr->trans, tr->transCount, tr->transCount->size);
    free_array(tr->transCount);
    free(tr->transCount);
}

static struct Transversals generate_transversals(const struct FieldState *fs) {
    struct Transversals trans;
    init_transversals(&trans, TEN);
    for (int d = 1; d < TEN; ++d) {
        generate_transversals_for_digit(&trans, fs, d, 0);
    }
    return trans;
}

void solve(const struct array2d *initialField) {
    struct array usedDigits;
    struct FieldState fs;
    struct Transversals tr;

    if ((initialField == NULL) || (initialField->a == NULL)) {
        printf("Error: initialField is NULL.\n");
        abort();
    }

    init_field_state(&fs, NINE, TEN);
    init_array(&usedDigits, TEN); 

    if((initialField->r != NINE) || (initialField->c != NINE)) {
        printf("Initial field size is incorrect (should be %d): rows = %d, columns = %d\n", NINE, initialField->r, initialField->c);
        abort();
    }
    
    copy_array2d(initialField, fs.field);
    if (!fill_used_digits(&fs)) {
        printf("Solution does not exist.\n");
    } else {
        tr = generate_transversals(&fs);
        if (find_solution(initialField, &fs, &usedDigits, &tr)) {
            print_array2d(fs.field);
        } else {
            printf("Solution does not exist.\n");
        }
    }

    free_array(&usedDigits);
    free_field_state(&fs);
    free_transversals(&tr);
}
