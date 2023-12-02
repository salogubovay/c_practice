#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "array.h"

unsigned long long factorial(int n);

unsigned long long number_of_combinations(int n, int k);

unsigned long long num_of_arrangements(int n, int r);

//Knuth 4a 7.2.1.3 Algorithm T
//returns all k-combinations of numbers from 1 to n
struct array2d * generate_combinations(int n, int t);

// Knuth 4a 7.2.1.2 Algorithm P (Plain changes)
//return all permutations of numbers from 1 to n
struct array2d * generate_permutations(int n);

//return all k-arrangements of numbers from 1 to n
struct array2d * generate_arrangements(int n, int k);
