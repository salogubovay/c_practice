#include <stdlib.h>
#include "array.h"

void init_array(struct array *ps, int n) {
    ps->a = calloc(n, sizeof(int));
    ps->size = n;
}

void free_array(struct array *ps) {
    free(ps->a);
    ps->size = 0;
}

