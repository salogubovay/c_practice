#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int n;
int maxNum;
int *innerCircle;
int *outerCircle;
int *usedNumbers;

void read_input() {
    int res = scanf("%d", &n);
    if ((res != 1) || (n < 3)) {
        printf("Incorrect input\n");
        abort();
    }
}

int * create_array(int size) {
    return calloc(size, sizeof(int));
}

#if 0
void print_array(int *pa, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", pa[i]);
    }
    printf("\n");
}
#endif

void print_magic_triplets(int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d,%d,%d", outerCircle[i], innerCircle[i], innerCircle[(i + 1) % size]);
        if (i != size - 1) {
            printf("; ");
        }
    }
    printf("\n");
}


void fill_circles(int pos, int size, int sum) {
    if (pos == size) {
        int outerNum = sum - innerCircle[0] - innerCircle[pos - 1];
        if ((outerNum >= 1) && (outerNum <= maxNum) && (usedNumbers[outerNum] == 0) && (outerCircle[0] < outerNum)) {
            outerCircle[pos - 1] = outerNum;
            print_magic_triplets(size); 
       }
       return;
    }  

    for (int d = 1; d <= maxNum; ++d) {
        if (usedNumbers[d] == 0) {
            int outerNum;
            usedNumbers[d] = 1;
            innerCircle[pos] = d;
            outerNum = sum - innerCircle[pos - 1] - innerCircle[pos];
            if ((outerNum >= 1) && (outerNum <= maxNum) && (usedNumbers[outerNum] == 0)) {
                outerCircle[pos - 1] = outerNum;
                if (outerCircle[0] <= outerCircle[pos - 1]) {
                    usedNumbers[outerNum] = 1;
                    fill_circles(pos + 1, size, sum);
                    usedNumbers[outerNum] = 0;
                }
            }
            usedNumbers[d] = 0;
        }
    }
}

int main() {
    int maxSum;
    printf("Enter number of triplets: ");
    read_input();
    innerCircle = create_array(n);
    outerCircle = create_array(n); 
    usedNumbers = create_array(2 * n + 1);
    maxNum = 2 * n;
    maxSum = 2 * n + 2 * n - 1 + 2 * n - 2;

    for (int s = 2 * n; s <= maxSum; ++s) {
        for (int d = 1; d <= maxNum; ++d) {
            innerCircle[0] = d;
            usedNumbers[d] = 1;
            fill_circles(1, n, s);
            usedNumbers[d] = 0;
        }
    }

    free(innerCircle);
    free(outerCircle);
    free(usedNumbers);
    return 0;
}
