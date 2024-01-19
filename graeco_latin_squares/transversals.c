#include "transversals.h"

unsigned factorial(int n) {
    unsigned fact = 1;
    for (int i = 1; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

void print_trans_count(const struct Transversals *tr) {
    for (int i = 1; i < tr->numbers; ++i) {
        printf("transCount[%d] = %d\n", i, tr->transCount->a[i]);
    }
}

static void check_sizes(const struct array2d *f1, const struct array2d *f2) {
    if ((f1->r != f2->r) || ( f1->c != f2->c)) {
        printf("f1 and f2 sizes are not equal\n");
        abort();
    }
}

void cover_with_number(const struct array2d *field, const struct array2d *pattern, const int number) {
    check_sizes(field, pattern);

    for (int i = 0; i < field->r; ++i) {
        for (int j = 0; j < field->c; ++j) {
            if ((pattern->a[i][j] != 0)) {
                field->a[i][j] = number;
            }
        }
    }
}

void uncover_with_number(const struct array2d *field, const int number) {
    for (int i = 0; i < field->r; ++i) {
        for (int j = 0; j < field->c; ++j) {
            if (field->a[i][j] == number) {
                field->a[i][j] = 0;
            }
        }
    }
}

bool can_be_covered(const struct array2d *field, const struct array2d *pattern) {
    check_sizes(field, pattern);
    
    for (int i = 0; i < field->r; ++i) {
        for (int j = 0; j < field->c; ++j) {
            if ((field->a[i][j] != 0) && (pattern->a[i][j] != 0)) {
                return false;
            }
        }
    }
    return true;
}

static void copy_array2d_numbers(const struct array2d *src, const struct array2d *trg, int num) {
    check_sizes(src, trg);
    for (int i = 0; i < src->r; ++i) {
        for (int j = 0; j < src->c; ++j) {
            if (src->a[i][j] == num) {
                trg->a[i][j] = num;
            }
        }
    }
}

static void add_next_transversal(const struct Transversals *tr, const struct FieldState *fs, const int number) {
    struct array2d nextTrans;
    int transNum;
   
    if (number >= tr->size) {
        printf("Number is greater than tr->size, number = %d, tr->size = %d\n", number, tr->size);
        abort();
    }   
    transNum = tr->transCount->a[number];
    if (transNum == tr->size) {
        printf("Transversals number is greater than tr->size, next transversal cannot be added.\n");
        abort();
    }
    init_array2d(&nextTrans, fs->field->r, fs->field->c);
    tr->trans[number][transNum] = nextTrans;
    copy_array2d(fs->field, &nextTrans);
    tr->transCount->a[number]++;
}

static void generate_transversals_for_number(const struct Transversals *tr, const struct FieldState *fs, int number, int row) {
    if (row == fs->field->r) {
        add_next_transversal(tr, fs, number);
        return;
    }

    for (int column = 0; column < fs->field->c; ++column) {
        if ((fs->field->a[row][column] == 0) && (fs->usedDigitsRows->a[row][number] == 0) && (fs->usedDigitsColumns->a[column][number] == 0)) {
            fs->field->a[row][column] = number;
            fs->usedDigitsRows->a[row][number] = 1;
            fs->usedDigitsColumns->a[column][number] = 1; 
            generate_transversals_for_number(tr, fs, number, row + 1);
            fs->field->a[row][column] = 0;
            fs->usedDigitsRows->a[row][number] = 0;
            fs->usedDigitsColumns->a[column][number] = 0; 
        }
    }
}

static void generate_orthogonal_transversals_for_all_numbers(const struct array2d *initialField, const struct Transversals *tr,  const struct FieldState *fs, int row) {
    if (row == fs->field->r) {
        int number = 0;
        for (int i = 0; i < initialField->r; ++i) {
            if (initialField->a[i][0] == fs->field->a[i][0]) {
                number = initialField->a[i][0];
                break;
            }
        }
        add_next_transversal(tr, fs, number);
#if 0  
    /*
        uncomment code if you want to find more orthogonal squares (search time will be increased)
    */
        for (int j = 0; j < initialField->c; ++j) {
            if (initialField->a[0][j] == fs->field->a[0][j]) {
                number = initialField->a[0][j];
                break;
            }
        }
        add_next_transversal(tr, fs, number);
#endif
        return;
    }

    for (int column = 0; column < fs->field->c; ++column) {
        int number = initialField->a[row][column];
        if ((fs->usedDigitsColumns->a[column][1] == 0) && (fs->usedNumbers->a[number] == 0)) { // index 1 is used to track column usage
            fs->field->a[row][column] = number;
            fs->usedDigitsColumns->a[column][1] = 1; 
            fs->usedNumbers->a[number] = 1;
            generate_orthogonal_transversals_for_all_numbers(initialField, tr, fs, row + 1);
            fs->field->a[row][column] = 0;
            fs->usedDigitsColumns->a[column][1] = 0;
            fs->usedNumbers->a[number] = 0;
        }
    }
}

void init_field_state(struct FieldState *fs, int size, int numbers) {
    fs->field = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->field, size, size);
    fs->usedDigitsRows = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->usedDigitsRows, size, numbers + 1);
    fs->usedDigitsColumns = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(fs->usedDigitsColumns, size, numbers + 1);
    fs->usedNumbers = (struct array *) calloc(1, sizeof(struct array));
    init_array(fs->usedNumbers, numbers + 1);
}

