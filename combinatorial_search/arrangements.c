#include "arrangements.h"

static unsigned long long factorial(int n) {
    unsigned long long fact = 1;
    for (int i = 2; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

static unsigned long long num_of_arrangements(int n, int r) {
    if (n <= r) {
        printf("Total number of elements n = %d is less than size of arrangement r = %d\n", n, r);
    }
    return factorial(n) / factorial(n - r);
}

static void fill_arrangements(struct array2d * a2d, int maxNum) {
    struct array usedNumbers, curArrangement;
    int count, pos;
    count = 0;
    pos = 0;
    init_array(&usedNumbers, maxNum + 1);
    init_array(&curArrangement, a2d->c);

    while (pos >= 0) {
        if (pos == a2d->c) {
            copy_array(curArrangement.a, a2d->a[count], curArrangement.size);
            ++count;
            --pos;
        } else {
            int curNum = curArrangement.a[pos] + 1;
            usedNumbers.a[curArrangement.a[pos]] = 0;
            curArrangement.a[pos] = 0;
            while (curNum < usedNumbers.size) {
                if (usedNumbers.a[curNum] == 0) {
                    usedNumbers.a[curNum] = 1;
                    curArrangement.a[pos] = curNum;
                    ++pos;
                    break;
                } else {
                    ++curNum;
                }
            }
            
            if (curNum == usedNumbers.size) {
                --pos; 
            }
        }
    } 

    free_array(&usedNumbers);
    free_array(&curArrangement);
}

struct array2d * generate_arrangements(int n, int maxNum) {
    struct array2d *a2d;
    unsigned long long size = num_of_arrangements(maxNum, n);
    a2d = calloc(1, sizeof(struct array2d)); 
    init_array2d(a2d, size, n);
    fill_arrangements(a2d, maxNum);
    return a2d;
}
