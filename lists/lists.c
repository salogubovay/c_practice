#include "lists.h"


void init_arraylist(struct ArrayListOfArrays *list) {
    list->arrays = (struct array **) calloc(INITIAL_SIZE, sizeof(struct array*));
    list->size = 0;
    list->capacity = INITIAL_SIZE;
}

struct ArrayListOfArrays * create_arraylist() {
    struct ArrayListOfArrays *list;
    list = (struct ArrayListOfArrays *) calloc(1, sizeof(struct ArrayListOfArrays));
    init_arraylist(list);
    return list;
}

static void resize_arraylist(struct ArrayListOfArrays *list, int newCapacity) {
    struct array **newArrays;
    if (list->capacity > newCapacity) {
        printf("New list capcity is lower than current capacity\n");
        abort();
    } 

    newArrays = (struct array **) calloc(newCapacity, sizeof(struct array*));
    for (int i = 0; i < list->size; ++i) {
        newArrays[i] = list->arrays[i];     
    }
    free(list->arrays);
    list->arrays = newArrays;
    list->capacity = newCapacity;
}

void add_array_to_arraylist(struct ArrayListOfArrays *list, struct array *a) {
    if (list->size == list->capacity) {
        resize_arraylist(list, list->capacity * 2);
    }

    list->arrays[list->size] = a;
    ++list->size;
}

void free_arraylist(struct ArrayListOfArrays *list) {
    for (int i = 0; i < list->size; ++i) {
        free_array(list->arrays[i]);
        free(list->arrays[i]);
    }
    free(list->arrays);
}

void print_arraylist(const struct ArrayListOfArrays *list) {
    for (int i = 0; i < list->size; ++i) {
        print_array(list->arrays[i]->a, list->arrays[i]->size);
    }
}