void free_field_state(struct FieldState *fs) {
    free_array2d(fs->field);
    free_array2d(fs->usedDigitsRows);
    free_array2d(fs->usedDigitsColumns);
    free_array(fs->usedNumbers);
    free(fs->field);
    free(fs->usedDigitsRows);
    free(fs->usedDigitsColumns);
    free(fs->usedNumbers);
}

struct array2d ** init_trans_array(int numbers, int size) {
    struct array2d **trans;
    trans = (struct array2d **) calloc(numbers, sizeof(struct array2d *));
    for (int i = 0; i < numbers; ++i) {
        trans[i] = (struct array2d *) calloc(size, sizeof(struct array2d));
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

static void init_transversals(struct Transversals *tr, int size, int numbers) {
    tr->numbers = numbers;
    tr->size = factorial(size);
    tr->trans = init_trans_array(numbers, tr->size); //numbers+1
    tr->transCount = (struct array *) calloc(1, sizeof(struct array));
    init_array(tr->transCount, numbers);
}


void free_transversals(struct Transversals *tr) {
    free_trans_array(tr->trans, tr->transCount, tr->transCount->size);
    free_array(tr->transCount);
    free(tr->transCount);
}

static struct Transversals * generate_transversals(const struct FieldState *fs, int numbers) {
    struct Transversals *trans = (struct Transversals *) calloc(1, sizeof(struct Transversals));
    init_transversals(trans, fs->field->r, numbers + 1);
    for (int num = 1; num <= numbers; ++num) {
        generate_transversals_for_number(trans, fs, num, 0);
    } 
    return trans;
}

struct Transversals * generate_transversals_public(int size, int numbers) {
    struct FieldState fs;
    struct Transversals *transversals;
    if (size <= 0) {
        printf("Field size is incorrect. Number of rows and columns should be greater than 0. Size = %d.\n", size);
        abort();
    }
    
    if (numbers < 1) {
        printf("Numbers value is incorrect. Numbers value should be greater than or equal to 1. Numbers = %d.\n", numbers);
        abort();
    }

    init_field_state(&fs, size, numbers);
    transversals = generate_transversals(&fs, numbers);
    free_field_state(&fs);
    return transversals;
}

static struct Transversals * generate_orthogonal_transversals(const struct array2d *initialField, const struct FieldState *fs, int numbers) {
    struct Transversals *trans = (struct Transversals *) calloc(1, sizeof(struct Transversals));
    init_transversals(trans, fs->field->r, numbers + 1);
    generate_orthogonal_transversals_for_all_numbers(initialField, trans, fs, 0);
    return trans;
}

struct Transversals * generate_orthogonal_transversals_public(const struct array2d *initialField) {
    struct FieldState fs;
    struct Transversals *transversals;
    int size;
    if ((initialField == NULL)) {
        printf("Incorrect initialField argument.\n");
        abort();
    }

    if ((initialField->r == 0) || (initialField->c == 0) || (initialField->r != initialField->c)) {
        printf("Field size is incorrect. Number of rows and columns should be equal and greater than 0. rows = %d, columns = %d.\n", initialField->r, initialField->c);
        abort();
    }
    
    size = initialField->r;
    init_field_state(&fs, size, size + 1); 
    transversals = generate_orthogonal_transversals(initialField, &fs, size);
    free_field_state(&fs);
    return transversals;
}
