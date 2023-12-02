#include "../combinatorial_search/combinatorics.h"

void read_input_char(char *pc) {
    int res = 0;
    res = scanf("%c", pc);
    if ((res == 0) || (*pc != 'a' && *pc != 'c' && *pc != 'p')) {
        printf("Incorrect input\n");
        abort();
    }
}

void read_input(int *pn) {
    int res = 0;
    res = scanf("%d", pn);
    if (res == 0) {
        printf("Incorrect input\n");
        abort();
    }
}

int main() {
    char c;
    int n, k;
    struct array2d *combObjects;
    printf("What kind of combinatorial objects do you want to generate?\n(a - arrangements, c - combinations, p - permutations): ");
    read_input_char(&c);

    if (c == 'a') {
        printf("Enter n and k (to generate k-arrangements of numbers from 1 to n): ");
        read_input(&n);
        read_input(&k);
        combObjects = generate_arrangements(n, k);
    } else if (c == 'c') {
        printf("Enter n and k (to generate k-combinations of numbers from 1 to n): ");
        read_input(&n);
        read_input(&k);
        combObjects = generate_combinations(n, k);
    } else {
        printf("Enter n (to generate all permutations of numbers from 1 to n): ");
        read_input(&n);
        combObjects = generate_permutations(n);
    }
    printf("Number of result rows = %d\n", combObjects->r);
    print_array2d(combObjects);
    free_array2d(combObjects);
    return 0;
}
