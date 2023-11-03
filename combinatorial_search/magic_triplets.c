#include <stdio.h>
#include <stdlib.h>
#include "sizes.h"
#include <stdbool.h>
#include <assert.h>


bool usedDigits[SIX + 1];
int ansIndex = 0;

struct permutation {
    char *permArray;
    char size;
} answer[ONE_HUNDRED];

void create_permutation(struct permutation *pPerm, int n) {
    pPerm->permArray = calloc(n, sizeof(char));
    pPerm->size = n;    
}

#if 0
//for testing
void print_permutation_as_triplets(struct permutation* pPerm) {
    char* a = pPerm->permArray;
    assert(pPerm->size == SIX);

    printf("%d,%d,%d; ", a[0], a[4], a[5]);
    printf("%d,%d,%d; ", a[1], a[5], a[3]);
    printf("%d,%d,%d\n", a[2], a[3], a[4]);
}
#endif

void print_triplets(struct permutation *pPerm) {
    char* a = pPerm->permArray;

    assert(pPerm->size == NINE);

    for (int i = 0; i < pPerm->size; i += 3) {
        printf("%d,%d,%d; ", a[i], a[i + 1], a[i + 2]);
    }
    printf("\n");
}

// triplets mapping to array indices: first = 0, 4, 5; second = 1, 5, 3; third = 2, 3, 4;
bool isMagic(struct permutation* pPerm) {
    char* a = pPerm->permArray;
    assert(pPerm->size == SIX);

    if ((a[0] + a[4] + a[5] == a[1] + a[5] + a[3]) && (a[1] + a[5] + a[3] == a[2] + a[3] + a[4])) {
        return true;
    } else {
        return false;
    }
}

void add_to_ans(struct permutation *pPerm) {
    struct permutation newPerm;

    assert(ansIndex < ONE_HUNDRED);
    assert(pPerm->size == SIX);

    create_permutation(&newPerm, NINE);
    answer[ansIndex] = newPerm;
    newPerm.permArray[0] = pPerm->permArray[0];
    newPerm.permArray[1] = pPerm->permArray[4];
    newPerm.permArray[2] = pPerm->permArray[5];
    newPerm.permArray[3] = pPerm->permArray[1];
    newPerm.permArray[4] = pPerm->permArray[5];
    newPerm.permArray[5] = pPerm->permArray[3];
    newPerm.permArray[6] = pPerm->permArray[2];
    newPerm.permArray[7] = pPerm->permArray[3];
    newPerm.permArray[8] = pPerm->permArray[4];
    ++ansIndex;
}


void generate_permutations(struct permutation *pPerm, int curPos, int lastDigit) {
    if ((curPos == pPerm->size) && isMagic(pPerm)) {
        add_to_ans(pPerm);
    } else {
        for (int d = 1; d <= lastDigit; ++d) {
            if (!usedDigits[d]) {
                usedDigits[d] = true;
                pPerm->permArray[curPos] = d;
                generate_permutations(pPerm, curPos + 1, lastDigit);
                usedDigits[d] = false;
            }
        }
    }
}

void free_permutation(struct permutation *pPerm) {
    free(pPerm->permArray);
    pPerm->permArray = 0;
    pPerm->size = 0;
}

bool check_cycle_shift(char *a, char *b, int size) {
    bool isShifted = false;
    int step, i;
    i = 0;
    step = 3;

    while (!isShifted && (i < size)) {
        if (b[0] == a[i]) {
            isShifted = true;
            for (int j = 0; j < size; ++j) {
                if (b[j] != a[(i + j) % size]) {
                    isShifted = false;
                    break;
                }
            }
        }  
        i += step; 
    }
 
    return isShifted;
}


void print_unique_permutations() {
    for (int i = 0; i < ansIndex; ++i) {
        bool isUnique = true;
        for (int j = 0; j < i; ++j) {
            if (check_cycle_shift(answer[i].permArray, answer[j].permArray, answer[i].size)) {
                isUnique = false;
                break;
            }
        } 
        if (isUnique) {
           print_triplets(&answer[i]);
        }
    }
}

int main() {
    struct permutation perm;
    create_permutation(&perm, SIX);
    generate_permutations(&perm, 0, SIX);
    free_permutation(&perm);
    print_unique_permutations();
    
    for (int i = 0; i < ansIndex; ++i) {
        free_permutation(&answer[i]); 
    }

    return 0;
}
