#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 9
#define DIGITS 10
#define MAX_LINE 100

static FILE * open_file(char *fileName) {
    FILE *in_file = (FILE *) calloc(1, sizeof(FILE));
    in_file = fopen(fileName, "r");
    if (in_file == NULL) {
        perror("Error opening file");
        abort(); 
    }
    return in_file;
}

static bool solve(int iF[][SIZE], int fF[][SIZE], char usedDR[][DIGITS], char usedDC[][DIGITS], char usedDS[][DIGITS], int pos) {
    if (pos == (SIZE * SIZE)) {
        return true;
    }

    int row = pos / SIZE;
    int column = pos % SIZE;
    int square = (row / 3) * 3 + (column / 3);
    
    if (iF[row][column] != 0) {
        return solve(iF, fF, usedDR, usedDC, usedDS, pos + 1);
    }

    for (int d = 1; d <= 9; ++d) {
        if ((usedDR[row][d] == 0) && (usedDC[column][d] == 0) && (usedDS[square][d] == 0)) {
            usedDR[row][d] = 1;
            usedDC[column][d] = 1;
            usedDS[square][d] = 1;
            fF[row][column] = d;
            if (solve(iF, fF, usedDR, usedDC, usedDS, pos + 1)) {
                return true; 
            } else {
                usedDR[row][d] = 0;
                usedDC[column][d] = 0;
                usedDS[square][d] = 0;
                fF[row][column] = 0;
            }
        }
    }
    return false;
}

static void read_next_field(FILE * file, int iF[][SIZE], int fF[][SIZE], char uDR[][DIGITS], char uDC[][DIGITS], char uDS[][DIGITS]) {
    char line[MAX_LINE] = {0};
    fgets(line, MAX_LINE, file);
    for (int r = 0; r < SIZE; ++r) {
        fgets(line, MAX_LINE, file);
        for (int c = 0; c < SIZE; ++c) {
            int d = line[c] - '0';
            iF[r][c] = d;    
            fF[r][c] = d;
            if (iF[r][c] != 0) {
                uDR[r][d] = 1;
                uDC[c][d] = 1;
                uDS[(r / 3) * 3 + (c / 3)][d] = 1;
            }
        } 
    }
}

static void print_field(int field[][SIZE]) {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            printf("%d", field[r][c]);
            if ((c % 3) == 2) {
                printf(" ");
            }
        } 
        printf("\n");
    }
}

int main() {
    int initialField[SIZE][SIZE] = {0};
    FILE *file;
    int t, ans;
    ans = 0;
    t = 50;
    file = open_file("p096_sudoku.txt");

    for (int q = 1; q <= t; ++q) {
        int filledField[SIZE][SIZE] = {0};
        char usedDigitsRows[SIZE][DIGITS] = {0};
        char usedDigitsColumns[SIZE][DIGITS] = {0};
        char usedDigitsSquares[SIZE][DIGITS] = {0};

        read_next_field(file, initialField, filledField, usedDigitsRows, usedDigitsColumns, usedDigitsSquares);
        solve(initialField, filledField, usedDigitsRows, usedDigitsColumns, usedDigitsSquares, 0);

        ans += filledField[0][0] * 100 + filledField[0][1] * 10 + filledField[0][2];

        printf("Grind = %d\n", q);
        print_field(filledField);
    }

    printf("answer = %d\n", ans);
    fclose(file);
    return 0;
}
