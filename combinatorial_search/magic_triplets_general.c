/*
Алгоритм формирования магических троек.

Заметим следующее:
    1) структуру троек можно разделить на 2 кольца: внутреннее и внешнее
    2) внутреннее кольцо состоит из n чисел, каждое из которых используется в двух тройках
    3) внешнее кольцо тоже состоит из n чисел, каждое из которых используется только в одной тройке
    4) структура троек нужно заполнить числами от 1 до 2 * n
    5) сумма во всех тройках должна быть одинаковая
    6) минимальная сумма в тройке >= 2 * n (максимальное число, которое можно использовать для заполнения)
    7) максимальная сумма в тройке <= 2 * n + 2 * n - 1 + 2 * n - 2 (сумма трёх самых больших чисел, которые можно использовать для заполнения)
    8) зная сумму, которой должны равняться все тройки, достаточно заполнить числа только во внутреннем кольце (число во внешнем кольце можно рассчитать как сумма тройки - сумма двух чисел внутреннего кольца)
    9) количество способов заполнения внутреннего кольца - это число размещений из 2n элементов по n элементов = (2n)! / (2n - n)!

Описание алгоритма:
    1) переберём все возможные суммы троек: от (2 * n) до (2 * n + 2 * n - 1 + 2 * n - 2)
    2) для каждой суммы последовательно будем заполнять внутреннее кольцо ранее неиспользованными числами
    3) для каждого заполненного элемента внутреннего кольца с индексом pos >= 1 будем рассчитывать число во внешнем кольце:
        а) если рассчитанное число во внешнем кольце < 1 или > 2 * n, то остановить заполнение и вернуться на одну позицию назад в заполнении внутреннего кольца
        б) если рассчитанное число во внешнем кольце > нулевого элемента внешнего кольца, то остановить заполнение (так как нам нужно вывести наборы троек в лексикографическом порядке) и вернуться на одну позицию назад в заполнении внутреннего кольца
    4) для каждой позиции (pos) заполнения внутреннего кольца поддерживается следующий инвариант:
        а) все числа, использованные в заполнении внутреннего и внешнего колец, различны
        б) суммы всех ранее заполненных троек одинаковые
    5) когда мы заполнили внутреннее кольцо (дошли до позиции pos = n) нам остаётся проверить последний элемент внешнего кольца, если он не использовался, то выводим получившийся набор троек и возвращаемся на одну позицию назад в заполнении внутреннего кольца.

Оценка сложности алгоритма полного перебора для заполнения внутреннего кольца:
    1) количество возможных сумм троек = 2 * n + 2 * n - 1 + 2 * n - 2 - 2 * n = 2 * (n + 1) - 3
    2) количество чисел для заполнения внутреннего кольца = 2 * n
    3) количество чисел во внутреннем кольце = n
    Приблизительная сложность алгоритма = O(n * (2 * n) ^ n)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "array.h"

static void read_input(int *pn) {
    int res = scanf("%d", pn);
    if ((res != 1) || (*pn < 3)) {
        printf("Incorrect input\n");
        abort();
    }
}

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

int main() {
    int maxSum, n; 
    struct array innerCircle, outerCircle, usedNumbers;

    printf("Enter number of triplets: ");
    read_input(&n);

    init_array(&innerCircle, n);
    init_array(&outerCircle, n);
    init_array(&usedNumbers, 2 * n + 1);

    maxSum = 2 * n + 2 * n - 1 + 2 * n - 2;

    for (int s = 2 * n; s <= maxSum; ++s) {
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

    return 0;
}
