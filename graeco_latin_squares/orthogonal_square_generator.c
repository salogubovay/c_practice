#include "transversals.h"

static bool generate_orthogonal_square(const struct array2d *initialField, const struct Transversals *tr, const struct FieldState *fs, const int number) {
    if (number == fs->usedDigitsRows->c) {
        return true;
    }

    for (int i = 0; i < tr->transCount->a[number]; ++i) {
        if (can_be_covered(fs->field, &tr->trans[number][i])) {
            cover_with_number(fs->field, &tr->trans[number][i], number);
            if (generate_orthogonal_square(initialField, tr, fs, number + 1)) {
                return true;
            }
            uncover_with_number(fs->field, number);   
        }
    }
    return false;
}

int main() {
    struct array2d initialField;
    struct Transversals *tr;
    struct FieldState fs;
    int size;
    size = 10;
    init_array2d(&initialField, size, size);
    read_array2d(&initialField);
    tr = generate_orthogonal_transversals_public(&initialField); 

    init_field_state(&fs, size, size);
    if (generate_orthogonal_square(&initialField, tr, &fs, 1)) {
        printf("Orthogonal square:\n");
        print_array2d(fs.field);
    } else {
        printf("Orthogonal square is not found.\n");
    }

    free_transversals(tr);
    free(tr);
    free_field_state(&fs);
    free_array2d(&initialField);
    return 0;
}
