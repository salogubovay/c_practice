#include <stdlib.h>

struct array {
    int *a;
    int size;
};

struct array2d {
    int **a;
    int r;
    int c;
};

void init_array(struct array *ps, int n);

void init_array2d(struct array2d *ps, int n, int m);

void free_array(struct array *ps);

void free_array2d(struct array2d *ps);

void fill_array_with_zeros(struct array *ps);

void copy_array(int * src, int * trg, int size);
