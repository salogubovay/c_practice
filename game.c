#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int MAX = 50;
long long *fib;

long long  find_nearest_fib(long long n) {
    int k = 1;

    while (fib[k] <= n) {
        k++;
    }

    return fib[k - 1];
 }
 
 long long next_turn(long long total, long long possible) {
     long long nearestFib = 0;
 
     while (total > 0) {
         nearestFib = find_nearest_fib(total);
         total -= nearestFib;
     }
    
     if (possible >= nearestFib) {
         return nearestFib;
     } else {
         return 1;
     }
}



void read_input(long long *pa, long long max) {
    int nitems;
    nitems = scanf("%lld", pa);
    if ((nitems != 1) || (*pa <= 0) || (*pa > max)) {
        printf("Wrong input!\n");
        abort();
    }
}

void create_fib_array(int size, int total) {
    long long currentValue = 0;
    fib  = (long long *) calloc(size, sizeof(long long));
    fib[1] = 1;

    for (int i = 2; (i < size) && (currentValue <= total); ++i) {
        currentValue = fib[i - 1] + fib[i - 2];
        fib[i] = currentValue; 
    }
}

int main() {
    long long total, possible, nextTurn, currentTurn, playerTurnOrder;
    bool isPlayersMove;
     
    printf("Hello! Please enter total number of matches (at most %d): ", INT_MAX);
    read_input(&total, INT_MAX);
    
    create_fib_array(MAX, total);

    possible = total - 1;

    printf("Choose your turn order (1 - first move yours, 2 - second move yours): ");

    read_input(&playerTurnOrder, 2);
    
    if (playerTurnOrder == 1) {
        isPlayersMove = true;
    } else {
        isPlayersMove = false;
    }

    while (total > 0) {
        if (isPlayersMove) {
            printf("Matches left = %lld. Choose number of matches to take. No more than %lld: ", total, possible);
            read_input(&currentTurn, possible);             
            isPlayersMove = false;
        } else {
            currentTurn = next_turn(total, possible);
            printf("Mathces left = %lld. AI took %lld matches.\n", total, currentTurn);
            isPlayersMove = true;
        }
        total -= currentTurn;
        possible = currentTurn * 2;  
    } 

    if (!isPlayersMove) {
        printf("You won!\n");
    } else {
        printf("You loose. Better luck next time!\n");
    }
    
    free(fib);   
    
    return 0; 
}
