#include "transversals.h"

static void read_int(int *pn, int maxNumber) {
    int res = scanf("%d", pn);
    if ((res != 1) || (*pn <= 0) || (*pn >= maxNumber)) {
        printf("Incorrect input.\n");
        abort();
    }
}

static bool generate_disordered_square(const struct Transversals *tr, const struct FieldState *fs, int number) {
    if (number == fs->usedDigitsRows->c) {
        return true;
    }

    for (int i = 0; i < tr->transCount->a[1]; ++i) {
        if (can_be_covered(fs->field, &tr->trans[1][i])) {
            cover_with_number(fs->field, &tr->trans[1][i], number);
            if (generate_disordered_square(tr, fs, number + 1)) {
                return true;
            }
            uncover_with_number(fs->field, number);   
        }
    }
    return false;
}

int main() {
    struct Transversals *tr;
    struct FieldState fs;
    int size, nums, transIdx;
    size = 10;
    nums = 1;
    tr = generate_transversals_public(size, nums); 
    printf("Enter initial transversal idx for number 1 (range from [0 to %d) ): ", tr->transCount->a[1]);
    read_int(&transIdx, tr->transCount->a[1]);
    init_field_state(&fs, size, size);
    cover_with_number(fs.field, &tr->trans[1][transIdx], 1);
    generate_disordered_square(tr, &fs, 2);
    print_array2d(fs.field); 
    free_transversals(tr);
    free_field_state(&fs);
    free(tr);
    return 0;
}
