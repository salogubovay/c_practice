#include "combinatorics.h"

unsigned long long factorial(int n) {
    unsigned long long fact = 1;
    for (int i = 2; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

unsigned long long number_of_combinations(int n, int k) {
    unsigned long long combs = 1;
     
    if (n <= k) {
        printf("Total number of elements n = %d is less than the size of combination r = %d\n", n, k);
        abort();
    }

    for (int i = k + 1; i <= n; ++i) {
        combs *= i;
    }

    return combs / factorial(n - k);
}

unsigned long long num_of_arrangements(int n, int k) {
    if (n <= k) {
        printf("Total number of elements n = %d is less than size of arrangement k = %d\n", n, k);
    }
    return factorial(n) / factorial(n - k);
}

struct array2d * generate_arrangements(int n, int k) {
    struct array2d *arrans, *combs, *perms;
    int arransNum, count;
    arransNum = num_of_arrangements(n, k);
    arrans = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(arrans, arransNum, k);
    combs = generate_combinations(n, k);
    perms = generate_permutations(k);
    count = 0;
    for (int c = 0; c < combs->r; ++c) {
        for (int p = 0; p < perms->r; ++p) {
            for (int i = 0; i < k; ++i) {
                arrans->a[count][i] = combs->a[c][perms->a[p][i] - 1];
            }
            ++count;
        }
    }

    free_array2d(combs);
    free_array2d(perms);
    free(combs);
    free(perms);
    return arrans;
}

//Knuth 4a 7.2.1.3 Algorithm T
struct array2d * generate_combinations(int n, int t) {
    int *c, j, x, count;
    unsigned long long combNum;
    struct array2d * combs;

    if ((t < 0) || (t >= n)) {
        printf("Condition 0 < t < n is not met\n");
        abort();
    }
    
    combNum = number_of_combinations(n, t);
    count = 0;
    combs = (struct array2d *) calloc(1, sizeof(struct array2d));
    init_array2d(combs, combNum, t);
    c = (int *) calloc(t + 3, sizeof(int));    
    for (int i = 1; i <= t; ++i) {
        c[i] = i - 1;
    }
    c[t + 1] = n;
    c[t + 2] = 0;
    j = t;
    while (1) {
        copy_array_elements_modify(c, combs->a[count++], 1, 0, t, 1); 
        if (j > 0) {
            x = j;
            c[j] = x;
            --j;
        } else {
            if (c[1] + 1 < c[2]) {
                c[1] = c[1] + 1;
            } else {
                j = 2;

                while (1) {
                    c[j - 1] = j - 2;
                    x = c[j] + 1;
                    if (x != c[j + 1]) {
                        break;
                    } else {
                        ++j;
                    }
                }

                if (j > t) {
                    break;
                }
                c[j] = x;
                --j;
            }
        }
    }

    free(c);
    return combs;
}


// Knuth 4a 7.2.1.2 Algorithm P (Plain changes)
struct array2d * generate_permutations(int n) {
    struct array2d *perms;
    int count, permNum;
    int *a, *c, *o;
    bool end = false;
    a = (int *) calloc(n + 1, sizeof(int));
    c = (int *) calloc(n + 1, sizeof(int));
    o = (int *) calloc(n + 1, sizeof(int));
    perms = (struct array2d *) calloc(1, sizeof (struct array2d));
    permNum = factorial(n);
    init_array2d(perms, permNum, n);
    count = 0;
    
    for (int i = 1; i <= n; ++i) {
        a[i] = i;
        c[i] = 0;
        o[i] = 1;
    }
    while (!end) {
        int j, s;
        bool isSwapped = false;
        copy_array_elements(a, perms->a[count++], 1, 0, n);
        j = n;
        s = 0;
        while (!isSwapped) {
            int q, temp;
            q = c[j] + o[j];
            if (q < 0) {
                o[j] = -o[j];
                --j;
                continue;
            }
            
            if (q == j) {
                if (j == 1) {
                    end = true;
                    break;
                }
                s = s + 1;
                o[j] = -o[j];
                --j;
                continue;
            }
        
            temp = a[j - c[j] + s];
            a[j - c[j] + s] = a[j - q + s];
            a[j - q + s] = temp;
            c[j] = q; 
            isSwapped = true; 
        }
    }
    free(a);
    free(c);
    free(o);
    return perms;
}

