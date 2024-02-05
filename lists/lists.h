#include "../array/array.h"

#define INITIAL_SIZE 10

struct ArrayListOfArrays {
    struct array **arrays;
    int capacity;
    int size;
};


void init_arraylist(struct ArrayListOfArrays *list);
void add_array_to_arraylist(struct ArrayListOfArrays *list, struct array *a);
void free_arraylist(struct ArrayListOfArrays *list);
struct ArrayListOfArrays * create_arraylist();
void print_arraylist(const struct ArrayListOfArrays *list);
