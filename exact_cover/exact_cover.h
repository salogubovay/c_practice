#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "../array/array.h"
#include "../lists/lists.h"


struct Node {
    struct Node *left;
    struct Node *right;
    struct Node *up;
    struct Node *down;
    struct Column *column;
    int rowId;
};

struct Column {
    struct Node head;
    struct Column *prev;
    struct Column *next;
    int size;
    int id;
};

struct ArrayListOfArrays * find_exact_cover(const struct array2d *initialMatrix);
