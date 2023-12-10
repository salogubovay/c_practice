#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../array/array.h"
#include "sizes.h"

struct array2d * solve(struct array2d *initialField);

static void fill_used_digits(struct array2d *inF, struct array2d *uDR, struct array2d *uDC, struct array2d *uDS);

static bool find_solution(struct array2d * inF, struct array2d *fF, struct array2d *usedDR, struct array2d *usedDC, struct array2d *usedDS, int pos);
