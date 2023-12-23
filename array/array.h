#include <stdlib.h>
#include <stdio.h>

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

void copy_array2d(struct array2d *src, struct array2d * trg);

void print_array2d(struct array2d *ps);

void copy_array_elements(int * src, int * trg, int srcStart, int destStart, int n);

void print_array(int *pa, int n);

void copy_array_elements_modify(int * src, int * trg, int srcStart, int destStart, int n, int m);

void read_digits_to_array2d_stdin(struct array2d *ps); 

void read_array2d(struct array2d *ps);

