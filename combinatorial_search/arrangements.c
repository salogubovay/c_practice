#include "arrangements.h"

static void print_magic_triplets(struct array *innerCircle, struct array *outerCircle) {
    for (int i = 0; i < innerCircle->size; ++i) {
        printf("%d,%d,%d", outerCircle->a[i], innerCircle->a[i], innerCircle->a[(i + 1) % innerCircle->size]);
        if (i != (innerCircle->size - 1)) {
            printf("; ");
        }
    }
    printf("\n");
}

static bool possible_to_generate_arrangement(struct array *innerCircle, struct array *outerCircle, struct array *usedNumbers, int curNum, int pos, int sum) {
    if (usedNumbers->a[curNum] == 0) {
        int outerNum;
        usedNumbers->a[curNum] = 1; 
        innerCircle->a[pos] = curNum;
        outerNum = sum - innerCircle->a[pos - 1] - innerCircle->a[pos];
        if ((outerNum >= 1) && (outerNum < usedNumbers->size) && (usedNumbers->a[outerNum] == 0)) {
            outerCircle->a[pos - 1] = outerNum;
            if (outerCircle->a[0] <= outerCircle->a[pos - 1]) {
                usedNumbers->a[outerNum] = 1;
                return true;
            } else {
                outerCircle->a[pos - 1] = 0;
            }
         }    
        usedNumbers->a[curNum] = 0;
        innerCircle->a[pos] = 0;
    }
    return false;
}

static void fill_circles(struct array *innerCircle, struct array *outerCircle, struct array *usedNumbers, int sum) {
    int pos = 1;
    while (pos > 0) {
        if (pos == innerCircle->size) {
            int outerNum = sum - innerCircle->a[0] - innerCircle->a[pos - 1];
            if ((outerNum >= 1 && (outerNum < usedNumbers->size) && usedNumbers->a[outerNum] == 0) && (outerCircle->a[0] < outerNum)) {
                outerCircle->a[pos - 1] = outerNum;
                print_magic_triplets(innerCircle, outerCircle);
                outerCircle->a[pos - 1] = 0;
            }
            --pos;
        } else {
            int curNum = innerCircle->a[pos] + 1;
            usedNumbers->a[innerCircle->a[pos]] = 0;
            innerCircle->a[pos] = 0;
            usedNumbers->a[outerCircle->a[pos - 1]] = 0;
            outerCircle->a[pos - 1] = 0;
            while (curNum < usedNumbers->size) {
                if (possible_to_generate_arrangement(innerCircle, outerCircle, usedNumbers, curNum, pos, sum)) {
                    ++pos;
                    break;
                } else {
                    ++curNum;
                }
            }
        
            if (curNum >= usedNumbers->size) {
                --pos;
            }
        }
    }
}

void generate_triplets(int n) {
    int maxSum; 
    struct array innerCircle, outerCircle, usedNumbers;

    init_array(&innerCircle, n);
    init_array(&outerCircle, n);
    init_array(&usedNumbers, 2 * n + 1);

    maxSum = 2 * n + 2 * n - 1 + 2 * n - 2;

    for (int s = 2 * n; s <= maxSum; ++s) {
        fill_circles(&innerCircle, &outerCircle, &usedNumbers, s);
        for (int d = 1; d < usedNumbers.size; ++d) {
            innerCircle.a[0] = d;
            usedNumbers.a[d] = 1;
            fill_circles(&innerCircle, &outerCircle, &usedNumbers, s);
            usedNumbers.a[d] = 0;
        }
    }

    free_array(&innerCircle);
    free_array(&outerCircle);
    free_array(&usedNumbers);
}
