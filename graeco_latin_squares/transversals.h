#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../array/array.h"
#include "sizes.h"


struct FieldState {
    struct array2d *field;
    struct array2d *usedDigitsRows;
    struct array2d *usedDigitsColumns;
    struct array *usedNumbers;
};

struct Transversals {
    struct array2d **trans;
    struct array *transCount;
    int numbers;
    int size;
};

void print_trans_count(const struct Transversals *tr);
void free_transversals(struct Transversals *tr);
void init_field_state(struct FieldState *fs, int size, int numbers);
void free_field_state(struct FieldState *fs);
struct Transversals * generate_transversals_public(int size, int numbers);
void cover_with_number(const struct array2d *field, const struct array2d *pattern, const int number);
void uncover_with_number(const struct array2d *field, const int number);
bool can_be_covered(const struct array2d *field, const struct array2d *pattern);
struct Transversals * generate_orthogonal_transversals_public(const struct array2d *initialField);
