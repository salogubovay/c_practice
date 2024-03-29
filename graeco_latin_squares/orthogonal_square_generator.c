#include "transversals.h"
#include "latin_squares.h"

#include <getopt.h>

#define OPTSTR "a" //print all found orthogonal squares

int orthCount = 0;

static void read_int(int *pn) {
    int res = scanf("%d", pn);
    if ((res != 1) || (*pn <= 0)) {
        printf("Size is incorrect\n");
        abort();
    }
}

static bool generate_orthogonal_square(const struct array2d *initialField, const struct Transversals *tr, const struct FieldState *fs, const int number) {
    if (number == fs->usedDigitsRows->c) {
        ++orthCount;
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

static void generate_all_orthogonal_squares(const struct array2d *initialField, const struct Transversals *tr, const struct FieldState *fs, const int number) {
    if (number == fs->usedDigitsRows->c) {
        ++orthCount;
        printf("Orthogonal square %d:\n", orthCount);
        print_array2d(fs->field);
        return;
    }

    for (int i = 0; i < tr->transCount->a[number]; ++i) {
        if (can_be_covered(fs->field, &tr->trans[number][i])) {
            cover_with_number(fs->field, &tr->trans[number][i], number);
            generate_all_orthogonal_squares(initialField, tr, fs, number + 1); 
            uncover_with_number(fs->field, number);   
        }
    }
}

int main(int argc, char *argv[]) {
    struct array2d initialField;
    struct Transversals *tr;
    struct FieldState fs;
    int size, opt;
    bool allMode = false;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF) {
        switch(opt) {
            case 'a':
                allMode = true;
                break;
        }
    }

    read_int(&size);
    init_array2d(&initialField, size, size);
    read_array2d(&initialField);
    if (is_disordered_square(&initialField)) {
        tr = generate_orthogonal_transversals_public(&initialField); 
        init_field_state(&fs, size, size);

        if (allMode) {
            generate_all_orthogonal_squares(&initialField, tr, &fs, 1);
        } else {
            generate_orthogonal_square(&initialField, tr, &fs, 1); 
            if (orthCount == 1) {
                print_array2d(fs.field);
            }
        }
        if (orthCount == 0) {
            printf("Orthogonal square is not found.\n");
        }
        free_transversals(tr);
        free(tr);
        free_field_state(&fs);
    }

    free_array2d(&initialField);
    return 0;
}
