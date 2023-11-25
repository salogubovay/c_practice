#include <stdbool.h>
#include <stdio.h>
#include "array.h"

static unsigned long long factorial(int n);

static unsigned long long num_of_arrangements(int n, int r);

static void fill_arrangements(struct array2d * a2d, int maxNum);

struct array2d * generate_arrangements(int n, int maxNum);
