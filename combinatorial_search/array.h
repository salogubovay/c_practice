struct array {
    int *a;
    int size;
};

void init_array(struct array *ps, int n);

void free_array(struct array *ps);

