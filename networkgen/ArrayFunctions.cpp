//
//  ArrayFunctions.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "ArrayFunctions.h"

#define MAX 64 /* Max Stacksize quicksort  Could use a C++ stack */

/*
 * Bubble sort is slow, but its a stable sort and easy to understand
 */
void bubbleSortList(int **list, const size_t pos, const size_t l){
    
//    std::cout<< "\t Bubble Sorting from " << pos << " to " << l << std::endl;
    //List of Length 1 is always sorted
    if( pos >= l)
        return;

    bool sorted = false;
    int t1, t2; // temp values

    while (!sorted){
        sorted = true;
    
        for(size_t i = pos; i < l - 1; i++){
            if (list[1][i] > list[1][i+1]) {
                sorted = false;
                
                //swap values
                t1 = list[0][i];
                t2 = list[1][i];
                
                list[0][i] = list[0][i+1];
                list[1][i] = list[1][i+1];
                
                list[0][i+1] = t1;
                list[1][i+1] = t2;

            }
        }
    }
}

/*
 * Although this quicksort is not *that* good, it is more then fast enough
 * If we reach the 1 milion pores and more this is no good...
 * !! --- This version is not a stable sort --- !!
 */
void quicksort_iterative(int **array, size_t len) {

    std::cout << "Starting Sort" << std::endl;
    size_t left = 0, stack[MAX], pos = 0, seed = rand();
    for (; ;) {                                                 /* outer loop */
        for (; left + 1 < len; len++) {                         /* sort left to len-1 */
            if (pos == MAX) len = stack[pos = 0];               /* stack overflow, reset */
            int pivot = array[0][left + seed % (len - left)];   /* pick random pivot */
            seed = seed * 69069 + 1;                            /* next pseudorandom number */
            stack[pos++] = len;                                 /* sort right part later */
            for (size_t right = left - 1; ;) {                  /* inner loop: partitioning */
                while (array[0][++right] < pivot);              /* look for greater element */
                while (pivot < array[0][--len]);                /* look for smaller element */
                if (right >= len) break;                        /* partition point found? */

                int temp = array[0][right];
                int temp1 = array[1][right];
                array[0][right] = array[0][len];                  /* the only swap */
                array[1][right] = array[1][len];                  /* the only swap */
                array[0][len] = temp;
                array[1][len] = temp1;
            }                            /* partitioned, continue left part */
        }
        if (pos == 0) break;                               /* stack empty? */
        left = len;                             /* left to right is sorted */
        len = stack[--pos];                      /* get next range to sort */
    }

    std::cout << " ---Sorting secondlist " << std::endl;
    int temp = 0;
    for (size_t i = 0; i < len; i++) {

        if (array[0][i] != array[0][temp]) {
            bubbleSortList(array, temp, i - 1);
            temp = i;
        }
    }

    std::cout << "Done!" << std::endl;
}
