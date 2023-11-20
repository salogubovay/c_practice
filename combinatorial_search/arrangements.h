#include <stdbool.h>
#include "array.h"

static void print_magic_triplets(struct array *innerCircle, struct array *outerCircle); 

static bool possible_to_generate_arrangement(struct array *innerCircle, struct array *outerCircle, struct array *usedNumbers, int curNum, int pos, int sum);

static void fill_circles(struct array *innerCircle, struct array *outerCircle, struct array *usedNumbers, int sum);

void generate_triplets(int n);
