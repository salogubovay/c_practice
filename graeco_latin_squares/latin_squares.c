#include "latin_squares.h"

static bool check_is_valid_num(int num, int size) {
    if ((1 <= num) && (num <= size)) {
        return true;
    } else {
        return false;
    }
}

static bool is_square(const struct array2d *initialField) {
    return (initialField->r == initialField->c) ? true : false;
}

static bool contains_valid_numbers(const struct array2d *initialField) {
    struct FieldState fs;
    int i;
    bool isValid;
    i = 0;
    isValid = true;

    init_field_state(&fs, initialField->r, initialField->c);
    
    while ((i < initialField->r) && isValid) {
        for (int j = 0; j < initialField->c; ++j) {
            int num = initialField->a[i][j];

            if (!check_is_valid_num(num, initialField->r)) {
                printf("Input square numbers should be in range [%d; %d]. " 
                       "But current input square contains num = %d at position row = %d, column = %d.\n", 1, initialField->r, num, i + 1, j + 1);
                isValid = false;
                break; 
            }

            if ((fs.usedDigitsRows->a[i][num] == 0) && (fs.usedDigitsColumns->a[j][num] == 0)) {
                fs.field->a[i][j] = num;
                fs.usedDigitsRows->a[i][num] = 1;
                fs.usedDigitsColumns->a[j][num] = 1;
            } else {
                printf("Input square is not disordered.\n");
                isValid = false;
                break;
            }
        }
        ++i;
    }
    free_field_state(&fs);
    return isValid;
}

bool is_disordered_square(const struct array2d *initialField) {
    if (initialField == NULL) {
        return false;
    }
   
    if (is_square(initialField)) {
        return contains_valid_numbers(initialField);
    } else {
        return false;
    }
}
