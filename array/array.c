#include "array.h"

void init_array(struct array *ps, int n) {
    ps->a = calloc(n, sizeof(int));
    ps->size = n;
}

void print_array(int *pa, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d", pa[i]);
        if (i < n - 1) {
            printf(" "); 
        } else {
            printf("\n");
        }
    }
}

void free_array(struct array *ps) {
    free(ps->a);
    ps->size = 0;
}

void init_array2d(struct array2d *ps, int n, int m) {
    ps->a = calloc(n, sizeof(int *));
    for(int i = 0; i < n; ++i) {
        ps->a[i] = calloc(m, sizeof(int));
    }
    ps->r = n;
    ps->c = m;
}

void free_array2d(struct array2d *ps) {
    for (int i = 0; i < ps->r; ++i) {
        free(ps->a[i]);
    }
    free(ps->a);
    ps->r = 0;
    ps->c = 0;
}

void print_array2d(struct array2d *ps) {
    for (int i = 0; i < ps->r; ++i) {
        print_array(ps->a[i], ps->c);
    }
}

void fill_array_with_zeros(struct array *ps) {
    for (int i = 0; i < ps->size; ++i) {
        ps->a[i] = 0;
    }
}

void copy_array(int * src, int * trg, int size) {
    for (int i = 0; i < size; ++i) {
        trg[i] = src[i];
    }
}

void copy_array_elements(int * src, int * trg, int srcStart, int destStart, int n) {
    for (int i = 0; i < n; ++i) {
        trg[destStart + i] = src[srcStart + i];
    }
}

void copy_array_elements_modify(int * src, int * trg, int srcStart, int destStart, int n, int m) {
    for (int i = 0; i < n; ++i) {
        trg[destStart + i] = src[srcStart + i] + m;
    }
}