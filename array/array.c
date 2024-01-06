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

void print_array2d(const struct array2d *ps) {
    for (int i = 0; i < ps->r; ++i) {
        print_array(ps->a[i], ps->c);
    }
}

void fill_array_with_zeros(const struct array *ps) {
    for (int i = 0; i < ps->size; ++i) {
        ps->a[i] = 0;
    }
}

void copy_array(int * src, int * trg, int size) {
    for (int i = 0; i < size; ++i) {
        trg[i] = src[i];
    }
}

void copy_array2d(const struct array2d *src, const struct array2d * trg) {
    if ((src->r != trg->r) || (src->c != trg->c)) {
        printf("Sizes of arrays are not equal.\nsrc: r = %d, c = %d\ntrg: r = %d, c = %d\n", src->r, src->c, trg->r, trg->c);
        abort();
    }

    for (int i = 0; i < src->c; ++i) {
        copy_array(src->a[i], trg->a[i], src->c);
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

void read_digits_to_array2d_stdin(const struct array2d *ps) { 
    char *line;
    int size = ps->c + 2;
    line  = (char *) calloc(size, sizeof(char));
    for (int row = 0; row < ps->r; ++row) {
        if (fgets(line, size, stdin) == NULL) {
            printf("Error EOF\n");
            abort();
        }
        for (int column = 0; column < ps->c; ++column) {
            int d = line[column] - '0';
            ps->a[row][column] = d;
        }
    }
    free(line);
}

void read_array2d(const struct array2d *ps) {
    int n, res;
    for (int i = 0; i < ps->r; ++i) {
        for (int j = 0; j < ps->c; ++j) {
            res = scanf("%d", &n);
            if (res != 1) {
                printf("Number has not been recognized.\n");
                abort();
            }
            ps->a[i][j] = n;
        }
    } 
}
